/********************************************************************
  File Information:
    FileName:     	oled.c
    Dependencies:   See INCLUDES section
    Processor:      PIC18F46J50
    Hardware:       PIC18F Starter Kit
    Complier:  	    Microchip C18 (for PIC18)
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

  File Description:
    
    Change History:
     Rev   Date         Description
     1.0                Initial release
	 1.1   2010-05-12   Put Negative char and String added

********************************************************************/

/******** Include files **********************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "oled.h"

#define	oledWR			LATEbits.LATE1
#define	oledWR_TRIS		TRISEbits.TRISE1
#define	oledRD			LATEbits.LATE0
#define	oledRD_TRIS		TRISEbits.TRISE0
#define	oledCS			LATEbits.LATE2
#define	oledCS_TRIS		TRISEbits.TRISE2
#define	oledRESET		LATDbits.LATD1
#define	oledRESET_TRIS	TRISDbits.TRISD1
#define	oledD_C			LATBbits.LATB5
#define	oledD_C_TRIS	TRISBbits.TRISB5

// Color
BYTE    _color;
// Clipping region control
SHORT _clipRgn;
// Clipping region borders
SHORT _clipLeft;
SHORT _clipTop;
SHORT _clipRight;
SHORT _clipBottom;


/*********************************************************************
* Function:  void  DelayMs(WORD time)
*
* PreCondition: none
*
* Input: time - delay in ms
*
* Output: none
*
* Side Effects: none
*
* Overview: delays execution on time specified in ms
*
********************************************************************/
#define DELAY_1MS 32000/9

void  DelayMs(WORD time){
unsigned delay;
	while(time--)
		for(delay=0; delay<DELAY_1MS; delay++);	
}

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/
void ResetDevice(void){

    unsigned long i;
    unsigned int data;

    oledWR = 0;
    oledWR_TRIS = 0;
    oledRD = 0;
    oledRD_TRIS = 0;
    oledCS = 1;
    oledCS_TRIS = 0;
    oledD_C	= 0;
    oledD_C_TRIS = 0;

    //Reset the device
    oledRESET = 0;
    for(i=0;i<100;i++){}
    oledRESET = 1;
    for(i=0;i<100;i++){}

	// Setup Display
	WriteCommand(0xAE);			// turn off the display (AF=ON, AE=OFF)
	
	WriteCommand(0xDB);			// set  VCOMH
	WriteCommand(0x23);			 

	WriteCommand(0xD9);			// set  VP
	WriteCommand(0x22);			 

	//////////////////////////////
	// User Set Up
	//////////////////////////////

	// Re-map
	WriteCommand(0xA1);			// [A0]:column address 0 is map to SEG0
								// [A1]:column address 131 is map to SEG0

	// COM Output Scan Direction
	WriteCommand(0xC8);			// C0 is COM0 to COMn, C8 is COMn to COM0

	// COM Pins Hardware Configuration
	WriteCommand(0xDA);			// set pins hardware configuration
	WriteCommand(0x12);

	// Multiplex Ratio
	WriteCommand(0xA8);			// set multiplex ratio
	WriteCommand(0x3F);			// set to 64 mux

	// Display Clock Divide
	WriteCommand(0xD5);			// set display clock divide
	WriteCommand(0xA0);			// set to 100Hz

	// Contrast Control Register
	WriteCommand(0x81);			// Set contrast control
	WriteCommand(0x60);			// display 0 ~ 127; 2C

	// Display Offset
	WriteCommand(0xD3);			// set display offset
	WriteCommand(0x00);			// no offset
	
	//Normal or Inverse Display
	WriteCommand(0xA6);			// Normal display

	WriteCommand(0xAD);			// Set DC-DC
	WriteCommand(0x8B);			// 8B=ON, 8A=OFF 
	
	// Display ON/OFF
	WriteCommand(0xAF);			// AF=ON, AE=OFF
	DelayMs(10);

	// Entire Display ON/OFF
	WriteCommand(0xA4);			// A4=ON

	// Display Start Line
	WriteCommand(0x40);			// Set display start line

	// Lower Column Address
	WriteCommand(0x00+OFFSET);	// Set lower column address

	// Higher Column Address
	WriteCommand(0x10);			// Set higher column address
}

/*********************************************************************
* Macros:  WriteCommand()
*
* Overview: Writes command word to the display controller. A delay
*			is inserted at the end to meet the controller requirements
*			on selected commands.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void WriteCommand(BYTE cmd)
{
    TRISD = 0x00;
    LATD  = cmd;
    oledRD = 1;
    oledWR = 1;
    oledD_C	= 0;
    oledCS = 0;
    oledWR = 0;
    oledWR = 1;
    oledCS = 1;
    TRISD = 0xFF;
}

/*********************************************************************
* Macros:  WriteData()
*
* Overview: Writes data word to the display controller. A delay
*			is inserted at the end to meet the controller requirements
*			on selected commands.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void WriteData(BYTE data)
{
    TRISD = 0x00;
    LATD  = data;
    oledRD = 1;
    oledWR = 1;
    oledD_C	= 1;
    oledCS = 0;
    oledWR = 0;
    oledWR = 1;
    oledCS = 1;
    TRISD = 0xFF;
}

void FillDisplay(unsigned char data)
{
	unsigned char i,j;

	for(i=0xB0;i<0xB8;i++)			// Go through all 8 pages
	{
		WriteCommand(i);		// Set page
		WriteCommand(0x00+OFFSET);		// Set lower column address
		WriteCommand(0x10);		// Set upper column address

		for(j=0;j<132;j++)			// Write to all 132 bytes
		{
			WriteData(data);
		}
	}
	return;
}

void oledWriteChar1x(char letter, unsigned char page, unsigned char column)
{
	BYTE i;

	letter -= ' ';					// Adjust character to table that starts at 0x20
	WriteCommand(page);
	column += OFFSET;
	WriteCommand(0x00+(column&0x0F));
	WriteCommand(0x10+((column>>4)&0x0F));

	WriteData(g_pucFont[letter][0]);	// Write first column
	WriteData(g_pucFont[letter][1]);	// Write second column
	WriteData(g_pucFont[letter][2]);	// Write third column
	WriteData(g_pucFont[letter][3]);	// Write fourth column
	WriteData(g_pucFont[letter][4]);	// Write fifth column
	WriteData(0x00);					// Write 1 column for buffer to next character
	return;
}

void oledWriteNegChar1x(char letter, unsigned char page, unsigned char column)
{
	BYTE i;

	letter -= ' ';					// Adjust character to table that starts at 0x20
	WriteCommand(page);
	column += OFFSET;
	WriteCommand(0x00+(column&0x0F));
	WriteCommand(0x10+((column>>4)&0x0F));

	WriteData(~g_pucFont[letter][0]);	// Write first column
	WriteData(~g_pucFont[letter][1]);	// Write second column
	WriteData(~g_pucFont[letter][2]);	// Write third column
	WriteData(~g_pucFont[letter][3]);	// Write fourth column
	WriteData(~g_pucFont[letter][4]);	// Write fifth column
	WriteData(0xff);					// Write 1 column for buffer to next character
	return;
}

void oledPutROMString(rom unsigned char *ptr,unsigned char page, unsigned char col)
{
	unsigned char i;
	
	i=col;
	page = page + 0xB1;
	while(*ptr)
	{
		oledWriteChar1x(*ptr,page,i);
		ptr++;
		i+=6;
	}
}

void oledPutROMNegString(rom unsigned char *ptr,unsigned char page, unsigned char col)
{
	unsigned char i;
	
	i=col;
	page = page + 0xB1;
	while(*ptr)
	{
		oledWriteNegChar1x(*ptr,page,i);
		ptr++;
		i+=6;
	}
}

void oledPutString(unsigned char *ptr,unsigned char page, unsigned char col)
{
	unsigned char i;
	
	i=col;
	page = page + 0xB1;
	while(*ptr)
	{
		oledWriteChar1x(*ptr,page,i);
		ptr++;
		i+=6;
	}
}

void oledPutNegString(unsigned char *ptr,unsigned char page, unsigned char col)
{
	unsigned char i;
	
	i=col;
	page = page + 0xB1;
	while(*ptr)
	{
		oledWriteNegChar1x(*ptr,page,i);
		ptr++;
		i+=6;
	}
}


void oledPutImage(rom unsigned char *ptr, unsigned char sizex, unsigned char sizey, unsigned char startx, unsigned char starty)
{
	unsigned char i,j,mask;
	unsigned int count = 0;
	
	startx += OFFSET;
	for(i=0xB0+starty;i<(0xB0+sizey);i++)
	{
		WriteCommand(i);
		WriteCommand(startx&0x0F);
		WriteCommand(0x10 | ((startx>>4)&0x0F));

		for(j=0;j<sizex;j++)
		{
			count ++;
			WriteData(*ptr++);

		}
	}
	return;
}

	


//////////////////////////////////////
//////////////////////////////////////
// OLED ROM CONSTANT DATA ////////////
//////////////////////////////////////
//////////////////////////////////////
ROM BYTE g_pucFont[95][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 }, // " " 0x20
    { 0x00, 0x00, 0x4f, 0x00, 0x00 }, // !   0x21
    { 0x00, 0x07, 0x00, 0x07, 0x00 }, // "   0x22
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 }, // #   0x23
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 }, // $   0x24
    { 0x23, 0x13, 0x08, 0x64, 0x62 }, // %   0x25
    { 0x36, 0x49, 0x55, 0x22, 0x50 }, // &   0x26
    { 0x00, 0x05, 0x03, 0x00, 0x00 }, // '   0x27
    { 0x00, 0x1c, 0x22, 0x41, 0x00 }, // (   0x28
    { 0x00, 0x41, 0x22, 0x1c, 0x00 }, // )   0x29
    { 0x14, 0x08, 0x3e, 0x08, 0x14 }, // *   0x2A
    { 0x08, 0x08, 0x3e, 0x08, 0x08 }, // +   0x2B
    { 0x00, 0x50, 0x30, 0x00, 0x00 }, // ,   0x2C
    { 0x08, 0x08, 0x08, 0x08, 0x08 }, // -   0x2D
    { 0x00, 0x60, 0x60, 0x00, 0x00 }, // .   0x2E
    { 0x20, 0x10, 0x08, 0x04, 0x02 }, // /   0x2F
    { 0x3e, 0x51, 0x49, 0x45, 0x3e }, // 0   0x30
    { 0x00, 0x42, 0x7f, 0x40, 0x00 }, // 1   0x31
    { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2   0x32
    { 0x21, 0x41, 0x45, 0x4b, 0x31 }, // 3   0x33
    { 0x18, 0x14, 0x12, 0x7f, 0x10 }, // 4   0x34
    { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5   0x35
    { 0x3c, 0x4a, 0x49, 0x49, 0x30 }, // 6   0x36
    { 0x01, 0x71, 0x09, 0x05, 0x03 }, // 7   0x37
    { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8   0x38
    { 0x06, 0x49, 0x49, 0x29, 0x1e }, // 9   0x39
    { 0x00, 0x36, 0x36, 0x00, 0x00 }, // :   0x3A
    { 0x00, 0x56, 0x36, 0x00, 0x00 }, // ;   0x3B
    { 0x08, 0x14, 0x22, 0x41, 0x00 }, // <   0x3C
    { 0x14, 0x14, 0x14, 0x14, 0x14 }, // =   0x3D
    { 0x00, 0x41, 0x22, 0x14, 0x08 }, // >   0x3E
    { 0x02, 0x01, 0x51, 0x09, 0x06 }, // ?   0x3F
    { 0x32, 0x49, 0x79, 0x41, 0x3e }, // @   0x40
    { 0x7e, 0x11, 0x11, 0x11, 0x7e }, // A   0x41
    { 0x7f, 0x49, 0x49, 0x49, 0x36 }, // B   0x42
    { 0x3e, 0x41, 0x41, 0x41, 0x22 }, // C   0x43
    { 0x7f, 0x41, 0x41, 0x22, 0x1c }, // D   0x44
    { 0x7f, 0x49, 0x49, 0x49, 0x41 }, // E   0x45
    { 0x7f, 0x09, 0x09, 0x09, 0x01 }, // F   0x46
    { 0x3e, 0x41, 0x49, 0x49, 0x7a }, // G   0x47
    { 0x7f, 0x08, 0x08, 0x08, 0x7f }, // H   0x48
    { 0x00, 0x41, 0x7f, 0x41, 0x00 }, // I   0x49
    { 0x20, 0x40, 0x41, 0x3f, 0x01 }, // J   0x4A
    { 0x7f, 0x08, 0x14, 0x22, 0x41 }, // K   0x4B
    { 0x7f, 0x40, 0x40, 0x40, 0x40 }, // L   0x4C
    { 0x7f, 0x02, 0x0c, 0x02, 0x7f }, // M   0x4D
    { 0x7f, 0x04, 0x08, 0x10, 0x7f }, // N   0x4E
    { 0x3e, 0x41, 0x41, 0x41, 0x3e }, // O   0x4F
    { 0x7f, 0x09, 0x09, 0x09, 0x06 }, // P   0X50
    { 0x3e, 0x41, 0x51, 0x21, 0x5e }, // Q   0X51
    { 0x7f, 0x09, 0x19, 0x29, 0x46 }, // R   0X52
    { 0x46, 0x49, 0x49, 0x49, 0x31 }, // S   0X53
    { 0x01, 0x01, 0x7f, 0x01, 0x01 }, // T   0X54
    { 0x3f, 0x40, 0x40, 0x40, 0x3f }, // U   0X55
    { 0x1f, 0x20, 0x40, 0x20, 0x1f }, // V   0X56
    { 0x3f, 0x40, 0x38, 0x40, 0x3f }, // W   0X57
    { 0x63, 0x14, 0x08, 0x14, 0x63 }, // X   0X58
    { 0x07, 0x08, 0x70, 0x08, 0x07 }, // Y   0X59
    { 0x61, 0x51, 0x49, 0x45, 0x43 }, // Z   0X5A
    { 0x00, 0x7f, 0x41, 0x41, 0x00 }, // [   0X5B
    { 0x02, 0x04, 0x08, 0x10, 0x20 }, // "\" 0X5C
    { 0x00, 0x41, 0x41, 0x7f, 0x00 }, // ]   0X5D
    { 0x04, 0x02, 0x01, 0x02, 0x04 }, // ^   0X5E
    { 0x40, 0x40, 0x40, 0x40, 0x40 }, // _   0X5F
    { 0x00, 0x01, 0x02, 0x04, 0x00 }, // `   0X60
    { 0x20, 0x54, 0x54, 0x54, 0x78 }, // a   0X61
    { 0x7f, 0x48, 0x44, 0x44, 0x38 }, // b   0X62
    { 0x38, 0x44, 0x44, 0x44, 0x20 }, // c   0X63
    { 0x38, 0x44, 0x44, 0x48, 0x7f }, // d   0X64
    { 0x38, 0x54, 0x54, 0x54, 0x18 }, // e   0X65
    { 0x08, 0x7e, 0x09, 0x01, 0x02 }, // f   0X66
    { 0x0c, 0x52, 0x52, 0x52, 0x3e }, // g   0X67
    { 0x7f, 0x08, 0x04, 0x04, 0x78 }, // h   0X68
    { 0x00, 0x44, 0x7d, 0x40, 0x00 }, // i   0X69
    { 0x20, 0x40, 0x44, 0x3d, 0x00 }, // j   0X6A
    { 0x7f, 0x10, 0x28, 0x44, 0x00 }, // k   0X6B
    { 0x00, 0x41, 0x7f, 0x40, 0x00 }, // l   0X6C
    { 0x7c, 0x04, 0x18, 0x04, 0x78 }, // m   0X6D
    { 0x7c, 0x08, 0x04, 0x04, 0x78 }, // n   0X6E
    { 0x38, 0x44, 0x44, 0x44, 0x38 }, // o   0X6F
    { 0x7c, 0x14, 0x14, 0x14, 0x08 }, // p   0X70
    { 0x08, 0x14, 0x14, 0x18, 0x7c }, // q   0X71
    { 0x7c, 0x08, 0x04, 0x04, 0x08 }, // r   0X72
    { 0x48, 0x54, 0x54, 0x54, 0x20 }, // s   0X73
    { 0x04, 0x3f, 0x44, 0x40, 0x20 }, // t   0X74
    { 0x3c, 0x40, 0x40, 0x20, 0x7c }, // u   0X75
    { 0x1c, 0x20, 0x40, 0x20, 0x1c }, // v   0X76
    { 0x3c, 0x40, 0x30, 0x40, 0x3c }, // w   0X77
    { 0x44, 0x28, 0x10, 0x28, 0x44 }, // x   0X78
    { 0x0c, 0x50, 0x50, 0x50, 0x3c }, // y   0X79
    { 0x44, 0x64, 0x54, 0x4c, 0x44 }, // z   0X7A
    { 0x00, 0x08, 0x36, 0x41, 0x00 }, // {   0X7B
    { 0x00, 0x00, 0x7f, 0x00, 0x00 }, // |   0X7C
    { 0x00, 0x41, 0x36, 0x08, 0x00 }, // }   0X7D
    { 0x02, 0x01, 0x02, 0x04, 0x02 }, // ~   0X7E
};