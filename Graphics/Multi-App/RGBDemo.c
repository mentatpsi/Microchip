/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
*****************************************************************************
 * FileName:        RGBDemo.c
 * Dependencies:    RGBDemo.h
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/10/08	    ... 
 * 06/24/09     Added checks for accelerated primitives.
 * 06/30/09	    Added implementation of detecting continuous
 *              touch screen press on a button on left &
 *				right buttons that control the RED value.
 * 04/30/10	    Fixed problem on polypoints when at the 
 *				edge of the screen.
 * 05/31/11     removed USB HID implementation
 * 10/13/11     Edit Boxes should be created with no EB_CARET state set.
 *****************************************************************************/
#include "MainDemo.h"

/////////////////////////////////////////////////////////////////////////////
//                              BEHAVIOR CONTROLS
/////////////////////////////////////////////////////////////////////////////
//#define DISABLECOLORSLIDER

/////////////////////////////////////////////////////////////////////////////
//                       STRINGS USED
/////////////////////////////////////////////////////////////////////////////
// strings that will use the GOL default font.
extern XCHAR    RightArrowStr[];
extern XCHAR    LeftArrowStr[];
extern XCHAR    ExitStr[];
extern XCHAR    MemoryErrorStr[];
const XCHAR     OneStr[] = {'1',0};
const XCHAR     TwoStr[] = {'2',0};
const XCHAR     FadeStr[] = {'F','a','d','e',0};

const XCHAR     HIDHardwareMsgStr[] = {'D','e','t','e','c','t','i','n','g',' ','H','I','D',' ','D','e','v','i','c','e',0};
const XCHAR     NoHIDHardwareMsgStr[] = {'N','o',' ','H','I','D',' ','D','e','v','i','c','e',' ','D','e','t','e','c','t','e','d',0};

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void            Int2Str(XCHAR *pStr, WORD value, SHORT charCount);  // convert integer to string
WORD            DrawColorPalette(void);
void            CalculatePaletteColor(WORD *red, WORD *green, WORD *blue, WORD pos);
WORD            ShowCurrentColor(void);
void            FadeColorControl(void);
void            DrawColorPointer(void);
void            UpdateColorPointer(SHORT center);
WORD            DrawIntensityLevel(WORD drawType);

#define WAIT_UNTIL_FINISH(x)    while(!x)

    /////////////////////////////////////////////////////////////////////////////
    //                            IMAGES USED
    /////////////////////////////////////////////////////////////////////////////
    //extern const BITMAP_FLASH bulb;
    
    /////////////////////////////////////////////////////////////////////////////
    //                            FONTS USED
    /////////////////////////////////////////////////////////////////////////////
    
    /////////////////////////////////////////////////////////////////////////////
    //                            STRUCTURES
    /////////////////////////////////////////////////////////////////////////////
    // RGB states
    typedef enum
{
    RGB_NORMAL_ST   = 0,
    RGB_PRESET_ST,
} RGB_STATES;

// states for the triangle drawn on the pallette
typedef enum
{
    CPTR_HIDE_ST    = 0x8001,           // the triangular color pointer will be hidden
    CPTR_SHOW_ST    = 0x8002,           // color pointer will be refreshed or shown
    CPTR_UPDATE_ST  = 0x8003,           // color pointer will be relocated
} CPTR_STATES;

   
/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////
	extern DWORD    tick;                   // tick counter
	
	// RGB demo states
	RGB_STATES      rgbState = RGB_NORMAL_ST;
	CPTR_STATES     cptrState = CPTR_HIDE_ST;

	// color maximum and minimum values
	BYTE            RedMax, GreenMax, BlueMax, UpdateColor, UpdateCurrent;
	BYTE            RedMin, GreenMin, BlueMin;
	
	// Preset Color Globals
	BYTE            Pres1Red = 0x4C;
	BYTE            Pres1Green = 0x8E;
	BYTE            Pres1Blue = 0xFF;
	
	BYTE            Pres2Red = 0xFF;
	BYTE            Pres2Green = 0xBB;
	BYTE            Pres2Blue = 0x4C;
	
	BYTE            RedLim;
	BYTE            GreenLim;
	BYTE            BlueLim;
	
	// RED Component Values
	WORD            RedRange = REDMAX;
	WORD            RedPage = 50;
	WORD            RedPos = 0x4C;
	
	// GREEN Component Values
	WORD            GreenRange = GREENMAX;
	WORD            GreenPage = 50;
	WORD            GreenPos = 0x8E;
	
	// Blue Component Values
	WORD            BlueRange = BLUEMAX;
	WORD            BluePage = 50;
	WORD            BluePos = 0xFF;
	
	// Intensity Component
	WORD            Intensity = 100;
	
	// fade animation enable flag
	BYTE            FadeModeEnable;
	
	// fading delta change (MAX IS 127)
	CHAR            FadeDirRed, FadeDirGreen, FadeDirBlue;
	
	// delta change variables when animating color fading
	BYTE            deltaR, deltaG, deltaB;
	
	// intensity global flag update
	BYTE            updateIntensity;
	
	// arrow points (the first 8 are the current values, the next 8 are the new values
	SHORT           PolyPoints[16];
	
	// this is used to increment or decrement color values
	// when button is held pressed
	DWORD           TickDiff = DEFAULTTICKDIFF;
	DWORD           PtrTickDiff = COLORPOINTERDELAY;
	
	GOL_SCHEME      *RGBRedScheme;          // alternative red style scheme
	GOL_SCHEME      *RGBGreenScheme;        // alternative green style scheme
	GOL_SCHEME      *RGBBlueScheme;         // alternative blue style scheme
	GOL_SCHEME      *Preset1ColorScheme;    // color scheme for the preset 1 button
	GOL_SCHEME      *Preset2ColorScheme;    // color scheme for the preset 2 button
	GOL_SCHEME      *ControlColorScheme;    // color scheme for the control buttons
	GOL_SCHEME      *LabelColorScheme;      // color scheme for labels

    // buffers for the edit boxes
    XCHAR    redValue[4], greenValue[4], blueValue[4];


/************************************************************************
 Function: WORD CreateRGBDemo(void)
                                                                       
 Overview: Creates the RGB Mood Lighting demo screen.
  		                                         
 Input: none
                                                                       
 Output: none.
************************************************************************/
void CreateRGBDemo(void)
{
    BUTTON          *pObj;
    OBJ_HEADER      *pGenObj;

    GOLFree();                          // free memory for the objects in the previous linked list and start new list

    SetColor(RGBBACKGROUND);
    ClearDevice();

    // initialize the state to normal state
    rgbState = RGB_NORMAL_ST;

    RedMax = REDMAX;
    GreenMax = GREENMAX;
    BlueMax = BLUEMAX;

    RedMin = 0;
    GreenMin = 0;
    BlueMin = 0;

    UpdateColor = TRUE;
    UpdateCurrent = TRUE;
    FadeModeEnable = FALSE;

    // create the style schemes

    // create red style scheme
    if ((RGBRedScheme = GOLCreateScheme()) == NULL)
        ShowError(MemoryErrorStr);
    // create green style scheme
    if ((RGBGreenScheme = GOLCreateScheme()) == NULL)
        ShowError(MemoryErrorStr);
    // create blue style scheme
    if ((RGBBlueScheme = GOLCreateScheme()) == NULL)
        ShowError(MemoryErrorStr);

    // create style scheme for the preset colors and fade control buttons
    if ((Preset1ColorScheme = GOLCreateScheme()) == NULL)
        ShowError(MemoryErrorStr);
    if ((Preset2ColorScheme = GOLCreateScheme()) == NULL)
        ShowError(MemoryErrorStr);
    if ((ControlColorScheme = GOLCreateScheme()) == NULL)
        ShowError(MemoryErrorStr);
    if ((LabelColorScheme = GOLCreateScheme()) == NULL)
        ShowError(MemoryErrorStr);

    RGBRedScheme->Color0 = RGB565CONVERT(0xCC, 0x00, 0x00);
    RGBRedScheme->Color1 = BRIGHTRED;
    RGBRedScheme->EmbossDkColor = RGB565CONVERT(139, 0, 0);
    RGBRedScheme->EmbossLtColor = RGB565CONVERT(255, 48, 48);
    RGBRedScheme->TextColor0 = RGB565CONVERT(0xC8, 0xD5, 0x85);
    RGBRedScheme->TextColor1 = RGB565CONVERT(0xCC, 0x00, 0x00);

    RGBGreenScheme->Color0 = RGB565CONVERT(0x23, 0x9E, 0x0A);
    RGBGreenScheme->Color1 = BRIGHTGREEN;
    RGBGreenScheme->EmbossDkColor = RGB565CONVERT(0, 100, 0);
    RGBGreenScheme->EmbossLtColor = RGB565CONVERT(152, 251, 152);
    RGBGreenScheme->TextColor0 = RGB565CONVERT(0xDF, 0xAC, 0x83);
    RGBGreenScheme->TextColor1 = RGB565CONVERT(0x23, 0x9E, 0x0A);

    RGBBlueScheme->Color0 = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    RGBBlueScheme->Color1 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    RGBBlueScheme->EmbossDkColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
    RGBBlueScheme->EmbossLtColor = RGB565CONVERT(0xA9, 0xDB, 0xEF);
    RGBBlueScheme->TextColor0 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    RGBBlueScheme->TextColor1 = BRIGHTBLUE;

    ControlColorScheme->Color0 = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    ControlColorScheme->Color1 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    ControlColorScheme->EmbossDkColor = RGB565CONVERT(0xD4, 0xED, 0xF7);
    ControlColorScheme->EmbossLtColor = RGB565CONVERT(0xD4, 0xED, 0xF7);
    ControlColorScheme->TextColor0 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    ControlColorScheme->TextColor1 = BRIGHTBLUE;

    Preset1ColorScheme->Color0 = RGB565CONVERT(Pres1Red, Pres1Green, Pres1Blue);
    Preset1ColorScheme->Color1 = RGB565CONVERT(Pres1Red, Pres1Green, Pres1Blue);
    Preset1ColorScheme->EmbossDkColor = RGB565CONVERT(0xD4, 0xED, 0xF7);
    Preset1ColorScheme->EmbossLtColor = RGB565CONVERT(0xD4, 0xED, 0xF7);
    Preset1ColorScheme->TextColor0 = RGB565CONVERT((REDMAX - 0x4C), (GREENMAX - 0x8E), (BLUEMAX - 0xFF));
    Preset1ColorScheme->TextColor1 = RGB565CONVERT((REDMAX - 0x4C), (GREENMAX - 0x8E), (BLUEMAX - 0xFF));

    Preset2ColorScheme->Color0 = RGB565CONVERT(Pres2Red, Pres2Green, Pres2Blue);
    Preset2ColorScheme->Color1 = RGB565CONVERT(Pres2Red, Pres2Green, Pres2Blue);
    Preset2ColorScheme->EmbossDkColor = RGB565CONVERT(0xD4, 0xED, 0xF7);
    Preset2ColorScheme->EmbossLtColor = RGB565CONVERT(0xD4, 0xED, 0xF7);
    Preset2ColorScheme->TextColor0 = RGB565CONVERT((REDMAX - 0xFF), (GREENMAX - 0xBB), (BLUEMAX - 0x4C));
    Preset2ColorScheme->TextColor1 = RGB565CONVERT((REDMAX - 0xFF), (GREENMAX - 0xBB), (BLUEMAX - 0x4C));

    // initialize the polygon points
    UpdateColorPointer(0);

    // set intensity drawing flag
    updateIntensity = TRUE;

    // now create the objects for the RGB demo
    pGenObj = (OBJ_HEADER *)StCreate
    (
        ID_HIDDENSTXT,
        PLTXPOS,
        PLTYPOS,
        PLTXPOS + PLTWIDTH,
        PLTYPOS + PLTHEIGHT,
        0,                              // do not draw since this will be
        NULL,                           // covered by the palette
        NULL
    );
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);

    pGenObj = (OBJ_HEADER *)BtnCreate
    (
        ID_UPBTN1,                      // object’s ID
        BTNUPXPOS,
        BTN1UPYPOS,
        BTNUPXPOS + BTNWIDTH,
        BTN1UPYPOS + BTNHEIGHT,         // object’s dimension
        0,                              // radius of the rounded edge
        BTN_DRAW,                       // draw the object after creation
        NULL,                           // bitmap used
        RightArrowStr,                  // use this text
        RGBRedScheme
    );                              // use red style scheme
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);

    pGenObj = (OBJ_HEADER *)BtnCreate
    (
        ID_UPBTN2,                  // object’s ID
        BTNUPXPOS,
        BTN2UPYPOS,
        BTNUPXPOS + BTNWIDTH,
        BTN2UPYPOS + BTNHEIGHT,     // object’s dimension
        0,                          // radius of the rounded edge
        BTN_DRAW,                   // draw the object after creation
        NULL,                       // bitmap used
        RightArrowStr,              // use this text
        RGBGreenScheme
    );
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);
                              // use green style scheme
    pGenObj = (OBJ_HEADER *)BtnCreate
    (
        ID_UPBTN3,                  // object’s ID
        BTNUPXPOS,
        BTN3UPYPOS,
        BTNUPXPOS + BTNWIDTH,
        BTN3UPYPOS + BTNHEIGHT,     // object’s dimension
        0,                          // radius of the rounded edge
        BTN_DRAW,                   // draw the object after creation
        NULL,                       // bitmap used
        RightArrowStr,              // use this text
        RGBBlueScheme
    );    
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);
                          // use blue style scheme
    pGenObj = (OBJ_HEADER *)SldCreate
    (
        ID_SLD1,                    // object’s ID
        SLDXPOS,
        SLD1YPOS,
        SLDXPOS + SLDWIDTH,
        SLD1YPOS + SLDHEIGHT,       // object’s dimension
        SLD_DRAW | SLD_SCROLLBAR,   // will be dislayed after creation
        RedMax,                     // range
        RedPage,                    // page
        RedPos,                     // pos
        RGBRedScheme
    );
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);

    pGenObj = (OBJ_HEADER *)SldCreate
    (
        ID_SLD2,                    // object’s ID
        SLDXPOS,
        SLD2YPOS,
        SLDXPOS + SLDWIDTH,
        SLD2YPOS + SLDHEIGHT,       // object’s dimension
        SLD_DRAW | SLD_SCROLLBAR,   // will be dislayed after creation
        GreenMax,                   // range
        GreenPage,                  // page
        GreenPos,                   // pos
        RGBGreenScheme
    );
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);

    pGenObj = (OBJ_HEADER *)SldCreate
    (
        ID_SLD3,                    // object’s ID
        SLDXPOS,
        SLD3YPOS,
        SLDXPOS + SLDWIDTH,
        SLD3YPOS + SLDHEIGHT,       // object’s dimension
        SLD_DRAW | SLD_SCROLLBAR,   // will be dislayed after creation
        BlueMax,                    // range
        BluePage,                   // page
        BluePos,                    // pos
        RGBBlueScheme
    );
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);

    pGenObj = (OBJ_HEADER *)BtnCreate
    (
        ID_DNBTN1,                  // object’s ID
        BTNDNXPOS,
        BTN1DNYPOS,
        BTNDNXPOS + BTNWIDTH,
        BTN1DNYPOS + BTNHEIGHT,     // object’s dimension
        0,                          // radius of the rounded edge
        BTN_DRAW,                   // draw the object after creation
        NULL,                       // bitmap used
        LeftArrowStr,               // use this text
        RGBRedScheme
    );  
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);
                            // use red style scheme
    pGenObj = (OBJ_HEADER *)BtnCreate
    (
        ID_DNBTN2,                  // object’s ID
        BTNDNXPOS,
        BTN2DNYPOS,
        BTNDNXPOS + BTNWIDTH,
        BTN2DNYPOS + BTNHEIGHT,     // object’s dimension
        0,                          // radius of the rounded edge
        BTN_DRAW,                   // draw the object after creation
        NULL,                       // bitmap used
        LeftArrowStr,               // use this text
        RGBGreenScheme
    );                              // use green style scheme
    pGenObj = (OBJ_HEADER *)BtnCreate
    (
        ID_DNBTN3,                  // object’s ID
        BTNDNXPOS,
        BTN3DNYPOS,
        BTNDNXPOS + BTNWIDTH,
        BTN3DNYPOS + BTNHEIGHT,     // object’s dimension
        0,                          // radius of the rounded edge
        BTN_DRAW,                   // draw the object after creation
        NULL,                       // bitmap used
        LeftArrowStr,               // use this text
        RGBBlueScheme
    );                              // use blue style scheme
    Int2Str(redValue, RedPos, 3);
    Int2Str(greenValue, GreenPos, 3);
    Int2Str(blueValue, BluePos, 3);

    pGenObj = (OBJ_HEADER *)EbCreate
    (
        ID_EB1,                     // ID
        EBXPOS,
        EB1YPOS,
        EBXPOS + EBWIDTH,
        EB1YPOS + EBHEIGHT,         // dimension
        EB_DRAW | EB_CENTER_ALIGN,   // will be dislayed after creation
        redValue,
        MAXCHARSIZE,
        RGBRedScheme
    );

    pGenObj = (OBJ_HEADER *)EbCreate
    (
        ID_EB2,             // ID
        EBXPOS,
        EB2YPOS,
        EBXPOS + EBWIDTH,
        EB2YPOS + EBHEIGHT, // dimension
        EB_DRAW | EB_CENTER_ALIGN,   // will be dislayed after creation
        greenValue,
        MAXCHARSIZE,
        RGBGreenScheme
    );
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);

    pGenObj = (OBJ_HEADER *)EbCreate
    (
        ID_EB3,             // ID
        EBXPOS,
        EB3YPOS,
        EBXPOS + EBWIDTH,
        EB3YPOS + EBHEIGHT, // dimension
        EB_DRAW | EB_CENTER_ALIGN,   // will be dislayed after creation
        blueValue,
        MAXCHARSIZE,
        RGBBlueScheme
    );
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);

    // this object is hidden. It controls the light intensity bars on the
    // right side of the screen (bars that changes colors from GREEN to YELLOW to RED)
    pGenObj = (OBJ_HEADER *)SldCreate
    (
        ID_SLD4,                                // object’s ID
        SLD4XPOS,
        SLD4YPOS,
        SLD4XPOS + SLD4WIDTH,
        SLD4YPOS + SLD4HEIGHT,                  // object’s dimension
        SLD_VERTICAL,                           // DO NOT show
        REDMAX,     // range
        1,          // page
        Intensity,  // pos
        NULL
    );
    if (pGenObj == NULL)
        ShowError(MemoryErrorStr);

    CreateCtrlButtons((XCHAR *)ExitStr, (XCHAR *)OneStr, (XCHAR *)TwoStr, (XCHAR *)FadeStr);

    // change the style schemes and the behavior of the control buttons
    pObj = (BUTTON *)GOLFindObject(ID_BUTTON_B);
    pObj->hdr.pGolScheme = Preset1ColorScheme;
    pObj = (BUTTON *)GOLFindObject(ID_BUTTON_C);
    pObj->hdr.pGolScheme = Preset2ColorScheme;
    pObj = (BUTTON *)GOLFindObject(ID_BUTTON_D);
    pObj->hdr.pGolScheme = ControlColorScheme;
    SetState(pObj, BTN_DRAW | BTN_TOGGLE);
}

/************************************************************************
 Function: WORD RGBDemoMsgCallback(WORD objMsg, OBJ_HEADER* pObj, 
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
WORD RGBDemoMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static DWORD    prevTick = 0;           // keeps previous value of tick
    EDITBOX         *pEb = NULL;
    SLIDER          *pSld = NULL;
    WORD            objectID;

    SHORT           temp;

    objectID = GetObjID(pObj);

    switch(objectID)
    {
        case ID_BUTTON_A:
            if(objMsg == BTN_MSG_RELEASED)
            {                               // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);

                // free all the created style schemes
                free(RGBRedScheme);
                free(RGBGreenScheme);
                free(RGBBlueScheme);
                free(Preset1ColorScheme);
                free(Preset2ColorScheme);
                free(ControlColorScheme);
                free(LabelColorScheme);
                screenState = CREATE_DEMOSELECTION;
            }

            return (1);

        // palette touch by the user
        case ID_HIDDENSTXT:
            if(objMsg == ST_MSG_SELECTED)
            {
                UpdateColorPointer(pMsg->param1);
                CalculatePaletteColor(&RedPos, &GreenPos, &BluePos, pMsg->param1 - PLTXPOS);

                SldSetPos((SLIDER *)GOLFindObject(ID_SLD1), RedPos);
                SldSetPos((SLIDER *)GOLFindObject(ID_SLD2), GreenPos);
                SldSetPos((SLIDER *)GOLFindObject(ID_SLD3), BluePos);
                SetState((SLIDER *)GOLFindObject(ID_SLD1), DRAW);
                SetState((SLIDER *)GOLFindObject(ID_SLD2), DRAW);
                SetState((SLIDER *)GOLFindObject(ID_SLD3), DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB1);
                Int2Str(EbGetText(pEb), RedPos, 3);
                SetState(pEb, EB_DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB2);
                Int2Str(EbGetText(pEb), GreenPos, 3);
                SetState(pEb, EB_DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB3);
                Int2Str(EbGetText(pEb), BluePos, 3);
                SetState(pEb, EB_DRAW);

                cptrState = CPTR_UPDATE_ST; // update the color pointer
                PtrTickDiff = tick + COLORPOINTERDELAY;
                UpdateCurrent = TRUE;

                // do not process messages for static text. we have
                // already processed it here
                return (0);
            }

            return (1);

        // ignore all slider touch screen control, sliders are only
        // controlled using the buttons.
        case ID_SLD1:
        case ID_SLD2:
        case ID_SLD3:
            #ifdef DISABLECOLORSLIDER
            return (0);
            #else
            if(FadeModeEnable == TRUE)
                return (0);
            if((objMsg == SLD_MSG_DEC) || (objMsg == SLD_MSG_INC))
            {

                // check if slider moved
                if(objectID == ID_SLD1)
                {
                    pEb = (EDITBOX *)GOLFindObject(ID_EB1);
                    RedPos = SldGetPos((SLIDER *)pObj);
                    Int2Str(EbGetText(pEb), RedPos, 3);
                }

                if(objectID == ID_SLD2)
                {
                    pEb = (EDITBOX *)GOLFindObject(ID_EB2);
                    GreenPos = SldGetPos((SLIDER *)pObj);
                    Int2Str(EbGetText(pEb), GreenPos, 3);
                }

                if(objectID == ID_SLD3)
                {
                    pEb = (EDITBOX *)GOLFindObject(ID_EB3);
                    BluePos = SldGetPos((SLIDER *)pObj);
                    Int2Str(EbGetText(pEb), BluePos, 3);
                }

                SetState(pEb, EB_DRAW);
                cptrState = CPTR_HIDE_ST;   // hide the color pointer
                UpdateCurrent = TRUE;
            }

            return (1);
            #endif // DISABLECOLORSLIDER			

        case ID_DNBTN1:
        case ID_UPBTN1:
            if(FadeModeEnable == TRUE)
                return (0);
            if(objMsg == BTN_MSG_RELEASED)
            {                               // check if button is pressed
                pSld = (SLIDER *)GOLFindObject(ID_SLD1);    // get slider 1 pointer
                if(objectID == ID_UPBTN1)
                    SldSetPos(pSld, SldGetPos(pSld) + 1);
                else
                    SldSetPos(pSld, SldGetPos(pSld) - 1);
                SetState(pSld, SLD_DRAW_THUMB);
                pEb = (EDITBOX *)GOLFindObject(ID_EB1);
                RedPos = SldGetPos(pSld);
                Int2Str(EbGetText(pEb), RedPos, 3);
                SetState(pEb, EB_DRAW);
                TickDiff = DEFAULTTICKDIFF;
                prevTick = tick;
                cptrState = CPTR_HIDE_ST;                   // hide the color pointer
                UpdateCurrent = TRUE;
            }

            if(objMsg == BTN_MSG_PRESSED)
            {   // check if button is pressed
                TickDiff = DEFAULTTICKDIFF;
                prevTick = tick;
            }

            // This is an example to show how to use message callback to
            // detect continuous button press	
            if(objMsg == BTN_MSG_STILLPRESSED)
            {   // check if button is still pressed
                if((tick - prevTick) > TickDiff)
                {
                    if(TickDiff > MINTICKDELAY)
                        TickDiff -= CHANGEDELAY;

                    pSld = (SLIDER *)GOLFindObject(ID_SLD1);
                    temp = SldGetPos(pSld);
                    if(objectID == ID_UPBTN1)
                    {
                        if((++temp) > REDMAX)
                            RedPos = REDMAX;
                        else
                            RedPos = temp;
                    }
                    else
                    {
                        if((--temp) < 0)
                            RedPos = 0;
                        else
                            RedPos = temp;
                    }

                    SldSetPos(pSld, RedPos);
                    SetState(pSld, SLD_DRAW_THUMB);

                    pEb = (EDITBOX *)GOLFindObject(ID_EB1);
                    Int2Str(EbGetText(pEb), RedPos, 3);
                    SetState(pEb, EB_DRAW);
                    cptrState = CPTR_HIDE_ST;   // hide the color pointer
                    prevTick = tick;
                }
            }

            return (1);

        case ID_DNBTN2:
        case ID_UPBTN2:
            if(FadeModeEnable == TRUE)
                return (0);
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                pSld = (SLIDER *)GOLFindObject(ID_SLD2);    // get slider 2 pointer
                if(objectID == ID_UPBTN2)
                    SldSetPos(pSld, SldGetPos(pSld) + 1);
                else
                    SldSetPos(pSld, SldGetPos(pSld) - 1);
                SetState(pSld, SLD_DRAW_THUMB);
                pEb = (EDITBOX *)GOLFindObject(ID_EB2);
                GreenPos = SldGetPos(pSld);
                Int2Str(EbGetText(pEb), GreenPos, 3);
                SetState(pEb, EB_DRAW);
                TickDiff = DEFAULTTICKDIFF;
                cptrState = CPTR_HIDE_ST;                   // hide the color pointer
                UpdateCurrent = TRUE;
            }

            return (1);

        case ID_DNBTN3:
        case ID_UPBTN3:
            if(FadeModeEnable == TRUE)
                return (0);
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                pSld = (SLIDER *)GOLFindObject(ID_SLD3);    // get slider 3 pointer
                if(objectID == ID_UPBTN3)
                    SldSetPos(pSld, SldGetPos(pSld) + 1);
                else
                    SldSetPos(pSld, SldGetPos(pSld) - 1);
                SetState(pSld, SLD_DRAW_THUMB);
                pEb = (EDITBOX *)GOLFindObject(ID_EB3);
                BluePos = SldGetPos(pSld);
                Int2Str(EbGetText(pEb), BluePos, 3);
                SetState(pEb, EB_DRAW);
                TickDiff = DEFAULTTICKDIFF;
                cptrState = CPTR_HIDE_ST;                   // hide the color pointer
                UpdateCurrent = TRUE;
            }

            return (1);

        case ID_BUTTON_B:                   
        case ID_BUTTON_C:                   
            if(FadeModeEnable == TRUE)
                return (0);
            if(objMsg == BTN_MSG_RELEASED)
            {                               // check if button is released
                if(objectID == ID_BUTTON_B)
                {                           
                    RedPos = Pres1Red;
                    GreenPos = Pres1Green;
                    BluePos = Pres1Blue;
                }
                else
                {
                    RedPos = Pres2Red;
                    GreenPos = Pres2Green;
                    BluePos = Pres2Blue;
                }

                SldSetPos((SLIDER *)GOLFindObject(ID_SLD1), RedPos);
                SldSetPos((SLIDER *)GOLFindObject(ID_SLD2), GreenPos);
                SldSetPos((SLIDER *)GOLFindObject(ID_SLD3), BluePos);
                SetState((SLIDER *)GOLFindObject(ID_SLD1), DRAW);
                SetState((SLIDER *)GOLFindObject(ID_SLD2), DRAW);
                SetState((SLIDER *)GOLFindObject(ID_SLD3), DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB1);
                Int2Str(EbGetText(pEb), RedPos, 3);
                SetState(pEb, EB_DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB2);
                Int2Str(EbGetText(pEb), GreenPos, 3);
                SetState(pEb, EB_DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB3);
                Int2Str(EbGetText(pEb), BluePos, 3);
                SetState(pEb, EB_DRAW);

                TickDiff = DEFAULTTICKDIFF;
                cptrState = CPTR_HIDE_ST;   // hide the color pointer
                UpdateCurrent = TRUE;
            }

            // check if button is being pressed
            if((objMsg == BTN_MSG_PRESSED) && (!GetState(pObj, BTN_PRESSED)))
            {
                TickDiff = PRESETTICKDELAY;
            }

            return (1);

        case ID_BUTTON_D:                   //ID_FADEBTN:
            // when fade button is pressed, the colors are animated to go from preset color 1
            // to preset color 2 and back with a timed delay. When this is enabled, the controls
            // on RGB sliders are turned off and preset selection are disabled.
            if((objMsg == BTN_MSG_PRESSED) && (!GetState(pObj, BTN_PRESSED)))
            {

                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);

                FadeModeEnable = TRUE;

                RedLim = Pres1Red;
                GreenLim = Pres1Green;
                BlueLim = Pres1Blue;

                FadeDirRed = (RedLim > RedPos) ? (FADECOLORDELTA) : (-FADECOLORDELTA);
                FadeDirGreen = (GreenLim > GreenPos) ? (FADECOLORDELTA) : (-FADECOLORDELTA);
                FadeDirBlue = (BlueLim > BluePos) ? (FADECOLORDELTA) : (-FADECOLORDELTA);

                TickDiff = FADECOLORDELAY;
            }
            else
            {
                FadeModeEnable = FALSE;
                TickDiff = DEFAULTTICKDIFF;
            }

            UpdateCurrent = TRUE;
            cptrState = CPTR_HIDE_ST;       // hide the color pointer
            return (1);

        case ID_SLD4:
            if((objMsg == SLD_MSG_DEC) || (objMsg == SLD_MSG_INC))
            {                               // check if slider is moved
                // manually call the default msg function of the slider to
                // update the level of the slider (Intensity level)
                SldMsgDefault(objMsg, (SLIDER *)pObj, pMsg);

                // make sure the redrawing of focus is not performed.
                // we need this since this slider is hidden
                // first we need to make sure the global focus pointer is not set to
                // the slider.
                if(GOLGetFocus() == pObj)
                    GOLGetFocus() = NULL;

                // now we can make sure the hidden slider is not redrawn    				
                ClrState(pObj, SLD_DRAW | SLD_FOCUSED | DRAW_FOCUS);
                updateIntensity = TRUE;

                // return 0 so that SldMsgDefault() will not be called again
                return (0);
            }

            return (1);

        default:
            return (1);
    }
}

/************************************************************************
 Function: WORD RGBDemoDrawCallback(void)
 
 Overview: This function is called by GOLDraw() function when the state 
 		   is in this particular demo. The current implementation of 
 		   this function is BLOCKING. For cases with hardware accelerated 
 		   primitive functions, this can be optimized to be non-blocking 
 		   by adding states similar to the Object Drawing functions.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD RGBDemoDrawCallback(void)
{
    // Note that this function is coded as blocking
    static DWORD    prevTick = 0;                   // keeps previous value of tick
    WORD            objectID;
    BUTTON          *pBtn;
    SLIDER          *pSld;
    EDITBOX         *pEb;
    void            *pObj;
    SHORT           temp;

    if(tick > PtrTickDiff)
    {
        if((cptrState == CPTR_SHOW_ST) || (cptrState == CPTR_UPDATE_ST))
        {
            cptrState = CPTR_HIDE_ST;
            UpdateCurrent = TRUE;
        }

        PtrTickDiff = tick;
    }

    if((tick - prevTick) > TickDiff)
    {

        // checks if the preset colors are to be changed.
        objectID = ID_BUTTON_B;
        while(objectID <= ID_BUTTON_C)
        {
            pBtn = (BUTTON *)GOLFindObject(objectID);
            if(GetState(pBtn, BTN_PRESSED))
            {
                if(objectID == ID_BUTTON_B)
                {
                    Pres1Red = RedPos;
                    Pres1Green = GreenPos;
                    Pres1Blue = BluePos;
                }
                else
                {
                    Pres2Red = RedPos;
                    Pres2Green = GreenPos;
                    Pres2Blue = BluePos;
                }

                pBtn->hdr.pGolScheme->Color0 = RGB565CONVERT(RedPos, GreenPos, BluePos);
                pBtn->hdr.pGolScheme->Color1 = RGB565CONVERT(RedPos, GreenPos, BluePos);
                pBtn->hdr.pGolScheme->TextColor0 = RGB565CONVERT
                    (
                        (REDMAX - RedPos),
                        (GREENMAX - GreenPos),
                        (BLUEMAX - BluePos)
                    );
                pBtn->hdr.pGolScheme->TextColor1 = RGB565CONVERT
                    (
                        (REDMAX - RedPos),
                        (GREENMAX - GreenPos),
                        (BLUEMAX - BluePos)
                    );

                SetState(pBtn, DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB1);
                Int2Str(EbGetText(pEb), RedPos, 3);
                SetState(pEb, EB_DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB2);
                Int2Str(EbGetText(pEb), GreenPos, 3);
                SetState(pEb, EB_DRAW);

                pEb = (EDITBOX *)GOLFindObject(ID_EB3);
                Int2Str(EbGetText(pEb), BluePos, 3);
                SetState(pEb, EB_DRAW);

                prevTick = tick;
                return (1);
            }

            objectID++;
        }

        // this moves the slider up and down while the up or down arrow buttons are pressed
        // check if the color increment/decrement buttons are still pressed
        objectID = ID_UPBTN1;
        while(objectID <= ID_DNBTN3)
        {
            pObj = GOLFindObject(objectID);
            if(GetState(pObj, BTN_PRESSED))
            {
                UpdateCurrent = TRUE;
                if(TickDiff > MINTICKDELAY)
                    TickDiff -= CHANGEDELAY;

                switch(objectID)
                {

                    // ID_DNBTN1 & ID_UPBTN1 are updated using message callback and
                    // BTN_MSG_STILLPRESSED message.
                    // ID_DNBTN2 & ID_UPBTN2 : ID_DNBTN3 & ID_UPBTN3  on the other hand
                    // are updated using the draw call back and checking the state.
                    case ID_DNBTN2:
                    case ID_UPBTN2:
                        pSld = (SLIDER *)GOLFindObject(ID_SLD2);
                        temp = SldGetPos(pSld);
                        if(objectID == ID_UPBTN2)
                        {
                            if((++temp) > GREENMAX)
                                GreenPos = GREENMAX;
                            else
                                GreenPos = temp;
                            SldSetPos(pSld, GreenPos);
                        }
                        else
                        {
                            if((--temp) < 0)
                                GreenPos = 0;
                            else
                                GreenPos = temp;
                            SldSetPos(pSld, GreenPos);
                        }

                        SetState(pSld, SLD_DRAW_THUMB);

                        pEb = (EDITBOX *)GOLFindObject(ID_EB2);
                        Int2Str(EbGetText(pEb), SldGetPos(pSld), 3);
                        SetState(pEb, EB_DRAW);
                        cptrState = CPTR_HIDE_ST;   // hide the color pointer
                        break;

                    case ID_DNBTN3:
                    case ID_UPBTN3:
                        pSld = (SLIDER *)GOLFindObject(ID_SLD3);
                        temp = SldGetPos(pSld);
                        if(objectID == ID_UPBTN3)
                        {
                            if((++temp) > BLUEMAX)
                                BluePos = BLUEMAX;
                            else
                                BluePos = temp;
                            SldSetPos(pSld, BluePos);
                        }
                        else
                        {
                            if((--temp) < 0)
                                BluePos = 0;
                            else
                                BluePos = temp;
                            SldSetPos(pSld, BluePos);
                        }

                        SetState(pSld, SLD_DRAW_THUMB);

                        pEb = (EDITBOX *)GOLFindObject(ID_EB3);
                        Int2Str(EbGetText(pEb), SldGetPos(pSld), 3);
                        SetState(pEb, EB_DRAW);
                        cptrState = CPTR_HIDE_ST;   // hide the color pointer
                        break;

                    default:
                        break;
                }   // end of switch()
            }       // end of if (GetState...)	

            objectID++;
            prevTick = tick;
        }           // end of while()

        // color fading animation implementation
        if(FadeModeEnable == TRUE)
        {

            // call fade color control to update the colors
            FadeColorControl();

            // update the objects in the screens				
            pSld = (SLIDER *)GOLFindObject(ID_SLD1);
            SldSetPos(pSld, RedPos);
            SetState(pSld, SLD_DRAW_THUMB);

            pEb = (EDITBOX *)GOLFindObject(ID_EB1);
            Int2Str(EbGetText(pEb), RedPos, 3);
            SetState(pEb, EB_DRAW);

            pSld = (SLIDER *)GOLFindObject(ID_SLD2);
            SldSetPos(pSld, GreenPos);
            SetState(pSld, SLD_DRAW_THUMB);

            pEb = (EDITBOX *)GOLFindObject(ID_EB2);
            Int2Str(EbGetText(pEb), GreenPos, 3);
            SetState(pEb, EB_DRAW);

            pSld = (SLIDER *)GOLFindObject(ID_SLD3);
            SldSetPos(pSld, BluePos);
            SetState(pSld, SLD_DRAW_THUMB);

            pEb = (EDITBOX *)GOLFindObject(ID_EB3);
            Int2Str(EbGetText(pEb), BluePos, 3);
            SetState(pEb, EB_DRAW);

            UpdateCurrent = TRUE;

            //cptrState = CPTR_HIDE_ST;					// hide the color pointer
            prevTick = tick;
        }           // end of if (FadeModeEnable == TRUE) {
    }               // end of if((tick-prevTick)>TickDiff)

    // update the light intensity level
    if(updateIntensity)
    {
        while(!DrawIntensityLevel(UPDATE_INTENSITY));
        // add here code to update color values and intensity if application controls
        // external lighting control.
    }

    if(UpdateColor == TRUE)
    {
        while(!DrawIntensityLevel(DRAW_INTENSITY));
        // add here code to update color values and intensity if application controls
        // external lighting control.
        while(!DrawColorPalette());
    }

    if(UpdateCurrent == TRUE)
    {
        DrawColorPointer();
        // add here code to update color values and intensity if application controls
        // external lighting control.
        while(!ShowCurrentColor());
    }

    return (1);
}

/************************************************************************
 Function: Int2Str(XCHAR *pStr, WORD value, SHORT charCount)
 
 Overview: Converts integer value to string to be displayed in the
           edit boxes that displays the RGB values.
 		    
 Input: pStr - pointer to the string holder of the value to be displayed
 		value - the integer value that will be converted.
 		charCount - the number of characters that was converted.

 Output: none
************************************************************************/
void Int2Str(XCHAR *pStr, WORD value, SHORT charCount)
{
   
    // this implements sprintf(strVal, "%d", temp); faster
    // note that this is just for values >= 0, while sprintf covers negative values.
    // this also does not check if the pStr pointer points to a valid allocated space.
    // caller should make sure it is allocated.
    // point to the end of the array
    
    // make sure the string will termintate.
    pStr = pStr + (charCount);
    *pStr = 0;
    
    // position to the first valid character position
    pStr--;
    charCount--;

    // convert the value to string starting from the ones, then tens, then hundreds etc...
    do
    {
        *pStr-- = (value % 10) + '0';
        value /= 10;
    } while(charCount--);
}

/************************************************************************
 Function: void FadeColorControl(void)
 
 Overview: Controls the changes in the RGB values to control the fading
 		   control.
 		    		    
 Input: none

 Output: none
************************************************************************/
void FadeColorControl(void)
{
    static BYTE RedChange = 0;
    static BYTE BlueChange = 0;
    static BYTE GreenChange = 0;

    SHORT       temp;

    // decide if change to increment or decrement
    // all three colors must reach the limits
    if(((RedChange) && (GreenChange)) && (BlueChange))
    {

        // change the target limit
        if(RedLim == Pres1Red)
            RedLim = Pres2Red;
        else
            RedLim = Pres1Red;

        if(GreenLim == Pres1Green)
            GreenLim = Pres2Green;
        else
            GreenLim = Pres1Green;

        if(BlueLim == Pres1Blue)
            BlueLim = Pres2Blue;
        else
            BlueLim = Pres1Blue;

        // decide if we increment or decrement
        FadeDirRed = (RedLim > RedPos) ? FADECOLORDELTA : -FADECOLORDELTA;
        FadeDirGreen = (GreenLim > GreenPos) ? FADECOLORDELTA : -FADECOLORDELTA;
        FadeDirBlue = (BlueLim > BluePos) ? FADECOLORDELTA : -FADECOLORDELTA;

        // reset the flags
        RedChange = 0;
        GreenChange = 0;
        BlueChange = 0;
    }

    /* Check for change in the RED color */
    if(!RedChange)
    {

        // get the next value
        temp = RedPos + FadeDirRed;
        if(FadeDirRed < 0)
        {
            if(temp <= RedLim)
            {
                temp = RedLim;
                RedChange = 1;
            }
        }
        else
        {
            if(temp >= RedLim)
            {
                temp = RedLim;
                RedChange = 1;
            }
        }

        RedPos = temp;
    }

    /* Check for change in the GREEN color */
    if(!GreenChange)
    {

        // get the next value
        temp = GreenPos + FadeDirGreen;
        if(FadeDirGreen < 0)
        {
            if(temp <= GreenLim)
            {
                temp = GreenLim;
                GreenChange = 1;
            }
        }
        else
        {
            if(temp >= GreenLim)
            {
                temp = GreenLim;
                GreenChange = 1;
            }
        }

        GreenPos = temp;
    }

    /* Check for change in the BLUE color */
    if(!BlueChange)
    {

        // get the next value
        temp = BluePos + FadeDirBlue;
        if(FadeDirBlue < 0)
        {
            if(temp <= BlueLim)
            {
                temp = BlueLim;
                BlueChange = 1;
            }
        }
        else
        {
            if(temp >= BlueLim)
            {
                temp = BlueLim;
                BlueChange = 1;
            }
        }

        BluePos = temp;
    }
}

/************************************************************************
 Function: WORD ShowCurrentColor(void)
 
 Overview: Function that updates the color previews on each RGB colors.
 		   		    		    
 Input: none

 Output: Returns if rendering of all custom objects/shapes is finished.
************************************************************************/
WORD ShowCurrentColor(void)
{
    SetColor(RGB565CONVERT(180, 180, 180));
    WAIT_UNTIL_FINISH(Rectangle(CURRBARXPOS, CURRBARYPOS, CURRBARXPOS + CURRBARWIDTH, CURRBARYPOS + CURRBARHEIGHT));

    SetColor(RGB565CONVERT(RedPos, GreenPos, BluePos));
    WAIT_UNTIL_FINISH(Bar(CURRBARXPOS + 2, CURRBARYPOS + 2, CURRBARXPOS + CURRBARWIDTH - 2, CURRBARYPOS + CURRBARHEIGHT - 2));
    UpdateCurrent = FALSE;
    return (TRUE);
}

/************************************************************************
 Function: WORD DrawColorPalette(void)
 
 Overview: Function that draws the color pallette.
		   		    		    
 Input: none

 Output: Returns if rendering of all custom objects/shapes is finished.
************************************************************************/
WORD DrawColorPalette(void)
{

    // This function updates the global color variables: RedPos, GreenPos and BluePos
    // or draws the RGB color palette.
    // 		task			meaning
    //		DRAWPALCOLOR	create the palette
    //		GETPALCOLOR		update the global color based on the x position given
    // When drawing the RGB color palette. The colors changes in the X direction. Y is not
    // affecting the colors. Here's how the colors changes:

    /*
		|------------------------------   RED   --------------------------------| 		
		|	inc->	|	MAX		|	MAX		|	dec->	|	0		|	0		|
		|------------------------------  GREEN ---------------------------------| 		
		|	0		|	0		|	inc->	|	MAX		|	MAX		|	dec->	|
		|------------------------------   BLUE  --------------------------------| 		
		|	MAX		|	dec->	|	0		|	0		|	inc->	|	MAX		|	
	
		The light and dark (intensity) effects are also added into the equations. 
		Controls are applied to the global variables RedMax, GreenMax, BlueMax for
		light effect and RedMin, GreenMin and BlueMin for dark effects.
	*/
    WORD    red, green, blue, trueXPos, y, x;

    for(x = 0; x <= PLTWIDTH; x++)
    {
        CalculatePaletteColor(&red, &green, &blue, x);

        trueXPos = x + PLTXPOS;

        SetColor(RGB565CONVERT((BYTE) red, (BYTE) green, (BYTE) blue));
        WAIT_UNTIL_FINISH(Bar(trueXPos, PLTYPOS, trueXPos, PLTYPOS + PLTHEIGHT));
        UpdateColor = FALSE;
    }

    SetColor(RGB565CONVERT(180, 180, 180));

    // this draws the blue rectangles that shows the current light intensity
    // dimensions are dependent on the location of the hidden slider that is being
    // used to detect the touch and decide the level of intensity
    y = 0;
    while(y < SLD4HEIGHT)
    {

        // rectangles are drawn every 5 pixels with height = 4 pixels.
        SetColor(RGB565CONVERT(0x4C, 0x8E, 0xFF));
        WAIT_UNTIL_FINISH(Rectangle(SLD4XPOS, SLD4YPOS + SLD4HEIGHT - (y + 3), SLD4XPOS + SLD4WIDTH, SLD4YPOS + SLD4HEIGHT - y));
        y += 5;
    }

    return (1);
}

/************************************************************************
 Function: WORD DrawIntensityLevel(WORD drawType)
 
 Overview: Draws the intensity control object on the screen.
 		   		    		    
 Input: none

 Output: Returns if rendering of all custom objects/shapes is finished.
************************************************************************/
WORD DrawIntensityLevel(WORD drawType)
{
    static SHORT    prevValue = 0;  // value of the intensity level
    WORD            value, y, temp; // intensity variables, value is taken from hidden slider
    SLIDER          *pSld;

    if(drawType == DRAW_INTENSITY)
        prevValue = 0;
    else
        prevValue = Intensity;

    // update the light intensity level
    // check the value of slider (we have to clear the draw state of the slider since it is
    // actually hidden and used only to keep track of the intensity level.
    pSld = (SLIDER *)GOLFindObject(ID_SLD4);
    value = SldGetPos(pSld);
    ClrState(pSld, DRAW);

    if(value < prevValue)
    {

        // remove bars
        SetColor(RGBBACKGROUND);
        while(prevValue > value)
        {
            y = (prevValue * (SLD4HEIGHT)) / SldGetRange(pSld);
            y = y - (y % 5);

            // draw a bar with the background color to remove the current bar drawn
            WAIT_UNTIL_FINISH
            (
                Bar
                    (
                        SLD4XPOS + 1,
                        SLD4YPOS + SLD4HEIGHT - (y + 3) + 1,
                        SLD4XPOS + SLD4WIDTH - 1,
                        SLD4YPOS + SLD4HEIGHT - y - 1
                    )
            );

            // decrement by three since we are drawing every 6 pixels
            prevValue -= 5;
            if(prevValue <= 0)
                prevValue = 0;
        }
    }

    // Draw bars if there the new value is greater
    // than the previous
    else
    {
        while(prevValue <= value)
        {

            // bars are drawn every 6 pixels with height = 4 pixels.
            y = (prevValue * (SLD4HEIGHT)) / SldGetRange(pSld);
            y = y - (y % 5);

            temp = (SLD4HEIGHT >> 1);
            if(y < temp)
            {
                SetColor(BRIGHTGREEN);
            }
            else if((y < (temp + (temp >> 1))) && (y >= temp))
            {
                SetColor(BRIGHTYELLOW);
            }
            else if(y >= (temp + (temp >> 1)))
            {
                SetColor(BRIGHTRED);
            }

            // draw a bar to increase in value
            WAIT_UNTIL_FINISH
            (
                Bar
                    (
                        SLD4XPOS + 1,
                        SLD4YPOS + SLD4HEIGHT - (y + 3) + 1,
                        SLD4XPOS + SLD4WIDTH - 1,
                        SLD4YPOS + SLD4HEIGHT - y - 1
                    )
            );

            // increment by three since we are drawing every 6 pixels
            prevValue += 5;
        }
    }

    // reset the flag
    updateIntensity = FALSE;
    Intensity = value;

    return (1);
}

/************************************************************************
 Function: void CalculatePaletteColor(WORD *red, WORD *green, 
 									  WORD *blue, WORD pos)
 
 Overview: Function that calculates the corresponding color based on the
 		   touched area on the pallete.
 		    		    		    
 Input: *red - pointer to the red value
 		*green - pointer to the green value
 		*blue - pointer to the blue value
 		pos - the position of the touch in the pallete.

 Output: none
************************************************************************/
void CalculatePaletteColor(WORD *red, WORD *green, WORD *blue, WORD pos)
{

    //int loc;
    WORD    temp, loc;

    // calculate the effect of x location
    loc = pos / DIVISION;   // gets the location in the palette
    temp = pos % DIVISION;  // calculates the x position in each division
    switch(loc)
    {
        case 0:
            *red = (temp * REDMAX) / DIVISION;                  // red here is increasing as x increases
            *green = 0;
            *blue = BLUEMAX;
            break;

        case 1:
            *red = REDMAX;
            *green = 0;
            *blue = BLUEMAX - ((temp * BLUEMAX) / DIVISION);    // blue here is decreasing as x increases
            break;

        case 2:
            *red = REDMAX;
            *green = (temp * GREENMAX) / DIVISION;              // green here is increasing as x increases
            *blue = 0;
            break;

        case 3:
            *red = REDMAX - ((temp * REDMAX) / DIVISION);       // red here is decreasing as x increases
            *green = GREENMAX;
            *blue = 0;
            break;

        case 4:
            *red = 0;
            *green = GREENMAX;
            *blue = (temp * BLUEMAX) / DIVISION;                // blue here is increasing as x increases
            break;

        case 5:
            *red = 0;
            *green = GREENMAX - ((temp * GREENMAX) / DIVISION); // green here is decreasing as x increases
            *blue = BLUEMAX;
            break;

        default:
            break;
    }
}

/************************************************************************
 Function: void DrawColorPointer(void)
 
 Overview: Function that draws the triagular locator of the user touch 
 		   in the pallete.
 		   calculates the corresponding color based on the
 		   touched area on the pallete.

 Input: none
 
 Output: none
************************************************************************/
void DrawColorPointer(void)
{
    SHORT   *pPoints;
    BYTE    i;

    if((cptrState == CPTR_UPDATE_ST) || (cptrState == CPTR_HIDE_ST))
    {

        // erase the color pointer triangle
        if(cptrState == CPTR_UPDATE_ST)
            pPoints = &PolyPoints[8];
        else
            pPoints = &PolyPoints[0];
        SetColor(RGBBACKGROUND);
        while(!DrawPoly(4, pPoints));
    }

    if((cptrState == CPTR_UPDATE_ST) || (cptrState == CPTR_SHOW_ST))
    {

        // now draw the new position
        pPoints = &PolyPoints[0];
        SetColor(WHITE);
        while(!DrawPoly(4, pPoints));
    }

    if(cptrState == CPTR_UPDATE_ST)
    {

        // update the previous positions
        for(i = 0; i < 8; i++)
        {
            PolyPoints[i + 8] = PolyPoints[i];
        }
    }
}

/************************************************************************
 Function: void UpdateColorPointer(SHORT center)
 
 Overview: Updates the polygon points before the polygon triangle 
 		   is drawn

 Input: center - position of the polygon (triangle) drawn.
 
 Output: none
************************************************************************/
void UpdateColorPointer(SHORT center)
{
    PolyPoints[4] = center;
    PolyPoints[5] = PLTYPOS - 1;

    // all the rest will adjust to the center point
    if (PolyPoints[4] - 4 < 0)
    	PolyPoints[0] = 0;
    else	
   		PolyPoints[0] = PolyPoints[4] - 4;
    PolyPoints[1] = PLTYPOS - 8;
    if (PolyPoints[4] + 4 > (GetMaxX()-1))
    	PolyPoints[2] = (GetMaxX()-1);
    else
    	PolyPoints[2] = PolyPoints[4] + 4;
    PolyPoints[3] = PLTYPOS - 8;
    PolyPoints[6] = PolyPoints[0];
    PolyPoints[7] = PolyPoints[1];
}
