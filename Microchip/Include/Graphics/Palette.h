/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Palette Support
 *****************************************************************************
 * FileName:        Palette.h
 * Dependencies:    Graphics.h
 * Processor:       PIC24, PIC32
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/06/09    Initial Release
 * 08/20/10    Modified PALETTE_EXTERNAL to be of type GFX_EXTDATA.
 * 03/20/12    Modified PALETTE_ENTRY structure to have packed attribute. 
 *****************************************************************************/
#ifndef _PALETTE_H
    #define _PALETTE_H

    #include "Graphics/Graphics.h"
    #include "Primitive.h"
    #include "GenericTypeDefs.h"

    #ifdef USE_PALETTE

/*********************************************************************
* Overview: Structure used for the palette entries. 
*           - For TFT: color is defined as 5-6-5 RGB format 
*             (5-bits for RED, 6-bits for GREEN and 5-bits for BLUE.
*           - For Monochrome: 4 bits are used to represent the luma.
*
*********************************************************************/
typedef union __attribute__ ((packed))
{
    WORD    value;						// a 16-bit value representing a color or palette entry
    struct __attribute__ ((packed))
    {
        WORD    r : 5;					// represents the RED component 
        WORD    g : 6;					// represents the GREEN component 
        WORD    b : 5;					// represents the BLUE component 
    } color;							// color value in 5-6-5 RGB format

    struct __attribute__ ((packed))
    {
        WORD    reserved : 12;			// reserved, used as a filler 
        WORD    luma : 4;				// monochrome LUMA value
    } monochrome;						// monochrome LUMA value

} PALETTE_ENTRY;


/*********************************************************************
* Overview: Structure for the palette header. 
*
*********************************************************************/
typedef struct
{
    WORD            id;					// User defined ID
    WORD            length;				// number of palette entries (number of colors in the palette)
} PALETTE_HEADER;

/*********************************************************************
* Overview: Structure for the palette stored in FLASH memory. 
*
*********************************************************************/
typedef struct
{
    SHORT           type;   			// Type must be FLASH
    PALETTE_HEADER  header;				// Contains information on the palette 
    PALETTE_ENTRY   *pPaletteEntry;		// Pointer to the palette. Number of entries is determined by the header.
} PALETTE_FLASH;

/*********************************************************************
* Overview: Structure for palette stored in EXTERNAL memory space.
*           (example: External SPI or parallel Flash, EDS_EPMP)
*
*********************************************************************/
typedef GFX_EXTDATA PALETTE_EXTERNAL;

/*********************************************************************
* Function: void PaletteInit(void)
*
* Overview: Initializes the color look up table (CLUT).
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: All rendering will use the new palette entries.
*
********************************************************************/
void    PaletteInit(void);

/*********************************************************************
* Function: void EnablePalette(void)
*
* Overview: Enables the Palette mode.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects:
*
********************************************************************/
void    EnablePalette(void);

/*********************************************************************
* Function: void DisablePalette(void)
*
* Overview: Disables the Palette mode.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects:
*
********************************************************************/
void    DisablePalette(void);

/*********************************************************************
* Function: BYTE IsPaletteEnabled(void)
*
* Overview: Returns if the Palette mode is enabled or not.
*
* PreCondition: none
*
* Input: none
*
* Output: Returns the palette mode status.
*		  1 - If the palette mode is enabled 
*		  0 - If the palette mode is disabled 
*
* Side Effects:
*
********************************************************************/
BYTE    IsPaletteEnabled(void);

/*********************************************************************
* Function: BYTE GetPaletteChangeError(void)
*
* Overview: Returns the Palette change error status
*
* PreCondition: none
*
* Input: none
*
* Output: Returns the palette change status.
*		  1 - If the palette change error occured
*		  0 - If no palette change error occured
*
* Side Effects: none
*
********************************************************************/
BYTE    GetPaletteChangeError(void);

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
void    ClearPaletteChangeError(void);

/*********************************************************************
* Function: BYTE SetPaletteBpp(BYTE bpp)
*
* Overview: Sets the color look up table (CLUT) number of valid entries.
*
* PreCondition: Palette must be initialized by PaletteInit().
*
* Input: bpp - Bits per pixel
*
* Output: Returns the status of the change.
*		  0 - Change was successful
*		  1 - Change was not successful
*
* Side Effects: none
*
********************************************************************/
BYTE    SetPaletteBpp(BYTE bpp);

/*********************************************************************
* Function: void RequestPaletteChange(void *pPalette, WORD startEntry, WORD length)
*
* Overview: Loads the palettes from the flash during vertical blanking period
*           if possible, otherwise loads immediately.
*
* PreCondition: Palette must be initialized by PaletteInit().
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
void    RequestPaletteChange(void *pPalette, WORD startEntry, WORD length);

/*********************************************************************
* Macro: RequestEntirePaletteChange(pPalette)
*
* Overview: Loads all the palette entries from the flash during
*           vertical blanking period if possible, otherwise
*           loads immediately.
*
* PreCondition: PPalette must be initialized by PaletteInit().
*
* Input: pPalette - Pointer to the palette structure
*
* Output: none
*
* Side Effects: There may be a slight flicker when the Palette entries
*               are getting loaded one by one.
*
********************************************************************/
        #define RequestEntirePaletteChange(pPalette)    RequestPaletteChange(pPalette, 0, 256)

/*********************************************************************
* Function: BYTE SetPalette(void *pPalette, WORD startEntry, WORD length)
*
* Overview: Programs a block of palette entries starting from startEntry and 
*			until startEntry + length from the flash immediately.
*
* PreCondition: Palette must be initialized by PaletteInit().
*
* Input: pPalette   - Pointer to the palette structure
*        startEntry - Start entry to load (inclusive)
*        length     - Number of entries
*
* Output: Returns the status of the palette set.
*		  0 - Set was successful
*		  1 - Set was not successful
*
* Side Effects: There may be a slight flicker when the Palette entries
*               are getting loaded one by one.
*
********************************************************************/
BYTE    SetPalette(void *pPalette, WORD startEntry, WORD length);

/*********************************************************************
* Macro: SetEntirePalette(pPalette)
*
* Overview: Programs the whole 256 entry palette with new color values
*			from flash.
*
* PreCondition: Palette must be initialized by PaletteInit().
*
* Input: pPalette - Pointer to the palette structure
*
* Output: Returns the status of the palette set.
*		  0 - Set was successful
*		  1 - Set was not successful
*
* Side Effects: There may be a slight flicker when the Palette entries
*               are getting loaded one by one.
*
********************************************************************/
        #define SetEntirePalette(pPalette)  SetPalette(pPalette, 0, 256)

/*********************************************************************
* Function: BYTE SetPaletteFlash(PALETTE_ENTRY *pPaletteEntry, WORD startEntry, WORD length)
*
* Overview: Loads the palettes from the flash immediately.
*
* PreCondition: Palette must be initialized by PaletteInit().
*
* Input: pPaletteEntry   - Pointer to the palette table in ROM
*        startEntry      - Start entry to load (inclusive)
*        length          - Number of entries
*
* Output: Returns the status of the palette set.
*		  0 - Set was successful
*		  1 - Set was not successful
*
* Side Effects: There may be a slight flicker when the Palette entries
*               are getting loaded one by one.
*
********************************************************************/
BYTE    SetPaletteFlash(PALETTE_ENTRY *pPaletteEntry, WORD startEntry, WORD length);

    #endif //USE_PALETTE
#endif
