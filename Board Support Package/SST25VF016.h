/*****************************************************************************
 *
 * Basic access to SPI Flash SST25VF016 and M25P80
 *
 *****************************************************************************
 * FileName:        SST25VF016.h
 * Dependencies:    Graphics.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/07/09	   	...
 * 03/08/11     Modified header file dependencies
 * 05/11/11     - Updated this file to support both SST25VF016 and M25P80 families
 *              - Although the file name is still specific for SST25VF016
 *****************************************************************************/
#ifndef _SST25VF016_H
#define _SST25VF016_H

/************************************************************************
 * Section:  Includes                                                       
 ************************************************************************/
    #include "drv_spi.h"


/************************************************************************
* Macro: SST25CSLow()                                                   
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
    #define SST25CSLow()    SST25_CS_LAT = 0;

/************************************************************************
* Macro: SST25CSHigh()
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
    #define SST25CSHigh()   SST25_CS_LAT = 1;

/************************************************************************
* Function: SST25Init()
*                                                                       
* Overview: this function setups SPI and IOs connected to SST25
*                                                                       
* Input: none
*                                                                       
* Output: none  
*                                                                       
************************************************************************/
void    SST25Init(DRV_SPI_INIT_DATA *pInitData);

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
BYTE    SST25IsWriteBusy(void);

/************************************************************************
* Function: void SST25WriteByte(BYTE data, DWORD address)                                           
*                                                                       
* Overview: this function writes a byte to the address specified
*                                                                       
* Input: byte to be written and address
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void    SST25WriteByte(BYTE data, DWORD address);

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
BYTE    SST25ReadByte(DWORD address);

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
void    SST25WriteWord(WORD data, DWORD address);

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
WORD    SST25ReadWord(DWORD address);

/************************************************************************
* Function: SST25WriteEnable()                                       
*
* Overview: this function allows writing into SST25. Must be called
*           before every write/erase command
*
* Input: none
*            
* Output: none
*
************************************************************************/
void    SST25WriteEnable(void);

/************************************************************************
* Function: BYTE SST25WriteArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function writes data array at the address specified
*                                                                       
* Input: flash memory address, pointer to the data array, data number
*                                                                       
* Output: return 1 if the operation was successfull
*                                                                     
************************************************************************/
BYTE    SST25WriteArray(DWORD address, BYTE *pData, WORD nCount);

/************************************************************************
* Function: void SST25ReadArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function reads  data into buffer specified
*                                                                       
* Input: flash memory address, pointer to the buffer, data number
*                                                                       
************************************************************************/
void    SST25ReadArray(DWORD address, BYTE *pData, WORD nCount);

/************************************************************************
* Function: void SST25ChipErase(void)
*                                                                       
* Overview: chip erase
*                                                                       
* Input: none
*                                                                       
************************************************************************/
void    SST25ChipErase(void);

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
void    SST25ResetWriteProtection(void);

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
void    SST25SectorErase(DWORD address);
#endif //_SST25VF016_H
