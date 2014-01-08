/********************************************************************
* FileName:		Console.c
* Dependencies: Console.h
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16, PIC18 Explorer
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute 
* Software only when embedded on a Microchip microcontroller or digital 
* signal controller and used with a Microchip radio frequency transceiver, 
* which are integrated into your product or third party product (pursuant 
* to the terms in the accompanying license agreement).  
*
* You should refer to the license agreement accompanying this Software for 
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
*********************************************************************
* File Description:
*
*   This file configures and provides the function for using the
*   UART to transmit data over RS232 to the computer.
*
* Change History:
*  Rev   Date         Author    Description
*  0.1   11/09/2006   yfy       Initial revision
*  1.0   01/09/2007   yfy       Initial release
*  2.0   4/24/2009    yfy       Modified for MiApp interface
*  2.1   6/20/2009    yfy       Add LCD support
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

/************************ HEADERS **********************************/
#include "WirelessProtocols/Console.h"
#include "SystemProfile.h"
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"

#if defined(ENABLE_CONSOLE)
#if defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__) || defined(__PIC32MX__)

/************************ VARIABLES ********************************/
ROM unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/************************ FUNCTIONS ********************************/

/*********************************************************************
* Function:         void ConsoleInit(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    UART2 is configured
*
* Overview:		    This function will configure the UART for use at 
*                   in 8 bits, 1 stop, no flowcontrol mode
*
* Note:			    None
********************************************************************/
void ConsoleInit(void)
{
    #ifdef __PIC32MX__
        OpenUART2(UART_EN, (1 << 12)|UART_TX_ENABLE, (CLOCK_FREQ/(1<<mOSCGetPBDIV())/16)/BAUD_RATE-1); 
    #else    
        U2BRG   = (CLOCK_FREQ/2/16)/BAUD_RATE-1;
        IFS1bits.U2RXIF = 0;
        U2STA  = 0;
        U2MODE = 0b0000000010000000;
        U2MODEbits.UARTEN = 1;
        U2STAbits.UTXEN = 1;
    #endif
}

/*********************************************************************
* Function:         void ConsolePutROMString(ROM char* str)
*
* PreCondition:     none
*
* Input:		    str - ROM string that needs to be printed
*
* Output:		    none
*
* Side Effects:	    str is printed to the console
*
* Overview:		    This function will print the inputed ROM string
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void ConsolePutROMString(ROM char* str)
{
    BYTE c;

    while( (c = *str++) )
        ConsolePut(c);
}

/*********************************************************************
* Function:         void ConsolePut(BYTE c)
*
* PreCondition:     none
*
* Input:		    c - character to be printed
*
* Output:		    none
*
* Side Effects:	    c is printed to the console
*
* Overview:		    This function will print the inputed character
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void ConsolePut(BYTE c)
{
    while(U2STAbits.TRMT == 0);
    U2TXREG = c;
}

/*********************************************************************
* Function:         BYTE ConsoleGet(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    one byte received by UART
*
* Side Effects:	    none
*
* Overview:		    This function will receive one byte from UART
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
BYTE ConsoleGet(void)
{
	char Temp;

    while(IFS1bits.U2RXIF == 0);

	Temp = U2RXREG;
    IFS1bits.U2RXIF = 0;
	return Temp;
}
	


/*********************************************************************
* Function:         void PrintChar(BYTE toPrint)
*
* PreCondition:     none
*
* Input:		    toPrint - character to be printed
*
* Output:		    none
*
* Side Effects:	    toPrint is printed to the console
*
* Overview:		    This function will print the inputed BYTE to 
*                   the console in hexidecimal form
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void PrintChar(BYTE toPrint)
{
    BYTE PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint>>4)&0x0F;
    ConsolePut(CharacterArray[toPrint]);
    toPrint = (PRINT_VAR)&0x0F;
    ConsolePut(CharacterArray[toPrint]);
    return;
}

/*********************************************************************
* Function:         void PrintDec(BYTE toPrint)
*
* PreCondition:     none
*
* Input:		    toPrint - character to be printed. Range is 0-99
*
* Output:		    none
*
* Side Effects:	    toPrint is printed to the console in decimal
*                   
*
* Overview:		    This function will print the inputed BYTE to 
*                   the console in decimal form
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void PrintDec(BYTE toPrint)
{
    ConsolePut(CharacterArray[toPrint/10]);
    ConsolePut(CharacterArray[toPrint%10]);
}

#elif defined(__18CXX)

/************************ VARIABLES ********************************/
ROM unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    
/************************ DEFINITIONS ******************************/
#define USART_USE_BRGH_HIGH
#if defined(USART_USE_BRGH_LOW)
    #define SPBRG_VAL   ( ((CLOCK_FREQ/BAUD_RATE)/64) - 1)
#else
    #define SPBRG_VAL   ( ((CLOCK_FREQ/BAUD_RATE)/16) - 1)
#endif

#if SPBRG_VAL > 255
    #error "Calculated SPBRG value is out of range for currnet CLOCK_FREQ."
#endif

/************************ FUNCTIONS ********************************/
    
/*********************************************************************
* Function:         void ConsoleInit(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    UART2 is configured
*
* Overview:		    This function will configure the UART for use at 
*                   in 8 bits, 1 stop, no flowcontrol mode
*
* Note:			    None
********************************************************************/
void ConsoleInit(void)
{
    #if defined(EIGHT_BIT_WIRELESS_BOARD)
        TRISDbits.TRISD2 = 1;
        TRISCbits.TRISC6 = 0;
        #if defined(USART_USE_BRGH_HIGH)
            TXSTA2 = 0x24;
        #else
            TXSTA2 = 0x20;
        #endif
        
        RCSTA2 = 0x90;
        SPBRG2 = SPBRG_VAL;
    #else   
        TRISCbits.TRISC7 = 1;
        TRISCbits.TRISC6 = 0;
        #if defined(USART_USE_BRGH_HIGH)
            TXSTA = 0x24;
        #else
            TXSTA = 0x20;
        #endif
    
        RCSTA = 0x90; // 0b10010000;
        SPBRG = SPBRG_VAL;
    #endif
}

/*********************************************************************
* Function:         void ConsolePutROMString(ROM char* str)
*
* PreCondition:     none
*
* Input:		    str - ROM string that needs to be printed
*
* Output:		    none
*
* Side Effects:	    str is printed to the console
*
* Overview:		    This function will print the inputed ROM string
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void ConsolePutROMString(ROM char* str)
{
    BYTE c;

    while( c = *str++ )
        ConsolePut(c);
}

/*********************************************************************
* Function:         void ConsolePut(BYTE c)
*
* PreCondition:     none
*
* Input:		    c - character to be printed
*
* Output:		    none
*
* Side Effects:	    c is printed to the console
*
* Overview:		    This function will print the inputed character
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void ConsolePut(BYTE c)
{
    while( !ConsoleIsPutReady() );
    #if defined(EIGHT_BIT_WIRELESS_BOARD)
        TXREG2 = c;
    #else
        TXREG = c;
    #endif
}


/*********************************************************************
* Function:         BYTE ConsoleGet(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    one byte received by UART
*
* Side Effects:	    none
*
* Overview:		    This function will receive one byte from UART
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
BYTE ConsoleGet(void)
{
    #if defined(EIGHT_BIT_WIRELESS_BOARD)
        // Clear overrun error if it has occured
        // New bytes cannot be received if the error occurs and isn't cleared
        if(RCSTA2bits.OERR)
        {
            RCSTA2bits.CREN = 0;   // Disable UART receiver
            RCSTA2bits.CREN = 1;   // Enable UART receiver
        }

        return RCREG2;

    #else
        // Clear overrun error if it has occured
        // New bytes cannot be received if the error occurs and isn't cleared
        if(RCSTAbits.OERR)
        {
            RCSTAbits.CREN = 0;   // Disable UART receiver
            RCSTAbits.CREN = 1;   // Enable UART receiver
        }

        return RCREG;
    #endif
}


/*********************************************************************
* Function:         void PrintChar(BYTE toPrint)
*
* PreCondition:     none
*
* Input:		    toPrint - character to be printed
*
* Output:		    none
*
* Side Effects:	    toPrint is printed to the console
*
* Overview:		    This function will print the inputed BYTE to 
*                   the console in hexidecimal form
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void PrintChar(BYTE toPrint)
{
    BYTE PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint>>4)&0x0F;
    ConsolePut(CharacterArray[toPrint]);
    toPrint = (PRINT_VAR)&0x0F;
    ConsolePut(CharacterArray[toPrint]);
    return;
}

/*********************************************************************
* Function:         void PrintDec(BYTE toPrint)
*
* PreCondition:     none
*
* Input:		    toPrint - character to be printed. Range is 0-99
*
* Output:		    none
*
* Side Effects:	    toPrint is printed to the console in decimal
*                   
*
* Overview:		    This function will print the inputed BYTE to 
*                   the console in decimal form
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void PrintDec(BYTE toPrint)
{
    if( toPrint >= 100 )
        ConsolePut(CharacterArray[toPrint/100]);
    if( toPrint >= 10 )
        ConsolePut(CharacterArray[(toPrint%100)/10]);
    ConsolePut(CharacterArray[toPrint%10]);
}
#else
    #error Unknown processor.  See Compiler.h
#endif

#endif  //ENABLE_CONSOLE
