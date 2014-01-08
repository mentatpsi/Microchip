  /*
********************************************************************************
                                                                                
Software License Agreement                                                      
                                                                                
Copyright © 2008 Microchip Technology Inc. and its licensors.  All         
rights reserved.                                                                
                                                                                
Microchip licenses to you the right to: (1) install Software on a single        
computer and use the Software with Microchip 16-bit microcontrollers and        
16-bit digital signal controllers ("Microchip Product"); and (2) at your        
own discretion and risk, use, modify, copy and distribute the device            
driver files of the Software that are provided to you in Source Code;           
provided that such Device Drivers are only used with Microchip Products         
and that no open source or free software is incorporated into the Device        
Drivers without Microchip's prior written consent in each instance.             
                                                                                
You should refer to the license agreement accompanying this Software for        
additional information regarding your rights and obligations.                   
                                                                                
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY         
KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY              
WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A          
PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE             
LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY,               
CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY           
DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY         
INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR         
LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,                 
SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY         
DEFENSE THEREOF), OR OTHER SIMILAR COSTS.                                       
                                                                                
********************************************************************************
*/

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "boot.h"
#include "MDD File System/FSIO.h"

#include "oled.h"

#include "mtouch.h"

#include "soft_start.h"

#define ROM_STRING rom unsigned char*

#if defined (PIC24FJ256GB110_PIM) 
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( 0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_OFF)

#elif defined(PIC18_STARTER_KIT)
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
     #pragma config RTCOSC = INTOSCREF//T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
     #pragma config DSWDTEN = OFF        //Disabled
     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
     #pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
     #pragma config WPEND = PAGE_0       //Start protection at page 0
     #pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored 

#endif

#if defined(__18CXX)
	#pragma code High_ISR = 0x08
	void Remapped_High_ISR (void)
	{
	     _asm goto APPLICATION_HIGH_ISR _endasm
	}
	#pragma code Low_ISR = 0x18
	void Remapped_Low_ISR (void)
	{
	     _asm goto APPLICATION_LOW_ISR _endasm
	}
#endif
/*
*******************************************************************************
Macro used to call main application
*******************************************************************************
*/

/******************************************************************************
  Function:
    int BootApplication ( void )

  Description:
    This macro is used to launch the application.

  Precondition:
    The application image must be correctly programmed into Flash at the 
    appropriate entry point.

  Parameters:
    None

  Returns:
    This call does not normally return.

  Remarks:
    The application's entry point is defined by the APPLICATION_ADDRESS
    macro in the boot_config.h header file.
******************************************************************************/

#define BootApplication()       (((int(*)(void))(APPLICATION_ADDRESS))())

#define FILE_FETCH		0
#define SCREEN_UPDATE	1
#define USER_INPUT		2

/******************************************************************************
  Function:
    void LoadApplication ( void )

  Description:
    This routine attempts to initialize and attach to the boot medium, locate
    the boot image file, and load and program it to the flash.

  Precondition:
    The boot loader IO must have been initialized.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
******************************************************************************/
void LoadApplication ( void )
{
    BOOL    LoadingApplication      = TRUE;

    BOOL    BootMediumAttached      = FALSE;
	BOOL	BootMediumInitialized    = FALSE;

    BOOL    BootImageFileFound      = FALSE;
    BOOL    BootImageFileError      = FALSE;

	SearchRec   searchRecord;
	int		status = -1;
	char	state;
	int		button1 = 0;
	int		button2 = 0;

	static char fileList[416];
	int filesCounter;
	char lineOffset;
	char lineSelect;
	char i;

    static unsigned char first_file_index;

	TRISCbits.TRISC7 = 0;
	LATCbits.LATC7 = 1;						 // deselect the BMA150

    ResetDevice();
	FillDisplay(0x00);

    MDD_InitIO();

    /* Loader main loop */
    while (LoadingApplication)
    {
        // Check for the boot medium to attach
        BootMediumAttached = BLMedia_MediumAttached();
		
        if (BootMediumAttached)
        {
			/* The boot media is attached; check if it is also initialized */
			if (BootMediumInitialized)
			{
				/* Media attashed and initialized */

				/* Initialize the mTouch library */
				mTouchInit();
			
				/* Calibrate the mTouch library */
				mTouchCalibrate();

			
                BootImageFileError = FALSE;

				state = FILE_FETCH;

				while (BootImageFileError == FALSE)
				{
					switch (state)
					{
						case FILE_FETCH:
							{
								/* Fetch the file list on the current directory */					
								filesCounter = 0;
								status = FindFirstpgm("*.*", ATTR_DIRECTORY, &searchRecord);
								if (status == 0)
								{	
									/* If a file was found, add it to the file list and continue to search */
									strcpy((char*)&fileList[filesCounter*sizeof(searchRecord.filename)],(const char*) &searchRecord.filename);
									filesCounter ++;
			
									while ((status == 0) && (i <= 31))						
									{
										status = FindNext(&searchRecord);
										if (status == 0)
										{
											strcpy((char*)&fileList[filesCounter*sizeof(searchRecord.filename)], (const char*)&searchRecord.filename);
											filesCounter++;
										}
										else
										{
											/* We hit the end of the current directory */
										}
									}
								}

                                first_file_index = filesCounter;

								status = FindFirstpgm("*.HEX", ATTR_ARCHIVE | ATTR_READ_ONLY | ATTR_HIDDEN, &searchRecord);
								if (status == 0)
								{	
									/* If a file was found, add it to the file list and continue to search */
									strcpy((char*)&fileList[filesCounter*sizeof(searchRecord.filename)], (const char*)&searchRecord.filename);
									filesCounter ++;
			
									while ((status == 0) && (i <= 64))						
									{
										status = FindNext(&searchRecord);
										if (status == 0)
										{
											strcpy((char*)&fileList[filesCounter*sizeof(searchRecord.filename)], (const char*)&searchRecord.filename);
											filesCounter ++;
										}
										else
										{
											/* We hit the end of the current directory */
										}
									}
								}
								if (filesCounter != 0)
								{
									/* At least one file or folder was found */
									state = SCREEN_UPDATE;
									lineSelect = 0;
									lineOffset = 0;
								}
								else
								{
			                        BootImageFileError = TRUE;
								}
								break;				
							}

						case USER_INPUT:
							{
								/* Read the R button - RA0 */
								button1 = mTouchReadButton(0);
								if (button1 < 600)
								{
									/* wait for the button to be released */
									while (button1 < 700)
									{
										button1 = mTouchReadButton(0);
									}

									/* This is a particular case of ".." directory */									
									if (fileList[(lineOffset+lineSelect)*sizeof(searchRecord.filename)] == 0x2E)
									{
										if (fileList[(lineOffset+lineSelect)*sizeof(searchRecord.filename)+1] == 0x2E)
										{
											status = FSchdirpgm("..");
											if (status == 0)
											{	
												state = FILE_FETCH;
											}
											else
											{
						                         BootImageFileError = TRUE;
											}
										}
									}
									else
									{
										status = FindFirst(&fileList[(lineOffset+lineSelect)*sizeof(searchRecord.filename)], ATTR_DIRECTORY | ATTR_ARCHIVE | ATTR_READ_ONLY | ATTR_HIDDEN, &searchRecord);
										if (status == 0)
										{	
											if (searchRecord.attributes & ATTR_DIRECTORY)
											{
												/* this is a folder */
												FSchdir(searchRecord.filename);
												state = FILE_FETCH;
											}
											else
											{
												FillDisplay(0x00);
												oledPutROMString((ROM_STRING)" The following demo  ", 0, 0);
												oledPutROMString((ROM_STRING)" will be loaded:     ", 1, 0);
												
                                                oledPutString((unsigned char *)searchRecord.filename, 2, 20);
	
												oledPutROMString((ROM_STRING)" Press the R button  ", 4, 0); 
												oledPutROMString((ROM_STRING)" to accept, or the  ", 5, 0);
												oledPutROMString((ROM_STRING)" L button to cancel ", 6, 0);
	
												/* Read the R button - RA0 */
												button1 = mTouchReadButton(0);
												/* Read the L button - RA3 */
												button2 = mTouchReadButton(3);

												/* wait for a button to be pressed */
												while ((button1 > 600) && (button2 > 600) )
												{
													button1 = mTouchReadButton(0);
													button2 = mTouchReadButton(3);
												}
												/* Check if the R button was pressed */
												if (button1 < 700)
												{
													/* wait for the button to be released */
													while (button1 < 700)
													{
														button1 = mTouchReadButton(0);
													}

													FillDisplay(0x00);
													oledPutROMString((ROM_STRING)"File is loading.     ", 0, 0);
													oledPutROMString((ROM_STRING)"   Please wait.", 1, 0);

													// Read the boot image file and program it to Flash
													if (BLMedia_LoadFile(searchRecord.filename))
													{
													    LoadingApplication = FALSE;
													    BLIO_ReportBootStatus(BL_PROGRAMMED, "BL: Application image has been loaded.\r\n");

														FillDisplay(0x00);
														oledPutROMString((ROM_STRING)"  Application image  ", 0, 0);
														oledPutROMString((ROM_STRING)" file:      ", 1, 0);
														oledPutString((unsigned char *)searchRecord.filename, 1, 40);
														oledPutROMString((ROM_STRING)" has been loaded.    ", 2, 0);
														oledPutROMString((ROM_STRING)"Press the R button to", 4, 0);
														oledPutROMString((ROM_STRING)"start the application", 5, 0);
														oledPutROMString((ROM_STRING)"or L button to cancel", 6, 0);

														/* Read the R button - RA0 */
														button1 = mTouchReadButton(0);
														/* Read the L button - RA3 */
														button2 = mTouchReadButton(3);
		
														/* wait for a button to be pressed */
														while ((button1 > 600) && (button2 > 600) )
														{
															button1 = mTouchReadButton(0);
															button2 = mTouchReadButton(3);
														}
														/* Check if the R button was pressed */
														if (button1 < 700)
														{
															/* wait for the button to be released */
															while (button1 < 700)
															{
																button1 = mTouchReadButton(0);
															}
															FillDisplay(0x00);
															BootApplication();
														}
														/* Check if the L button was pressed */
														if (button2 < 700)
														{
															/* wait for the button to be released */
															while (button2 < 700)
															{
																button2 = mTouchReadButton(3);
															}
															state = FILE_FETCH;
														}
								                     }
								                     else
								                     {
								                         // Error reported by lower layer
								                         BootImageFileError = TRUE;
														/* Refresh the file list */
														state = FILE_FETCH;

														//FillDisplay(0x00);
														oledPutROMString((ROM_STRING)"  An error occurred ", 0, 0);
														oledPutROMString((ROM_STRING)" while loading image", 1, 0);
														oledPutROMString((ROM_STRING)" file:      ", 2, 0);
														oledPutString((unsigned char *)searchRecord.filename, 2, 40);
														oledPutROMString((ROM_STRING)"  Press any key to   ", 4, 0);
														oledPutROMString((ROM_STRING)"load a different file", 5, 0);

														/* Read the R button - RA0 */
														button1 = mTouchReadButton(0);
														/* Read the L button - RA3 */
														button2 = mTouchReadButton(3);
														/* wait for a button to be pressed */
														while ((button1 > 600) && (button2 > 600) )
														{
															button1 = mTouchReadButton(0);
															button2 = mTouchReadButton(3);
														}
														/* wait for the button to be released */
														while ((button1 < 700) || (button2 < 700))
														{
															button1 = mTouchReadButton(0);
															button2 = mTouchReadButton(3);
														}
														state = FILE_FETCH;
								                     }
												}

												/* Check if the L button was pressed */
												if (button2 < 700)
												{
													/* wait for the button to be released */
													while (button2 < 700)
													{
														button2 = mTouchReadButton(3);
													}
													state = FILE_FETCH;
												}

											}
										}
										else
										{
					                         BootImageFileError = TRUE;
										}
									}
								}

								/* Read the L button - RA3 */
								button1 = mTouchReadButton(3);
								if (button1 < 600)
								{
									/* wait for the button to be released */
									while (button1 < 700)
									{
										button1 = mTouchReadButton(3);
									}
									status = FSchdirpgm("..");
									if (status == 0)
									{	
										state = FILE_FETCH;
									}
									else
									{
										/* The Cancel button was pressed in the root directory: Restart */
									    SD_CS = 1;
										FillDisplay(0x00);
				                        _asm
											GOTO 0x0000
										_endasm
									}
								}
	
								/* Scroll bar - RA1/RA2*/
								button1 = mTouchReadButton(1);
								button2 = mTouchReadButton(2);
								if ((button1 < 700) || (button2 < 700))
								{
									if (button1 < button2)
									{
										lineSelect--;
									}
									else
									{
										lineSelect++;
									}
									/* wait for the slider to be released */
									while ((button1 < 750) || (button2 < 750))
									{
										button1 = mTouchReadButton(1);
										button2 = mTouchReadButton(2);
									}

									state = SCREEN_UPDATE;
								}
								break;							
							}

						case SCREEN_UPDATE:
							{
									if (lineSelect < 0)
									{
										lineSelect = 0;
										lineOffset--;
									}
									if (lineSelect == 7)
									{
										lineSelect = 6;
										lineOffset++;
									}
									if (lineOffset + 7 > filesCounter)
									{
										lineOffset --;
									}
									if (lineOffset < 0)
									{
										lineOffset = 0;
									}

									/* Clear the display */
									FillDisplay(0x00);
									for (i = 0; i <7; i++)
									{
										if ((lineOffset+i) < filesCounter)
										{
                                            BOOL invert_line;

        									if (lineOffset+lineSelect >= filesCounter)
        									{
                                                lineSelect--;
        									}

                                            if(i!=lineSelect)
                                            {
                                                invert_line = FALSE;
                                            }
                                            else
                                            {
                                                invert_line = TRUE;
                                            } 

                                            if(((i+lineOffset) < first_file_index) & (first_file_index != 0))
                                            {
                                                oledPutROMString(folder_image, i, 0);
                                            }					    
                                            oledPutStringInvertOption((unsigned char *)&fileList[(lineOffset+i)*sizeof(searchRecord.filename)], i, 15, invert_line);
										}
									}

								state = USER_INPUT;
								break;
							}
						default:
							{
								state = FILE_FETCH;
								break;
							}
					}
				}
			    /* Error occurred while working with files on SD card */
				/* ... or neither a folder or a hex file found on the card */
				BootMediumInitialized = FALSE;
				//FillDisplay(0x00);
				oledPutROMString((ROM_STRING)"                     ", 0, 0);
				oledPutROMString((ROM_STRING)" Error occurred while", 1, 0);
				oledPutROMString((ROM_STRING)"working with files on", 2, 0);
				oledPutROMString((ROM_STRING)"the micro SD card, or", 3, 0);
				oledPutROMString((ROM_STRING)"neither a folder or a", 4, 0);
				oledPutROMString((ROM_STRING)"hex file found.      ", 5, 0);
				oledPutROMString((ROM_STRING)"                     ", 6, 0);
			}
			else
			{
				TRISCbits.TRISC7 = 0;
				LATCbits.LATC7 = 1;						 // deselect the BMA150

				/* Initialize the boot media */
	            BootMediumInitialized = BLMedia_InitializeTransport();

				PORTCbits.RC7 = 1;						 // deselect the BMA150

			    SD_CS = 1;

	            if (BootMediumInitialized)
	            {
	                BLIO_ReportBootStatus(BL_MEDIUM_INIT, "BL: Media initialized\r\n");
	            }
			}
		}
		else
		{
		    /* Media was disconnected */
			BootMediumInitialized = FALSE;
			//FillDisplay(0x00);
			oledPutROMString((ROM_STRING)"                     ", 0, 0);
			oledPutROMString((ROM_STRING)" Card not detected!  ", 1, 0);
			oledPutROMString((ROM_STRING)"                     ", 2, 0);
			oledPutROMString((ROM_STRING)"Please insert a valid", 3, 0);
			oledPutROMString((ROM_STRING)"    MicroSD card to  ", 4, 0);
			oledPutROMString((ROM_STRING)"      continue       ", 5, 0);
			oledPutROMString((ROM_STRING)"                     ", 6, 0);
		}

        /* Watch for user to abort the load */
        if (BLIO_AbortLoad())
        {
            LoadingApplication = FALSE;
        }
	}
}


/******************************************************************************
  Function:
    int main(void)

  Description:
    This is the boot loader's main C-language entry point.  It initializes 
    the boot loader's IO, and uses it to determine if the boot loader should
    be invoked.  If so, it attempts to load the application.  After loading
    and programming the boot image (or immediately, if the boot loader is
    not invoked), it checks the to see if the image in Flash is valid and, 
    if so, calls the application's main entry point.

  Precondition:
    The appropriate startup code must have been executed.

  Parameters:
    None

  Returns:
    Integer exit code (0)

  Remarks:
    This routine is executed only once, after a reset.
******************************************************************************/

#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{
	unsigned long i;
    unsigned char j;
    unsigned char count[22];

    Nop();

    while (1)
    {
	    while(AppPowerReady() == FALSE)
		{
			// Soft Start the APP_VDD	
		}

	    /* Initialize the boot loader IO */
	    BLIO_InitializeIO();
	    BLIO_ReportBootStatus(BL_RESET, "BL: ***** Reset *****\r\n");

	    /* Initialize the oled display */
	    ResetDevice();

		FillDisplay(0xFF);

		/* Flash the screen white and black for Production Tests. */
		for(i = 0; i <200000; i++);
		FillDisplay(0x00);
		for(i = 0; i <200000; i++);

		oledPutROMString((ROM_STRING)"      Microchip      ", 0, 0);
		oledPutROMString((ROM_STRING)"  PIC18F Starter Kit ", 1, 0);
		oledPutROMString((ROM_STRING)"    Bootloader v1.3  ", 1, 0);
		oledPutROMString((ROM_STRING)"Press the menu button", 3, 0);
		oledPutROMString((ROM_STRING)" to load a new demo. ", 4, 0);

        for(j=0;j<sizeof(count);j++)
        {
            count[j]=' ';
        }

        count[21] = 0x00;

		j = 0x39;
		while (j > 0x30 )
		{
			i = 0;

            count[10] = j;
            oledPutString((unsigned char*)(count), 6, 0);

			while (i < 300000)
			{
		    	/* Check to see if the user requested loading of a new application */
			   	if (BLIO_LoaderEnabled() == TRUE)
			    {
			        BLIO_ReportBootStatus(BL_LOADING, "BL: Loading new application image\r\n");

                    for(i=0;i<100000;i++){} //crude debounce
                    while(BLIO_LoaderEnabled() == TRUE){}		

            		oledPutROMString((ROM_STRING)"Use 'up' and 'down'  ", 0, 0);
            		oledPutROMString((ROM_STRING)" buttons to navigate.", 1, 0);
            		oledPutROMString((ROM_STRING)"Press the 'accept'   ", 2, 0);
            		oledPutROMString((ROM_STRING)" button to select a  ", 3, 0);
            		oledPutROMString((ROM_STRING)" file.               ", 4, 0);
            		oledPutROMString((ROM_STRING)"Press menu button to ", 5, 0);
            		oledPutROMString((ROM_STRING)" continue.           ", 6, 0);

                    for(i=0;i<100000;i++){} //crude debounce
                    while(BLIO_LoaderEnabled() == FALSE){}
                    for(i=0;i<100000;i++){} //crude debounce
                    while(BLIO_LoaderEnabled() == TRUE){}
                    for(i=0;i<100000;i++){} //crude debounce
			        LoadApplication();
			    }
				i ++;
			}
			j--;
		}

        /* Launch the application if the image in Flash is valid */
        if (BL_ApplicationIsValid())
        {
            FillDisplay(0x00);
    
            /* Launch the application */
            BLIO_ReportBootStatus(BL_BOOTING, "BL: Launching application\r\n");
    
            //Turn off the App Vdd
            AppPowerDisable();
    
    	    /* Must deinitialize the boot loader IO */
    	    BLIO_DeinitializeIO();
    	
  	        BootApplication();
    	}

	    /* Should never get here if a valid application was loaded */
	    BLIO_ReportBootStatus(BL_BOOT_FAIL, "BL: Application failed to launch\r\n");
	    BL_ApplicationFailedToLaunch();
    }
	
	/* Should never hit this point */
	#if defined(__18CXX)
		return;
	#else
		return 0;
	#endif            
}

