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

 Change History:
  Rev   Description
  ----  -----------------------------------------
  2.7   Updated to include PIC32 support

********************************************************************************
*/

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "boot.h"
#include "MDD File System/FSIO.h"
#include "USB/usb.h"
#include "USB/usb_host_msd_scsi.h"

// *****************************************************************************
// *****************************************************************************
// Configuration Bits
// *****************************************************************************
// *****************************************************************************

#define PLL_96MHZ_OFF   0xFFFF
#define PLL_96MHZ_ON    0xF7FF


// *****************************************************************************
// *****************************************************************************
// Configuration Bits
// *****************************************************************************
// *****************************************************************************

#if defined __C30__ || defined __XC16__

    #if defined(__PIC24FJ256GB110__)
        _CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & PLL_96MHZ_ON & PLLDIV_DIV2 & IESO_OFF) // Primary HS OSC with PLL, USBPLL /2
        _CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)   // JTAG off, watchdog timer off
        _CONFIG3(0xFFFF);
    #elif defined(__PIC24FJ64GB004__)
        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
        _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
    #elif defined(__PIC24FJ256GB106__)
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
    #elif defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__) 
        _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
    #endif

#else

    #warning Cannot define configuration bits.

#endif

// Macro used to call main application
/****************************************************************************
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
***************************************************************************/

#define BootApplication()       (((int(*)(void))(APPLICATION_ADDRESS))())


/****************************************************************************
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
***************************************************************************/
void LoadApplication ( void )
{
    BOOL    LoadingApplication      = TRUE;
    BOOL    TransportInitialized    = FALSE;
    //BOOL    FileSystemInitialized   = FALSE;
    BOOL    BootMediumAttached      = FALSE;
    BOOL    BootImageFileFound      = FALSE;
    BOOL    BootImageFileError      = FALSE;
    int     ErrCount                = 0;

    // Loader main loop
    while (LoadingApplication)
    {
        if (TransportInitialized)
        {
            // Keep the boot medium alive
            TransportInitialized = BLMedia_MonitorMedia();
            
            // Check for the boot medium to attach
            BootMediumAttached = BLMedia_MediumAttached();
            if (BootMediumAttached)
            {
                if (!BootImageFileError)
                {
                    // Attempt to locate the boot image file
                    BootImageFileFound = BLMedia_LocateFile(BOOT_FILE_NAME);
                    if (BootImageFileFound)
                    {
                        BLIO_ReportBootStatus(BL_FOUND_FILE, "BL: Application image file has been found\r\n");

                        // Read the boot image file and program it to Flash
                        if (BLMedia_LoadFile(BOOT_FILE_NAME))
                        {
                            LoadingApplication = FALSE;
                            BLIO_ReportBootStatus(BL_PROGRAMMED, "BL: Application image has been programmed\r\n");
                        }
                        else
                        {
                            // Error reported by lower layer
                            BootImageFileError = TRUE;
                        }
                    }
                    else
                    {
                        // Count and, if necessary, report the errors locating the file
                        ErrCount++;
                        if (ErrCount > MAX_LOCATE_RETRYS)
                        {
                            ErrCount = 0;
                            BootImageFileError = TRUE;
                            BLIO_ReportBootStatus(BL_FILE_NOT_FOUND, "BL: Application image not found\r\n");
                        }
                    }
                }
            }
            else
            {
                BootImageFileError = FALSE;
            }
        }
        else
        {
            // Initialize transport layer used to access the boot image's file system
            TransportInitialized = BLMedia_InitializeTransport();
            if (TransportInitialized)
            {
                BLIO_ReportBootStatus(BL_TRANSPORT_INIT, "BL: Transport initialized\r\n");
            }
        }

        // Watch for user to abort the load
        if (BLIO_AbortLoad())
        {
            LoadingApplication = FALSE;
        }
    }

} // LoadApplication


/****************************************************************************
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
***************************************************************************/
int main ( void )
{
    // Initialize the boot loader IO
    BLIO_InitializeIO();
    BLIO_ReportBootStatus(BL_RESET, "BL: ***** Reset *****\r\n");

    // Check to see if the user requested loading of a new application
    if (BLIO_LoaderEnabled())
    {
       #if defined(PIC24FJ64GB004_PIM) || defined(PIC24FJ256DA210_DEV_BOARD)
    	//On the PIC24FJ64GB004 Family of USB microcontrollers, the PLL will not power up and be enabled
    	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
    	//This allows the device to power up at a lower initial operating frequency, which can be
    	//advantageous when powered from a source which is not gauranteed to be adequate for 32MHz
    	//operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
    	//power up the PLL.
        {
            unsigned int pll_startup_counter = 600;
            CLKDIVbits.PLLEN = 1;
            while(pll_startup_counter--);
        }
    
        //Device switches over automatically to PLL output after PLL is locked and ready.
        #endif

        #if defined(__PIC24F__)
        INTCON2bits.ALTIVT = 1;
        #endif

        mLED_3_On();
        BLIO_ReportBootStatus(BL_LOADING, "BL: Loading new application image\r\n");
        LoadApplication();
    }

    // Launch the application if the image in Flash is valid
    if (BL_ApplicationIsValid())
    {
        BLIO_ReportBootStatus(BL_BOOTING, "BL: Launching application\r\n");

        // Must disable all interrupts
        BLMedia_DeinitializeTransport();
        U1IE = 0;
        U1IR = 0xFF;
        IEC5 = 0;
        IFS5 = 0;
        INTCON2bits.ALTIVT = 0;

  
        ////////////////////////////
        // Launch the application //
        ////////////////////////////
        BootApplication();
    }

    // Should never get here if a valid application was loaded.
    BLIO_ReportBootStatus(BL_BOOT_FAIL, "BL: Application failed to launch\r\n");
    BL_ApplicationFailedToLaunch();

    // Hang system
    while (1)
        ;

    return 0;

} // main

/*
*******************************************************************************
EOF
*******************************************************************************
*/

