/*************************************************
	PRGAoled.c - Additional driver functions for 
				MicroChip Starter Kit Oled Display

	P. R. Glassel & Associates
	30255 Fir Trail
	Stacy, MN 55079
	Web:	www.prga.com
	Email:	sales@prga.com
	Ph:		651-408-9007

	Author: J. W. Hartley
	Revision:	20-Apr-10	Initial Edition
				15-Jun-10	Update comments, Released internally for review

***************************************************/

/************************
	Some Notes on the Oled Display Controller - the Sino Wealth SH1101A
	1. The P/S and C86 lines are hardwired = 1. This specifies the 8080-parallel interface
	2. oledD_C = A0 in the HS1101A docs. The D_C is Data or Command
	3. oledWR  = WR in the HS1101A docs.
	4. oledRD  = RD in the HS1101A docs.

	A Command Write is:  oledD_C=0, oledRD=1, oledCS = 0,  oledWR=1/0/1, oledCS=1;
	A Data Write is:     oledD_C=1, oledRD=1, oledCS = 0,  oledWR=1/0/1, oledCS=1;
	A Data Read is :     oledD_C=1, oledWR=1, oledCS = 0,  oledRD=1/0/1, oledCS=1;
	A Status Read is:    oledD_C=0, oledWR=1, oledCS = 0,  oledRD=1/0/1, oledCS=1;

	PIC18F Starter Kit 1 has a 12 MHz clock, t=83ns, so a 1-cycle instruction is 333ns long.
	The minimum SH1101A Display controller control pulsewidth=100-120ns. 

	The PORTD bit 6 is shared with the accelerometer?

*************************/

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

/************************************************************************************
	Timing Note:	A write to a port is completed (at the I/O pin) after the end of Q4
					of the write instruction. This is after the beginning of Q1 of the next instruction.

					A read of a port is latched at the end of Q1 of the current instruction.

					The display controller needs a up to 140ns to respond to the read.

					There is also a 150ns delay (max) between writing to the I/O port and the signal 
					showing up on the pin.

					So... 
					There needs to be a 150+140=290ns delay (worst case) from the write to the 
					PIC18 register that causes the display controller read clock going low, and 
					actually reading the data in the PIC18 register.  
					This is accomplished with 4 NOPs between the writing the RD control bit causing the 
					read-line going low and reading the data port. 

					Ref: See the PIC18F46J50 Family Data Sheet, sec 29, Fig. 29-6, Table 29-23.

************************************************************************************/

char OledReadData(void)
{
	char btemp;

	TRISD = 0xFF;				// set PORTD to input for the read
	oledD_C = 1;				// Set the control lines - the A0 line=data
	oledWR = 1;
	oledRD = 1;
	oledCS = 0;					// Select the Display Controller
			
	oledRD = 0;					// Toggle the RD line to read the data
	Nop(); Nop(); Nop(); Nop();	// Delay so the Read can complete
								// The controller doesn't respond to the read fast enough!
	btemp = PORTD;				// Read The result - The dummy read
	oledRD = 1;					// Toggle the RD line
								// the next read is for real
	oledRD = 0;	
	Nop(); Nop(); Nop(); Nop();	// Delay so the Read can complete
	btemp = PORTD;				// Read The result
	oledRD = 1;					// deassert the read control
	oledCS = 1;					// deselect the controller
	return(btemp);
								// leave PORTD as an input
}

		// The Read/Modify/Write is split into two functions so that it can easily used
		// to set or clear a bit or otherwise manipulate the byte that contains the address. 
char OledRMW_Read(void)			// This performs the Read of a Read/Modify/Write 
{									
	char	btemp;
								// the Column address & Page have already been set!
	WriteCommand( 0xE0 );		// Start a read/modify/write
								// The WriteCommand leaves TRISD=0xFF=input, so we don't have to do it.
	oledD_C = 1;				// Set the control lines for Data
	oledWR = 1;
	oledRD = 1;
	oledCS = 0;					// Select the Display Controller
	oledRD = 0;					// Toggle the RD line for the dummy read
	Nop(); Nop(); Nop(); Nop();	// Delay so the Read can complete
	btemp = PORTD;				// Read The result- this is the Dummy read!
								// just keep the CS active for the second real read
	oledRD = 1;					// Toggle the RD line
	oledRD = 0;					// Assert the read control
	Nop(); Nop(); Nop(); Nop();	// Delay so the Read can complete
	btemp = PORTD;				// The real read
	oledRD = 1;					// deassert the read control and end the read
								// CS is left active until the end of the RMW cycle
	return(btemp);				// return the byte read
}

// This is the Write/End of a Read/Modify/Write
void OledRMW_Write(char data)
{								// CS is still active from the OledRMW_Read function
								// The Write line is inactive=1
    TRISD = 0x00;				// Start the Write, make PortD = output.
    LATD  = data;				// output the new value
    oledWR = 0;					// toggle the Write line to re-write the data
    oledWR = 1;
								// we can leave the chip selected for the following END command
	WriteCommand( 0xEE );		// End the Read/Modify/Write;
								// the WriteCommand leaves the DisplayController=DeSelected
								// and PortD=Input
}

//**************************** 
// Plotting an X,Y pixel
//****************************
//  The inputs are all unsigned since Max X = 132
void PRGAWritePixel(BYTE X, BYTE Y, BYTE Data) 
{
	BYTE	oled_column;
	BYTE	this_data;

		// first the X coordinate
	oled_column = X + OFFSET;  						// add the column offset=2 from oled.h
		// first set the X coordinate: the column-low and high nibbles
	WriteCommand( oled_column & 0x0F );				// set the column: lower 4 bits
	WriteCommand( 0x10+((oled_column>>4)&0x0F) );	// set the column: upper 4 bits
	WriteCommand( 0xB0 | ((Y>>3)&0x07) );			// Set the Y page address
			
	this_data = OledRMW_Read();						// do the Read of the Read/Modify/Write

	if(Data)
		this_data = this_data | (0x01<<(Y&0x07)) ;	// set the bit
	else
		this_data = this_data & (!(0x01<<(Y&0x07))) ;	// clear the bit

 	OledRMW_Write( this_data );						// Write the bit and complete the RMW
 
	return;
}

//	This is an alternate way to organize the RMW cycle. It is not used here.
/****************************************************************************
void OledRMWDataOR(char data)		// This performa a Read/Modify/Write to OR in the data
{									// return the byte modified for debugging
	char	btemp;
								// the Column address & Page have already been set!
	WriteCommand( 0xE0 );		// Start a read/modify/write
			// The WriteCommand leaves TRISD=0xFF=input, so we don't have to do it.
	oledD_C = 1;				// Set the control lines for Data
	oledWR = 1;
	oledRD = 1;
	oledCS = 0;					// Select the Display Controller
	oledRD = 0;					// Toggle the RD line for the dummy read
	Nop(); Nop(); Nop(); Nop();	// Delay so the Read can complete
	btemp = PORTD;				// Read The result- this is the Dummy read!
								// just keep the CS active for the second real read
	oledRD = 1;					// Toggle the RD line
	oledRD = 0;
	Nop(); Nop(); Nop(); Nop();	// Delay so the Read can complete
	btemp = PORTD;				// The real read
	oledRD = 1;					// end the read

	btemp = btemp | data;		// merge the new data

    TRISD = 0x00;				// Start the Write, make PortD = output.
    LATD  = btemp;				// output the new value
    oledWR = 0;					// toggle the Write line to re-write the data
    oledWR = 1;
    							// we can leave the chip selected for the following END command
								// the WriteCommand will set up TRISD=0x00=Output anyway, so don't waste the instruction.
	WriteCommand( 0xEE );		// End the Read/Modify/Write;
								// the WriteCommand leaves the DisplayController=DeSelected
								//  and PortD=Input
}
*****************************************************************************/

// end of file
