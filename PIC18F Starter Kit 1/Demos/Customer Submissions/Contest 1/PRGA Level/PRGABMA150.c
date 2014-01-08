/*************************************************
	PRGABMA150.c - Additional driver functions for MicroChip Demo XL Chip

	Author: J. W. Hartley
	Revision:	13-Apr-10	Initial Edition
				01-Jun-10	Additional comments, clean-up of debugging code

***************************************************/

#include "Compiler.h"			// Some compiler stuff
#include "GenericTypeDefs.h"	// General data types
#include "BMA150.h"				// Use the provided bit definitions

#include "PRGABMA150.h"		// prototypes for these functions

/********************************************

	Setup of XL Chip for PRGA Microchip PIC18F Starter Board Level Demo.

	This setup up the BMA150 accelerometer for a range of +/-2g and
	and a bandwidth of 25Hz. We want as much sensitivity as possible and
	a narrow bandwidth to minimize the noise. 

***************************************************/

void PRGAInitBma150 (void)		// Set up XL chip for PRGA Level Demo
{								// This is called AFTER the MicroChip InitBma150()
	WORD i;
	BMA150_REG reg;

/*************************************** 
	Bits 5, 6 and 7 of register addresses 14h and 34h do contain critical sensor individual 
	calibration data which must not be changed or deleted by any means. 
 
	In order to properly modify addresses 14h and/or 34h for range and/or bandwidth selection 
	using bits 0, 1, 2, 3 and 4, it is highly recommended to read-out the complete byte, perform bit-
	slicing and write back the complete byte with unchanged bits 5, 6 and 7.  
*****************************************/ 
 
	reg.val = BMA150_ReadByte(BMA150_ADDR14);	// get the current value so that we can replace bits 5-7
												// the follow are bit fields defined in BMA150.h
												// so we don't change the calibration bits.
	reg.range =BMA150_RANGE_2G;					// Set range to +/-2g
	reg.bandwidth = BMA150_BW_25;				// Set Bandwidth to 25 Hz 
    BMA150_WriteByte(BMA150_ADDR14,reg.val);	// rewrite to update the values
}

/* End of file - PRGA_BMA150.c */
 