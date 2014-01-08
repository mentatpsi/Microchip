/*****************************************************************************
 *
 * Basic access to SPI EEPROM 25LC256 located on Explorer 16.
 *
 *****************************************************************************
 * FileName:        MCHP25LC256.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
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
 * 03/11/11	    Ported from EEPROM
 * 
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined (USE_MCHP25LC256)

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "drv_spi.h"
#include "MCHP25LC256.h"

WORD MCHP25LC256_spi_channel;

/************************************************************************
* Function: MCHP25LC256Init                                                  
*                                                                       
* Overview: this function setup SPI and IOs connected to EEPROM
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void MCHP25LC256Init(DRV_SPI_INIT_DATA *pInitData)
{
    // initialize the SPI channel to be used
    // SPI_CHANNEL_USED is defined in the hardware profile
    DRV_SPI_Initialize(pInitData->channel, pInitData);
    MCHP25LC256_spi_channel = pInitData->channel;
}

/************************************************************************
* Function: void MCHP25LC256WriteByte(BYTE data, WORD address)                                           
*                                                                       
* Overview: this function writes a byte to the address specified
*                                                                       
* Input: data to be written and address
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void MCHP25LC256WriteByte(BYTE data, WORD address)
{
    MCHP25LC256WriteEnable();
    MCHP25LC256CSLow();

    SPIPut(MCHP25LC256_spi_channel, EEPROM_CMD_WRITE);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, ((WORD_VAL) address).v[1]);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, ((WORD_VAL) address).v[0]);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, data);
    SPIGet(MCHP25LC256_spi_channel);

    MCHP25LC256CSHigh();

    // Wait for write end
    while(MCHP25LC256ReadStatus().Bits.WIP);
}

/************************************************************************
* Function: BYTE MCHP25LC256ReadByte(WORD address)             
*                                                                       
* Overview: this function reads a byte from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
BYTE MCHP25LC256ReadByte(WORD address)
{
    BYTE    temp;
    MCHP25LC256CSLow();

    SPIPut(MCHP25LC256_spi_channel, EEPROM_CMD_READ);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, ((WORD_VAL) address).v[1]);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, ((WORD_VAL) address).v[0]);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, 0);
    temp = SPIGet(MCHP25LC256_spi_channel);

    MCHP25LC256CSHigh();
    return (temp);
}

/************************************************************************
* Function: void MCHP25LC256WriteWord(WODR data, WORD address)                                           
*                                                                       
* Overview: this function writes a 16-bit word to the address specified
*                                                                       
* Input: data to be written and address
*                                                                       
* Output: none                                                         
*                                                                       
************************************************************************/
void MCHP25LC256WriteWord(WORD data, DWORD address)
{
     WORD        shortAddress = (WORD) address;
     
//        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
//    AD1PCFGLbits.PCFG0 = 1;
//    AD1PCFGLbits.PCFG6 = 1;
//    AD1PCFGLbits.PCFG7 = 1;
//    AD1PCFGLbits.PCFG8 = 1;
//        #endif

    MCHP25LC256WriteByte(((WORD_VAL) data).v[0], shortAddress);
    MCHP25LC256WriteByte(((WORD_VAL) data).v[1], shortAddress + 1);
}

/************************************************************************
* Function: WORD MCHP25LC256ReadWord(WORD address)             
*                                                                       
* Overview: this function reads a 16-bit word from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
WORD MCHP25LC256ReadWord(DWORD address)
{
    WORD        shortAddress = (WORD) address;
    WORD_VAL    temp;

//        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
//    AD1PCFGLbits.PCFG0 = 1;
//    AD1PCFGLbits.PCFG6 = 1;
//    AD1PCFGLbits.PCFG7 = 1;
//    AD1PCFGLbits.PCFG8 = 1;
//        #endif
    temp.v[0] = MCHP25LC256ReadByte(shortAddress);
    temp.v[1] = MCHP25LC256ReadByte(shortAddress + 1);

    return (temp.Val);
}

/************************************************************************
* Function: MCHP25LC256WriteEnable()                                         
*                                                                       
* Overview: this function allows write/erase MCHP25LC256. Must be called  
* before every write/erase command.                                         
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void MCHP25LC256WriteEnable(void)
{
    MCHP25LC256CSLow();
    SPIPut(MCHP25LC256_spi_channel, EEPROM_CMD_WREN);
    SPIGet(MCHP25LC256_spi_channel);
    MCHP25LC256CSHigh();
}

/************************************************************************
* Function: MCHP25LC256ReadStatus()                                          
*                                                                       
* Overview: this function reads status register
*                                                                       
* Input: none                                                          
*                                                                       
* Output: status register value
*                                                                       
************************************************************************/
union _MCHP25LC256Status_ MCHP25LC256ReadStatus(void)
{
    BYTE    temp;

    MCHP25LC256CSLow();
    SPIPut(MCHP25LC256_spi_channel, EEPROM_CMD_RDSR);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, 0);
    temp = SPIGet(MCHP25LC256_spi_channel);
    MCHP25LC256CSHigh();

    return (union _MCHP25LC256Status_)temp;
}

/************************************************************************
* Function: BYTE MCHP25LC256WriteArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function writes a data array at the address specified
*                                                                       
* Input: flash memory address, pointer to the data array, data number
*                                                                       
* Output: return 1 if the operation was successfull
*                                                                     
************************************************************************/
BYTE MCHP25LC256WriteArray(DWORD address, BYTE *pData, WORD nCount)
{
    DWORD_VAL   addr;
    BYTE        *pD;
    WORD        counter;

    addr.Val = address;
    pD = pData;

    // WRITE
    MCHP25LC256WriteEnable();
    MCHP25LC256CSLow();

    SPIPut(MCHP25LC256_spi_channel, EEPROM_CMD_WRITE);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, addr.v[1]);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, addr.v[0]);
    SPIGet(MCHP25LC256_spi_channel);

    for(counter = 0; counter < nCount; counter++)
    {
        SPIPut(MCHP25LC256_spi_channel, *pD++);
        SPIGet(MCHP25LC256_spi_channel);
        addr.Val++;

        // check for page rollover
        if((addr.v[0] & 0x7f) == 0)
        {
            MCHP25LC256CSHigh();

            // Wait for completion of the write operation
            while(MCHP25LC256ReadStatus().Bits.WIP);

            // Start writing of the next page
            MCHP25LC256WriteEnable();
            MCHP25LC256CSLow();

            SPIPut(MCHP25LC256_spi_channel, EEPROM_CMD_WRITE);
            SPIGet(MCHP25LC256_spi_channel);

            SPIPut(MCHP25LC256_spi_channel, addr.v[1]);
            SPIGet(MCHP25LC256_spi_channel);

            SPIPut(MCHP25LC256_spi_channel, addr.v[0]);
            SPIGet(MCHP25LC256_spi_channel);
        }
    }

    MCHP25LC256CSHigh();

    // Wait for write end
    while(MCHP25LC256ReadStatus().Bits.WIP);

    // VERIFY
    for(counter = 0; counter < nCount; counter++)
    {
        if(*pData != MCHP25LC256ReadByte(address))
            return (0);
        pData++;
        address++;
    }

    return (1);
}

/************************************************************************
* Function: void MCHP25LC256ReadArray(WORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function reads data into buffer specified
*                                                                       
* Input: flash memory address, pointer to the data buffer, data number
*                                                                       
************************************************************************/
void MCHP25LC256ReadArray(WORD address, BYTE *pData, WORD nCount)
{
    MCHP25LC256CSLow();

    SPIPut(MCHP25LC256_spi_channel, EEPROM_CMD_READ);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, ((WORD_VAL) address).v[1]);
    SPIGet(MCHP25LC256_spi_channel);

    SPIPut(MCHP25LC256_spi_channel, ((WORD_VAL) address).v[0]);
    SPIGet(MCHP25LC256_spi_channel);

    while(nCount--)
    {
        SPIPut(MCHP25LC256_spi_channel, 0);
        *pData++ = SPIGet(MCHP25LC256_spi_channel);
    }

    MCHP25LC256CSHigh();
}

#endif // #if defined (USE_25LC256)

