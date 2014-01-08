/*****************************************************************************
 *
 * Basic access to parallel FLASH SST39VF400.
 *
 *****************************************************************************
 * FileName:        SST39VF400.c
 * Dependencies:    SST39VF400.h
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
#include "MainDemo.h"


#define  CS2_BASE_ADDRESS   0x00030000ul 
#define  SST39_FLASH_SIZE	0x00040000ul


/************************************************************************
*   LOCAL PROTOTYPES
************************************************************************/
void lWrite16(DWORD address, WORD data);
WORD lRead16(DWORD address);


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
void SST39Init(void)
{
	PMCON1bits.PMPEN = 0;

	ANSDbits.ANSD7 = 0;   // PMD15
	ANSDbits.ANSD6 = 0;   // PMD14
	ANSFbits.ANSF0 = 0;   // PMD11

	ANSBbits.ANSB13 = 0;  // PMA10
	ANSBbits.ANSB12 = 0;  // PMA11
	ANSBbits.ANSB11 = 0;  // PMA12
	ANSBbits.ANSB10 = 0;  // PMA13

	PMCON1bits.ADRMUX = 0;	    // address is not multiplexed
	PMCON1bits.MODE = 3;        // master mode
	PMCON1bits.CSF = 0;         // PMCS1 pin used for chip select 1, PMCS2 pin used for chip select 2
	PMCON1bits.ALP = 1;         // set address latch strobes to high active level (for sn74lvc16373)
	PMCON1bits.ALMODE = 0;      // "smart" address strobes are not used
	PMCON1bits.BUSKEEP = 0;     // bus keeper is not used
	
	PMCS1BSbits.BASE = 0x02;    // CS1 start address , don't care , not used
	PMCS2BSbits.BASE = (CS2_BASE_ADDRESS>>16)&0x00ff; // set CS1 end address and CS2 start address
	PMCON2bits.RADDR = 0xff;    // set CS2 end address

	PMCON2bits.MSTSEL = 0;

	PMCON3bits.PTWREN = 1;      // enable write strobe port
	PMCON3bits.PTRDEN = 1;      // enable read strobe port
	PMCON3bits.PTBE0EN = 1;     // enable byte enable port
	PMCON3bits.PTBE1EN = 1;     // enable byte enable port
	PMCON3bits.AWAITM = 0;      // set address latch pulses width to 1/2 Tcy
	PMCON3bits.AWAITE = 0;      // set address hold time to 1/4 Tcy

	PMCON4 = 0xFFFF;            // PMA0 - PMA15 address lines are enabled
	PMCON3 |= 0x0007;           // PMA16 address line is enabled

	PMCS1CFbits.CSDIS = 1;       // disable CS1 functionality  
	PMCS2CFbits.CSDIS = 1;       // disable CS2 functionality  

	PMCS2CFbits.CSDIS = 0;      // enable CS
	PMCS2CFbits.CSP = 0;        // CS active low (for 61/62WV51216ALL)
	PMCS2CFbits.CSPTEN = 1;     // enable CS port
	PMCS2CFbits.BEP = 0;        // byte enable active low (for 61/62WV51216ALL)
	PMCS2CFbits.WRSP = 0;       // write strobe active low (for 61/62WV51216ALL)
	PMCS2CFbits.RDSP = 0;       // read strobe active low (for 61/62WV51216ALL)
	PMCS2CFbits.SM = 0;         // read and write strobes on separate lines 
	PMCS2CFbits.PTSZ = 2;       // data bus width is 16-bit 

	PMCS2MDbits.ACKM = 0;        // PMACK is not used
	PMCS2MDbits.DWAITB = 0;      // access time 1 Tcy
	PMCS2MDbits.DWAITM = 4;
	PMCS2MDbits.DWAITE = 0;
	PMCS2MDbits.AMWAIT = 0;

	PMCON1bits.PMPEN = 1;
}

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
BYTE SST39WriteWord(DWORD address, WORD data)
{
    lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(0x000005555,0x00a0);
    lWrite16(address,data);

    SST39WaitProgram();

    if(SST39ReadWord(address) == data)
        if(SST39ReadWord(address) == data)
            return (1);

    return (0); // failed
}

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
WORD SST39ReadWord(DWORD address)
{
WORD temp;
	
	temp = lRead16(address);

    return (temp);
}

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
BYTE SST39WriteArray(DWORD address, WORD *pData, WORD nCount)
{
    WORD    counter;
    WORD    *pD;
    DWORD   addr;

    pD = pData;
    addr = address;

    // write
    for(counter = 0; counter < nCount; counter++)
    {
        while(0 == SST39WriteWord(addr, *pD));
        addr++;
        pD++;
    }

    pD = pData;
    addr = address;

    // verify
    for(counter = 0; counter < nCount; counter++)
    {
        if(*pD++ != SST39ReadWord(addr++))
            return (0);
    }

    return (1);
}

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
void SST39ReadArray(DWORD address, WORD *pData, WORD nCount)
{
    WORD    counter;
    WORD    temp;

    for(counter = 0; counter < nCount; counter++)
    {
		temp = lRead16(address);
		*pData++ = temp;
    }

}

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
void SST39WaitProgram(void)
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
* Function: void SST39ChipErase(void)                                 
*
* Overview: erase whole chip
*
* Input: none
*            
* Output: none
*
************************************************************************/
void SST39ChipErase(void)
{
    lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(0x000005555,0x0080);
    lWrite16(0x000005555,0x00aa);
    lWrite16(0x000002aaa,0x0055);
    lWrite16(0x000005555,0x0010);

    SST39WaitProgram();
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
WORD data;
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
