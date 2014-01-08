/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Palette Support
 *****************************************************************************
 * FileName:        Palette.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/06/09     Version 1.0 release
 * 03/20/12     Fixed error on SetPalette() when palette is from external memory.
 *****************************************************************************/
#include "Graphics/Palette.h"

#ifdef USE_PALETTE

extern void StartVBlankInterrupt(void);
BYTE    PaletteBpp;
BYTE    blPaletteChangeError;
void    *pPendingPalette;
WORD    PendingStartEntry;
WORD    PendingLength;

/*********************************************************************
* Function: void RequestPaletteChange(void *pPalette, WORD startEntry, WORD length)
*
* Overview: Loads the palettes from the flash during vertical blanking period
*           if possible, otherwise loads immediately.
*
* PreCondition: PaletteInit() must be called before.
*
* Input: pPalette   - Pointer to the palette structure
*        startEntry - Start entry to load (inclusive)
*        length     - Number of entries
*
* Output: none
*
* Side Effects: There may be a slight flicker when the Palette entries
*               are getting loaded one by one.
*
********************************************************************/
void RequestPaletteChange(void *pPalette, WORD startEntry, WORD length)
{
    pPendingPalette = pPalette;
    PendingStartEntry = startEntry;
    PendingLength = length;
    StartVBlankInterrupt();
}

/*********************************************************************
* Function: BYTE SetPalette(void *pPalette, WORD startEntry, WORD length)
*
* Overview: Loads the palettes from the flash immediately.
*
* PreCondition: PaletteInit() must be called before.
*
* Input: pPalette   - Pointer to the palette structure
*        startEntry - Start entry to load (inclusive)
*        length     - Number of entries
*
* Output: Status: Zero -> Success, Non-zero -> Error.
*
* Side Effects: There may be a slight flicker when the Palette entries
*               are getting loaded one by one.
*
********************************************************************/
BYTE SetPalette(void *pPalette, WORD startEntry, WORD length)
{
    BYTE            status = -1;
    PALETTE_FLASH   *pPaletteFlash;
#ifdef USE_PALETTE_EXTERNAL
    PALETTE_ENTRY   paletteEntry[256];
#endif

    switch(*((SHORT *)pPalette))
    {
        case FLASH:
            pPaletteFlash = (PALETTE_FLASH *)pPalette;
            if(length > pPaletteFlash->header.length)
            {
                length = pPaletteFlash->header.length;
            }

            status = SetPaletteFlash(pPaletteFlash->pPaletteEntry, startEntry, length);
            break;

    #ifdef USE_PALETTE_EXTERNAL
        case EXTERNAL:
            ExternalMemoryCallback(pPalette, (startEntry * sizeof(PALETTE_ENTRY)), sizeof(PALETTE_ENTRY)*length, (void*)paletteEntry);

            status = SetPaletteFlash(paletteEntry, startEntry, length);
            break;
    #endif
    }

    return (status);
}

/*********************************************************************
* Function: BYTE GetPaletteChangeError(void)
*
* Overview: Returns the Palette change error status
*
* PreCondition: none
*
* Input: none
*
* Output: NoError -> Zero; Error -> Non Zero
*
* Side Effects: none
*
********************************************************************/
BYTE GetPaletteChangeError(void)
{
    return (blPaletteChangeError);
}

/*********************************************************************
* Function: void ClearPaletteChangeError(void)
*
* Overview: Clears the Palette change error status
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void ClearPaletteChangeError(void)
{
    blPaletteChangeError = 0;
}

#endif
