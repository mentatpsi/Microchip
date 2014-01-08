/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This demo shows an animation demo using Picture widget and meter widget
 * that controls the speed of the animation.
 *****************************************************************************
 * FileName:        Animation.c
 * Dependencies:    Animation.h, MainDemo.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software LicenseJpeg
  Agreement
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok
 * Paolo Tamayo         03/10/08    ...
 *****************************************************************************/
#include "MainDemo.h"

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
#define REPEAT_TIMEOUT          200
#define DEBOUNCE_TIMEOUT        300

#define MAX_METER_VALUE         150
#define PICTURE_WIDTH           (2 * GetImageWidth((void *) &Engine1) + 1)
#define PICTURE_HEIGHT          (2 * GetImageHeight((void *) &Engine1) + 1)
#define PICTURE_XINDENT         ((GetMaxX() - 2 * PICTURE_WIDTH) / 3)
#define PICTURE_YINDENT         ((CtrlBtnTop() - PICTURE_HEIGHT) / 2)
#define WAIT_UNTIL_FINISH(x)    while(!x)

    /////////////////////////////////////////////////////////////////////////////
    //                     IMAGES USED (the animated image)
    /////////////////////////////////////////////////////////////////////////////
extern const IMAGE_FLASH   	   	Engine1;
extern const IMAGE_FLASH       	Engine2;
extern const IMAGE_FLASH       	Engine3;
extern const IMAGE_FLASH       	Engine4;

/////////////////////////////////////////////////////////////////////////////
//                            STRINGS USED
/////////////////////////////////////////////////////////////////////////////
extern XCHAR                   	ExitStr[];
extern XCHAR                   	ScaleStr[];
extern XCHAR                   	LeftArrowStr[];
extern XCHAR                   	RightArrowStr[];
extern XCHAR                   	MeterStr[];

/////////////////////////////////////////////////////////////////////////////
//                            GLOBALS
/////////////////////////////////////////////////////////////////////////////
METER                           *pMeter;        // pointer to the slider controlling speed
PICTURE                         *pPicture;      // pointer to the picture object
WORD                            animate = 1;    // switch controlling animation, if animate > 0

// the image starts moving

/************************************************************************
 Function: void CreateAnimation(void)
                                                                       
 Overview: Shows next image in animation sequence
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void NextAnimation(DWORD tick)
{
    static DWORD    prevTick = 0;
    static DWORD    prevBtnTick = 0;
    static DWORD    repeatT = REPEAT_TIMEOUT;
    BUTTON          *pBtn;
    WORD            value;

    // check if increasing speed button is pressed
    pBtn = (BUTTON *)GOLFindObject(ID_BUTTON_C);
    if(GetState(pBtn, BTN_PRESSED))
    {
        if((tick - prevBtnTick) > repeatT)
        {
            MtrSetVal(pMeter, (MtrGetVal(pMeter) - 1));
            SetState(pMeter, MTR_DRAW_UPDATE);
            if(repeatT > 30)
                repeatT -= 10;
            prevBtnTick = tick;
        }

        // check if decreasing speed button is pressed
    }
    else
    {
        pBtn = (BUTTON *)GOLFindObject(ID_BUTTON_D);
        if(GetState(pBtn, BTN_PRESSED))
        {
            if((tick - prevBtnTick) > repeatT)
            {
                MtrSetVal(pMeter, (MtrGetVal(pMeter) + 1));
                SetState(pMeter, MTR_DRAW_UPDATE);
                prevBtnTick = tick;
                if(repeatT > 30)
                    repeatT -= 10;
            }
        }
        else
        {
            repeatT = REPEAT_TIMEOUT;
            prevBtnTick = tick;
        }
    }

    // change the speed of the animation here based on the current
    // meter value
    value = MtrGetVal(pMeter);
    if(value != 0)
    {
        if((tick - prevTick) > ((500) - (3 * MtrGetVal(pMeter))))
        {
            switch(animate)
            {
                case 1:     PictSetBitmap(pPicture, (void *) &Engine1); animate++; break;
                case 2:     PictSetBitmap(pPicture, (void *) &Engine2); animate++; break;
                case 3:     PictSetBitmap(pPicture, (void *) &Engine3); animate++; break;
                case 4:     PictSetBitmap(pPicture, (void *) &Engine4); animate = 1; break;
                default:    animate = 1; break;
            }

            SetState(pPicture, PICT_DRAW);  // redraw
            prevTick = tick;
        }
    }
}

/************************************************************************
 Function: void CreateAnimation(void)
                                                                       
 Overview: Creates the animation screen.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void CreateAnimation(void)
{
    SHORT   j, height, endPoint;

    // free memory for the objects in the previous linked list and start new list
    GOLFree();

    SetColor(SCREEN_BACKGROUND_COLOR);
    ClearDevice();

    // draw the band of lines with increasing thickness background of the demo
    height = 13;
    endPoint = GetMaxY() - 35;
    SetColor(RGB565CONVERT(0x4C, 0x8E, 0xFF));
    for(j = 2; j <= endPoint; j += (height + 6))
    {
        WAIT_UNTIL_FINISH(Bar(0, j, GetMaxX(), j + height));
        if(height <= 0)
            height = 0;
        else
            height -= 1;
    }

    pPicture = PictCreate
        (
            ID_PICTURE1,                        // ID
            PICTURE_XINDENT,
            PICTURE_YINDENT,
            PICTURE_XINDENT + PICTURE_WIDTH - 1,
            PICTURE_YINDENT + PICTURE_HEIGHT - 1,
            PICT_DRAW,                          // will be dislayed, has frame
            2,                                  // scale factor is x1
            (void *) &Engine1,                  // bitmap
            altScheme
        );                                      // default GOL scheme
    pMeter = MtrCreate
        (
            ID_METER2,
            (2 * PICTURE_XINDENT + PICTURE_WIDTH),
            PICTURE_YINDENT - 20,
            (2 * (PICTURE_XINDENT + PICTURE_WIDTH)),
            PICTURE_YINDENT + PICTURE_HEIGHT + 20,
            MTR_DRAW | MTR_RING | MTR_ACCURACY, // draw normal meter object
            MAX_METER_VALUE,                    // set initial value
            0,
            MAX_METER_VALUE,                    // set range
            (void *) &GOLFontDefault,           // Title font to be used
            (void *) &GOLMediumFont,            // Value font to be used
            MeterStr,                           //
            meterScheme
        );  // alternative GOL scheme

    //pMeter->pTitleFont = (void*)&GOLFontDefault;
    //pMeter->pValueFont = (void*)&GOLMediumFont;
    // create the demo navigation/control buttons
    CreateCtrlButtons(ExitStr, ScaleStr, LeftArrowStr, RightArrowStr);
}

/************************************************************************
 Function: WORD MsgAnimation(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: This function is called by GOLMsg() function when in this 
 		   particular demo each time the valid message is received for the 
  		   objects in the demo.
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the original GOL message.
                                                                       
 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD MsgAnimation(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static BYTE scale = 2;
    WORD        value;

    if(objMsg == BTN_MSG_RELEASED)
    {
        switch(GetObjID(pObj))
        {
            case ID_BUTTON_A:
                if(objMsg == BTN_MSG_RELEASED)
                {

                    // do not process if user moved the touch away from the button
                    // returning 1 wil update the button
                    if(pMsg->uiEvent == EVENT_MOVE)
                        return (1);

                    // implements some form of debounce
                    // add code here.
                    screenState = CREATE_DEMOSELECTION;
                    return (0);
                }

            case ID_BUTTON_B:
                if(objMsg == BTN_MSG_RELEASED)
                {

                    // do not process if user moved the touch away from the button
                    // returning 1 wil update the button
                    if(pMsg->uiEvent == EVENT_MOVE)
                        return (1);
                }

                if(scale == 1)
                {
                    PictSetScale(pPicture, 2);      // x2
                    scale = 2;
                    ClrState(pPicture, PICT_FRAME); // set redraw state
                }
                else
                {
                    PictSetScale(pPicture, 1);      // x1
                    scale = 1;
                    SetState(pPicture, PICT_FRAME); // set redraw state
                }

                SetState(pPicture, PICT_DRAW);      // set redraw state
                return (1);                         // process by default

            case ID_BUTTON_C:
                if(GetState((BUTTON *)GOLFindObject(ID_BUTTON_C), BTN_PRESSED))
                {
                    value = MtrGetVal(pMeter);      // get current value
                    MtrDecVal(pMeter, 1);
                    SetState(pMeter, MTR_DRAW_UPDATE);
                }

                return (1);

            case ID_BUTTON_D:
                if(GetState((BUTTON *)GOLFindObject(ID_BUTTON_D), BTN_PRESSED))
                {
                    value = MtrGetVal(pMeter);      // get current value
                    MtrIncVal(pMeter, 1);
                    SetState(pMeter, MTR_DRAW_UPDATE);
                }

                return (1);                         // process by default

            default:
                return (1);                         // process by default
        }
    }

    return (1); // process by default
}

/************************************************************************
 Function: void InitAnimationStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void InitAnimationStyleScheme(GOL_SCHEME *pScheme)
{
    pScheme->Color0 = BLACK;
    pScheme->Color1 = WHITE;
    pScheme->TextColor0 = BRIGHTBLUE;
    pScheme->TextColor1 = WHITE;
    pScheme->EmbossDkColor = GRAY20;
    pScheme->EmbossLtColor = GRAY80;
    pScheme->pFont = (void *) &GOLSmallFont;
}
