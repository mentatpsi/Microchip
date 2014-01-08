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
 * Copyright � 2011 Microchip Technology Inc.  All rights reserved.
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
 *****************************************************************************/

/*****************************************************************************
 * SECTION: Includes
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include "MainDemo.h"
#include "gfx_font_resources.h"

extern volatile WORD tick;

/*****************************************************************************
 * SECTION: Defines
 *****************************************************************************/
const SHORT ekgPoints[] ={
    0x0068,0x0068,0x0068,0x0068,0x0068,0x0069,0x0068,0x0068,
    0x0068,0x0067,0x0067,0x0066,0x0066,0x0068,0x0068,0x0068,
    0x0067,0x0066,0x0066,0x0066,0x0066,0x0065,0x0065,0x0065,
    0x0065,0x0065,0x0065,0x0064,0x0063,0x0063,0x0063,0x0064,
    0x0064,0x0065,0x0065,0x0064,0x0064,0x0064,0x0064,0x0065,
    0x0066,0x0066,0x0062,0x0061,0x0060,0x0059,0x0055,0x004a,
    0x0049,0x0047,0x0047,0x0048,0x004a,0x004f,0x0053,0x0056,
    0x005c,0x005d,0x0060,0x0061,0x0062,0x0064,0x0064,0x0064,
    0x0065,0x0066,0x0067,0x0068,0x0068,0x0069,0x0070,0x0070,
    0x0055,0x0030,0x0004,0x0028,0x005d,0x006c,0x0069,0x0069,
    0x0069,0x0069,0x0068,0x0067,0x0065,0x0063,0x0061,0x005e,
    0x005e,0x0062,0x0064,0x0066,0x0067,0x0068,0x0068,0x006a
};
// EKG circular buffer size
#define EKG_BUFFER_SIZE 320

// EKG data circular buffer
static SHORT    ekgBufferToErase[EKG_BUFFER_SIZE];
static WORD     ekgPos;
static SHORT    PosX;
static SHORT 	PosY;
static WORD     var;

void PutStringToEpd( WORD startX, WORD startY, const char* text);

/*****************************************************************************
 * void ResetEKG(void)
 *****************************************************************************/
void ResetEKG(void)
{
    ekgPos = PosX = var = 0;
}

/*****************************************
 * void GraphEKG()
 *****************************************/
void GraphEKG(void)
{
	if ( tick > 300 )
	{
	    static SHORT    prevPosY = -1;
	    
	    tick = 0;
	    
	    PosY = ekgPoints[ekgPos];
	    ekgPos++;
	
	    if(ekgPos >= sizeof(ekgPoints) / sizeof(SHORT))
	    {
	        ekgPos = 0;
	    }	    
	    
	    SHORT erasePrevPot;
	        	
		// Erase 
	    // Line is 2 pixel wide	  
		WORD Pxls = 0xFFFF;	
	  	GFX_DRIVER_LoadImage( SRC_1BIT, 27+PosX, 40+ekgBufferToErase[PosX], 28+PosX, 41+ekgBufferToErase[PosX], &Pxls );

        if(PosX > 0)
        {
            erasePrevPot = ekgBufferToErase[PosX - 1];
        }
        else
        {
            erasePrevPot = ekgBufferToErase[EKG_BUFFER_SIZE - 54 - 1];
        }

        while(erasePrevPot != ekgBufferToErase[PosX])
        {
            GFX_DRIVER_LoadImage( SRC_1BIT, 27 + PosX, 40+erasePrevPot, 29 + PosX, 41 + erasePrevPot, &Pxls );

            if(erasePrevPot > ekgBufferToErase[PosX])
                erasePrevPot--;
            else
                erasePrevPot++;
        }			
		
		// Draw	
		Pxls = 0;	
	  	GFX_DRIVER_LoadImage( SRC_1BIT, 27+PosX, 40+PosY, 28+PosX, 41+PosY, &Pxls );

        if(prevPosY != -1)
	    {
	
	        while(prevPosY != PosY)
	        {
	  	        GFX_DRIVER_LoadImage( SRC_1BIT, 27+PosX, 40+prevPosY, 27+PosX, 41+prevPosY, &Pxls );
	
	            if(prevPosY > PosY)
	                prevPosY--;
	            else
	                prevPosY++;
	        }
	    }
	    
	    prevPosY  = PosY;		
	 	
		// Save to erase later on
		ekgBufferToErase[PosX] = PosY;
		// keep in boundary
		PosX = PosX+1 > 320-56 ? 0 : PosX + 1;
		
		var++;
	    
	    // Set  text output
		if ( PosY != ekgBufferToErase[PosX ? PosX-1 : 0] || ((var&0xFF)==0xFF) )
		{
			char strText[16];
			
	        SetFont((void *)&DroidSans_24);

            sprintf( strText, " %02d.%02d bpm ", ekgPos ? (var&0x8 ? 64 : 65) : 0, ekgPos ? 0x3f&(var>>2) : 0 );
			PutStringToEpd( 32, 204, strText);  
			
			if ( var & 0x8 || (var&0xFF)==0xFF || ! ekgPos )
			{
				sprintf( strText, " SYS. %03d ", ekgPos ? ((var&0x70) == 0x70 ? 134 : 135) : 0 );
				PutStringToEpd( 180, 174, strText);  
			}
			if ( !(var & 0x8 ) || (var&0xFF)==0xFF || ! ekgPos )
			{
				sprintf( strText, " DIA.  %3d ", ekgPos ? ((var&0xE0) == 0x70 ? 85 : 84) : 0 );
				PutStringToEpd( 180, 204, strText);  
			}
	 	}
 	}		
}

/*****************************************
 * void PutStringToEpd()
 *****************************************/
void PutStringToEpd( WORD x, WORD y, const char* text)
{
    GLYPH_ENTRY *pChTable;
    BYTE        *pChImage;
    BYTE        chWidth;
    BYTE 		sign[256];
           
    while(*text != 0)
    { 
		pChTable = (GLYPH_ENTRY *) (((FONT_FLASH *)currentFont.pFont)->address + sizeof(FONT_HEADER)) + ((UXCHAR)*text - (UXCHAR)currentFont.fontHeader.firstChar);    
		pChImage = (BYTE *) (((FONT_FLASH *)currentFont.pFont)->address + ((DWORD)(pChTable->offsetMSB) << 8) + pChTable->offsetLSB);
		chWidth  = (pChTable->width+7)/8;
		chWidth *= 8; 
		   
		memcpy( sign, pChImage, currentFont.fontHeader.height * chWidth / 8 );
		 
		GFX_DRIVER_LoadImage( SRC_1BIT, x, y, x + chWidth - 1, y + (WORD)currentFont.fontHeader.height - 1, (WORD*)sign );
		x += pChTable->width;
		text++;

    }	
}

