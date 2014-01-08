/*****************************************************************************
 * Simple UART driver
 *****************************************************************************
 * FileName:        UART.h
 * Dependencies:    
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		06/07/07	...
 * Anton Alkhimenok		02/05/08	PIC32 support is added
 *****************************************************************************/
#ifndef __UART_H__
    #define __UART_H__

    #define BAUDRATE    115200ul    //38400

/*********************************************************************
* Function: void UARTInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes UART 
*
* Note: none
*
********************************************************************/
extern void UARTInit(void);

/*********************************************************************
* Function: void UARTPutChar(BYTE ch)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: puts character
*
* Note: none
*
********************************************************************/
extern void UARTPutChar(BYTE ch);

/*********************************************************************
* Function: void UARTPutByte(BYTE hex)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: puts HEX code of byte
*
* Note: none
*
********************************************************************/
extern void UARTPutByte(BYTE byte);

/*********************************************************************
* Function: void UARTPutWord(WORD_VAL word)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: puts HEX code of word
*
* Note: none
*
********************************************************************/
extern void UARTPutWord(WORD word);

/*********************************************************************
* Function: void UARTPutDWord(DWORD_VAL dword)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: puts HEX code of double word
*
* Note: none
*
********************************************************************/
extern void UARTPutDWord(DWORD dword);

/*********************************************************************
* Function: void UARTPutString(char* str)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: puts string
*
* Note: none
*
********************************************************************/
extern void UARTPutString(char *str);

/*********************************************************************
* Function: BYTE UARTGetChar()
*
* PreCondition: none
*
* Input: none
*
* Output: last character received
*
* Side Effects: none
*
* Overview: returns last character received
*
* Note: none
*
********************************************************************/
extern BYTE UARTGetChar(void);

/*********************************************************************
* Function: BYTE UARTWaitChar()
*
* PreCondition: none
*
* Input: none
*
* Output: character received
*
* Side Effects: none
*
* Overview: wait for character
*
* Note: none
*
********************************************************************/
extern BYTE UARTWaitChar(void);

/*********************************************************************
* Macros: UARTIsReceived()
*
* PreCondition: none
*
* Input: none
*
* Output: zero if character is not received
*
* Side Effects: none
*
* Overview: checks if data is available
*
* Note: none
*
********************************************************************/
    #define UARTIsDA()  U2STAbits.URXDA

/*********************************************************************
* Function: BYTE Char2Hex(BYTE ch)
*
* PreCondition: none
*
* Input: ASCII to be converted
*
* Output: number
*
* Side Effects: none
*
* Overview: converts ASCII coded digit into number
*
* Note: none
*
********************************************************************/
extern BYTE Char2Hex(BYTE ch);

/*********************************************************************
* Function: BYTE Hex2Char(BYTE hex)
*
* PreCondition: none
*
* Input: number
*
* Output: ASCII code
*
* Side Effects: none
*
* Overview: converts low nibble into ASCII coded digit
*
* Note: none
*
********************************************************************/
extern BYTE Hex2Char(BYTE hex);
#endif // __UART_H__
