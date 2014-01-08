/*****************************************************************************
 *
 * Basic access to parallel FLASH SST39LF400
 *
 *****************************************************************************
 * FileName:        SST39LF400.h
 * Dependencies:    none
 * Processor:       PIC24F
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2009 Microchip Technology Inc.  All rights reserved.
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
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 4/12/2010	Original, ported from SST39LF400A
 * 8/11/2010	Removed dependency on Graphics file. 
 * 1/14/2011    Swapped SST39LF400WriteWord() parameters for consistency
 *              with other drivers.
 * 5/25/2011    Modified WriteArray and ReadArray functions to match
 *              common interface for other external flash drivers.
 ********************************************************************/
 
#ifndef _SST39LF400_H
#define _SST39LF400_H

    #include "Compiler.h"
    #include "GenericTypeDefs.h"
	#include "HardwareProfile.h"

 /************************************************************************
* Function: SST39LF400Init                                                  
*                                                                       
* Overview: this function initializes IOs and PMP.
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST39LF400Init();

 /************************************************************************
* Function: BYTE SST39LF400WriteWord(DWORD address, WORD data)
*                                                                       
* Overview: this function writes one word
*                                                                       
* Input: address and word to be written
*                                                                       
* Output: non-zero if it's successful
*
* Notes: none
*                                                                       
************************************************************************/
BYTE SST39LF400WriteWord(WORD data, DWORD address);

/************************************************************************
* Function: WORD SST39LF400ReadWord(DWORD address)
*                                                                       
* Overview: this function reads a word from address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
WORD SST39LF400ReadWord(DWORD address);

/************************************************************************
* Function: BYTE SST39LF400WriteArray(DWORD address, BYTE *pData, WORD nCount)
*                                                                       
* Overview: this function writes data array at the address specified
*                                                                       
* Input: flash memory address, pointer to the data array, data number
*                                                                       
* Output: return 1 if the operation was successfull
*
* Notes: none
*                                                                       
************************************************************************/
BYTE SST39LF400WriteArray(DWORD address, BYTE *pData, WORD nCount);

/************************************************************************
* Function: void SST39LF400ReadArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function reads data array from the address specified
*                                                                       
* Input: flash memory address, pointer to the buffer receiving data, data number
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST39LF400ReadArray(DWORD address, BYTE *pData, WORD nCount);

/************************************************************************
* Function: void SST39LF400WaitProgram()
*                                                                       
* Overview: this function waits for program/erase operation completion
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST39LF400WaitProgram(void);

/************************************************************************
* Function: void SST39LF400ChipErase(void)                                 
*
* Overview: erase whole chip
*
* Input: none
*            
* Output: none
*
************************************************************************/
void SST39LF400ChipErase(void);

/************************************************************************
* Function: void SST39LF400SectorErase(DWORD address)                                 
*
* Overview: This function erases 2K Word section defined by address
*
* Input: address - The address location of the sector to be erased.
*				   The address is decided by Address[17:11] address lines.
*            
* Output: none
*
************************************************************************/
void SST39LF400SectorErase(DWORD address);

/************************************************************************
* Function: void SST39LF400CheckID(void *pID)  
*
* Overview: Reads the product ID 
*
* Input: none
*            
* Output: none
*
************************************************************************/
WORD SST39LF400CheckID();

#endif //_SST39LF400_H
