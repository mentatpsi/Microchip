/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Analog Clock
 *****************************************************************************
 * FileName:        AnalogClock.c
 * Dependencies:    AnalogClock.h
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
 *****************************************************************************/
#include "Graphics/Graphics.h"

#if defined(USE_ANALOGCLOCK) 

#define THICKNESS_SECOND 4

#define SECOND 1
#define MINUTE 2
#define HOUR 3

/*********************************************************************
* Function: ANALOGCLOCK  *AcCreate(WORD ID, SHORT left,SHORT top,
*                        SHORT right,SHORT bottom,SHORT hour,SHORT minute,
*                        SHORT radius,BOOL sechand,WORD state,void *pBitmap,
*                        GOL_SCHEME  *pScheme)
*
*
* Notes: Creates an ANALOGCLOCK object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned.
*
********************************************************************/
ANALOGCLOCK *AcCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    SHORT       hour,
    SHORT       minute,
    SHORT       radius,
    BOOL		sechand,
    WORD        state,
    void        *pBitmap,
    GOL_SCHEME  *pScheme
)
{
    ANALOGCLOCK  *pAc = NULL;
    pAc = (ANALOGCLOCK *)GFX_malloc(sizeof(ANALOGCLOCK));
    if(pAc == NULL)
        return (NULL);

    pAc->hdr.ID = ID;                   // unique id assigned for referencing
    pAc->hdr.pNxtObj = NULL;            // initialize pointer to NULL
    pAc->hdr.type = OBJ_ANALOGCLOCK;    // set object type
    pAc->hdr.left = left;               // left position
    pAc->hdr.top = top;                 // top position
    pAc->hdr.right = left;              // left position
    pAc->hdr.bottom = top;              // top position
    pAc->radius = radius;
    pAc->valueS = 0;
    pAc->prev_valueS = pAc->valueS;
    pAc->valueM = minute;
    pAc->prev_valueM = pAc->valueM-1;
    pAc->valueH = (hour*5) + (minute/12);
    pAc->prev_valueH = pAc->valueH-1;
    pAc->centerx = (left + ((right-left)>>1)); 
    pAc->centery = (top + ((bottom-top)>>1));
    pAc->pBitmap = pBitmap;             // location of bitmap
    pAc->hdr.state = state;             // state
    pAc->hdr.DrawObj = AcDraw;			// draw function
    pAc->hdr.MsgObj = NULL;             // no message function
    pAc->hdr.MsgDefaultObj = NULL;		// no default message function
    pAc->hdr.FreeObj = NULL;
 
    // Set the color scheme to be used
    if(pScheme == NULL)
        pAc->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pAc->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    GOLAddObject((OBJ_HEADER *)pAc);

    return (pAc);
}

/*********************************************************************
* Function: WORD AcDraw(ANALOGCLOCK *pAc)
*
*
* Notes: This is the state machine to draw the clock.
*
********************************************************************/
WORD AcDraw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        BEVEL_DRAW,
        TICK
    } AC_DRAW_STATES;

    static AC_DRAW_STATES state = REMOVE;
    static SHORT width, height, radius;

    static GFX_COLOR faceClr, handClr;
  
    ANALOGCLOCK *pAc;
    
    pAc = (ANALOGCLOCK *)pObj;
 

   if(IsDeviceBusy())
        return (0);
	
    switch(state)
    {
        case REMOVE:
            if(IsDeviceBusy())
                return (0);

            if(GetState(pAc, AC_HIDE))     // Hide the button (remove from screen)
            {                      
                SetColor(pAc->hdr.pGolScheme->CommonBkColor);
                if(!Bar(pAc->hdr.left, pAc->hdr.top, pAc->hdr.right, pAc->hdr.bottom))
                {
                    return (0);
                }
            }
                      
            if(GetState(pAc, AC_TICK))
            {
                   state = TICK;
                   return(1); //buthave to do goto erase
                //goto erase_current_pos;
            }

            if(GetState(pAc, UPDATE_HOUR))
            {
                   AcHandsDraw(pAc, HOUR, THICKNESS_SECOND+4, handClr, pAc->pBitmap);
                   state = TICK;
                   return(1); //buthave to do goto erase
                //goto erase_current_pos;
            }

            if(GetState(pAc, UPDATE_MINUTE))
            {
                   AcHandsDraw(pAc, MINUTE, THICKNESS_SECOND+4, handClr, pAc->pBitmap);
                   state = TICK;
                   return(1); //buthave to do goto erase
                //goto erase_current_pos;
            }

            if(GetState(pAc, UPDATE_SECOND))
            {
                   AcHandsDraw(pAc, SECOND, THICKNESS_SECOND+4, handClr, pAc->pBitmap);
                   state = TICK;
                   return(1); //buthave to do goto erase
                //goto erase_current_pos;
            }



            radius = pAc->radius;    // get radius
            width = (pAc->hdr.right - pAc->hdr.left) - (radius * 2);  // get width
            height = (pAc->hdr.bottom - pAc->hdr.top) - (radius * 2); // get height
            state = BEVEL_DRAW;

        case BEVEL_DRAW:

            faceClr = pAc->hdr.pGolScheme->Color0;
            handClr = pAc->hdr.pGolScheme->Color1;

            SetLineThickness(NORMAL_LINE);
            SetLineType(SOLID_LINE);

            SetColor(faceClr);

            FillCircle(pAc->centerx,pAc->centery, radius);  //Draw Face of Analog Clock
           
            SetColor(handClr);
            FillCircle(pAc->centerx,pAc->centery, 8);  //Draw Middle of Analog Clock

			AcHandsDraw(pAc, SECOND, THICKNESS_SECOND, handClr, pAc->pBitmap);
			AcHandsDraw(pAc, HOUR, THICKNESS_SECOND+4, handClr, pAc->pBitmap);
			AcHandsDraw(pAc, MINUTE, THICKNESS_SECOND+4, handClr, pAc->pBitmap);

            state = REMOVE;
            break;

		case TICK:
			
			AcHandsDraw(pAc, SECOND, THICKNESS_SECOND, handClr, pAc->pBitmap);				
								
			if(pAc->valueS++ == 60) 
			{
				pAc->valueS = 1;
				if(pAc->valueM++ == 60)
				{
					pAc->valueM = 1;			
				}
				if(pAc->valueM%12 == 0)
				{
					pAc->valueH++;
					if(pAc->valueH == 60)
						pAc->valueH = 0;
					AcHandsDraw(pAc, HOUR, THICKNESS_SECOND+4, handClr, pAc->pBitmap);	
				}			
				AcHandsDraw(pAc, MINUTE, THICKNESS_SECOND+4, handClr, pAc->pBitmap);		
			}
			

			if( ((pAc->valueS - pAc->valueM)<4 && (pAc->valueS - pAc->valueM)>=0) || ((pAc->valueS - pAc->valueM)<-56 && (pAc->valueS - pAc->valueM)>-59) )
				AcHandsDraw(pAc, MINUTE, THICKNESS_SECOND+4, handClr, pAc->pBitmap);
				
			if( ((pAc->valueS - pAc->valueH)<4  && (pAc->valueS - pAc->valueH)>=0) || ((pAc->valueS - pAc->valueH)<-56 && (pAc->valueS - pAc->valueH)>-59) )
				AcHandsDraw(pAc, HOUR, THICKNESS_SECOND+4, handClr, pAc->pBitmap);
				
				
			if(pAc->valueS-1 == pAc->valueM)
				AcHandsDraw(pAc, MINUTE, THICKNESS_SECOND+4, handClr, pAc->pBitmap);
			if(pAc->valueS-1 == pAc->valueH)
				AcHandsDraw(pAc, HOUR, THICKNESS_SECOND+4, handClr, pAc->pBitmap);				
	
            state = REMOVE;
            return (1);
    }

    return (1);
}




/* */
/*********************************************************************
* Function: WORD AcHandsDraw(ANALOGCLOCK *pAc, SHORT hand, SHORT thickness, WORD color, void *pBitmap)
*
*
* Notes: This is the state machine to draw the current hand positions of the clock.
*
********************************************************************/
WORD AcHandsDraw(ANALOGCLOCK *pAc, SHORT hand, SHORT thickness, WORD color, void *pBitmap)
{
	unsigned int templ=0,tempr, temps;
	SHORT 	x1, y1, x2, y2, xi, yi;
	SHORT 	x2_prev, y2_prev, xi_prev, yi_prev;
	static SHORT 	value=0, prev_value=0;
    SHORT   deltaX, deltaY;
    SHORT   error, stepErrorLT, stepErrorGE;
    SHORT   stepX, stepY;
    SHORT   steep;
    SHORT   temp;
    static SHORT	cnt_thick=0;
    SHORT 	radius = pAc->radius;
    BYTE    colorDepth;
    register FLASH_WORD *flashAddress = 0;
    static WORD                sizeX=0, sizeY=0;


			
    // Move pointer to size information
    if(NULL != pBitmap)
    {
        flashAddress = (FLASH_WORD *)((IMAGE_FLASH *)pBitmap)->address + 1;
    	colorDepth = *(flashAddress+1);
        // Read image size
        sizeY = *flashAddress;
        flashAddress++;
        sizeX = *flashAddress;
        flashAddress++;
    }
    
    switch(hand)
    {
		case SECOND:
			value = pAc->valueS;
			prev_value = pAc->prev_valueS; 
			pAc->prev_valueS = pAc->valueS;
            radius -=10; 
			break;  

		case MINUTE:
			value = pAc->valueM;
			prev_value = pAc->prev_valueM; 
			pAc->prev_valueM = pAc->valueM;
			radius -= 3;
			break;  
			
		case HOUR:
			value = pAc->valueH;
			prev_value = pAc->prev_valueH; 
			pAc->prev_valueH = pAc->valueH;
			radius -= 30;
			break;  

		default:
			break;  	   
	} 
    		
    x1=pAc->centerx;
    y1=pAc->centery;

    x2_prev= pAc->centerx;
    y2_prev=pAc->centery;
    x2= pAc->centerx;
    y2=pAc->centery;

    prev_value += 45;
    value += 45;

    //Added for proper orientation
    if(prev_value >= 60) 
        prev_value -= 60;
    
    if(value >= 60) 
        value -= 60;

    GetCirclePoint(radius, (prev_value*6),&x2_prev,&y2_prev);
    GetCirclePoint(radius, (value*6),&x2,&y2);

    x2_prev += pAc->centerx;
    y2_prev +=pAc->centery;
    x2      += pAc->centerx;
    y2      +=pAc->centery;

    xi=pAc->hdr.left;
    yi=pAc->hdr.top;
    xi_prev=(((pAc->hdr.right - radius + pAc->hdr.left + radius) - sizeX) >> 1) + 1;
    yi_prev=(((pAc->hdr.bottom - radius + pAc->hdr.top + radius) - sizeY) >> 1) + 1;

    if(value == 15)
	    x2 += 1;

    if(prev_value == 15)
	    x2_prev += 1;
    	

    if(value == 30)
	    y2 += 1;

    if(prev_value == 30)
	    y2_prev += 1;	
    	
    tempr = (thickness*thickness)>>1;
    temps = ((radius-(11))*(radius-(11)));		
    ////////////////////////////////////////////////////////

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    // Ready 
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif


	
	//#########################################################################################
	//Draw the bitmap of area covered by prev hand
	
    if(x1 == x2_prev)
    {
        if(y1 > y2_prev)
        {
            temp = y1;
            y1 = y2_prev;
            y2_prev = temp;
        }


        for(temp = y1; temp < y2_prev + 1; temp++)   //reDraws at top and bottom
        {
			
				for(cnt_thick=-thickness>>2;cnt_thick<thickness>>2;cnt_thick++)
				{	
					if(prev_value==45)
					templ = ((x1 + cnt_thick-x2_prev )*(x1 + cnt_thick-x2_prev ) + (temp - y1 )*(temp - y1 ));
					else if(prev_value==15)
					templ = ((x1 + cnt_thick-x2_prev )*(x1 + cnt_thick-x2_prev ) + (temp - y2_prev )*(temp - y2_prev ));
					
					
					if(templ>tempr && templ<temps)
					{
                    if(pBitmap != NULL)
					{SetColor(*(flashAddress + sizeX*(temp-yi_prev)+(x1 + cnt_thick - xi_prev)));}
                    else SetColor(pAc->hdr.pGolScheme->Color0);
					PutPixel(x1 + cnt_thick, temp);
					}
				}	
        }

        goto current_hand;
    }

    if(y1 == y2_prev)
    {
        if(x1 > x2_prev)
        {
            temp = x1;
            x1 = x2_prev;
            x2_prev = temp;
        }

        for(temp = x1; temp < x2_prev + 1; temp++)    //redraws at left and right
        {


				for(cnt_thick=-thickness>>2;cnt_thick<thickness>>2;cnt_thick++)
				{
					
					if(prev_value==30)
					templ = ((temp-x1 )*(temp-x1 ) + (y1 + cnt_thick - y2_prev )*(y1 + cnt_thick - y2_prev ));
					else
					templ = ((temp-x2_prev )*(temp-x2_prev ) + (y1 + cnt_thick - y2_prev )*(y1 + cnt_thick - y2_prev ));
					
					
					if(templ>tempr && templ<temps)
					{
                    if(pBitmap != NULL)
					{SetColor(*(flashAddress + sizeX*(y1 + cnt_thick -yi_prev)+(temp - xi_prev)));}
                    else SetColor(pAc->hdr.pGolScheme->Color0);
					PutPixel(temp, y1 + cnt_thick);
					}
				}

        }

        goto current_hand;
    }

    stepX = 0;
    deltaX = x2_prev - x1;
    if(deltaX < 0)
    {
        deltaX = -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY = 0;
    deltaY = y2_prev - y1;
    if(deltaY < 0)
    {
        deltaY = -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep = 0;
    if(deltaX < deltaY)
    {
        ++steep;
        temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = stepX;
        stepX = stepY;
        stepY = temp;
        PutPixel(y1, x1);
    }
    else
    {
        PutPixel(x1, y1);
    }

    // If the current error greater or equal zero
    stepErrorGE = deltaX << 1;

    // If the current error less than zero
    stepErrorLT = deltaY << 1;

    // Error for the first pixel
    error = stepErrorLT - deltaX;

    while(--deltaX >= 0)
    {
        if(error >= 0)
        {
            y1 += stepY;
            error -= stepErrorGE;
        }

        x1 += stepX;
        error += stepErrorLT;


            if(steep)
            {
				for(cnt_thick=-thickness>>2;cnt_thick<thickness>>2;cnt_thick++)
				{
					templ = ((y1 + cnt_thick-x2_prev) *(y1 + cnt_thick-x2_prev) + (x1 - y2_prev)*(x1 - y2_prev));
					
					if(templ>tempr && templ<temps)
					{

                    if(pBitmap != NULL)
					{SetColor(*(flashAddress + sizeX*(x1 -yi_prev)+(y1 + cnt_thick - xi_prev)));}					
                    else SetColor(pAc->hdr.pGolScheme->Color0);

					PutPixel(y1 + cnt_thick,x1);
					}
				}	

            }
            else
            {

				for(cnt_thick=-thickness>>2;cnt_thick<thickness>>2;cnt_thick++)
				{
					templ = ((x1-x2_prev)*(x1-x2_prev) + (y1 + cnt_thick - y2_prev)*(y1 + cnt_thick - y2_prev));
					
					if(templ>tempr && templ<temps)
					{
                        if(pBitmap != NULL){
						SetColor(*(flashAddress + sizeX*(y1 + cnt_thick -yi_prev)+(x1 - xi_prev)));
                        }
                        else SetColor(pAc->hdr.pGolScheme->Color0);
        
						PutPixel(x1,y1 + cnt_thick);
					}
				}

            }
    }   
	//#########################################################################################
	
	//Draw the current hand
current_hand:
	SetColor(color);

    		x1=pAc->centerx;
			y1=pAc->centery;


    if(x1 == x2)
    {
        if(y1 > y2)
        {
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        for(temp = y1; temp < y2 + 1; temp++)
        {
	        
	        		if(value==45)
	        		templ = ((x1 + cnt_thick-x2)*(x1 + cnt_thick-x2) + (temp - y1)*(temp - y1));
	        		else if(value==15)
	        		templ = ((x1 + cnt_thick-x2)*(x1 + cnt_thick-x2) + (temp - y2)*(temp - y2));
					
		
							if(templ>tempr && templ<temps)
						for(cnt_thick=-thickness>>2;cnt_thick<thickness>>2;cnt_thick++)
						{
							PutPixel(x1 + cnt_thick, temp);
						}

        }

        return (1);
    }

    if(y1 == y2)
    {
        if(x1 > x2)
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
        }

        for(temp = x1; temp < x2 + 1; temp++)
        {
	        
	        		if(value==30)
	        		templ = ((temp-x1)*(temp-x1) + (y1 + cnt_thick - y2)*(y1 + cnt_thick - y2));
	        		else// if(value==0)
	        		templ = ((temp-x2)*(temp-x2) + (y1 + cnt_thick - y2)*(y1 + cnt_thick - y2));
					
					
					if(templ>tempr && templ<temps)
					
				for(cnt_thick=-thickness>>2;cnt_thick<thickness>>2;cnt_thick++)
				{
					PutPixel(temp, y1 + cnt_thick);
				}	
        }

        return (1);
    }

    stepX = 0;
    deltaX = x2 - x1;
    if(deltaX < 0)
    {
        deltaX = -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY = 0;
    deltaY = y2 - y1;
    if(deltaY < 0)
    {
        deltaY = -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep = 0;
    if(deltaX < deltaY)
    {
        ++steep;
        temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = stepX;
        stepX = stepY;
        stepY = temp;
        PutPixel(y1, x1);
    }
    else
    {
        PutPixel(x1, y1);
    }

    // If the current error greater or equal zero
    stepErrorGE = deltaX << 1;

    // If the current error less than zero
    stepErrorLT = deltaY << 1;

    // Error for the first pixel
    error = stepErrorLT - deltaX;


    while(--deltaX >= 0)
    {
        if(error >= 0)
        {
            y1 += stepY;
            error -= stepErrorGE;
        }

        x1 += stepX;
        error += stepErrorLT;

            if(steep)
            {
				for(cnt_thick=-thickness>>2;cnt_thick<thickness>>2;cnt_thick++)
				{
					
					templ = ((y1 + cnt_thick-x2) *(y1 + cnt_thick-x2) + (x1 - y2)*(x1 - y2));

					if(templ>tempr && templ<temps)
					PutPixel(y1 + cnt_thick,x1);

				}
            }
            else
            {
				for(cnt_thick=-thickness>>2;cnt_thick<thickness>>2;cnt_thick++)
				{
					templ = ((x1-x2)*(x1-x2) + (y1 + cnt_thick - y2)*(y1 + cnt_thick - y2));

					if(templ>tempr && templ<temps)
					PutPixel(x1,y1 + cnt_thick);
				}	
            }
    }   // end of while
	
    return (1);
}

/*********************************************************************
* Function: AcSetHour(STATICTEXT *pAc, XCHAR *pText)
*
* Notes: Sets the Hour value of time.
*
********************************************************************/
void AcSetHour(ANALOGCLOCK *pAc, SHORT hour)
{

     pAc->valueH = (hour*5) + (pAc->valueM/12);

}

/*********************************************************************
* Function: AcSetMinute(STATICTEXT *pSt, XCHAR *pText)
*
* Notes: Sets the Second value of time.
*
********************************************************************/
void AcSetSecond(ANALOGCLOCK *pAc, SHORT second)
{

     pAc->valueS = second;

}

/*********************************************************************
* Function: AcSetMinute(STATICTEXT *pSt, XCHAR *pText)
*
* Notes: Sets the Minute value of time.
*
********************************************************************/
void AcSetMinute(ANALOGCLOCK *pAc, SHORT minute)
{

     pAc->valueM = minute;

}

#endif 
