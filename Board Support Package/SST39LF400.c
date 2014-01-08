/*****************************************************************************
 *
 * Basic access to parallel FLASH SST39LF400.
 *
 *****************************************************************************
 * FileName:        SST39LF400.c
 * Dependencies:    SST39LF400.h
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 4/12/2010	Original, ported from SST39LF400A
 * 8/11/2010	Removed dependency on Graphics file. 
 * 1/14/2011    Implemented read and write function using EDS access.
 *              Swapped SST39LF400WriteWord() parameters for consistency
 *              with other drivers.
 * 5/25/2011    Modified WriteArray and ReadArray functions to match
 *              common interface for other external flash drivers.
 * 10/9/2012    Removed EDS code in lRead16() and lWrite16() so we 
 *              do not need to set -mlarge-arrays compiler settings
 *              for the file.
 ********************************************************************/
#include "SST39LF400.h"
#include <stdint.h>

#if defined (USE_SST39LF400)

#include "Graphics/gfxepmp.h"

#define  	CS2_BASE_ADDRESS   		GFX_EPMP_CS2_BASE_ADDRESS  
#define 	SST39LF400_FLASH_SIZE	0x00400000ul

// union for data 
typedef union
{
    struct
    {
        uint8_t uint16Byte[2];
    };

    uint16_t uint16Word;
} SST39_UINT16;

// dummy declaration for EPMP CS2 start address 
__eds__ WORD __attribute__((eds, noload, address(CS2_BASE_ADDRESS))) EPMPCS2Start;

/************************************************************************
*   LOCAL PROTOTYPES
************************************************************************/
void lWrite16(DWORD address, WORD data);
WORD lRead16(DWORD address);


/************************************************************************
* Function: SST39LF400Init()                                                 
*                                                                       
* Overview: This function initializes IOs and Parallel Master Port. 
*                                                                       
* Input: Address of the array for the current EPMP SFR register
*		 contents. 
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST39LF400Init()
{
    
    DriverInterfaceInit();

}

/************************************************************************
* Function: BYTE SST39LV400WriteWord(DWORD address, WORD data)
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
BYTE SST39LF400WriteWord(WORD data, DWORD address)
{
	WORD temp;
	
    lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(0x000005555,0x00a0);
    lWrite16(address,data);
    
    SST39LF400WaitProgram();

    if((temp = SST39LF400ReadWord(address)) == data)
        if((temp = SST39LF400ReadWord(address)) == data)
            return (1);

    return (0); // failed
}

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
WORD SST39LF400ReadWord(DWORD address)
{
WORD temp;
	
	temp = lRead16(address);

    return (temp);
}

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
BYTE SST39LF400WriteArray(DWORD address, BYTE *pData, WORD nCount)
{
    WORD            counter, maxCount;
    SST39_UINT16    writeData;
    BYTE            *pD;
    DWORD           addr;

    // Note that shifting of the address and count is performed
    // here since the incoming data is addressed on a byte location 
    // and the length (nCount) is the number of bytes
    pD = pData;
    addr = address >> 1;
    maxCount = nCount >> 1;

    // write
    for(counter = 0; counter < maxCount; counter++)
    {
        // this takes care of cases when the incoming data is 
        // not aligned to 16 bit words
        writeData.uint16Byte[0] = *pD++;
        writeData.uint16Byte[1] = *pD++;

        while(0 == SST39LF400WriteWord(writeData.uint16Word, addr));
        addr++;
    }

    pD = pData;
    addr = address >> 1;

    // verify
    for(counter = 0; counter < maxCount; counter++)
    {   
        writeData.uint16Byte[0] = *pD++;
        writeData.uint16Byte[1] = *pD++;
        
        if(writeData.uint16Word != SST39LF400ReadWord(addr++))
            return (0);
    }

    return (1);
}

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
void SST39LF400ReadArray(DWORD address, BYTE *pData, WORD nCount)
{
    WORD            counter, maxCount;
    BYTE            *pD;
    DWORD           addr;
    SST39_UINT16    writeData;

    // Note that shifting of the address and count is performed
    // here since the incoming data is addressed on a byte location 
    // and the length (nCount) is the number of bytes

    addr = address >> 1;
    pD   = pData;
    maxCount = nCount >> 1;

    for(counter = 0; counter < maxCount; counter++)
    {
		writeData.uint16Word = lRead16(addr);

        // this takes care of cases where the pData is not 16-bit aligned
        *pD++ = writeData.uint16Byte[0]; 
        *pD++ = writeData.uint16Byte[1];

        addr++;
    }

}

/************************************************************************
* Function: void SST39LF400WaitProgram()
*                                                                       
* Overview: this function waits for program/erase operation completion 
*			using the Toggle Bit wait option.
*                                                                       
* Input: none                                                          
*                                                                       
* Output: none
*                                                                       
************************************************************************/
void SST39LF400WaitProgram(void)
{
WORD temp;
WORD tempPrev;

	temp = lRead16(0);
	temp &= 0x0040;
	tempPrev = ~temp;
	tempPrev &= 0x0040;
    
    while(temp != tempPrev)
    {
		tempPrev = temp;
		temp = lRead16(0);
		temp &= 0x0040;
    }   // end of while

}

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
void SST39LF400ChipErase(void)
{
    lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(0x000005555,0x0080);
    lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(0x000005555,0x0010);

    SST39LF400WaitProgram();
}

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
void SST39LF400SectorErase(DWORD address)
{
    lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(0x000005555,0x0080);
    lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(address,0x0030);

    SST39LF400WaitProgram();
}

/************************************************************************
* Function: WORD SST39LF400CheckID()  
*
* Overview: Reads the product ID 
*
* Input: none
*            
* Output: Returns 1 if the expected product ID is read correctly
*         else it returns 0.
*
************************************************************************/
WORD SST39LF400CheckID()
{
	WORD testArray[2] = {0,0};
	
   	lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(0x000005555,0x0090);
    testArray[0] = lRead16 (0x000000000);
    testArray[1] = lRead16 (0x000000001);

    // exit the query mode 
    lWrite16(0x000005555,0x00F0);
    
   	if ((testArray[0] == 0x00BF) && (testArray[1] == 0x2780))
   		return 1;
	else
		return 0;   		
}


/************************************************************************
************************************************************************/
void lWrite16(DWORD address, WORD data)
{
    WORD pointer;
    WORD temp;  
    
    address <<= 1;
    address += (DWORD)CS2_BASE_ADDRESS;
    pointer = ((DWORD_VAL)address).w[0];
    pointer |= 0x8000;
    address <<= 1;
    temp = DSWPAG;
    DSWPAG = ((DWORD_VAL)address).v[2];
    *((WORD*)pointer) = data;

    while(PMCON2bits.BUSY);

    DSWPAG = temp;

}

/************************************************************************
************************************************************************/
WORD lRead16(DWORD address)
{

    WORD pointer;
    volatile WORD data;
    WORD temp;
	
    address <<= 1;
    address += CS2_BASE_ADDRESS;
    pointer = ((DWORD_VAL)address).w[0];
    pointer |= 0x8000;
    address <<= 1;
    temp = DSRPAG;
    DSRPAG = ((DWORD_VAL)address).v[2];
    data = *((WORD*)pointer);
    
    while(PMCON2bits.BUSY);

    data = PMDIN1;
    DSRPAG = temp;

    return data;

}

#endif //USE_SST39LF400
