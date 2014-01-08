/*****************************************************************************
 *
 * Basic access to SPI EEPROM 25LC256 located on Explorer 16.
 *
 *****************************************************************************
 * FileName:        EEPROM.h
 * Dependencies:    Graphics.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 07/10/07		...
 * 02/07/08    	PIC32 support
 *****************************************************************************/
#ifndef _EEPROM_H
    #define _EEPROM_H

   	#include "Compiler.h"
    #include "GenericTypeDefs.h"
	#include "HardwareProfile.h"
	
/************************************************************************
* EEPROM Commands                                                       
************************************************************************/
    #define EEPROM_PAGE_SIZE    (unsigned)64
    #define EEPROM_PAGE_MASK    (unsigned)0x003f
    #define EEPROM_CMD_READ     (unsigned)0x03
    #define EEPROM_CMD_WRITE    (unsigned)0x02
    #define EEPROM_CMD_WRDI     (unsigned)0x04
    #define EEPROM_CMD_WREN     (unsigned)0x06
    #define EEPROM_CMD_RDSR     (unsigned)0x05
    #define EEPROM_CMD_WRSR     (unsigned)0x01

/************************************************************************
* Structure STATREG and union _EEPROMStatus_                            
*                                                                       
* Overview: provide bits and byte access to EEPROM status value      
*                                                                       
************************************************************************/
struct STATREG
{
    BYTE    WIP : 1;
    BYTE    WEL : 1;
    BYTE    BP0 : 1;
    BYTE    BP1 : 1;
    BYTE    RESERVED : 3;
    BYTE    WPEN : 1;
};

union _EEPROMStatus_
{
    struct STATREG  Bits;
    BYTE            Char;
};

/************************************************************************
* Macro: EEPROMCSLow()                                                   
*                                                                       
* Preconditions: CS IO must be configured as output
*                                                                       
* Overview: this macro pulls down CS line                    
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none                                                         
*                                                                       
************************************************************************/
    #define EEPROMCSLow()   EEPROM_CS_LAT = 0;

/************************************************************************
* Macro: EEPROMCSHigh()
*                                                                       
* Preconditions: CS IO must be configured as output
*                                                                       
* Overview: this macro set CS line to high level
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
    #define EEPROMCSHigh()  EEPROM_CS_LAT = 1;

/************************************************************************
* Function: EEPROMInit()
*                                                                       
* Overview: this function setups SPI and IOs connected to EEPROM
*                                                                       
* Input: none
*                                                                       
* Output: none  
*                                                                       
************************************************************************/
void                    EEPROMInit(void);

/************************************************************************
* Function: union _EEPROMStatus_ EEPROMReadStatus()
*                                                                       
* Overview: this function reads status register
*                                                                       
* Input: none             
*                                                                       
* Output: status register value
*                                                                       
************************************************************************/
union _EEPROMStatus_    EEPROMReadStatus(void);

/************************************************************************
* Function: void EEPROMWriteByte()                                           
*                                                                       
* Overview: this function writes a byte to the address specified
*                                                                       
* Input: byte to be written and address
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void                    EEPROMWriteByte(BYTE data, WORD address);

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
BYTE                    EEPROMReadByte(WORD address);

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
void                    EEPROMWriteWord(WORD data, WORD address);

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
WORD                    EEPROMReadWord(WORD address);

/************************************************************************
* Function: EEPROMWriteEnable()                                       
*
* Overview: this function allows writing into EEPROM. Must be called
*           before every write/erase command
*
* Input: none
*            
* Output: none
*
************************************************************************/
void                    EEPROMWriteEnable(void);

/************************************************************************
* Function: BYTE EEPROMWriteArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function writes data array at the address specified
*                                                                       
* Input: flash memory address, pointer to the data array, data number
*                                                                       
* Output: return 1 if the operation was successfull
*                                                                     
************************************************************************/
BYTE                    EEPROMWriteArray(DWORD address, BYTE *pData, WORD nCount);

/************************************************************************
* Function: void EEPROMReadArray(WORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function reads  data into buffer specified
*                                                                       
* Input: flash memory address, pointer to the buffer, data number
*                                                                       
************************************************************************/
void                    EEPROMReadArray(WORD address, BYTE *pData, WORD nCount);
#endif //_EEPROM_H
