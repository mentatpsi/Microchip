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
#include "gfx_font_resources.h"
#include "MainDemo.h"
#include "ID.h"

/*****************************************************************************
 * void CreateWidgets(void)
 *****************************************************************************/
void CreateWidgets( void )
{	
    SldCreate
    (
        ID_SLIDER1,                            // ID
        50,
        50,
        270,
        80,                                    // dimension
        SLD_DRAW,                              // will be dislayed after creation
        100,                                   // range
        10,                                    // page
        50,                                    // pos
        NULL                                   // default GOL scheme
    );      
     
    StCreate
    (
        ID_STATICTEXT3,                        // ID
        50,
        85,
        180,
        115,                                   // dimension
        ST_DRAW | ST_CENTER_ALIGN,             // display text
        (XCHAR *)"Static Text",                // text
        NULL                                   // default GOL scheme
    );

    EbCreate
    (
        ID_EDITBOX1,    			           // ID
        185,
        85,
        270,
        115,                                   // dimension
        EB_DRAW,                               // will be dislayed after creation
        "$10.55",
        10,
        NULL                                   // default GOL scheme
    );                              
     
    CbCreate
    (
        ID_CHECKBOX1,                          // ID
        50,
        120,
        270,
        150,                                   // dimension
        CB_DRAW | CB_CHECKED | CB_DRAW_CHECK,  // will be dislayed after creation
        (XCHAR *)"Check Box",                  // "Scale"
        NULL                                   // default GOL scheme
    );      

    RbCreate
    (
        ID_RADIOBUTTON1,                       // ID
        50,
        155,
        270,
        185,                                   // dimension
        RB_DRAW | RB_GROUP,                    // will be dislayed and focused after creation
                                               // first button in the group
        (XCHAR *)"Radio 1",                    // "Heavy Load"
        NULL                                   // default GOL scheme
    );
    
    RbCreate
    (
        ID_RADIOBUTTON2,                       // ID
        50,
        190,
        270,
        220,                                   // dimension
        RB_DRAW | RB_CHECKED,                  // will be dislayed and checked after creation
        (XCHAR *)"Radio 2",                    // "Normal Load"
        NULL                                   // default GOL scheme
    );                          	  
}

void DeleteWidgets( void )
{  
	GOLDeleteObjectByID(ID_RADIOBUTTON2); 
	GOLDeleteObjectByID(ID_RADIOBUTTON1);
	GOLDeleteObjectByID(ID_CHECKBOX1); 
	GOLDeleteObjectByID(ID_EDITBOX1);
	GOLDeleteObjectByID(ID_STATICTEXT3);
	GOLDeleteObjectByID(ID_SLIDER1);
}  
