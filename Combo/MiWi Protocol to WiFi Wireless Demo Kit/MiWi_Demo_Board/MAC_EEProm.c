/********************************************************************
/********************************************************************
* FileName:		MAC_EEprom.c
* Dependencies:    
* Processor:	PIC18	
* Complier:     Microchip C18 v3.04 or higher
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
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
*  This file reads the 25AA02E48 MAC EEProm
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/02/2011    ccs       Initial revision
********************************************************************/
#include "MAC_EEProm.h"
#include "SystemProfile.h"
#include "WirelessProtocols/MSPI.h"


#define SPI_WRT_STATUS  0x01
#define SPI_WRITE       0x02
#define SPI_READ        0x03
#define SPI_DIS_WRT     0x04
#define SPI_RD_STATUS   0x05
#define SPI_EN_WRT      0x06

#define EEPROM_MAC_ADDR 0xFA

extern BYTE myLongAddress[];


/*********************************************************************
* Function:         void EEPROMRead(BYTE *dest, BYTE addr, BYTE count)
*
* PreCondition:     none
*
* Input:		    BYTE *dest  - Destination buffer.
*                   BYTE addr   - Address to start reading from.
*                   BYTE count  - Number of bytes to read.
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Following routine reads bytes from the EEProm and puts
*                   them in a buffer.
*                    
*
* Note:			    
**********************************************************************/   
void EEPROMRead(BYTE *dest, BYTE addr, BYTE count)
{
    EE_nCS = 0;
    SPIPut2(SPI_READ);
    SPIPut2(addr);
    while( count )
    {
        *dest++ = SPIGet2();
        count--;
    }
    EE_nCS = 1;
} 


/*********************************************************************
* Function:         void Read_MAC_Address(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Following routine reads the MAC Address form the
*                   EEProm and loads it into myLongAddress[].
*                    
*
* Note:			    
**********************************************************************/  
void Read_MAC_Address(void)
{
    BYTE i;
    
	if( MY_ADDRESS_LENGTH > 6 )
	{
	    for(i = 0; i < 3; i++)
	    {
	        EEPROMRead(&(myLongAddress[MY_ADDRESS_LENGTH-1-i]), EEPROM_MAC_ADDR+i, 1);
	    }
	    myLongAddress[4] = 0xFF;
	    if( MY_ADDRESS_LENGTH > 7 )
	    {
	        myLongAddress[3] = 0xFE;
	    }
	    for(i = 0; i < 3; i++)
	    {
	        EEPROMRead(&(myLongAddress[2-i]), EEPROM_MAC_ADDR+3+i, 1);
	    }
	}
	else
	{
	    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
	    {
	        EEPROMRead(&(myLongAddress[MY_ADDRESS_LENGTH-1-i]), EEPROM_MAC_ADDR+i, 1);
	    }   
	}
}



    	