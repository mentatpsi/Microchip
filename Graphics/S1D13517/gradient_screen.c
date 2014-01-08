/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        main_screen.c
 * Dependencies:    See Includes Section
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
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
 *****************************************************************************/

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "Graphics/Graphics.h"
//#include "icons.h"
#include "MainDemo.h"//** Remove when complete!!!!!!!!!
#include "gfx_schemes.h"
#include "gfx_screens.h"
#include "gol_ids.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define GRADIENT_SCREEN_SIZE_X  (GetMaxX() - (GetMaxX() >> 2) - 100)
#define GRADIENT_SCREEN_SIZE_Y  (GetMaxY()  - 120)
#define GRADIENT_SIZE_V         (GRADIENT_SCREEN_SIZE_X / 3 )
#define GRADIENT_SIZE_H         (GRADIENT_SCREEN_SIZE_Y >> 1)

/*****************************************************************************
 * void CreateGradientScreen(void)
 *****************************************************************************/
void CreateGradientScreen(void)
{

    GFX_GRADIENT_STYLE scheme1;

    scheme1.gradientType         = GRAD_UP; 
    scheme1.gradientStartColor   = BRIGHTRED;
    scheme1.gradientEndColor     = BLACK;
    scheme1.gradientLength       = GRADIENT_SIZE_H; 

 	SetColor(GFX_SchemeGetCurrentScheme()->Color0);	 
	while(!FillBevel((GetMaxX() >> 2) + 20,90 ,GetMaxX() - 10, GetMaxY()-10,5));
     
	while(!AlphaBlendWindow(GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
					 GFX_PAGE1, (GetMaxX() >> 2) + 15, 85,
					 GetDrawBufferAddress(), (GetMaxX() >> 2) + 15, 85,
				     (GetMaxX())-((GetMaxX() >> 2) + 15), 
				     GetMaxY() - 90,  	
				     GFX_SchemeGetDefaultScheme()->AlphaValue));

    BevelGradient(  (GetMaxX() >> 2) +40, 
   				     120,  
   				     (GetMaxX() >> 2) +40 + (GRADIENT_SCREEN_SIZE_X /3), 
   				     90+(GRADIENT_SCREEN_SIZE_Y >>1),
   				     10, 
   				     scheme1.gradientStartColor,
                     scheme1.gradientEndColor,
                     100,
                     scheme1.gradientType);

   scheme1.gradientType = GRAD_DOWN; 

   BevelGradient(   (GetMaxX() >> 2) +65 + (GRADIENT_SCREEN_SIZE_X /3), 
                    120, 
                    (GetMaxX() >> 2) +65 + ((GRADIENT_SCREEN_SIZE_X <<1)/3), 
                    90+(GRADIENT_SCREEN_SIZE_Y >>1),
                    10, 
                    scheme1.gradientStartColor,
                    scheme1.gradientEndColor,
                    100,
                    scheme1.gradientType);

    scheme1.gradientType = GRAD_LEFT;
    scheme1.gradientLength =GRADIENT_SIZE_V; 

    BevelGradient(  (GetMaxX() >> 2) +90 + ((GRADIENT_SCREEN_SIZE_X <<1)/3), 
                    120, 
                    GetMaxX() - 20, 
                    90+(GRADIENT_SCREEN_SIZE_Y >>1),
                    10, 
                    scheme1.gradientStartColor,
                    scheme1.gradientEndColor,
                    100,
                    scheme1.gradientType);

    scheme1.gradientType = GRAD_RIGHT;

    BevelGradient(  (GetMaxX() >> 2)+ 40, 
                    120+(GRADIENT_SCREEN_SIZE_Y >>1), 
                    (GetMaxX() >> 2) +40 + (GRADIENT_SCREEN_SIZE_X /3), 
                    GetMaxY()-40,
                    10, 
                    scheme1.gradientStartColor,
                    scheme1.gradientEndColor,
                    100,
                    scheme1.gradientType);

   scheme1.gradientType = GRAD_DOUBLE_VER;

   BevelGradient(   (GetMaxX() >> 2) +65 + (GRADIENT_SCREEN_SIZE_X /3), 
                    120+(GRADIENT_SCREEN_SIZE_Y >>1), 
                    (GetMaxX() >> 2) +65 + ((GRADIENT_SCREEN_SIZE_X <<1)/3), 
                    GetMaxY()-40,
                    10, 
                    scheme1.gradientStartColor,
                    scheme1.gradientEndColor,
                    100,
                    scheme1.gradientType);

   scheme1.gradientLength = GRADIENT_SIZE_H;
   scheme1.gradientType = GRAD_DOUBLE_HOR;

   BevelGradient(   (GetMaxX() >> 2) +90 + ((GRADIENT_SCREEN_SIZE_X <<1)/3), 
                    120+(GRADIENT_SCREEN_SIZE_Y >>1), 
                    GetMaxX() - 20, 
                    GetMaxY()-40,
                    10, 
                    scheme1.gradientStartColor,
                    scheme1.gradientEndColor,
                    100,
                    scheme1.gradientType);
}
