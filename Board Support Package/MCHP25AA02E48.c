/*****************************************************************************
*
 * Basic access to Microchip 2K SPI Bus Serial EEPROM with 
 * EUI-48™ Node Identity.
 *
 *****************************************************************************
 * FileName:        MCHP25AA02E48.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 06/27/11	    ...
 * 
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined (USE_MCHP25AA02E48)

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "drv_spi.h"
#include "MCHP25AA02E48.h"

static DRV_SPI_INIT_DATA eepromInitData;

/************************************************************************
* Function: MCHP25AA02E48Init                                                  
*                                                                       
* Overview: this function setup SPI and IOs connected to EEPROM
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void MCHP25AA02E48Init(DRV_SPI_INIT_DATA *pInitData)
{
    // initialize the SPI channel to be used
    // SPI_CHANNEL_USED is defined in the hardware profile
    DRV_SPI_Initialize(pInitData->channel, pInitData);
    memcpy(&eepromInitData, pInitData, sizeof(DRV_SPI_INIT_DATA));
}

/************************************************************************
* Function: void MCHP25AA02E48WriteByte(BYTE data, WORD address)                                           
*                                                                       
* Overview: this function writes a byte to the address specified
*                                                                       
* Input: data to be written and address
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void MCHP25AA02E48WriteByte(BYTE data, WORD address)
{
    MCHP25AA02E48WriteEnable();

    while(!DRVSPILock(eepromInitData.channel))
        ;

    DRV_SPI_Initialize(eepromInitData.channel, (DRV_SPI_INIT_DATA *)&eepromInitData);

    MCHP25AA02E48CSLow();

    DRVSPIPut(eepromInitData.channel, EEPROM_CMD_WRITE);
    DRVSPIGet(eepromInitData.channel);

    DRVSPIPut(eepromInitData.channel, ((WORD_VAL) address).v[0]);
    DRVSPIGet(eepromInitData.channel);

    DRVSPIPut(eepromInitData.channel, data);
    DRVSPIGet(eepromInitData.channel);

    MCHP25AA02E48CSHigh();

    DRVSPIUnLock(eepromInitData.channel);

    // Wait for write end
    while(MCHP25AA02E48ReadStatus().Bits.WIP);
}

/************************************************************************
* Function: BYTE MCHP25AA02E48ReadByte(WORD address)             
*                                                                       
* Overview: this function reads a byte from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
BYTE MCHP25AA02E48ReadByte(WORD address)
{
    BYTE    temp;
    
    
    while(!DRVSPILock(eepromInitData.channel))
        ;

    DRV_SPI_Initialize(eepromInitData.channel, (DRV_SPI_INIT_DATA *)&eepromInitData);

    MCHP25AA02E48CSLow();

    DRVSPIPut(eepromInitData.channel, EEPROM_CMD_READ);
    DRVSPIGet(eepromInitData.channel);

    DRVSPIPut(eepromInitData.channel, ((WORD_VAL) address).v[0]);
    DRVSPIGet(eepromInitData.channel);

    DRVSPIPut(eepromInitData.channel, 0);
    temp = DRVSPIGet(eepromInitData.channel);

    MCHP25AA02E48CSHigh();
    DRVSPIUnLock(eepromInitData.channel);

    return (temp);
}

/************************************************************************
* Function: void MCHP25AA02E48WriteWord(WODR data, WORD address)                                           
*                                                                       
* Overview: this function writes a 16-bit word to the address specified
*                                                                       
* Input: data to be written and address
*                                                                       
* Output: none                                                         
*                                                                       
************************************************************************/
void MCHP25AA02E48WriteWord(WORD data, DWORD address)
{
    WORD        shortAddress = (WORD) address;

    MCHP25AA02E48WriteByte(((WORD_VAL) data).v[0], shortAddress);
    MCHP25AA02E48WriteByte(((WORD_VAL) data).v[1], shortAddress + 1);
}

/************************************************************************
* Function: WORD MCHP25AA02E48ReadWord(WORD address)             
*                                                                       
* Overview: this function reads a 16-bit word from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
WORD MCHP25AA02E48ReadWord(DWORD address)
{
    WORD        shortAddress = (WORD) address;
    WORD_VAL    temp;

    temp.v[0] = MCHP25AA02E48ReadByte(shortAddress);
    temp.v[1] = MCHP25AA02E48ReadByte(shortAddress + 1);

    return (temp.Val);
}

/************************************************************************
* Function: MCHP25AA02E48WriteEnable()                                         
*                                                                       
* Overview: this function allows write/erase MCHP25AA02E48. Must be called  
* before every write/erase command.                                         
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void MCHP25AA02E48WriteEnable(void)
{
    while(!DRVSPILock(eepromInitData.channel))
        ;

    DRV_SPI_Initialize(eepromInitData.channel, (DRV_SPI_INIT_DATA *)&eepromInitData);

    MCHP25AA02E48CSLow();
    DRVSPIPut(eepromInitData.channel, EEPROM_CMD_WREN);
    DRVSPIGet(eepromInitData.channel);
    MCHP25AA02E48CSHigh();
    
    DRVSPIUnLock(eepromInitData.channel);
}

/************************************************************************
* Function: MCHP25AA02E48ReadStatus()                                          
*                                                                       
* Overview: this function reads status register
*                                                                       
* Input: none                                                          
*                                                                       
* Output: status register value
*                                                                       
************************************************************************/
union _MCHP25AA02E48Status_ MCHP25AA02E48ReadStatus(void)
{
    BYTE    temp;

    while(!DRVSPILock(eepromInitData.channel))
        ;

    DRV_SPI_Initialize(eepromInitData.channel, (DRV_SPI_INIT_DATA *)&eepromInitData);

    MCHP25AA02E48CSLow();
    DRVSPIPut(eepromInitData.channel, EEPROM_CMD_RDSR);
    DRVSPIGet(eepromInitData.channel);

    DRVSPIPut(eepromInitData.channel, 0);
    temp = DRVSPIGet(eepromInitData.channel);
    MCHP25AA02E48CSHigh();
    DRVSPIUnLock(eepromInitData.channel);

    return (union _MCHP25AA02E48Status_)temp;
}

/************************************************************************
* Function: BYTE MCHP25AA02E48WriteArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function writes a data array at the address specified
*                                                                       
* Input: flash memory address, pointer to the data array, data number
*                                                                       
* Output: return 1 if the operation was successfull
*                                                                     
************************************************************************/
BYTE MCHP25AA02E48WriteArray(DWORD address, BYTE *pData, WORD nCount)
{
    DWORD_VAL   addr;
    BYTE        *pD;
    WORD        counter;

    addr.Val = address;
    pD = pData;

    // WRITE
    MCHP25AA02E48WriteEnable();
    
    while(!DRVSPILock(eepromInitData.channel))
        ;

    DRV_SPI_Initialize(eepromInitData.channel, (DRV_SPI_INIT_DATA *)&eepromInitData);

    MCHP25AA02E48CSLow();

    DRVSPIPut(eepromInitData.channel, EEPROM_CMD_WRITE);
    DRVSPIGet(eepromInitData.channel);

    DRVSPIPut(eepromInitData.channel, addr.v[0]);
    DRVSPIGet(eepromInitData.channel);

    for(counter = 0; counter < nCount; counter++)
    {
        DRVSPIPut(eepromInitData.channel, *pD++);
        DRVSPIGet(eepromInitData.channel);
        addr.Val++;

        // check for page rollover
        if((addr.v[0] & 0x7f) == 0)
        {
            MCHP25AA02E48CSHigh();

            // Wait for completion of the write operation
            while(MCHP25AA02E48ReadStatus().Bits.WIP);

            // Start writing of the next page
            MCHP25AA02E48WriteEnable();
            MCHP25AA02E48CSLow();

            DRVSPIPut(eepromInitData.channel, EEPROM_CMD_WRITE);
            DRVSPIGet(eepromInitData.channel);

            DRVSPIPut(eepromInitData.channel, addr.v[0]);
            DRVSPIGet(eepromInitData.channel);
        }
    }

    MCHP25AA02E48CSHigh();
    DRVSPIUnLock(eepromInitData.channel);

    // Wait for write end
    while(MCHP25AA02E48ReadStatus().Bits.WIP);

    // VERIFY
    for(counter = 0; counter < nCount; counter++)
    {
        if(*pData != MCHP25AA02E48ReadByte(address))
            return (0);
        pData++;
        address++;
    }

    return (1);
}

/************************************************************************
* Function: void MCHP25AA02E48ReadArray(WORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function reads data into buffer specified
*                                                                       
* Input: flash memory address, pointer to the data buffer, data number
*                                                                       
************************************************************************/
void MCHP25AA02E48ReadArray(WORD address, BYTE *pData, WORD nCount)
{
    while(!DRVSPILock(eepromInitData.channel))
        ;

    DRV_SPI_Initialize(eepromInitData.channel, (DRV_SPI_INIT_DATA *)&eepromInitData);

    MCHP25AA02E48CSLow();

    DRVSPIPut(eepromInitData.channel, EEPROM_CMD_READ);
    DRVSPIGet(eepromInitData.channel);

    DRVSPIPut(eepromInitData.channel, ((WORD_VAL) address).v[0]);
    DRVSPIGet(eepromInitData.channel);

    while(nCount--)
    {
        DRVSPIPut(eepromInitData.channel, 0);
        *pData++ = DRVSPIGet(eepromInitData.channel);
    }

    MCHP25AA02E48CSHigh();
    DRVSPIUnLock(eepromInitData.channel);
}
/************************************************************************
* Function: DWORD MCHP25AA02E48GetEUI48NodeAddress(BYTE *eui48NodeAddr)
************************************************************************/
void MCHP25AA02E48GetEUI48NodeAddress(BYTE *eui48NodeAddr)
{
    MCHP25AA02E48ReadArray(0xFA, eui48NodeAddr, 6);
}
/************************************************************************
* Function: void MCHP25AA02E64GetEUI48NodeAddress(BYTE *eui64NodeAddr)
************************************************************************/
void MCHP25AA02E48GetEUI64NodeAddress(BYTE *eui64NodeAddr)
{
    MCHP25AA02E48ReadArray(0xFA, eui64NodeAddr, 3);
    MCHP25AA02E48ReadArray(0xFD, (BYTE *)&eui64NodeAddr[5], 3);

    eui64NodeAddr[3] = 0xFF;
    eui64NodeAddr[4] = 0xFE;
}
#endif // #if defined (USE_25LC256)

