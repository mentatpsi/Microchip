/*****************************************************************************
 * GRAPHICS PICTail PLUS BOARD MEMORY PROGRAMMER MODULE
 * This program receives Intel HEX file from UART and programs
 * flash memory installed on Graphics PICTail board.
 *
 * Please read ReadMe file for details.
 *
 *****************************************************************************
 * FileName:        ProgramFlash.c
 * Dependencies:    ProgramFlash.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
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
 * 08/04/10    Creation...
 *****************************************************************************/
#include "ProgramFlash.h"

// Local functions prototypes
static BYTE	ConvertRecord2Bin (void);
static BYTE ProcessRecord(void);

// Acknowledgement
#define ACK 'Y'
#define NAK 'N'

// Main state mashine
#define START   0
#define DATA    1

// Current memory address
static DWORD_VAL   MemAddress;
// Buffer for incoming data
static BYTE        buffer[1024];
static BYTE        state;

///////////////////////////////////// MAIN ////////////////////////////////////
int ProgramFlash(void)
{
    BYTE    *pointer = NULL;
    BYTE    byte;

 	#if defined (PIC24FJ256DA210_DEV_BOARD)
    // Make all the analog pins digital.
	ANSA = 0x0000;
    ANSB = 0x0000;		
    ANSC = 0x0000;		
    ANSD = 0x0000;
    ANSE = 0x0000;
    ANSF = 0x0000;
    ANSG = 0x0000;		
    
	#endif

   	FLASHInit();
    UARTInit();
   
   	// erase the flash memory
	ChipErase();
	
    state = START;
	
    while(1)
    {
        byte = UARTWaitChar();

        switch(state)
        {
            case START:

                // start of record is detected
                if(byte == ':')
                {
                    pointer = buffer;
                    state = DATA;
                }

                /*
                if(byte == '\r')
                {
                    UARTPutChar(NAK);
                }
*/
                break;

            case DATA:

                // end of record
                if(byte == '\r')
                {

                    // end of string
                    *pointer++ = 0;
                    *pointer++ = 0;

                    state = START;

                    if(!ConvertRecord2Bin())
                    {

                        // checksum is wrong
                        UARTPutChar(NAK);
                        break;
                    }

                    if(!ProcessRecord())
                    {

                        // programming error
                        UARTPutChar(NAK);
                        UARTPutChar('P');
                        CreateError("Write Data is not read correctly");
                        break;
                    }

                    // record is processed successfully
                    UARTPutChar(ACK);
                    break;
                }

                *pointer++ = byte;
                break;

            default:
                break;
        }   // end of switch
    }       // end of while

    return (0);
}

/* */
BYTE ConvertRecord2Bin(void)
{
    BYTE    *pIn;
    BYTE    *pOut;
    WORD    checksum = 0;
    BYTE    byte;

    pIn = pOut = buffer;

    while(*pIn)
    {

        // Get first nibble
        byte = Char2Hex(*pIn++);
        byte <<= 4;

        // Get second nibble
        byte |= Char2Hex(*pIn++);
        checksum += byte;
        *pOut++ = byte;
    }

    checksum &= 0x00ff;

    // last byte was a transmitted checksum, compare with calculated
    if(checksum)
        return (0); // error
    return (1);     // good
}

typedef struct _RECORD_HEADER_
{
    BYTE    reclen;
    BYTE    msbOffset;
    BYTE    lsbOffset;
    BYTE    rectype;
} RECORD_HEADER;

#define REC_DATA        0
#define REC_EOF         1
#define REC_UPPERADDR   4
#define REC_STARTADDR   5

/* */

BYTE ProcessRecord(void)
{
	
    BYTE            *pData;
    RECORD_HEADER   *pRecord;
	DWORD_VAL   	Address;

    pRecord = (RECORD_HEADER *)buffer;
    pData = buffer + sizeof(RECORD_HEADER);

    switch(pRecord->rectype)
    {

        // data record
        case REC_DATA:

            // set lower 16 bit of the linear address
            MemAddress.v[1] = pRecord->msbOffset;
            MemAddress.v[0] = pRecord->lsbOffset;

                    
			#if defined (USE_SST39LF400)
            Address.Val = (MemAddress.Val >> 1);
            pRecord->reclen = pRecord->reclen >> 1;
            #else
            Address.Val = MemAddress.Val;
            #endif

            return (WriteArray(Address.Val, pData, pRecord->reclen));


        // end of file record
        case REC_EOF:
            return (1);

        // extended linear address record
        case REC_UPPERADDR:

            // set upper 16 bit of the linear address
            MemAddress.v[3] = *pData++;
            MemAddress.v[2] = *pData;
            return (1);

        // start linear address record
        case REC_STARTADDR:

            // set full address
            MemAddress.v[3] = *pData++;
            MemAddress.v[2] = *pData++;
            MemAddress.v[1] = *pData++;
            MemAddress.v[0] = *pData;
            return (1);

        default:
            return (1);
    }   // end of switch

    return (0);
}
