/*****************************************************************************
 *
 * Basic access to SPI EEPROM 25LC256 located on Explorer 16.
 *
 *****************************************************************************
 * FileName:        25LC256.c
 * Dependencies:    EEPROM.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		07/10/07	...
 * Anton Alkhimenok     02/07/08    PIC32 support
 *****************************************************************************/
#include "EEPROM.h"

#if defined (USE_MCHP25LC256)

/************************************************************************
* Function: EEPROMInit                                                  
*                                                                       
* Overview: this function setup SPI and IOs connected to EEPROM
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void EEPROMInit(void)
{

    // Initialize SPI
        #ifdef __PIC32MX
    SPI2STAT = 0;
    SPI2CON = 0;
    SPI2BRG = 0;
    SPI2CONbits.MSTEN = 1;
    SPI2CONbits.CKP = 1;
    SPI2CONbits.SMP = 1;
    SPI2BRG = 1;
    SPI2CONbits.ON = 1;
        #else
    SPI2STAT = 0;
    SPI2CON1 = 0x001b;
    SPI2CON1bits.MSTEN = 1;
    SPI2CON2 = 0;
    SPI2CON1bits.MODE16 = 0;
    SPI2CON1bits.CKE = 0;
    SPI2CON1bits.CKP = 1;
    SPI2CON1bits.SMP = 1;
    SPI2STATbits.SPIEN = 1;
        #endif

    // Set IOs directions for EEPROM SPI
    EEPROM_CS_LAT = 1;
    EEPROM_CS_TRIS = 0;
    EEPROM_SCK_TRIS = 0;
    EEPROM_SDO_TRIS = 0;
    EEPROM_SDI_TRIS = 1;

        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    AD1PCFGL = 0xFFFF;
    RPOR9bits.RP18R = 11;                   // assign RP18 for SCK2
    RPOR8bits.RP16R = 10;                   // assign RP16 for SDO2
    RPINR22bits.SDI2R = 17;                 // assign RP17 for SDI2
        #elif defined(__PIC24FJ256GA110__)
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR10bits.RP21R = 11;                  // assign RP21 for SCK2
    RPOR9bits.RP19R = 10;                   // assign RP19 for SDO2
    RPINR22bits.SDI2R = 26;                 // assign RP26 for SDI2
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
        #endif
}

/************************************************************************
* Function SPIPut(BYTE data)                                                       
*                                                                       
* Overview:  this function sends a byte                     
*                                                                       
* Input: byte to be sent
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SPIPut(BYTE data)
{
        #ifdef __PIC32MX

    // Wait for free buffer
    while(!SPI2STATbits.SPITBE);
    SPI2BUF = data;

    // Wait for data byte
    while(!SPI2STATbits.SPIRBF);
        #else

    // Wait for free buffer
    while(SPI2STATbits.SPITBF);
    SPI2BUF = data;

    // Wait for data byte
    while(!SPI2STATbits.SPIRBF);
        #endif
}

/************************************************************************
* Macros SPIGet()                                                       
*                                                                       
* Overview:  this macros gets a byte from SPI                      
*                                                                       
* Input: none
*                                                                       
* Output: none
*                                                                       
************************************************************************/
    #define SPIGet() \
    /* */            \
    SPI2BUF

/************************************************************************
* Function: void EEPROMWriteByte(BYTE data, WORD address)                                           
*                                                                       
* Overview: this function writes a byte to the address specified
*                                                                       
* Input: data to be written and address
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void EEPROMWriteByte(BYTE data, WORD address)
{
    EEPROMWriteEnable();
    EEPROMCSLow();

    SPIPut(EEPROM_CMD_WRITE);
    SPIGet();

    SPIPut(((WORD_VAL) address).v[1]);
    SPIGet();

    SPIPut(((WORD_VAL) address).v[0]);
    SPIGet();

    SPIPut(data);
    SPIGet();

    EEPROMCSHigh();

    // Wait for write end
    while(EEPROMReadStatus().Bits.WIP);
}

/************************************************************************
* Function: BYTE EEPROMReadByte(WORD address)             
*                                                                       
* Overview: this function reads a byte from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
BYTE EEPROMReadByte(WORD address)
{
    BYTE    temp;
    EEPROMCSLow();

    SPIPut(EEPROM_CMD_READ);
    SPIGet();

    SPIPut(((WORD_VAL) address).v[1]);
    SPIGet();

    SPIPut(((WORD_VAL) address).v[0]);
    SPIGet();

    SPIPut(0);
    temp = SPIGet();

    EEPROMCSHigh();
    return (temp);
}

/************************************************************************
* Function: void EEPROMWriteWord(WODR data, WORD address)                                           
*                                                                       
* Overview: this function writes a 16-bit word to the address specified
*                                                                       
* Input: data to be written and address
*                                                                       
* Output: none                                                         
*                                                                       
************************************************************************/
void EEPROMWriteWord(WORD data, WORD address)
{
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    AD1PCFGLbits.PCFG0 = 1;
    AD1PCFGLbits.PCFG6 = 1;
    AD1PCFGLbits.PCFG7 = 1;
    AD1PCFGLbits.PCFG8 = 1;
        #endif
    EEPROMWriteByte(((WORD_VAL) data).v[0], address);
    EEPROMWriteByte(((WORD_VAL) data).v[1], address + 1);
}

/************************************************************************
* Function: WORD EEPROMReadWord(WORD address)             
*                                                                       
* Overview: this function reads a 16-bit word from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
WORD EEPROMReadWord(WORD address)
{
    WORD_VAL    temp;

        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    AD1PCFGLbits.PCFG0 = 1;
    AD1PCFGLbits.PCFG6 = 1;
    AD1PCFGLbits.PCFG7 = 1;
    AD1PCFGLbits.PCFG8 = 1;
        #endif
    temp.v[0] = EEPROMReadByte(address);
    temp.v[1] = EEPROMReadByte(address + 1);

    return (temp.Val);
}

/************************************************************************
* Function: EEPROMWriteEnable()                                         
*                                                                       
* Overview: this function allows write/erase EEPROM. Must be called  
* before every write/erase command.                                         
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void EEPROMWriteEnable(void)
{
    EEPROMCSLow();
    SPIPut(EEPROM_CMD_WREN);
    SPIGet();
    EEPROMCSHigh();
}

/************************************************************************
* Function: EEPROMReadStatus()                                          
*                                                                       
* Overview: this function reads status register
*                                                                       
* Input: none                                                          
*                                                                       
* Output: status register value
*                                                                       
************************************************************************/
union _EEPROMStatus_ EEPROMReadStatus(void)
{
    BYTE    temp;

    EEPROMCSLow();
    SPIPut(EEPROM_CMD_RDSR);
    SPIGet();

    SPIPut(0);
    temp = SPIGet();
    EEPROMCSHigh();

    return (union _EEPROMStatus_)temp;
}

/************************************************************************
* Function: BYTE EEPROMWriteArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function writes a data array at the address specified
*                                                                       
* Input: flash memory address, pointer to the data array, data number
*                                                                       
* Output: return 1 if the operation was successfull
*                                                                     
************************************************************************/
BYTE EEPROMWriteArray(DWORD address, BYTE *pData, WORD nCount)
{
    DWORD_VAL   addr;
    BYTE        *pD;
    WORD        counter;

    addr.Val = address;
    pD = pData;

    // WRITE
    EEPROMWriteEnable();
    EEPROMCSLow();

    SPIPut(EEPROM_CMD_WRITE);
    SPIGet();

    SPIPut(addr.v[1]);
    SPIGet();

    SPIPut(addr.v[0]);
    SPIGet();

    for(counter = 0; counter < nCount; counter++)
    {
        SPIPut(*pD++);
        SPIGet();
        addr.Val++;

        // check for page rollover
        if((addr.v[0] & 0x7f) == 0)
        {
            EEPROMCSHigh();

            // Wait for completion of the write operation
            while(EEPROMReadStatus().Bits.WIP);

            // Start writing of the next page
            EEPROMWriteEnable();
            EEPROMCSLow();

            SPIPut(EEPROM_CMD_WRITE);
            SPIGet();

            SPIPut(addr.v[1]);
            SPIGet();

            SPIPut(addr.v[0]);
            SPIGet();
        }
    }

    EEPROMCSHigh();

    // Wait for write end
    while(EEPROMReadStatus().Bits.WIP);

    // VERIFY
    for(counter = 0; counter < nCount; counter++)
    {
        if(*pData != EEPROMReadByte(address))
            return (0);
        pData++;
        address++;
    }

    return (1);
}

/************************************************************************
* Function: void EEPROMReadArray(WORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function reads data into buffer specified
*                                                                       
* Input: flash memory address, pointer to the data buffer, data number
*                                                                       
************************************************************************/
void EEPROMReadArray(WORD address, BYTE *pData, WORD nCount)
{
    EEPROMCSLow();

    SPIPut(EEPROM_CMD_READ);
    SPIGet();

    SPIPut(((WORD_VAL) address).v[1]);
    SPIGet();

    SPIPut(((WORD_VAL) address).v[0]);
    SPIGet();

    while(nCount--)
    {
        SPIPut(0);
        *pData++ = SPIGet();
    }

    EEPROMCSHigh();
}

#endif // #if defined (USE_MCHP25LC256)

