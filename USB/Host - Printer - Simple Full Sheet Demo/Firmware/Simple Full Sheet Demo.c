/******************************************************************************

Simple Full Sheet Demo

This is a simple printer demo to show how to send commands to different types
of printers.  When a printer attaches, a print sample is sent to the printer.
No other user interaction is required to generate the print-out.

This demo runs on an Explorer 16 (DM240001) with a PIC24FJ256GB110 (USB) PIM
(MA240014) and a USB PICtail Plus Daughter Board (AC164127).

In its initial state, this demo supports printers that:
    * Advertise the Printer Class with either unidirectional or bidirectional
        interface.
    * Advertise either Postscript or PCL (PCL 5) support in their device ID
        string.

The demo has been tested with these printers:
    * Lexmark E250dn - laser printer that supports both PostScript and PCL 5.
        This demo is configured to explicitly use PostScript for this printer.
    * HP Deskjet 460 - mobile inkjet printer that uses PCL 3, a limited
        subset of PCL 5.

To modify this demo for a different printer, run the configuration tool
USBConfig.exe to generate the appropriate configuration files usb_config.c and
usb_config.h.

Refer to the Simple POS Demo for an example of sending commands to a POS
printer.

******************************************************************************/

/******************************************************************************

 FileName:         Simple Full Sheet Demo.c
 Dependencies:     USB Embedded Host library
 Processor:        PIC24F
 Compiler:         C30 v3.10b/C32 v1.02
 Company:          Microchip Technology, Inc.

 Change History:
  Rev           Description
  ----------    -----------
  2.6 - 2.6A    No chance except stack revision number
  2.7           Updated the demo to show how to use the USBHostPrinterGetStatus()
                function.
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "uart2.h"
#include "Graphics/Primitive.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_printer.h"


// *****************************************************************************
// *****************************************************************************
// Constants
// *****************************************************************************
// *****************************************************************************

#define MAX_ALLOWED_CURRENT                 250


// *****************************************************************************
// *****************************************************************************
// Configuration Bits
// *****************************************************************************
// *****************************************************************************

#if defined __C30__ || defined __XC16__
    #if defined(__PIC24FJ256GB110__)
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_ON)
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
    #else
        #warning "No configuration bits set for this project.  USB needs specific configuration settings.  Please verify that the configuration bits are set correctly."
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
// Data Structures
// *****************************************************************************
// *****************************************************************************

typedef struct
{
    BYTE    printer;
    BYTE    printerAttached     : 1;
    BYTE    printerStatusSent   : 1;
    BYTE    printerStatusDone   : 1;
    BYTE    pagePrinted         : 1;
    BYTE    overcurrentStateUSB : 1;
} PRINT_STATUS;


// *****************************************************************************
// *****************************************************************************
// Internal Function Prototypes
// *****************************************************************************
// *****************************************************************************

void    MonitorVBUS( void );
#if defined( __C30__ ) || defined __XC16__
    void PrintImageFullSheet( BYTE __prog__ *ptr, USB_PRINTER_IMAGE_INFO *imageInfo );
#elif defined( __PIC32MX__ )
    void PrintImageFullSheet( const BYTE *ptr, USB_PRINTER_IMAGE_INFO *imageInfo );
#endif


// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

char                            adddressMicrochip[4][28] = {   "Microchip Technology Inc.",
                                                                "2355 W. Chandler Blvd.",
                                                                "Chandler, AZ 85224-6199",
                                                                "Main Office 1-480-792-7200" };
char                            buffer[50];
char                            businessCard[8][24]    =   {   "PIC24FJ256GB1 / PIC32",
                                                                "USB Embedded Host",
                                                                "Printer Class Support",
                                                                "Printer Languages:",
                                                                "     * PCL 5",
                                                                "     * PostScript",
                                                                "     * ESC/POS" };
USB_PRINTER_IMAGE_INFO          imageInfo;
extern const IMAGE_FLASH       logoMCHP;
char                            notes[4][20]           =   {    "Bitmapped Image",
                                                                "Text - Helvetica",
                                                                "Text - Avant Garde",
                                                                "Vector Graphics" };
USB_PRINTER_GRAPHICS_PARAMETERS params;
USB_PRINTER_DEVICE_ID           printerInfo;
BYTE                            returnCode;
PRINT_STATUS                    status;


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
    switch( (INT)event )
    {
        // --------------------------------------------------------------------------
        // Printer events

        case EVENT_PRINTER_ATTACH:
            UART2PrintString( "APP:  Printer attached.\r\n" );
            status.printerAttached      = 1;
            status.printerStatusSent    = 0;
            status.printerStatusDone    = 0;
            printerInfo                 = *(USB_PRINTER_DEVICE_ID *)data;
            return TRUE;
            break;

        case EVENT_PRINTER_UNSUPPORTED:
            UART2PrintString( "APP: Unsupported printer attached\r\n" );
            return TRUE;
            break;

        case EVENT_PRINTER_DETACH:
            UART2PrintString( "APP:  Printer detached\r\n" );
            status.printerAttached      = 0;
            status.pagePrinted          = 0;
            status.printerStatusSent    = 0;
            status.printerStatusDone    = 0;
            printerInfo.deviceAddress   = 0;
            return TRUE;
            break;

        case EVENT_PRINTER_TX_DONE:
//            UART2PrintString( "APP: OUT transfer complete\r\n" );
            return TRUE;
            break;

        case EVENT_PRINTER_RX_DONE:
//            UART2PrintString( "APP: IN transfer complete\r\n" );
            return TRUE;
            break;

        case EVENT_PRINTER_REQUEST_DONE:
//            UART2PrintString( "APP: Printer Status: " );
//            UART2PutHex( status.printer );
//            UART2PrintString( "\r\n" );
            status.printerStatusDone    = 1;
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
                return TRUE;
            }
            else
            {
                UART2PrintString(  "Device requires too much current." );
            }
            break;

        case EVENT_VBUS_RELEASE_POWER:
            // Turn off Vbus power.
            // The PIC24F with the Explorer 16 cannot turn off Vbus through software.
            return TRUE;
            break;

        case EVENT_HUB_ATTACH:
            UART2PrintString(  "Hubs are not supported." );
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            UART2PrintString(  "Device is not supported." );
            return TRUE;
            break;

        case EVENT_CANNOT_ENUMERATE:
            UART2PrintString(  "Cannot enumerate device." );
            return TRUE;
            break;

        case EVENT_CLIENT_INIT_ERROR:
            UART2PrintString(  "Client driver initialization error." );
            return TRUE;
            break;

        case EVENT_OUT_OF_MEMORY:
            UART2PrintString(  "Out of heap memory." );
            return TRUE;
            break;

        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            UART2PrintString( "Unspecified." );
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
    #if defined( __PIC24FJ256GB110__)
        if (AD1CON1bits.DONE)
        {
            if (ADC_READING_VBUS < OVERCURRENT_TRIP_READING)
            {
                USBHostVbusEvent( EVENT_VBUS_OVERCURRENT, USB_ROOT_HUB, 0 );
                if (!status.overcurrentStateUSB)
                {
                    UART2PrintString( "\r\n***** USB Error - overcurrent detected *****\r\n" );
                    status.overcurrentStateUSB = TRUE;
                }
            }
            else if (ADC_READING_VBUS > OVERCURRENT_RESET_READING)
            {
                if (status.overcurrentStateUSB)
                {
                    USBHostVbusEvent( EVENT_VBUS_POWER_AVAILABLE, USB_ROOT_HUB, 0 );
                    UART2PrintString( "\r\n***** USB overcurrent condition removed *****\r\n" );
                }
                status.overcurrentStateUSB = FALSE;
            }
            AD1CON1     = 0x80E4;       // Start the next conversion.
        }
    #endif
}



/****************************************************************************
  Function:

  Description:

  Precondition:

  Parameters:

  Returns:

  Remarks:
  ***************************************************************************/

#if defined( __C30__ ) || defined __XC16__
void PrintImageFullSheet( BYTE __prog__ *ptr, USB_PRINTER_IMAGE_INFO *imageInfo )
#elif defined( __PIC32MX__ )
void PrintImageFullSheet( const BYTE *ptr, USB_PRINTER_IMAGE_INFO *imageInfo )
#endif
{
    WORD                    currentRow;
    BYTE                    returnCode;
    WORD                    widthBytes;

    imageInfo->width    = ((WORD)ptr[5] << 8) + ptr[4];
    imageInfo->height   = ((WORD)ptr[3] << 8) + ptr[2];

    ptr += 10; // skip the header info (??, bits per pixel, Y size, X size, pallete[0], pallete[1] )

    widthBytes = (imageInfo->width + 7) / 8;

    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_IMAGE_START, USB_DATA_POINTER_RAM(imageInfo), sizeof(USB_PRINTER_IMAGE_INFO), 0 );

    for (currentRow=0; currentRow<imageInfo->height; currentRow++)
    {
        USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_IMAGE_DATA_HEADER, USB_NULL, imageInfo->width, 0 );
        USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_IMAGE_DATA, USB_DATA_POINTER_ROM(ptr), imageInfo->width, USB_PRINTER_TRANSFER_FROM_ROM );
        ptr += widthBytes;
    }

    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_IMAGE_STOP, USB_NULL, 0, 0 );
}


/****************************************************************************
  Function:

  Description:

  Precondition:

  Parameters:

  Returns:

  Remarks:
  ***************************************************************************/

void WriteLine( SHORT x, SHORT y, char *p )
{
    BYTE returnCode;

    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_SET_POSITION, USB_NULL, USBHostPrinterPosition(x, y), 0 );
    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_TEXT_START, USB_NULL, 0, 0 );
    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_TEXT, USB_DATA_POINTER_RAM(p), strlen(p), 0 );
    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_TEXT_STOP, USB_NULL, 0, 0 );
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
        #if defined( __PIC24FJ256GB110__ ) || defined(__PIC24FJ256GB210__)
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
            OSCCON = 0x3302;    // Enable secondary oscillator
            CLKDIV = 0x0000;    // Set PLL prescaler (1:1)
        #elif defined(__PIC24FJ64GB004__)
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
        #elif defined(__PIC24FJ256DA210__)
            //TX RF3 RP16
            //RX RD0 RP11
            // Configure U2RX - put on RP11
            RPINR19bits.U2RXR = 11;
            // Configure U2TX - put on RP16
            RPOR8bits.RP16R = 5;
        #endif

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

    UART2Init();

    UART2PrintString( "\r\n\r\nUSB Embedded Host Simple Full Sheet Printer Demo\r\n" );

    // Initialize USB Embedded Host
    USBInitialize(0);

    // Turn on the A/D converter to monitor Vbus.
    InitializeVbusMonitor();


    while(1)
    {
        MonitorVBUS();
        USBTasks();

        if (status.printerAttached)
        {
            if(status.printerStatusDone == 0)
            {
                if(status.printerStatusSent == 0)
                {
                    if(USBHostPrinterGetStatus( printerInfo.deviceAddress, &status.printer ) == USB_SUCCESS)
                    {
                        status.printerStatusSent = 1;
                    }
                }
            } 
            else
            {

                if(status.printer != 0x18)
                {
                    //if there was an error in the printer status then setup to 
                    //  check the status again
                    status.printerStatusSent = 0;
                    status.printerStatusDone = 0;
                }
                else
                {
    
                    if (!status.pagePrinted)
                    {
                        status.pagePrinted = 1;
        
                        UART2PrintString( "Printing to full sheet printer...\r\n" );
        
                        // Initialize
                        USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_JOB_START, USB_NULL, 0, 0 );
        
                        if (!printerInfo.support.supportFlags.supportsVectorGraphics)
                        {
                            // In the demo's initial configuration, this section executes for the HP Deskjet 460.
        
                            UART2PrintString( "Vector graphics are not supported.\r\n" );
        
                            imageInfo.resolution    = 75;
                            imageInfo.scale         = 1.0;
                            imageInfo.positionX     = (PRINTER_PAGE_PORTRAIT_WIDTH - 0x120)/2;
                            imageInfo.positionY     = 100;
                            #if defined( __C30__ ) || defined __XC16__
                                PrintImageFullSheet( (BYTE __prog__ *)(logoMCHP.address), &imageInfo );
                            #elif defined( __PIC32MX__ )
                                PrintImageFullSheet( (const BYTE *)(logoMCHP.address), &imageInfo );
                            #endif
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_COURIER, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_SIZE, USB_NULL, (DWORD)24, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_BOLD, USB_NULL, 0, 0 );
        
                            WriteLine( 200, 325, &(businessCard[0][0]) );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_MEDIUM, USB_NULL, 0, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_SIZE, USB_NULL, (DWORD)20, 0 );
        
                            WriteLine( 200, 350, &(businessCard[1][0]) );
                            WriteLine( 200, 375, &(businessCard[2][0]) );
                            WriteLine( 200, 400, &(businessCard[3][0]) );
                            WriteLine( 200, 425, &(businessCard[4][0]) );
                            WriteLine( 200, 450, &(businessCard[5][0]) );
                            WriteLine( 200, 475, &(businessCard[6][0]) );
                            WriteLine( 200, 500, &(businessCard[7][0]) );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_TIMES_NEW_ROMAN, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_SIZE, USB_NULL, (DWORD)18, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_BOLD, USB_NULL, 0, 0 );
        
                            WriteLine( 50, PRINTER_PAGE_PORTRAIT_HEIGHT - 145, &(adddressMicrochip[0][0]) );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_MEDIUM, USB_NULL, 0, 0 );
        
                            WriteLine( 50, PRINTER_PAGE_PORTRAIT_HEIGHT - 120, &(adddressMicrochip[1][0]) );
                            WriteLine( 50, PRINTER_PAGE_PORTRAIT_HEIGHT - 95, &(adddressMicrochip[2][0]) );
                            WriteLine( 50, PRINTER_PAGE_PORTRAIT_HEIGHT - 70, &(adddressMicrochip[3][0]) );
                        }
                        else
                        {
                            // In the demo's initial configuration, this section executes for the Lexmark E250dn.
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_ORIENTATION_LANDSCAPE, USB_NULL, 0, 0 );

                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE_WIDTH, USB_NULL, PRINTER_LINE_WIDTH_THICK, 0 );
        
                            params.sBevel.xL    = 50;     // X-axis position of the left side of the bevel.
                            params.sBevel.yT    = 50;     // Y-axis position of the top of the bevel.
                            params.sBevel.xR    = PRINTER_PAGE_LANDSCAPE_WIDTH  - 50;     // X-axis position of the right side of the bevel.
                            params.sBevel.yB    = PRINTER_PAGE_LANDSCAPE_HEIGHT - 50;     // Y-axis position of the bottom of the bevel.
                            params.sBevel.r     = 20;      // The radius of the cicle that defines the rounded corner
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_BEVEL, USB_DATA_POINTER_RAM(&params), sizeof(params.sBevel), 0 );
        
                            imageInfo.resolution    = 75;
                            imageInfo.scale         = 1.0;
                            imageInfo.positionX     = 100;
                            imageInfo.positionY     = 100;
                            #if defined( __C30__ ) || defined __XC16__
                                PrintImageFullSheet( (BYTE __prog__ *)(logoMCHP.address), &imageInfo );
                            #elif defined( __PIC32MX__ )
                                PrintImageFullSheet( (const BYTE *)(logoMCHP.address), &imageInfo );
                            #endif
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_HELVETICA, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_SIZE, USB_NULL, (DWORD)18, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_BOLD, USB_NULL, 0, 0 );
        
                            WriteLine( 100, PRINTER_PAGE_LANDSCAPE_HEIGHT - 145, &(adddressMicrochip[0][0]) );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_MEDIUM, USB_NULL, 0, 0 );
        
                            WriteLine( 100, PRINTER_PAGE_LANDSCAPE_HEIGHT - 120, &(adddressMicrochip[1][0]) );
                            WriteLine( 100, PRINTER_PAGE_LANDSCAPE_HEIGHT - 95, &(adddressMicrochip[2][0]) );
                            WriteLine( 100, PRINTER_PAGE_LANDSCAPE_HEIGHT - 70, &(adddressMicrochip[3][0]) );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_AVANT_GARDE, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_SIZE, USB_NULL, (DWORD)24, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_BOLD, USB_NULL, 0, 0 );
        
                            WriteLine( 450, 125, &(businessCard[0][0]) );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_MEDIUM, USB_NULL, 0, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_SIZE, USB_NULL, (DWORD)20, 0 );

                            WriteLine( 450, 150, &(businessCard[1][0]) );
                            WriteLine( 450, 175, &(businessCard[2][0]) );
                            WriteLine( 450, 200, &(businessCard[3][0]) );
                            WriteLine( 450, 225, &(businessCard[4][0]) );
                            WriteLine( 450, 250, &(businessCard[5][0]) );
                            WriteLine( 450, 275, &(businessCard[6][0]) );
                            WriteLine( 450, 300, &(businessCard[7][0]) );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_PALATINO, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_SIZE, USB_NULL, (DWORD)16, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_ITALIC, USB_NULL, 0, 0 );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE_WIDTH, USB_NULL, PRINTER_LINE_WIDTH_NORMAL, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE_TYPE, USB_NULL, PRINTER_LINE_TYPE_DASHED, 0 );

                            WriteLine( 120, 310, &(notes[0][0]) );
                            params.sLine.x1 = 170;
                            params.sLine.y1 = 290;
                            params.sLine.x2 = 220;
                            params.sLine.y2 = 260;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE, USB_DATA_POINTER_RAM(&params), sizeof(params.sLine), 0 );
        
                            WriteLine( 216, 396, &(notes[1][0]) );
                            params.sLine.x1 = 266;
                            params.sLine.y1 = 400;
                            params.sLine.x2 = 216;
                            params.sLine.y2 = 445;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE, USB_DATA_POINTER_RAM(&params), sizeof(params.sLine), 0 );
        
                            WriteLine( 440, 330, &(notes[2][0]) );
                            params.sLine.x1 = 490;
                            params.sLine.y1 = 310;
                            params.sLine.x2 = 515;
                            params.sLine.y2 = 280;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE, USB_DATA_POINTER_RAM(&params), sizeof(params.sLine), 0 );
        
                            WriteLine( 450, 410, &(notes[3][0]) );
                            params.sLine.x1 = 550;
                            params.sLine.y1 = 415;
                            params.sLine.x2 = 590;
                            params.sLine.y2 = 430;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE, USB_DATA_POINTER_RAM(&params), sizeof(params.sLine), 0 );
                            params.sLine.x1 = 500;
                            params.sLine.y1 = 415;
                            params.sLine.x2 = 500;
                            params.sLine.y2 = 540;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE, USB_DATA_POINTER_RAM(&params), sizeof(params.sLine), 0 );

                            #define TAO_UNIT    4
                            #define TAO_XL      (PRINTER_PAGE_LANDSCAPE_WIDTH - 200)
                            #define TAO_YT      (PRINTER_PAGE_LANDSCAPE_HEIGHT - 200)
                            #define TAO_XC      (TAO_XL + TAU_UNIT * 12)
                            #define TAO_YC      (TAO_YL + TAU_UNIT * 12)
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE_WIDTH, USB_NULL, PRINTER_LINE_WIDTH_NORMAL, 0 );
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_LINE_TYPE, USB_NULL, PRINTER_LINE_TYPE_SOLID, 0 );
        
                            params.sArc.xL       = TAO_XL;
                            params.sArc.yT       = TAO_YT;
                            params.sArc.xR       = TAO_XL + TAO_UNIT * 24;
                            params.sArc.yB       = TAO_YT + TAO_UNIT * 24;
                            params.sArc.r1       = 0;
                            params.sArc.r2       = TAO_UNIT * 12;
                            params.sArc.octant   = 0xF0;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_ARC, USB_DATA_POINTER_RAM(&params), sizeof(params.sArc), 0 );
        
                            params.sCircle.x    = TAO_XL + TAO_UNIT * 12;
                            params.sCircle.y    = TAO_YT + TAO_UNIT * 18;
                            params.sCircle.r    = TAO_UNIT * 6;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_CIRCLE_FILLED, USB_DATA_POINTER_RAM(&params), sizeof(params.sCircle), 0 );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_COLOR, USB_NULL, PRINTER_COLOR_WHITE, 0 );

                            params.sCircle.x    = TAO_XL + TAO_UNIT * 12;
                            params.sCircle.y    = TAO_YT + TAO_UNIT * 6;
                            params.sCircle.r    = TAO_UNIT * 6;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_CIRCLE_FILLED, USB_DATA_POINTER_RAM(&params), sizeof(params.sCircle), 0 );
        
                            params.sCircle.x    = TAO_XL + TAO_UNIT * 12;
                            params.sCircle.y    = TAO_YT + TAO_UNIT * 18;
                            params.sCircle.r    = TAO_UNIT;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_CIRCLE_FILLED, USB_DATA_POINTER_RAM(&params), sizeof(params.sCircle), 0 );
        
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_COLOR, USB_NULL, PRINTER_COLOR_BLACK, 0 );
        
                            params.sCircle.x    = TAO_XL + TAO_UNIT * 12;
                            params.sCircle.y    = TAO_YT + TAO_UNIT * 6;
                            params.sCircle.r    = TAO_UNIT;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_CIRCLE_FILLED, USB_DATA_POINTER_RAM(&params), sizeof(params.sCircle), 0 );
        
                            params.sCircle.x    = TAO_XL + TAO_UNIT * 12;
                            params.sCircle.y    = TAO_YT + TAO_UNIT * 12;
                            params.sCircle.r    = TAO_UNIT * 12;
                            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_CIRCLE, USB_DATA_POINTER_RAM(&params), sizeof(params.sCircle), 0 );
                        }
        
                        // Terminate
                        UART2PrintString( "Demo complete.\r\n" );
                        USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_JOB_STOP, USB_NULL, 0, 0 );
                    }
                }
            }
        }

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



