/**********************************************************************
* FileName:		SST_SerialFlash.c
* Dependencies: none   
* Processor:	PIC18F46J50	
* Complier:     Microchip C18 v3.04 or higher
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice for P2P Software:
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute 
* Software only when embedded on a Microchip microcontroller or digital 
* signal controller and used with a Microchip radio frequency transceiver, 
* which are integrated into your product or third party product (pursuant 
* to the terms in the accompanying license agreement).   
*
* You should refer to the license agreement accompanying this Software for 
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED ï¿½AS ISï¿½ WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
****************************************************************************/
#include "SST_SerialFlash.h"
#include "SystemProfile.h"
#include "WirelessProtocols/MSPI.h"


#define SPI_READ            0x03
#define SPI_SECTOR_ERASE    0x20
#define SPI_BLOCK_ERASE     0x52
#define SPI_CHIP_ERASE      0x60
#define SPI_BYTE_PROGRAM    0x02
#define SPI_AUTO_ADDRESS_INC 0xAF
#define SPI_READ_STATUS_REG 0x05
#define SPI_WRITE_ENABLE    0x06
#define SPI_READ_ID         0x90
#define SPI_WRITE_DISABLE   0x04

#define READ_MANUFACTURER_ID 0xBF
#define READ_DEVICE_ID      0x49

/*********************************************************************
* Function:         void SSTRead(BYTE *dest, BYTE addr, BYTE count)
*
* PreCondition:     none
*
* Input:		    BYTE *dest  - Destination buffer.
*                   BYTE *addr   - Address to start reading from.
*                   BYTE count  - Number of bytes to read.
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Following routine reads bytes from the EEProm and puts
*                   them in a buffer.
*                    
*
* Note:			    
**********************************************************************/   
void SSTRead(BYTE *dest, BYTE *addr, BYTE count)
{
    EE_nCS = 0;
    SPIPut2(SPI_READ);
    SPIPut2(*addr);
    SPIPut2(*addr++);
    SPIPut2(*addr++);
    while( count )
    {
        *dest++ = SPIGet2();
        count--;
    }
    EE_nCS = 1;
} 

/*********************************************************************
* Function:         void SSTWrite(BYTE *src, BYTE *addr, BYTE count)
*
* PreCondition:     none
*
* Input:		    BYTE *src  - Source buffer.
*                   BYTE *addr   - Address to start reading from.
*                   BYTE count  - Number of bytes to read.
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Following routine reads bytes from the EEProm and puts
*                   them in a buffer.
*                    
*
* Note:			    
**********************************************************************/   
void SSTWrite(BYTE *src, BYTE *addr, BYTE count)
{
    EE_nCS = 0;
    SPIPut2(SPI_WRITE_ENABLE);
    SPIPut2(SPI_AUTO_ADDRESS_INC);
    SPIPut2(*addr);
    SPIPut2(*addr++);
    SPIPut2(*addr++);
    while( count )
    {
        SPIPut(*src++);
        count--;
        if(count != 0) SPIPut2(SPI_AUTO_ADDRESS_INC);
    }
    SPIPut2(SPI_WRITE_DISABLE);
    EE_nCS = 1;
} 

/*********************************************************************
* Function:         void SSTGetID(BYTE dest)
*
* PreCondition:     none
*
* Input:		    BYTE dest  - Destination buffer.
*                   
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Following routine reads bytes from the SST Flash and puts
*                   them in a buffer.
*                    
*
* Note:			    
**********************************************************************/   
void SSTGetID(BYTE *dest)
{
    EE_nCS = 0;
    SPIPut2(SPI_READ_ID);
    SPIPut2(0x00);
    SPIPut2(0x00);
    SPIPut2(0x00);
    *dest = SPIGet2();
    EE_nCS = 1;
} 
