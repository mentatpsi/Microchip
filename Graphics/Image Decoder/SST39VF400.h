/*****************************************************************************
 *
 * Basic access to parallel FLASH SST39VF400
 *
 *****************************************************************************
 * FileName:        SST39VF400.h
 * Dependencies:    none
 * Processor:       PIC24F
 * Compiler:       	MPLAB C30 V3.00
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		08/27/09	...
 *****************************************************************************/
#ifndef _SST39_H
#define _SST39_H

 /************************************************************************
* Function: SST39Init                                                  
*                                                                       
* Overview: this function initializes IOs and PMP
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST39Init(void);

/************************************************************************
* Function: BYTE SST39WriteWord(DWORD address, WORD data)
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
BYTE SST39WriteWord(DWORD address, WORD data);

/************************************************************************
* Function: WORD SST39ReadWord(DWORD address)
*                                                                       
* Overview: this function reads a word from address specified         
*                                                                       
* Input: address                                                     
*                                                                       
* Output: data read
*                                                                       
************************************************************************/
WORD SST39ReadWord(DWORD address);

/************************************************************************
* Function: BYTE SST39WriteArray(DWORD address, WORD* pData, nCount)
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
BYTE SST39WriteArray(DWORD address, WORD *pData, WORD nCount);

/************************************************************************
* Function: void SST39ReadArray(DWORD address, WORD* pData, nCount)
*                                                                       
* Overview: this function reads data array from the address specified
*                                                                       
* Input: flash memory address, pointer to the buffer receiving data, data number
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST39ReadArray(DWORD address, WORD *pData, WORD nCount);

/************************************************************************
* Function: void SST39WaitProgram()
*                                                                       
* Overview: this function waits for program/erase operation completion
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST39WaitProgram(void);

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
void SST39ChipErase(void);

#endif //_SST39_H
