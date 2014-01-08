/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        AN1182Demo.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 07/20/07	    ...
 * 03/18/11     Ported to common Graphics Application Note Demo
 *****************************************************************************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Graphics/Graphics.h"
#include "AN1182Demo.h"
#include "HelloWorldFonts.h"


/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_BTNA 10                                      // Button 1 ID
#define ID_BTNB 11                                      // Button 2 ID
#define ID_STXT 20                                      // Static text ID
    
    /////////////////////////////////////////////////////////////////////////////

//                              OBJECT DIMENSIONS DEFINES
/////////////////////////////////////////////////////////////////////////////
// static text dimension
#define STXWIDTH    GetMaxX()-40                        // static text width
#define STXHEIGHT   40                                  // static text height
#define STXXPOS     20                                  // static text left/top pos
#define STXYPOS     ((GetMaxY()-STXHEIGHT-ARROWHEIGHT+3) >> 1)

// string select buttons dimensions
#define SELBTNYPOS  STXYPOS + STXHEIGHT + 3             // button left/top pos
#define SELBTNXPOS  STXXPOS
#define ARROWHEIGHT (40 + (GOL_EMBOSS_SIZE * 2))        // button height
#define ARROWWIDTH  (45 + (GOL_EMBOSS_SIZE * 2))        // button width

// slider dimensions
#define SLIDERXPOS      (STXXPOS + ARROWWIDTH)          // slider left/top pos
#define SLIDERYPOS      (STXYPOS + STXHEIGHT + 3)
#define SLIDERWIDTH     (STXWIDTH - (ARROWWIDTH << 1))  // slider width
#define HWDATAMAX       11                               // # of "Hello World" strings
#define HELLOWORLDDELAY 30                              // default animation delay
    
    
    /////////////////////////////////////////////////////////////////////////////

//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void InitHWData (void);                             // initialize string struct arrays
void CreateHelloWorld(void); 						// create the components


/////////////////////////////////////////////////////////////////////////////

//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
extern const IMAGE_FLASH   redLArrow;              // bitmap used for button 1
extern const IMAGE_FLASH   redRArrow;              // bitmap used for button 2


/////////////////////////////////////////////////////////////////////////////

//                            FONTS USED
/////////////////////////////////////////////////////////////////////////////
extern const FONT_FLASH     RussianFont;            // font for Russian translation
extern const FONT_FLASH     ArabicFont;             // font for Arabic translation
extern const FONT_FLASH     ChineseFont;            // font for Chinese translation
extern const FONT_FLASH     JapaneseFont;           // font for Japanese translation
extern const FONT_FLASH     KoreanFont;             // font for Korean translation

// Extended glyphs
extern const FONT_FLASH     HindiFont;              // font for Hindi translation
extern const FONT_FLASH     ThaiFont;               // font for Thai translation

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
//SCREEN_STATES   screenState = SS_CREATE_HELLOWORLD; // current state of main demo state mashine
GOL_SCHEME      *AN1182Scheme;                          // alternative style scheme
GOL_SCHEME      *AN1182Scheme2;                            // alternative style scheme
STATICTEXT      *pSt;                                   // pointer to the static text object
WORD            helloDelay = HELLOWORLDDELAY;

// strings that will use the GOL default font.
const XCHAR     EnglishStr[] = {'H','e','l','l','o',' ','W','o','r','l','d','!',0};
const XCHAR     FrenchStr[] = {'B','o','n','j','o','u','r',' ','M','o','n','d','e','!',0};
const XCHAR     GermanStr[] = {'H','a','l','l','o',' ','W','e','l','t','!',0};
const XCHAR     ItalianStr[] = {'C','i','a','o',' ','M','o','n','d','o','!',0};
const XCHAR     DutchStr[] = {'H','e','l','l','o',' ','W','e','r','e','l','d','!',0};

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

/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateAN1182(void)
// Input: None
// Output: Returns non-zero if the objects are created 
//         returns 0 when one of the objects was not created.
// Overview: Creates the AN1182 Demo screen.
/////////////////////////////////////////////////////////////////////////////
WORD CreateAN1182(void)
{

    // initialize the list of Hello World translation
    InitHWData();

    // set screen to WHITE
    SetColor(WHITE);
    ClearDevice();

    // Create and assign colors to the alternate style scheme
    AN1182Scheme2 = GOLCreateScheme();         // create alternative style scheme
    AN1182Scheme2->TextColor0 = BRIGHTBLUE;    // set the color of text
    AN1182Scheme2->pFont = pHWData->pHWFont;   // initialize the font used for the style scheme
    CreateHelloWorld();
    
    return 1;

}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD MsgAN1182Callback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed 
//         by default
// Overview: This is called by GOLMsgCallback() to determine if 
//           a valid message for objects in this demo needs to be processed.
/////////////////////////////////////////////////////////////////////////////
WORD MsgAN1182Callback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD        objectID;
    STATICTEXT  *pSt;

    objectID = GetObjID(pObj);
    switch(objectID)
    {
        case ID_BTNA:
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                pHWData = pHWData->pHWPrev;                     // adjust global pointer to previous string
                pSt = (STATICTEXT *)GOLFindObject(ID_STXT);     // get pointer to static text
                pSt->hdr.pGolScheme->pFont = pHWData->pHWFont;  // change font used in static text
                StSetText(pSt, pHWData->pHWStr);                // set the new string
                SetState(pSt, ST_DRAW);                         // set redraw state
            }

            break;

        case ID_BTNB:
            if(objMsg == BTN_MSG_RELEASED)
            {
                pHWData = pHWData->pHWNext;                     // adjust global pointer to next string
                pSt = (STATICTEXT *)GOLFindObject(ID_STXT);     // get pointer to static text
                pSt->hdr.pGolScheme->pFont = pHWData->pHWFont;  // change font used in static text
                StSetText(pSt, pHWData->pHWStr);                // set the new string
                SetState(pSt, ST_DRAW);                         // set redraw state
            }

            break;

        default:
            break;
    }

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD DrawAN1182Callback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: GOLDrawCallback() calls this function for demo specific drawing 
//           requirements.
/////////////////////////////////////////////////////////////////////////////
WORD DrawAN1182Callback(void)
{
    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: void CreateHelloWorld()
// Output: none
// Overview: Create the objects that will show the Hello World Demo.
// 			 This is composed of two buttons and a static text.
//			 The two buttons will select the string that will be displayed.

//			 The strings are arranged in a linked list configured as a ring.
/////////////////////////////////////////////////////////////////////////////
void CreateHelloWorld(void)
{
    AN1182Scheme2->pFont = pHWData->pHWFont;

    StCreate
    (
        ID_STXT,
        STXXPOS,
        STXYPOS,
        STXXPOS + STXWIDTH,
        STXYPOS + STXHEIGHT,
        ST_CENTER_ALIGN | ST_DRAW | ST_FRAME,
        pHWData->pHWStr,
        AN1182Scheme2
    );

    BtnCreate
    (
        ID_BTNA,                    // object’s ID
        SELBTNXPOS,
        SELBTNYPOS,
        SELBTNXPOS + ARROWWIDTH,
        SELBTNYPOS + ARROWHEIGHT,   // object’s dimension
        0,                          // radius of the rounded edge
        BTN_DRAW,                   // draw the object after creation
        (void *) &redLArrow,        // bitmap used
        NULL,                       // use this text
        NULL
    );                              // use default style scheme
    BtnCreate
    (
        ID_BTNB,
        SLIDERXPOS + SLIDERWIDTH,
        SLIDERYPOS,
        SLIDERXPOS + SLIDERWIDTH + ARROWWIDTH,
        SLIDERYPOS + ARROWHEIGHT,
        0,
        BTN_DRAW,
        (void *) &redRArrow,
        NULL,
        NULL
    );                              // use default style scheme
}

/////////////////////////////////////////////////////////////////////////////
// Function: void InitHWData()
// Output: none

// Overview: Initialize the ring linked list.
/////////////////////////////////////////////////////////////////////////////
void InitHWData(void)
{
    int    i;

    // Get all the translation of "Hello World" and store them into
    // the list.
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
            case 9:     HWLang[i].pHWFont = (void *) &HindiFont; HWLang[i].pHWStr = (XCHAR *)HindiStr; break;
            case 10:    HWLang[i].pHWFont = (void *) &ThaiFont; HWLang[i].pHWStr = (XCHAR *)ThaiStr; break;
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

