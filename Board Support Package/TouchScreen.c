/*****************************************************************************
 *
 * Simple 4 wire resistive touch screen driver
 *
 *****************************************************************************
 * FileName:        TouchScreenResistive.c
 * Processor:       PIC24, PIC32, dsPIC, PIC24H
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/19/11		Ported from TouchScreen driver.
 * 10/03/11     Modified calibration screen.
 *****************************************************************************/
#include "GraphicsConfig.h"

#if defined (USE_TOUCHSCREEN)

#include "Graphics/Graphics.h"
#include "TouchScreen.h"
#include "TimeDelay.h"

//////////////////////// LOCAL PROTOTYPES ////////////////////////////
extern void TouchHardwareInit(void *initValues);
extern void TouchStoreCalibration(void);
extern void	TouchLoadCalibration(void);
extern void TouchCalHWGetPoints(void);

extern const WORD mchpTouchScreenVersion;

void 	TouchCheckForCalibration(void);



// Default Calibration Inset Value (percentage of vertical or horizontal resolution)
// Calibration Inset = ( CALIBRATIONINSET / 2 ) % , Range of 0–20% with 0.5% resolution
// Example with CALIBRATIONINSET == 20, the calibration points are measured
// 10% from the corners.
#define CALIBRATIONINSET   20                               // range 0 <= CALIBRATIONINSET <= 40 

#define CAL_X_INSET    (((GetMaxX()+1)*(CALIBRATIONINSET>>1))/100)
#define CAL_Y_INSET    (((GetMaxY()+1)*(CALIBRATIONINSET>>1))/100)

//////////////////////// FUNCTION POINTERS ///////////////////////////
NVM_READ_FUNC           pCalDataRead = NULL;                // function pointer to data read
NVM_WRITE_FUNC          pCalDataWrite = NULL;               // function pointer to data write
NVM_SECTORERASE_FUNC    pCalDataSectorErase = NULL;         // function pointer to data sector erase

//////////////////////// GLOBAL VARIABLES ////////////////////////////
#define CALIBRATION_DELAY   300                             // delay between calibration touch points

#if (COLOR_DEPTH == 8) || (COLOR_DEPTH == 16) || (COLOR_DEPTH == 24)
    #define RESISTIVETOUCH_MSG_BACKGROUND_COLOR     WHITE
    #define RESISTIVETOUCH_MSG_FOREGROUND1_COLOR    BRIGHTRED
    #define RESISTIVETOUCH_MSG_FOREGROUND2_COLOR    BLACK
#elif (COLOR_DEPTH == 4)
    #define RESISTIVETOUCH_MSG_BACKGROUND_COLOR     GRAY015
    #define RESISTIVETOUCH_MSG_FOREGROUND1_COLOR    GRAY008
    #define RESISTIVETOUCH_MSG_FOREGROUND2_COLOR    GRAY000
#elif (COLOR_DEPTH == 1)
    #define RESISTIVETOUCH_MSG_BACKGROUND_COLOR     WHITE
    #define RESISTIVETOUCH_MSG_FOREGROUND1_COLOR    BLACK
    #define RESISTIVETOUCH_MSG_FOREGROUND2_COLOR    BLACK
#endif


/*********************************************************************
* Function: void TouchInit(NVM_WRITE_FUNC pWriteFunc, NVM_READ_FUNC pReadFunc, NVM_SECTORERASE_FUNC pSectorErase, void *initValues)
*
* PreCondition: none
*
* Input: pWriteFunc - non-volatile memory write function pointer
*        pReadFunc - non-volatile memory read function pointer
*        pSectorErase - non-volatile memory sector function pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: Initializes the touch screen hardware.
*
* Note: none
*
********************************************************************/
void TouchInit(NVM_WRITE_FUNC pWriteFunc, NVM_READ_FUNC pReadFunc, NVM_SECTORERASE_FUNC pSectorErase, void *initValues)
{

    TouchHardwareInit(initValues);
    DelayMs(2);

	// assign the addresses of the callback functions
	// if these are NULL, the TouchLoadCalibration()
	// and TouchStoreCalibration() will skip the actual
	// read and/or writes.
	pCalDataWrite = pWriteFunc;
	pCalDataSectorErase = pSectorErase;
	pCalDataRead = pReadFunc;
    
	// if callbacks has been assigned check if calibration is valid
	if (pCalDataRead != NULL)
	{
		// check if version of library is correct
		// MCHP_TOUCHSCREEN_RESISTIVE_VERSION should be defined in the hardware profile.
		if (pCalDataRead(ADDRESS_RESISTIVE_TOUCH_VERSION) != mchpTouchScreenVersion)
		{
			// not calibrated yet, perform the calibration 
			TouchCalibration();
			// store the calibration data
			TouchStoreCalibration();
		}
		else
		{
			// check if user wants to run calibration 
			TouchCheckForCalibration();
		}

		// load the calibration values, to check if the store worked
		TouchLoadCalibration();
	}
	else
	{
		// Since there is no Non-Voltile memory, run the calibration since to 
		// make sure the screen is always calibrated after touch screen initialization.
		TouchCalibration();
	}	

}

/*********************************************************************
* Function: void TouchGetMsg(GOL_MSG* pMsg)
*
* PreCondition: none
*
* Input: pointer to the message structure to be populated
*
* Output: none
*
* Side Effects: none
*
* Overview: populates GOL message structure
*
* Note: none
*
********************************************************************/
void TouchGetMsg(GOL_MSG *pMsg)
{
    static SHORT    prevX = -1;
    static SHORT    prevY = -1;

    SHORT           x, y;

    x = TouchGetX();
    y = TouchGetY();
    pMsg->type = TYPE_TOUCHSCREEN;
    pMsg->uiEvent = EVENT_INVALID;

    if((x == -1) || (y == -1))
    {
        y = -1;
        x = -1;
    }

    if((prevX == x) && (prevY == y) && (x != -1) && (y != -1))
    {
        pMsg->uiEvent = EVENT_STILLPRESS;
        pMsg->param1 = x;
        pMsg->param2 = y;
        return;
    }

    if((prevX != -1) || (prevY != -1))
    {
        if((x != -1) && (y != -1))
        {

            // Move
            pMsg->uiEvent = EVENT_MOVE;
        }
        else
        {

            // Released
            pMsg->uiEvent = EVENT_RELEASE;
            pMsg->param1 = prevX;
            pMsg->param2 = prevY;
            prevX = x;
            prevY = y;
            return;
        }
    }
    else
    {
        if((x != -1) && (y != -1))
        {

            // Pressed
            pMsg->uiEvent = EVENT_PRESS;
        }
        else
        {

            // No message
            pMsg->uiEvent = EVENT_INVALID;
        }
    }

    pMsg->param1 = x;
    pMsg->param2 = y;
    prevX = x;
    prevY = y;
}
/*********************************************************************
* Function: void TouchCheckForCalibration()
*
* PreCondition: TouchInit() must be performed before calling this 
*		        function.
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Checks if the user wants to run the calibration routine. 
*           The calibration check is performed by detecting a continuous
*		    1 second (approximate) touch on the screen. When calibration
*		    is detected, it calls the actual calibration sequence
*           TouchLoadCalibration().
*
* Note: none
*
********************************************************************/
void TouchCheckForCalibration(void)
{
	WORD count;
	
	// check for calibration
	// this tests any touches on the touch screen, user has to touch the screen for more than 1
	// second to make the calibration work 
	count = 0;
	while(1)
	{
		DelayMs(100);
		// check if there is a touch
		if ((TouchGetX() == -1) && (TouchGetY() == -1))
			break;
		else
			count++;
		if (count == 10)
		{
			// do calibration
        	TouchCalibration();
        	TouchStoreCalibration();
        	break;
  		}      	
 	}  	
	
}	

/*********************************************************************
* Function:  void TouchCalibration()
*
* PreCondition: InitGraph() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: calibrates touch screen
*
* Note: none
*
********************************************************************/
void TouchCalibration(void)
{
    static const XCHAR  scr1StrLn1[] = {'P','e','r','f','o','r','m','i','n','g',' ','t','o','u','c','h',0};
    static const XCHAR  scr1StrLn2[] = {'s','c','r','e','e','n',' ','c','a','l','i','b','r','a','t','i','o','n','.',0};
    static const XCHAR  scr1StrLn3[] = {'T','o','u','c','h',' ','s','c','r','e','e','n',' ','t','o',0};
    static const XCHAR  scr1StrLn4[] = {'c','o','n','t','i','n','u','e','.',0};

    static const XCHAR  scr2StrLn1[] = {'T','o',' ','R','E','P','E','A','T',' ','c','a','l','i','b','r','a','t','i','o','n',',',0};
    static const XCHAR  scr2StrLn2[] = {'r','e','s','e','t',' ','t','h','e',' ','b','o','a','r','d',' ','w','h','i','l','e',0};
    static const XCHAR  scr2StrLn3[] = {'p','r','e','s','s','i','n','g',' ','o','n',' ','t','h','e',' ','s','c','r','e','e','e','n',0};
    static const XCHAR  scr2StrLn4[] = {'u','n','t','i','l',' ','t','h','e',' ','c','a','l','i','b','r','a','t','i','o','n',0}; 
    static const XCHAR  scr2StrLn5[] = {'p','r','o','m','p','t',' ','a','p','p','e','a','r','s','.',0};

    SHORT               x, y;
    SHORT               textHeight, textStart;
    
    SetFont((void *) &FONTDEFAULT);
    
    textHeight = GetTextHeight((void *) &FONTDEFAULT);
    textStart =  (GetMaxY() - (textHeight*8)) >> 1;

    SetColor(RESISTIVETOUCH_MSG_BACKGROUND_COLOR);
    ClearDevice();

    SetColor(RESISTIVETOUCH_MSG_FOREGROUND1_COLOR);
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr1StrLn1, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (textHeight), (XCHAR *)scr1StrLn1)));
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr1StrLn2, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (2*textHeight), (XCHAR *)scr1StrLn2)));

    SetColor(RESISTIVETOUCH_MSG_FOREGROUND1_COLOR);
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr1StrLn3, (void *) &FONTDEFAULT))>>1,  \
    							textStart + (4*textHeight), (XCHAR *)scr1StrLn3)));
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr1StrLn4, (void *) &FONTDEFAULT))>>1,  \
    							textStart + (5*textHeight), (XCHAR *)scr1StrLn4)));

    // Wait for release
    do
    {
        x = TouchGetRawX();
        y = TouchGetRawY();
    } while((y != -1) && (x != -1));

    // Wait for touch and release
    do
    {
        x = TouchGetRawX();
        y = TouchGetRawY();
    } while((y == -1) || (x == -1));
    
    do
    {
        x = TouchGetRawX();
        y = TouchGetRawY();
    } while((y != -1) && (x != -1));

    DelayMs(CALIBRATION_DELAY);
    
    SetColor(RESISTIVETOUCH_MSG_BACKGROUND_COLOR);
    ClearDevice();


    
    // call actual calibration routine
    TouchCalHWGetPoints();

    SetColor(RESISTIVETOUCH_MSG_BACKGROUND_COLOR);
    ClearDevice();

    SetColor(RESISTIVETOUCH_MSG_FOREGROUND2_COLOR);
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr2StrLn1, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (1*textHeight), (XCHAR *)scr2StrLn1)));
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr2StrLn2, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (2*textHeight), (XCHAR *)scr2StrLn2)));
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr2StrLn3, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (3*textHeight), (XCHAR *)scr2StrLn3)));
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr2StrLn4, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (4*textHeight), (XCHAR *)scr2StrLn4)));
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr2StrLn5, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (5*textHeight), (XCHAR *)scr2StrLn5)));
    SetColor(RESISTIVETOUCH_MSG_FOREGROUND1_COLOR);
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr1StrLn3, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (6*textHeight), (XCHAR *)scr1StrLn3)));
    while(!(OutTextXY((GetMaxX()-GetTextWidth((XCHAR *)scr1StrLn4, (void *) &FONTDEFAULT))>>1,  \
    							 textStart + (7*textHeight), (XCHAR *)scr1StrLn4)));

    // Wait for touch
    do
    {
        x = TouchGetRawX();
        y = TouchGetRawY();
    } while((y == -1) || (x == -1));

    DelayMs(CALIBRATION_DELAY);

    SetColor(RESISTIVETOUCH_MSG_FOREGROUND2_COLOR);
    ClearDevice();
}


#endif // #if defined (USE_TOUCHSCREEN_RESISTIVE)

