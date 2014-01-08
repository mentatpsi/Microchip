/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        ElevatorDemo.c
 * Dependencies:    ElevatorDemo.h
 * Processor:       PIC24F
 * Compiler:       	MPLAB C30 V3.00
 * Linker:          MPLAB LINK30 
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/07/11	   ...
 *****************************************************************************/
#include "ElevatorDemo.h"

// Configuration bits
	#if defined (__PIC24FJ256DA210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif	

/////////////////////////////////////////////////////////////////////////////
// Non-Volatile Memory Component
/////////////////////////////////////////////////////////////////////////////
#if defined (USE_SST39LF400)

    // initialize Parallel Flash Memory for CPU access (CPU access on the parallel
    // flash is used only to read/write touch screen calibration data
    #define ReadExternalMemArray(address, pdata, len) DA210DEVBOARD_SST39LF400ArrayWord(address, pdata, len)
    #define InitExternalMem(pInitData)  SST39LF400Init();

#elif defined (USE_SST25VF016)

    // SPI Memory Settings
    const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
    #define ReadExternalMemArray(address, pdata, len) ReadArray(address, pdata, len)
    #define InitExternalMem(pInitData) FLASHInit((DRV_SPI_INIT_DATA *)pInitData)             
#endif    


// macro top refresh the background for the animated area
    #define GraphicsMemCopyCont2Disc(                                   \
                            SourceBuffer, DestinationBuffer,            \
                            SourceOffset, DestinationOffset,            \
                            width, height)                              \
                                                                        \
            ROPBlock(   SourceBuffer, DestinationBuffer,                \
                        SourceOffset, DestinationOffset,                \
                        GFX_DATA_CONTINUOUS, GFX_DATA_DISCONTINUOUS,    \
                        RCC_COPY, RCC_ROP_C, 0,                         \
                        width, height);

    #define GraphicsMemCopyDisc2Cont(                                   \
                            SourceBuffer, DestinationBuffer,            \
                            SourceOffset, DestinationOffset,            \
                            width, height)                              \
                                                                        \
            ROPBlock(   SourceBuffer, DestinationBuffer,                \
                        SourceOffset, DestinationOffset,                \
                        GFX_DATA_DISCONTINUOUS, GFX_DATA_CONTINUOUS,    \
                        RCC_COPY, RCC_ROP_C, 0,                         \
                        width, height);


/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void            TickInit(void);
void            ShowElevatorCarLocation(WORD floor, void *pFont, DWORD offset); 
void            InitializeBoard(void);
void            CheckExternalMemory(void);
void            CheckExternalFlashHex();
void            InitializeElevatorAnimation(void);
void            UpdateElevatorAnimation(void);
void            ShowElevatorMessage(XCHAR *pString);

void            DA210DEVBOARD_SST39LF400WriteWord(WORD data, DWORD address);
WORD            DA210DEVBOARD_SST39LF400ReadWord(DWORD address);
void            DA210DEVBOARD_SST39LF400SectorErase(DWORD address);
void            DA210DEVBOARD_SST39LF400ArrayWord(DWORD address, BYTE *pData, WORD nCount);

void 			NumberToString(WORD Value, XCHAR *pText, BYTE NoOfDigits, BYTE DotPos);
void 			CreateError(XCHAR* pText, void *pFont);

volatile DWORD  tick = 0, MsgAnimateTick, AnimateTick, FloorTick;             // tick counter,

volatile ELEVATOR_ANIMATION  ElevatorStruct; 
extern   USB_HANDLE USBGenericInHandle;

/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////

/* */
int main(void)
{
    void *pUsedFont;

    // initialize the board components and the Graphics Library
    InitializeBoard();              

    // hide the screen by turning everything to BLACK
#ifndef DEBUG_DEMO_MODE
    _DPTEST = 1;
#endif

    /* =============================================== */
    // This is the start of the Elevator Demo Code    
    /* =============================================== */
    /*
        This demo is configured to use two buffers.
        1. Main Buffer - the buffer used to display pixels on the screen
        2. Hidden Buffer - the buffer used to pre-render images on the screen.
                           These images are used to do animation and refresh portion
                           of the background that is overwritten by the animation.

        Here are the steps in the demo from initialization of the buffer 
        to actual operation
        1.	Allocate two buffers in RAM 
            a.	Main buffer will be the one displayed on the screen
            b.	Hidden buffer will be the one where we pre-render animated images
        2.	Render the two composite arrow images on the main buffer
        3.	Copy these images into the hidden buffer 
        4.	Render the background to the main buffer
        5.	Copy the area of the background where the animation will occur to the hidden buffer. There will be two main areas:
            a.	The area where the floor numbers are shown
            b.	The area where the arrows are animated
        6.	To animate copy a portion of the selected image to animate. The copying (or refreshing)
            of the animation is done by incrementing the copied area from the composite image.
            The repeating nature of the copy will result in the animation effect.
      
    */
    // set the font to use
    pUsedFont = (void*)&ELEVATOR_FONT;
    SetFont(pUsedFont);

    InitializeElevatorAnimation();

    ShowElevatorCarLocation(ElevatorStruct.floorLocation, pUsedFont, ElevatorStruct.floorStatusXStart); 
    
    // show the screen
    _DPTEST = 0;
    
    while(1)
    {

       	/* =============================================== */
        // This portion detects the direction of the car
       	/* =============================================== */
        // check direction of the elevator car
        if (GetHWButtonCenter() == HW_BUTTON_PRESS)
        {
            // stop the elevator car
            ElevatorStruct.carDir = CAR_STOPPED;
            ElevatorStruct.initAnimationArea = TRUE;
            ElevatorStruct.updateAnimationArea = FALSE;

            // debounce the press
            while(GetHWButtonCenter() == HW_BUTTON_PRESS);

            // this is just an example message 
            ShowElevatorMessage(OpeningStr);
        }    
        else if ((GetHWButtonLeft() == HW_BUTTON_PRESS) && (ElevatorStruct.carDir != CAR_GOING_DOWN))
        {
            // check if already at the highest floor
            if (ElevatorStruct.floorLocation != MINFLOOR)
            {
                
                ShowElevatorMessage(WaitStr);

                // set to go up
                while(GetHWButtonLeft() == HW_BUTTON_PRESS);
                ElevatorStruct.carDir = CAR_GOING_DOWN;
                ElevatorStruct.arrowSourceOffset = ElevatorStruct.downArrowOffset;
     
#if (DISP_ORIENTATION == 0)   
                ElevatorStruct.animateLineStart = ElevatorStruct.animationAreaHeight; 
                ElevatorStruct.animateLineCurrent = ElevatorStruct.animationAreaHeight;
#elif (DISP_ORIENTATION == 90)   
                ElevatorStruct.animateLineStart = ElevatorStruct.animationAreaWidth; 
                ElevatorStruct.animateLineCurrent = ElevatorStruct.animationAreaWidth;
#endif
                ElevatorStruct.animateLineStop = 0;

                ElevatorStruct.incValue = -1*ANIMATEINCREMENT;
                ElevatorStruct.updateAnimationArea = TRUE;
                T3CONbits.TON = 1;  //  turn on timer
                AnimateTick = tick;

            }
        }    
        else if ((GetHWButtonRight() == HW_BUTTON_PRESS) && (ElevatorStruct.carDir != CAR_GOING_UP))
        {
            if (ElevatorStruct.floorLocation != MAXFLOOR)
            {
                ShowElevatorMessage(WaitStr);

                // set to go down
                while(GetHWButtonRight() == HW_BUTTON_PRESS);
                ElevatorStruct.carDir = CAR_GOING_UP;
                ElevatorStruct.arrowSourceOffset = ElevatorStruct.upArrowOffset;
    
                ElevatorStruct.animateLineStart = 0;
                ElevatorStruct.animateLineCurrent = 0;
#if (DISP_ORIENTATION == 0)   
                ElevatorStruct.animateLineStop = ElevatorStruct.animationAreaHeight;
#elif (DISP_ORIENTATION == 90)   
                ElevatorStruct.animateLineStop = ElevatorStruct.animationAreaWidth;
#endif    
                ElevatorStruct.incValue = ANIMATEINCREMENT;

                ElevatorStruct.updateAnimationArea = TRUE;
                T3CONbits.TON = 1;  //  turn on timer
                AnimateTick = tick;

            }
        }


       	/* =============================================== */
        // This initialize the animated components
       	/* =============================================== */
        if (ElevatorStruct.initAnimationArea == TRUE) 
        {    
            FloorTick = tick;
            AnimateTick = tick;
            ElevatorStruct.animateLineCurrent = ElevatorStruct.animateLineStart;

            if (_VMRGN)
            {
            
                // remove the animated arrow or refresh the background where
                // the arrows are animated
                GraphicsMemCopyCont2Disc(                                  
                            HIDDENBUFFER_START_ADDRESS,             \
                            GFX_DISPLAY_BUFFER_START_ADDRESS,       \
                            ElevatorStruct.backgroundSourceOffset,  \
                            ElevatorStruct.arrowDestOffset,         \
                            ElevatorStruct.animationAreaWidth,      \
                            ElevatorStruct.animationAreaHeight      \
                            );

                ShowElevatorCarLocation(ElevatorStruct.floorLocation, pUsedFont, ElevatorStruct.floorStatusXStart); 

                ElevatorStruct.initAnimationArea = FALSE;
            }
        } // if (ElevatorStruct.initAnimationArea == TRUE)    

       	/* =============================================== */
        // This portion updates the animation
       	/* =============================================== */
        if (ElevatorStruct.updateAnimationArea == TRUE)
        {
            if (_VMRGN)
            {
                ElevatorStruct.updateAnimationArea = FALSE;
                UpdateElevatorAnimation();
            }
        }

       	/* =============================================== */
        // This portion updates the animation
       	/* =============================================== */
        if (ElevatorStruct.updateMessage == TRUE)
        {
            ElevatorStruct.updateMessage = FALSE;
            ShowElevatorMessage(NULL);
        }

        
        if (((ElevatorStruct.carDir == CAR_GOING_UP) || (ElevatorStruct.carDir == CAR_GOING_DOWN)) && (ElevatorStruct.updateCurrentFloor == TRUE))
        {

            // show the floor number
            if (ElevatorStruct.carDir == CAR_GOING_DOWN)
                ElevatorStruct.floorLocation--;
            else if (ElevatorStruct.carDir == CAR_GOING_UP)
                ElevatorStruct.floorLocation++;

            if ((ElevatorStruct.floorLocation == MINFLOOR) || (ElevatorStruct.floorLocation == MAXFLOOR))
            {
                // when floor limit is reached stop the animation
                ElevatorStruct.carDir = CAR_STOPPED;
                ElevatorStruct.initAnimationArea = TRUE;
                ShowElevatorMessage(HaltStr);
            }

            while(!_VMRGN);

            // update the area where the floor number is displayed
            GraphicsMemCopyCont2Disc(                               \
                        HIDDENBUFFER_START_ADDRESS,                 \
                        GFX_DISPLAY_BUFFER_START_ADDRESS,           \
                        ElevatorStruct.floorBkGrndSourceOffset,     \
                        ElevatorStruct.floorBkGrndDestOffset,       \
                        ElevatorStruct.floorStatusWidth,            \
                        ElevatorStruct.floorStatusHeight);

            // show the current floor number
            ShowElevatorCarLocation(ElevatorStruct.floorLocation, pUsedFont, ElevatorStruct.floorStatusXStart); 

            ElevatorStruct.updateCurrentFloor = FALSE;
        }

       	/* =============================================== */
        // add other application specific code here
      	/* =============================================== */


    } // end of while(1)

} // end of main


/*********************************************************************
* Function: void InitializeElevatorAnimation(void)
*
* Overview: Initializes the images and saves animated areas in the 
*           hidden buffer. This is done to make the updates of areas
*           of the screen faster.
*
* PreCondition: none
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
void InitializeElevatorAnimation(void)
{
    void *pImage, *pFont, *pMsgFont;
    XCHAR TempStr[] = {'8',0};

   
    pFont = (void*)&ELEVATOR_FONT;
    pMsgFont = (void*)&ELEVATOR_MSG_FONT;

    /* =============================================== */
    // Initialize the variables
    /* =============================================== */
    // initialize to the active animated image
    pImage = (void*)&DOWNARROWIMAGE;

    // initialize location of the elevator car
    ElevatorStruct.floorLocation = DEFAULTFLOORNUMBER;

    // initialize dimension of the animated area
#if (DISP_ORIENTATION == 0)   

    ElevatorStruct.animationAreaHeight = GetImageHeight(pImage) >> 1;
    ElevatorStruct.animationAreaWidth = GetImageWidth(pImage);

#elif (DISP_ORIENTATION == 90)   

    ElevatorStruct.animationAreaHeight = GetImageWidth(pImage);
    ElevatorStruct.animationAreaWidth = GetImageHeight(pImage) >> 1;

#endif

    // Note that the width and height of the RCCGPU copied areas are based on the native orientation of the screen

    /* =============================================== */
    // The offset of the arrow animation on the screen
    /* =============================================== */
    // default location of the animated arrow in the screen 
#if (DISP_ORIENTATION == 0)   

    ElevatorStruct.arrowDestOffset = (((GetMaxX()>>1) - ElevatorStruct.animationAreaWidth) >> 1)  + \
                                     (((GetMaxY()-ElevatorStruct.animationAreaHeight-GetTextHeight(pMsgFont)) >>1) * LINEOFFSET) ;

#elif (DISP_ORIENTATION == 90)   

    ElevatorStruct.arrowDestOffset = (((DISP_HOR_RESOLUTION/3) - ElevatorStruct.animationAreaWidth) >> 1)  + \
                                     (((DISP_VER_RESOLUTION - ElevatorStruct.animationAreaHeight) >> 1) * DISP_HOR_RESOLUTION);

#endif

    /* =============================================== */
    // The x,y location of the text showing the current 
    // floor location of the elevator car
    /* =============================================== */
    // location of the floor status on the display
    // this location is based on the design or layout of the screen
#if (DISP_ORIENTATION == 0)   

    ElevatorStruct.floorStatusXStart = (GetMaxX() >> 1) + (((GetMaxX() >> 1) - (GetTextWidth(TempStr, pFont)*MAXFLOORCHARS)) >> 1);
    ElevatorStruct.floorStatusYStart = ((GetMaxY()-GetTextHeight(pFont)-GetTextHeight(pMsgFont))>>1);

#elif (DISP_ORIENTATION == 90)   

    ElevatorStruct.floorStatusXStart = (GetMaxX() - (GetTextWidth(TempStr, pFont) * MAXFLOORCHARS)) >> 1;
    ElevatorStruct.floorStatusYStart = ((GetMaxY()/3 - GetTextHeight(pFont))>>1) + GetMaxY()/3;

#endif

    /* =============================================== */
    // The offset of the composite arrow images 
    // located in the hidden buffer.
    // NOTE: The composite arrow images are stored in 
    //       hidden buffer in a discontinuous manner.
    //       Therefore, the offsets are based on that.
    /* =============================================== */

#if (DISP_ORIENTATION == 0)   

    ElevatorStruct.downArrowOffset = 0;
    ElevatorStruct.upArrowOffset = (DWORD)((ElevatorStruct.animationAreaHeight<<1) * ElevatorStruct.animationAreaWidth);

#elif (DISP_ORIENTATION == 90)   

    ElevatorStruct.downArrowOffset = 0; 
    ElevatorStruct.upArrowOffset = (DWORD)GetImageHeight(pImage);

#endif
    // variable offset used, changes to the up or down arrow depending 
    // on the direction of the elevator car
    ElevatorStruct.arrowSourceOffset = ElevatorStruct.downArrowOffset;                    

    // location of copy of the background of the animated area.
#if (DISP_ORIENTATION == 0)   

    ElevatorStruct.backgroundSourceOffset = ElevatorStruct.upArrowOffset + (DWORD)((ElevatorStruct.animationAreaHeight<<1) * ElevatorStruct.animationAreaWidth);  

#elif (DISP_ORIENTATION == 90)   

    ElevatorStruct.backgroundSourceOffset = (DWORD)(ElevatorStruct.animationAreaHeight * LINEOFFSET);  

#endif

    /* =============================================== */
    // Floor number location and background offsets
    /* =============================================== */
    
#if (DISP_ORIENTATION == 0)   

    ElevatorStruct.floorStatusWidth  = GetTextWidth(TempStr, pFont)*MAXFLOORCHARS;
    ElevatorStruct.floorStatusHeight = GetTextHeight(pFont); 
    ElevatorStruct.floorBkGrndSourceOffset = (DWORD)(ElevatorStruct.backgroundSourceOffset + ((DWORD)ElevatorStruct.animationAreaWidth * (DWORD)ElevatorStruct.animationAreaHeight));
    ElevatorStruct.floorBkGrndDestOffset = ElevatorStruct.floorStatusXStart + (DWORD)(ElevatorStruct.floorStatusYStart * LINEOFFSET);

#elif (DISP_ORIENTATION == 90)

    ElevatorStruct.floorStatusHeight = GetTextHeight(pFont);
    ElevatorStruct.floorStatusWidth  = GetTextWidth(TempStr, pFont)*MAXFLOORCHARS; 
    ElevatorStruct.floorBkGrndSourceOffset = (DWORD)(ElevatorStruct.backgroundSourceOffset + ((DWORD)ElevatorStruct.animationAreaWidth * (DWORD)ElevatorStruct.animationAreaHeight));
    ElevatorStruct.floorBkGrndDestOffset = ElevatorStruct.floorStatusYStart + (DWORD)(ElevatorStruct.floorStatusXStart * LINEOFFSET);

#endif

    /* =============================================== */
    // Message location and background offsets
    /* =============================================== */
    ElevatorStruct.currLang = LANGUAGEUSED;

#if (DISP_ORIENTATION == 0)   

    ElevatorStruct.msgXStart = 0;
    ElevatorStruct.msgYStart = ElevatorStruct.floorStatusYStart + ElevatorStruct.floorStatusHeight;                 

    ElevatorStruct.msgBkGrndSourceOffset = ElevatorStruct.floorBkGrndSourceOffset + (DWORD)(ElevatorStruct.floorStatusHeight * ElevatorStruct.floorStatusWidth);
    ElevatorStruct.msgBkGrndDestOffset = ElevatorStruct.msgXStart + (DWORD)((DWORD)ElevatorStruct.msgYStart * LINEOFFSET);    
    ElevatorStruct.msgWidth = GetMaxX();
    ElevatorStruct.msgHeight = GetTextHeight(pMsgFont);

#elif (DISP_ORIENTATION == 90)   

    ElevatorStruct.msgXStart = 0;
    ElevatorStruct.msgYStart = ElevatorStruct.floorStatusYStart + ElevatorStruct.floorStatusHeight;                 

    ElevatorStruct.msgBkGrndSourceOffset = ElevatorStruct.floorBkGrndSourceOffset + (DWORD)(ElevatorStruct.floorStatusHeight * ElevatorStruct.floorStatusWidth);
    ElevatorStruct.msgBkGrndDestOffset = ElevatorStruct.msgYStart + (DWORD)(ElevatorStruct.msgXStart * LINEOFFSET);
    ElevatorStruct.msgWidth = GetTextHeight(pMsgFont);
    ElevatorStruct.msgHeight = GetMaxX();

#endif


    // disable the transparency for now to pre-render the arrows
    TransparentColorDisable();          

    WORD imageXLoc, imageYLoc, destDataType;
    WORD imageWidth, imageHeight;
    

#if (DISP_ORIENTATION == 0)   
    imageXLoc = 0;
    imageYLoc = 0;
    imageWidth = ElevatorStruct.animationAreaWidth;
    imageHeight = ElevatorStruct.animationAreaHeight << 1;
    destDataType = GFX_DATA_CONTINUOUS;
#elif (DISP_ORIENTATION == 90)
    imageXLoc = (GetMaxX() + 1) - ElevatorStruct.animationAreaHeight;
    imageYLoc = 0;
    imageWidth = ElevatorStruct.animationAreaWidth << 1;
    imageHeight = ElevatorStruct.animationAreaHeight;
    destDataType = GFX_DATA_DISCONTINUOUS;
#endif                    

    PutImage(imageXLoc, imageYLoc, (void*)&DOWNARROWIMAGE, 1);
    ROPBlock(    
            GFX_DISPLAY_BUFFER_START_ADDRESS,       
            HIDDENBUFFER_START_ADDRESS,             
            0, ElevatorStruct.downArrowOffset,
            GFX_DATA_DISCONTINUOUS, destDataType, 
            RCC_COPY, RCC_ROP_C, 0,                 
            imageWidth, imageHeight);


    PutImage(imageXLoc, imageYLoc, (void*)&UPARROWIMAGE, 1);
    ROPBlock(    
            GFX_DISPLAY_BUFFER_START_ADDRESS,       
            HIDDENBUFFER_START_ADDRESS,             
            0, ElevatorStruct.upArrowOffset,
            GFX_DATA_DISCONTINUOUS, destDataType, 
            RCC_COPY, RCC_ROP_C, 0,                 
            imageWidth, imageHeight);

    /* =============================================== */
    // Render the background
    /* =============================================== */
    PutImage(0,0,(void*)&BACKGROUND_IMAGE,1);

    // copy the area where the animation will occur to the hidden buffer
    // ElevatorStruct.arrowDestOffset is the location of the animation area
    GraphicsMemCopyDisc2Cont(                           
                GFX_DISPLAY_BUFFER_START_ADDRESS,       
                HIDDENBUFFER_START_ADDRESS,             
                ElevatorStruct.arrowDestOffset,         
                ElevatorStruct.backgroundSourceOffset,  
                ElevatorStruct.animationAreaWidth,      
                ElevatorStruct.animationAreaHeight);

    // copy the area where the floor numbers will be shown to the hidden buffer
    GraphicsMemCopyDisc2Cont(                           \
                GFX_DISPLAY_BUFFER_START_ADDRESS, 
                HIDDENBUFFER_START_ADDRESS,             \
                ElevatorStruct.floorBkGrndDestOffset,   \
                ElevatorStruct.floorBkGrndSourceOffset, \
                ElevatorStruct.floorStatusWidth,        \
                ElevatorStruct.floorStatusHeight);

    // copy the area where messages will be displayed to the hidden buffer
    GraphicsMemCopyDisc2Cont(                           \
                GFX_DISPLAY_BUFFER_START_ADDRESS, 
                HIDDENBUFFER_START_ADDRESS,             \
                ElevatorStruct.msgBkGrndDestOffset,     \
                ElevatorStruct.msgBkGrndSourceOffset,   \
                ElevatorStruct.msgWidth,        \
                ElevatorStruct.msgHeight);

    /* =============================================== */
    // Pre-render the animated images in the hidden buffer
    /* =============================================== */
    GFX_SetDisplayArea(HIDDENBUFFER_START_ADDRESS);

#ifdef DEBUG_DEMO_MODE
    while(GetHWButtonRight() == HW_BUTTON_PRESS);
#endif
    
    // switch back to main display buffer
    GFX_SetDisplayArea(GFX_DISPLAY_BUFFER_START_ADDRESS);

    /* =============================================== */
    // the following are variables used in the loops
    /* =============================================== */
    // default increment value    
    ElevatorStruct.incValue = ANIMATEINCREMENT;                              
    // end of animation (loop back when reached)
    ElevatorStruct.animateLineStop  = ElevatorStruct.animationAreaHeight;  
    // start at line 0
    ElevatorStruct.animateLineStart = 0;                                   
    ElevatorStruct.animateLineCurrent = 0;

    ElevatorStruct.carDir = CAR_STOPPED;
    ElevatorStruct.initAnimationArea = FALSE;
    ElevatorStruct.updateAnimationArea = FALSE;
    ElevatorStruct.updateCurrentFloor = TRUE;
    ElevatorStruct.updateMessage = FALSE;

    // re-enable transparency so we can ignore the background
    // of the arrows
    TransparentColorEnable(TRANSCOLOR);

}

/*********************************************************************
* Function: void UpdateElevatorAnimation(void)
*
* Overview: Updates the animation of the arrows.
*
* PreCondition: none
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
void UpdateElevatorAnimation(void)
{
    /* =============================================== */
    // This portion draws the animation
    /* =============================================== */
    if ((ElevatorStruct.carDir == CAR_GOING_UP) || (ElevatorStruct.carDir == CAR_GOING_DOWN))
    {    
        // refresh the portion of the background that needs to be refreshed
        GraphicsMemCopyCont2Disc(                                  
                    HIDDENBUFFER_START_ADDRESS,             \
                    GFX_DISPLAY_BUFFER_START_ADDRESS,       \
                    ElevatorStruct.backgroundSourceOffset,  \
                    ElevatorStruct.arrowDestOffset,         \
                    ElevatorStruct.animationAreaWidth,      \
                    ElevatorStruct.animationAreaHeight      \
                    );

        // update the location of the next copy
        ElevatorStruct.animateLineCurrent += (ElevatorStruct.incValue);

        // force the animateLineCurrent to be the limit since it is at the end anyway
        if (((ElevatorStruct.carDir == CAR_GOING_DOWN) && ((SHORT)ElevatorStruct.animateLineCurrent <= (SHORT)ElevatorStruct.animateLineStop)) || \
            ((ElevatorStruct.carDir == CAR_GOING_UP) && ((SHORT)ElevatorStruct.animateLineCurrent >= (SHORT)ElevatorStruct.animateLineStop)))
        {
            ElevatorStruct.animateLineCurrent = ElevatorStruct.animateLineStart;
        }

#if (DISP_ORIENTATION == 0)

        // refresh the arrow to animate the arrow movement
        ROPBlock(   HIDDENBUFFER_START_ADDRESS, GFX_DISPLAY_BUFFER_START_ADDRESS, 
                    ElevatorStruct.arrowSourceOffset + (DWORD)(ElevatorStruct.animateLineCurrent * ElevatorStruct.animationAreaWidth), 
                    ElevatorStruct.arrowDestOffset,  
                    GFX_DATA_CONTINUOUS, GFX_DATA_DISCONTINUOUS, 
                    RCC_TRANSPARENT_COPY, RCC_ROP_C, TRANSCOLOR, 
                    ElevatorStruct.animationAreaWidth, 
                    ElevatorStruct.animationAreaHeight);

#elif (DISP_ORIENTATION == 90)

        // refresh the arrow to animate the arrow movement
        ROPBlock(   HIDDENBUFFER_START_ADDRESS, GFX_DISPLAY_BUFFER_START_ADDRESS, 
                    ElevatorStruct.arrowSourceOffset + ElevatorStruct.animateLineCurrent,
                    ElevatorStruct.arrowDestOffset,  
                    GFX_DATA_DISCONTINUOUS, GFX_DATA_DISCONTINUOUS, 
                    RCC_TRANSPARENT_COPY, RCC_ROP_C, TRANSCOLOR, 
                    ElevatorStruct.animationAreaWidth, 
                    ElevatorStruct.animationAreaHeight);

#endif
        
    }
}



/*********************************************************************
* Function: NumberToString( WORD Value,      XCHAR *pText, 
*                           BYTE NoOfDigits, BYTE DotPos )
*
* Overview: Convert the number Value to string pointed to by pText
*
* PreCondition: none
*
* Input: Value - the number to be convered
*        pText - pointer to the array of XCHAR 
*        NoOfDigits - the number of digits to use (or length of output string) 
*        DotPos - if there is a decimal on the string.
*
* Output: None, result of conversion is stored in the 
*         memory pointed to by pText.
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
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

/*********************************************************************
* Function: WORD ExternalMemoryCallback(GFX_EXTDATA *pExtDataStruct, 
*                                       LONG offset, 
*                                       WORD nCount, 
*                                       void *pBuffer)
*
* Overview: Callback for external memory access. Required by the 
*           Graphics Library's Primitive Layer when using
*           External Fonts and Images.
*
* PreCondition: Graphics Library must be initialized.
*
* Input: pExtDataStruct - pointer to the graphics external 
*                         data structure.
*        offset - offset of the data in memory
*        nCount - number of bytes to get from external memory
*        buffer - location of the retrieved data from external 
*                 memory
*
* Output: None
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD ExternalMemoryCallback(GFX_EXTDATA *pExtDataStruct, LONG offset, WORD nCount, void *pBuffer)
{

	#if defined (USE_SST25VF016)
        ReadExternalMemArray(pExtDataStruct->address + offset, (BYTE *)pBuffer, nCount);
    #endif
    return (nCount);

}

/*********************************************************************
* Function:  Timer3 ISR
*
* Overview: ISR for the timer used for tick generation and resistive 
*           touch screen detection. tick frequency is based on 
*           the timer initialization.
*
* PreCondition: none
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
void __attribute__((interrupt, shadow, auto_psv)) _T3Interrupt(void)
{
    tick++;

    TMR3 = 0;
    // Clear flag
    IFS0bits.T3IF = 0;

    if ((ElevatorStruct.carDir == CAR_GOING_UP) || (ElevatorStruct.carDir == CAR_GOING_DOWN))
    {
        if ((DWORD)(tick - FloorTick) > FLOORANIMATIONDELAY)
        {
            ElevatorStruct.updateCurrentFloor = TRUE;
            FloorTick = tick;
    
        }
        if ((DWORD)(tick - AnimateTick) > ANIMATIONDELAY)
        {   
            ElevatorStruct.updateAnimationArea = TRUE;
            AnimateTick = tick;
        }
    }
    else if (ElevatorStruct.carDir == CAR_STOPPED) 
    {
        if ((DWORD)(tick - FloorTick) > (FLOORANIMATIONDELAY*4))
        {
            ElevatorStruct.updateMessage = TRUE;
            //turn off timer
            T3CONbits.TON = 0;  
        }
    }
}

/*********************************************************************
* Function:  void TickInit(void)
*
* Overview: Tick generator initialization.
*
* PreCondition: Graphics Library must be initialized.
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
/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD       500 // us
#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000
/* */
void TickInit(void)
{
    tick = 0;
    MsgAnimateTick = AnimateTick = FloorTick = tick;
    
    // Initialize Timer3
    TMR3 = 0;
    PR3 = TICK_PERIOD;
    IFS0bits.T3IF = 0;  //Clear flag
    IEC0bits.T3IE = 1;  //Enable interrupt
    T3CONbits.TON = 1;  //Run timer
    
}

/*********************************************************************
* Function:  void InitializeBoard(void)
*
* Overview: Initializes the hardware components including the 
*           PIC device used.
*
* PreCondition: Graphics Library must be initialized.
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
void InitializeBoard(void)
{

#if defined(PIC24FJ256DA210_DEV_BOARD)
    // for PIC24FJ256DA210 Development Board
    _ANSG8 = 0; /* S1 */
    _ANSE9 = 0; /* S2 */
    _ANSB5 = 0; /* S3 */
#endif

#if defined (USE_SST25VF016)

    // initialize graphics library and display driver
    InitGraph();                    

    // Initialize the SPI Flash Memory if used
    //Set IOs directions for SST25 SPI
    SST25_CS_LAT = 1;
    SST25_CS_TRIS = 0;

    // Set the pins to be digital 
	SST25_SDI_ANS = 0;
    SST25_SDO_ANS = 0;

    SST25_SCK_TRIS = 0;
    SST25_SDO_TRIS = 0;
    SST25_SDI_TRIS = 1;

    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

	#if (SST25_SPI_CHANNEL == 1)
	    RPOR1bits.RP2R = 8;                 // assign RP2 for SCK1
	    RPOR0bits.RP1R = 7;                 // assign RP1 for SDO1
	    RPINR20bits.SDI1R = 0;              // assign RP0 for SDI1
    #elif (SST25_SPI_CHANNEL == 2)
        RPOR1bits.RP2R = 11;                // assign RP2 for SCK2
	    RPOR0bits.RP1R = 10;                // assign RP1 for SDO2
	    RPINR22bits.SDI2R = 0;              // assign RP0 for SDI2
	#endif

        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    // initialize the flash memory
    InitExternalMem(&SPI_Init_Data); 

#endif

    /////////////////////////////////////////////////////
	/* =============================================== */
	// check if the external 
	// flash is accessible 
	/* =============================================== */
    CheckExternalMemory();
    
    // initialize the timer that manages the tick counter
    TickInit();                                 

    HardwareButtonInit();           	        // Initialize the hardware buttons
   
    // initialize the screen to screen background
    SetColor(ELEVATOR_DEMO_BACKGROUND_COLOR);
    ClearDevice();
}

/*********************************************************************
* Function:  void CheckExternalMemory(void)
*
* Overview: Check if the external flash is initialized and contains 
*           the proper data for the demo.
*
* PreCondition: none
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
void CheckExternalMemory(void)
{
	// before anything else let's check if the parallel flash is accessible 
	// check if we can read the Product ID ( note that this is specific to SST39LF400A or SST39VF400A)

#if defined (USE_SST39LF400)

	SST39LF400Init();	
	if (SST39LF400CheckID() == 0)
	{
        InitGraph();	    
		CreateError((XCHAR*) ParallelFlashNotAccessibleString, (void*)&GOLFontDefault);
	}
    InitGraph();	    

#endif

    CheckExternalFlashHex();
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
        ReadExternalMemArray(GRC_CRC32_EXTERNAL_ADDR, (BYTE *)&externalCRC, 8);

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
        OutTextXY(10,(textHeight*3), (XCHAR*)PressToContinueString1);
        OutTextXY(10,(textHeight*4), (XCHAR*)PressToContinueString2);
        OutTextXY(10,(textHeight*5), (XCHAR*)ExternalMemoryProgString1);
        OutTextXY(10,(textHeight*6), (XCHAR*)ExternalMemoryProgString2);
        OutTextXY(10,(textHeight*7), (XCHAR*)ViaString);

        // wait for user to be ready
        while(GetHWButtonProgram() == HW_BUTTON_PRESS);
        DelayMs(500);

        // wait for user to be ready
        while(GetHWButtonProgram() == HW_BUTTON_RELEASE);
        DelayMs(500);

        InitExternalMem(&SPI_Init_Data);

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

/*********************************************************************
* Function:  void ShowElevatorCarLocation(WORD floor, 
*                                         void *pFont, 
*                                         DWORD offset) 
*
* Overview: Updates the floor number on the screen.
*
* PreCondition:  Graphics Library must be initialized.
*
* Input: floor - the current floor the elevator car is located.
*        pFont - font used to display the floor location.
*        offset - offset of the text. This is decided by the
*                 layout of the screen. 
*                 
*
* Output: None
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
void ShowElevatorCarLocation(WORD floor, void *pFont, DWORD offset) 
{
    WORD  floorXStart, floorYStart; 
    static  XCHAR   currentFloorStr[5] = {0,0,0,0,0};

    if (floor > 9)
        NumberToString(floor, currentFloorStr, 2, 0);
    else
        NumberToString(floor, currentFloorStr, 1, 0);
    
    floorXStart = ElevatorStruct.floorStatusXStart + \
                  ((ElevatorStruct.floorStatusWidth - GetTextWidth(currentFloorStr, pFont)) >> 1);
    floorYStart = ElevatorStruct.floorStatusYStart;

    SetColor(FLOORTEXTCOLOR);
    SetFont(pFont);

    OutTextXY(floorXStart, floorYStart, currentFloorStr);
    while(IsDeviceBusy());
}

/*********************************************************************
* Function:  void ShowElevatorMessage(XCHAR *pString) 
*
* Overview: Show the elevator message on the screen
*           specified by the pString pointer. Erases the 
*           previous displayed string.
*
* PreCondition: Graphics Library must be initialized. Elevator
*               structure must be initialized.
*
* Input: pString - pointer to the string to be displayed.
*
* Output: None
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
void ShowElevatorMessage(XCHAR *pString) 
{
    static XCHAR *pPrevStr = NULL;
    void *pFont;
    WORD msgXStart;
    
    pFont = (void*) &ELEVATOR_MSG_FONT;
    msgXStart = (GetMaxX()-GetTextWidth(pPrevStr, pFont))>>1;

    SetFont(pFont);   

    // refresh the portion of the background that needs to be refreshed
    GraphicsMemCopyCont2Disc(                                  
                HIDDENBUFFER_START_ADDRESS,             
                GFX_DISPLAY_BUFFER_START_ADDRESS,       
                ElevatorStruct.msgBkGrndSourceOffset,   
                ElevatorStruct.msgBkGrndDestOffset,     
                ElevatorStruct.msgWidth,                
                ElevatorStruct.msgHeight                
                );

    msgXStart = (GetMaxX()-GetTextWidth(pString, pFont))>>1;

    SetColor(FLOORTEXTCOLOR);     
    OutTextXY(msgXStart, ElevatorStruct.msgYStart, (XCHAR *)pString);

    while(IsDeviceBusy());
    pPrevStr = pString;
}

/*********************************************************************
* Function: CreateError( XCHAR *pText, void *pFont)
*
* Overview: Create Error Flash (Blue Screen) with the message
*           from the pointer pText.
*
* PreCondition: Graphics Library must be initialized.
*
* Input: pText - pointer to the array of XCHAR to display
*
* Output: None
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
void CreateError(XCHAR* pText, void *pFont)
{
    // NOTE: RunTimeErrorStr[] must be initialized prior

    // Blue Screen Error
    SetColor(119);
    ClearDevice();
    SetColor(-1);
	
    // Flash Error Message
    SetFont(pFont);
    if(pText == NULL)
    {
        while(!OutTextXY(0, 0, (XCHAR *)RunTimeErrorStr));
    }
    else
    {
        while(!OutTextXY(0,0, pText));
    }
    while(1);
}

#if defined (USE_SST39LF400)
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
#endif //#if defined (USE_SST39LF400)


