/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This demo shows a drawing tool using the touh screen module.
  *****************************************************************************
 * FileName:        DrawTool.c
 * Dependencies:    DrawTool.h, MainDemo.h
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
 * Anton Alkhimenok		03/10/08    ...
 *****************************************************************************/
#include "MainDemo.h"

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
#define DRAW_INDENT             5
#define DRAW_COLORS_NUMBER      6
#define DRAW_COLOR_LINES        WHITE
#define DRAW_COLOR_PAD          LIGHTGRAY
#define DRAW_COLORBOX_WIDTH     100
#define DRAW_COLORBOX_HEIGHT    ((CtrlBtnTop() - ((DRAW_COLORS_NUMBER + 1) * DRAW_INDENT)) / DRAW_COLORS_NUMBER)
#define ColorBoxRight()         (GetMaxX() - DRAW_INDENT)
#define ColorBoxLeft()          (ColorBoxRight() - DRAW_COLORBOX_WIDTH)
#define ColorBoxTop(number)     ((number * (DRAW_COLORBOX_HEIGHT + DRAW_INDENT)) + DRAW_INDENT)
#define ColorBoxBottom(number)  (ColorBoxTop(number) + DRAW_COLORBOX_HEIGHT)
#define FrameLeft()             DRAW_INDENT
#define FrameRight()            (ColorBoxLeft() - DRAW_INDENT)
#define FrameTop()              DRAW_INDENT
#define FrameBottom()           ColorBoxBottom((DRAW_COLORS_NUMBER - 1))

/////////////////////////////////////////////////////////////////////////////
//                            STRINGS USED
/////////////////////////////////////////////////////////////////////////////
extern XCHAR    ExitStr[];
extern XCHAR    ClearStr[];

/////////////////////////////////////////////////////////////////////////////
//                            GLOBALS
/////////////////////////////////////////////////////////////////////////////
WORD            drawColor = BRIGHTRED;

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void            DrawToolFrame(void);
void            CreateColorButtons(void);
void            DrawToolPad(void);

/************************************************************************
 Function: void CreateDrawTool()

 Overview: Creates the drawing tool demo screen.
 
 Input: none
 
 Output: none
************************************************************************/
void CreateDrawTool(void)
{

    // free memory for the objects in the previous linked list and start new list
    GOLFree();

    SetColor(SCREEN_BACKGROUND_COLOR);
    ClearDevice();

    DrawToolFrame();
    CreateCtrlButtons(ExitStr, NULL, NULL, ClearStr);
    CreateColorButtons();
    DrawToolPad();
    StCreate
    (
        0,
        (FrameLeft() + DRAW_INDENT - 1),
        (FrameTop() + DRAW_INDENT - 1),
        (FrameRight() - DRAW_INDENT + 1),
        (FrameBottom() - DRAW_INDENT + 1),
        0,
        NULL,
        NULL
    );
}

/************************************************************************
 Function: void CreateColorButtons()

 Overview: Creates the color selection buttons.
 
 Input: none
 
 Output: none
************************************************************************/
void CreateColorButtons(void)
{
    WORD    counter;

    SetColor(DRAW_COLOR_LINES);
    for(counter = 0; counter < DRAW_COLORS_NUMBER; counter++)
    {
        while
        (
            !Rectangle
                (
                    (ColorBoxLeft() - 1),
                    (ColorBoxTop(counter) - 1),
                    (ColorBoxRight() + 1),
                    (ColorBoxBottom(counter) + 1)
                )
        );
    }

    SetColor(BRIGHTRED);
    BtnCreate(ID_BUTTON1, ColorBoxLeft(), ColorBoxTop(0), ColorBoxRight(), ColorBoxBottom(0), 0, 0, NULL, NULL, NULL);
    while(!Bar(ColorBoxLeft(), ColorBoxTop(0), ColorBoxRight(), ColorBoxBottom(0)));

    SetColor(BRIGHTGREEN);
    BtnCreate(ID_BUTTON2, ColorBoxLeft(), ColorBoxTop(1), ColorBoxRight(), ColorBoxBottom(1), 0, 0, NULL, NULL, NULL);
    while(!Bar(ColorBoxLeft(), ColorBoxTop(1), ColorBoxRight(), ColorBoxBottom(1)));

    SetColor(BRIGHTBLUE);
    BtnCreate(ID_BUTTON3, ColorBoxLeft(), ColorBoxTop(2), ColorBoxRight(), ColorBoxBottom(2), 0, 0, NULL, NULL, NULL);
    while(!Bar(ColorBoxLeft(), ColorBoxTop(2), ColorBoxRight(), ColorBoxBottom(2)));

    SetColor(BRIGHTCYAN);
    BtnCreate(ID_BUTTON4, ColorBoxLeft(), ColorBoxTop(3), ColorBoxRight(), ColorBoxBottom(3), 0, 0, NULL, NULL, NULL);
    while(!Bar(ColorBoxLeft(), ColorBoxTop(3), ColorBoxRight(), ColorBoxBottom(3)));

    SetColor(BRIGHTMAGENTA);
    BtnCreate(ID_BUTTON5, ColorBoxLeft(), ColorBoxTop(4), ColorBoxRight(), ColorBoxBottom(4), 0, 0, NULL, NULL, NULL);
    while(!Bar(ColorBoxLeft(), ColorBoxTop(4), ColorBoxRight(), ColorBoxBottom(4)));

    SetColor(BRIGHTYELLOW);
    BtnCreate(ID_BUTTON6, ColorBoxLeft(), ColorBoxTop(5), ColorBoxRight(), ColorBoxBottom(5), 0, 0, NULL, NULL, NULL);
    while(!Bar(ColorBoxLeft(), ColorBoxTop(5), ColorBoxRight(), ColorBoxBottom(5)));
}

/************************************************************************
 Function: void DrawToolFrame()

 Overview: Draws the frame of the canvass where user can doodle.
 		   Frame color will be the same as the current drawing color.	
 
 Input: none
 
 Output: none
************************************************************************/
void DrawToolFrame(void)
{
    SetColor(drawColor);
    while(!Bar(FrameLeft(), FrameTop(), (FrameLeft() + DRAW_INDENT), FrameBottom()));
    while(!Bar((FrameRight() - DRAW_INDENT), FrameTop(), FrameRight(), FrameBottom()));
    while(!Bar((FrameLeft() + DRAW_INDENT), FrameTop(), (FrameRight() - DRAW_INDENT), (FrameTop() + DRAW_INDENT)));
    while
    (
        !Bar
            (
                (FrameLeft() + DRAW_INDENT),
                (FrameBottom() - DRAW_INDENT),
                (FrameRight() - DRAW_INDENT),
                FrameBottom()
            )
    );
    SetColor(DRAW_COLOR_LINES);
    while(!Rectangle(FrameLeft() - 1, FrameTop() - 1, FrameRight() + 1, FrameBottom() + 1));
}

/************************************************************************
 Function: DrawToolPad()

 Overview: Draws the canvass where user can doodle.
 
 Input: none
 
 Output: none
************************************************************************/
void DrawToolPad(void)
{
    SetColor(DRAW_COLOR_PAD);
    while
    (
        !Bar
            (
                (FrameLeft() + DRAW_INDENT + 1),
                (FrameTop() + DRAW_INDENT + 1),
                (FrameRight() - DRAW_INDENT - 1),
                (FrameBottom() - DRAW_INDENT - 1)
            )
    );
}

/************************************************************************
 Function: WORD MsgDrawTool(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
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
WORD MsgDrawTool(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static SHORT    prevX = -1; // previous point
    static SHORT    prevY = -1;
    SHORT           x, y;       // current point
    if(GetObjType(pObj) == OBJ_STATICTEXT)
    {

        // Get touched position
        x = pMsg->param1;
        y = pMsg->param2;

        SetColor(drawColor);

        // If previous position is not valid or event is PRESS
        if((pMsg->uiEvent == EVENT_PRESS) || (prevX < 0))
        {
            PutPixel(x, y);     // Draw pixel
        }
        else
        {
            while(!Line(prevX, prevY, x, y));

            // Connect with thick line previous and current points
            while(!Line(prevX - 1, prevY, x - 1, y));
            while(!Line(prevX + 1, prevY, x + 1, y));
            while(!Line(prevX, prevY - 1, x, y - 1));
            while(!Line(prevX, prevY + 1, x, y + 1));
        }

        prevX = x;
        prevY = y;              // Store position
    }
    else
    {

        // Reset if outside of box
        prevX = -1;

        switch(GetObjID(pObj))
        {
            case ID_BUTTON_A:
                if(objMsg == BTN_MSG_RELEASED)

                    // do not process if user moved the touch away from the button
                    if(pMsg->uiEvent != EVENT_MOVE)
                        screenState = CREATE_DEMOSELECTION;
                return (1);

            case ID_BUTTON_D:
                if(objMsg == BTN_MSG_RELEASED)

                    // do not process if user moved the touch away from the button
                    if(pMsg->uiEvent != EVENT_MOVE)
                        DrawToolPad();
                return (1);
        }

        if(objMsg == BTN_MSG_PRESSED)
        {
            switch(GetObjID(pObj))
            {
                case ID_BUTTON1:    drawColor = BRIGHTRED; break;
                case ID_BUTTON2:    drawColor = BRIGHTGREEN; break;
                case ID_BUTTON3:    drawColor = BRIGHTBLUE; break;
                case ID_BUTTON4:    drawColor = BRIGHTCYAN; break;
                case ID_BUTTON5:    drawColor = BRIGHTMAGENTA; break;
                case ID_BUTTON6:    drawColor = BRIGHTYELLOW; break;
            }

            DrawToolFrame();
        }
    }

    return (0);
}
