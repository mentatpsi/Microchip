/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        GraphicsCallbacks.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, XC16, MPLAB C32
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
 *****************************************************************************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Graphics/Graphics.h"
#include "GraphicsCallbacks.h"

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT DIMENSIONS
/////////////////////////////////////////////////////////////////////////////
#define SLDR_XSTART		20
#define SLDR_YSTART		105
#define SLDR_WIDTH		(GetMaxX()-40)
#define SLDR_HEIGHT		45

#define BTN_WIDTH		130
#define BTN_YSTART		160
#define BTN1_XSTART		((GetMaxX()>>1)-10-BTN_WIDTH)
#define BTN2_XSTART		((GetMaxX()>>1)+10)
#define BTN_HEIGHT		50

#define BAR_XSTART		((GetMaxX()-200)>>1)
#define BAR_WIDTH		4

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
extern const IMAGE_FLASH redRightArrow;
extern const IMAGE_FLASH redLeftArrow;

/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
GOL_SCHEME   *pScheme;                // alternative style scheme
SLIDER       *pSld;                     // pointer to the slider object
WORD         update = 0;                // variable to update customized graphics
WORD         prevValue = 0;

const XCHAR  LeftStr[] = {'L','E','F','T',0};
const XCHAR  RightStr[] = {'R','I','G','H','T',0};


/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateScreen(void)
// Input: None
// Output: Returns non-zero if the objects are created 
//         returns 0 when one of the objects was not created.
// Overview: Creates the Demo screen.
/////////////////////////////////////////////////////////////////////////////
WORD CreateScreen(void)
{
    pScheme = GOLCreateScheme();  // create alternative style scheme
    pScheme->TextColor0 = BLACK;
    pScheme->TextColor1 = BRIGHTBLUE;

    SetColor(BLACK);
    ClearDevice();

    update = 1;                     // to initialize the user graphics
    prevValue = 0;

    if (!BtnCreate
    (
        ID_BTN1,                    // object’s ID
        BTN1_XSTART,
        BTN_YSTART,
        BTN1_XSTART+BTN_WIDTH,
        BTN_YSTART+BTN_HEIGHT,      // object’s dimension
        0,                          // radius of the rounded edge
        BTN_DRAW,                   // draw the object after creation
        NULL,                       // no bitmap used
        (XCHAR*)LeftStr,            // use this text
        pScheme                // use alternative style scheme
    ))
        return 0;                              

    if (!BtnCreate
    (
    	ID_BTN2, 
    	BTN2_XSTART, 
    	BTN_YSTART, 
    	BTN2_XSTART+BTN_WIDTH, 
    	BTN_YSTART+BTN_HEIGHT, 
    	0, 
    	BTN_DRAW, 
    	NULL, 
    	(XCHAR*)RightStr, 
    	pScheme
    ))
        return 0;

    if (!(pSld = SldCreate
        (
            ID_SLD1,                // object’s ID
            SLDR_XSTART,
            SLDR_YSTART,
            SLDR_XSTART+SLDR_WIDTH,
            SLDR_YSTART+SLDR_HEIGHT,// object’s dimension
            SLD_DRAW,               // draw the object after creation
            100,                    // range
            5,                      // page
            50,                     // initial position
            NULL                    // use default style scheme
        )))
        return 0;                          

    return 1;
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
    WORD    objectID;
    SLIDER  *pSldObj;

    objectID = GetObjID(pObj);

    if(objectID == ID_BTN1)
    {
        if(objMsg == BTN_MSG_PRESSED)
        {   // check if button is pressed
            BtnSetBitmap(pObj, (void *) &redLeftArrow);     // set bitmap to show
            SetState(pObj, BTN_TEXTRIGHT);                  // move the text to the right
            pSldObj = (SLIDER *)GOLFindObject(ID_SLD1);     // find slider pointer
            SldDecPos(pSldObj);                             // decrement the slider position
            SetState(pSldObj, SLD_DRAW_THUMB);              // redraw only the thumb
        }
        else
        {
            BtnSetBitmap(pObj, NULL);                       // remove the bitmap
            ClrState(pObj, BTN_TEXTRIGHT);                  // place the text back in the middle
        }

        update = 1;
    }

    if(objectID == ID_BTN2)
    {
        if(objMsg == BTN_MSG_PRESSED)
        {
            BtnSetBitmap(pObj, (void *) &redRightArrow);    // set bitmap to show
            SetState(pObj, BTN_TEXTLEFT);                   // move the text to the left
            pSldObj = (SLIDER *)GOLFindObject(ID_SLD1);     // find slider pointer
            SldIncPos(pSldObj);                             // increment the slider position
            SetState(pSldObj, SLD_DRAW_THUMB);              // redraw only the thumb
        }
        else
        {
            BtnSetBitmap(pObj, NULL);                       // remove the bitmap
            ClrState(pObj, BTN_TEXTLEFT);                   // place the text back in the middle
        }

        update = 1;
    }

    if(objectID == ID_SLD1)
    {
        if((objMsg == SLD_MSG_INC) || (objMsg == SLD_MSG_DEC))
        {
            update = 1;
        }
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
    WORD        value, y, x;    // variables for the slider position

    if(update)
    {

        /* User defined graphics:	
		    This draws a series of bars indicating the value/position of the 
			slider's thumb. The height of the bars follows the equation of a 
			parabola "(y-k)^2 = 4a(x-h) with vertex at (k, h) at (60,100) on 
			the display. The value 110 is the 4*a constant. x & y are calculated
			based on the value of the slider thumb. The bars are drawn from 
			60 to 260 in the x-axis and 10 to 100 in the y-axis. Bars are drawn
			every 6 pixels with width of 4 pixels.
			
			Only the bars that are added or removed are drawn. Thus resulting 
			in an efficient customized drawing. 
		*/

        // check the value of slider
        value = SldGetPos(pSld);

        // remove bars if there the new value is less
        // than the previous
        SetColor(BLACK);
        if(value < prevValue)
        {
            while(prevValue > value)
            {

                // get the height of the bar to be removed
                y = (prevValue * prevValue) / 110;

                // bars are drawn every 6 pixels with width = 4 pixels.
                x = (prevValue * 2);
                x = x - (x % 6);

                // draw a BLACK colored bar to remove the current bar drawn
                while(!Bar(x+BAR_XSTART, 100-y, x+BAR_XSTART+BAR_WIDTH, 100));

                // decrement by three since we are drawing every 6 pixels
                prevValue -= 3;
            }
        }

        // Draw bars if there the new value is greater
        // than the previous
        else
        {
            while(prevValue < value)
            {
                if(prevValue < 60)
                {
                    SetColor(BRIGHTGREEN);
                }
                else if((prevValue < 80) && (prevValue >= 60))
                {
                    SetColor(BRIGHTYELLOW);
                }
                else if(prevValue >= 80)
                {
                    SetColor(BRIGHTRED);
                }

                // get the height of the bar to be drawn
                y = (prevValue * prevValue) / 110;

                // bars are drawn every 6 pixels with width = 4 pixels.
                x = (prevValue * 2);
                x = x - (x % 6);

                // draw a bar to increase in value
                while(!Bar(x+BAR_XSTART, 100-y, x+BAR_XSTART+BAR_WIDTH, 100));

                // increment by three since we are drawing every 6 pixels
                prevValue += 3;
            }
        }

        // prevValue will have the current value after drawing or removing bars.
        // reset the update flag
        update = 0;
    }

    return (1);
}


