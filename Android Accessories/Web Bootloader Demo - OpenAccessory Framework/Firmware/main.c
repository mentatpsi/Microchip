/*******************************************************************************

    USB Android Accessory web bootloader demo with accessory in host mode

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

FileName:        main.c
Dependencies:    None
Processor:       PIC24/dsPIC30/dsPIC33/PIC32MX
Compiler:        C30/C32
Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the "Company") for its PICmicro(R) Microcontroller is intended and
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

Change History
  Rev      Description
  -----    ----------------------------------
  2.9      Initial revision
*******************************************************************************/

// Include files
#include "USB/usb.h"
#include "USB/usb_host_android.h"
#include "Compiler.h"
#include "HardwareProfile.h"
#include "timer.h"
#include "boot_commands.h"
#include "boot_config.h"
#include "custom_bin_boot.h"

// If a maximum current rating hasn't been defined, then define 500mA by default
#ifndef MAX_ALLOWED_CURRENT
    #define MAX_ALLOWED_CURRENT             (500)         // Maximum power we can supply in mA
#endif

// Define a debug error printing
#define DEBUG_ERROR(a)   Nop(); Nop(); Nop();

// Configuration bits for the device.  Please refer to the device datasheet for each device
//   to determine the correct configuration bit settings
#if defined __C30__ || defined __XC16__
    #define PLL_96MHZ_OFF   0xFFFF
    #define PLL_96MHZ_ON    0xF7FF

    // Configuration Bit settings  for an Explorer 16 with USB PICtail Plus
    //      Primary Oscillator:             HS
    //      Internal USB 3.3v Regulator:    Disabled
    //      IOLOCK:                         Set Once
    //      Primary Oscillator Output:      Digital I/O
    //      Clock Switching and Monitor:    Both disabled
    //      Oscillator:                     Primary with PLL
    //      USB 96MHz PLL Prescale:         Divide by 2
    //      Internal/External Switch Over:  Enabled
    //      WDT Postscaler:                 1:32768
    //      WDT Prescaler:                  1:128
    //      WDT Window:                     Non-window Mode
    //      Comm Channel:                   EMUC2/EMUD2
    //      Clip on Emulation Mode:         Reset into Operation Mode
    //      Write Protect:                  Disabled
    //      Code Protect:                   Disabled
    //      JTAG Port Enable:               Disabled

    #if defined(__PIC24FJ256GB110__)
        _CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & PLL_96MHZ_ON & PLLDIV_DIV2) // Primary HS OSC with PLL, USBPLL /2
        _CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)   // JTAG off, watchdog timer off
    #elif defined(__PIC24FJ64GB004__)
        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_ON)
        _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
    #elif defined(__PIC24FJ64GB502__)
        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(I2C1SEL_PRI & IOL1WAY_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
        _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
    #elif defined(__PIC24FJ256GB106__)
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( 0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
    #elif defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
        _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
    #elif defined(__dsPIC33EP512MU810__)
        _FOSCSEL(FNOSC_FRC);
        _FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
        _FWDT(FWDTEN_OFF);    
    #endif
#elif defined( __PIC32MX__ )
    #pragma config UPLLEN   = ON            // USB PLL Enabled
    #pragma config FPLLMUL  = MUL_15        // PLL Multiplier
    #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
    #pragma config FPLLODIV = DIV_1         // PLL Output Divider
    #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
    #pragma config FWDTEN   = OFF           // Watchdog Timer
    #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
    //#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
    #pragma config OSCIOFNC = OFF           // CLKO Enable
    #pragma config POSCMOD  = HS            // Primary Oscillator
    #pragma config IESO     = OFF           // Internal/External Switch-over
    #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
    #pragma config FNOSC    = PRIPLL        // Oscillator Selection
    #pragma config CP       = OFF           // Code Protect
    #pragma config BWP      = OFF           // Boot Flash Write Protect
    #pragma config PWP      = OFF           // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
    #pragma config DEBUG    = ON            // Background Debugger Enable
#else
    #error Cannot define configuration bits.
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

// C30 and C32 Exception Handlers
// If your code gets here, you either tried to read or write
// a NULL pointer, or your application overflowed the stack
// by having too many local variables or parameters declared.
#if defined(__C30__) || defined __XC16__
	void _ISR __attribute__((__no_auto_psv__)) _AddressError(void)
	{
        DEBUG_ERROR("Address error");
        while(1){}
	}
	void _ISR __attribute__((__no_auto_psv__)) _StackError(void)
	{
	    DEBUG_ERROR("Stack error");
        while(1){}
	}
	void _ISR __attribute__((__no_auto_psv__)) _DefaultInterrupt(void)
	{
	    DEBUG_ERROR("General error");
        while(1){}
	}
	
#elif defined(__C32__)
	void _general_exception_handler(unsigned cause, unsigned status)
	{
    	#if defined(DEBUG_MODE)
            unsigned long address = _CP0_GET_EPC();
        #endif

		DEBUG_ERROR("exception");
        while(1){}
	}
#endif

//Local prototypes
#if defined(__C32__)
static void InitPIC32(void);
#endif

// Global variables
static void* device_handle = NULL;
BOOL device_attached = FALSE;

// Constants for Android Accessory Information
static char manufacturer[] = "Microchip Technology Inc.";
static char model[] = "Web Bootloader Accessory Demo";
static char description[] = DEMO_BOARD_NAME_STRING;
static char version[] = "1.0";
static char uri[] = "http://www.microchip.com/android";
static char serial[] = "N/A";

ANDROID_ACCESSORY_INFORMATION myDeviceInfo =
{
    manufacturer,
    sizeof(manufacturer),
    model,
    sizeof(model),
    description,
    sizeof(description),
    version,
    sizeof(version),
    uri,
    sizeof(uri),
    serial,
    sizeof(serial)
};




/****************************************************************************
  Function:
    int main(void)

  Summary:
    main function

  Description:
    main function

  Precondition:
    None

  Parameters:
    None

  Return Values:
    int - exit code for main function

  Remarks:
    None
  ***************************************************************************/
int main(void)
{
    BOOL validation_failed = FALSE;
    
    // Enable the bootloader's interrupt vector table
    #if defined(__PIC24F__)
        INTCON2bits.ALTIVT = 1;
    #endif
    
    // Initialize if PIC32
    #if defined(__PIC32MX__)
        InitPIC32();
    #endif
    
    // Initialize the switches
    mInitAllSwitches();
    
    // If a software reset occurred, then it's most likely because we are trying to
    //  enter into bootloader mode from the application. So delay for 2 seconds before
    //  re-enabling the USB module to make sure that the attached android device
    //  detects a detachment.
    if(RCONbits.SWR)
    {
        DelayMs(2000);
    }
    
    // Initialize USB module and Android Host driver
    USBInitialize(0);
    AndroidAppStart(&myDeviceInfo);
    
    // Run loop
    while(1)
    {
        // If the bootloader is enabled through hardware, there's been a software reset,
        //  or validation of the boot region failed, then try to enter the bootloader
        if(BLIO_LoaderEnabled() || RCONbits.SWR || validation_failed)
        {
            // First wait until the device is attached, though
            while(device_attached == FALSE)
            {
                //Keep the USB stack running
                USBTasks();
            }
        
            // Then enter bootloader mode
            EnterBootLoaderMode(device_handle);
        }
        
        // Validate the boot regions, if the program memory has been written to correctly
        if(ValidationMethod())
        {
            // Delay 2 seconds, so status of bootload on Android can be seen
            DelayMs(2000);
            
            // De-initialize the USB
            BLMedia_DeinitializeTransport();
            
            // Delay another 2 seconds so the android device detects the detach
            DelayMs(2000);
            
            // Enable the bootloader's interrupt vector table (PIC24) or disable interrupts (PIC32)
            #if defined(__PIC24F__)
                INTCON2bits.ALTIVT = 0;
            #elif defined(__PIC32MX__)
                INTDisableInterrupts();
            #endif
            
            // Enter into the application code
            BootApplication();
        }
        else
        {
            // Otherwise validation has failed, set flag
            validation_failed = TRUE;
        }      
    }    
}

/****************************************************************************
  Function:
    BOOL USB_ApplicationDataEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )

  Summary:
    Handles USB data application events

  Description:
    Handles USB data application events

  Precondition:
    None

  Parameters:
    BYTE address - address of the device causing the event
    USB_EVENT event - the event that has occurred
    void* data - data associated with the event
    DWORD size - the size of the data in the data field

  Return Values:
    BOOL - Return TRUE of the event was processed.  Return FALSE if the event
           wasn't handled.

  Remarks:
    None
  ***************************************************************************/
BOOL USB_ApplicationDataEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    return FALSE;
}


/****************************************************************************
  Function:
    BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )

  Summary:
    Handles USB application events

  Description:
    Handles USB application events

  Precondition:
    None

  Parameters:
    BYTE address - address of the device causing the event
    USB_EVENT event - the event that has occurred
    void* data - data associated with the event
    DWORD size - the size of the data in the data field

  Return Values:
    BOOL - Return TRUE of the event was processed.  Return FALSE if the event
           wasn't handled.

  Remarks:
    None
  ***************************************************************************/
BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    switch( (INT)event )
    {
        case EVENT_VBUS_REQUEST_POWER:
            // The data pointer points to a byte that represents the amount of power
            // requested in mA, divided by two.  If the device wants too much power,
            // we reject it.
            if (((USB_VBUS_POWER_EVENT_DATA*)data)->current <= (MAX_ALLOWED_CURRENT / 2))
            {
                return TRUE;
            }
            else
            {
                DEBUG_ERROR( "\r\n***** USB Error - device requires too much current *****\r\n" );
            }
            break;

        case EVENT_VBUS_RELEASE_POWER:
        case EVENT_HUB_ATTACH:
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
        case EVENT_DETACH:              // USB cable has been detached (data: BYTE, address of device)
        case EVENT_ANDROID_DETACH:
            device_attached = FALSE;
            DeviceDetached();           // Tell bootloader that device was detached
            return TRUE;
            break;

        // Android Specific events
        case EVENT_ANDROID_ATTACH:
            device_attached = TRUE;
            device_handle = data;
            return TRUE;

        default :
            break;
    }
    return FALSE;
}


/****************************************************************************
  Function:
    void InitPIC32(void)

  Summary:
    Initialize the PIC32 core to the correct modes and clock speeds

  Description:
    Initialize the PIC32 core to the correct modes and clock speeds

  Precondition:
    Only runs on PIC32

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/
#if defined(__PIC32MX__)
static void InitPIC32(void)
{
    int  value;
    
    #if defined(RUN_AT_60MHZ)
        // Use OSCCON default
    #else
        OSCCONCLR = 0x38000000; //PLLODIV
        #if defined(RUN_AT_48MHZ)
            OSCCONSET = 0x08000000; //PLLODIV /2
        #elif defined(RUN_AT_24MHZ)
            OSCCONSET = 0x10000000; //PLLODIV /4
        #else
            #error Cannot set OSCCON
        #endif
    #endif
    
    value = SYSTEMConfigWaitStatesAndPB( GetSystemClock() );
    
    // Enable the cache for the best performance
    CheKseg0CacheOn();
    
    INTEnableSystemMultiVectoredInt();
    
    DDPCONbits.JTAGEN = 0;

    value = OSCCON;
    while (!(value & 0x00000020))
    {
        value = OSCCON;    // Wait for PLL lock to stabilize
    }
    
    INTEnableInterrupts();
}
#endif

