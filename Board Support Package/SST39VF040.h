/*****************************************************************************
 *
 * Basic access to parallel FLASH SST39VF040
 *
 *****************************************************************************
 * FileName:        SST39VF040.h
 * Dependencies:    None
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
 * Date			Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/14/08		...
 *****************************************************************************/
#ifndef _SST39_H
    #define _SST39_H

#include "GenericTypeDefs.h"

#ifdef __PIC32MX
	#define PMDIN1  PMDIN
#endif

/************************************************************************
* Structure SST39STATREG and union SST39STATUS                            
*                                                                       
* Overview: provide bits and byte access to erase/write status value      
*                                                                       
************************************************************************/
typedef struct _SST39STATREG_
{
    BYTE    RESERVED : 5;
    BYTE    FAILED : 1;
    BYTE    TOGGLE : 1;
} SST39STATREG;

typedef union _SST39STATUS_
{
    SST39STATREG    Bits;
    BYTE            Val;
} SST39STATUS;

/************************************************************************
* Function: SST39Init()
*                                                                       
* Overview: this function sets PMP and IOs
*                                                                       
* Input: none
*                                                                       
* Output: none  
*                                                                       
************************************************************************/
void    SST39Init(void);

/************************************************************************
* Function: void SST39WaitProgram()                                          
*                                                                       
* Overview: this function waits for program/erase operation completion
*                                                                       
* Input: none                                                          
*                                                                       
* Output: 1 if it was successfull 0 - if not
*                                                                       
************************************************************************/
void    SST39WaitProgram(void);

/************************************************************************
* Function: BYTE SST39WriteByte(DWORD address, BYTE data)
*                                                                       
* Overview: this function writes one byte
*                                                                       
* Input: flash memory address and byte to be written
*                                                                       
* Output: non-zero if successful
*
* Notes: none
*                                                                       
************************************************************************/
BYTE    SST39WriteByte(DWORD address, BYTE data);

/************************************************************************
* Function: BYTE SST39WriteArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function writes data array at the address specified
*                                                                       
* Input: flash memory address, pointer to the data array, data number
*                                                                       
* Output: return 1 if the operation was successfull
*                                                                       
************************************************************************/
BYTE    SST39WriteArray(DWORD address, BYTE *pData, WORD nCount);

/************************************************************************
* Function: void SST39ReadArray(DWORD address, BYTE* pData, nCount)
*                                                                       
* Overview: this function reads data array from the address specified
*                                                                       
* Input: flash memory address, pointer to the buffer receiving data, data number
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void    SST39ReadArray(DWORD address, BYTE *pData, WORD nCount);

/************************************************************************
* Function: BYTE SST39ReadByte(DWORD address)       
*                                                                       
* Overview: this function reads a byte from the address specified
*                                                                       
* Input: address          
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
BYTE    SST39ReadByte(DWORD address);

/************************************************************************
* Function: WORD SST39ReadWord(DWORD address)             
*                                                                       
* Overview: this function reads a 16-bit word from the address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
WORD    SST39ReadWord(DWORD address);

/************************************************************************
* Function: void SST39ChipErase(void)                                 
*
* Overview: erase whole chip
*
* Input: none
*            
* Output: none
*
************************************************************************/
void    SST39ChipErase(void);
#endif //_SST39_H
