/*****************************************************************************
 
                Microchip Memory Disk Drive File System
 
 *****************************************************************************
 FileName:        Demonstration2.c
 Dependencies:    FSIO.h
 Processor:       PIC24F
 Compiler:        C30
 Company:         Microchip Technology, Inc.

 Software License Agreement

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PICmicro® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PICmicro Microcontroller products. The
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


   Note:  This file is included to give you a basic demonstration of how the
           functions in this library work.  Prototypes for these functions,
           along with more information about them, can be found in FSIO.h
*****************************************************************************/

//DOM-IGNORE-BEGIN
/********************************************************************
 Change History:
  Rev            Description
  ----           -----------------------
  1.3.0          Initial Revision to support Long File Name Format
  1.3.4          Added support for PIC24FJ256DA210 Board.
                 Added support for PIC24FJ256GB110 PIM on Explorer 16 Board.
                 Removed 'COE_OFF' from _CONFIG1 settings.
                 Cleaned up the unnecessary part of main() function.
********************************************************************/
//DOM-IGNORE-END

/*******************************************************************************
//NOTE : ENABLE MACRO "SUPPORT_LFN" IN "FSconfig.h" FILE TO WORK WITH THIS DEMO.
         THIS DEMO IS INTENDED TO SHOW LONG FILE NAME ASCII OPERATIONS
********************************************************************************/

#include "FSIO.h"

char sendBuffer[] = "This is test string 1";
char send2[] = "2";
char receiveBuffer[50];

#if defined (__PIC24FJ128GA010__) || defined (__PIC24FJ256DA210__)
    _CONFIG2(IESO_OFF & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS)
    _CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx2 & FWDTEN_OFF)
#elif defined(__PIC24FJ256GB110__)
    _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
    _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_ON)
#endif

char path1[80];
int main (void)
{
   FSFILE * pointer;
   #if defined(SUPPORT_LFN)
   char count = 80;
   #endif
   char * pointer2;
   SearchRec rec;
   unsigned char attributes;
   unsigned char size = 0, i;

	// Turn on the secondary oscillator
	__asm__ ("MOV #OSCCON,w1");
	__asm__ ("MOV.b #0x02, w0");
	__asm__ ("MOV #0x46, w2");
	__asm__ ("MOV #0x57, w3");
	__asm__ ("MOV.b w2, [w1]");
	__asm__ ("MOV.b w3, [w1]");
	__asm__ ("MOV.b w0, [w1]");

	// Activate the RTCC module
	__asm__ ("mov #NVMKEY,W0");
	__asm__ ("mov #0x55,W1");
	__asm__ ("mov #0xAA,W2");
	__asm__ ("mov W1,[W0]");
	__asm__ ("nop");
	__asm__ ("mov W2,[W0]");
	__asm__ ("bset RCFGCAL,#13");
	__asm__ ("nop");
	__asm__ ("nop");
	RCFGCALbits.RTCPTR0 = 1;
	RCFGCALbits.RTCPTR1 = 1;
	// Set it to the correct time
	// These values won't be accurate
	RTCVAL = 0x0007;   
	RTCVAL = 0x0717;
	RTCVAL = 0x0208;
	RTCVAL = 0x2137;
	RCFGCAL = 0x8000;

	#if defined(__PIC24FJ256DA210__)

		// Make Analog Pins Digital
		ANSB = 0x0000 ; 
		ANSA = 0x0000;
		ANSC = 0x0000;
		ANSD = 0x0000;

		// Enable PLL
		CLKDIVbits.PLLEN = 1; 

		// Configure SPI1 PPS pins (ENC28J60/ENCX24J600/MRF24WB0M or other PICtail Plus cards)
		__builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

		RPOR1bits.RP2R = 8;       // assign RP2 for SCK1
		RPOR0bits.RP1R = 7;       // assign RP1 for SDO1
		RPINR20bits.SDI1R = 0;    // assign RP0 for SDI1

		__builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS                

	#elif defined(__PIC24FJ256GB110__)

		AD1PCFGL = 0xFFFF;

		//Initialize the SPI
		RPINR20bits.SDI1R = 23;
		RPOR7bits.RP15R = 7;
		RPOR0bits.RP0R = 8;    

		//enable a pull-up for the card detect, just in case the SD-Card isn't attached
		//  then lets have a pull-up to make sure we don't think it is there.
		CNPU5bits.CN68PUE = 1; 

	#endif

   while (!MDD_MediaDetect());

   // Initialize the library
   while (!FSInit());

#ifdef ALLOW_WRITES
   // Create a file
   pointer = FSfopen ("FILE1.TXT", "w");
   if (pointer == NULL)
      while(1);

   // Write 21 1-byte objects from sendBuffer into the file
   if (FSfwrite (sendBuffer, 1, 21, pointer) != 21)
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
   if (FSfwrite (send2, 1, 1, pointer) != 1)
      while(1);

   // Close the file
   if (FSfclose (pointer))
      while(1);

   // Create a second file
   pointer = FSfopen ("Microchip File 1.TXT", "w");
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
   pointer = FSfopen ("FILE1.TXT", "r");
   if (pointer == NULL)
      while(1);

   if (FSrename ("Microchip File 2.TXT", pointer))
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
   if (FSmkdir (".\\Mchp Directory 1\\Dir2\\Directory 3"))
      while(1);

   // Change to 'Directory 3' in our new tree
   if (FSchdir ("Mchp Directory 1\\Dir2\\Directory 3"))
      while(1);

   // Create another tree in 'Directory 3'
   if (FSmkdir ("Directory 4\\Directory 5\\Directory 6"))
      while(1);

   // Create a third file in directory THREE
   pointer = FSfopen ("CWD.TXT", "w");
   if (pointer == NULL)
      while(1);

   #if defined(SUPPORT_LFN)
   // Get the name of the current working directory
   /* it should be "\Microchip Directory 1\Dir2\Directory 3" */
   pointer2 = wFSgetcwd ((unsigned short int *)path1, count);
   #endif

   if (pointer2 != path1)
      while(1);

   // Simple string length calculation
   i = 0;
   while(*((unsigned short int *)path1 + i) != 0x00)
   {
      size++;
      size++;
      i++;
   }
   // Write the name to CWD.TXT
   if (FSfwrite (path1, size, 1, pointer) != 1)
      while(1);

   // Close the file
   if (FSfclose (pointer))
      while(1);

   // Create some more directories
   if (FSmkdir ("Directory 4\\Directory 5\\Directory 7\\..\\Directory 8\\..\\..\\DIRNINE\\Directory 10\\..\\Directory 11\\..\\Directory 12"))
      while(1);

   /*******************************************************************
      Now our tree looks like this

 \ -> Mchp Directory 1 -> Dir2 -> Directory 3 -> Directory 4 -> Directory 5 -> Directory 6
                                                                                 -> Directory 7
                                                                                 -> Directory 8

                                                                    DIRNINE -> Directory 10
                                                                            -> Directory 11
                                                                            -> Directory 12
   ********************************************************************/

   // This will delete only Directory 8
   // If we tried to delete Directory 5 with this call, the FSrmdir
   // function would return -1, since Directory 5 is non-empty
   if (FSrmdir ("\\Mchp Directory 1\\Dir2\\Directory 3\\Directory 4\\Directory 5\\Directory 8", FALSE))
      while(1);

   // This will delete DIRNINE and all three of its sub-directories
   if (FSrmdir ("Directory 4\\DIRNINE", TRUE))
      while(1);

   // Change directory to the root dir
   if (FSchdir ("\\"))
      while(1);
#endif

#ifdef ALLOW_FILESEARCH
   // Set attributes
   attributes = ATTR_ARCHIVE | ATTR_READ_ONLY | ATTR_HIDDEN;

   // Functions "FindFirst" & "FindNext" can be used to find files
   // and directories with required attributes in the current working directory.

   // Find the first TXT file with any (or none) of those attributes that
   // has a name beginning with the letters "Mic"
   // These functions are more useful for finding out which files are
   // in your current working directory
   if (FindFirst ("Mic*.TXT", attributes, &rec))
      while(1);

 //   Find file to get "Microchip File 2.TXT"
      if (FindNext (&rec))
         while(1);

   #if defined(SUPPORT_LFN) 
  // Delete file 2
   // If the file name is long
   if(rec.utf16LFNfoundLength)
   {
      // NOTE : "wFSremove" function deletes specific file not directory.
      //        To delete directories use "wFSrmdir" function
      if (wFSremove (rec.utf16LFNfound))
         while(1);
   }
   else
   #endif
   {
      // NOTE : "FSremove" function deletes specific file not directory.
      //        To delete directories use "FSrmdir" function
      if (FSremove (rec.filename))
         while(1);
   }

#endif


/*********************************************************************
   The final contents of our card should look like this:
   \ -> Microchip File 1.TXT
      -> Mchp Directory 1 -> Dir2 -> Directory 3 -> CWD.TXT
                                                 -> Directory 4 -> Directory 5 -> Directory 6
                                                                               -> Directory 7

*********************************************************************/


   while(1);
}








