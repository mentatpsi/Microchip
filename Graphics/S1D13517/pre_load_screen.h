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

#ifndef PRE_LOAD_SCREEN_H_FILE
#define PRE_LOAD_SCREEN_H_FILE

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "GenericTypeDefs.h"

/*****************************************************************************
 * SECTION: Enumerations
 *****************************************************************************/
typedef enum
{
    PRE_LOAD_PAGE_1,
    PRE_LOAD_PAGE_2,
    PRE_LOAD_PAGE_3,
    PRE_LOAD_PAGE_4,
    PRE_LOAD_PAGE_5,
    PRE_LOAD_PAGE_6,
    PRE_LOAD_PAGE_7,
    NUM_PRE_LOAD_PAGES
}PRE_LOAD_PAGE;

typedef enum
{
    PRE_LOAD_BACKGROUND_SCHEME_RAINBOW,
    PRE_LOAD_BACKGROUND_SCHEME_BLUE,
    NUM_PRE_LOAD_BACKGROUND_SCHEMES

}PRE_LOAD_BACKGROUND;
/*****************************************************************************
 * SECTION: Externs
 *****************************************************************************/
extern PRE_LOAD_PAGE preLoadPage;
extern PRE_LOAD_BACKGROUND preLoadBackGroundScheme;
/*****************************************************************************
 * SECTION: Inline Functions
 *****************************************************************************/
extern inline void __attribute__((always_inline)) SetPreLoadPage(PRE_LOAD_PAGE page)
{
    preLoadPage = page;
}

extern inline void __attribute__((always_inline)) SetPreLoadBackGroundScheme(PRE_LOAD_BACKGROUND background)
{
    preLoadBackGroundScheme = background;
}

/*****************************************************************************
 * SECTION: Function protypes
 *****************************************************************************/
void CreatePreLoadScreen(void);
WORD PreLoadDrawScreens(void);

#endif
