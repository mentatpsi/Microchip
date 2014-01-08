/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  TextEntry
 *****************************************************************************
 * FileName:        Transitions.c
 * Dependencies:    Transitions.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 Version 3.00, C32
 * Linker:          MPLAB LINK30, LINK32
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
 * Date                 Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 24/12/2010           Initial version
 *****************************************************************************************/
#include "Graphics/Transitions.h"
#include "HardwareProfile.h"
#include "TimeDelay.h"


#ifdef USE_TRANSITION_EFFECTS

extern WORD _transitionpending, _left, _top, _right, _bottom, _type, _delay_ms, _param1, _param2;
extern DWORD _srcpageaddr, _destpageaddr;
extern void PlainCopyRectangle(void);

#define RCC_SRC_ADDR_DISCONTINUOUS 1
#define RCC_DEST_ADDR_DISCONTINUOUS 1

extern WORD Startx;
extern WORD Starty;
extern WORD Width;
extern WORD Height;

/************************************************************************
* Function: void PushRectangle(void)
*                                                                       
* Overview: New screen slides-in pushing out the old slide
*                                                                       
* Input:    param1 (GFXTransition()) -> Pixel-block size to expand
*           param2 (GFXTransition()) -> Sliding direction LEFT_TO_RIGHT/RIGHT_TO_LEFT/TOP_TO_BOTTOM/BOTTOM_TO_TOP                                                         
*                                                                       
* Output:   none
*                                                                       
************************************************************************/
void __attribute__((weak)) PushRectangle(void)
{
    SHORT x, y;
    WORD blocksize = _param1;
    WORD direction = _param2;
    
    if(blocksize == 0)
    {
        blocksize = 1;
    }
    else if(blocksize > GFX_TRANSITION_MAX_BLOCKSIZE)
    {
        blocksize = GFX_TRANSITION_MAX_BLOCKSIZE;
    }
    
    direction = GFXTranslateDirection(direction, DISP_ORIENTATION);
    
    while(IsDeviceBusy());
    
    if(direction == LEFT_TO_RIGHT)
    {
        for(x = Width; x > 0; x -= blocksize)
        {
            CopyBlock(_destpageaddr, _destpageaddr, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx + Width - x, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx + Width - x + blocksize, (x - blocksize) > 0? (x - blocksize): 0, Height);
            while(IsDeviceBusy());
            CopyBlock(_srcpageaddr,  _destpageaddr, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx + x - blocksize, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, Width - x + blocksize, Height);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    else if(direction == RIGHT_TO_LEFT)
    {
        for(x = Width; x > 0; x -= blocksize)
        {
            CopyBlock(_destpageaddr, _destpageaddr, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx + blocksize, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx, (x - blocksize) > 0? (x - blocksize): 0, Height);
            while(IsDeviceBusy());
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx + x - blocksize, Width - x + blocksize, Height);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    else if(direction == TOP_TO_BOTTOM)
    {
        for(y = Height; y > 0; y -= blocksize)
        {
            SHORT y1;
            for(y1 = Height - blocksize; y1 > (Height - y); y1 -= blocksize)
            {
                CopyBlock(_destpageaddr, _destpageaddr, ((DWORD)(Starty + y1) * DISP_HOR_RESOLUTION) + Startx, ((DWORD)(Starty + y1 + blocksize) * DISP_HOR_RESOLUTION) + Startx, Width, blocksize);
                while(IsDeviceBusy());
            }
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)(Starty + y - blocksize) * DISP_HOR_RESOLUTION) + Startx, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx, Width, Height - y + blocksize);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    else if(direction == BOTTOM_TO_TOP)
    {
        for(y = Height; y > 0; y -= blocksize)
        {
            CopyBlock(_destpageaddr, _destpageaddr, ((DWORD)(Starty + blocksize) * DISP_HOR_RESOLUTION) + Startx, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx, Width, (y - blocksize) > 0? (y - blocksize): 0);
            while(IsDeviceBusy());
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx, ((DWORD)(Starty + y - blocksize) * DISP_HOR_RESOLUTION) + Startx, Width, Height - y + blocksize);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    PlainCopyRectangle();
}

#endif //USE_TRANSITION_EFFECTS
