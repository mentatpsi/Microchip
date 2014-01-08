/********************************************************************
* FileName:		LCD_ST7032.c
* Dependencies:    
* Processor:	PIC18	
* Complier:     Microchip C18 v3.04 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
*
* Copyright � 2007-2010 Microchip Technology Inc.  All rights reserved.
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
* SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY 
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
*  This file defines functions used to display information on a display
*  that uses a ST73032 contorller.
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/09/2011    ccs       Initial revision
********************************************************************/

#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "TimeDelay.h"
#include <plib.h>
//#include "WirelessProtocols/MSPI.h"
#include "LCD_ST7032.h"


BYTE LCDText[16*2+1];


void LCD_WriteCmd(BYTE data)
{
    BYTE tmp;

    LCD_CS = 0;
    LCD_RS = 0;
    
    SpiChnPutC(SPI_CHANNEL3, data);
    tmp = SpiChnGetC(SPI_CHANNEL3);

    LCD_CS = 1;
}


void LCD_WriteData(BYTE data)
{
    BYTE tmp;
    
    LCD_CS = 0;
    LCD_RS = 1;

    SpiChnPutC(SPI_CHANNEL3, data);
    tmp = SpiChnGetC(SPI_CHANNEL3);

    LCD_CS = 1;
}


void LCDInit(void)
{
    LCD_CS = 1;
    LCD_RESET = 0;    
    DelayMs(2);
    LCD_RESET = 1;
    DelayMs(20);
    LCD_WriteCmd(LCD_WAKEUP);
    DelayMs(2);
    LCD_WriteCmd(LCD_WAKEUP);
    LCD_WriteCmd(LCD_WAKEUP);
    LCD_WriteCmd(LCD_FUNCTION_SET);
    LCD_WriteCmd(LCD_INT_OSC);
    LCD_WriteCmd(LCD_PWR_CTRL);
    LCD_WriteCmd(LCD_FOLLOWER_CTRL);
    
    LCD_WriteCmd(LCDCMD_CONTRASTSET_LOWBYTE);
    LCD_WriteCmd(LCD_ON);
    LCD_WriteCmd(LCD_ENTRY_MODE);
    LCD_WriteCmd(LCDCMD_CLEARDISPLAY);
    DelayMs(10);
}
   
      
/******************************************************************************
 * Function:        void LCDErase(void)
 *
 * PreCondition:    LCDInit() must have been called once
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Clears LCDText[] and the LCD's internal display buffer
 *
 * Note:            None
 *****************************************************************************/
void LCDErase(void)
{
	// Clear display
	LCD_WriteCmd(LCDCMD_CLEARDISPLAY);;    
	DelayMs(2);

	// Clear local copy
	memset(LCDText, ' ', 32);
}

void LCDBacklightON(void)
{
    LCD_BKLT = 1;
}
    
void LCDBacklightOFF(void)
{
    LCD_BKLT = 0;
}


void LCDUpdate(void)
{
    BYTE i;

    LCD_WriteCmd(LCDCMD_SET_DDRAM_ADDRESS | LCD_FIRSTLINE_ADDRESS);
    
    for(i = 0; i < 16; i++)
    {
        if( LCDText[i] == 0 )
        {
            LCD_WriteData(' ');
        }
        else
        {
            LCD_WriteData(LCDText[i]);
        }
    }
    
    LCD_WriteCmd(LCDCMD_SET_DDRAM_ADDRESS | LCD_SECONDLINE_ADDRESS);
    
    for(i = 16; i < 32; i++)
    {
        if( LCDText[i] == 0 )
        {
            LCD_WriteData(' ');
        }
        else
        {
            LCD_WriteData(LCDText[i]);
        }
    }
}
   
 
/*********************************************************************
 * Function:        void LCDDisplay(char *text, BYTE value, BOOL delay)
 *
 * PreCondition:    LCD has been initialized
 *
 * Input:           text - text message to be displayed on LCD
 *                  value - the text message allows up to one byte 
 *                          of variable in the message
 *                  delay - whether need to display the message for
 *                          2 second without change
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function display the text message on the LCD, 
 *                  including up to one BYTE variable, if applicable.
 *
 * Note:            This routine is only effective if Explorer16 or
 *                  PIC18 Explorer demo boards are used
 ********************************************************************/
void LCDDisplay(char *text, BYTE value, BOOL delay)
{
    LCDErase();
    sprintf((char *)LCDText, (char*)text, value); 
    LCDUpdate();
    
    // display the message for 2 seconds
    if( delay )
    {
        BYTE i;
        for(i = 0; i < 8; i++)
        {
            DelayMs(250);
        }
    }
}
                   