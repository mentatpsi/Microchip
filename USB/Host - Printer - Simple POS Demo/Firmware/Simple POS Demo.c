/******************************************************************************

Simple POS Demo

This is a simple POS printer demo to show how to send commands to a POS
printers.  When a printer attaches, a print sample is sent to the printer.
No other user interaction is required to generate the print-out.

This demo runs on an Explorer 16 (DM240001) with a PIC24FJ256GB110 (USB) PIM
(MA240014) and a USB PICtail Plus Daughter Board (AC164127).

In its initial state, this demo supports the printer:

    * Epson TM-T88IV - thermal receipt printer that uses the ESC/POS printer
        language.  This is a fast thermal printer that supports most of the
        features currently available in the ESC/POS printer language driver.

To modify this demo for a different printer, run the configuration tool
USBConfig.exe to generate the appropriate configuration files usb_config.c and
usb_config.h.

Refer to the Simple Full Sheet Demo for an example of sending commands to a
full sheet printer.

******************************************************************************/

/******************************************************************************

* FileName:         Simple POS Demo.c
* Dependencies:     USB Embedded Host library
* Processor:        PIC24F/PIC32
* Compiler:         C30 v3.10b/C32 v1.02
* Company:          Microchip Technology, Inc.

*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "uart2.h"
#include "GraphicsConfig.h"
#include "Graphics/Primitive.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_printer.h"
#include "USB/usb_host_printer_esc_pos.h"


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
    void PrintImagePOS( BYTE __prog__ *ptr, USB_PRINTER_IMAGE_INFO *imageInfo );
#elif defined( __PIC32MX__ )
    void PrintImagePOS( const BYTE *ptr, USB_PRINTER_IMAGE_INFO *imageInfo );
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

char                            buffer[50];
USB_PRINTER_IMAGE_INFO          imageInfo;
extern const IMAGE_FLASH        logoMCHP;
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
void PrintImagePOS( BYTE __prog__ *ptr, USB_PRINTER_IMAGE_INFO *imageInfo )
#elif defined( __PIC32MX__ )
void PrintImagePOS( const BYTE *ptr, USB_PRINTER_IMAGE_INFO *imageInfo )
#endif
{
    WORD                    currentRow;
    BYTE                    depthBytes;
    BYTE                    *imageDataPOS;
    BYTE                    returnCode;

    imageInfo->width    = ((WORD)ptr[5] << 8) + ptr[4];
    imageInfo->height   = ((WORD)ptr[3] << 8) + ptr[2];

    ptr += 10; // skip the header info (??, bits per pixel, Y size, X size, pallete[0], pallete[1] )

    depthBytes = (imageInfo->densityVertical + 7) / 8;     // Must be 1, 3, or 5.

    imageDataPOS = (BYTE *)malloc( imageInfo->width * depthBytes );
    if (imageDataPOS == NULL)
    {
        UART2PrintString( "No memory for POS image print\r\n" );
        return;
    }

    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_IMAGE_START, USB_DATA_POINTER_RAM(imageInfo), sizeof(USB_PRINTER_IMAGE_INFO), 0 );

    currentRow = 0;
    while (currentRow < imageInfo->height)
    {
        USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_IMAGE_DATA_HEADER, USB_NULL, imageInfo->width, 0 );

        ptr = USBHostPrinterPOSImageDataFormat( USB_DATA_POINTER_ROM(ptr), USB_PRINTER_TRANSFER_FROM_ROM, imageInfo->height, imageInfo->width, &currentRow, depthBytes, imageDataPOS ).pointerROM;
        USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_IMAGE_DATA, USB_DATA_POINTER_RAM(imageDataPOS), imageInfo->width, USB_PRINTER_TRANSFER_COPY_DATA);
    }

    free( imageDataPOS );

    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_IMAGE_STOP, USB_NULL, 0, 0 );
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

        #endif
        OSCCON = 0x3302;    // Enable secondary oscillator
        CLKDIV = 0x0000;    // Set PLL prescaler (1:1)
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

    //Device switches over automatically to PLL output after PLL is locked and ready.
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

    UART2PrintString( "\r\n\r\nUSB Embedded Host Simple POS Printer Demo\r\n" );

    // Initialize USB Embedded Host
    USBInitialize(0);

    // Turn on the A/D converter to monitor Vbus.
    //InitializeVbusMonitor();

    while(1)
    {
        //MonitorVBUS();
        USBTasks();

        if (status.printerAttached)
        {
            if (!status.pagePrinted)
            {
                status.pagePrinted = 1;

                UART2PrintString( "Printing to Epson TM-T88IV POS printer...\r\n" );

                // Initialize
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_JOB_START, USB_NULL, 0, 0 );

                // Center
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_JUSTIFICATION_CENTER, USB_NULL, 0, 0 );

                #ifdef USB_PRINTER_POS_24_DOT_IMAGE_SUPPORT
                    imageInfo.densityVertical   = 24;
                #else
                    imageInfo.densityVertical   = 8;
                #endif
                imageInfo.densityHorizontal = 2;    // Double density
                imageInfo.positionX         = 0;
                imageInfo.positionY         = 0;
                #if defined( __C30__ ) || defined __XC16__
                    PrintImagePOS( (BYTE __prog__ *)(logoMCHP.address), &imageInfo );
                #elif defined( __PIC32MX__ )
                    PrintImagePOS( (const BYTE *)(logoMCHP.address), &imageInfo );
                #endif

                // Font A
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_POS_18x36, 0 );

                // Text
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_TEXT_LINE, USB_DATA_POINTER_RAM("\n\nMicrochip Technology Inc."), 3, USB_PRINTER_TRANSFER_COPY_DATA );

                // Left
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_JUSTIFICATION_LEFT, USB_NULL, 0, 0 );

                // Font B
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_POS_12x24, 0 );

                // Text
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_TEXT_LINE, USB_DATA_POINTER_RAM("DM240001 Explorer 16 Demo Board                 $129.99"), 1, USB_PRINTER_TRANSFER_COPY_DATA );
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_TEXT_LINE, USB_DATA_POINTER_RAM("MA240014 PIC24F USB PIM                          $25.00"), 1, USB_PRINTER_TRANSFER_COPY_DATA );
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_TEXT_LINE, USB_DATA_POINTER_RAM("AC164131 USB PICtail Plus Daughter Board         $60.00"), 1, USB_PRINTER_TRANSFER_COPY_DATA );
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_TEXT_LINE, USB_DATA_POINTER_RAM("DM320003 PIC32 USB Starter Board                 $55.00"), 2, USB_PRINTER_TRANSFER_COPY_DATA );
                //USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_TEXT_LINE, USB_DATA_POINTER_RAM("AC164127 Graphics PICtail Plus Daughter Board   $135.00"), 2, USB_PRINTER_TRANSFER_COPY_DATA );

                // Double height
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_POS_12x48, 0 );

                // Center
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_JUSTIFICATION_CENTER, USB_NULL, 0, 0 );

                // Text
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_TEXT_LINE, USB_DATA_POINTER_RAM("Supporting POS Printers with PIC24F and PIC32..."), 3, USB_PRINTER_TRANSFER_COPY_DATA );

                // Bold
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_BOLD, USB_NULL, 0, 0 );

                // Largest internal font
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_POS_36x72, 0 );

                #ifdef USB_PRINTER_POS_REVERSE_TEXT_SUPPORT
                    // Reversed
                    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_FONT_REVERSE, USB_NULL, 1, 0 );
                #else
                    UART2PrintString( "This printer does not support reversed print.\r\n" );
                #endif

                // Text
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_TEXT_LINE, USB_DATA_POINTER_RAM(" PRICELESS!"), 3, USB_PRINTER_TRANSFER_COPY_DATA );

                // Normal weight
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_MEDIUM, USB_NULL, 0, 0 );

                #ifdef USB_PRINTER_POS_REVERSE_TEXT_SUPPORT
                    // Not reversed
                    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_FONT_REVERSE, USB_NULL, 0, 0 );
                #endif

                // Original font
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_FONT_NAME, USB_NULL, USB_PRINTER_FONT_POS_18x36, 0 );

                // Bar Code
                #ifdef USB_PRINTER_POS_BARCODE_SUPPORT
                    strcpy( buffer, "00808080116" );

                    params.sBarCode.height          = 75;       // Bar code height in dots.
                    params.sBarCode.type            = USB_PRINTER_POS_BARCODE_UPC_A;    // Bar code type.  See the USB_PRINTER_POS_BARCODE_FORMAT enumeration.
                    params.sBarCode.textPosition    = BARCODE_TEXT_BELOW;               // Position of the readable text.  Valid values are BARCODE_TEXT_OMIT, BARCODE_TEXT_ABOVE, BARCODE_TEXT_BELOW, BARCODE_TEXT_ABOVE_AND_BELOW.
                    params.sBarCode.textFont        = BARCODE_TEXT_12x24;               // Font of the readable text.  Valid values are dependent on the particular POS printer.
                    params.sBarCode.data            = (BYTE *)buffer;                   // Pointer to the barcode data.
                    params.sBarCode.dataLength      = 11;                               // Number of bytes of barcode data.
                    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_BARCODE, USB_DATA_POINTER_RAM(&params.sBarCode), sizeof(params.sBarCode), 0 );
                #else
                    UART2PrintString( "This printer does not support bar codes.\r\n" );
                #endif

                // Feed and cut
                #ifdef USB_PRINTER_POS_CUTTER_SUPPORT
                    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_CUT_PARTIAL, USB_NULL, 100, 0 );
                #else
                    UART2PrintString( "This printer does not have a cutter.\r\n" );
                    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_FEED, USB_NULL, 2, USB_PRINTER_TRANSFER_COPY_DATA );
                #endif

                // Terminate
                UART2PrintString( "Demo complete.\r\n" );
                USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_JOB_STOP, USB_NULL, 0, 0 );
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



