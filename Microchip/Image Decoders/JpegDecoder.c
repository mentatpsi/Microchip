#define __JPEGDECODER_C__
/******************************************************************************

* FileName:        JpegDecoder.c
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
                     03-Sep-2010    Changed paint block to more efficient code.
*******************************************************************************/

#include "Image Decoders/ImageDecoder.h"

#ifdef IMG_SUPPORT_JPEG

#define JPEG_SAMPLE_1x1 0
#define JPEG_SAMPLE_1x2 1
#define JPEG_SAMPLE_2x1 2
#define JPEG_SAMPLE_2x2 3

/*****************************/
/**** FUNCTION PROTOTYPES ****/
/*****************************/
void jpeg_idct_islow (SHORT *inbuf, WORD *quantptr);

/*************************/
/**** DATA STRUCTURES ****/
/*************************/
typedef struct _JPEGDECODER
{
        IMG_FILE *pImageFile;                       /* Image file pointer */

        /*********** From APP0 ***********/
        BYTE blJFIF;                            /* JFIF marker found flag */
        BYTE bMajorRev;                         /* Should be 1 */
        BYTE bMinorRev;                         /* Should be 0-2 but is not a show stopper */
        /*------- The x/y densities and thumbnail data are ignored --------*/

        /*********** From SOF0 ***********/
        BYTE bDataBits;                         /* Data precision, can be 8(, 12 or 16) */
        WORD wWidth;                            /* Width in pixels */
        WORD wHeight;                           /* Height in pixels */
        BYTE bChannels;                         /* Number of channels e.g. YCbCr = 3 */
        BYTE abChannelType[MAX_CHANNELS];
        BYTE abChannelHSampFactor[MAX_CHANNELS];
        BYTE abChannelVSampFactor[MAX_CHANNELS];
        BYTE abChannelQuantTableMap[MAX_CHANNELS];

        /*********** From DQT ***********/
        BYTE blQuantUses16bits;                 /* If flag is set, it is an error as 16 bit is not supported */
        WORD awQuantTable[MAX_CHANNELS][64];    /* Supports only 8 & 16 bit resolutions */

        /*********** From DRI ***********/
        WORD wRestartInterval;                  /* The restart interval in blocks */

        /*********** From DHT ***********/
        BYTE bHuffTables;
        BYTE abHuffAcSymLen[MAX_HUFF_TABLES][16];   /* Supports only 8 bit resolution */
        BYTE abHuffAcSymbol[MAX_HUFF_TABLES][256];  /* Maximum possible symbols are 256 */
        BYTE abHuffDcSymLen[MAX_HUFF_TABLES][16];   /* Supports only 8 bit resolution */
        BYTE abHuffDcSymbol[MAX_HUFF_TABLES][16];   /* Maximum possible symbols are 16 for DC :-) */
        /*********** For Huffman-Decoding ***********/
        WORD awHuffAcSymStart[MAX_HUFF_TABLES][16]; /* Starting symbol for each length */
        WORD awHuffDcSymStart[MAX_HUFF_TABLES][16]; /* Starting symbol for each length */

        /*********** From SOS ***********/
        BYTE abChannelHuffAcTableMap[MAX_CHANNELS];
        BYTE abChannelHuffDcTableMap[MAX_CHANNELS];

        BYTE bError;

        /*********** Work memory ***********/
        WORD wWorkBits;
        BYTE bBitsAvailable;
        BYTE bBlocksInOnePass;
        SHORT asOneBlock[MAX_BLOCKS][64];     /* Temporary storage for a 8x8 block */
        WORD  wBlockNumber;
        BYTE  abChannelMap[MAX_BLOCKS];
        BYTE  bSubSampleType;
        SHORT asPrevDcValue[MAX_CHANNELS];
        BYTE *pbCurrentHuffSymLenTable;
        BYTE *pbCurrentHuffSymbolTable;
        WORD *pwCurrentHuffSymStartTable;
        WORD *pwCurrentQuantTable;
        BYTE abDataBuffer[MAX_DATA_BUF_LEN];
        WORD wBufferLen;
        WORD wBufferIndex;
        BYTE bFirstBit;

        WORD wPrevX;
        WORD wPrevY;
} JPEGDECODER;

/**************************/
/**** CONSTANT TABLES  ****/
/**************************/
static const BYTE abZigzag[64] = 
{
  0,  1,  8,  16, 9,  2,  3,  10,
  17, 24, 32, 25, 18, 11, 4,  5,
  12, 19, 26, 33, 40, 48, 41, 34,
  27, 20, 13, 6,  7,  14, 21, 28,
  35, 42, 49, 56, 57, 50, 43, 36,
  29, 22, 15, 23, 30, 37, 44, 51,
  58, 59, 52, 45, 38, 31, 39, 46,
  53, 60, 61, 54, 47, 55, 62, 63
};

/**************************/
/******* FUNCTIONS  *******/
/**************************/

/*******************************************************************************
Function:       void JPEG_vResetDecoder(JPEGDECODER *pJpegDecoder)

Precondition:   None

Overview:       This function resets the variables so that new jpeg image can be
                decoded

Input:          JPEGDECODER

Output:         None
*******************************************************************************/
static void JPEG_vResetDecoder(JPEGDECODER *pJpegDecoder)
{
        WORD wIndex;
        BYTE *pbptr = (BYTE*)pJpegDecoder;
        for(wIndex = 0; wIndex < sizeof(JPEGDECODER); wIndex++)
        {
                  pbptr[wIndex] = 0;
        }
}

/*******************************************************************************
Function:       WORD JPEG_wReadWord(IMG_FILE *pfile)

Precondition:   None

Overview:       This function reads and returns a single word obtained as
                Higher byte first followed by lower byte

Input:          Image file

Output:         One word
*******************************************************************************/
static WORD JPEG_wReadWord(IMG_FILE *pfile)
{
     BYTE btemp;
     WORD wData;
     
     IMG_FREAD(&btemp, sizeof(btemp), 1, pfile);
     wData = btemp;
     IMG_FREAD(&btemp, sizeof(btemp), 1, pfile);
     wData = (wData << 8) | btemp;
     
     return wData;
}

/*******************************************************************************
Function:       BYTE JPEG_bReadByte(IMG_FILE *pfile)

Precondition:   None

Overview:       This function reads and returns a single byte from the file

Input:          Image file

Output:         One byte
*******************************************************************************/
static BYTE JPEG_bReadByte(IMG_FILE *pfile)
{
     BYTE bData;
     
     IMG_FREAD(&bData, sizeof(bData), 1, pfile);

     return bData;
}

/*******************************************************************************
Function:       BYTE JPEG_bReadHeader(JPEGDECODER *pJpegDecoder)

Precondition:   None

Overview:       This function reads the JPEG file header and 
                fills the data structure

Input:          JPEGDECODER

Output:         Error code - '0' means no error
*******************************************************************************/
static BYTE JPEG_bReadHeader(JPEGDECODER *pJpegDecoder)
{
     BYTE blSOSOver = FALSE;
     while(!IMG_FEOF(pJpegDecoder->pImageFile))
     {
             BYTE btemp, bcount, bsection;
             WORD wSegLen, wOffset;
             
             if(blSOSOver == TRUE)
             {
                     if(pJpegDecoder->bChannels == 1)
                     {
                             pJpegDecoder->bBlocksInOnePass = 1;
                             pJpegDecoder->bSubSampleType = JPEG_SAMPLE_1x1;
                     }    
                     else if(pJpegDecoder->bChannels == 3)
                     {
                             if((pJpegDecoder->abChannelHSampFactor[0] == 1 && pJpegDecoder->abChannelVSampFactor[0] == 1) &&
                                (pJpegDecoder->abChannelHSampFactor[1] == 1 && pJpegDecoder->abChannelVSampFactor[1] == 1) &&
                                (pJpegDecoder->abChannelHSampFactor[2] == 1 && pJpegDecoder->abChannelVSampFactor[2] == 1))
                             {
                                     pJpegDecoder->bBlocksInOnePass = 3;
                                     pJpegDecoder->abChannelMap[0] = 0;
                                     pJpegDecoder->abChannelMap[1] = 1;
                                     pJpegDecoder->abChannelMap[2] = 2;
                                     pJpegDecoder->bSubSampleType = JPEG_SAMPLE_1x1;
                             }
                             else if((pJpegDecoder->abChannelHSampFactor[0] == 1 && pJpegDecoder->abChannelVSampFactor[0] == 2) &&
                                     (pJpegDecoder->abChannelHSampFactor[1] == 1 && pJpegDecoder->abChannelVSampFactor[1] == 1) &&
                                     (pJpegDecoder->abChannelHSampFactor[2] == 1 && pJpegDecoder->abChannelVSampFactor[2] == 1))
                             {
                                     pJpegDecoder->bBlocksInOnePass = 4;
                                     pJpegDecoder->abChannelMap[0] = 0;
                                     pJpegDecoder->abChannelMap[1] = 0;
                                     pJpegDecoder->abChannelMap[2] = 1;
                                     pJpegDecoder->abChannelMap[3] = 2;
                                     pJpegDecoder->bSubSampleType = JPEG_SAMPLE_1x2;
                             }
                             else if((pJpegDecoder->abChannelHSampFactor[0] == 2 && pJpegDecoder->abChannelVSampFactor[0] == 1) &&
                                     (pJpegDecoder->abChannelHSampFactor[1] == 1 && pJpegDecoder->abChannelVSampFactor[1] == 1) &&
                                     (pJpegDecoder->abChannelHSampFactor[2] == 1 && pJpegDecoder->abChannelVSampFactor[2] == 1))
                             {
                                     pJpegDecoder->bBlocksInOnePass = 4;
                                     pJpegDecoder->abChannelMap[0] = 0;
                                     pJpegDecoder->abChannelMap[1] = 0;
                                     pJpegDecoder->abChannelMap[2] = 1;
                                     pJpegDecoder->abChannelMap[3] = 2;
                                     pJpegDecoder->bSubSampleType = JPEG_SAMPLE_2x1;
                             }
                             else if((pJpegDecoder->abChannelHSampFactor[0] == 2 && pJpegDecoder->abChannelVSampFactor[0] == 2) &&
                                     (pJpegDecoder->abChannelHSampFactor[1] == 1 && pJpegDecoder->abChannelVSampFactor[1] == 1) &&
                                     (pJpegDecoder->abChannelHSampFactor[2] == 1 && pJpegDecoder->abChannelVSampFactor[2] == 1))
                             {
                                     pJpegDecoder->bBlocksInOnePass = 6;
                                     pJpegDecoder->abChannelMap[0] = 0;
                                     pJpegDecoder->abChannelMap[1] = 0;
                                     pJpegDecoder->abChannelMap[2] = 0;
                                     pJpegDecoder->abChannelMap[3] = 0;
                                     pJpegDecoder->abChannelMap[4] = 1;
                                     pJpegDecoder->abChannelMap[5] = 2;
                                     pJpegDecoder->bSubSampleType = JPEG_SAMPLE_2x2;
                             }
                             else
                             {
                                     JPEG_SendError(100);
                             }
                     }
                     return 0;
             }

             IMG_FREAD(&btemp, sizeof(btemp), 1, pJpegDecoder->pImageFile);

             if(btemp != 0xFF)
             {
                     continue;
             }

             IMG_FREAD(&bsection, sizeof(bsection), 1, pJpegDecoder->pImageFile);
             switch(bsection)
             {
               case SOI:
               case TEM:
               case EOI:
               case RST0:
               case RST1:
               case RST2:
               case RST3:
               case RST4:
               case RST5:
               case RST6:
               case RST7: break;

               case SOF0: /* Start of frame */
                          wSegLen = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          if(wSegLen <= 8)
                          {
                                     JPEG_SendError(100);
                                     break;
                          }
                          pJpegDecoder->bDataBits = JPEG_bReadByte(pJpegDecoder->pImageFile);
                          pJpegDecoder->wHeight   = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          pJpegDecoder->wWidth    = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          pJpegDecoder->bChannels = JPEG_bReadByte(pJpegDecoder->pImageFile);
                          if(wSegLen < 8 + (pJpegDecoder->bChannels * 3))
                          {
                                     JPEG_SendError(100);
                                     break;
                          }
                          for(bcount = 0; bcount < pJpegDecoder->bChannels; bcount++)
                          {
                                     pJpegDecoder->abChannelType[bcount] = JPEG_bReadByte(pJpegDecoder->pImageFile);
                                     btemp = JPEG_bReadByte(pJpegDecoder->pImageFile);
                                     pJpegDecoder->abChannelHSampFactor[bcount] = btemp >> 4;
                                     pJpegDecoder->abChannelVSampFactor[bcount] = btemp & 0x0F;
                                     pJpegDecoder->abChannelQuantTableMap[bcount] = JPEG_bReadByte(pJpegDecoder->pImageFile);
                          }
                          break;

               case APP0: /* Start of Application */
                          wSegLen = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          if(wSegLen < 16)
                          {
                                     JPEG_SendError(100);
                                     break;
                          }
                          else
                          {
                                     unsigned char buf[5];
                                     IMG_FREAD(buf, sizeof(buf[0]), 5, pJpegDecoder->pImageFile);
                                     if(buf[0] == 'J' && buf[1] == 'F' && buf[2] == 'I' && buf[3] == 'F' && buf[4] == '\0')
                                     {
                                               pJpegDecoder->blJFIF = TRUE;
                                     }
                          }
                          pJpegDecoder->bMajorRev = JPEG_bReadByte(pJpegDecoder->pImageFile);
                          pJpegDecoder->bMinorRev = JPEG_bReadByte(pJpegDecoder->pImageFile);
                          if(pJpegDecoder->bMajorRev != 1)
                          {
                                     JPEG_SendError(100);
                                     break;
                          }
                          /* Ignore other bytes in this segment */
                          break;

               case DRI: /* Start of Quantization table */
                          wSegLen = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          if(wSegLen == 4)
                          {
                                     pJpegDecoder->wRestartInterval = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          }
                          break;

               case DQT: /* Start of Quantization table */
                          wSegLen = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          if(wSegLen < 67)
                          {
                                     JPEG_SendError(100);
                                     break;
                          }

                          do
                          {
                                     BYTE bQTableIndex, bCounter;

                                     btemp = JPEG_bReadByte(pJpegDecoder->pImageFile);
                                     bQTableIndex = btemp & 0x0F;
                                     pJpegDecoder->blQuantUses16bits |= (btemp >> 4);
                                     
                                     for(bCounter = 0; bCounter < 64; bCounter++)
                                     {
                                               BYTE bData1, bData2 = 0;
                                               if(pJpegDecoder->blQuantUses16bits == 0)
                                               {
                                                         IMG_FREAD(&bData1, sizeof(BYTE), 1, pJpegDecoder->pImageFile);
                                                         pJpegDecoder->awQuantTable[bQTableIndex][abZigzag[bCounter]] = bData1;
                                               }
                                               else
                                               {
                                                         IMG_FREAD(&bData1, sizeof(BYTE), 1, pJpegDecoder->pImageFile);
                                                         IMG_FREAD(&bData2, sizeof(BYTE), 1, pJpegDecoder->pImageFile);
                                                         pJpegDecoder->awQuantTable[bQTableIndex][abZigzag[bCounter]] = (((WORD)bData1) << 8) + (WORD)bData2;
                                               }
                                     }
                                     wSegLen -= (pJpegDecoder->blQuantUses16bits == 0)? 65: 129; /* Table length + information byte */
                          } while(wSegLen >= 67);
                          break;

               case DHT: /* Start of Huffmann table */
                          wSegLen = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          if(wSegLen < 19)
                          {
                                     JPEG_SendError(100);
                                     break;
                          }

                          do
                          {
                                     BYTE bHTableIndex, bCounter;
                                     WORD wNumOfSymbols;
                                     BYTE blIsAc;
                                     BYTE *pbLenTable, *pbSymTable;

                                     btemp = JPEG_bReadByte(pJpegDecoder->pImageFile);
                                     bHTableIndex = btemp & 0x0F;
                                     blIsAc = (btemp >> 4) & 0x01;
                                     
                                     if(blIsAc == 0)
                                     {
                                               pbLenTable = (BYTE*)&pJpegDecoder->abHuffDcSymLen[bHTableIndex][0];
                                               pbSymTable = (BYTE*)&pJpegDecoder->abHuffDcSymbol[bHTableIndex][0];
                                     }
                                     else
                                     {
                                               pbLenTable = (BYTE*)&pJpegDecoder->abHuffAcSymLen[bHTableIndex][0];
                                               pbSymTable = (BYTE*)&pJpegDecoder->abHuffAcSymbol[bHTableIndex][0];
                                     }

                                     IMG_FREAD(pbLenTable, sizeof(BYTE), 16, pJpegDecoder->pImageFile);

                                     for(bCounter = 0, wNumOfSymbols = 0; bCounter < 16; bCounter++)
                                     {
                                               wNumOfSymbols += pbLenTable[bCounter];
                                     }

                                     wSegLen -= 17; /* This table length + information byte */

                                     if(wSegLen < wNumOfSymbols || (blIsAc == 1 && wNumOfSymbols > 256) || (blIsAc == 0 && wNumOfSymbols > 16))
                                     {
                                               JPEG_SendError(100);
                                               break;
                                     }

                                     IMG_FREAD(pbSymTable, sizeof(BYTE), wNumOfSymbols, pJpegDecoder->pImageFile);
                                     wSegLen -= wNumOfSymbols; /* This table length + information byte */
                                     pJpegDecoder->bHuffTables++;
                          } while(wSegLen >= 19);
                          break;

               case SOS: /* Start of Scan parameters */
                          wSegLen = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          if(wSegLen < 3)
                          {
                                     JPEG_SendError(100);
                                     break;
                          }

                          btemp = JPEG_bReadByte(pJpegDecoder->pImageFile);
                          wOffset = wSegLen - (3 + (btemp * 2));

                          if(pJpegDecoder->bChannels != btemp || wSegLen < 3 + (btemp * 2))
                          {
                                     JPEG_SendError(100);
                                     break;
                          }
                          else
                          {
                                     BYTE bCounter, bChannelId = 0xFF;
                                     
                                     for(bCounter = 0; bCounter < pJpegDecoder->bChannels; bCounter++)
                                     {
                                               BYTE bindex;

                                               btemp = JPEG_bReadByte(pJpegDecoder->pImageFile);
                                               for(bindex = 0; bindex < MAX_CHANNELS; bindex++)
                                               {
                                                          if(pJpegDecoder->abChannelType[bindex] == btemp)
                                                          {
                                                                    bChannelId = bindex;
                                                                    break;
                                                          }
                                               }

                                               if(bChannelId < MAX_CHANNELS)
                                               {
                                                          btemp = JPEG_bReadByte(pJpegDecoder->pImageFile);
                                                          pJpegDecoder->abChannelHuffAcTableMap[bChannelId] = btemp & 0x0F;
                                                          pJpegDecoder->abChannelHuffDcTableMap[bChannelId] = btemp >> 4;
                                               }
                                     }
                                     IMG_FSEEK(pJpegDecoder->pImageFile, wOffset, 1);
                          }
                          blSOSOver = TRUE;
                          break;

               default: /* Any other segment with length */
                          wSegLen = JPEG_wReadWord(pJpegDecoder->pImageFile);
                          if(wSegLen < 2)
                          {
                                     JPEG_SendError(100);
                                     break;
                          }
                          IMG_FSEEK(pJpegDecoder->pImageFile, wSegLen - 2, 1);
             }
     }
     return 0;
}

/*******************************************************************************
Function:       BYTE JPEG_bGenerateHuffmanTables(JPEGDECODER *pJpegDecoder)

Precondition:   None

Overview:       This function generated the table required for Huffman decoding
                from the data read from the header

Input:          JPEGDECODER

Output:         Error code - '0' means no error
*******************************************************************************/
static BYTE JPEG_bGenerateHuffmanTables(JPEGDECODER *pJpegDecoder)
{
     BYTE bLength, bTable;

     for(bTable = 0; bTable < pJpegDecoder->bHuffTables / 2; bTable++)
     {
            pJpegDecoder->awHuffAcSymStart[bTable][0] = 0;
            pJpegDecoder->awHuffDcSymStart[bTable][0] = 0;

            for(bLength = 1; bLength < 16; bLength++)
            {
                   pJpegDecoder->awHuffAcSymStart[bTable][bLength] = (pJpegDecoder->awHuffAcSymStart[bTable][bLength - 1] + pJpegDecoder->abHuffAcSymLen[bTable][bLength - 1]) << 1;
                   pJpegDecoder->awHuffDcSymStart[bTable][bLength] = (pJpegDecoder->awHuffDcSymStart[bTable][bLength - 1] + pJpegDecoder->abHuffDcSymLen[bTable][bLength - 1]) << 1;
            }
     }
     return 0;
}

/*******************************************************************************
Function:       BYTE JPEG_bGet1Bit(JPEGDECODER *pJpegDecoder)

Precondition:   None

Overview:       This function returns 1 bit as the lsb of the returned byte.
                It automatically fills the buffer if it becomes empty.
                It also converts 0xFF00 into 0.

Input:          JPEGDECODER

Output:         One bit
*******************************************************************************/
static BYTE JPEG_bGet1Bit(JPEGDECODER *pJpegDecoder)
{
     BYTE bBit = 0;

     if(pJpegDecoder->wBufferIndex >= pJpegDecoder->wBufferLen)
     {
            pJpegDecoder->wBufferLen = IMG_FREAD(&pJpegDecoder->abDataBuffer[0], sizeof(BYTE), MAX_DATA_BUF_LEN, pJpegDecoder->pImageFile);
            while(pJpegDecoder->abDataBuffer[pJpegDecoder->wBufferLen - 1] == 0xFF)
            {
                   pJpegDecoder->wBufferLen--;
                   IMG_FSEEK(pJpegDecoder->pImageFile, -1, 1);
            }
            pJpegDecoder->wBufferIndex = 0;
     }

     while(pJpegDecoder->bBitsAvailable == 0) /* Be very careful to touch this part of code! You must know exactly what you are doing */
     {
            pJpegDecoder->bBitsAvailable = 16;
            pJpegDecoder->wWorkBits = pJpegDecoder->abDataBuffer[pJpegDecoder->wBufferIndex++];
            pJpegDecoder->wWorkBits = (pJpegDecoder->wWorkBits << 8) + pJpegDecoder->abDataBuffer[pJpegDecoder->wBufferIndex++];
            if(pJpegDecoder->wBufferIndex > pJpegDecoder->wBufferLen) /* wBufferIndex need not be even because of the below condition */
            {
                   pJpegDecoder->bBitsAvailable = 8;
            }
            else if((pJpegDecoder->wWorkBits & 0x00FF) == 0x00FF)
            {
                   pJpegDecoder->bBitsAvailable = 8;
                   pJpegDecoder->wBufferIndex--;
            }
            else if(pJpegDecoder->wWorkBits >= 0xFF00)
            {
                   if(pJpegDecoder->wWorkBits == 0xFF00)
                   {
                         pJpegDecoder->bBitsAvailable = 8;
                   }
            }
     }

     bBit = (pJpegDecoder->wWorkBits & 0x8000)? 0x01: 0;

     pJpegDecoder->wWorkBits <<= 1;
     pJpegDecoder->bBitsAvailable--;

     return bBit;
}

/*******************************************************************************
Function:       WORD JPEG_wGetBits(JPEGDECODER *pJpegDecoder, BYTE bLen)

Precondition:   None

Overview:       This function returns bLen number of bits as the lsb of the
                returned word and it automatically fills the buffer
                if it becomes empty.

Input:          JPEGDECODER, Number of bits

Output:         Requested number of bits
*******************************************************************************/
static WORD JPEG_wGetBits(JPEGDECODER *pJpegDecoder, BYTE bLen)
{
     WORD wVal = 0;

     wVal = pJpegDecoder->bFirstBit = JPEG_bGet1Bit(pJpegDecoder);
     bLen--;

     while(bLen)
     {
            wVal = (wVal << 1) + JPEG_bGet1Bit(pJpegDecoder);
            bLen--;
     }

     return wVal;   
}

/*******************************************************************************
Function:       WORD JPEG_wGetRestartWord(JPEGDECODER *pJpegDecoder)

Precondition:   File pointer must point to the restart word

Overview:       Returns the restart word

Input:          JPEGDECODER

Output:         Restart word
*******************************************************************************/
static WORD JPEG_wGetRestartWord(JPEGDECODER *pJpegDecoder)
{
     WORD wRestartWord;
     while((pJpegDecoder->bBitsAvailable & 0x07) != 0) /* This is to clearoff wnwanted bits to go to fresh byte */
     {
            JPEG_bGet1Bit(pJpegDecoder);
     }
     wRestartWord = JPEG_wGetBits(pJpegDecoder, 16);
     return(wRestartWord);
}

/*******************************************************************************
Function:       SHORT JPEG_sGetBitsValue(JPEGDECODER *pJpegDecoder, BYTE bLen)

Precondition:   None

Overview:       Returns the signed value of the bLen bits of data

Input:          JPEGDECODER, Number of bits

Output:         Signed number
*******************************************************************************/
static SHORT JPEG_sGetBitsValue(JPEGDECODER *pJpegDecoder, BYTE bLen)
{
     WORD wVal = 0;

     if(bLen != 0)
     {  
            wVal = JPEG_wGetBits(pJpegDecoder, bLen);
            if(pJpegDecoder->bFirstBit == 0)
            {
                   wVal = ~(wVal | (0xFFFF << bLen));
                   return ((SHORT)wVal * -1);
            }
     }
     return (SHORT)wVal;
}

/*******************************************************************************
Function:       BYTE JPEG_bGetNextHuffByte(JPEGDECODER *pJpegDecoder)

Precondition:   File pointer must point to the Huffman data

Overview:       Returns the Huffman decoded data

Input:          JPEGDECODER

Output:         Huffman decoded data
*******************************************************************************/
static BYTE JPEG_bGetNextHuffByte(JPEGDECODER *pJpegDecoder)
{
     BYTE bBits, bSymbol = 0;
     WORD wBitPattern = 0, wSymbolOffset = 0;

     for(bBits = 0; bBits < 16; bBits++)
     {
            BYTE bSymbols;
            WORD wDiff;

            wBitPattern = (wBitPattern << 1) + JPEG_bGet1Bit(pJpegDecoder);
            bSymbols = pJpegDecoder->pbCurrentHuffSymLenTable[bBits];
            if(bSymbols == 0)
            {
                   continue;
            }

            wDiff = wBitPattern - pJpegDecoder->pwCurrentHuffSymStartTable[bBits];
            if(wDiff < bSymbols)
            {
                   bSymbol = pJpegDecoder->pbCurrentHuffSymbolTable[wSymbolOffset + wDiff];
                   break;
            }
            wSymbolOffset += bSymbols;
     }
     return bSymbol;
}

#define range_limit(x) (x<0)?0:(x>0xFF)?0xFF:x
/*******************************************************************************
Function:       BYTE JPEG_bDecodeOneBlock(JPEGDECODER *pJpegDecoder)

Precondition:   File pointer must point to a new block of data

Overview:       Decodes the 8x8 pixel values of all the channels
                (A multiple of 8x8 block if subsampling is used)

Input:          JPEGDECODER

Output:         Error code - '0' means no error
*******************************************************************************/
static BYTE JPEG_bDecodeOneBlock(JPEGDECODER *pJpegDecoder)
{
     BYTE bBlock, bCounter;

     IMG_vLoopCallback();
     for(bBlock = 0; bBlock < pJpegDecoder->bBlocksInOnePass; bBlock++)
     {
            BYTE bByteCount, bHuffbyte;

            if((pJpegDecoder->wRestartInterval > 0) && (pJpegDecoder->wBlockNumber == pJpegDecoder->wRestartInterval * pJpegDecoder->bBlocksInOnePass))
            {
                   WORD wRestartWord = JPEG_wGetRestartWord(pJpegDecoder);
                   if(wRestartWord < 0xFFD0 || wRestartWord > 0xFFD7)
                   {
                            JPEG_SendError(100);
                   }                          
                   for(bCounter = 0; bCounter < MAX_CHANNELS; bCounter++)
                   {
                            pJpegDecoder->asPrevDcValue[bCounter] = 0;
                   }                   
                   pJpegDecoder->wBlockNumber = 0;
            }

            for(bCounter = 0; bCounter < 64; bCounter++)
            {
                   pJpegDecoder->asOneBlock[bBlock][bCounter] = 0;
            }

            pJpegDecoder->pwCurrentQuantTable = &pJpegDecoder->awQuantTable[pJpegDecoder->abChannelQuantTableMap[pJpegDecoder->abChannelMap[bBlock]]][0];

            /* Decode DC value */
            bByteCount = 0;
            pJpegDecoder->pbCurrentHuffSymLenTable = &pJpegDecoder->abHuffDcSymLen[pJpegDecoder->abChannelHuffDcTableMap[pJpegDecoder->abChannelMap[bBlock]]][0];
            pJpegDecoder->pbCurrentHuffSymbolTable = &pJpegDecoder->abHuffDcSymbol[pJpegDecoder->abChannelHuffDcTableMap[pJpegDecoder->abChannelMap[bBlock]]][0];
            pJpegDecoder->pwCurrentHuffSymStartTable = &pJpegDecoder->awHuffDcSymStart[pJpegDecoder->abChannelHuffDcTableMap[pJpegDecoder->abChannelMap[bBlock]]][0];
            bHuffbyte = JPEG_bGetNextHuffByte(pJpegDecoder);
            pJpegDecoder->asOneBlock[bBlock][0] = JPEG_sGetBitsValue(pJpegDecoder, bHuffbyte & 0x0F) + pJpegDecoder->asPrevDcValue[pJpegDecoder->abChannelMap[bBlock]];
            pJpegDecoder->asPrevDcValue[pJpegDecoder->abChannelMap[bBlock]] = pJpegDecoder->asOneBlock[bBlock][0];

            /* Decode AC value */
            bByteCount = 1;
            pJpegDecoder->pbCurrentHuffSymLenTable = &pJpegDecoder->abHuffAcSymLen[pJpegDecoder->abChannelHuffAcTableMap[pJpegDecoder->abChannelMap[bBlock]]][0];
            pJpegDecoder->pbCurrentHuffSymbolTable = &pJpegDecoder->abHuffAcSymbol[pJpegDecoder->abChannelHuffAcTableMap[pJpegDecoder->abChannelMap[bBlock]]][0];
            pJpegDecoder->pwCurrentHuffSymStartTable = &pJpegDecoder->awHuffAcSymStart[pJpegDecoder->abChannelHuffAcTableMap[pJpegDecoder->abChannelMap[bBlock]]][0];
            while(bByteCount < 64)
            {
                   bHuffbyte = JPEG_bGetNextHuffByte(pJpegDecoder);
                   bByteCount += (bHuffbyte >> 4);
                   if(bHuffbyte == 0 /* EOB */)
                   {
                          break;
                   }
                   if(bByteCount > 63)
                   {
                          JPEG_SendError(100);
                   }
                   pJpegDecoder->asOneBlock[bBlock][abZigzag[bByteCount++]] = JPEG_sGetBitsValue(pJpegDecoder, bHuffbyte & 0x0F);
            }
            pJpegDecoder->wBlockNumber++;
            jpeg_idct_islow(&pJpegDecoder->asOneBlock[bBlock][0],pJpegDecoder->pwCurrentQuantTable);
     }

//     SetVisualPage(1);

     return 0;
}

#define JPEG_WRITE_TO_DISPLAY
/*******************************************************************************
Function:       void JPEG_vInitDisplay(JPEGDECODER *pJpegDecoder)

Precondition:   None

Overview:       Initializes the (x, y) co-ordinates to (0, 0)

Input:          JPEGDECODER

Output:         None
*******************************************************************************/
static void JPEG_vInitDisplay(JPEGDECODER *pJpegDecoder)
{
 #ifndef JPEG_WRITE_TO_DISPLAY
     printf("%4u %4u\n", pJpegDecoder->wWidth, pJpegDecoder->wHeight);
 #else
    pJpegDecoder->wPrevX = 0;
    pJpegDecoder->wPrevY = 0;
//    SetActivePage(1);
 #endif
}

/*******************************************************************************
Function:       BYTE JPEG_bPaintOneBlock(JPEGDECODER *pJpegDecoder)

Precondition:   One block - 8x8 pixel data of all channels must be decoded

Overview:       Displays one 8x8 on the screen
                (A multiple of 8x8 block if subsampling is used)

Input:          JPEGDECODER

Output:         Error code - '0' means no error
*******************************************************************************/
static BYTE JPEG_bPaintOneBlock(JPEGDECODER *pJpegDecoder)
{
     BYTE bCounter;

 #ifndef JPEG_WRITE_TO_DISPLAY

     for(bCounter = 0; bCounter < 64; bCounter++)
     {
            printf("%3u %3u %3u\n", abBlockR[bCounter]&0xF8, abBlockG[bCounter]&0xFC, abBlockB[bCounter]&0xF8);
     }

 #else

     WORD wX, wY;
     bCounter = 0;
     BYTE r,g,b;

     if(pJpegDecoder->bSubSampleType == JPEG_SAMPLE_1x1)
     {
            SHORT *psY = &pJpegDecoder->asOneBlock[0][0], *psCb = &pJpegDecoder->asOneBlock[1][0], *psCr = &pJpegDecoder->asOneBlock[2][0];

            for(wY = 0; wY < 8; wY++)
            {
                    for(wX = 0; wX < 8; wX++)
                    {
                            LONG s1 = ((*psY) + 128)*128, s2 = (*psCb), s3 = (*psCr);
                            r = range_limit((s1 + 180*s3)>>7);
                            g = range_limit((s1 - 44*s2 - 91*s3)>>7);
                            b = range_limit((s1 + 227*s2)>>7);
                            IMG_vSetColor(r, g, b);
                            IMG_vPutPixel(pJpegDecoder->wPrevX + wX, pJpegDecoder->wPrevY + wY);
                            psY++; psCb++; psCr++;
                    }  
             }

            pJpegDecoder->wPrevX += 8;

            if(pJpegDecoder->wPrevX >= pJpegDecoder->wWidth)
            {
                    pJpegDecoder->wPrevX = 0;
                    pJpegDecoder->wPrevY += 8;
            }
     }       
     else if(pJpegDecoder->bSubSampleType == JPEG_SAMPLE_1x2)
     {
            SHORT *psY, *psCb = &pJpegDecoder->asOneBlock[2][0], *psCr = &pJpegDecoder->asOneBlock[3][0];
            BYTE bBlock, bOffsetY[2] = {0,8};

            for(bBlock = 0; bBlock < 2; bBlock++)
            {
                    psY = &pJpegDecoder->asOneBlock[bBlock][0];
                    psCb = &pJpegDecoder->asOneBlock[2][bBlock*32];
                    psCr = &pJpegDecoder->asOneBlock[3][bBlock*32];
                    for(wY = 0; wY < 8; wY++)
                    {
                            for(wX = 0; wX < 8; wX++)
                            {
                                     LONG s1 = ((*psY) + 128)*128;
                                     LONG s2 = psCb[(wY>>1)*8+wX];
                                     LONG s3 = psCr[(wY>>1)*8+wX];
                                     r = range_limit((s1 + 180*s3)>>7);
                                     g = range_limit((s1 - 44*s2 - 91*s3)>>7);
                                     b = range_limit((s1 + 227*s2)>>7);
                                     IMG_vSetColor(r, g, b);
                                     IMG_vPutPixel(pJpegDecoder->wPrevX + wX,
                                              pJpegDecoder->wPrevY + bOffsetY[bBlock] + wY);
                                     psY++;
                            }
                    }
            }

            pJpegDecoder->wPrevX += 8;

            if(pJpegDecoder->wPrevX >= pJpegDecoder->wWidth)
            {
                    pJpegDecoder->wPrevX = 0;
                    pJpegDecoder->wPrevY += 16;
            }
     }       
     else if(pJpegDecoder->bSubSampleType == JPEG_SAMPLE_2x1)
     {
            SHORT *psY, *psCb = &pJpegDecoder->asOneBlock[2][0], *psCr = &pJpegDecoder->asOneBlock[3][0];
            BYTE bBlock, bOffsetX[2] = {0,8};

            for(bBlock = 0; bBlock < 2; bBlock++)
            {
                    psY = &pJpegDecoder->asOneBlock[bBlock][0];
                    psCb = &pJpegDecoder->asOneBlock[2][bBlock*4];
                    psCr = &pJpegDecoder->asOneBlock[3][bBlock*4];
                    for(wY = 0; wY < 8; wY++)
                    {
                            for(wX = 0; wX < 8; wX++)
                            {
                                     LONG s1 = ((*psY) + 128)*128;
                                     LONG s2 = psCb[(wY*8)+(wX>>1)];
                                     LONG s3 = psCr[(wY*8)+(wX>>1)];
                                     r = range_limit((s1 + 180*s3)>>7);
                                     g = range_limit((s1 - 44*s2 - 91*s3)>>7);
                                     b = range_limit((s1 + 227*s2)>>7);
                                     IMG_vSetColor(r, g, b);
                                     IMG_vPutPixel(pJpegDecoder->wPrevX + bOffsetX[bBlock] + wX,
                                              pJpegDecoder->wPrevY + wY);
                                     psY++;
                            }
                    }
            }

            pJpegDecoder->wPrevX += 16;

            if(pJpegDecoder->wPrevX >= pJpegDecoder->wWidth)
            {
                    pJpegDecoder->wPrevX = 0;
                    pJpegDecoder->wPrevY += 8;
            }
     }
     else if(pJpegDecoder->bSubSampleType == JPEG_SAMPLE_2x2)
     {
            SHORT *psY, *psCb, *psCr;
            BYTE bBlock, bOffsetX[4] = {0,8,0,8}, bOffsetY[4] = {0,0,8,8}, bCbCrOffset[4] = {0,4,32,36};

            for(bBlock = 0; bBlock < 4; bBlock++)
            {
                WORD wXPos = pJpegDecoder->wPrevX + bOffsetX[bBlock];
				WORD wYPos = pJpegDecoder->wPrevY + bOffsetY[bBlock];
				
				psY = &pJpegDecoder->asOneBlock[bBlock][0];
				psCb = &pJpegDecoder->asOneBlock[4][bCbCrOffset[bBlock]];
				psCr = &pJpegDecoder->asOneBlock[5][bCbCrOffset[bBlock]];
				for(wY = 0; wY < 8; wY++)
				{
					WORD wYPos2 = wYPos + wY;
					
					for(wX = 0; wX < 4; wX++)
					{
						WORD psCxIndex = (wY>>1)*8 + wX;
						LONG s2 = psCb[psCxIndex];
						LONG s3 = psCr[psCxIndex];
						WORD wX2;
						
						for(wX2 = 0; wX2 < 2; wX2++)
						{
							LONG s1 = ((*psY++) + 128)*128;
							r = range_limit((s1 + 180*s3)>>7);
							g = range_limit((s1 - 44*s2 - 91*s3)>>7);
							b = range_limit((s1 + 227*s2)>>7);
							IMG_vSetColor(r, g, b);
							IMG_vPutPixel(wXPos + wX*2 + wX2, wYPos2);
						}    
					}
				}
            }

            pJpegDecoder->wPrevX += 16;

            if(pJpegDecoder->wPrevX >= pJpegDecoder->wWidth)
            {
                    pJpegDecoder->wPrevX = 0;
                    pJpegDecoder->wPrevY += 16;
            }
     }       

 #endif
     return 0;
}

#ifndef IMG_USE_NON_BLOCKING_DECODING

/*******************************************************************************
Function:       BYTE JPEG_bDecode(IMG_FILE *pfile, BOOL bFirstTime)

Precondition:   The global variables of Image decoder must be set

Overview:       This function decodes and displays a jpeg image

Input:          Image file, ignored BOOLean

Output:         Error code - '0' means no error
*******************************************************************************/
BYTE JPEG_bDecode(IMG_FILE *pfile, BOOL bFirstTime)
{
     WORD whblocks, wvblocks;
     WORD wi, wj;
     JPEGDECODER JPEG_JpegDecoder;

     JPEG_vResetDecoder(&JPEG_JpegDecoder);
     JPEG_JpegDecoder.pImageFile = pfile;
     if(JPEG_bReadHeader(&JPEG_JpegDecoder) != 0)
     {
         return JPEG_JpegDecoder.bError;
     }
    
     IMG_wImageWidth = JPEG_JpegDecoder.wWidth;
     IMG_wImageHeight = JPEG_JpegDecoder.wHeight;
     IMG_vSetboundaries();

     JPEG_bGenerateHuffmanTables(&JPEG_JpegDecoder);

     whblocks = JPEG_JpegDecoder.wWidth >> 3;
     wvblocks = JPEG_JpegDecoder.wHeight >> 3;

     if(whblocks * 8 < JPEG_JpegDecoder.wWidth) /* Odd sizes */
     {
         whblocks++;
     }

     if(wvblocks * 8 < JPEG_JpegDecoder.wHeight) /* Odd sizes */
     {
         wvblocks++;
     }

     if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_1x2)
     {
         wvblocks =  (wvblocks>>1) + (wvblocks&1);
     }
     else if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_2x1)
     {
         whblocks = (whblocks>>1) + (whblocks&1);
     }
     else if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_2x2)
     {
         wvblocks =  (wvblocks>>1) + (wvblocks&1);
         whblocks = (whblocks>>1) + (whblocks&1);
     }

     JPEG_vInitDisplay(&JPEG_JpegDecoder);

     for(wi = 0; wi < whblocks; wi++)
     {
            for(wj = 0; wj < wvblocks; wj++)
            {
                   IMG_vCheckAndAbort();
                   JPEG_bDecodeOneBlock(&JPEG_JpegDecoder); /* Fills a block after correcting the zigzag, dequantizing, IDCR and color conversion to RGB */
                   JPEG_bPaintOneBlock(&JPEG_JpegDecoder); /* Sends the block to the Graphics unit */
            }
     }
     return JPEG_JpegDecoder.bError;
}

#else

/*******************************************************************************
Function:       BYTE JPEG_bDecode(IMG_FILE *pfile, BOOL bFirstTime)

Precondition:   The global variables of Image decoder must be set

Overview:       This function decodes and displays a jpeg image

Input:          Image file, BOOLean indicating if this is the first time calling 
				the JPEG_bDecode function (needed to reset internal decoding 
				state variables).  If bFirstTime is TRUE, pfile must be set.  
				If bFirstTime is FALSE, pfile is ignored (uses previously 
				provided file handle).

Output:         Error code - '0' means not yet completed
*******************************************************************************/
BYTE JPEG_bDecode(IMG_FILE *pfile, BOOL bFirstTime)
{
	static WORD whblocks, wvblocks;
	static WORD wi, wj;
	static JPEGDECODER JPEG_JpegDecoder;
	static enum
	{
		INITIAL,
		HEADER_DECODED,
		BLOCK_DECODE,
		DECODE_DONE
	} decodestate;


	if(bFirstTime)
		decodestate = INITIAL;
	
    switch(decodestate)
    {
    
        case INITIAL:   JPEG_vResetDecoder(&JPEG_JpegDecoder);
                        JPEG_JpegDecoder.pImageFile = pfile;
                        if(JPEG_bReadHeader(&JPEG_JpegDecoder) != 0)
                        {
                            return 1;
                        }
                        decodestate = HEADER_DECODED;
                        return 0;
        
        case HEADER_DECODED:
                            IMG_wImageWidth = JPEG_JpegDecoder.wWidth;
                            IMG_wImageHeight = JPEG_JpegDecoder.wHeight;
                            IMG_vSetboundaries();

                            JPEG_bGenerateHuffmanTables(&JPEG_JpegDecoder);

                            whblocks = JPEG_JpegDecoder.wWidth >> 3;
                            wvblocks = JPEG_JpegDecoder.wHeight >> 3;

                            if(whblocks * 8 < JPEG_JpegDecoder.wWidth) /* Odd sizes */
                            {
                                whblocks++;
                            }

                            if(wvblocks * 8 < JPEG_JpegDecoder.wHeight) /* Odd sizes */
                            {
                                wvblocks++;
                            }

                            if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_1x2)
                            {
                                wvblocks =  (wvblocks>>1) + (wvblocks&1);
                            }
                            else if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_2x1)
                            {
                                whblocks = (whblocks>>1) + (whblocks&1);
                            }
                            else if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_2x2)
                            {
                                wvblocks =  (wvblocks>>1) + (wvblocks&1);
                                whblocks = (whblocks>>1) + (whblocks&1);
                            }

                            JPEG_vInitDisplay(&JPEG_JpegDecoder);
                             
                            wi = 0;
                            wj = 0;

                            decodestate = BLOCK_DECODE;
                            return 0;

        case BLOCK_DECODE:  if(wi < whblocks)
                            {
                                JPEG_bDecodeOneBlock(&JPEG_JpegDecoder); /* Fills a block after correcting the zigzag, dequantizing, IDCR and color conversion to RGB */
                                JPEG_bPaintOneBlock(&JPEG_JpegDecoder); /* Sends the block to the Graphics unit */
                                wj++;
                                
                                if(wj >= wvblocks)
                                {
                                    wj = 0;
                                    wi++;
                                }
                                return 0;
                            }

                            decodestate = DECODE_DONE;
                            // No break needed

        case DECODE_DONE:	return 1;

        default:            return 1;
    }
}

#endif /* #ifndef IMG_USE_NON_BLOCKING_DECODING */

#endif /* #ifdef IMG_SUPPORT_JPEG */

#undef __JPEGDECODER_C__
