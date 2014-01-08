/******************************************************************************
 *
 *               Microchip Memory Disk Drive File System
 *
 ******************************************************************************
 * FileName:        Demonstration.c
 * Dependencies:    FSIO.h
 * Processor:       PIC18
 * Compiler:        C18
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the ï¿½Companyï¿½) for its PICmicroï¿½ Microcontroller is intended and
 * supplied to you, the Companyï¿½s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN ï¿½AS ISï¿½ CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
*****************************************************************************/


/*****************************************************************************
	Note:  This file is included to give you a basic demonstration of how the
           functions in this library work.  Prototypes for these functions,
           along with more information about them, can be found in FSIO.h
*****************************************************************************/

//DOM-IGNORE-BEGIN
/********************************************************************
 Change History:
  Rev            Description
  ----           -----------------------
  1.2.4 - 1.2.6  No Change
  1.2.6          Add support for the PIC18F46J50_PIM
  1.3.4          Added support for PIC18F8722 on PIC18 Explorer Board
********************************************************************/
//DOM-IGNORE-END


#include "FSIO.h"


#if defined(PIC18F87J50_PIM)				// Configuration bits for PIC18F87J50 FS USB Plug-In Module board
        #pragma config XINST    = OFF   	// Extended instruction set
        #pragma config STVREN   = ON      	// Stack overflow reset
        #pragma config PLLDIV   = 3         // (12 MHz crystal used on this board)
        #pragma config WDTEN    = OFF      	// Watch Dog Timer (WDT)
        #pragma config CP0      = OFF      	// Code protect
        #pragma config CPUDIV   = OSC1      // OSC1 = divide by 1 mode
        #pragma config IESO     = OFF      	// Internal External (clock) Switchover
        #pragma config FCMEN    = OFF      	// Fail Safe Clock Monitor
        #pragma config FOSC     = HSPLL     // Firmware must also set OSCTUNE<PLLEN> to start PLL!
        #pragma config WDTPS    = 32768
//      #pragma config WAIT     = OFF      	// Commented choices are
//      #pragma config BW       = 16      	// only available on the
//      #pragma config MODE     = MM      	// 80 pin devices in the 
//      #pragma config EASHFT   = OFF      	// family.
        #pragma config MSSPMSK  = MSK5
//      #pragma config PMPMX    = DEFAULT
//      #pragma config ECCPMX   = DEFAULT
        #pragma config CCP2MX   = DEFAULT   
#elif defined(PIC18F46J50_PIM)
     #pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
     #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
     #pragma config STVREN = ON            //stack overflow/underflow reset enabled
     #pragma config XINST = OFF          //Extended instruction set disabled
     #pragma config CPUDIV = OSC1        //No CPU system clock divide
     #pragma config CP0 = OFF            //Program memory is not code-protected
     #pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
     #pragma config T1DIG = ON           //Sec Osc clock source may be selected
     #pragma config LPT1OSC = OFF        //high power Timer1 mode
     #pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
     #pragma config IESO = OFF           //Two-Speed Start-up disabled
     #pragma config WDTPS = 32768        //1:32768
     #pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
     #pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
     #pragma config DSWDTEN = OFF        //Disabled
     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
     #pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
     #pragma config WPEND = PAGE_0       //Start protection at page 0
     #pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored 
#elif defined(__18F8722)
	#pragma config OSC=HSPLL, FCMEN=OFF, IESO=OFF, PWRT=OFF, WDT=OFF, LVP=OFF, XINST=OFF
#else
#endif

char sendBuffer[22] = "This is test string 1";
char send2[2] = "2";
char receiveBuffer[50];

char dirname1[16] = ".\\ONE\\TWO\\THREE";
char dirname2[14] = "ONE\\TWO\\THREE";
char dirname3[14] = "FOUR\\FIVE\\SIX";
char dirname4[60] = "FOUR\\FIVE\\SEVEN\\..\\EIGHT\\..\\..\\NINE\\TEN\\..\\ELEVEN\\..\\TWELVE";
char dirname5[31] = "\\ONE\\TWO\\THREE\\FOUR\\FIVE\\EIGHT";
char dirname6[10] = "FOUR\\NINE";
char dirname7[2];

void main (void)
{
	FSFILE * pointer;
	char path[30];
	char count = 30;
	char * pointer2;
	SearchRec rec;
	unsigned char attributes;
	unsigned char size = 0, i;

    #if (defined(__18CXX) & !defined(PIC18F87J50_PIM)) || defined(__18F8722)
        ADCON1 |= 0x0F;                 // Default all pins to digital
    #elif !defined(PIC18F87J50_PIM)
        AD1PCFG = 0xFFFF;
    #endif
    #if defined(PIC18F87J50_PIM)
    WDTCONbits.ADSHR = 1;			// Select alternate SFR location to access ANCONx registers
    ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital
    WDTCONbits.ADSHR = 0;			// Select normal SFR locations
    #endif
    #if defined(PIC18F46J50_PIM)
    ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital
    #endif
    //********* Initialize Peripheral Pin Select (PPS) *************************
    //  This section only pertains to devices that have the PPS capabilities.
    //    When migrating code into an application, please verify that the PPS
    //    setting is correct for the port pins that are used in the application.
    #if defined(PIC18F46J50_PIM)
    RPINR21 = 1;   //SDI = RP1
    RPOR4 = 10;    //RP4 = SCK
    RPOR2 = 9;     //RP2 = SDO
    RPINR22 = 4;   //SCK = RP4

    //enable a pull-up for the card detect, just in case the SD-Card isn't attached
    //  then lets have a pull-up to make sure we don't think it is there.
    INTCON2bits.RBPU = 0; 

    #endif
    while (!MDD_MediaDetect());

	// Initialize the library
	while (!FSInit());

#ifdef ALLOW_WRITES
	// Set the clock value
	// This will determine the create time for the file we're about to make
	// This will set the time and date to 3:05:26 PM on July 27, 2007.
	if (SetClockVars (2007, 7, 27, 15, 5, 26))
		while(1);

	// Create a file
	pointer = FSfopenpgm ("FILE3.TXT", "w");
	if (pointer == NULL)
		while(1);

	// Write 21 1-byte objects from sendBuffer into the file
	if (FSfwrite ((void *)sendBuffer, 1, 21, pointer) != 21)
		while(1);

	// FSftell returns the file's current position
	if (FSftell (pointer) != 21)
		while(1);

	// FSfseek sets the position one byte before the end
	// It can also set the position of a file forward from the
	// beginning or forward from the current position
	if (FSfseek(pointer, 1, SEEK_END))
		while(1);

	// Write a 2 at the end of the string
	if (FSfwrite ((void*)send2, 1, 1, pointer) != 1)
		while(1);

	// Set the time again
	// When called before fclose, this will determine the last time
	// accessed and modified.  This time will be 4 seconds after the last one.
	if (SetClockVars (2007, 7, 27, 15, 5, 30))
		while(1);

	// Close the file
	if (FSfclose (pointer))
		while(1);

	// Set the clock again
	// This time is the last one possible with the FAT file system
	// 11:59:59 PM, December 31, 2106.
	if (SetClockVars (2107, 12, 31, 23, 59, 59))
		while(1);

	// Create a second file
	pointer = FSfopenpgm ("FILE1.TXT", "w");
	if (pointer == NULL)
		while(1);

	// Write the string to it again
	if (FSfwrite ((void *)sendBuffer, 1, 21, pointer) != 21)
		while(1);

	// Close the file
	if (FSfclose (pointer))
		while(1);
#endif

	// Open file 1 in read mode
	pointer = FSfopenpgm ("FILE3.TXT", "r");
	if (pointer == NULL)
		while(1);

   if (FSrenamepgm ("FILE2.TXT", pointer))
      while(1);

	// Read one four-byte object
	if (FSfread (receiveBuffer, 4, 1, pointer) != 1)
		while(1);

	// Check if this is the end of the file- it shouldn't be
	if (FSfeof (pointer))
		while(1);

	// Close the file
	if (FSfclose (pointer))
		while(1);

	// Make sure we read correctly
	if ((receiveBuffer[0] != 'T') || 
			(receiveBuffer[1] != 'h')  || 
			(receiveBuffer[2] != 'i')  || 
			(receiveBuffer[3] != 's'))
	{
		while(1);
	}

#ifdef ALLOW_DIRS
	// Create a small directory tree
	// Beginning the path string with a '.' will create the tree in
	// the current directory.  Beginning with a '..' would create the
	// tree in the previous directory.  Beginning with just a '\' would 
	// create the tree in the root directory.  Beginning with a dir name
	// would also create the tree in the current directory
	if (FSmkdir (dirname1))
		while(1);

	// Change to directory THREE in our new tree
	if (FSchdir (dirname2))
		while(1);

	// Create another tree in directory THREE
	if (FSmkdir (dirname3))
		while(1);

	// Create a third file in directory THREE
	pointer = FSfopenpgm ("FILE3.TXT", "w");
	if (pointer == NULL)
		while(1);

	// Get the name of the current working directory
	/* it should be "\ONE\TWO\THREE"       */
	pointer2 = FSgetcwd (path, count);
	if (pointer2 != path)
		while(1);

	// Simple string length calculation
	i = 0;
	while(*(path + i) != 0x00)
	{
		size++;
		i++;
	}
	// Write the name to FILE3.TXT
	if (FSfwrite ((void *)path, size, 1, pointer) != 1)
		while(1);

	// Close the file
	if (FSfclose (pointer))
		while(1);

	// Create some more directories
	if (FSmkdir (dirname4))
		while(1);

	/*******************************************************************
		Now our tree looks like this

		\ -> ONE -> TWO -> THREE -> FOUR -> FIVE -> SIX
                                                 -> SEVEN
                                                 -> EIGHT
                                            NINE -> TEN
                                                 -> ELEVEN
                                                 -> TWELVE
	********************************************************************/

	// This will delete only directory eight
	// If we tried to delete directory FIVE with this call, the FSrmdir
	// function would return -1, since FIVE is non-empty
	if (FSrmdir (dirname5, FALSE))
		while(1);

	// This will delete directory NINE and all three of its sub-directories
	if (FSrmdir (dirname6, TRUE))
		while(1);

	// You can't initialize an array in PIC18 to just a backslash
	// Initialize it manually
	dirname7[0] = '\\';
	dirname7[1] = 0;
	// Change directory to the root dir
	if (FSchdir (dirname7))
		while(1);

#endif

#ifdef ALLOW_FILESEARCH
	// Set attributes
	attributes = ATTR_ARCHIVE | ATTR_READ_ONLY | ATTR_HIDDEN;

   // Functions "FindFirstpgm" & "FindNext" can be used to find files
   // and directories with required attributes in the current working directory.

	// Find the first TXT file with any (or none) of those attributes that
	// has a name beginning with the letters "FILE"
	// These functions are more useful for finding out which files are 
	// in your current working directory
	if (FindFirstpgm ("FILE*.TXT", attributes, &rec))
		while(1);

	// Keep finding files until we get FILE2.TXT
	while(rec.filename[4] != '2')
	{
		if (FindNext (&rec))
			while(1);
	}

	// Delete file 2.
	// NOTE : "FSremove" function deletes specific file not directory.
	//        To delete directories use "FSrmdir" function
	if (FSremove (rec.filename))
		while(1);
#endif

/*********************************************************************
	The final contents of our card should look like this:
	\ -> FILE1.TXT
      -> ONE       -> TWO -> THREE -> FILE3.TXT
                                   -> FOUR      -> FIVE -> SIX
                                                        -> SEVEN

*********************************************************************/


	while(1);
}








