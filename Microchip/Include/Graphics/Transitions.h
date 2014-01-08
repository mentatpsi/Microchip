/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Primitive Layer 
 *  Transition
 *****************************************************************************
 * FileName:        Transitions.h
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
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
 * Author                           Date                    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Microchip            	        24/12/2010	            Initial version
 *****************************************************************************************/
 
#ifndef _TRANSITIONS_H
    #define _TRANSITIONS_H

    #include "Graphics/Graphics.h"
    #include "GenericTypeDefs.h"

/*********************************************************************
* Overview: Transition type enumeration to determine the type of 
*           the transition. Each type will require specific parameters when
*           setting up the transition operation (GFXSetupTransition() or
*           GFXTransition()).
*
*	<TABLE>
*    	GFX_TRANSITION_TYPE         param1              param2 (sets the direction of transition)
*     	########################    ################    ###################################################
*		PLAIN                       pixel block size    not used
*		EXPANDING_RECTANGLE         pixel block size    not used
*		CONTRACTING_RECTANGLE       pixel block size    not used
*		SLIDE                       pixel block size    LEFT_TO_RIGHT, RIGHT_TO_LEFT, TOP_TO_BOTTOM, BOTTOM_TO_TOP
*		PUSH                        pixel block size    LEFT_TO_RIGHT, RIGHT_TO_LEFT, TOP_TO_BOTTOM, BOTTOM_TO_TOP
*		EXPANDING_LINE              pixel block size    HORIZONTAL, VERTICAL
*		CONTRACTING_LINE            pixel block size    HORIZONTAL, VERTICAL  				
*	</TABLE>

*********************************************************************/
typedef enum 
{
    PLAIN = 0,              // no transition effect
    SLIDE,                  // param1 -> Pixel-block size, param2 -> Sliding direction  LEFT_TO_RIGHT/RIGHT_TO_LEFT/TOP_TO_BOTTOM/BOTTOM_TO_TOP 
    PUSH,                   // param1 -> Pixel-block size, param2 -> Sliding direction  LEFT_TO_RIGHT/RIGHT_TO_LEFT/TOP_TO_BOTTOM/BOTTOM_TO_TOP 
    EXPANDING_RECTANGLE,    // param1 -> Pixel-block size 
    CONTRACTING_RECTANGLE,  // param1 -> Pixel-block size 
    EXPANDING_LINE,         // param1 -> Pixel-block size, param2 -> direction  HORIZONTAL/VERTICAL 
    CONTRACTING_LINE        // param1 -> Pixel-block size, param2 -> direction  HORIZONTAL/VERTICAL 
} GFX_TRANSITION_TYPE;

/*********************************************************************
* Overview: Direction enumeration to determine the direction of the 
*           selected GFX_TRANSITION_TYPE. 
*********************************************************************/
typedef enum 
{
    LEFT_TO_RIGHT = 0,		// option used in SLIDE, PUSH transition type (GFX_TRANSITION_TYPE)
    RIGHT_TO_LEFT,			// option used in SLIDE, PUSH transition type (GFX_TRANSITION_TYPE)
    TOP_TO_BOTTOM,			// option used in SLIDE, PUSH transition type (GFX_TRANSITION_TYPE)
    BOTTOM_TO_TOP,			// option used in SLIDE, PUSH transition type (GFX_TRANSITION_TYPE)
    HORIZONTAL,				// option used in EXPANDING_LINE and CONTRACTING_LINE transition type (GFX_TRANSITION_TYPE) 
    VERTICAL				// option used in EXPANDING_LINE and CONTRACTING_LINE transition type (GFX_TRANSITION_TYPE)
} GFX_TRANSITION_DIRECTION;

/*********************************************************************
* Overview: The maximum number of pixels than can be moved in one step 
*           of a transition.
*********************************************************************/
#define GFX_TRANSITION_MAX_BLOCKSIZE 10

/************************************************************************
* Function: BYTE GFXIsTransitionPending(void)
*                                                                       
* Overview: This function returns the status of a pending transition, 
*           set up by GFXSetupTransition(), waiting to be executed. 
*                                                                       
* Input:    none                                                          
*                                                                       
* Output:   Returns transition status
*           - 0 : No pending transition
*           - 1 : There is a pending transition
*                                                                       
************************************************************************/
BYTE GFXIsTransitionPending(void);

/************************************************************************
* Function: BYTE GFXExecutePendingTransition(DWORD srcpageaddr,
*                                            DWORD destpageaddr)
*                                                                       
* Overview: This function executes the transition that was set up by
*           GFXSetupTransition(). Status of the transition is returned to
*           indicate if the transition was executed or not. This function 
*           is used by the double buffering feature (USE_DOUBLE_BUFFERING)
*           to perform transition operation after the current screen is 
*           fully rendered. This function assumes that the source page
*           and destination page are already set up.
*                                                                       
* Input:    srcpageaddr - Source page address for the transition
*           destpageaddr - Destination page address for the transition
*                                                                       
* Output:    Returns status of transition
*           - 0  : Transition executed successfully
*           - -1 : Transition not executed
*                                                                       
************************************************************************/
BYTE GFXExecutePendingTransition(DWORD srcpageaddr, DWORD destpageaddr);

/************************************************************************
* Function: BYTE GFXSetupTransition(SHORT left, SHORT top, SHORT right, 
*                                   SHORT bottom, GFX_TRANSITION_TYPE type, 
*                                   WORD delay_ms, WORD param1, WORD param2)
*                                                                       
* Overview: This sets up the transition effect using the GFX_TRANSITION_TYPE 
*           and the given parameters. The actual transition execution will  
*           occur when GFXExecutePendingTransition() is called. When 
*           DOUBLE_BUFFERING is enabled, GFXExecutePendingTransition() is 
*           executed after the current screen is fully rendered.
*                                                                       
* Input:    left - left x coordinate
*           top - top y coordinate
*           right - right x coordinate
*           bottom - bottom y coordinate
*           type - Transition type
*           delay_ms - Delay in milli seconds between redraws in the 
*                      screen while executing the transition
*           param1 - Transition-type specific parameter
*           param2 - Transition-type specific parameter
*                                                                       
* Output:   Returns success of the setup 
*           - 0 : Parameters successfully saved for the new transition
*           - -1 : Parameters not saved, there is a pending transition 
*                                                                       
************************************************************************/
BYTE GFXSetupTransition(SHORT left, SHORT top, SHORT right, SHORT bottom, GFX_TRANSITION_TYPE type, WORD delay_ms, WORD param1, WORD param2);

/************************************************************************
* Function: BYTE GFXTransition(SHORT left, SHORT top, SHORT right, 
*                              SHORT bottom, GFX_TRANSITION_TYPE type, 
*                              DWORD srcpageaddr, DWORD destpageaddr,
*                              WORD delay_ms, WORD param1, WORD param2)
*                                                                       
* Overview: This immediately executes the transition effect using the
*           GFX_TRANSITION_TYPE and the given parameters. 
*                                                                       
* Input:    left         -  left x coordinate
*           top          -  top y coordinate
*           right        -  right x coordinate
*           bottom       -  bottom y coordinate
*           type         -  Transition type
*           srcpageaddr  -  Source page address for the transition
*           destpageaddr -  Destination page address for the transition
*           delay_ms     -  Delay in milli seconds between redraws in the 
*                           screen while executing the transition
*           param1       -  Transition-type specific parameter
*           param2       -  Transition-type specific parameter
*                                                                       
* Output:   Returns status of transition
*           - 0  : Transition executed successfully
*           - -1 : Transition not executed
*                                                                       
************************************************************************/
BYTE GFXTransition(SHORT left, SHORT top, SHORT right, SHORT bottom, GFX_TRANSITION_TYPE type, DWORD srcpageaddr, DWORD destpageaddr, WORD delay_ms, WORD param1, WORD param2);

/************************************************************************
* Function: WORD GFXTranslateDirection(WORD direction, WORD orientation)
*                                                                       
* Overview: This function returns the actual direction taking the 
*           screen orientation into consideration (local function)
*                                                                       
* Input:    direction - The absolute direction required      
*           orientation - The screen rotation/orientation                                                    
*                                                                       
* Output:   The new direction taking the screen orientation into 
*           consideration
*                                                                       
************************************************************************/
WORD GFXTranslateDirection(WORD direction, WORD orientation);



#endif
