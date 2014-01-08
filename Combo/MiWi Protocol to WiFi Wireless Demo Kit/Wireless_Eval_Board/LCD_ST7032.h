/********************************************************************
* FileName:		LCD_ST7032.h
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
*  Header file for LCD_ST7032.c
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/09/2011    ccs       Initial revision
********************************************************************/
#ifndef __LCD_ST7032_H_
	#define __LCD_ST7032_H_
    
    #define	LCD_LINELENGTH				    16    
    #define	LCD_FIRSTLINE_ADDRESS	        0x00
    #define	LCD_SECONDLINE_ADDRESS	        0x40
        
    #define     LCD_WAKEUP                      0x30
    #define     LCD_FUNCTION_SET                0x39    // 0x39	-	8 bit interface; 2 lines; Instruction Table 1
    #define     LCD_INT_OSC                     0x14    // 0x14	-	Set Internal OSC and frame frequency -> 183 Hz
    #define     LCD_PWR_CTRL                    0x56    // 0x56	-	Power Control; Booster ON; Contrast C5=1
    #define     LCD_FOLLOWER_CTRL               0x6D    // 0x6D	-	Follower Control; Follower On; Amplification = 0b101
    #define     LCD_ON                          0x0E    // 0x0C	-	Display ON
    #define     LCD_ENTRY_MODE                  0x06    // 0x06	-	Entry Mode: Left to right
    #define	LCDCMD_CLEARDISPLAY			    0x01    // 0x01	-	Clear Display
    #define	LCDCMD_CONTRASTSET_HIGHBYTE	    0x54
    #define     LCDCMD_CONTRASTSET_LOWBYTE	    0x70    // 0x70	-	Set Contrast low byte				 -> 0x00
    #define     LCDCMD_SET_DDRAM_ADDRESS	    0x80

    extern BYTE LCDText[16*2+1];
    
    void LCDInit(void);
    void LCDUpdate(void);
    void LCDErase(void);
    void LCDBacklightON(void);
    void LCDBacklightOFF(void);
    void LCDDisplay(char *, BYTE, BOOL);
    void LCDWriteLine(WORD number, char *line);

#endif