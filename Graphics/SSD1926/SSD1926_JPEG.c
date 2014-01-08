/******************************************************************************
 *    SSD1926 hardware JPEG decoder driver
 ******************************************************************************
 * FileName:        SSD1926_JPEG.c
 * Dependencies:    see included files below
 * Processor:       PPIC24F, PIC24H, dsPIC, PIC32
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
Sean Justice        15_Sept-2008      First release
Anton Alkhimenok    06_Jun-2009       Ported to PIC24
*******************************************************************************/

#include "MainDemo.h"
#include "Graphics/SSD1926.h"

/**************************************************************************
 EXTERNS
**************************************************************************/
extern DWORD Cluster2Sector(DISK * disk, DWORD cluster);
extern BYTE SDSectorDMARead(DWORD sector_addr, DWORD dma_addr, UINT16 num_blk);
extern void SetReg(WORD index, BYTE value);

/**************************************************************************
 LOCAL FUNCTIONS PROTOTYPES
**************************************************************************/
BYTE            SDFile2JPEGFIFO(DWORD fifoAddress, DWORD byteSize, FSFILE *stream);
void            SSD1926Setup(SSD_JPEG_DECODE *decode);
SSD_JPEG_ERR    JPEGHeader(SSD_JPEG_DECODE *decode);
void            JPEGResize(SSD_JPEG_DECODE *decode);
void            JPEGStart(void);
SSD_JPEG_ERR    JPEGData(SSD_JPEG_DECODE *decode);
void            JPEGStop(void);

/**************************************************************************
  Function:
    void SSD1926SetRGB(void)
  Conditions:
    None.
  Input:
    None.
  Return:
    None.
  Side Effects:
    None.
  Description:
    Sets main and floating windows to RGB mode
  Remarks:
    This mode is used by the graphics library.
  **************************************************************************/
inline void __attribute__ ((always_inline))
/* */
SSD1926SetRGB(void)
{
    SetReg(REG_RGB_SETTING, 0xc0);
}

/**************************************************************************
  Function:
    void SSD1926SetYUV(void)
  Conditions:
    None.
  Input:
    None.
  Return:
    None.
  Side Effects:
    Objects drawn by graphics library cannot be displayed correctly in this mode.
  Description:
    Sets main and floating windows to YUV mode
  Remarks:
    Usually JPEG files use YUV format.
  **************************************************************************/
inline void __attribute__ ((always_inline))
/* */
SSD1926SetYUV(void)
{
    SetReg(REG_RGB_SETTING, 0x00);
}

/**************************************************************************
  Function:
    BYTE JPEGPutImage(char* filename)
  Conditions:
    FSInit() file system initialization must be done, 
    SSD1926SetYUV() YUV mode must be set.
  Input:
    filename - pointer to the file name string.
  Return:
    Non-zero if the operation was successful.
  Side Effects:
    None.
  Description:
    Displays the JPEG image form the JPEG file specified in the screen center.
  Remarks:
    Usually JPEG files use YUV format.
  **************************************************************************/
BYTE JPEGPutImage(char *filename)
{
    FSFILE          *jpeg_file;
    SSD_JPEG_DECODE jpeg_decode;

    jpeg_file = FSfopen(filename, "r");

    if(!jpeg_file)
        return (FALSE);

    jpeg_decode.stream = (void *)jpeg_file;
    SSD1926Setup(&jpeg_decode);
    JPEGHeader(&jpeg_decode);
    JPEGResize(&jpeg_decode);
    JPEGStart();
    JPEGData(&jpeg_decode);
    JPEGStop();
    FSfclose(jpeg_file);
    return (TRUE);
}

/**************************************************************************
  Function:
    BYTE SDFile2JPEGFIFO(DWORD fifoAddress, WORD byteSize, FSFILE *stream)
  Summary:
    Read portion data from a file on SD card and put it in SSD1926 memory
  Conditions:
    File is opened in a read mode
  Input:
    fifoAddress -   Destination buffer in SSD1926 memory
    byteSize    -   Size in bytes
    stream      -   File to be read from
  Return:
    non-zero if the operation was successful.
  Side Effects:
    None.
  Description:
    The SDFile2JPEGFIFO function will read data from the specified file on SD card connected
    directly to SSD1926 and move the data to SSD1926 internal RAM assigned for JPEG FIFO.
  Remarks:
    None.
  **************************************************************************/
BYTE SDFile2JPEGFIFO(DWORD fifoAddress, DWORD byteSize, FSFILE *stream)
{
    DISK    *dsk;
    DWORD   len;
    DWORD   seek, sec_sel;
    DWORD   pos;

    len = byteSize / MEDIA_SECTOR_SIZE;
    dsk = stream->dsk;
    pos = stream->pos;
    seek = stream->seek;

    sec_sel = (DWORD) Cluster2Sector(dsk, stream->ccls);
    sec_sel += (DWORD) stream->sec;         // add the sector number to it
    while(len)
    {
        if(seek >= stream->size)
        {
            seek = stream->size;
            pos = 0;
            break;
        }

        if(stream->sec >= dsk->SecPerClus)
        {
            stream->sec = 0;
            if(FILEget_next_cluster(stream, (DWORD) 1) != CE_GOOD)
            {
                return (FALSE);
            }

            sec_sel = (DWORD) Cluster2Sector(dsk, (DWORD) stream->ccls);
            sec_sel += (DWORD) stream->sec;  // add the sector number to it
        }

        if(((DWORD) (stream->sec) + (DWORD) len) > (DWORD) (dsk->SecPerClus))
            pos = dsk->SecPerClus - stream->sec;
        else
            pos = len;

        if(!SDSectorDMARead(sec_sel, fifoAddress, pos))
            return (FALSE);

        len -= pos;
        stream->sec += pos;
        sec_sel += pos;
        seek += pos * MEDIA_SECTOR_SIZE;
        fifoAddress += pos * MEDIA_SECTOR_SIZE;
    }

    stream->pos = 0;                        // save off the positon
    stream->seek = seek;                    // save off the seek
    return (TRUE);
}

/*********************************************************************
 * Function:       
 *  void SSD1926Setup(SSD_JPEG_DECODE *decode)
 * PreCondition:    
 *  JPEG file should be opened.
 * Input:           
 *  decode - pointer to decoder parameters 
 * Output:          
 *  None.
 * Side Effects:    
 *  None.
 * Overview:        
 *  Sets up JPEG engine for a new file
 * Note:
 *  None.           
 ********************************************************************/
void SSD1926Setup(SSD_JPEG_DECODE *decode)
{
    DWORD_VAL   size;

    decode->bytes_read = 0;

    SetReg(SSD_JPEG_REG_RST_MARKER_STATUS, 0);

    SetReg(SSD_JPEG_REG_FIFO_SIZE, (BYTE) (SSD_JPEG_FIFO_BLK_NUMBER - 1));

    // set the FIFO address
    decode->fifo_addr = SSD_JPEG_FIFO_START_ADDR;
    decode->fifo_addr_end = SSD_JPEG_FIFO_START_ADDR + (SSD_JPEG_FIFO_BLK_NUMBER * SSD_JPEG_FIFO_BLK_SIZE);

    size.Val = SSD_JPEG_FIFO_START_ADDR / 4;    // each address is 32 bits
    SetReg(SSD_JPEG_REG_DEST_START_ADDR_0, size.v[0]);
    SetReg(SSD_JPEG_REG_DEST_START_ADDR_1, size.v[1]);
    SetReg(SSD_JPEG_REG_DEST_START_ADDR_2, size.v[2]);

    // enable the JPEG and set YUV range
    SetReg(SSD_JPEG_REG_CTRL, 0x11);

    // reset the module
    SetReg(SSD_JPEG_REG_CMD, 0x80);
    DelayMs(1);
    SetReg(SSD_JPEG_REG_CMD, 0x00);

    // Set the RST Marker Operatation to Data revise
    SetReg(SSD_JPEG_REG_RST_MARKER, 0x02);

    // Set the RST Marker Status
    SetReg(SSD_JPEG_REG_RST_MARKER_STATUS, 0x00);

    // load the file size in bytes
    FSfseek(decode->stream, 0, SEEK_END);
    size.Val = FSftell(decode->stream);
    FSfseek(decode->stream, size.Val, SEEK_END);

    decode->size = size.Val;

    SetReg(SSD_JPEG_REG_FILE_SIZE_0, ((DWORD_VAL) size).v[0]);
    SetReg(SSD_JPEG_REG_FILE_SIZE_1, ((DWORD_VAL) size).v[1]);
    SetReg(SSD_JPEG_REG_FILE_SIZE_2, ((DWORD_VAL) size).v[2]);

    // set the JPEG Op mode
    SetReg(SSD_JPEG_REG_OP_MODE_ENC, 0x04);

    // turn on the resizer
    SetReg(SSD_JPEG_REG_RESIZER_CTL, 0x01);

    // start!
    SetReg(SSD_JPEG_REG_CMD, 0x01);
}

/*********************************************************************
 * Function:       
 *  SSD_JPEG_ERR JPEGHeader(SSD_JPEG_DECODE *decode)
 * PreCondition:    
 *  SSD1926Setup(...) must be called first.
 * Input:           
 *  decode - pointer to decoder parameters
 * Output:          
 *  Error code. Returns NO_ERR if the operation was successful.
 * Side Effects:    
 *  None.
 * Overview:        
 *  Loads the JPEG header.
 * Note:
 *  None. 
 ********************************************************************/
SSD_JPEG_ERR JPEGHeader(SSD_JPEG_DECODE *decode)
{
    while(1)
    {
        if(GetReg(SSD_JPEG_REG_RST_MARKER_STATUS))
            return (ERR_DECODE);

        if(GetReg(SSD_JPEG_REG_RAW_STATUS1) & 0x10) // has decode information availble
        {
            register WORD_VAL   size;

            size.v[0] = GetReg(SSD_JPEG_REG_DECODE_X_SIZE);
            size.v[1] = GetReg(SSD_JPEG_REG_DECODE_X_SIZE + 1);
            decode->image_width = size.Val;

            size.v[0] = GetReg(SSD_JPEG_REG_DECODE_Y_SIZE);
            size.v[1] = GetReg(SSD_JPEG_REG_DECODE_Y_SIZE + 1);
            decode->image_height = size.Val;

            decode->op_mode = GetReg(SSD_JPEG_REG_OP_MODE);

            return (NO_ERR);
        }

        if(!(GetReg(SSD_JPEG_REG_RAW_STATUS) & 0x01))
            continue;

        if(decode->bytes_read < decode->size)
        {
            if(!SDFile2JPEGFIFO(SSD_JPEG_FIFO_START_ADDR, SSD_JPEG_FIFO_SIZE, decode->stream))
            { }

            decode->bytes_read += SSD_JPEG_FIFO_SIZE;
        }
        else
            return (ERR_NO_DATA);
    }
}

/*********************************************************************
 * Function:
 *  void SSDJPEGResize(SSD_JPEG_DECODE *decode)
 * PreCondition:    
 *  Should be called after JPEGHeader(...)
 * Input:           
 *  decode - pointer to decoder parameters 
 * Output:          
 *  None.
 * Side Effects:    
 *  None.
 * Overview:        
 *  Calculates the size reducing factor (can be only a power of 2) and 
 *  sets it for the JPEG engine. Changes the image size parameters accordingly.
 * Note:
 *  None.           
 ********************************************************************/
void JPEGResize(SSD_JPEG_DECODE *decode)
{
    const DWORD __align[] = { 8, 16, 16, 32};
    WORD_VAL    size;
    BYTE        div = 1;

    if((decode->image_height > (GetMaxY() + 1)) || (decode->image_width > (GetMaxX() + 1)))
    {
        div = 2;
        while(div < 8)
        {
            if(((decode->image_height / div) <= (GetMaxY() + 1)) && ((decode->image_width / div) <= (GetMaxX() + 1)))
                break;

            div <<= 1;
        }

        SetReg(SSD_JPEG_REG_RESIZER_OP_0, div);
        SetReg(SSD_JPEG_REG_RESIZER_OP_1, 0x02);
    }

    decode->display_height = div * (GetMaxY() + 1);
    decode->display_width = div * (GetMaxX() + 1);

    // set the source address
    DWORD_VAL   destAddr;
    DWORD       x, y;

    x = (decode->display_width - decode->image_width) / div / 2;
    y = (decode->display_height - decode->image_height) / div / 2;

    destAddr.Val = ((y * (GetMaxX() + 1) + x) >> 1);    // each address is 32 bits, each pixel is 2 bytes
    destAddr.v[0] &= ~0x01;                             // LSB must be zero
    SetReg(SSD_JPEG_REG_SRC_START_ADDR_0, destAddr.v[0]);
    SetReg(SSD_JPEG_REG_SRC_START_ADDR_1, destAddr.v[1]);
    SetReg(SSD_JPEG_REG_SRC_START_ADDR_2, destAddr.v[2]);

    decode->display_height = decode->display_height & (~(__align[decode->op_mode] - 1));
    decode->display_width = decode->display_width & (~(__align[decode->op_mode] - 1));

    size.Val = decode->display_width - 1;
    SetReg(SSD_JPEG_REG_RESIZER_STARTX_0, 0);
    SetReg(SSD_JPEG_REG_RESIZER_STARTX_1, 0);
    SetReg(SSD_JPEG_REG_RESIZER_ENDX_0, size.v[0]);
    SetReg(SSD_JPEG_REG_RESIZER_ENDX_1, size.v[1]);

    size.Val = decode->display_height - 1;
    SetReg(SSD_JPEG_REG_RESIZER_STARTY_0, 0);
    SetReg(SSD_JPEG_REG_RESIZER_STARTY_1, 0);
    SetReg(SSD_JPEG_REG_RESIZER_ENDY_0, size.v[0]);
    SetReg(SSD_JPEG_REG_RESIZER_ENDY_1, size.v[1]);
}

/*********************************************************************
 * Function:       
 *  void JPEGStart(void)
 * PreCondition:    
 *  should be called after JPEGHeader(...).   
 * Input:           
 *  None.
 * Output:          
 *  None.
 * Side Effects:    
 *  None.
 * Overview:        
 *  Starts the JPEG data decoding
 * Note:           
 ********************************************************************/
void JPEGStart(void)
{

    // clear all status of the JPEG Link Buffer and FIFO
    SetReg(SSD_JPEG_REG_STATUS, 0x07);  // write 1 to clear
    SetReg(SSD_JPEG_REG_STATUS1, 0x30); // write 1 to clear
    SetReg(SSD_JPEG_REG_START_STOP, 0x01);
}

/*********************************************************************
 * Function:       
 *  SSD_JPEG_ERR JPEGData(SSD_JPEG_DECODE *decode)
 * PreCondition:    
 *  should be called after JPEGStart(...).
 * Input:           
 *  decode - pointer to decoder parameters
 * Output:          
 *  Error code. Returns NO_ERR if the operation was successful.
 * Side Effects:    
 *  None.
 * Overview:        
 *  Decodes the picture on the screen.
 * Note:
 *  None. 
 ********************************************************************/
SSD_JPEG_ERR JPEGData(SSD_JPEG_DECODE *decode)
{
    while(1)
    {
        if(GetReg(SSD_JPEG_REG_RST_MARKER_STATUS))
            return (ERR_DECODE);

        if(GetReg(SSD_JPEG_REG_DECODE_VALUE) & 0xF8)
            return (ERR_DECODE);

        if(GetReg(SSD_JPEG_REG_RAW_STATUS1) == 0x22)
            return (NO_ERR);

        if(!(GetReg(SSD_JPEG_REG_RAW_STATUS) & 0x01))   // FIFO is not empty
            continue;

        if(decode->bytes_read < decode->size)
        {
            if(!SDFile2JPEGFIFO(SSD_JPEG_FIFO_START_ADDR, SSD_JPEG_FIFO_SIZE, decode->stream))
            { }

            decode->bytes_read += SSD_JPEG_FIFO_SIZE;
        }
        else
            return (ERR_NO_DATA);
    }
}

/*********************************************************************
 * Function:       
 *  void JPEGStop(void)
 * PreCondition:    
 *  None.
 * Input:           
 *  None.
 * Output:          
 *  None.
 * Side Effects:    
 *  None.
 * Overview:        
 *  Stops the JPEG data decoding
 * Note:           
 ********************************************************************/
void JPEGStop(void)
{
    SetReg(SSD_JPEG_REG_CTRL, 0x10);
    SetReg(SSD_JPEG_REG_START_STOP, 0x00);
}
