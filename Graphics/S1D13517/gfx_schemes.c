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
#include "MainDemo.h"
#include "gfx_schemes.h"
/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
#define RED4        RGBConvert(139, 0, 0)
#define FIREBRICK1  RGBConvert(255, 48, 48)
#define DARKGREEN   RGBConvert(0, 100, 0)
#define PALEGREEN   RGBConvert(152, 251, 152)

#define ALPHA_VAL   50
/*****************************************************************************
 * SECTION: Variables
 *****************************************************************************/
static GOL_SCHEME      *altScheme;                                 // alternative style scheme
static GOL_SCHEME      *altTScheme;                                // alternative style scheme
static GOL_SCHEME      *navScheme;                                 // style scheme for the navigation
static GOL_SCHEME      *redScheme;                                 // alternative red style scheme
static GOL_SCHEME      *greenScheme;                               // alternative green style scheme
static GOL_SCHEME      *yellowScheme;                              // alternative yellow style scheme
static GOL_SCHEME      *redTScheme;                                // alternative red style scheme
static GOL_SCHEME      *greenTScheme;                              // alternative green style scheme
static GOL_SCHEME      *yellowTScheme;                             // alternative yellow style scheme


static GFX_SCHEMES selectedScheme = GFX_SCHEME_1;
static GOL_SCHEME *selectableSchemes[NUM_GFX_SCHEMES];
static GOL_SCHEME *selectableTSchemes[NUM_GFX_SCHEMES];
/*****************************************************************************
 * void GFX_SchemeInit(void)
 *****************************************************************************/
void GFX_SchemeInit(void)
{
    // create the the style schemes
    navScheme = GOLCreateScheme();      // alternative scheme for the navigate buttons
    altScheme = GOLCreateScheme();      // create alternative 1 style scheme
    altTScheme = GOLCreateScheme();     // create alternative 5 style scheme
    redScheme = GOLCreateScheme();      // create red style scheme
    greenScheme = GOLCreateScheme();    // create green style scheme
    yellowScheme = GOLCreateScheme();   // create yellow style scheme
    redTScheme = GOLCreateScheme();      // create red style scheme
    greenTScheme = GOLCreateScheme();    // create green style scheme
    yellowTScheme = GOLCreateScheme();   // create yellow style schemeavigate buttons
    
	navScheme->AlphaValue = ALPHA_VAL;
	
    altScheme->Color0 = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    altScheme->Color1 = RGB565CONVERT(255, 102, 0);
    altScheme->EmbossDkColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altScheme->EmbossLtColor = RGB565CONVERT(0xA9, 0xDB, 0xEF);
    altScheme->ColorDisabled = RGB565CONVERT(0xD4, 0xE1, 0xF7);
    altScheme->TextColor1 = BRIGHTBLUE;
    altScheme->TextColor0 = RGB565CONVERT(255, 102, 0);
    altScheme->TextColorDisabled = RGB565CONVERT(0xB8, 0xB9, 0xBC);
    altScheme->gradientScheme.gradientType = GRAD_LEFT;
    altScheme->gradientScheme.gradientStartColor = WHITE;
    altScheme->gradientScheme.gradientEndColor = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    altScheme->gradientScheme.gradientLength = 50;                
    altScheme->AlphaValue = ALPHA_VAL;

    altTScheme->Color0 = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altTScheme->Color1 = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altTScheme->EmbossDkColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altTScheme->EmbossLtColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altTScheme->ColorDisabled = RGB565CONVERT(0x1E, 0x00, 0xE5);
	altTScheme->CommonBkColor = BRIGHTBLUE;
    altTScheme->TextColor1 = BRIGHTBLUE;
    altTScheme->TextColor0 = RGB565CONVERT(255, 102, 0);
    altTScheme->TextColorDisabled = RGB565CONVERT(0xB8, 0xB9, 0xBC);

    redScheme->Color0 = RGB565CONVERT(0xCC, 0x00, 0x00);
    redScheme->Color1 = BRIGHTRED;
    redScheme->EmbossDkColor = RED4;
    redScheme->EmbossLtColor = FIREBRICK1;
    redScheme->TextColor0 = RGB565CONVERT(0xC8, 0xD5, 0x85);
    redScheme->TextColor1 = BLACK;
    redScheme->AlphaValue = ALPHA_VAL;
    redScheme->gradientScheme.gradientStartColor = WHITE;
    redScheme->gradientScheme.gradientEndColor = RGB565CONVERT(0xCC, 0x00, 0x00);
    redScheme->gradientScheme.gradientLength = 50; 
 
  	redTScheme->Color0 = RED4;
    redTScheme->Color1 = RED4;
    redTScheme->EmbossDkColor = RED4;
    redTScheme->EmbossLtColor = RED4;
    redTScheme->TextColor0 = RED4;
    redTScheme->TextColor1 = RED4;
    redTScheme->gradientScheme.gradientStartColor = WHITE;
    redTScheme->gradientScheme.gradientEndColor = RGB565CONVERT(0xCC, 0x00, 0x00);
    redTScheme->gradientScheme.gradientLength = 50; 
       
    greenScheme->Color0 = RGB565CONVERT(0x23, 0x9E, 0x0A);
    greenScheme->Color1 = BRIGHTGREEN;
    greenScheme->EmbossDkColor = DARKGREEN;
    greenScheme->EmbossLtColor = PALEGREEN;
    greenScheme->TextColor0 = RGB565CONVERT(0xDF, 0xAC, 0x83);
    greenScheme->TextColor1 = BLACK;
    greenScheme->AlphaValue = ALPHA_VAL;
    greenScheme->gradientScheme.gradientStartColor = WHITE;
    greenScheme->gradientScheme.gradientEndColor = RGB565CONVERT(0x23, 0x9E, 0x0A);
    greenScheme->gradientScheme.gradientLength = 50; 

    greenTScheme->Color0 = DARKGREEN;
    greenTScheme->Color1 = DARKGREEN;
    greenTScheme->EmbossDkColor = DARKGREEN;
    greenTScheme->EmbossLtColor = DARKGREEN;
    greenTScheme->TextColor0 = DARKGREEN;
    greenTScheme->TextColor1 = DARKGREEN;
        
    yellowScheme->Color0 = BRIGHTYELLOW;
    yellowScheme->Color1 = YELLOW;
    yellowScheme->EmbossDkColor = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowScheme->EmbossLtColor = RGB565CONVERT(0xFD, 0xFF, 0xB2);
    yellowScheme->TextColor0 = RGB565CONVERT(0xAF, 0x34, 0xF3);
    yellowScheme->TextColor1 = RED;
	yellowScheme->AlphaValue = ALPHA_VAL;
	yellowScheme->gradientScheme.gradientStartColor = WHITE;
    yellowScheme->gradientScheme.gradientEndColor = BRIGHTYELLOW;
    yellowScheme->gradientScheme.gradientLength = 50; 

    yellowTScheme->Color0 = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowTScheme->Color1 = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowTScheme->EmbossDkColor = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowTScheme->EmbossLtColor = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowTScheme->TextColor0 = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowTScheme->TextColor1 = RGB565CONVERT(0xFF, 0x94, 0x4C);
    
    selectableSchemes[GFX_SCHEME_1] = altScheme;
    selectableSchemes[GFX_SCHEME_2] = redScheme;
    selectableSchemes[GFX_SCHEME_3] = greenScheme;
    selectableSchemes[GFX_SCHEME_4] = yellowScheme;

    selectableTSchemes[GFX_SCHEME_1] = altTScheme;
    selectableTSchemes[GFX_SCHEME_2] = redTScheme;
    selectableTSchemes[GFX_SCHEME_3] = greenTScheme;
    selectableTSchemes[GFX_SCHEME_4] = yellowTScheme;
}
/*****************************************************************************
 * void GFX_SchemeSetCurrentScheme(GFX_SCHEMES scheme)
 *****************************************************************************/
void GFX_SchemeSetCurrentSchemeIdx(GFX_SCHEMES scheme)
{
    selectedScheme = scheme;
}
/*****************************************************************************
 * GFX_SCHEMES GFX_SchemeGetCurrentSchemeIdx(void)
 *****************************************************************************/
GFX_SCHEMES GFX_SchemeGetCurrentSchemeIdx(void)
{
    return selectedScheme;
}
/*****************************************************************************
 * GOL_SCHEME *GFX_SchemeGetCurrentScheme(void)
 *****************************************************************************/
GOL_SCHEME *GFX_SchemeGetCurrentScheme(void)
{
    return selectableSchemes[selectedScheme];
}
/*****************************************************************************
 * GOL_SCHEME *GFX_SchemeGetCurrentTScheme(void)
 *****************************************************************************/
GOL_SCHEME *GFX_SchemeGetCurrentTScheme(void)
{
    return selectableTSchemes[selectedScheme];
}
/*****************************************************************************
 * GOL_SCHEME *GFX_SchemeGetDefaultScheme(void)
 *****************************************************************************/
GOL_SCHEME *GFX_SchemeGetDefaultScheme(void)
{
    return altScheme;
}
/*****************************************************************************
 * GOL_SCHEME *GFX_SchemeGetDefaultScheme(void)
 *****************************************************************************/
void GFX_SchemeSetAlpha(short level)
{
    GFX_SCHEMES idx;

    level = Percentage2Alpha(level);

   for(idx = GFX_SCHEME_1; idx < NUM_GFX_SCHEMES; idx++)
   {
        selectableSchemes[idx]->AlphaValue = level;
        selectableTSchemes[idx]->AlphaValue = level;
   }
}
