#define __GIFDECODER_C__
/******************************************************************************

* FileName:        GifDecoder.c
* Dependencies:    Image decoding library; project requires File System library
* Processor:       PIC24/dsPIC30/dsPIC33/PIC32MX
* Compiler:        C30 v2.01/C32 v0.00.18
* Company:         Microchip Technology, Inc.

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

Author                 Date           Comments
--------------------------------------------------------------------------------
Pradeep Budagutta    03-Mar-2008    First release
*******************************************************************************/

#include "Image Decoders/ImageDecoder.h"

#ifdef IMG_SUPPORT_GIF

/*************************/
/**** DATA STRUCTURES ****/
/*************************/
typedef struct _GIFDECODER
{
        IMG_FILE *pImageFile;                       /* Image file pointer */
        WORD wImageWidth;
        WORD wImageHeight;
        WORD wImageX;
        WORD wImageY;
        WORD wScreenWidth;
        WORD wScreenHeight;
        WORD wGlobalPaletteEntries;
        WORD wLocalPaletteEntries;
        BYTE bBgColorIndex;
        BYTE bPixelAspectRatio;
        BYTE blGifMarkerFlag : 1;
        BYTE blGloabalColorTableFlag : 1;
        BYTE blLocalColorTableFlag : 1;
        BYTE blInterlacedFlag : 1;
        BYTE blFirstcodeFlag : 1;
        BYTE bInterlacePass : 3;
        #if GIF_USE_16_BITS_PER_PIXEL == 0
        BYTE aPalette[256][3]; /* Each palette entry has RGB */
        #else
        WORD awPalette[256]; /* Each palette entry has RGB */
        #endif
        /* For decoding */
        BYTE abSymbol[4096];

        #if GIF_CRUSH_PREV_SYMBOL_PTR_TABLE == 0        
        WORD awPrevSymbolPtr[4096];
        #else
        WORD awPrevSymbolPtr[(4096 * 3)/4];
        #endif

        WORD wInitialSymbols;
        WORD wMaxSymbol;
        BYTE bInitialSymbolBits;
        BYTE bMaxSymbolBits;
        LONG lGlobalColorTablePos;
        /* Work memory */
        BYTE bWorkBits;
        BYTE bRemainingDataInBlock;
        BYTE bRemainingBits;
        WORD wCurrentX;
        WORD wCurrentY;
} GIFDECODER;


/**************************/
/****** LOOKUP TABLE ******/
/**************************/
static const WORD GIF_awMask[] = { 0x000, 0x001, 0x003, 0x007, 0x00F, 0x01F, 0x03F, 0x07F, 0x0FF, 0x1FF, 0x3FF, 0x7FF, 0xFFF };

/**************************/
/******* FUNCTIONS  *******/
/**************************/
/*******************************************************************************
Function:       void GIF_vResetData(GIFDECODER *pGifDec)

Precondition:   None

Overview:       This function resets the variables so that new GIF image
                can be decoded

Input:          GIF decoder's data structure

Output:         None
*******************************************************************************/
static void GIF_vResetData(GIFDECODER *pGifDec)
{
    pGifDec->pImageFile = NULL;
    pGifDec->wImageWidth = 0;
    pGifDec->wImageHeight = 0;
    pGifDec->wImageX = 0;
    pGifDec->wImageY = 0;
    pGifDec->wScreenWidth = 0;
    pGifDec->wScreenHeight = 0;
    pGifDec->wGlobalPaletteEntries = 0;
    pGifDec->wLocalPaletteEntries = 0;
    pGifDec->bBgColorIndex = 0;
    pGifDec->bPixelAspectRatio = 0;
    pGifDec->blGifMarkerFlag = 0;
    pGifDec->blGloabalColorTableFlag = 0;
    pGifDec->blLocalColorTableFlag = 0;
    pGifDec->blInterlacedFlag = 0;
    pGifDec->blFirstcodeFlag = 1;
    pGifDec->bInterlacePass = 0;
    pGifDec->wMaxSymbol = 0;
    pGifDec->bMaxSymbolBits = 0;
    pGifDec->wInitialSymbols = 0;
    pGifDec->bInitialSymbolBits = 0;
    pGifDec->bWorkBits = 0;
    pGifDec->bRemainingDataInBlock = 0;
    pGifDec->bRemainingBits = 0;
    pGifDec->wCurrentX = 0;
    pGifDec->wCurrentY = 0;
    pGifDec->lGlobalColorTablePos = 0;
}

/*******************************************************************************
Function:       void GIF_vPutPrevCode(GIFDECODER *pGifDec, WORD wAddress, WORD wCode)

Precondition:   None

Overview:       This function puts the data(code) to the provided address

Input:          GIF decoder's data structure, Address, Data(Code)

Output:         None
*******************************************************************************/
static void GIF_vPutPrevCode(GIFDECODER *pGifDec, WORD wAddress, WORD wCode)
{
    #if GIF_CRUSH_PREV_SYMBOL_PTR_TABLE == 0        
        pGifDec->awPrevSymbolPtr[wAddress] = wCode;
    #else
        WORD wCrushedAddress = (wAddress * 3) / 4;
        if((wAddress & 0x03) == 0)
        {
                  pGifDec->awPrevSymbolPtr[wCrushedAddress] &= 0xF000;
                  pGifDec->awPrevSymbolPtr[wCrushedAddress] |= (wCode & 0x0FFF);
        }
        else if((wAddress & 0x03) == 1)
        {
                  pGifDec->awPrevSymbolPtr[wCrushedAddress] &= 0x0FFF;
                  pGifDec->awPrevSymbolPtr[wCrushedAddress + 1] &= 0xFF00;
                  pGifDec->awPrevSymbolPtr[wCrushedAddress] |= (wCode << 12);
                  pGifDec->awPrevSymbolPtr[wCrushedAddress + 1] |= ((wCode >> 4) & 0x00FF);
        }
        else if((wAddress & 0x03) == 2)
        {
                  pGifDec->awPrevSymbolPtr[wCrushedAddress] &= 0x00FF;
                  pGifDec->awPrevSymbolPtr[wCrushedAddress + 1] &= 0xFFF0;
                  pGifDec->awPrevSymbolPtr[wCrushedAddress] |= (wCode << 8);
                  pGifDec->awPrevSymbolPtr[wCrushedAddress + 1] |= ((wCode >> 8) & 0x000F);
        }
        else
        {
                  pGifDec->awPrevSymbolPtr[wCrushedAddress] &= 0x000F;
                  pGifDec->awPrevSymbolPtr[wCrushedAddress] |= (wCode << 4);
        }
    #endif
}

/*******************************************************************************
Function:       WORD GIF_wGetPrevCode(GIFDECODER *pGifDec, WORD wAddress)

Precondition:   None

Overview:       This function gets the data(code) from the provided address

Input:          GIF decoder's data structure, Address

Output:         Data(Code)
*******************************************************************************/
static WORD GIF_wGetPrevCode(GIFDECODER *pGifDec, WORD wAddress)
{
        WORD wCode;
    #if GIF_CRUSH_PREV_SYMBOL_PTR_TABLE == 0        
        wCode = pGifDec->awPrevSymbolPtr[wAddress];
    #else
        WORD wCrushedAddress = (wAddress * 3) / 4;
        if((wAddress & 0x03) == 0)
        {
                  wCode = (pGifDec->awPrevSymbolPtr[wCrushedAddress] & 0x0FFF);
        }
        else if((wAddress & 0x03) == 1)
        {
                  wCode = (pGifDec->awPrevSymbolPtr[wCrushedAddress] >> 12);
                  wCode |= ((pGifDec->awPrevSymbolPtr[wCrushedAddress + 1] & 0x00FF) << 4);
        }
        else if((wAddress & 0x03) == 2)
        {
                  wCode = (pGifDec->awPrevSymbolPtr[wCrushedAddress] >> 8);
                  wCode |= ((pGifDec->awPrevSymbolPtr[wCrushedAddress + 1] & 0x000F) << 8);
        }
        else
        {
                  wCode = (pGifDec->awPrevSymbolPtr[wCrushedAddress] >> 4);
        }
    #endif
        return wCode;
}

/*******************************************************************************
Function:       void GIF_vInitializeTable(GIFDECODER *pGifDec)

Precondition:   pGifDec->wInitialSymbols must be set to a proper value by
                reading the Header

Overview:       This function initializes the code table to the initial number 
                of symbols

Input:          GIF decoder's data structure

Output:         None
*******************************************************************************/
static void GIF_vInitializeTable(GIFDECODER *pGifDec)
{
        WORD wCounter;
        for(wCounter = 0; wCounter < pGifDec->wInitialSymbols; wCounter++)
        {
                 pGifDec->abSymbol[wCounter] = wCounter;
                 GIF_vPutPrevCode(pGifDec, wCounter, pGifDec->wInitialSymbols);
        }
}

/*******************************************************************************
Function:       void GIF_vReadColorTable(GIFDECODER *pGifDec, WORD wNumberOfEntries)

Precondition:   None

Overview:       This function initializes the code table to the initial number 
                of symbols

Input:          GIF decoder's data structure

Output:         None
*******************************************************************************/
static void GIF_vReadColorTable(GIFDECODER *pGifDec, WORD wNumberOfEntries)
{
        BYTE r, g, b;
        WORD wCounter;
        for(wCounter = 0; wCounter < wNumberOfEntries; wCounter++)
        {
                 IMG_FREAD(&r, sizeof(BYTE), 1, pGifDec->pImageFile); /* R */
                 IMG_FREAD(&g, sizeof(BYTE), 1, pGifDec->pImageFile); /* G */
                 IMG_FREAD(&b, sizeof(BYTE), 1, pGifDec->pImageFile); /* B */
                 #if GIF_USE_16_BITS_PER_PIXEL == 0
                 pGifDec->aPalette[wCounter][0] = r;
                 pGifDec->aPalette[wCounter][1] = g;
                 pGifDec->aPalette[wCounter][2] = b;
                 #else
                 pGifDec->awPalette[wCounter] = RGB565CONVERT(r, g, b);
                 #endif
        }
}

/*******************************************************************************
Function:       BYTE GIF_bReadHeader(GIFDECODER *pGifDec)

Precondition:   None

Overview:       This function reads the GIF file's header information

Input:          GIF decoder's data structure

Output:         Error code - '0' means no error
*******************************************************************************/
static BYTE GIF_bReadHeader(GIFDECODER *pGifDec)
{   
        BYTE abByte[6];
        BYTE bFlags;

        IMG_FREAD(abByte, sizeof(BYTE), 6, pGifDec->pImageFile);  /* Marker */
        if(abByte[0] == 'G' && abByte[1] == 'I' && abByte[2] == 'F' && 
           abByte[3] == '8' && (abByte[4] == '7' || abByte[4] == '9') &&
           abByte[5] == 'a')
        {
                  pGifDec->blGifMarkerFlag = 1;
        }
        else
        {
            return(100);
        }

        IMG_FREAD(&pGifDec->wScreenWidth, sizeof(WORD), 1, pGifDec->pImageFile);
        IMG_FREAD(&pGifDec->wScreenHeight, sizeof(WORD), 1, pGifDec->pImageFile);
        IMG_FREAD(&bFlags, sizeof(BYTE), 1, pGifDec->pImageFile);  /* Packed fields */
        IMG_FREAD(&pGifDec->bBgColorIndex, sizeof(BYTE), 1, pGifDec->pImageFile);
        IMG_FREAD(&pGifDec->bPixelAspectRatio, sizeof(BYTE), 1, pGifDec->pImageFile);
        if(bFlags & 0x80)
        {
                  pGifDec->blGloabalColorTableFlag = 1;
        }
        pGifDec->wGlobalPaletteEntries = 0x01 << ((bFlags & 0x07) + 1);

        if(pGifDec->blGloabalColorTableFlag == 1)
        {
                  pGifDec->lGlobalColorTablePos = IMG_FTELL(pGifDec->pImageFile);
                  GIF_vReadColorTable(pGifDec, pGifDec->wGlobalPaletteEntries);
        }
        return(0);
}

/*******************************************************************************
Function:       BYTE GIF_bReadNextImageDescriptor(GIFDECODER *pGifDec)

Precondition:   File pointer must be pointing to proper location (Like start of
                extension)

Overview:       This function reads the next image descriptor

Input:          GIF decoder's data structure

Output:         Error code - '0' means no error
*******************************************************************************/
static BYTE GIF_bReadNextImageDescriptor(GIFDECODER *pGifDec)
{
       BYTE bSection, bSectionDetails, bBlockSize, bTemp;
       BYTE bFlags;
       BYTE bCounter;
       do
       {
            IMG_FREAD(&bSection, sizeof(BYTE), 1, pGifDec->pImageFile);
            if(bSection == 0x21) /* Extension block */
            {
                        IMG_FREAD(&bSectionDetails, sizeof(BYTE), 1, pGifDec->pImageFile);
                        switch(bSectionDetails)
                        {
/* GRAPHICS EXTENSION */           case 0xF9: IMG_FREAD(&bBlockSize, sizeof(BYTE), 1, pGifDec->pImageFile);
                                              IMG_FSEEK(pGifDec->pImageFile, bBlockSize, 1);
                                              break;
/* PLAIN TEXT EXTENSION */         case 0x01:
/* APPLICATION EXTENSION */        case 0xFF: IMG_FREAD(&bBlockSize, sizeof(BYTE), 1, pGifDec->pImageFile);
                                              IMG_FSEEK(pGifDec->pImageFile, bBlockSize, 1);
/* COMMENT EXTENSION */            case 0xFE: IMG_FREAD(&bBlockSize, sizeof(BYTE), 1, pGifDec->pImageFile);
                                              for(bCounter = 0; bCounter < bBlockSize; bCounter++)
                                              {
                                                    IMG_FREAD(&bTemp, sizeof(BYTE), 1, pGifDec->pImageFile);
                                              }
                                              break;

                                   default:   return(100);
                        }
/* BLOCK TERMINATOR */  IMG_FREAD(&bTemp, sizeof(BYTE), 1, pGifDec->pImageFile);
                        if(bTemp != 0)
                        {
                                   return(100);
                        }
            }
            else if(bSection != 0x2C)
            {
                 return(100);
            }
       }
       while(bSection != 0x2C);

       pGifDec->blFirstcodeFlag = 1;
       pGifDec->bInterlacePass = 0;
       pGifDec->bWorkBits = 0;
       pGifDec->bRemainingDataInBlock = 0;
       pGifDec->bRemainingBits = 0;
       IMG_FREAD(&pGifDec->wImageX, sizeof(WORD), 1, pGifDec->pImageFile);
       IMG_FREAD(&pGifDec->wImageY, sizeof(WORD), 1, pGifDec->pImageFile);
       IMG_FREAD(&pGifDec->wImageWidth, sizeof(WORD), 1, pGifDec->pImageFile);
       IMG_FREAD(&pGifDec->wImageHeight, sizeof(WORD), 1, pGifDec->pImageFile);
       IMG_FREAD(&bFlags, sizeof(BYTE), 1, pGifDec->pImageFile);  /* Packed fields */
       if(bFlags & 0x40)
       {
            pGifDec->blInterlacedFlag = 1;
       }
       if(bFlags & 0x80)
       {
            pGifDec->blLocalColorTableFlag = 1;
       }
       pGifDec->wLocalPaletteEntries = 0x01 << ((bFlags & 0x07) + 1);
       if(pGifDec->blLocalColorTableFlag == 1)
       {
                 GIF_vReadColorTable(pGifDec, pGifDec->wLocalPaletteEntries);
       }
       pGifDec->wCurrentX = pGifDec->wImageX;
       pGifDec->wCurrentY = pGifDec->wImageY;
       return 0;
}

/*******************************************************************************
Function:       WORD GIF_wGetNextByte(GIFDECODER *pGifDec)

Precondition:   None

Overview:       This function reads the next data byte. It also handles the
                data block intra-boundaries

Input:          GIF decoder's data structure

Output:         Data byte, 0xFFFF means error
*******************************************************************************/
static WORD GIF_wGetNextByte(GIFDECODER *pGifDec)
{
       BYTE bByte;
       if(pGifDec->bRemainingDataInBlock == 0)
       {
                 IMG_FREAD(&pGifDec->bRemainingDataInBlock, sizeof(BYTE), 1, pGifDec->pImageFile);
                 if(pGifDec->bRemainingDataInBlock == 0)
                 {
                             return 0xFFFF; /* End of data */
                 }
       }
       IMG_FREAD(&bByte, sizeof(BYTE), 1, pGifDec->pImageFile);
       pGifDec->bRemainingDataInBlock--;
       return (WORD)bByte;
}

/*******************************************************************************
Function:       WORD GIF_wGetNextSymbol(GIFDECODER *pGifDec)

Precondition:   pGifDec->bMaxSymbolBits must be properly updated

Overview:       This function reads the next code symbol brom the data stream

Input:          GIF decoder's data structure

Output:         Next Code, 0xFFFF means error
*******************************************************************************/
static WORD GIF_wGetNextSymbol(GIFDECODER *pGifDec)
{
       WORD wDataBits = 0xFFFF;
       if(pGifDec->bRemainingBits < pGifDec->bMaxSymbolBits)
       {
                 WORD wTemp1, wTemp2;
                 BYTE bBitsRequired;

                 bBitsRequired = pGifDec->bMaxSymbolBits - pGifDec->bRemainingBits;
                 wDataBits = pGifDec->bWorkBits & GIF_awMask[pGifDec->bRemainingBits];
                 wTemp1 = GIF_wGetNextByte(pGifDec);
                 if(wTemp1 == 0xFFFF)
                 {
                             return 0xFFFF;
                 }
                 pGifDec->bWorkBits = (BYTE)wTemp1;
                 if(bBitsRequired > 8)
                 {
                             wTemp2 = GIF_wGetNextByte(pGifDec);
                             if(wTemp2 == 0xFFFF)
                             {
                                       return 0xFFFF;
                             }
                             pGifDec->bWorkBits = (BYTE)wTemp2;
                             wTemp1 |= wTemp2 << 8;
                 }
                 wDataBits |= wTemp1 << pGifDec->bRemainingBits;
                 pGifDec->bRemainingBits = 8 - (bBitsRequired % 8);
                 if(bBitsRequired == 8)
                 {
                             pGifDec->bRemainingBits = 0;
                 }
                 pGifDec->bWorkBits >>= 8 - pGifDec->bRemainingBits;
       }
       else
       {
                 wDataBits = pGifDec->bWorkBits;
                 pGifDec->bRemainingBits -= pGifDec->bMaxSymbolBits;
                 pGifDec->bWorkBits >>= pGifDec->bMaxSymbolBits;
       }
       wDataBits &= GIF_awMask[pGifDec->bMaxSymbolBits];
       return wDataBits;
}

/*******************************************************************************
Function:       void GIF_vPaintData(GIFDECODER *pGifDec, BYTE bData)

Precondition:   pGifDec->blInterlacedFlag must be properly set

Overview:       This function puts the actual pixel on the display. It also
                takes care of the interlaced pixel arrangement.

Input:          GIF decoder's data structure, palette index

Output:         None
*******************************************************************************/
static void GIF_vPaintData(GIFDECODER *pGifDec, BYTE bData)
{
    #if GIF_USE_16_BITS_PER_PIXEL == 0
        IMG_vSetColor(pGifDec->aPalette[bData][0], pGifDec->aPalette[bData][1], pGifDec->aPalette[bData][2]);
    #else
        IMG_vSetColor565(pGifDec->awPalette[bData]);
    #endif
       IMG_vPutPixel(pGifDec->wCurrentX, pGifDec->wCurrentY);
       pGifDec->wCurrentX++;
       if(pGifDec->wCurrentX >= pGifDec->wImageWidth)
       {
                IMG_vLoopCallback();
                pGifDec->wCurrentX = pGifDec->wImageX;
                if(pGifDec->blInterlacedFlag == 0)
                {
                             pGifDec->wCurrentY++;
                }
                else
                {
                             switch(pGifDec->bInterlacePass)
                             {
                               case 0: pGifDec->wCurrentY += 8;
                                       break;
                               case 1: pGifDec->wCurrentY += 8;
                                       break;
                               case 2: pGifDec->wCurrentY += 4;
                                       break;
                               case 3: pGifDec->wCurrentY += 2;
                                       break;
                             }
                             if(pGifDec->wCurrentY - pGifDec->wImageY >= pGifDec->wImageHeight)
                             {
                                       switch(pGifDec->bInterlacePass)
                                       {
                                         case 0: pGifDec->wCurrentY = pGifDec->wImageY + 4;
                                                 break;
                                         case 1: pGifDec->wCurrentY = pGifDec->wImageY + 2;
                                                 break;
                                         case 2: pGifDec->wCurrentY = pGifDec->wImageY + 1;
                                                 break;
                                         case 3: pGifDec->wCurrentY = pGifDec->wImageY + 0;
                                                 break;
                                       }
                                       pGifDec->bInterlacePass++;
                             }
                }
       }
}

/*******************************************************************************
Function:       void GIF_vDisplayCode(GIFDECODER *pGifDec, WORD wCode)

Precondition:   None

Overview:       This function puts the stream of pixels corresponding to the
                code. This uses recursion.

Input:          GIF decoder's data structure, code

Output:         None
*******************************************************************************/
static void GIF_vDisplayCode(GIFDECODER *pGifDec, WORD wCode)
{
       WORD wPrevCode = GIF_wGetPrevCode(pGifDec, wCode);
       if(wPrevCode != pGifDec->wInitialSymbols)
       {
                GIF_vDisplayCode(pGifDec, wPrevCode);
       }
       GIF_vPaintData(pGifDec, pGifDec->abSymbol[wCode]);
}

/*******************************************************************************
Function:       BYTE GIF_bTraceFirstData(GIFDECODER *pGifDec, WORD wCode)

Precondition:   None

Overview:       This function gets the first symbol of the code

Input:          GIF decoder's data structure, code

Output:         First symbol of the code
*******************************************************************************/
static BYTE GIF_bTraceFirstData(GIFDECODER *pGifDec, WORD wCode)
{
       while(GIF_wGetPrevCode(pGifDec, wCode) != pGifDec->wInitialSymbols)
       {
                wCode = GIF_wGetPrevCode(pGifDec, wCode);
       }
       return pGifDec->abSymbol[wCode];
}

/*******************************************************************************
Function:       GIF_vExecuteClearCode(GIFDECODER *pGifDec)

Precondition:   None

Overview:       This function sets the code table to the initial condition

Input:          GIF decoder's data structure

Output:         None
*******************************************************************************/
static void GIF_vExecuteClearCode(GIFDECODER *pGifDec)
{
       pGifDec->wMaxSymbol = pGifDec->wInitialSymbols;
       pGifDec->bMaxSymbolBits = pGifDec->bInitialSymbolBits;
       pGifDec->blFirstcodeFlag = 1;
}

/*******************************************************************************
Function:       BYTE GIF_bDecodeNextImage(GIFDECODER *pGifDec)

Precondition:   Header must be read and file pointer should point to the end
                of the previous image

Overview:       This function sets the code table to the initial condition

Input:          GIF decoder's data structure

Output:         Error code - '0' means no error
*******************************************************************************/
static BYTE GIF_bDecodeNextImage(GIFDECODER *pGifDec)
{
        BYTE bBlockTerminator;
        WORD wCode;
        if(GIF_bReadNextImageDescriptor(pGifDec) != 0)
        {
            return(100);
        }

        IMG_FREAD(&pGifDec->bMaxSymbolBits, sizeof(BYTE), 1, pGifDec->pImageFile);
        if(pGifDec->bMaxSymbolBits > 11)
        {
            return(100);
        }
        pGifDec->wMaxSymbol = (0x01 << pGifDec->bMaxSymbolBits) + 1;
        pGifDec->bMaxSymbolBits++;
        pGifDec->wInitialSymbols = pGifDec->wMaxSymbol;
        pGifDec->bInitialSymbolBits = pGifDec->bMaxSymbolBits;
        GIF_vInitializeTable(pGifDec);

        /* Actual decoding starts here */
        while(!IMG_FEOF(pGifDec->pImageFile))
        {
                IMG_vCheckAndAbort();
                wCode = GIF_wGetNextSymbol(pGifDec);
                if(wCode >= 4096)
                {
                         return(100);
                }
                if(wCode == pGifDec->wInitialSymbols) /* End code */
                {
                         break;
                }
                if(wCode == pGifDec->wInitialSymbols - 1) /* Clear code */
                {
                         GIF_vExecuteClearCode(pGifDec);
                         continue;
                }
                if(wCode <= pGifDec->wMaxSymbol) /* Code exists */
                {
                         GIF_vDisplayCode(pGifDec, wCode);
                         if(pGifDec->blFirstcodeFlag == 0 && pGifDec->wMaxSymbol < 4095)
                         {
                                   pGifDec->wMaxSymbol++;
                                   pGifDec->abSymbol[pGifDec->wMaxSymbol] = GIF_bTraceFirstData(pGifDec, wCode);
                         }
                         if(pGifDec->wMaxSymbol < 4095)
                         {
                                   GIF_vPutPrevCode(pGifDec, pGifDec->wMaxSymbol + 1, wCode);
                         }
                         pGifDec->blFirstcodeFlag = 0;
                }
                else if(wCode == pGifDec->wMaxSymbol + 1 && wCode <= 4095)
                {
                         if(pGifDec->blFirstcodeFlag == 1)
                         {
                                   return(100);
                         }
                         pGifDec->wMaxSymbol++;
                         pGifDec->abSymbol[pGifDec->wMaxSymbol] = GIF_bTraceFirstData(pGifDec, GIF_wGetPrevCode(pGifDec, pGifDec->wMaxSymbol));
                         GIF_vDisplayCode(pGifDec, wCode);
                         if(pGifDec->wMaxSymbol < 4095)
                         {
                                   GIF_vPutPrevCode(pGifDec, pGifDec->wMaxSymbol + 1, wCode);
                         }
                }
                else
                {
                         return(100);
                }

                if((pGifDec->bMaxSymbolBits < 12) && (pGifDec->wMaxSymbol >= (0x01 << pGifDec->bMaxSymbolBits) - 1))
                {
                         pGifDec->bMaxSymbolBits++;
                }
        }
        if(pGifDec->blLocalColorTableFlag == 1) /* Restore Global color table */
        {
                if(pGifDec->lGlobalColorTablePos > 0)
                {
                         LONG lFilePos = IMG_FTELL(pGifDec->pImageFile);
                         IMG_FSEEK(pGifDec->pImageFile, pGifDec->lGlobalColorTablePos, 0);
                         GIF_vReadColorTable(pGifDec, pGifDec->wGlobalPaletteEntries);
                         IMG_FSEEK(pGifDec->pImageFile, lFilePos, 0);
                }
        }
        IMG_FREAD(&bBlockTerminator, sizeof(BYTE), 1, pGifDec->pImageFile);
        if(bBlockTerminator == 0)
        {
                BYTE bTemp;
                IMG_FREAD(&bTemp, sizeof(BYTE), 1, pGifDec->pImageFile);
                if(bTemp == 0x3B) /* End of GIF stream */
                {
                         return 0x3B;
                }
                IMG_FSEEK(pGifDec->pImageFile, -1, 1);
        }
        return bBlockTerminator;
}

/*******************************************************************************
Function:       BYTE GIF_bDecode(IMG_FILE *pFile)

Precondition:   None

Overview:       This function decodes and displays a GIF image

Input:          Image file

Output:         Error code - '0' means no error
*******************************************************************************/
BYTE GIF_bDecode(IMG_FILE *pFile)
{
        GIFDECODER GifDec;

        GIF_vResetData(&GifDec);
        GifDec.pImageFile = pFile;
        GIF_bReadHeader(&GifDec);
        if(GifDec.blGifMarkerFlag == 0)
        {
            return(100);
        }
        IMG_wImageWidth = GifDec.wScreenWidth;
        IMG_wImageHeight = GifDec.wScreenHeight;
        IMG_vSetboundaries();
        return GIF_bDecodeNextImage(&GifDec);
}

#endif
#undef __GIFDECODER_C__
