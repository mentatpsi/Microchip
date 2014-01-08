/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        RCCGPU-IPU Demo.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
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
 * 07/27/10	    ...
 *****************************************************************************/
#include "MainDemo.h"

#if defined (__PIC24FJ256DA210__)
    _CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
    _CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
    _CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
#else
    #error "This Demo runs only on PIC24FJ256DA210 Development Board with the Parallel Flash"        
#endif	

/////////////////////////////////////////////////////////////////////////////
// Non-Volatile Memory Initialization Function 
/////////////////////////////////////////////////////////////////////////////
#if defined (USE_SST39LF400)
    // initialize Parallel Flash Memory for CPU access (CPU access on the parallel
    // flash is used only to read/write touch screen calibration data
    #define FlashInit(pInitData) SST39LF400Init((PARALLEL_INTERFACE_INIT_FUNC)&DriverInterfaceInit);                    
#endif

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL MACROS
/////////////////////////////////////////////////////////////////////////////
#define ICON_DISTANCE_START_TO_START (ICON_WIDTH + ICON_SPACING)

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void            NumberToString(WORD Value, XCHAR *pText, BYTE NoOfDigits, BYTE DotPos);
void            CreateError(char* string);

void            InitializeIcons(void);
void            InitializeSlidingScreen(void);
void            DetectGesture(GOL_MSG *pMsg, USER_GESTURE *pGest);
void            TouchScreenSlide(USER_GESTURE *pGest);
void            MomentumScreenSlide(USER_GESTURE *pGest);
void            DrawSelectedIcon(void);
void            DrawDeSelectedIcon(void);
void            TickInit(void);
void            ShowDummyMenu(void);
void            TouchPressIcon(GOL_MSG *pMsg);

void            DA210DEVBOARD_SST39LF400WriteWord(WORD data, DWORD address);
WORD            DA210DEVBOARD_SST39LF400ReadWord(DWORD address);
void            DA210DEVBOARD_SST39LF400SectorErase(DWORD address);
void            DA210DEVBOARD_SST39LF400ArrayWord(DWORD address, BYTE *pData, WORD nCount);
void            CheckExternalFlashHex();

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES AND FONTS USED
/////////////////////////////////////////////////////////////////////////////
// see IconData.c (h) and DemoFlashResource.c (h)

/////////////////////////////////////////////////////////////////////////////
//                            GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////
GOL_SCHEME      *menuScheme;                        // alternative style scheme
GOL_SCHEME      *altScheme;                         // alternative style scheme
GOL_SCHEME      *meterScheme;                       // style scheme used to draw the meter

ICON_MANAGER    iconMgr;

DWORD           VirtualAddressMin = GFX_DISPLAY_BUFFER_START_ADDRESS;
DWORD           VirtualAddressMax;
WORD            SwipeMagnitude;
WORD            EnterSubDemo;

GOL_MSG         msg;                            // GOL message structure to interact with GOL                    
USER_GESTURE    userAction;                     // user action structure to define user gestures

// current state of main demo state mashine
SCREEN_STATES   screenState = CREATE_MAINMENU;               

volatile DWORD  tick = 0, prevTick;             // tick counter,

DWORD  DispBuffAddrHome[ICON_MAX_COUNT+1];      // home address values for the display buffer to settle
                                                // this is used to adjust the screen to center an icon always
extern USB_HANDLE USBGenericInHandle;

const XCHAR ExitStr[]  = {'E','x','i','t',0};
const XCHAR MenuStr[]  = {'M','e','n','u',0};
const XCHAR HelpStr[]  = {'H','e','l','p',0};
const XCHAR UpStr[]    = {'U','p',0};
const XCHAR DownStr[]  = {'D','o','w','n',0};
const XCHAR ScaleStr[] = {'S','c','a','l','e',0};
const XCHAR MeterStr[] = {'R','P','M',0};

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                                MAIN
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
int main(void)
{
    
    	
     #if defined(PIC24FJ256DA210_DEV_BOARD)
    
    _ANSG8 = 0; /* S1 */
    _ANSE9 = 0; /* S2 */
    _ANSB5 = 0; /* S3 */
        
    #endif

    Delay10us(20);

    /////////////////////////////////////////////////////////////////////////////
	/* =============================================== */
	// Before Anything else, let's check if the 		
	// parallel flash is accessible 
	/* =============================================== */

	// before anything else let's check if the parallel flash is accessible 
	// check if we can read the Product ID ( note that this is specific to SST39LF400A or SST39VF400A)
	
	SST39LF400Init();	
	if (SST39LF400CheckID() == 0)
	{
	    InitGraph();                      	// initialize graphics library &
		CreateError("Parallel Flash is not accessible");
	}	

    CheckExternalFlashHex();

	/* =============================================== */

    HardwareButtonInit();           	// Initialize the hardware buttons

    GOLInit();                          // Initialize GOL

	SetFont((void*)&GOLFontDefault);	// use the default font
    menuScheme = GOLCreateScheme();
	altScheme =  GOLCreateScheme();
    meterScheme = GOLCreateScheme();

    InitAnimationStyleScheme(meterScheme);

    // set the default screen to icon number (range is 0 - (ICON_MAX_COUNT-1)
    iconMgr.IconSelected = 3;

    // initialize the timer that manages the tick counter
    TickInit();                         
    
    // initialize the components for Resistive Touch Screen  
    TouchInit(  (NVM_WRITE_FUNC)&DA210DEVBOARD_SST39LF400WriteWord,             \
                (NVM_READ_FUNC)&DA210DEVBOARD_SST39LF400ReadWord,               \
                (NVM_SECTORERASE_FUNC)&DA210DEVBOARD_SST39LF400SectorErase, 0);   	    
	
    // clear screen
    SetColor(SCREEN_BACKGROUND_COLOR);
    ClearDevice(); 


#ifdef USE_TRANSPARENT_COLOR
	TransparentColorEnable(ICON_TRANSPARENTCOLOR);
#endif	
    
    // initialize to default userAction 
    userAction.slideDir = SLIDE_TO_NOWHERE;
    userAction.swipeMagnitude = SWIPE_MAG_NONE;
    userAction.type = GEST_IDLE;
    userAction.touchMoveThreshold = TOUCH_SLIDE_THRESHOLD;
    
    EnterSubDemo = 0;

	while(1)
	{

   	    if(GOLDraw())		
    	{
        	TouchGetMsg(&msg);      // Get message from touch screen
	        GOLMsg(&msg);           // Process message
	    }	

    }   	
	
}

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                                END OF MAIN
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/************************************************************************************
* Function: InitializeIcons(void)
*
* Notes: Initialize the structure to manage the icons to be used
*
*************************************************************************************/
void InitializeIcons(void)
{

    WORD    i, j;
    SHORT   vLeft, vRight, vTop, vBottom;
    DWORD   dispBufAddr, addrDelta;

    iconMgr.IconCount = ICON_MAX_COUNT;
    
    iconMgr.pIcons[0] = (void*)&icon1;
    iconMgr.pIcons[1] = (void*)&icon2;
    iconMgr.pIcons[2] = (void*)&icon3;
    iconMgr.pIcons[3] = (void*)&icon4;
    iconMgr.pIcons[4] = (void*)&icon5;
    iconMgr.pIcons[5] = (void*)&icon6;
    
    iconMgr.pIconsLarge[0] = (void*)&icon1_large;
    iconMgr.pIconsLarge[1] = (void*)&icon2_large;
    iconMgr.pIconsLarge[2] = (void*)&icon3_large;
    iconMgr.pIconsLarge[3] = (void*)&icon4_large;
    iconMgr.pIconsLarge[4] = (void*)&icon5_large;
    iconMgr.pIconsLarge[5] = (void*)&icon6_large;

    addrDelta = ((GetMaxY() + 1) * (ICON_DISTANCE_START_TO_START));
    dispBufAddr = GFX_DISPLAY_BUFFER_START_ADDRESS + ((addrDelta<<1) * (ICON_MAX_COUNT-1));

    // Initialize the limit of the strip. This is is set here
    // so when the icon count is increased it will adjust automatically
    VirtualAddressMax = dispBufAddr;

    // initialize the first location of the first icon
    vLeft = ICON_SPACING>>1;
    vTop = CENTER_IN_Y - (ICON_HEIGHT >> 1);
    vRight = vLeft + ICON_WIDTH;
    vBottom = vTop + ICON_HEIGHT;

    for(j = 0, i = 0; j < ICON_MAX_COUNT + 1; j++)
    {
        // we have empty slots on both ends, hence the 
        // j count of ICON_MAX_COUNT + 2        
        if ((j < ICON_MAX_COUNT + 1) && (j > 0))
        {
            iconMgr.IconLeft[i] = vLeft;
            iconMgr.IconTop[i] = vTop;
            iconMgr.IconRight[i] = vRight;
            iconMgr.IconBottom[i] = vBottom;

            i++;
        }

        if (j < ICON_MAX_COUNT + 1)
            DispBuffAddrHome[j] = dispBufAddr;

        // adjust location
        vLeft += (ICON_DISTANCE_START_TO_START);
        vRight = (vLeft + ICON_WIDTH);

        dispBufAddr -= (addrDelta<<1);    

    }
    
}  

/************************************************************************************
* Function: InitializeSlidingScreen()
*
* Notes: Initialize the sliding strip with the icons
*
*************************************************************************************/
void InitializeSlidingScreen()
{

    WORD    i;    
    DWORD   dispBufAddr, addrDelta;
    
    // turn the display black to hide the creation of the strip
    G1CON2bits.DPTEST = 1;

    // create the long strip that will contain all the icons    
    // a +1 in the count is added since we want to have an empty space
    // at the two edges of the strip
    //     [  empty Icon1 Icon2 Icon3 ...  IconN empty ]
    
    // clear the strip first
    // initialize the icons in the virtual screen
    // by pre-rendering the icons on the screen

    dispBufAddr = GFX_DISPLAY_BUFFER_START_ADDRESS;
    addrDelta = ((GetMaxY() + 1) * (ICON_DISTANCE_START_TO_START));
    SetColor(SCREEN_BACKGROUND_COLOR);

    for(i = ICON_MAX_COUNT + 1; i > 0; i--)
    {
        GFX_SetDisplayArea(dispBufAddr);
        ClearDevice();
        dispBufAddr += (addrDelta<<1);    
    }

    addrDelta = ((GetMaxY() + 1) * (ICON_DISTANCE_START_TO_START));
    dispBufAddr = GFX_DISPLAY_BUFFER_START_ADDRESS + ((addrDelta<<1) * (ICON_MAX_COUNT-1));

    // adjust virtual location of the visible area (this shows which of the strip is currently
    // shown on the screen.
    iconMgr.currentStart = (ICON_SPACING + ICON_WIDTH + 1);

    for(i = 0; i < ICON_MAX_COUNT; i++)
    {
        GFX_SetDisplayArea(DispBuffAddrHome[i+1]);

        PutImage(   iconMgr.IconLeft[i] - iconMgr.currentStart,     \
                    iconMgr.IconTop[i],                             \
                    (void*)iconMgr.pIcons[i], 1);
        iconMgr.currentStart += (ICON_SPACING + ICON_WIDTH + 1);
    }
    
    // track the position of the viewable area
    iconMgr.currentStart = iconMgr.IconLeft[iconMgr.IconSelected] - ICON_DISTANCE_START_TO_START - ICON_SPACING;
    iconMgr.currentEnd = iconMgr.currentStart + GetMaxX() + 1;


    // set the display buffer to show that this icon is selectable and in the middle
    dispBufAddr = DispBuffAddrHome[iconMgr.IconSelected];
    GFX_SetDisplayArea(dispBufAddr);

    // Set the visible range. 
    /* The visible range is a subset of the virtual range.
       The virtual range starts from 0 (far left side) to max (far right side).
       The calculation is based on the number of icons.
       The last icon on the left to be located in ((GetMaxX()+1)/2) - (ICON_WIDTH/2)) 
       of the virtual range.
       If one icon is centered (located in the center of the screen), you will only see three icons.
       So the distance between icons will be determined by the equation:
            ICON_SPACING (distance between icons) = ((GetMaxX()+1) - (icon width * 3))/3
       
       Let's take an example:
       If there are 6 icons we will place the left most icon in location:
            ((ICON_SPACING/2) * 3) + ICON_WIDTH 
       The succeeding icons will be placed by incrementing the left-top position
       of the previous icon with (ICON_WIDTH + ICON_SPACING)     
       
       So if the icon selected in the initial screen is the 3rd icon from the right,
       the visible area of the virtual screen is:
            visible start = GetMaxX() - (ICON_SPACING + ICON_WIDTH)
            visible end = visible start + GetMaxX() + 1
       
    */ 
    iconMgr.visibleStart = iconMgr.IconLeft[iconMgr.IconSelected] - (((ICON_SPACING >> 1) * 3) + ICON_WIDTH);
    iconMgr.visibleEnd = iconMgr.visibleStart + GetMaxX();

    // turn the screen back on
    G1CON2bits.DPTEST = 0;
   
}


/************************************************************************************
* Function: TouchPressIcon(GOL_MSG *pMsg)
*
* Notes: Detects if there is a press on an icon.
*
*************************************************************************************/
void TouchPressIcon(GOL_MSG *pMsg)
{

    if
    (
        (CENTER_IN_X-(ICON_WIDTH>>1)                < pMsg->param1) &&
        (CENTER_IN_X+(ICON_WIDTH>>1)                > pMsg->param1) &&
        (iconMgr.IconTop[iconMgr.IconSelected]      < pMsg->param2) &&
        (iconMgr.IconBottom[iconMgr.IconSelected]   > pMsg->param2)
    )
    {     

        if (pMsg->uiEvent == EVENT_PRESS) 
        {
            // an icon is being pressed

            // add code here to show an icon is being pressed
            // this can be different actions. 
            // Example: 
            // - Turn the icon to gray scale image
            // - make the icon smaller
        
        }
        else //if (pMsg->uiEvent == EVENT_RELEASE))
        {
            // release the pressed icon open to another menu
            if (iconMgr.IconSelected < 3)
                EnterSubDemo = 1;
            else if (iconMgr.IconSelected >= 3)
                EnterSubDemo = 2;
            
        }
    }


}

/************************************************************************************
* Function: DetectGesture(GOL_MSG *pMsg, USER_GESTURE *pGest)
*
* Notes: Slide the strip using the touchscreen. It will track the x position of 
*        the touch and use the change in x to determine if it moves left or right.
*
*************************************************************************************/
void DetectGesture(GOL_MSG *pMsg, USER_GESTURE *pGest)
{                            
    BYTE prevEvent = EVENT_INVALID, currEvent; 
   
    
    // Note: if there are multiple user inputs that uses GOL messaging
    // enable this check here since this code is only using touch screen
    //if (pMsg->type != TYPE_TOUCHSCREEN)
    //    return;
        
    // get the current touch event
    currEvent = pMsg->uiEvent;
    
    // if the previous is a MOVE and the next message is an invalid message then effectively there 
    // was a release
    if ((pMsg->uiEvent == EVENT_INVALID) && (prevEvent == EVENT_MOVE))
        pMsg->uiEvent = EVENT_RELEASE;
    
    prevEvent = pMsg->uiEvent;

    // if this is the initial touch assign the current touch position to where the press occured
    if (pMsg->uiEvent == EVENT_PRESS)
    {
        pGest->currPos = pMsg->param1;
        pGest->prevPos = pMsg->param1;
        pGest->type = GEST_PRESS;
        
        pGest->touchMoveThreshold = TOUCH_SLIDE_THRESHOLD;
        return;
    }        
    else if (pMsg->uiEvent == EVENT_MOVE)
    {
        // if the event is move track it
        // check if the change is greater than touchMoveThreshold (in pixels)
        pGest->delta = pMsg->param1 - pGest->currPos;
        if (abs(pGest->delta) <= (pGest->touchMoveThreshold))
            return;
        else
        {
            // now allow the touch to be easily tracked by changing the threshold
            pGest->touchMoveThreshold = 1;
        }    
        
        // set the direction of movement
        if (pGest->delta > 0)
            pGest->slideDir = SLIDE_TO_RIGHT;
        else
            pGest->slideDir = SLIDE_TO_LEFT;

        pGest->type = GEST_MOVE;

        // update the touch positions
        pGest->prevPos = pGest->currPos; 
        pGest->currPos = pMsg->param1;
    }
    else if (pMsg->uiEvent == EVENT_RELEASE)
    {
        pGest->touchMoveThreshold = TOUCH_SLIDE_THRESHOLD;
        
        // check if the action was a swipe
        if (pGest->swipeMagnitude > SWIPE_MAG_NONE)
        {
            pGest->type = GEST_SWIPE;
        }    
        else    
        {
            pGest->type = GEST_RELEASE;
            pGest->swipeMagnitude = SWIPE_MAG_NONE;

        }    
        pGest->touchMoveThreshold = TOUCH_SLIDE_THRESHOLD;

        pGest->prevPos = -1; 
        pGest->currPos = -1;

    }
    else
    {
        // catch all
        pGest->type = GEST_IDLE;
        pGest->touchMoveThreshold = TOUCH_SLIDE_THRESHOLD;
        
    }    
    prevTick = tick; 

    
}    

/************************************************************************************
* Function: TouchScreenSlide(USER_GESTURE *pGest)
*
* Notes: Slide the strip using the touchscreen. It will track the x position of 
*        the touch and use the change in x to determine if it moves left or right.
*
*************************************************************************************/
void TouchScreenSlide(USER_GESTURE *pGest)
{
    DWORD   dispBufAddr = GFX_GetDisplayArea();         // this is the dynamic display buffer address
    LONG    slideDelta = (GetMaxY() + 1) << 1;          // this delta is the address change

    SHORT   adjustVal = 1;

    
    if (pGest->delta < 0)
    {
        adjustVal = 1;
        slideDelta *= -1;
    }    
    else if (pGest->delta > 0)
    {
        adjustVal = -1;    
    }    
    else
    {
        // do nothing since we do not need to slide
        return;
    }
    
    // Track the touch routine
    // if the event is move track it
    // check if the change is greater than touchMoveThreshold (in pixels)
    

   	// check if it is beyond the limit or we do not need to move
   	if (((dispBufAddr <= VirtualAddressMin) && (adjustVal > 0)) || 
   	    ((dispBufAddr >= VirtualAddressMax) && (adjustVal < 0)) || 
   	     (pGest->delta == 0))
	    return;

    if (iconMgr.IconSelected >= 0)
        DrawDeSelectedIcon();

   	while(1)
	{
       	// check if it is beyond the limit or we do not need to move
       	if (((dispBufAddr <= VirtualAddressMin) && (adjustVal > 0)) || 
       	    ((dispBufAddr >= VirtualAddressMax) && (adjustVal < 0)) || 
       	     (pGest->delta == 0))
    	    break;
    	else
    	{    
    	    pGest->delta += adjustVal;
  	        dispBufAddr += slideDelta; 
  	        
  	        // should only adjust when needed
            iconMgr.visibleStart += adjustVal;
            iconMgr.visibleEnd += adjustVal;
    	}    

        // wait for start of frame so we do not update in the middle of the screen
        while(!_HMRGN);
        GFX_SetDisplayArea(dispBufAddr);
    }   
  
}    

/************************************************************************************
* Function: MomentumScreenSlide(USER_GESTURE *pGest)
*
* Notes: Slide the strip to one of the home display buffer addresses so an icon is
*        always centered.
*
*************************************************************************************/
void MomentumScreenSlide(USER_GESTURE *pGest)
{
    LONG    slideDelta = ((GetMaxY() + 1) * SLIDE_DELTA)<<1;    // this delta is the address change
    DWORD   slideAddressMomentum = 0, slideAddressLimit = 0;
    SHORT   adjustVal = 1;
    int     i;

    // find where the screen is located 
    for(i=0;i<ICON_MAX_COUNT+1;i++)
    {
        // must cover all the reqions
        /*
            The edges:
            - when at the lowest address location
            - when at the highest address location
        */
        if (DispBuffAddrHome[i] == GFX_GetDisplayArea())  
        {
            // we do not need to adjust
            pGest->slideDir = SLIDE_TO_NOWHERE;
            iconMgr.IconSelected = i;
            DrawSelectedIcon();
            return;
        }    
        else if ((DispBuffAddrHome[i+1] < GFX_GetDisplayArea())  && (GFX_GetDisplayArea() < DispBuffAddrHome[i]))
        {
            // found where the screen is located
            slideAddressMomentum = GFX_GetDisplayArea();

            // get the limit of momentum adjust
            if (pGest->slideDir == SLIDE_TO_RIGHT)
            {
                if (i == 0)
                {
                    slideAddressLimit = VirtualAddressMax;
                }
                else
                {    
                    slideAddressLimit = DispBuffAddrHome[i];
                }
                iconMgr.IconSelected = i;
                adjustVal = -SLIDE_DELTA;

            }    
            else if (pGest->slideDir == SLIDE_TO_LEFT)
            {
                if ((i + 1) >= ICON_MAX_COUNT)
                {
                    slideAddressLimit = VirtualAddressMin;
                    iconMgr.IconSelected = ICON_MAX_COUNT-1;
                }
                else    
                {
                    slideAddressLimit = DispBuffAddrHome[i+1];
                    iconMgr.IconSelected = i+1;
                }
                
                slideDelta *= -1;

            }    
            else 
            {   
                slideAddressLimit = GFX_GetDisplayArea();
            }

            // since we already found the location we can break out of the for loop
            break;    
        }    
    }

    // now slide the screen to center the closest icon in the direction of drag
    while(1)
    {
        // wait for start of frame so we do not update in the middle of the screen
        while(!_HMRGN);
        

        if (((pGest->slideDir == SLIDE_TO_LEFT) && (slideAddressMomentum <= slideAddressLimit))  || 
            ((pGest->slideDir == SLIDE_TO_RIGHT) && (slideAddressMomentum >= slideAddressLimit)) 
           )
            break;
                
        // change the display buffer address
        GFX_SetDisplayArea(slideAddressMomentum);
        
        // adjust the address to the next position
        slideAddressMomentum += slideDelta;
        
        // catch if the address is just short of the limit
        // this is needed so we synchronize with the visibleStart and visibleEnd variables
        if (slideAddressMomentum > VirtualAddressMax)
            slideAddressMomentum = VirtualAddressMax;
        if (slideAddressMomentum < VirtualAddressMin)
            slideAddressMomentum = VirtualAddressMin;
            
        
        // should only adjust when needed
        iconMgr.visibleStart += adjustVal;
        iconMgr.visibleEnd += adjustVal;
            
        DelayMs(2);    
        //Delay10us(50);    
    }    
    
    DrawSelectedIcon();

    prevTick = tick; 
    
}    


void DrawSelectedIcon(void)
{
    
    // draw the selected icon 
    PutImage(CENTER_IN_X-(ICON_LARGE_WIDTH>>1), CENTER_IN_Y-(ICON_LARGE_HEIGHT>>1), (void*)iconMgr.pIconsLarge[iconMgr.IconSelected], 1); 
    
  	// make sure the GPUs are done before exiting
    GFX_WaitForCommandQueue(16);
    GFX_WaitForGpu();


}    

void DrawDeSelectedIcon(void)
{
    // redraw the selected icon to its smaller version
    SetColor(SCREEN_BACKGROUND_COLOR);
    Bar(CENTER_IN_X-(ICON_LARGE_WIDTH>>1), CENTER_IN_Y-(ICON_LARGE_HEIGHT>>1), CENTER_IN_X+(ICON_LARGE_WIDTH>>1), CENTER_IN_Y+(ICON_LARGE_HEIGHT>>1));

  	// make sure the GPUs are done before rendering the image
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

    PutImage(CENTER_IN_X-(ICON_WIDTH>>1), CENTER_IN_Y-(ICON_HEIGHT>>1), (void*)iconMgr.pIcons[iconMgr.IconSelected], 1); 

    iconMgr.IconSelected = -1;
}    


/************************************************************************************
* Function: ShowDummyMenu(void) 
*
* Notes: Will show a dummy menu when selecting an icon
*
*************************************************************************************/
void ShowDummyMenu(void)
{
    XCHAR msg1[] = "Menu Selection";
    XCHAR msg2[] = "Press S2 to go back to";
    XCHAR msg3[] = "Main Menu Selection";
    
    WORD    textWidth, textHeight;
    SHORT   TextStartLocation;
    
    GFX_SetDisplayArea(GFX_DISPLAY_BUFFER_START_ADDRESS);

   	SetColor(BRIGHTBLUE);
   	
   	textHeight = GetTextHeight((void *) &FONTDEFAULT);
   	
   	textWidth = GetTextWidth(msg1, (void *) &FONTDEFAULT);
   	TextStartLocation = ((GetMaxY() - textHeight) >> 1) - textHeight;

   	OutTextXY((GetMaxX() - textWidth) >> 1, TextStartLocation, msg1);

   	textWidth = GetTextWidth(msg2, (void *) &FONTDEFAULT);
   	TextStartLocation += textHeight;
   	
   	OutTextXY((GetMaxX() - textWidth) >> 1, TextStartLocation, msg2);

   	textWidth = GetTextWidth(msg3, (void *) &FONTDEFAULT);
   	TextStartLocation += textHeight;
   	
   	OutTextXY((GetMaxX() - textWidth) >> 1, TextStartLocation, msg3);
    
}    

/************************************************************************************
* Function: NumberToString(WORD Value,XCHAR *pText, BYTE NoOfDigits, BYTE DotPos )
*
* Notes: convert the number to string
*
*************************************************************************************/
void NumberToString(WORD Value, XCHAR *pText, BYTE NoOfDigits, BYTE DotPos)
{
    BYTE    i;
    BYTE    bIndex;

    for(bIndex = 0; bIndex < NoOfDigits; bIndex++)
    {
        pText[NoOfDigits - bIndex - 1] = '0' + (Value % 10);
        Value /= 10;
    }

    if(DotPos != 0 && DotPos <= NoOfDigits)
    {
        for(i = 0; i < DotPos; i++)
        {
            pText[NoOfDigits - i] = pText[NoOfDigits - 1 - i];
        }

        pText[NoOfDigits - DotPos] = '.';
        pText[NoOfDigits + 1] = '\0';
    }

    //If dot position is 0 or greater than number of digits, then don't put dot in the display
    else
    {
        pText[NoOfDigits] = '\0';
    }
}


void CreateMainMenu()
{
    // create the main menu

	InitializeIcons();
    InitializeSlidingScreen();
    DrawSelectedIcon();
    
}


void ProcessMainMenuMsgs()
{

    /* ============================================================ */ 
    // This are the Gesture detection and screen action routines
    /* ============================================================ */ 

    /* ************************************************************ */
  	// Detect the user gesture 
    /* ************************************************************ */
   	DetectGesture(&msg, &userAction);
   	
    if ((msg.uiEvent == EVENT_PRESS) || (msg.uiEvent == EVENT_RELEASE))
	{
        /* ************************************************************ */
    	// Detect if there is a press on an icon
        /* ************************************************************ */
        TouchPressIcon(&msg);
    }   	


    /* ************************************************************ */
	// Detect if there is a drag on the screen
    /* ************************************************************ */
	if (userAction.type == GEST_MOVE)
	{
    	// move the icons to the left 
        TouchScreenSlide(&userAction);
    }   	

    /* ************************************************************ */
    // Gesture is a swipe or user is not touching the screen anymore 
    // Move the icons to a settled positions
    /* ************************************************************ */
	if (userAction.type == GEST_RELEASE) 
	{
        MomentumScreenSlide(&userAction);
    }          

    /* ============================================================ */ 
    // End of Gesture detection and screen action routines
    /* ============================================================ */ 

}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed by default
// Overview: it's a user defined function. GOLMsg() function calls it each
//           time the valid message for the object received
/////////////////////////////////////////////////////////////////////////////
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{

//   WORD objID;

    // process messages for demo screens
    switch(screenState)
    {
        case DISPLAY_MAINMENU:

            break;

        case DISPLAY_FISHFINDER:
            return (FishFinderMsgCallback(objMsg, pObj, pMsg));

        case DISPLAY_ANIMATION:
            return (AnimationMsgCallback(objMsg, pObj, pMsg));
        
        default:
            break;
    }        

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLDrawCallback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: it's a user defined function. GOLDraw() function calls it each
//           time when GOL objects drawing is completed. User drawing should be done here.
//           GOL will not change color, line type and clipping region settings while
//           this function returns zero.
/////////////////////////////////////////////////////////////////////////////
WORD GOLDrawCallback(void)
{
   
   switch(screenState)
    {
        case CREATE_MAINMENU:
            
            GOLFree();
            CreateMainMenu();
            screenState = DISPLAY_MAINMENU;         // switch to next state
            
            break;
            
        case DISPLAY_MAINMENU:

            ProcessMainMenuMsgs();

            if (EnterSubDemo == 1)
            {
                GFX_SetDisplayArea(GFX_DISPLAY_BUFFER_START_ADDRESS);
                screenState = CREATE_FISHFINDER; 
                EnterSubDemo = 0;
            } 
            else if (EnterSubDemo == 2)
            {
                GFX_SetDisplayArea(GFX_DISPLAY_BUFFER_START_ADDRESS);
                screenState = CREATE_ANIMATION; 
                EnterSubDemo = 0;
            }

            break;
            
        case CREATE_FISHFINDER:

            CreateFishFinderDemo();
            screenState = DISPLAY_FISHFINDER;       // switch to next state
            break;
            
        case DISPLAY_FISHFINDER:

            FishFinderDrawCallback();
            break;

        case CREATE_ANIMATION:
            
            CreateAnimationDemo();
            screenState = DISPLAY_ANIMATION;        // switch to next state
            break;
            
        case DISPLAY_ANIMATION:
            NextAnimation(tick);
            break;
    
        default: 
            break;    
    }    
    return (1);

}    


/************************************************************************
 Function: void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, 
 								  XCHAR *pTextC, XCHAR *pTextD)

 Overview: Creates the navigation and control buttons of the menu.
 
 Input: pTextA - pointer to button A label.
		pTextB - pointer to button B label.
		pTextC - pointer to button C label.
		pTextB - pointer to button D label.

 Output: none
************************************************************************/
void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, XCHAR *pTextC, XCHAR *pTextD)
{
    WORD    state;

    state = BTN_DRAW | BTN_NOPANEL;
    if(pTextA == NULL)
        state = BTN_DISABLED | BTN_DRAW | BTN_NOPANEL;
    BtnCreate
    (
        ID_BUTTON_A,
        CtrlBtnLeft(0),
        CtrlBtnTop(),
        CtrlBtnRight(0),
        CtrlBtnBottom(),
        0,
        state,
        (void *) &MenuButton,
        pTextA,
        menuScheme
    );

    state = BTN_DRAW | BTN_NOPANEL;
    if(pTextB == NULL)
        state = BTN_DISABLED | BTN_DRAW | BTN_NOPANEL;
    BtnCreate
    (
        ID_BUTTON_B,
        CtrlBtnLeft(1),
        CtrlBtnTop(),
        CtrlBtnRight(1),
        CtrlBtnBottom(),
        0,
        state,
        (void *) &MenuButton, 
        pTextB,
        menuScheme
    );

    state = BTN_DRAW | BTN_NOPANEL;
    if(pTextC == NULL)
        state = BTN_DISABLED | BTN_DRAW | BTN_NOPANEL;
  	BtnCreate
    (
        ID_BUTTON_C,
        CtrlBtnLeft(2),
        CtrlBtnTop(),
        CtrlBtnRight(2),
        CtrlBtnBottom(),
        0,
        state,
        (void *) &MenuButton, 
        pTextC,
        menuScheme
    );

    state = BTN_DRAW | BTN_NOPANEL;
    if(pTextD == NULL)
        state = BTN_DISABLED | BTN_DRAW | BTN_NOPANEL;
    BtnCreate
    (
        ID_BUTTON_D,
        CtrlBtnLeft(3),
        CtrlBtnTop(),
        CtrlBtnRight(3),
        CtrlBtnBottom(),
        0,
        state,
        (void *) &MenuButton, 
        pTextD,
        menuScheme
    );

}


// dummy function
WORD ExternalMemoryCallback(GFX_EXTDATA *memory, LONG offset, WORD nCount, void *buffer)
{
	// dummy function
    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: Timer3 ISR
// Input: none
// Output: none
// Overview: increments tick counter. Tick is approx. 1 ms.
/////////////////////////////////////////////////////////////////////////////
#ifdef __PIC32MX__
    #define __T3_ISR    __ISR(_TIMER_3_VECTOR, ipl4)
#else
    #define __T3_ISR    __attribute__((interrupt, shadow, auto_psv))
#endif

/* */
void __T3_ISR _T3Interrupt(void)
{
    tick++;

    // Clear flag
    #ifdef __PIC32MX__
    mT3ClearIntFlag();
    #else
    IFS0bits.T3IF = 0;
    #endif
    
    TouchDetectPosition();
}

/////////////////////////////////////////////////////////////////////////////
// Function: void TickInit(void)
// Input: none
// Output: none
// Overview: Initilizes the tick timer.
/////////////////////////////////////////////////////////////////////////////

/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD       500 // us
#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000

/* */
void TickInit(void)
{
    tick = 0;
    prevTick = tick;
    
    // Initialize Timer4
    #ifdef __PIC32MX__
    OpenTimer3(T3_ON | T3_PS_1_8, TICK_PERIOD);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_4);
    #else
    TMR3 = 0;
    PR3 = TICK_PERIOD;
    IFS0bits.T3IF = 0;  //Clear flag
    IEC0bits.T3IE = 1;  //Enable interrupt
    T3CONbits.TON = 1;  //Run timer
    #endif
    
}

/************************************************************************************
* Function: DA210DEVBOARD_SST39LF400SectorErase(DWORD address)
*
* Notes: Erase the sector specified by the address in the parallel flash.
*        The graphics module is locked out during this function but
*        since this function occurs fast enough and only called when 
*        calibrating the touch screen, it will not be noticeable.
*
*************************************************************************************/
void DA210DEVBOARD_SST39LF400SectorErase(DWORD address)
{
    // this function locks out the graphics module from accessing
    // external memory until the MSTSEL is reset back to Graphics

    // to minimize effects on the screen refresh 
    // wait until the VSYNC starts
    while(!_VMRGN); 

  	PMCON2bits.MSTSEL = 0;		// set CPU as Master
    SST39LF400SectorErase(address);
  	PMCON2bits.MSTSEL = 3;		// set Graphics as Master

}    

/************************************************************************************
* Function: DA210DEVBOARD_SST39LF400WriteWord(WORD data, DWORD address)
*
* Notes: Write a word to parallel flash with the given address.
*        The graphics module is locked out during this function but
*        since this function occurs fast enough and only called when 
*        calibrating the touch screen, it will not be noticeable.
*
*************************************************************************************/
void DA210DEVBOARD_SST39LF400WriteWord(WORD data, DWORD address)
{
    // this function locks out the graphics module from accessing
    // external memory until the MSTSEL is reset back to Graphics

    // to minimize effects on the screen refresh 
    // wait until the VSYNC starts
    while(!_VMRGN); 

  	PMCON2bits.MSTSEL = 0;		// set CPU as Master
    SST39LF400WriteWord(data, address);
  	PMCON2bits.MSTSEL = 3;		// set Graphics as Master
    
}
 
/************************************************************************************
* Function: WORD DA210DEVBOARD_SST39LF400ReadWord(DWORD address)
*
* Notes: Reads the data from the parallel flash located in the given address.  
*        The graphics module is locked out during this function but
*        since this function occurs fast enough and only called when 
*        calibrating the touch screen, it will not be noticeable.
*
*************************************************************************************/
WORD DA210DEVBOARD_SST39LF400ReadWord(DWORD address)
{
    // this function locks out the graphics module from accessing
    // external memory until the MSTSEL is reset back to Graphics

    WORD readWord;

    // to minimize effects on the screen refresh 
    // wait until the VSYNC starts
    while(!_VMRGN); 

  	PMCON2bits.MSTSEL = 0;		// set CPU as Master
    readWord = SST39LF400ReadWord(address);
  	PMCON2bits.MSTSEL = 3;		// set Graphics as Master
    
    return readWord;

}    

/************************************************************************************
* Function: void DA210DEVBOARD_SST39LF400ArrayWord(DWORD address, BYTE *pData, WORD nCount)
*
* Notes: Reads the array of data from the parallel flash located in the given address.  
*        The graphics module is locked out during this function but
*        since this function occurs fast enough and only called when 
*        calibrating the touch screen, it will not be noticeable.
*
*************************************************************************************/
void DA210DEVBOARD_SST39LF400ArrayWord(DWORD address, BYTE *pData, WORD nCount)
{
    // this function locks out the graphics module from accessing
    // external memory until the MSTSEL is reset back to Graphics

    // to minimize effects on the screen refresh 
    // wait until the VSYNC starts
    while(!_VMRGN); 

  	PMCON2bits.MSTSEL = 0;		// set CPU as Master
    SST39LF400ReadArray(address, pData, nCount);
  	PMCON2bits.MSTSEL = 3;		// set Graphics as Master
    
}    

/*********************************************************************
* Function:  void CheckExternalFlashHex(void)
*
* Overview: Checks if the data in external flash is correct.
*
* PreCondition:  Graphics Library must be initialized.
*
* Input: none
*
* Output: None
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
void CheckExternalFlashHex()
{
    typedef struct 
    {
        UINT32 mchpSignature;
        UINT32 mchpCRCData;
    } CRC_CHECK; 

    CRC_CHECK externalCRC, expectedCRC;   
    WORD textHeight;
    void *pFont;
    XCHAR *pStr = NULL;
    BOOL  setProgram = FALSE;

    XCHAR ProgrammingModeString[] = {'P','r','o','g','r','a','m','m','i','n','g',' ','M','o','d','e',0}; 
    XCHAR InvalidExternalString[] = {'E','x','t','e','r','n','a','l',' ','d','a','t','a',' ','i','n','v','a','l','i','d','.',0};
    XCHAR PressToContinueString1[] = {'P','r','e','s','s',' ','S','2',' ','t','h','e','n',0};
    XCHAR PressToContinueString2[] = {'s','e','n','d',' ','d','a','t','a',' ','u','s','i','n','g',0};
    XCHAR ExternalMemoryProgString1[] = {'"','E','x','t','e','r','n','a','l',' ','M','e','m','o','r','y',0};
    XCHAR ExternalMemoryProgString2[] = {'P','r','o','g','r','a','m','m','e','r','"',' ','u','t','i','l','i','t','y',0};
#ifdef USE_COMM_PKT_MEDIA_USB
    XCHAR ViaString[] = {'v','i','a',' ','U','S','B','.','.','.',0};
#else
    XCHAR ViaString[] =  {'v','i','a',' ','U','A','R','T','.','.','.',0};
#endif

    InitGraph();

    pFont = (void*) &GOLFontDefault;
    SetFont(pFont);
    textHeight = GetTextHeight(pFont);

    // check if the CRC matches the data stored in the external flash memory
    expectedCRC.mchpCRCData = GRC_CRC32_EXTERNAL_MARKER;
    expectedCRC.mchpSignature = 0x5048434D;                // this is "MCHP"

    // check if programming is prompted     
    if(GetHWButtonProgram() == HW_BUTTON_PRESS)
    {
        pStr = (XCHAR*) ProgrammingModeString;
        setProgram = TRUE;
	} 

    if (setProgram == FALSE)
    {
        DA210DEVBOARD_SST39LF400ArrayWord(GRC_CRC32_EXTERNAL_ADDR, (BYTE *)&externalCRC, 8);
        if  ((expectedCRC.mchpCRCData != externalCRC.mchpCRCData) || \
             (expectedCRC.mchpSignature != externalCRC.mchpSignature))
        {
            // expected and read CRC does not match, proceed to programming flash first
            // run the flash programming 
            pStr = (XCHAR*) InvalidExternalString;
            setProgram = TRUE;
        }
    }

    if (setProgram == TRUE)
    {
        // expected and read CRC does not match, proceed to programming flash first
        // run the flash programming 
        SetColor(BLACK);
        ClearDevice();
        SetColor(WHITE);
        OutTextXY(10,(textHeight)  , pStr);
        OutTextXY(10,(textHeight*3), PressToContinueString1);
        OutTextXY(10,(textHeight*4), PressToContinueString2);
        OutTextXY(10,(textHeight*5), ExternalMemoryProgString1);
        OutTextXY(10,(textHeight*6), ExternalMemoryProgString2);
        OutTextXY(10,(textHeight*7), ViaString);

        // wait for user to be ready      		
        while(GetHWButtonProgram() == HW_BUTTON_PRESS);
        DelayMs(500);

        // wait for user to be ready      		
        while(GetHWButtonProgram() == HW_BUTTON_RELEASE);
        DelayMs(500);

     	SST39LF400Init();	

        // Call the external flash programming routine
        ProgramFlash();

        // check if USB has sent reply to host then do a detach
        while(!USBHandleBusy(USBGenericInHandle));
        USBDeviceDetach();

        DelayMs(100);

        // Force Reset to force the checking of the flash memory if programming was a success
        Reset();
    }
}	

//  Create Error Flash (Blue Screen)
void CreateError(char* string)
{
	// Blue Screen Error
	SetColor(119);
	ClearDevice();
	SetColor(-1);
	
	// Flash Error Message
	SetFont((void*)&GOLFontDefault);
	if(string == NULL)
		{while(!OutTextXY(0, 0, "Runtime Error."));}
	else{while(!OutTextXY(0,0, string));}
	while(1);
}


