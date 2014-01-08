/*************************************************
	PRGAoled.h - Additional driver functions for 
				MicroChip Starter Kit Oled Display

	Author: J. W. Hartley
	Revision:	20-Apr-10	Initial Edition

***************************************************/

char OledReadData(void);
//void OledRMWDataOR(char data);
char OledRMW_Read(void);
void OledRMW_Write(char data);
void PRGAWritePixel(BYTE X, BYTE Y, BYTE Data);

// end of file
