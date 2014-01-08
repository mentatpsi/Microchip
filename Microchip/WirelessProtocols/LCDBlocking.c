/*********************************************************************
 *
 *       LCD Access Routines
 *
 *********************************************************************
 * FileName:        LCDBlocking.c
 * Dependencies:    Compiler.h, HardwareProfile.h
 * Processor:       PIC18, PIC24, PIC32, dsPIC30F, dsPIC33F
 * Compiler:        Microchip C18 v3.02 or higher
 *					Microchip C30 v2.01 or higher
 *                  Microchip C32 v1.02 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2002-2010 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product (“Device”) which is 
 *      integrated into Licensee’s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder     4/03/06		Original
 * Howard Schlunder     4/12/06		Changed from using PMP to LCDWrite()
 * Howard Schlunder		8/10/06		Fixed a delay being too short 
 *									when CLOCK_FREQ was a smaller 
 *									value, added FOUR_BIT_MODE
 * Yifeng Yang          6/20/09     Added support of using I/O Expender
 *                                  in PIC18 Explorer demo board
 * Yifeng Yang          5/28/10     Add support of LCD module via
 *                                  SPI and/or I2C interfaces
 ********************************************************************/
#define __LCDBLOCKING_C

#include <string.h>
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "WirelessProtocols/LCDBlocking.h"
#include "TimeDelay.h"
#include "WirelessProtocols/Console.h"
#include "Transceivers/Transceivers.h"

//#define FOUR_BIT_MODE
#define SAMSUNG_S6A0032		// This LCD driver chip has a different means of entering 4-bit mode.  



#if defined(EXPLORER16)  
    // LCDText is a 32 byte shadow of the LCD text.  Write to it and 
    // then call LCDUpdate() to copy the string into the LCD module.
    BYTE LCDText[16*2+1];

    /******************************************************************************
     * Function:        static void LCDWrite(BYTE RS, BYTE Data)
     *
     * PreCondition:    None
     *
     * Input:           RS - Register Select - 1:RAM, 0:Config registers
     *					Data - 8 bits of data to write
     *
     * Output:          None
     *
     * Side Effects:    None
     *
     * Overview:        Controls the Port I/O pins to cause an LCD write
     *
     * Note:            None
     *****************************************************************************/
    static void LCDWrite(BYTE RS, BYTE Data)
    {
    	#if defined(LCD_DATA_TRIS)
    		LCD_DATA_TRIS = 0x00;
    	#else
    		LCD_DATA0_TRIS = 0;
    		LCD_DATA1_TRIS = 0;
    		LCD_DATA2_TRIS = 0;
    		LCD_DATA3_TRIS = 0;
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_TRIS = 0;
    		LCD_DATA5_TRIS = 0;
    		LCD_DATA6_TRIS = 0;
    		LCD_DATA7_TRIS = 0;
    		#endif
    	#endif
    	LCD_RS_TRIS = 0;
    	LCD_RD_WR_TRIS = 0;
    	LCD_RD_WR_IO = 0;
    	LCD_RS_IO = RS;
    
    #if defined(FOUR_BIT_MODE)
    	#if defined(LCD_DATA_IO)
    		LCD_DATA_IO = Data>>4;
    	#else
    		LCD_DATA0_IO = Data & 0x10;
    		LCD_DATA1_IO = Data & 0x20;
    		LCD_DATA2_IO = Data & 0x40;
    		LCD_DATA3_IO = Data & 0x80;
    	#endif
    	Nop();					// Wait Data setup time (min 40ns)
    	Nop();
    	LCD_E_IO = 1;
    	Nop();					// Wait E Pulse width time (min 230ns)
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	LCD_E_IO = 0;
    #endif
    
    	#if defined(LCD_DATA_IO)
    		LCD_DATA_IO = Data;
    	#else
    		LCD_DATA0_IO = ((Data & 0x01) == 0x01);
    		LCD_DATA1_IO = ((Data & 0x02) == 0x02);
    		LCD_DATA2_IO = ((Data & 0x04) == 0x04);
    		LCD_DATA3_IO = ((Data & 0x08) == 0x08);
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_IO = ((Data & 0x10) == 0x10);
    		LCD_DATA5_IO = ((Data & 0x20) == 0x20);
    		LCD_DATA6_IO = ((Data & 0x40) == 0x40);
    		LCD_DATA7_IO = ((Data & 0x80) == 0x80);
    		#endif
    	#endif
    	Nop();					// Wait Data setup time (min 40ns)
    	Nop();
    	LCD_E_IO = 1;
    	Nop();					// Wait E Pulse width time (min 230ns)
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	LCD_E_IO = 0;
    
    	#if defined(LCD_DATA_TRIS)
    		LCD_DATA_TRIS = 0xFF;
    	#else
    		LCD_DATA0_TRIS = 1;
    		LCD_DATA1_TRIS = 1;
    		LCD_DATA2_TRIS = 1;
    		LCD_DATA3_TRIS = 1;
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_TRIS = 1;
    		LCD_DATA5_TRIS = 1;
    		LCD_DATA6_TRIS = 1;
    		LCD_DATA7_TRIS = 1;
    		#endif
    	#endif
    	LCD_RS_TRIS = 1;
    	LCD_RD_WR_TRIS = 1;
    }
    
    
    /******************************************************************************
     * Function:        void LCDInit(void)
     *
     * PreCondition:    None
     *
     * Input:           None
     *
     * Output:          None
     *
     * Side Effects:    None
     *
     * Overview:        LCDText[] is blanked, port I/O pin TRIS registers are 
     *					configured, and the LCD is placed in the default state
     *
     * Note:            None
     *****************************************************************************/
    void LCDInit(void)
    {
    	BYTE i;
    
    	memset(LCDText, ' ', sizeof(LCDText)-1);
    	LCDText[sizeof(LCDText)-1] = 0;
    
    	// Setup the I/O pins
    	LCD_E_IO = 0;
    	LCD_RD_WR_IO = 0;
    
    
    	#if defined(LCD_DATA_TRIS)
    		LCD_DATA_TRIS = 0x00;
    	#else
    		LCD_DATA0_TRIS = 0;
    		LCD_DATA1_TRIS = 0;
    		LCD_DATA2_TRIS = 0;
    		LCD_DATA3_TRIS = 0;
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_TRIS = 0;
    		LCD_DATA5_TRIS = 0;
    		LCD_DATA6_TRIS = 0;
    		LCD_DATA7_TRIS = 0;
    		#endif
    	#endif
    	LCD_RD_WR_TRIS = 0;
    	LCD_RS_TRIS = 0;
    	LCD_E_TRIS = 0;
    
    
    	// Wait the required time for the LCD to reset
		DelayMs(40);
    
    	// Set the default function
    	// Go to 8-bit mode first to reset the instruction state machine
    	// This is done in a loop 3 times to absolutely ensure that we get 
    	// to 8-bit mode in case if the device was previously booted into 
    	// 4-bit mode and our PIC got reset in the middle of the LCD 
    	// receiving half (4-bits) of an 8-bit instruction
    	LCD_RS_IO = 0;
    	#if defined(LCD_DATA_IO)
    		LCD_DATA_IO = 0x03;
    	#else
    		LCD_DATA0_IO = 1;
    		LCD_DATA1_IO = 1;
    		LCD_DATA2_IO = 0;
    		LCD_DATA3_IO = 0;
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_IO = 0;
    		LCD_DATA5_IO = 0;
    		LCD_DATA6_IO = 0;
    		LCD_DATA7_IO = 0;
    		#endif
    	#endif
    	Nop();					// Wait Data setup time (min 40ns)
    	Nop();
    	for(i = 0; i < 3; i++)
    	{
    		LCD_E_IO = 1;
			Delay10us(1);			// Wait E Pulse width time (min 230ns)
    		LCD_E_IO = 0;
			DelayMs(2);
    	}
    	
    #if defined(FOUR_BIT_MODE)
    	#if defined(SAMSUNG_S6A0032)
    		// Enter 4-bit mode (requires only 4-bits on the S6A0032)
    		#if defined(LCD_DATA_IO)
    			LCD_DATA_IO = 0x02;
    		#else
    			LCD_DATA0_IO = 0;
    			LCD_DATA1_IO = 1;
    			LCD_DATA2_IO = 0;
    			LCD_DATA3_IO = 0;
    		#endif
    		Nop();					// Wait Data setup time (min 40ns)
    		Nop();
    		LCD_E_IO = 1;
			Delay10us(1);			// Wait E Pulse width time (min 230ns)
    		LCD_E_IO = 0;
    	#else
    		// Enter 4-bit mode with two lines (requires 8-bits on most LCD controllers)
    		LCDWrite(0, 0x28);
    	#endif
    #else
    	// Use 8-bit mode with two lines
    	LCDWrite(0, 0x38);
    #endif
		Delay10us(5);
    	
    	// Set the entry mode
    	LCDWrite(0, 0x06);	// Increment after each write, do not shift
		Delay10us(5);
    
    	// Set the display control
    	LCDWrite(0, 0x0C);		// Turn display on, no cusor, no cursor blink
		Delay10us(5);
    
    	// Clear the display
    	LCDWrite(0, 0x01);	
		DelayMs(2);
    	
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
    	LCDWrite(0, 0x02);
		DelayMs(2);
    
    	// Output first line
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
    		LCDWrite(1, LCDText[i]);
			Delay10us(5);
    	}
    	
    	// Set the address to the second line
    	LCDWrite(0, 0xC0);
		Delay10us(5);
    
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
    		LCDWrite(1, LCDText[i]);
			Delay10us(5);
    	}
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
    	LCDWrite(0, 0x01);
		DelayMs(2);
    
    	// Clear local copy
    	memset(LCDText, ' ', 32);
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
    
#elif defined(PIC18_EXPLORER)

    #include "WirelessProtocols/MSPI.h"
    // LCDText is a 32 byte shadow of the LCD text.  Write to it and 
    // then call LCDUpdate() to copy the string into the LCD module.
    BYTE LCDText[16*2+1];
        
    void LCDInitSPI(void)
    {
        TRISCbits.TRISC5 = 0;
        TRISCbits.TRISC3 = 0;
        PIR1bits.SSPIF = 0;    
    }
    
    void LCDInitPortA(void)
    {
        BYTE preRFIE = RFIE;
        RFIE = 0;
        LCD_CS = 0;
        SPIPut(0x40);
        SPIPut(0x00);
        SPIPut(0x00);
        LCD_CS = 1;
        RFIE = preRFIE;
    }
    
    void LCDInitPortB(void)
    {
        BYTE preRFIE = RFIE;
        RFIE = 0;
        LCD_CS = 0;
        SPIPut(0x40);
        SPIPut(0x01);
        SPIPut(0x00);
        LCD_CS = 1;
        RFIE = preRFIE;
    }
    
    void LCDWritePortA(BYTE v)
    {
        BYTE preRFIE = RFIE;
        RFIE = 0;
        LCD_CS = 0;
        SPIPut(0x40);
        SPIPut(0x12);
        SPIPut(v);
        LCD_CS = 1;
        RFIE = preRFIE;
    }
    
    void LCDWritePortB(BYTE v)
    {
        BYTE preRFIE = RFIE;
        RFIE = 0;
        LCD_CS = 0;
        SPIPut(0x40);
        SPIPut(0x13);
        SPIPut(v);
        LCD_CS = 1;
        RFIE = preRFIE;
    }
    
    void LCDInitWrite(BYTE v)
    {
        LCDWritePortA(0);
        LCDWritePortB(v);
        Nop();
        Nop();
        Nop();
        
        LCDWritePortA(0x40);
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        LCDWritePortA(0);
    }
    
    void LCD_i_write(BYTE v)
    {
        LCDWritePortA(0);
        DelayMs(1);
        LCDWritePortB(v);
        Nop();
        Nop();
        Nop();
        Nop();
        LCDWritePortA(0x40);
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        LCDWritePortA(0x00);
        return;
    }
    
    void LCD_d_write(BYTE v)
    {
        DelayMs(1);
        LCDWritePortA(0x80);
        LCDWritePortB(v);
        Nop();
        Nop();
        Nop();
        Nop();
        LCDWritePortA(0xC0);
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        LCDWritePortA(0);
        
    } 



    /******************************************************************************
     * Function:        void LCDInit(void)
     *
     * PreCondition:    None
     *
     * Input:           None
     *
     * Output:          None
     *
     * Side Effects:    None
     *
     * Overview:        LCDText[] is blanked, port I/O pin TRIS registers are 
     *					configured, and the LCD is placed in the default state
     *
     * Note:            None
     *****************************************************************************/
    void LCDInit(void)
    {
        
        LCD_CS_TRIS = 0;
        LCD_CS = 1;
        DelayMs(10);
        
        LCD_RESET_TRIS = 0;
        LCD_RESET = 0;
        DelayMs(10);
        LCD_RESET = 1;
        
        // init SPI
        LCDInitSPI();
        
        // init MCP923S17 Port
        LCDInitPortA();
        
        // step B
        LCDInitPortB();
        
        LCDWritePortA(0);
    
        // Function Set
        DelayMs(10);
        LCDInitWrite(0x3C);
        
        DelayMs(10);
        LCDInitWrite(0x0C);    // display off
        
        DelayMs(10);
        LCDInitWrite(0x01);    // display clear
        
        DelayMs(10);
        LCDInitWrite(0x0C);    // Entry mode
        
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
        BYTE i;

        LCD_i_write(0x80);
        for(i = 0; i < 16; i++)
        {
            if( LCDText[i] == 0 )
            {
                LCD_d_write(' ');
            }
            else
            {
                LCD_d_write(LCDText[i]);
            }
        }
        LCD_i_write(0xC0);
        for(i = 16; i < 32; i++)
        {
            if( LCDText[i] == 0 )
            {
                LCD_d_write(' ');
            }
            else
            {
                LCD_d_write(LCDText[i]);
            }
        }
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
    	LCDInitWrite(0x01);    
    	DelayMs(2);
    
    	// Clear local copy
    	memset(LCDText, ' ', 32);
    }


#elif defined(EIGHT_BIT_WIRELESS_BOARD) && defined(SENSOR_PORT_LCD)
    #if defined(SENSOR_PORT_UART)
        #define LCD_SLAVEADDRESS            0x3E
        #define LCD_WRITE_CMD               0x32
        #define LCD_INIT_CMD                0x81
        #define LCDCMD_FIRSTLINE_ADDRESS	0x00
        
        BYTE LCDCommand[4];
        BYTE CommandLen = 0;
        BYTE TextLen = 0;
        BYTE CommandPtr = 0;
        BYTE TextPtr = 0;
    #else
        #include "WirelessProtocols/MSPI.h"
    #endif
    
    // LCDText is a 32 byte shadow of the LCD text.  Write to it and 
    // then call LCDUpdate() to copy the string into the LCD module.
    BYTE LCDText[16*2+1];

    #if defined(SENSOR_PORT_UART)
        void I2CTask(void)
        {
            
            if( CommandPtr < CommandLen || TextPtr < TextLen )
            {
                while (SSP2CON2bits.SEN == 1);
                if( CommandPtr < CommandLen )
                {
                    SSP2BUF = LCDCommand[CommandPtr];
                    //if (SSP2CON2bits.ACKSTAT == 0)
                    {
                        CommandPtr++;
                    }    
                    if( CommandPtr == CommandLen && TextLen == 0 )
                    {
                        //DelayMs(300);
                        SSP2CON2bits.PEN = 1;
                        DelayMs(300);
                    }    
                } 
                else if( TextPtr < TextLen )
                {
                    if( LCDText[TextPtr] == 0 )
                    {
                        LCDText[TextPtr] = ' ';
                    }
                    SSP2BUF = LCDText[TextPtr];
    
                    //if (SSP2CON2bits.ACKSTAT == 0)
                    {
                        TextPtr++;
                    }  
                    if( TextPtr == TextLen )
                    {
                        //DelayMs(300);
                        SSP2CON2bits.PEN = 1;
                        DelayMs(300);
                    }     
                }
            } 
        }    
    
        void I2CWrite(BYTE Commandlen, BYTE BufferLen)
        {
            while(SSP2CON2bits.PEN == 1 );
            CommandPtr = 0;
            TextPtr = 0;
            CommandLen = Commandlen;
            TextLen = BufferLen;
            SSP2CON2bits.SEN = 1;
        }
    #endif
        
            
    void LCDInit(void)
    {
        #if defined(SENSOR_PORT_UART)   
            TRISD |= 0x53;
            //PORTEbits.RDPU = 1;
            
            PIR3bits.SSP2IF = 0;	                    //Clear SSP IF
        	SSP2STAT = 0x00;                //Enable Slewrate Control, Clearing flags
        	//SSP2ADD = 0x19;					//Fclk = 100kHz
        	SSP2ADD = 0xF0;
        	SSP2CON1 = 0x28;                //Enable Master Mode
        	SSP2CON2 = 0x00;
        	PIE3bits.SSP2IE = 1;
    
        	INTCONbits.GIEH = 1;
        	INTCONbits.GIEL = 1;
    	
            DelayMs(300);
            
            LCDCommand[0] = LCD_SLAVEADDRESS << 1;
            LCDCommand[1] = LCD_INIT_CMD;
            LCDCommand[2] = 0x00;
            
            I2CWrite(3, 0);
            
            DelayMs(200);
        #else
            TRISDbits.TRISD0 = 1;
            TRISDbits.TRISD1 = 1;
    
        	TRISDbits.TRISD6 = 0;
        	TRISDbits.TRISD2 = 1;
        	TRISDbits.TRISD4 = 0;
        	TRISCbits.TRISC6 = 0;
        
        	SPISSPSTAT = 0x80;//0xC0;	
        	SPISSPCON = 0x22;//0x21; 
        	
        	SPInCS_LAT = 0;	
        #endif
    }    
    
    void LCDErase(void)
    {
        BYTE i;
        
        for(i = 0; i < 32; i++)
        {
            LCDText[i] = ' ';
        }
    }
    
    void LCDUpdate(void)
    {
        #if defined(SENSOR_PORT_UART)   
            LCDCommand[0] = LCD_SLAVEADDRESS<<1;
            LCDCommand[1] = LCD_WRITE_CMD;
            LCDCommand[2] = LCDCMD_FIRSTLINE_ADDRESS;
            LCDCommand[3] = 32;
                        I2CWrite(4, 32);
        #else
            BYTE i;
        
            DelayMs(10);
            SPIPut2(0x32);                  //LCD Subsystem Write Data Command
            DelayMs(5);                     
            SPIPut2(0);                     //Screen Address
            DelayMs(5);
            SPIPut2(32);                    //Data length (16 + 3)
            for(i = 0; i < 32; i++)
            {
                DelayMs(5);
                if( LCDText[i] == 0 )
                {
                    SPIPut2(' ');
                }
                else
                {    
                    SPIPut2(LCDText[i]);
                }    
            }
            
            DelayMs(300);
        #endif
    }    
    
    
#endif
