/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
*****************************************************************************
 * FileName:        MultiFontsDemo.c
 * Dependencies:    MultiFontsDemo.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 * Paolo A. Tamayo		07/20/07	... 
 *****************************************************************************/
#include "MainDemo.h"
#include "ResourcesInternalFlash C30.h"

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
// OBJEC IDs
#define ID_BTN1 10                              // Button 1 ID
#define ID_BTN2 11                              // Button 2 ID
#define ID_STXT 20                              // Static text ID

// OBJECT DIMENSIONS DEFINES
// static text dimension
#define STXWIDTH	(GetMaxX()-60)					// static text width
#define STXHEIGHT	(GetMaxY()-CTRLBTN_HEIGHT-60)	// static text height
#define STXXPOS     30                          // static text left/top pos
#define STXYPOS     30



// string select buttons dimensions
#define SELBTNYPOS  STXYPOS + STXHEIGHT + 3     // button left/top pos
#define SELBTNXPOS  STXXPOS
#define ARROWHEIGHT 27 + (GOL_EMBOSS_SIZE * 2)  // button height
#define ARROWWIDTH  30 + (GOL_EMBOSS_SIZE * 2)  // button width

// slider dimensions
#define SLIDERXPOS      STXXPOS + ARROWWIDTH    // slider left/top pos
#define SLIDERYPOS      STXYPOS + STXHEIGHT + 3
#define SLIDERWIDTH     STXWIDTH - (ARROWWIDTH << 1)    // slider width
#define HWDATAMAX       9                               // # of "Hello World" strings
#if defined(__PIC24FJ256DA210__)
	#define HELLOWORLDDELAY 22                              // default animation delay
	#define DELTAMOVE       1
#else
	#define HELLOWORLDDELAY 10                              // default animation delay
	#define DELTAMOVE       1
#endif	

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void    InitHWData(void);                               // initialize string struct arrays
void    AnimateText(BYTE inc);                          // refresh the string function
#define WAIT_UNTIL_FINISH(x)    while(!x)

/////////////////////////////////////////////////////////////////////////////
//                            FONTS USED
/////////////////////////////////////////////////////////////////////////////
// See "ResourcesInternalFlash C30.h" or "ResourcesInternalFlash C32.h" for fonts used.

extern XCHAR                RightArrowStr[];
extern XCHAR                LeftArrowStr[];
extern XCHAR                ExitStr[];

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
WORD                        update = 0;                 // variable to update customized graphics
WORD                        helloDelay = HELLOWORLDDELAY;

const XCHAR                 EnglishStr[] = {'H','e','l','l','o',' ','W','o','r','l','d','!',0};
const XCHAR                 FrenchStr[] = {'B','o','n','j','o','u','r',' ','M','o','n','d','e','!',0};
const XCHAR                 GermanStr[] = {'H','a','l','l','o',' ','W','e','l','t','!',0};
const XCHAR                 ItalianStr[] = {'C','i','a','o',' ','M','o','n','d','o','!',0};
const XCHAR                 DutchStr[] = {'H','e','l','l','o',' ','W','e','r','e','l','d','!',0};

GOL_SCHEME                  *FontScheme2;               // alternative red style scheme

/////////////////////////////////////////////////////////////////////////////

//                       		STRUCTURES
/////////////////////////////////////////////////////////////////////////////

// structure used to rotate around the used fonts and "Hello World" strings	
typedef struct
{
    void    *pHWFont;   // pointer to the font used
    XCHAR   *pHWStr;    // pointer to the string
    void    *pHWPrev;   // pointer to the previous list member
    void    *pHWNext;   // pointer to the next list member
} HWDATA;

// array of structures that will hold the strings and its pointers to corresponding font tables.
// this will be configured as a ringed linked list
HWDATA  HWLang[HWDATAMAX];

// global pointer to the linked list.
HWDATA  *pHWData;

/************************************************************************
 Function: void CreateMultiFontsDemo(void)
                                                                       
 Overview: Creates the Multi-Font demo screen.
  		                                         
 Input: none
                                                                       
 Output: none
************************************************************************/
void CreateMultiFontsDemo(void)
{
    SHORT           i, j;

    // we need this since Static cannot have an uninitialized pointer to text
    // plus we need the static text area but we are only using it as a place
    // holder for the text that we will overwrite on the static text area
    static XCHAR    spaceChar[2] = {0x20,0};

    GOLFree();                          // free memory for the objects in the previous linked list and start new list
    SetColor(SCREEN_BACKGROUND_COLOR);
    ClearDevice();

    // draw the small squares
    SetColor(RGB565CONVERT(0x4C, 0x8E, 0xFF));
    for(j = 5; j <= GetMaxY(); j += 16)
    {
        for(i = 5; i <= GetMaxX(); i += 16)
        {
            WAIT_UNTIL_FINISH(Bar(i, j, i + 3, j + 3));
        }
    }

    InitHWData();

    FontScheme2 = GOLCreateScheme();    // create alternative style scheme
    FontScheme2->pFont = pHWData->pHWFont;
    FontScheme2->Color0 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    FontScheme2->Color1 = RGB565CONVERT(0xFF, 0xBB, 0x4C);

    StCreate
    (
        ID_STXT,
        STXXPOS,
        STXYPOS,
        STXXPOS + STXWIDTH,
        STXYPOS + STXHEIGHT,
        ST_DRAW | ST_FRAME,
        (void *) &spaceChar,
        FontScheme2
    );

    CreateCtrlButtons(ExitStr, LeftArrowStr, NULL, RightArrowStr);
}

/************************************************************************
 Function: void AnimateText(BYTE mov)
                                                                       
 Overview: Routine to move or animate the text.
  		                                         
 Input: mov - number of pixels the text will be moved
                                                                       
 Output: none
************************************************************************/
void AnimateText(BYTE mov)
{
    static SHORT    xPos = STXXPOS, yPos = STXYPOS;
    static SHORT    x, y;
    SHORT           width;
    SHORT           height;

    SHORT           newX, newY, oldX, oldY;
    XCHAR           NewChar, *pString;

    // set the clipping region
    SetClip(CLIP_ENABLE);
    SetClipRgn(STXXPOS + 2, STXYPOS + 2, STXXPOS + STXWIDTH - 2, STXYPOS + STXHEIGHT - 2);

    // set the font
    SetFont(pHWData->pHWFont);

    // calculate string width & height	
    width = GetTextWidth((XCHAR *)pHWData->pHWStr, pHWData->pHWFont);
    height = GetTextHeight(pHWData->pHWFont);

    //-----------------------------------------------------------------
    // interlace the erasing and printing of characters
    // check first if we need to move in the positive or negative direction
    if((xPos + width) >= (STXXPOS + STXWIDTH))
        x = -(mov);
    if(xPos <= (STXXPOS))
        x = (mov);

    if((yPos + height) >= (STXYPOS + STXHEIGHT))
        y = -(mov);
    if(yPos <= (STXYPOS))
        y = (mov);

    pString = pHWData->pHWStr;
    oldX = xPos;
    oldY = yPos;
    newX = xPos + x;
    newY = yPos + y;
    while((XCHAR)15 < (XCHAR)(NewChar = *pString++))
    {

        // remove the old position of the character
        SetColor(FontScheme2->CommonBkColor);
        MoveTo(oldX, oldY);
        WAIT_UNTIL_FINISH(OutChar(NewChar));
        oldX = GetX();
        oldY = GetY();

        // display the character in the new position
        SetColor(BRIGHTBLUE);
        MoveTo(newX, newY);
        WAIT_UNTIL_FINISH(OutChar(NewChar));
        newX = GetX();
        newY = GetY();
    }

    xPos += x;
    yPos += y;

    // disable the clipping
    SetClip(CLIP_DISABLE);
}

/************************************************************************
 Function: WORD MultiFontDemoMsgCallback(WORD objMsg, OBJ_HEADER* pObj, 
 										GOL_MSG* pMsg)

 Overview: This function is called by GOLMsg() function when in this 
 		   particular demo each time the valid message is received for the 
  		   objects in the demo.
           
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 

 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD MultiFontDemoMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD        objectID;
    STATICTEXT  *pSt;

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
                free(FontScheme2);
                screenState = CREATE_DEMOSELECTION;
                return (1);
            }

        case ID_BUTTON_B:
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);

                pHWData = pHWData->pHWPrev;
                pSt = (STATICTEXT *)GOLFindObject(ID_STXT); // get pointer to static text
                SetState(pSt, ST_DRAW);                     // set redraw state
            }

            break;

        case ID_BUTTON_D:
            if(objMsg == BTN_MSG_RELEASED)
            {

                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);

                pHWData = pHWData->pHWNext;
                pSt = (STATICTEXT *)GOLFindObject(ID_STXT); // get pointer to static text
                SetState(pSt, ST_DRAW);                     // set redraw state
            }

            break;

        default:
            break;
    }

    return (1);
}

/************************************************************************
 Function: WORD MultiFontDemoDrawCallback(void)
 
 Overview: This function is called by GOLDraw() function when the state 
 		   is in this particular demo.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD MultiFontDemoDrawCallback(DWORD tick)
{
    static DWORD    prevTick = 0;   // keeps previous value of tick
    if((tick - prevTick) > helloDelay)
    {
        AnimateText(DELTAMOVE);
        prevTick = tick;
    }

    return (1);
}

/* */
void InitHWData(void)
{
    int i;

    for(i = 0; i < HWDATAMAX; i++)
    {
        switch(i)
        {
            case 0:     HWLang[i].pHWFont = (void *) &GOLFontDefault; HWLang[i].pHWStr = (XCHAR *)EnglishStr; break;
            case 1:     HWLang[i].pHWFont = (void *) &ChineseFont; HWLang[i].pHWStr = (XCHAR *)ChineseStr; break;
            case 2:     HWLang[i].pHWFont = (void *) &JapaneseFont; HWLang[i].pHWStr = (XCHAR *)JapaneseStr; break;
            case 3:     HWLang[i].pHWFont = (void *) &GOLFontDefault; HWLang[i].pHWStr = (XCHAR *)ItalianStr; break;
            case 4:     HWLang[i].pHWFont = (void *) &RussianFont; HWLang[i].pHWStr = (XCHAR *)RussianStr; break;
            case 5:     HWLang[i].pHWFont = (void *) &GOLFontDefault; HWLang[i].pHWStr = (XCHAR *)GermanStr; break;
            case 6:     HWLang[i].pHWFont = (void *) &GOLFontDefault; HWLang[i].pHWStr = (XCHAR *)DutchStr; break;
            case 7:     HWLang[i].pHWFont = (void *) &GOLFontDefault; HWLang[i].pHWStr = (XCHAR *)FrenchStr; break;
            case 8:     HWLang[i].pHWFont = (void *) &KoreanFont; HWLang[i].pHWStr = (XCHAR *)KoreanStr; break;
            default:    break;
        }

        // make the list a ring list
        if(i == (HWDATAMAX - 1))
        {
            HWLang[i].pHWNext = (void *) &HWLang[0];
            HWLang[i].pHWPrev = (void *) &HWLang[i - 1];
        }
        else if(i == 0)
        {
            HWLang[i].pHWNext = (void *) &HWLang[i + 1];
            HWLang[i].pHWPrev = (void *) &HWLang[HWDATAMAX - 1];
        }
        else
        {
            HWLang[i].pHWNext = (void *) &HWLang[i + 1];
            HWLang[i].pHWPrev = (void *) &HWLang[i - 1];
        }
    }

    pHWData = &HWLang[0];
}
