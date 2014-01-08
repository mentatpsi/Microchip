/*****************************************************************************
 
                Microchip Memory Disk Drive File System
 
 *****************************************************************************
 FileName:        Demonstration2.c
 Dependencies:    FSIO.h
 Processor:       dsPIC33E, PIC24E
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
  1.3.4   		Initial Revision to support Long File Name Format
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

_FOSCSEL(FNOSC_FRC);			
_FOSC(FCKSM_CSECMD & POSCMD_XT & OSCIOFNC_OFF & IOL1WAY_OFF);
_FWDT(FWDTEN_OFF);
_FPOR(FPWRT_PWR128 & BOREN_ON & ALTI2C1_ON & ALTI2C2_ON);
_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF);

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

    // Initialize and configure Primary PLL, and enabled Secondary Oscillator
    PLLFBD = 58;			/* M  = 60	*/
    CLKDIVbits.PLLPOST = 0;	/* N1 = 2	*/
    CLKDIVbits.PLLPRE = 0;	/* N2 = 2	*/
    OSCTUN = 0;
    __builtin_write_OSCCONH(0x03);		
    __builtin_write_OSCCONL(0x03);
    while (OSCCONbits.COSC != 0x3); 
    while (_LOCK == 0);

   // Activate the RTCC module
   __builtin_write_RTCWEN();
   Nop();
   Nop();
   RCFGCALbits.RTCPTR0 = 1;
   RCFGCALbits.RTCPTR1 = 1;
   
   // Set it to the correct time
   // These values won't be accurate
   RTCVAL = 0x0011;   
   RTCVAL = 0x0815;
   RTCVAL = 0x0108;
   RTCVAL = 0x2137;
   RCFGCAL = 0x8000;

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
   /* it should be "\Mchp Directory 1\Dir2\Directory 3" */
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








