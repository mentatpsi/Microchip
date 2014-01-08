/******************************************************************************

Simple Charger

This demo shows how to create a generic charger using the USB Host stack.  Many
USB devices will not allow a device to charge its batteries until it has been
enumerated and allowed to draw a certain amount of current.  This demo will
allow a device to draw up to 500 mA (the USB maximum), so ensure that the
hardware is also capable of providing that amount of current.

This demo runs on an Explorer 16 (DM240001) with a PIC24FJ256GB110 (USB) PIM
(MA240014), a USB PICtail Plus Daughter Board (AC164127), and an external power
supply to provide more current.

The demo has been tested with these devices:
    * iPod Nano
    * Blackberry Curve (100mA charging only)
    * Motorola RAZR

******************************************************************************/

/******************************************************************************

* FileName:         Simple Charger.c
* Dependencies:     USB Embedded Host library
* Processor:        PIC24F
* Compiler:         C30 v3.10b/C32 v1.02
* Company:          Microchip Technology, Inc.

*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "uart2.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_charger.h"


// *****************************************************************************
// *****************************************************************************
// Constants
// *****************************************************************************
// *****************************************************************************

#define MAX_ALLOWED_CURRENT                 500


// *****************************************************************************
// *****************************************************************************
// Configuration Bits
// *****************************************************************************
// *****************************************************************************
// Configuration Bit settings
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

#if defined __C30__ || defined __XC16__
    #if defined(__PIC24FJ256GB110__)
        _CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & PLL_96MHZ_ON & PLLDIV_DIV2 & IESO_OFF) // Primary HS OSC with PLL, USBPLL /2
        _CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)   // JTAG off, watchdog timer off
    #elif defined(__PIC24FJ64GB004__)
        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
        _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
    #elif defined(__PIC24FJ256GB106__)
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
    #elif defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
        _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
    #elif defined(__PIC24FJ64GB502__)
        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
	_CONFIG2(I2C1SEL_PRI & IOL1WAY_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
	_CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
	_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
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
    #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
    #pragma config OSCIOFNC = OFF           // CLKO Enable
    #pragma config POSCMOD  = HS            // Primary Oscillator
    #pragma config IESO     = OFF           // Internal/External Switch-over
    #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
    #pragma config FNOSC    = PRIPLL        // Oscillator Selection
    #pragma config CP       = OFF           // Code Protect
    #pragma config BWP      = OFF           // Boot Flash Write Protect
    #pragma config PWP      = OFF           // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#else

    #error Cannot define configuration bits.

#endif

// *****************************************************************************
// *****************************************************************************
// Internal Function Prototypes
// *****************************************************************************
// *****************************************************************************

void    MonitorVBUS( void );


// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

BOOL    overcurrentStateUSB;        // We really only need a bit here.


//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
    BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event,
                void *data, DWORD size )

  Summary:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.

  Description:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.  If the application is able to handle the event, it
    returns TRUE.  Otherwise, it returns FALSE.

  Precondition:
    None

  Parameters:
    BYTE address    - Address of device where event occurred
    USB_EVENT event - Identifies the event that occured
    void *data      - Pointer to event-specific data
    DWORD size      - Size of the event-specific data

  Return Values:
    TRUE    - The event was handled
    FALSE   - The event was not handled

  Remarks:
    The application may also implement an event handling routine if it
    requires knowledge of events.  To do so, it must implement a routine that
    matches this function signature and define the USB_HOST_APP_EVENT_HANDLER
    macro as the name of that function.
  ***************************************************************************/

BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    switch( (int) event )
    {
        // --------------------------------------------------------------------------
        // Charger events

        case EVENT_CHARGER_ATTACH:
            UART2PrintString( "Device attached for charging.\r\n" );
            return TRUE;
            break;

        case EVENT_CHARGER_DETACH:
            UART2PrintString( "Charging device detached\r\n" );
            return TRUE;
            break;

        // --------------------------------------------------------------------------
        // Host events

        case EVENT_VBUS_REQUEST_POWER:
            // The data pointer points to a byte that represents the amount of power
            // requested in mA, divided by two.  If the device wants too much power,
            // we reject it.
            if (((USB_VBUS_POWER_EVENT_DATA*)data)->current <= (MAX_ALLOWED_CURRENT / 2))
            {
                UART2PrintString(  "Device requires " );
                UART2PutDec( (unsigned char)((WORD)((USB_VBUS_POWER_EVENT_DATA*)data)->current * 2 / 10) );
                UART2PutDec( (unsigned char)((WORD)((USB_VBUS_POWER_EVENT_DATA*)data)->current * 2 % 10) );
                UART2PrintString(  "mA.\r\n" );
                return TRUE;
            }
            else
            {
                UART2PrintString(  "Device requires too much current.\r\n" );
            }
            break;

        case EVENT_VBUS_RELEASE_POWER:
            // Turn off Vbus power.
            // The PIC24F with the Explorer 16 cannot turn off Vbus through software.
            return TRUE;
            break;

        case EVENT_HUB_ATTACH:
            UART2PrintString(  "Hubs are not supported.\r\n" );
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            UART2PrintString(  "Device is not supported.\r\n" );
            return TRUE;
            break;

        case EVENT_CANNOT_ENUMERATE:
            UART2PrintString(  "Cannot enumerate device.\r\n" );
            return TRUE;
            break;

        case EVENT_CLIENT_INIT_ERROR:
            UART2PrintString(  "Client driver initialization error.\r\n" );
            return TRUE;
            break;

        case EVENT_OUT_OF_MEMORY:
            UART2PrintString(  "Out of heap memory.\r\n" );
            return TRUE;
            break;

        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            UART2PrintString( "Unspecified USB error.\r\n" );
            return TRUE;
            break;

        default:
            break;
    }

    return FALSE;
}



//******************************************************************************
//******************************************************************************
// Internal Functions
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
     void InitializeVbusMonitor( void )

  Description:
    This function initializes monitoring of the VBUS analog input.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void InitializeVbusMonitor( void )
{
    #if defined( __C30__) || defined __XC16__
        // Set up the A/D converter
        #if defined(__PIC24FJ256DA210__) 
            //The over current detection of the PIC24FJ256DA210 development board is
            //  not connected by default.
        #elif defined(__PIC24FJ256GB210__)
            ANSBbits.ANSB8 = 1;
        #else
            AD1PCFGL    = 0xFEFF;       // Disable digital input on AN8
        #endif
        AD1CSSL     = 0x0000;       // No scan
        AD1CHS      = 0x0008;       // Mux A uses AN8
        AD1CON3     = 0x1F05;       // 31 Tad auto-sample, Tad = 5*Tcy
        AD1CON2     = 0x0000;       // AVdd, AVss, MUX A only
        AD1CON1     = 0x80E4;       // Turn on, auto sampling, auto convert
    #endif

    return;
}



/****************************************************************************
  Function:
    void MonitorVBUS( void )

  Description:
    This routine monitors VBUS to check for overcurrent conditions.  The
    Explorer 16 with the PIC24FJ256GA110 PIM and USB PICtail Plus has an
    analog input dedicated to monitoring Vbus.  The voltage on Vbus is
    divided by two and fed into RB8/AN8.  The valid range for Vbus is
    4.4V - 5.25V.  If we are in an overcurrent condition, Vbus will be
    lower than 4.4V.  Full range on the A/D is 0x3FF for 3.3V.  So any
    value lower than the value calculated below on AN8 is overcurrent.
    Otherwise, the current level is fine.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    TRUE    - An overcurrent situation exists. USB shut down.
    FALSE   - Normal USB operation.

  Remarks:
    Since Vbus monitoring is application-specific, it is the application's
    responsibility.

    If we get an overcurrent, we must shut down immediately to avoid brownout
    or blackout.
  ***************************************************************************/

#define ADC_READING_VBUS            ADC1BUF0
#define OVERCURRENT_RESET_VOLTAGE   4800ul        // In millivolts
#define OVERCURRENT_TRIP_VOLTAGE    4750ul        // In millivolts
#define SYSTEM_VOLTAGE              3300ul        // In millivolts
#define OVERCURRENT_RESET_READING   (1023ul * (OVERCURRENT_RESET_VOLTAGE/2) / SYSTEM_VOLTAGE)
#define OVERCURRENT_TRIP_READING    (1023ul * (OVERCURRENT_TRIP_VOLTAGE/2) / SYSTEM_VOLTAGE)

void MonitorVBUS( void )
{
    #if defined( __C30__) || defined __XC16__
        if (AD1CON1bits.DONE)
        {
            if (ADC_READING_VBUS < OVERCURRENT_TRIP_READING)
            {
                USBHostVbusEvent( EVENT_VBUS_OVERCURRENT, USB_ROOT_HUB, 0 );
                if (!overcurrentStateUSB)
                {
                    UART2PrintString( "\r\n***** USB Error - overcurrent detected *****\r\n" );
                    overcurrentStateUSB = TRUE;
                }
            }
            else if (ADC_READING_VBUS > OVERCURRENT_RESET_READING)
            {
                if (overcurrentStateUSB)
                {
                    USBHostVbusEvent( EVENT_VBUS_POWER_AVAILABLE, USB_ROOT_HUB, 0 );
                    UART2PrintString( "\r\n***** USB overcurrent condition removed *****\r\n" );
                }
                overcurrentStateUSB = FALSE;
            }
            AD1CON1     = 0x80E4;       // Start the next conversion.
        }
    #endif
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Main
//******************************************************************************
//******************************************************************************
//******************************************************************************

int main (void)
{

    #if defined (__C30__) || defined __XC16__
        #if defined( __PIC24FJ256GB110__ )
            // Configure U2RX - put on pin 49 (RP10)
            RPINR19bits.U2RXR = 10;
            // Configure U2TX - put on pin 50 (RP17)
            RPOR8bits.RP17R = 5;

            // Configure SPI2 Data In - put on pin 11 (RP26)
            RPINR22bits.SDI2R = 26;

            // Configure SPI2 Clock Out - put on pin 10 (RP21)
            RPOR10bits.RP21R = 11;

            // Configure SPI2 Data Out - put on pin 12 (RP19)
            RPOR9bits.RP19R = 10;

        #endif
        OSCCON = 0x3302;    // Enable secondary oscillator
        CLKDIV = 0x0000;    // Set PLL prescaler (1:1)
    #elif defined(__PIC32MX__)
        {
            int  value;

            value = SYSTEMConfigWaitStatesAndPB( GetSystemClock() );

            // Enable the cache for the best performance
            CheKseg0CacheOn();

            INTEnableSystemMultiVectoredInt();

            value = OSCCON;
            while (!(value & 0x00000020))
            {
                value = OSCCON;    // Wait for PLL lock to stabilize
            }
        }

        AD1PCFG = 0xFFFF;   // Set analog pins to digital.
        TRISF   = 0x00;
    #else
        #error Cannot initialize.
    #endif

   #if defined(__PIC24FJ64GB004__)
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
    #endif

    UART2Init();

    UART2PrintString( "\r\n\r\nGeneral USB Charger\r\n\r\n" );

    // Initialize USB Embedded Host
    USBInitialize( 0 );

    // Turn on the A/D converter to monitor Vbus.
    InitializeVbusMonitor();


    while(1)
    {
        MonitorVBUS();
        USBTasks();
    }
}


/*******************************************************************************
Function:       void __attribute__((__interrupt__, auto_psv)) _XXX(void)

These are just here to catch any spurious interrupts that we see during
debugging.

*******************************************************************************/
#if defined( __C30__ ) || defined __XC16__

void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void)
{
  UART2PrintString( "!!! Default interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}

void __attribute__((interrupt, auto_psv)) _OscillatorFail(void)
{
  UART2PrintString( "!!! Oscillator Fail interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _AddressError(void)
{
  UART2PrintString( "!!! Address Error interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _StackError(void)
{
  UART2PrintString( "!!! Stack Error interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _MathError(void)
{
  UART2PrintString( "!!! Math Error interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}

#endif



