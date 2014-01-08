/************************************************************************
    File Information:
    FileName:       lcd_pic18_explorer_board.c
    Dependencies:   See INCLUDES section
    Processor:      PIC18 Microcontrollers
    Hardware:       The code is natively intended to be used on the following
                    hardware platforms: PIC18 Explorer Board
    Complier:       Microchip C18 (for PIC18) 
    Company:        Microchip Technology, Inc.

    Software License Agreement:

    The software supplied herewith by Microchip Technology Incorporated
    (the “Company”) for its PIC® Microcontroller is intended and
    supplied to you, the Company’s customer, for use solely and
    exclusively on Microchip PIC Microcontroller products. The
    software is owned by the Company and/or its supplier, and is
    protected under applicable copyright laws. All rights are reserved.
    Any use in violation of the foregoing restrictions may subject the
    user to criminal sanctions under applicable laws, as well as to
    civil liability for the breach of the terms and conditions of this
    license.

    THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

  Summary:
    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the application software
	.
       
    This file is located in the project directory

  Description:
    Application file
    
    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the application software.
    
    This file is located in the project directory.
    
 ********************************************************************/

/********************************************************************
 Change History:
  Rev    Description
  ----   -----------
  1.0   

********************************************************************/
#ifndef __LCD_PIC18_EXPLORER
	#define __LCD_PIC18_EXPLORER

/** I N C L U D E S **********************************************************/

#include <GenericTypeDefs.h> 
#include <Compiler.h>

extern BYTE LCDText[33]; //16*2+1
void LCDInit(void);
void LCDUpdate(void);
void LCDErase(void);
void LCDWriteLine(WORD number, char *line);
void LCDConfigWrite (BYTE byte);
void LCDDataWrite(BYTE data); 



#define LCD_GO_HOME() LCDConfigWrite(0x02)
#define LCD_SET_LINE1_DISPLAY_ADDRES(address)  LCDConfigWrite(address + 0x80)
#define LCD_SET_LINE2_DISPLAY_ADDRES(address)  LCDConfigWrite(address + 0xC0)

#endif //#ifndef __LCD_PIC18_EXPLORER









