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
  0.03   Under Development

********************************************************************/
/** I N C L U D E S **********************************************************/
 #include "lcd_pic18_explorer_board.h"
 #include <delays.h>
 #include "HardwareProfile.h"
 
 #define delay()   Nop();//Delay10TCYx(6);

 #if defined PHD_USE_LCD_DISPLAY
/** V A R I A B L E S ********************************************************/
 // LCDText is a 32 byte shadow of the LCD text.  Write to it and 
 // then call LCDUpdate() to copy the string into the LCD module. 
 BYTE LCDText[16*2+1];

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void SpiInit(void);
void PortASpiInit(void);
void PortBSpiInit(void);
void WritePortA(BYTE LcdEnable, BYTE LcdRS);
void WritePortB(BYTE tempWr);
void LCDConfigWrite (BYTE byte);
void IWrite (BYTE tempWr);
void LcdBusy(void);
void SDelay (void); 
void LCDDataWrite(BYTE data); 
void SpiSendByte (BYTE byte);
    
    
void LCDDataWrite(BYTE data)
{
    LcdBusy(); 
    WritePortA(0,1);  
    WritePortB(data);
    Nop(); Nop(); Nop(); Nop(); 
    WritePortA(1,1);         
    Nop(); Nop(); Nop(); Nop(); Nop(); Nop();  
    WritePortA(0,0); 
}  	 
   
void LCDConfigWrite (BYTE byte)	 
{
    WritePortA(0,0);
    WritePortB(byte); 
    Nop();Nop();Nop();
    WritePortA(1,0); 
    Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();
    WritePortA(0,0);
}   
 
void SDelay (void)
{
    Nop(); //Delay10TCYx(9); 
} 
     
void LcdBusy(void)
{
    SDelay(); 
    SDelay(); 
}     
     
void LcdLine_1(void)
{
    LCDConfigWrite(0xC0); 
}  	 
	
void LCDInit(void)
{
    LCD_CS_TRIS = 0; // set chip select to output
    LCD_CS = 1;  // unselect LCD 
        
    LCD_RST_TRIS = 0;  // Set LCD Reset to output
    LCD_RST = 1;   // 
        
    delay();delay(); delay();  
        
    SpiInit(); 
    PortASpiInit(); 
    PortBSpiInit();
         
    WritePortA(0,0);    
    delay();  
    
    LCDConfigWrite(0x3C); 
    delay(); 
      
    LCDConfigWrite(0x0C); //Display off 
    delay();
        
    LCDConfigWrite(0x01); //Display Clear
    delay();
          
    LCDConfigWrite(0x06); //Entry Mode 
} 
     
void SpiInit(void)
{
    #if defined(PIC18F47J53_PIM) 
        RPOR4 = 11; // RP4/RB1 as SCK2
        RPOR2 = 10; // RP2/RA5 as SDO2 
    #elif defined(PIC18F46J50_PIM) 
        RPOR4 = 10; // RP4/RB1 as SCK2
        RPOR2 = 9; // RP2/RA5 as SDO2 
    #endif 
    SPI_CLK_TRIS = 0; 
    SPI_SDO_TRIS = 0;
    #if defined (PIC18F47J53_PIM) || defined(PIC18F46J50_PIM) 
        SSP2CON1 = 0x20; 
        SSP2STATbits.CKE = 1; 
        PIR3bits.SSP2IF = 0; 
    #elif defined(PIC18F87J50_PIM)
        SSP1CON1 = 0x20; 
        SSP1STATbits.CKE = 1; 
        PIR1bits.SSP1IF = 0;
    #endif  
}   
     
void SpiSendByte (BYTE byte)
{
    #if defined (PIC18F47J53_PIM) || defined(PIC18F46J50_PIM) 
        SSP2BUF = byte;
        while(PIR3bits.SSP2IF == 0); 
        PIR3bits.SSP2IF = 0; 
    #elif defined(PIC18F87J50_PIM)
        SSP1BUF = byte;
        while(PIR1bits.SSP1IF == 0); 
        PIR1bits.SSP1IF = 0; 
    #endif 
}     
     
void PortASpiInit(void)
{
    LCD_CS = 0;  
    SpiSendByte (0x40); 
    SpiSendByte (0x00); 
    SpiSendByte (0x00); 
    LCD_CS = 1; 
}    
     
void PortBSpiInit(void)
{
    LCD_CS = 0; 
    SpiSendByte (0x40); 
    SpiSendByte (0x01);         
    SpiSendByte (0x00);    
    LCD_CS = 1; 
}    
	 
void WritePortA(BYTE LcdEnable, BYTE LcdRS)
{
    BYTE LcdControl1 = 0x00; 
    LCD_CS = 0; 
    SpiSendByte (0x40); 
    SpiSendByte (0x12); 
    LcdControl1 |= LcdEnable<<6; // LCD Enable bit position is 1
    LcdControl1 |= LcdRS<<7; 
    SpiSendByte (LcdControl1); 
	LCD_CS = 1;
}  
     
void WritePortB(BYTE tempWr)
{
    LCD_CS = 0; 
    SpiSendByte (0x40);
    SpiSendByte (0x13);
    SpiSendByte (tempWr);  
    LCD_CS = 1;  
}    
     
     
void LCDErase(void)
{
    LCDConfigWrite(0x01);
    // Clear local copy
    memset(LCDText, ' ', 32); 
}  
     
void ReturnHome(void)
{
    LCDConfigWrite(0x02); 
}   

/******************************************************************************
 * Function:        void LCDUpdate(void)
 *
 * PreCondition:    LCDInit() must have been called once
 *
 * Input:           LCDText[]
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Copies the contents of the local LCDText[] array into the 
 *					LCD's internal display buffer.  Null terminators in 
 *					LCDText[] terminate the current line, so strings may be 
 *					printed directly to LCDText[].
 *
 * Note:            None
 *****************************************************************************/
 void LCDUpdate(void)
 {
	BYTE i, j;

	// Go home
	LCDConfigWrite(0x02);
	
	//DelayMs(2);
	
	// Output first line
	//while(i< 16) 	
	for(i = 0; i < 16u; i++)
	{
		// Erase the rest of the line if a null char is 
		// encountered (good for printing strings directly)
		if(LCDText[i] == 0u)
		{
			for(j=i; j < 16u; j++)
			{
				LCDText[j] = ' ';
			}
		}
		//if (SPIQueuecount == 0)
		//{ 
		    LCDDataWrite(LCDText[i]);
		    //i++; 
		    //if (i == 16)
		    //{
    		    //i=0; 
    		//}    
		//}
		//else 
		    //break;     
//		Delay10us(5);
	}
	
	// Set the address to the second line
	LCDConfigWrite(0xC0);
//	Delay10us(5);
//
	// Output second line
	for(i = 16; i < 32u; i++)
	{
		// Erase the rest of the line if a null char is 
		// encountered (good for printing strings directly)
		if(LCDText[i] == 0u)
		{
			for(j=i; j < 32u; j++)
			{
				LCDText[j] = ' ';
			}
		}
		LCDDataWrite(LCDText[i]);
//		Delay10us(5);
	}
	//SpiTxService(); 
 }
 
         
void LCDWriteLine(WORD number, char *line)
{
    BYTE    i;
    BYTE    j;

    j = 0;
    i = 0;
        
    if(number == 2)
    {
        while ((LCDText[j] != 0) && (j < 16))
        {
            j++;
        }
    }    

    do 
    {
        LCDText[j++] = line[i++];
    }    
    while ((LCDText[j-1] != 0) && (j < 31));
    
    LCDUpdate();
}
#endif // PHD_USE_LCD_DISPLAY
 

 