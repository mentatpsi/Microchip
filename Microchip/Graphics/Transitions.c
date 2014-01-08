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
 * Copyright � 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
 * Date                     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 24/12/2010	    		Initial version
 * 11/23/2011               Fix ExpandRectangle() issue where the edge shows 
 *                          garbage data. 
 * 11/29/2011               Fix ContractRectangle() issue where one line 
 *                          is not copied correctly. 
 *****************************************************************************************/
#include "Graphics/Transitions.h"
#include "HardwareProfile.h"
#include "TimeDelay.h"

#ifdef USE_TRANSITION_EFFECTS

/* Local Function Prototypes            */
void        PushRectangle(void);
void        PlainCopyRectangle(void);
/****************************************/

WORD _transitionpending, _left, _top, _right, _bottom, _type, _delay_ms, _param1, _param2;
DWORD _srcpageaddr, _destpageaddr;

WORD Startx;
WORD Starty;
WORD Width;
WORD Height;

/************************************************************************
* Function: WORD GFXTranslateDirection(WORD direction, WORD orientation)
*                                                                       
* Overview: This function returns the actual direction taking the 
*           screen orientation into consideration (local function)
*                                                                       
* Input:    direction   ->  The absolute direction required      
*           orientation ->  The screen rotation/orientation                                                    
*                                                                       
* Output:   The new direction taking the screen orientation into 
*           consideration
*                                                                       
************************************************************************/
WORD GFXTranslateDirection(WORD direction, WORD orientation)
{
    switch(orientation)
    {
        case 90:
                switch(direction)
                {
                    case LEFT_TO_RIGHT: return BOTTOM_TO_TOP;
                    case RIGHT_TO_LEFT: return TOP_TO_BOTTOM;
                    case TOP_TO_BOTTOM: return LEFT_TO_RIGHT;
                    case BOTTOM_TO_TOP: return RIGHT_TO_LEFT;
                    case HORIZONTAL:    return VERTICAL;
                    case VERTICAL:      return HORIZONTAL;
                }
                break;

        case 180:
                switch(direction)
                {
                    case LEFT_TO_RIGHT: return RIGHT_TO_LEFT;
                    case RIGHT_TO_LEFT: return LEFT_TO_RIGHT;
                    case TOP_TO_BOTTOM: return BOTTOM_TO_TOP;
                    case BOTTOM_TO_TOP: return TOP_TO_BOTTOM;
                }
                break;

        case 270:
                switch(direction)
                {
                    case LEFT_TO_RIGHT: return TOP_TO_BOTTOM;
                    case RIGHT_TO_LEFT: return BOTTOM_TO_TOP;
                    case TOP_TO_BOTTOM: return RIGHT_TO_LEFT;
                    case BOTTOM_TO_TOP: return LEFT_TO_RIGHT;
                    case HORIZONTAL:    return VERTICAL;
                    case VERTICAL:      return HORIZONTAL;
                }
                break;
    }
    
    return (direction);
}

/************************************************************************
* Function: BYTE GFXIsTransitionPending(void)
*                                                                       
* Overview: This function returns the status if there is any pending 
*           transition waiting to be executed. (Used with DOUBLE_BUFFERING)
*                                                                       
* Input:    none                                                          
*                                                                       
* Output:   0 -> No
*           1 -> Yes
*                                                                       
************************************************************************/
BYTE GFXIsTransitionPending(void)
{
    return _transitionpending;
}

/************************************************************************
* Function: BYTE GFXExecutePendingTransition(DWORD srcpageaddr,
*                                            DWORD destpageaddr)
*                                                                       
* Overview: This function executes the pending transition and returns the 
*           value returned by GFXTransition(). (Used with DOUBLE_BUFFERING)
*                                                                       
* Input:    srcpageaddr  -> Source page address for the transition
*           destpageaddr -> Destination page address for the transition
*                                                                       
* Output:   See the output of GFXTransition()
*                                                                       
************************************************************************/
BYTE GFXExecutePendingTransition(DWORD srcpageaddr, DWORD destpageaddr)
{
    if(_transitionpending)
    {
        _transitionpending = 0;
        return GFXTransition(_left, _top, _right, _bottom, _type, srcpageaddr, destpageaddr, _delay_ms, _param1, _param2);
    }
    return 0;
}

/************************************************************************
* Function: BYTE GFXSetupTransition(SHORT left, SHORT top, SHORT right, 
*                                   SHORT bottom, GFX_TRANSITION_TYPE type, 
*                                   WORD delay_ms, WORD param1, WORD param2)
*                                                                       
* Overview: This function saves the parameters to be used and marks the 
*           transition to be pending which is executed when 
*           GFXExecutePendingTransition() is called. (Used with DOUBLE_BUFFERING)
*                                                                       
* Input:    left     ->  left x coordinate
*           top      ->  top y coordinate
*           right    ->  right x coordinate
*           bottom   ->  bottom y coordinate
*           type     ->  Transition type
*           delay_ms ->  Delay in milli seconds between redraws in the 
*                        screen while executing the transition
*           param1   ->  Transition-type specific parameter
*           param2   ->  Transition-type specific parameter
*                                                                       
* Output:   0       ->  Parameters successfully saved for the new transition
*           -1      ->  Parameters not saved as a transition is still pending
*                                                                       
************************************************************************/
BYTE GFXSetupTransition(SHORT left, SHORT top, SHORT right, SHORT bottom, GFX_TRANSITION_TYPE type, WORD delay_ms, WORD param1, WORD param2)
{
    if (_transitionpending) return -1;
    
    _transitionpending = 1;
    _left = left;
    _top = top;
    _right = right;
    _bottom = bottom;
    _type = type;
    _delay_ms = delay_ms;
    _param1 = param1;
    _param2 = param2;
    
    return 0;
}

/************************************************************************
* Function: void PlainCopyRectangle(void)
*                                                                       
* Overview: No transition effect (Sudden transition effect)
*                                                                       
* Input:    none                                                          
*                                                                       
* Output:   none
*                                                                       
************************************************************************/
void PlainCopyRectangle(void)
{
    while(IsDeviceBusy());
    CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, Width, Height);
    while(IsDeviceBusy());
}

/************************************************************************
* Function: void ExpandRectangle(void)
*                                                                       
* Overview: Expanding rectangle from the mid to the periphery
*                                                                       
* Input:    param1 (GFXTransition()) -> Pixel-block size to expand
*                                                                       
* Output:   none
*                                                                       
************************************************************************/
void ExpandRectangle(void)
{
    #define SCALE 100

    WORD x = Width / 2;
    WORD y = Height / 2;
    WORD den = (x > y)? y: x;
    WORD xpitch = x * SCALE / den;
    WORD ypitch = y * SCALE / den;
    WORD i;
    WORD blocksize = _param1;
    
    if(blocksize == 0)
    {
        blocksize = 1;
    }
    else if(blocksize > GFX_TRANSITION_MAX_BLOCKSIZE)
    {
        blocksize = GFX_TRANSITION_MAX_BLOCKSIZE;
    }
    
    while(IsDeviceBusy());
    
    for(i = 0; i <= den; i += blocksize)
    {
        x = (Width / 2) - ((i * xpitch) / SCALE);
        y = (Height / 2) - ((i * ypitch) / SCALE);
        CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)(Starty + y) * DISP_HOR_RESOLUTION) + Startx + x, ((DWORD)(Starty + y) * DISP_HOR_RESOLUTION) + Startx + x, 2 * xpitch * i / SCALE, 2 * ypitch * i / SCALE);
        DelayMs(_delay_ms);
        while(IsDeviceBusy());
    }
    PlainCopyRectangle();
}

/************************************************************************
* Function: void ContractRectangle(void)
*                                                                       
* Overview: Expanding rectangle from the mid to the periphery
*                                                                       
* Input:    param1 (GFXTransition()) -> Pixel-block size to expand
*                                                                       
* Output:   none
*                                                                       
************************************************************************/
void ContractRectangle(void)
{
    #define SCALE 100

    WORD x = Width / 2;
    WORD y = Height / 2;
    WORD den = (x > y)? y: x;
    WORD xpitch = x * SCALE / den;
    WORD ypitch = y * SCALE / den;
    WORD i;
    WORD blocksize = _param1;
    
    if(blocksize == 0)
    {
        blocksize = 1;
    }
    else if(blocksize > GFX_TRANSITION_MAX_BLOCKSIZE)
    {
        blocksize = GFX_TRANSITION_MAX_BLOCKSIZE;
    }
    
    while(IsDeviceBusy());
    
    for(i = 0; i <= den; i += blocksize)
    {
        x = (i * xpitch) / SCALE;
        y = (i * ypitch) / SCALE;
        CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, x, Height);
        CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx + Width - x, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx + Width - x, x, Height);
        CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, Width, y);
        CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)(Starty + Height - y) * DISP_HOR_RESOLUTION) + Startx, ((DWORD)(Starty + Height - y) * DISP_HOR_RESOLUTION) + Startx, Width, y);
        DelayMs(_delay_ms);
        while(IsDeviceBusy());
    }
    PlainCopyRectangle();
}

/************************************************************************
* Function: void ExpandLine(void)
*                                                                       
* Overview: Expanding line from the mid to the periphery
*                                                                       
* Input:    param1 (GFXTransition())  -> Pixel-block size to expand
*               param2 (GFXTransition()) ->  Direction of transition - HORIZONTAL/VERTICAL
*                                                                       
* Output:   none
*                                                                       
************************************************************************/
void ExpandLine(void)
{    
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

    if(direction == HORIZONTAL)
    {
        WORD i;
        
        for(i = 1; i <= Width / (2 * blocksize); i++)
        {
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx + (Width / 2) - (i * blocksize), ((DWORD)(Starty) * DISP_HOR_RESOLUTION) + Startx + (Width / 2) - (i * blocksize), 2 * i * blocksize, Height);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    else if(direction == VERTICAL)
    {
        WORD i;
        
        for(i = 1; i <= Height / (2 * blocksize); i++)
        {
            CopyBlock(_srcpageaddr, _destpageaddr, (DWORD)(Starty + (Height / 2) - (i * blocksize)) * DISP_HOR_RESOLUTION + Startx, (DWORD)(Starty + (Height / 2) - (i * blocksize)) * DISP_HOR_RESOLUTION + Startx, Width, 2 * i * blocksize);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }

    PlainCopyRectangle();
}

/************************************************************************
* Function: void ContractLine(void)
*                                                                       
* Overview: Expanding line from the mid to the periphery
*                                                                       
* Input:    param1 (GFXTransition())  -> Pixel-block size to expand
*               param2 (GFXTransition()) ->  Direction of transition - HORIZONTAL/VERTICAL
*                                                                       
* Output:   none
*                                                                       
************************************************************************/
void ContractLine(void)
{    
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

    if(direction == HORIZONTAL)
    {
        WORD i;
        
        for(i = 0; i < Width / (2 * blocksize); i++)
        {
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, (i + 1) * blocksize, Height);
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx + Width - (i * blocksize) - blocksize, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx + Width - (i * blocksize) - blocksize, (i + 1) * blocksize, Height);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    else if(direction == VERTICAL)
    {
        WORD i;
        
        for(i = 0; i < Height / (2 * blocksize); i++)
        {
            CopyBlock(_srcpageaddr, _destpageaddr, (DWORD)Starty * DISP_HOR_RESOLUTION + Startx, (DWORD)Starty * DISP_HOR_RESOLUTION + Startx, Width, (i + 1) * blocksize);
            CopyBlock(_srcpageaddr, _destpageaddr, (DWORD)(Starty + Height - (i * blocksize) - blocksize) * DISP_HOR_RESOLUTION + Startx, (DWORD)(Starty + Height - (i * blocksize) - blocksize) * DISP_HOR_RESOLUTION + Startx, Width, (i + 1) * blocksize);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }

    PlainCopyRectangle();
}

/************************************************************************
* Function: void SlideRectangle(void)
*                                                                       
* Overview: New screen slides-in overwriting the old screen
*                                                                       
* Input:    param1 (GFXTransition()) -> Pixel-block size to expand
*           param2 (GFXTransition()) -> Sliding direction LEFT_TO_RIGHT/RIGHT_TO_LEFT/TOP_TO_BOTTOM/BOTTOM_TO_TOP
*                                                                       
* Output:   none
*                                                                       
************************************************************************/
void SlideRectangle(void)
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
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx + x - blocksize, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, Width - x + blocksize, Height);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    else if(direction == RIGHT_TO_LEFT)
    {
        for(x = Width; x > 0; x -= blocksize)
        {
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx + x - blocksize, Width - x + blocksize, Height);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    else if(direction == TOP_TO_BOTTOM)
    {
        for(y = Height; y > 0; y -= blocksize)
        {
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)(Starty + y - blocksize) * DISP_HOR_RESOLUTION) + Startx, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, Width, Height - y + blocksize);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    else if(direction == BOTTOM_TO_TOP)
    {
        for(y = Height; y > 0; y -= blocksize)
        {
            CopyBlock(_srcpageaddr, _destpageaddr, ((DWORD)Starty * DISP_HOR_RESOLUTION) + Startx, ((DWORD)(Starty + y - blocksize) * DISP_HOR_RESOLUTION) + Startx, Width, Height - y + blocksize);
            DelayMs(_delay_ms);
            while(IsDeviceBusy());
        }
    }
    PlainCopyRectangle();
}
/************************************************************************
* Function: BYTE GFXTransition(SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, GFX_TRANSITION_TYPE type, 
*                              DWORD srcpageaddr, DWORD destpageaddr,
*                              WORD delay_ms, WORD param1, WORD param2)
*                                                                       
* Overview: This function saves the parameters to be used and marks the 
*           transition to be pending which is executed when 
*           GFXExecutePendingTransition() is called
*                                                                       
* Input:    left         ->  left x coordinate
*           top          ->  top y coordinate
*           right        ->  right x coordinate
*           bottom       ->  bottom y coordinate
*           type         ->  Transition type
*           srcpageaddr  ->  Source page address for the transition
*           destpageaddr ->  Destination page address for the transition
*           delay_ms     ->  Delay in milli seconds between redraws in the 
*                            screen while executing the transition
*           param1       ->  Transition-type specific parameter
*           param2       ->  Transition-type specific parameter
*                                                                       
* Output:   0       ->  Transition executed successfully
*           -1      ->  Transition not executed
*                                                                       
************************************************************************/
BYTE GFXTransition(SHORT left, SHORT top, SHORT right, SHORT bottom, GFX_TRANSITION_TYPE type, DWORD srcpageaddr, DWORD destpageaddr, WORD delay_ms, WORD param1, WORD param2)
{
    BYTE retval = 0;
    SHORT width, height;

    if(left > right)
    {
        return (-1); /* Don't draw */
    }

    if(top > bottom)
    {
        return (-1); /* Don't draw */
    }
    
    left    = (left < 0)    ? 0: (left > GetMaxX())  ? GetMaxX(): left;
    top     = (top < 0)     ? 0: (top > GetMaxY())   ? GetMaxY(): top;
    right   = (right < 0)   ? 0: (right > GetMaxX()) ? GetMaxX(): right;
    bottom  = (bottom < 0)  ? 0: (bottom > GetMaxY())? GetMaxY(): bottom;

    #if (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

        width   =   right - left + 1;
        height  =   bottom - top + 1;

    #elif (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

        height  =   right - left + 1;
        width   =   bottom - top + 1;

    #endif

    #if (DISP_ORIENTATION == 90)

        bottom  =   DISP_VER_RESOLUTION - left;
        left    =   top;
        right   =   left + width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 180)

        right   =   DISP_HOR_RESOLUTION - left;
        bottom  =   DISP_VER_RESOLUTION - top;
        left    =   right - width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 270)

        right   =   DISP_HOR_RESOLUTION - top;
        top     =   left;
        bottom  =   top + height;
        left    =   right - width;

    #endif

    Startx = left;
    Starty = top;
    Width = width;
    Height = height;

    _delay_ms = delay_ms;
    _srcpageaddr = srcpageaddr;
    _destpageaddr = destpageaddr;
    _param1 = param1;
    _param2 = param2;

    switch(type)
    {
    
        case EXPANDING_RECTANGLE:
                                ExpandRectangle();
                                break;
        
        case CONTRACTING_RECTANGLE:
                                ContractRectangle();
                                break;

        case SLIDE:
                                SlideRectangle();
                                break;

        case PUSH:
                                PushRectangle();
                                break;

        case EXPANDING_LINE:
                                ExpandLine();
                                break;

        case CONTRACTING_LINE:
                                ContractLine();
                                break;

        default:
                                PlainCopyRectangle();
                                retval = 0xFF;
                                break;
    };

   return (retval);
}

#endif //USE_TRANSITION_EFFECTS
