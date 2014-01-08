/*****************************************************************************
 *
 * Basic access to SPI Flash SST25VF016 and M25P80
 *
 *****************************************************************************
 * FileName:        SST25VF016.c
 * Dependencies:    SST25VF016.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/07/09	   ...
 * 03/08/11    - Modified header file dependencies
 *             - Abstracted out SPI component 
 * 05/11/11    - Updated this file to support both SST25VF016 and M25P80 families
 *             - Although the file name is still specific for SST25VF016
 * 02/29/12    - Updated this file for specific M25P80 families usage.
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined (USE_SST25VF016) || defined(USE_M25P80)

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "drv_spi.h"
#include "SST25VF016.h"

/************************************************************************
* SST25 Commands                                                       
************************************************************************/
    #define SST25_CMD_READ  (unsigned)0x03
    #define SST25_CMD_WRITE (unsigned)0x02
    #define SST25_CMD_WREN  (unsigned)0x06
	#define SST25_CMD_RDSR  (unsigned)0x05

    #if defined(USE_M25P80)
      #define SST25_CMD_EWSR  (unsigned)0x06
      #define SST25_CMD_SER   (unsigned)0xD8
      #define SST25_CMD_READ_HIGH_SPEED  (unsigned) 0x0B
      #define SST25_CMD_ERASE (unsigned) 0xC7  // in M25P80 data sheet this is BULK ERASE
	  
      #define SST25_WIP_STATUS (unsigned) 0x01
      #define SST25_WEL_STATUS (unsigned) 0x02
	  #define SST25_STATUS_MASK (unsigned) 0x60
    #elif defined(USE_SST25VF016)
      #define SST25_CMD_EWSR  (unsigned)0x50
      #define SST25_CMD_SER   (unsigned)0x20
      #define SST25_CMD_ERASE (unsigned)0x60
	#else
	  #error "EWSR and SER commands not defined for the selected Serial flash."
    #endif	
     
    #define SST25_CMD_WRSR  (unsigned)0x01  // Write Status Register 


static DRV_SPI_INIT_DATA spiInitData;

/************************************************************************
* Function: SST25Init                                                  
*                                                                       
* Overview: this function setup SPI and IOs connected to SST25
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST25Init(DRV_SPI_INIT_DATA *pInitData)
{
    // initialize the SPI channel to be used
    // SPI_CHANNEL_USED is defined in the hardware profile
    DRV_SPI_Initialize(pInitData->channel, pInitData);
    memcpy(&spiInitData, pInitData, sizeof(DRV_SPI_INIT_DATA));

    SST25ResetWriteProtection();
}

/************************************************************************
* Function: void SST25WriteByte(BYTE data, DWORD address)                                           
*                                                                       
* Overview: this function writes a byte to the address specified
*                                                                       
* Input: data to be written and address
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void SST25WriteByte(BYTE data, DWORD address)
{
    SST25WriteEnable();

    while(!SPILock(spiInitData.channel))
        ;

    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    SST25CSLow();

    SPIPut(spiInitData.channel, SST25_CMD_WRITE);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[2]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[1]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[0]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, data);
    SPIGet(spiInitData.channel);

    SST25CSHigh();

    SPIUnLock(spiInitData.channel);

    // Wait for write end
    while(SST25IsWriteBusy());
}

/************************************************************************
* Function: BYTE SST25ReadByte(DWORD address)             
*                                                                       
* Overview: this function reads a byte from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
BYTE SST25ReadByte(DWORD address)
{
    BYTE    temp;
    
    while(!SPILock(spiInitData.channel))
        ;
    
    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    SST25CSLow();

    SPIPut(spiInitData.channel, SST25_CMD_READ);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[2]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[1]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[0]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, 0);
    temp = SPIGet(spiInitData.channel);

    SST25CSHigh();

    SPIUnLock(spiInitData.channel);

    return (temp);
}

/************************************************************************
* Function: void SST25WriteWord(WODR data, DWORD address)                                           
*                                                                       
* Overview: this function writes a 16-bit word to the address specified
*                                                                       
* Input: data to be written and address
*                                                                       
* Output: none                                                         
*                                                                       
************************************************************************/
void SST25WriteWord(WORD data, DWORD address)
{
    SST25WriteByte(((WORD_VAL) data).v[0], address);
    SST25WriteByte(((WORD_VAL) data).v[1], address + 1);
}

/************************************************************************
* Function: WORD SST25ReadWord(DWORD address)             
*                                                                       
* Overview: this function reads a 16-bit word from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
WORD SST25ReadWord(DWORD address)
{
    WORD_VAL    temp;

    temp.v[0] = SST25ReadByte(address);
    temp.v[1] = SST25ReadByte(address + 1);

    return (temp.Val);
}

/************************************************************************
* Function: SST25WriteEnable()                                         
*                                                                       
* Overview: this function allows write/erase SST25. Must be called  
* before every write/erase command.                                         
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void SST25WriteEnable(void)
{
    while(!SPILock(spiInitData.channel))
        ;
    
    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    SST25CSLow();
    SPIPut(spiInitData.channel, SST25_CMD_WREN);
    SPIGet(spiInitData.channel);
    SST25CSHigh();
    SPIUnLock(spiInitData.channel);
}

/************************************************************************
* Function: BYTE SST25IsWriteBusy(void)  
*                                                                       
* Overview: this function reads status register and chek BUSY bit for write operation
*                                                                       
* Input: none                                                          
*                                                                       
* Output: non zero if busy
*                                                                       
************************************************************************/
BYTE SST25IsWriteBusy(void)
{
    BYTE    temp;

    while(!SPILock(spiInitData.channel))
        ;

    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    SST25CSLow();
    SPIPut(spiInitData.channel, SST25_CMD_RDSR);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, 0);
    temp = SPIGet(spiInitData.channel);
    SST25CSHigh();
    SPIUnLock(spiInitData.channel);

    return (temp & 0x01);
}

/************************************************************************
* Function: BYTE SST25WriteArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function writes a data array at the address specified
*                                                                       
* Input: flash memory address, pointer to the data array, data number
*                                                                       
* Output: return 1 if the operation was successfull
*                                                                     
************************************************************************/
BYTE SST25WriteArray(DWORD address, BYTE *pData, WORD nCount)
{
    DWORD   addr;
    BYTE    *pD;
    WORD    counter;
#if defined(USE_M25P80)    
    BYTE status;
#endif

    addr = address;
    pD = pData;

    // WRITE
    for(counter = 0; counter < nCount; counter++)
    {
        SST25WriteByte(*pD++, addr++);
    }

#if defined(USE_M25P80)

    // check status of Write in Progress
    // wait for BULK ERASE to be done
    SST25CSLow();
    SPIPut(spiInitData.channel, SST25_CMD_RDSR);
    SPIGet(spiInitData.channel);
    while(1)
    {
        SPIPut(spiInitData.channel, 0);
        status = (SPIGet(spiInitData.channel)& SST25_WIP_STATUS);
        if ((status & SST25_WIP_STATUS) == 0)
            break;
    }

    SST25CSHigh();
	
#endif

    // VERIFY
    for(counter = 0; counter < nCount; counter++)
    {
        if(*pData != SST25ReadByte(address))
            return (0);
        pData++;
        address++;
    }

    return (1);
}

/************************************************************************
* Function: void SST25ReadArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function reads data into buffer specified
*                                                                       
* Input: flash memory address, pointer to the data buffer, data number
*                                                                       
************************************************************************/
void SST25ReadArray(DWORD address, BYTE *pData, WORD nCount)
{
    while(!SPILock(spiInitData.channel))
        ;

    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    SST25CSLow();

    SPIPut(spiInitData.channel, SST25_CMD_READ);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[2]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[1]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[0]);
    SPIGet(spiInitData.channel);

    while(nCount--)
    {
        SPIPut(spiInitData.channel, 0);
        *pData++ = SPIGet(spiInitData.channel);
    }

    SST25CSHigh();
    SPIUnLock(spiInitData.channel);
}

/************************************************************************
* Function: void SST25ChipErase(void)
*                                                                       
* Overview: chip erase
*                                                                       
* Input: none
*                                                                       
************************************************************************/
void SST25ChipErase(void)
{
#if defined(USE_M25P80)    
    BYTE status;
#endif

#if defined(USE_SST25VF016)
    SST25WriteEnable();
#endif

    while(!SPILock(spiInitData.channel))
        ;

    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);
    SST25CSLow();
#if defined(USE_M25P80)    
    // send write enable command
    SPIPut(spiInitData.channel, SST25_CMD_EWSR);
    SPIGet(spiInitData.channel);

    SST25CSHigh();
    Nop();
    SST25CSLow();

    // verify if the WEL bit is set high
    while(1)
    {
        SPIPut(spiInitData.channel, SST25_CMD_RDSR);
        SPIGet(spiInitData.channel);
        status = 0xFF;
        while((status & SST25_STATUS_MASK) > 0)
        {
            SPIPut(spiInitData.channel, 0);
            status = SPIGet(spiInitData.channel);
        }
        if ((status & SST25_WEL_STATUS) == SST25_WEL_STATUS)
            break;
    }
    SST25CSHigh();
#endif

    SST25CSLow();

    SPIPut(spiInitData.channel, SST25_CMD_ERASE);
    SPIGet(spiInitData.channel);

    SST25CSHigh();

#if defined (USE_M25P80)
    // wait for BULK ERASE to be done
    SST25CSLow();
    SPIPut(spiInitData.channel, SST25_CMD_RDSR);
    SPIGet(spiInitData.channel);
    while(1)
    {
        SPIPut(spiInitData.channel, 0);
        status = (SPIGet(spiInitData.channel)& SST25_WIP_STATUS);
        if ((status & SST25_WIP_STATUS) == 0)
            break;
    }

    SST25CSHigh();
#endif
    SPIUnLock(spiInitData.channel);
    
    // Wait for write end
    while(SST25IsWriteBusy());
}

/************************************************************************
* Function: void SST25ResetWriteProtection()
*                                                                       
* Overview: this function reset write protection bits
*                                                                       
* Input: none                                                     
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST25ResetWriteProtection(void)
{
#if defined(USE_M25P80)    
    BYTE status;
#endif

    while(!SPILock(spiInitData.channel))
        ;
    
    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    SST25CSLow();
    
    // send write enable command
    SPIPut(spiInitData.channel, SST25_CMD_EWSR);
    SPIGet(spiInitData.channel);
	
    SST25CSHigh();
	
#if defined(USE_M25P80)    
    SST25CSLow();
    // verify if the WEL bit is set high
    while(1)
    {
        SPIPut(spiInitData.channel, SST25_CMD_RDSR);
        SPIGet(spiInitData.channel);

        status = 0xFF;
        while((status & SST25_STATUS_MASK) > 0)
        {
            SPIPut(spiInitData.channel, 0);
            status = SPIGet(spiInitData.channel);
        }
        if ((status & SST25_WEL_STATUS) == SST25_WEL_STATUS)
            break;
    }
    SST25CSHigh();
#endif


    SST25CSLow();

    SPIPut(spiInitData.channel, SST25_CMD_WRSR);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, 0);
    SPIGet(spiInitData.channel);

    SST25CSHigh();
    SPIUnLock(spiInitData.channel);

    // Wait for write end
    while(SST25IsWriteBusy());
}

/************************************************************************
* Function: void SST25SectorErase(DWORD address)                                           
*                                                                       
* Overview: this function erases a 4Kb sector
*                                                                       
* Input: address within sector to be erased
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void SST25SectorErase(DWORD address)
{
    SST25WriteEnable();
    
    while(!SPILock(spiInitData.channel))
        ;

    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    SST25CSLow();

    SPIPut(spiInitData.channel, SST25_CMD_SER);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[2]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[1]);
    SPIGet(spiInitData.channel);

    SPIPut(spiInitData.channel, ((DWORD_VAL) address).v[0]);
    SPIGet(spiInitData.channel);

    SST25CSHigh();
    SPIUnLock(spiInitData.channel);

    // Wait for write end
    while(SST25IsWriteBusy());
}

#endif // #if defined (USE_SST25VF016) || defined(USE_M25P80)

