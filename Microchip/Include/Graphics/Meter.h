/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Meter
 *****************************************************************************
 * FileName:        Meter.h
 * Dependencies:    None 
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS" WITHOUT WARRANTY OF ANY
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
 * 11/12/07		Version 1.0 release
 * 07/31/08		Added arc colors options
 * 08/20/08		Added accuracy option for displaying values
 * 01/18/10		Fixed MtrIncVal() and MtrDecVal() macros
 * 08/05/11     - Modified compile time setting macro names. 
 *              - Relocated internal macros to the Meter.c.
 *****************************************************************************/
#ifndef _METER_H
    #define _METER_H

    #include <Graphics/GOL.h>
    #include "GenericTypeDefs.h"

/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define MTR_DISABLED    0x0002      // Bit for disabled state.
    #define MTR_RING        0x0004      // Bit for ring type, scales are drawn over the ring
                                        // default is only scales drawn.
    #define MTR_ACCURACY    0x0008      // Sets the meter accuracy to one decimal places
                                        // when displaying the values. Application must multiply
                                        // the minValue, maxValue and values passed to the widget
                                        // by RESOLUTION.
    #define MTR_DRAW_UPDATE 0x1000      // Bit to indicate an update only.
    #define MTR_DRAW        0x4000      // Bit to indicate object must be redrawn.
    #define MTR_HIDE        0x8000      // Bit to indicate object must be removed from screen.

    // Meter types
    #define MTR_WHOLE_TYPE      0
    #define MTR_HALF_TYPE       1
    #define MTR_QUARTER_TYPE    2

#ifndef METER_TYPE
/*************************************************************************** 
* Overview: This is a compile time setting to select the type if meter shape.
*  There are three types:
* 	    - MTR_WHOLE_TYPE   - Meter drawn with 6 octants used.
* 		- MTR_HALF_TYPE    - Meter drawn with semi circle shape.
* 		- MTR_QUARTER_TYPE - Meter drawn with quarter circle shape.
*  Set only one value at a time. This is done to save code space.
*  User can define the colors of the arcs for each type.
*  MTR_WHOLE_TYPE will use all the arc colors (arcColor1 - arcColor6)
*  MTR_HALF_TYPE will use arc colors (arcColor5, arcColor4, arcColor3, arcColor2)
*  MTR_QUARTER_TYPE will use arc colors (arcColor3, arcColor2)
*  Set the meter type in Meter.h file and arc colors using
*  MtrSetScaleColors(pMtr, arc1, arc2, arc3, arc4, arc5, arc6) macro.
***************************************************************************/
    #define METER_TYPE  MTR_WHOLE_TYPE

//#define METER_TYPE					MTR_HALF_TYPE				// Meter drawn with semi circle shape.
//#define METER_TYPE					MTR_QUARTER_TYPE			// Meter drawn with quarter circle shape.
#endif

/*************************************************************************** 
* Overview: This is a Meter compile time option to enable the display of the values.
*           This enables the display of the values. Displaying the values will 
*           have an effect on the rendering time requirement of the object.
***************************************************************************/
    #define MTR_BUILD_OPTION_DISPLAY_VALUES_ENABLE   

#ifndef MTR_BUILD_OPTION_SCALECHARCOUNT
/*************************************************************************** 
* Overview: This is a Meter compile time option to define how many characters 
*           will be allocated for the scale labels. Use this define in 
*           accordance to the maxValue-minValue. 
*           Example: 
*           if maxValue-minValue = 500, SCALECHARCOUNT should be 3. 
*           if maxValue-minValue = 90, SCALECHARCOUNT = 2
*           You must include the decimal point if this feature is enabled 
*           (see MTR_ACCURACY state bit).
***************************************************************************/
    #define MTR_BUILD_OPTION_SCALECHARCOUNT  4           
#endif
                                        
#ifndef MTR_BUILD_OPTION_DEGREECOUNT
/*************************************************************************** 
* Overview: This is a Meter compile time option to define how many degrees 
*           per scale, computed per octant 
*           Example: for 5 division per octant 45/5 = 9.
*                    So every 9 degrees a scale is drawn
*                    for a 5 scale division per octant. 
***************************************************************************/
    #define MTR_BUILD_OPTION_DEGREECOUNT 9               
#endif

#ifndef MTR_BUILD_OPTION_RESOLUTION
/*************************************************************************** 
* Overview: This is a Meter compile time option to define the factor 
*           that the meter widget will divide minValue, maxValue
***************************************************************************/
    #define MTR_BUILD_OPTION_RESOLUTION  10               
#endif
// and current value. Used only when MTR_ACCURACY state bit is set.

/*********************************************************************
* Overview: Defines the parameters required for a meter Object.
*			Depending on the type selected the meter is drawn with 
*			the defined shape parameters and values set on the 
*			given fields.
*
*********************************************************************/
typedef struct
{
    OBJ_HEADER  hdr;            // Generic header for all Objects (see OBJ_HEADER).
    XCHAR       *pText;         // The text label of the meter.
    SHORT       value;          // Current value of the meter.
    SHORT       minValue;       // minimum value the meter can display
    SHORT       maxValue;       // maximum value the meter can display (range is maxValue - minValue)
    SHORT       xCenter;        // The x coordinate center position. This is computed automatically.
    SHORT       yCenter;        // The y coordinate center position. This is computed automatically.
    SHORT       radius;         // Radius of the meter, also defines the needle length.

    // This is computed automatically.
    SHORT       xPos;           // The current x position of the needle. This is computed automatically.
    SHORT       yPos;           // The current y position of the needle. This is computed automatically.
    GFX_COLOR   arcColor6;      // Arc 6 color parameter.
    GFX_COLOR   arcColor5;      // Arc 5 color parameter
    GFX_COLOR   arcColor4;      // Arc 4 color parameter
    GFX_COLOR   arcColor3;      // Arc 3 color parameter
    GFX_COLOR   arcColor2;      // Arc 2 color parameter
    GFX_COLOR   arcColor1;      // Arc 1 color parameter

    // The following three points define three fonts used in meter widget, they can be different from the scheme font.
    // Note that the sizes of these fonts are not checked with the meter dimension. In cases where font sizes are
    // larger than the meter dimension, some overlaps will occur.
    void        *pTitleFont;    // Pointer to the font used in the title of the meter
    void        *pValueFont;    // Pointer to the font used in the current reading (if displayed) of the meter
} METER;

/*********************************************************************
* Function: METER  *MtrCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*							  SHORT bottom, WORD state, SHORT value, 
*							  SHORT minValue, SHORT maxValue, 
*							  void *pTitleFont, void  *pValueFont,				 
*							  XCHAR *pText, GOL_SCHEME *pScheme)				 
*           
* Overview: This function creates a METER object with the parameters given. 
*			It automatically attaches the new object into a global linked list of 
*			objects and returns the address of the object.
*
* PreCondition: none
*
* Input: ID - Unique user defined ID for the object instance.
*        left - Left most position of the object.
* 		 top - Top most position of the object. 
*		 right - Right most position of the object.
*		 bottom - Bottom most position of the object.
*        state - Sets the initial state of the object.
*		 value - Initial value set to the meter.
*		 minValue - The minimum value the meter will display.
*		 maxValue - The maximum value the meter will display.
*        pTitleFont - Pointer to the font used for the Title.
*        pScheme - Pointer to the font used for the Value.
*        pText - Pointer to the text label of the meter.
*        pScheme - Pointer to the style scheme used.
*
* Output: Returns the pointer to the object created.
*
* Example:
*   <CODE> 
*	#define ID_METER 101
*
*	extern const FONT_FLASH GOLMediumFont; 		// medium font
*	extern const FONT_FLASH GOLSmallFont; 		// small font
*
*	GOL_SCHEME *pMeterScheme;
*	METER *pMtr;
*	
*		pMeterScheme = GOLCreateScheme();
*
*		pMtr = MtrCreate(
*				ID_METER,				// assign ID
*				30, 50, 150, 180,		// set dimension
*				MTR_DRAW|MTR_RING,		// draw object after creation
*				0,						// set initial value
*				0, 100,					// set minimum and maximum value
*				(void*)&GOLMediumFont,	// set title font
*				(void*)&GOLSmallFont,	// set value font
*				"Speed",				// Text Label
*				pMeterScheme);			// style scheme   
*			  	  
*		// check if meter was created
*		if (pMtr == NULL)		
*		   return 0;
*
*		// Change range colors: Normal values to WHITE
*		//						Critical values to BLUE
*		//						Danger values to RED
*		// assume that WHITE, GREEN, YELLOW and RED have been defined.
*		MtrSetScaleColors(pMtr, WHITE, WHITE, WHITE, GREEN, YELLOW, RED);				
*
*		// use GOLDraw() to draw the meter and all other objects you created
*		while(!GOLDraw());
*		// OR to draw the meter manually use this:
*		//while(!MtrDraw(pMtr);
*
*	</CODE> 
*
* Side Effects: none
*
********************************************************************/
METER   *MtrCreate
        (
            WORD        ID,
            SHORT       left,
            SHORT       top,
            SHORT       right,
            SHORT       bottom,
            WORD        state,
            SHORT       value,
            SHORT       minValue,
            SHORT       maxValue,
            void        *pTitleFont,
            void        *pValueFont,
            XCHAR       *pText,
            GOL_SCHEME  *pScheme
        );

/*********************************************************************
* Function: WORD MtrTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Overview: This function evaluates the message from a user if the 
*			message will affect the object or not. The table below enumerates the translated 
*			messages for each event of the touch screen and keyboard inputs.
*
*	<TABLE>
*    	Translated Message   Input Source  Events         	Description
*     	##################   ############  ######         	###########
*     	MTR_MSG_SET      	 System		   EVENT_SET	    If event set occurs and the meter ID is sent in parameter 1.
*		OBJ_MSG_INVALID		 Any		   Any			  	If the message did not affect the object.							 
*	</TABLE>
*
* PreCondition: none
*
* Input: pMtr  - The pointer to the object where the message will be
*				 evaluated to check if the message will affect the object.
*        pMsg  - Pointer to the message struct containing the message from 
*        		 the user interface.
*
* Output: Returns the translated message depending on the received GOL message:
*		  - MTR_MSG_SET - Meter ID is given in parameter 1 for a TYPE_SYSTEM message.
*         - OBJ_MSG_INVALID - Meter is not affected.
*
* Side Effects: none
*
********************************************************************/
WORD    MtrTranslateMsg(void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Function: MtrMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Overview: This function performs the actual state change 
*			based on the translated message given. Meter value is set
*			based on parameter 2 of the message given. The following state changes 
*			are supported:
*	<TABLE>
*    	Translated Message   Input Source  Set/Clear State Bit		Description
*     	##################   ############  ######     				###########
*     	MTR_MSG_SET          System  	   Set MTR_DRAW_UPDATE  	Meter will be redrawn to update the needle position and value displayed.
*	</TABLE>
*
* PreCondition: none
*
* Input: translatedMsg - The translated message.
*        pMtr          - The pointer to the object whose state will be modified.
*        pMsg          - The pointer to the GOL message.
*        
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    MtrMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg);

/*********************************************************************
* Macros:  MtrGetVal(pMtr)
*
* Overview: This macro returns the current value of the meter.
*			Value is always in the minValue-maxValue range inclusive.
*
* PreCondition: none
*
* Input: pMtr - Pointer to the object.
*
* Output: Returns current value of the meter.
*
* Side Effects: none
*
********************************************************************/
    #define MtrGetVal(pMtr) ((pMtr)->value)

/*********************************************************************
* Function: MtrSetVal(METER *pMtr, SHORT newVal)
*
* Overview: This function sets the value of the meter to the passed
*			newVal. newVal is checked to be in the minValue-maxValue 
*			range inclusive. If newVal is not in the range, minValue 
*			maxValue is assigned depending on the given newVal 
*			if less than minValue or above maxValue.
*
* PreCondition: none
*
* Input: pMtr   - The pointer to the object.
*        newVal - New value to be set for the Meter.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void    MtrSetVal(METER *pMtr, SHORT newVal);

/*********************************************************************
* Macros:  MtrIncVal(pMtr, deltaValue)
*
* Overview: This macro is used to directly increment the value. 
*
* PreCondition: none
*
* Input: pMtr - Pointer to the object.
*		 deltaValue - Number to be added to the current Meter value.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MtrIncVal(pMtr, deltaValue)  MtrSetVal(pMtr, ((pMtr)->value + deltaValue))

/*********************************************************************
* Macros:  MtrDecVal(pMtr, deltaValue)
*
* Overview: This macro is used to directly decrement the value. 
*
* PreCondition: none
*
* Input: pMtr - Pointer to the object.
*        deltaValue - Number to be subtracted to the current Meter value.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MtrDecVal(pMtr, deltaValue)  MtrSetVal(pMtr, ((pMtr)->value - deltaValue))

/*********************************************************************
* Macros:  MtrSetScaleColors(pMtr, arc1, arc2, arc3, arc4, arc5, arc6)	
*								{	pMtr->arcColor6=arc6;			
*									pMtr->arcColor5=arc5;		
*									pMtr->arcColor4=arc4;			
*									pMtr->arcColor3=arc3;			
*									pMtr->arcColor2=arc2;			
*									pMtr->arcColor1=arc1;	}
*
* Overview: Scale colors can be used to highlight values of the meter.
*           User can set these colors to define the arc colors and scale colors.
*			This also sets the color of the meter value when displayed. Limitation is that
*			color settings are set to the following angles:
*			Color Boundaries		Type Whole		Type Half		Type Quarter
*			Arc 6					225  to 180		not used		not used
*			Arc 5					179  to 135		179  to 135 	not used
*			Arc 4					134  to  90		134  to  90		not used 
*			Arc 3					 89  to  45		 89  to  45		89  to  45		
*			Arc 2					 44  to   0		 44  to   0		44  to   0 
*			Arc 1					-45  to  -1		not used 		not used
*			As the meter is drawn colors are changed depending on the 
*			angle of the scale and label being drawn.
*
* PreCondition: The object must be created (using MtrCreate()) before
*				a call to this macro is performed.
*
* Input: pMtr - Pointer to the object.
*		 arc1 - color for arc 1. 
*		 arc2 - color for arc 2. 
*		 arc3 - color for arc 3. 
*		 arc4 - color for arc 4. 
*		 arc5 - color for arc 5. 
*		 arc6 - color for arc 6. 
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MtrSetScaleColors(pMtr, arc1, arc2, arc3, arc4, arc5, arc6) \
    {                                                                   \
        pMtr->arcColor6 = arc6;                                         \
        pMtr->arcColor5 = arc5;                                         \
        pMtr->arcColor4 = arc4;                                         \
        pMtr->arcColor3 = arc3;                                         \
        pMtr->arcColor2 = arc2;                                         \
        pMtr->arcColor1 = arc1;                                         \
    }

/*********************************************************************
* Function: WORD MtrDraw(void *pObj)
*
* Overview: This function renders the object on the screen using 
* 			the current parameter settings. Location of the object is 
*			determined by the left, top, right and bottom parameters. 
*			The colors used are dependent on the state of the object. 
*			The font used is determined by the style scheme set.
*			
*			Depending on the defined settings, value of the meter
*			will displayed or hidden. Displaying the value will require
*			a little bit more rendering time depending on the size
*			of the meter and font used.
*
*			When rendering objects of the same type, each object 
*			must be rendered completely before the rendering of the 
*			next object is started. This is to avoid incomplete 
*			object rendering.
*
* PreCondition: Object must be created before this function is called.
*
* Input: pMtr - Pointer to the object to be rendered.
*        
* Output: Returns the status of the drawing 
*		  - 1 - If the rendering was completed and 
*		  - 0 - If the rendering is not yet finished. 
*		  Next call to the function will resume the 
*		  rendering on the pending drawing state.
*
* Example:
*	See MtrCreate() example.
*
* Side Effects: none
*
********************************************************************/
WORD MtrDraw(void *pObj);

/*********************************************************************
* Macro: MtrSetTitleFont(pMtr, pNewFont)   (((METER*)pMtr)->pTitleFont = pNewFont)
*
* Overview: This function sets the font of title. 
*
* PreCondition: Font must be created before this function is called.
*
* Input: pMtr - Pointer to the object.
*		 pNewFont - Pointer to the new font used for the title.
*        
* Output: N/A
*
* Side Effects: none
*
********************************************************************/
    #define MtrSetTitleFont(pMtr, pNewFont) (((METER *)pMtr)->pTitleFont = pNewFont)

/*********************************************************************
* Macro: MtrSetValueFont(pMtr, pNewFont)   (((METER*)pMtr)->pValueFont = pNewFont)
*
* Overview: This function sets the font of value. 
*
* PreCondition: Font must be created before this function is called.
*
* Input: pMtr - Pointer to the object.
*		 pNewFont - Pointer to the new font used for the value.
*        
* Output: N/A
*
* Side Effects: none
*
********************************************************************/
    #define MtrSetValueFont(pMtr, pNewFont) (((METER *)pMtr)->pValueFont = pNewFont)
#endif // _METER_H
