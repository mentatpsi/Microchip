/******************************************************************************
    ESC/POS Printer Language Support

  Summary:
    This file provides support for the ESC/POS printer language when using the
    USB Embedded Host Printer Client Driver.

  Description:
    This file provides support for the ESC/POS printer language when using the
    USB Embedded Host Printer Client Driver.

    The exact implementation of ESC/POS varies across manufacturers and even
    across different models from the same manufacturer.  Some POS printers use
    specialized Device ID strings (obtained with the GET DEVICE ID class-
    specific request) to indicate the deviations from the strict ESC/POS
    specification.  Also, many printers indicate a custom USB Peripheral Device
    rather than the Printer Class, and therefore do not support the
    GET DEVICE ID device request.  For example:
    <code>
    Printer          Tested VID/PID  Class    Device ID Substring
    -------------------------------------------------------------
    Bixolon SRP-270  0x0419/0x3C01   Printer  ESC
    Epson TM-T88IV   0x04B8/0x0202   Custom   Not supported
    Seiko DPU-V445   0x0619/0x0111   Printer  Not present
    Seiko MPU-L465   0x0619/0x0109   Printer  SIIMPU
    </code>
    Therefore, dynamic language determination is not recommended for POS
    printers.  Instead, create your application to target a either a single
    printer model or a group of printer models with identical requirements,
    and indicate specific VID, PID, and printer language via the USB
    Configuration Tool, and test the application to ensure consistent behavior
    across all supported printer models.

    The ESC/POS language support code provides several #defines that allow the
    language support file to automatically configure itself for different
    printer models.  These #defines can be set using the USB Configuration Tool
    (USBConfig.exe or MPLAB VDI).  Note that they are determined at compile
    time, not run time, so only one type of printer can be utilized by an
    application.  Printer models other that the ones explicitly tested may
    require other modifications to the language support code.

  Notes:
    Currently, only standard mode is supported.

    The black and white bit image polarity is 0=white, 1=black, which is
    reversed from the Microchip Graphics Library polarity.  This driver will
    automatically convert the image data to the required format, as long as the
    image data is located in ROM (USB_PRINTER_TRANSFER_FROM_ROM) or it is
    copied from a RAM buffer (USB_PRINTER_TRANSFER_COPY_DATA).  If the data is
    to be sent directly from its original RAM location, the data must already
    be in the format required by the printer language.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

 FileName:        usb_host_printer_esc_pos.h
 Dependencies:    None
 Processor:       PIC24F/PIC32MX
 Compiler:        C30/C32
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

Change History:
  Rev         Description
  ----------  ----------------------------------------------------------
  2.6 - 2.7a   No change

*******************************************************************************/
//DOM-IGNORE-END


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GenericTypeDefs.h"
#include "USB/usb.h"
#include "USB/usb_host_printer.h"
#include "USB/usb_host_printer_esc_pos.h"


//#define DEBUG_MODE
#if defined( DEBUG_MODE )
    #include "uart2.h"
#endif


#ifdef USB_PRINTER_LANGUAGE_ESCPOS


// *****************************************************************************
// *****************************************************************************
// Section: Configuration
// *****************************************************************************
// *****************************************************************************

#if !defined(USB_ENABLE_TRANSFER_EVENT)
    #error The USB Host Printer Client Driver requires transfer events.
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************


#define DC2                                 "\x12"
#define DC2_CHAR                            '\x12'
#define ESC                                 "\x1B"    //"\033"
#define ESC_CHAR                            '\x1B'
#define FF                                  "\x0C"
#define FF_CHAR                             '\x0C'
#define LF                                  "\x0A"
#define LF_CHAR                             '\x0A'
#define GS                                  "\x1D"
#define GS_CHAR                             '\x1D'
#define NUL                                 "\x00"
#define NUL_CHAR                            '\x00'

#define COMMAND_COLOR_BLACK                 ESC "r0"
#define COMMAND_COLOR_RED                   ESC "r1"
#define COMMAND_FILL_SHADED                 DC2 "$4\x01"
#define COMMAND_FILL_SOLID                  DC2 "$4\x02"
#define COMMAND_FONT_REVERSE_ON             GS "B1"
#define COMMAND_FONT_REVERSE_OFF            GS "B0"
#define COMMAND_FORMFEED                    FF
#define COMMAND_JOB_START                   ESCAPE "@"
#define COMMAND_JUSTIFY_CENTER              ESC "a1"
#define COMMAND_JUSTIFY_LEFT                ESC "a0"
#define COMMAND_JUSTIFY_RIGHT               ESC "a2"
//#define COMMAND_LINE_TYPE_DASHED            DC2 "$2\x01"
//#define COMMAND_LINE_TYPE_DOTTED            DC2 "$2\x02"
//#define COMMAND_LINE_TYPE_SOLID             DC2 "$2\x00"
//#define COMMAND_LINE_WIDTH                  DC2 "$3\xFF"
#define COMMAND_MODE_PAGE                   ESC "L"
#define COMMAND_MODE_STANDARD               ESC "S"
#define COMMAND_ORIENTATION_HORIZONTAL      ESC "V0" ESC "{0"
#define COMMAND_ORIENTATION_VERTICAL        ESC "V1"
#define COMMAND_ORIENTATION_UPSIDE_DOWN     ESC "{1"
//#define COMMAND_PRINT_AND_FEED              ESC "J\xFF"
//#define COMMAND_PRINT_LENGTH                DC2 "H"
//#define COMMAND_PRINT_WIDTH                 DC2 "K"
//#define COMMAND_RECTANGLE                   DC2 "$1"
//#define COMMAND_SET_PITCH                   GS "P\xFF\xFF"
//#define COMMAND_SET_POSITION_X              ESC "$"
//#define COMMAND_SET_POSITION_X_RELATIVE     ESC "\"
//#define COMMAND_SET_POSITION_Y              GS "$"
//#define COMMAND_SET_POSITION_Y_RELATIVE     GS "\"

#define COMMAND_SET_LEFT_MARGIN             GS "L\xFF\xFF"


// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************

//-----------------------------------------------------------------------------
/* Printer Status Structure

This structure holds the information about an attached printer.  One instance
of this structure is needed for each attached printer.
*/

typedef struct
{
    USB_PRINTER_FUNCTION_SUPPORT    support;    // The functions supported by this printer.

    WORD    currentHeight;  // The current height of the page in points.
    WORD    currentWidth;   // The current width of the page in points.

    WORD    currentX;       // Current X-axis position.
    WORD    currentY;       // Current Y-axis position.

    BYTE    deviceAddress;  // Address of the attached printer
    BYTE    fontName;       // Currently selected font, translated
    BYTE    fontSize;       // Size of the current font, vertical and horizontal scaling

    BYTE    density;        // Vertical and horizontal dot density specification
                            // of the currently printing image.
    BYTE    imageDataWidth; // Number of bytes to send for each column of the
                            // currently printing image;
    WORD    imageWidth;     // Dot width of the currently printing image.

    union
    {
        BYTE    value;

        struct
        {
            BYTE    isLandscape     : 1;    // Landscape(1) or portrait(0)
            BYTE    isBold          : 1;    // If the font is bold
            BYTE    isUnderlined    : 1;    // If the font is underlined
            BYTE    reversePrint    : 1;    // Text is printed in reversed colors.
            BYTE    inPageMode      : 1;    // If the printer is in Page or Standard mode.
        };
    } printerFlags;

} PRINTER_STATUS_ESC_POS;



// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

#ifdef USE_PRINTER_POS_EXTENDED_BARCODE_FORMAT
    BYTE                            barCodeFormats[USB_PRINTER_POS_BARCODE_MAX] =
                                    {
                                        65,     // USB_PRINTER_POS_BARCODE_UPC_A
                                        66,     // USB_PRINTER_POS_BARCODE_UPC_E
                                        67,     // USB_PRINTER_POS_BARCODE_EAN13
                                        68,     // USB_PRINTER_POS_BARCODE_EAN8
                                        69,     // USB_PRINTER_POS_BARCODE_CODE39
                                        70,     // USB_PRINTER_POS_BARCODE_ITF
                                        71,     // USB_PRINTER_POS_BARCODE_CODABAR
                                        72,     // USB_PRINTER_POS_BARCODE_CODE93
                                        73,     // USB_PRINTER_POS_BARCODE_CODE128
                                        74      // USB_PRINTER_POS_BARCODE_EAN128
                                    };
#endif

USB_PRINTER_GRAPHICS_PARAMETERS localParams;
PRINTER_STATUS_ESC_POS          printerListESCPOS[USB_MAX_PRINTER_DEVICES];


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define PAGE_IS_LANDSCAPE(x)    ((x & 0x01) == 0x01)
#define PAGE_IS_PORTRAIT(x)     ((x & 0x01) == 0x00)
#define FONT_IS_BOLD(x)         ((x & 0x02) == 0x02)
#define FONT_IS_ITALIC(x)       ((x & 0x04) == 0x04)

#define _SetCurrentPosition(x,y)                    \
    {                                               \
        printerListESCPOS[printer].currentX = x;    \
        printerListESCPOS[printer].currentY = y;    \
    }

#ifndef USB_MALLOC
    #define USB_MALLOC(size) malloc(size)
#endif

#ifndef USB_FREE
    #define USB_FREE(ptr) free(ptr)
#endif

#define USB_FREE_AND_CLEAR(ptr) {USB_FREE(ptr); ptr = NULL;}

// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************

#if defined( USB_PRINTER_POS_BARCODE_SUPPORT )
    static BYTE _BarcodeCharacterValueCodabar( char c );
    static BYTE _BarcodeCharacterValueCode39( char c );
    static char _BarcodeValueCharacterCodabar( BYTE v );
    static char _BarcodeValueCharacterCode39( BYTE v );
#endif
static BYTE _PrintFontCommand( BYTE printer, BYTE transferFlags );
static BYTE _PrintStaticCommand( BYTE printer, char *command, BYTE transferFlags );


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BYTE USBHostPrinterLanguageESCPOS( BYTE address,
        USB_PRINTER_COMMAND command, USB_DATA_POINTER data, DWORD size, BYTE transferFlags )

  Summary:
    This function executes printer commands for an ESC/POS printer.

  Description:
    This function executes printer commands for an ESC/POS printer.  When
    the application issues a printer command, the printer client driver
    determines what language to use to communicate with the printer, and
    transfers the command to that language support routine.  As much as
    possible, commands are designed to produce the same output regardless
    of what printer language is used.

    Not all printer commands support data from both RAM and ROM.  Unless
    otherwise noted, the data pointer is assumed to point to RAM, regardless of
    the value of transferFlags.  Refer to the specific command to see if ROM
    data is supported.

  Preconditions:
    None

  Parameters:
    BYTE address                - Device's address on the bus
    USB_PRINTER_COMMAND command - Command to execute.  See the enumeration
                                    USB_PRINTER_COMMAND for the list of
                                    valid commands and their requirements.
    USB_DATA_POINTER data    - Pointer to the required data.  Note that
                                    the caller must set transferFlags
                                    appropriately to indicate if the pointer is
                                    a RAM pointer or a ROM pointer.
    DWORD size                  - Size of the data.  For some commands, this
                                    parameter is used to hold the data itself.
    BYTE transferFlags          - Flags that indicate details about the
                                    transfer operation.  Refer to these flags
                                    * USB_PRINTER_TRANSFER_COPY_DATA
                                    * USB_PRINTER_TRANSFER_STATIC_DATA
                                    * USB_PRINTER_TRANSFER_NOTIFY
                                    * USB_PRINTER_TRANSFER_FROM_ROM
                                    * USB_PRINTER_TRANSFER_FROM_RAM

  Return Values:
    USB_PRINTER_SUCCESS             - The command was executed successfully.
    USB_PRINTER_UNKNOWN_DEVICE      - A printer with the indicated address is not
                                        attached
    USB_PRINTER_TOO_MANY_DEVICES    - The printer status array does not have
                                        space for another printer.
    USB_PRINTER_OUT_OF_MEMORY       - Not enough available heap space to
                                        execute the command.
    other                           - See possible return codes from the
                                        function USBHostPrinterWrite().

  Remarks:
    When developing new commands, keep in mind that the function
    USBHostPrinterCommandReady() will be used before calling this function to
    see if there is space available in the output transfer queue.
    USBHostPrinterCommandReady() will routine TRUE if a single space is
    available in the output queue.  Therefore, each command can generate only
    one output transfer.

    Multiple printer languages may be used in a single application.  The USB
    Embedded Host Printer Client Driver will call the routine required for the
    attached device.
  ***************************************************************************/

BYTE USBHostPrinterLanguageESCPOS( BYTE address,
        USB_PRINTER_COMMAND command, USB_DATA_POINTER data, DWORD size, BYTE transferFlags )
{
    char    *buffer;
    int     i;
    BYTE    printer = 0;

    if (command != USB_PRINTER_ATTACHED)
    {
        // Try to find the current printer.  If we can't find the printer in the list,
        // put it in the list at the first available location.
        for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListESCPOS[printer].deviceAddress != address); printer++ );
        if (printer == USB_MAX_PRINTER_DEVICES)
        {
            return USB_PRINTER_UNKNOWN_DEVICE;
        }
    }

    switch( command )
    {
        //---------------------------------------------------------------------
        case USB_PRINTER_ATTACHED:
            for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListESCPOS[printer].deviceAddress != 0); printer++ );
            if (printer != USB_MAX_PRINTER_DEVICES)
            {
                printerListESCPOS[printer].deviceAddress        = address;
                printerListESCPOS[printer].support              = *((USB_PRINTER_FUNCTION_SUPPORT *)(data.pointerRAM));
                printerListESCPOS[printer].printerFlags.value   = 0;
                return USB_PRINTER_SUCCESS;
            }
            return USB_PRINTER_TOO_MANY_DEVICES;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_DETACHED:
            for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListESCPOS[printer].deviceAddress != address); printer++ );
            if (printer != USB_MAX_PRINTER_DEVICES)
            {
                printerListESCPOS[printer].deviceAddress = 0;
            }
            return USB_PRINTER_SUCCESS;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_JOB_START:
            _SetCurrentPosition( 0, 0 );

            buffer = (char *)USB_MALLOC( 2 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            // Initialize printer
            buffer[0] = ESC_CHAR;
            buffer[1] = '@';

            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, 2, transferFlags );
            break;


        //---------------------------------------------------------------------
        case USB_PRINTER_JOB_STOP:
            // For compatibility with full sheet operation, if we are in page
            // mode, perform the USB_PRINTER_EJECT_PAGE command.
            if (printerListESCPOS[printer].printerFlags.inPageMode)
            {
                goto EjectPage;
            }
            return USB_PRINTER_SUCCESS;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_NAME:
            // If an illegal size is specified, return with an error.
            if (size >= USB_PRINTER_FONT_POS_MAX_FONT)
            {
                return USB_PRINTER_BAD_PARAMETER;
            }

            switch ((BYTE)size)
            {
                case USB_PRINTER_FONT_POS_18x36:
                    size = 0x00;
                    break;
                case USB_PRINTER_FONT_POS_18x72:
                    size = 0x10;
                    break;
                case USB_PRINTER_FONT_POS_36x36:
                    size = 0x20;
                    break;
                case USB_PRINTER_FONT_POS_36x72:
                    size = 0x30;
                    break;
                case USB_PRINTER_FONT_POS_12x24:
                    size = 0x01;
                    break;
                case USB_PRINTER_FONT_POS_12x48:
                    size = 0x11;
                    break;
                case USB_PRINTER_FONT_POS_24x24:
                    size = 0x21;
                    break;
                case USB_PRINTER_FONT_POS_24x48:
                    size = 0x31;
                    break;
            }

            printerListESCPOS[printer].fontName = (BYTE)size;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_UPRIGHT:
            // Italic printing is not supported, but we will not return an
            // error if the user specifies upright.
            return USB_PRINTER_SUCCESS;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_BOLD:
            printerListESCPOS[printer].printerFlags.isBold = 1;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_MEDIUM:
            printerListESCPOS[printer].printerFlags.isBold = 0;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_EJECT_PAGE:
EjectPage:
            buffer = (char *)USB_MALLOC( 1 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            buffer[0] = FF_CHAR;

            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, 1, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_TEXT_START:
            // No text initialization is required.
            return USB_PRINTER_SUCCESS;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_TEXT:
        case USB_PRINTER_TRANSPARENT:
            // If the user's data is in ROM, we have to copy it to RAM first,
            // so the USB Host routines can read it.
            if (transferFlags & USB_PRINTER_TRANSFER_FROM_ROM)
            {
                USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            }
            if (transferFlags & USB_PRINTER_TRANSFER_COPY_DATA)
            {
                buffer = (char *)USB_MALLOC( size );
                if (buffer == NULL)
                {
                    return USB_PRINTER_OUT_OF_MEMORY;
                }

                if (transferFlags & USB_PRINTER_TRANSFER_FROM_ROM)
                {
                    DWORD   di;
                    #if defined( __C30__ ) || defined __XC16__
                        char __prog__   *ptr;
                    #elif defined( __PIC32MX__ )
                        const char      *ptr;
                    #endif

                    ptr = ((USB_DATA_POINTER)data).pointerROM;
                    for (di=0; di<size; di++)
                    {
                        buffer[di] = *ptr++;
                    }
                }
                else
                {
                    DWORD   di;
                    char    *ptr;

                    ptr = ((USB_DATA_POINTER)data).pointerRAM;
                    for (di=0; di<size; di++)
                    {
                        buffer[di] = *ptr++;
                    }
                }
            }
            else
            {
                buffer = ((USB_DATA_POINTER)data).pointerRAM;
            }

            return USBHostPrinterWrite( address, buffer, size, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_TEXT_STOP:
            buffer = (char *)USB_MALLOC( 3 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            buffer[0] = ESC_CHAR;
            buffer[1] = 'd';
            buffer[2] = (BYTE)size;

            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, 3, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_START:
            #define VERTICAL_DENSITY        ((USB_PRINTER_IMAGE_INFO *)(data.pointerRAM))->densityVertical
            #define HORIZONTAL_DENSITY      ((USB_PRINTER_IMAGE_INFO *)(data.pointerRAM))->densityHorizontal

            // Check for legal density settings.
            #if defined( USB_PRINTER_POS_24_DOT_IMAGE_SUPPORT ) && defined( USB_PRINTER_POS_36_DOT_IMAGE_SUPPORT )
            if (!((VERTICAL_DENSITY == 8) || (VERTICAL_DENSITY == 24) || (VERTICAL_DENSITY == 36)))
            #elif defined( USB_PRINTER_POS_24_DOT_IMAGE_SUPPORT ) && !defined( USB_PRINTER_POS_36_DOT_IMAGE_SUPPORT )
            if (!((VERTICAL_DENSITY == 8) || (VERTICAL_DENSITY == 24)))
            #else
            if (!((VERTICAL_DENSITY == 8)))
            #endif
            {
                return USB_PRINTER_BAD_PARAMETER;
            }
            if (!((HORIZONTAL_DENSITY == 1) || (HORIZONTAL_DENSITY == 2)))
            {
                return USB_PRINTER_BAD_PARAMETER;
            }

            // Set up dot density specification.
            printerListESCPOS[printer].density          = 0;    // 8-dot image
            printerListESCPOS[printer].imageDataWidth   = 1;
            #if defined( USB_PRINTER_POS_24_DOT_IMAGE_SUPPORT ) && !defined( USB_PRINTER_POS_36_DOT_IMAGE_SUPPORT )
                if (VERTICAL_DENSITY == 24)
                {
                    printerListESCPOS[printer].density          = 32;
                    printerListESCPOS[printer].imageDataWidth   = 3;
                }
            #elif defined( USB_PRINTER_POS_24_DOT_IMAGE_SUPPORT ) && defined( USB_PRINTER_POS_36_DOT_IMAGE_SUPPORT )
                if (VERTICAL_DENSITY == 24)
                {
                    printerListESCPOS[printer].density          = 16;
                    printerListESCPOS[printer].imageDataWidth   = 3;
                }
                if (VERTICAL_DENSITY == 36)
                {
                    printerListESCPOS[printer].density          = 32;
                    printerListESCPOS[printer].imageDataWidth   = 5;
                }
            #endif
            if (HORIZONTAL_DENSITY == 2)
            {
                printerListESCPOS[printer].density ++;
            }

            printerListESCPOS[printer].imageWidth       = ((USB_PRINTER_IMAGE_INFO *)(data.pointerRAM))->width;

            buffer = (char *)USB_MALLOC( 3 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            i = 0;

            // Set line spacing to 8 dots, so there are no gaps between the image lines.
            buffer[i++] = ESC_CHAR;
            buffer[i++] = '3';
            buffer[i++] = USB_PRINTER_POS_IMAGE_LINE_SPACING;

            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, i, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_DATA_HEADER:
            buffer = (char *)USB_MALLOC( 6 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            i = 0;

            buffer[i++] = LF_CHAR;

            buffer[i++] = ESC_CHAR;
            buffer[i++] = '*';
            buffer[i++] = printerListESCPOS[printer].density;

            buffer[i++] = printerListESCPOS[printer].imageWidth & 0xFF;
            buffer[i++] = printerListESCPOS[printer].imageWidth >> 8;

            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, i, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_DATA:
            // The polarity of POS raster graphics (0=white, 1=black) is
            // backwards from other printer languages and the graphics library.
            // To maintain compatibility, the values will be flipped for all
            // copied data.

            // For ESC/POS, the amount of data to transfer is the width of the
            // image times the byte depth of the data, as specified by the
            // vertical dot density.
            size *= printerListESCPOS[printer].imageDataWidth;

            // If the user's data is in ROM, we have to copy it to RAM first,
            // so the USB Host routines can read it.
            if (transferFlags & USB_PRINTER_TRANSFER_FROM_ROM)
            {
                USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            }
            if (transferFlags & USB_PRINTER_TRANSFER_COPY_DATA)
            {
                buffer = (char *)USB_MALLOC( size );
                if (buffer == NULL)
                {
                    return USB_PRINTER_OUT_OF_MEMORY;
                }

                if (transferFlags & USB_PRINTER_TRANSFER_FROM_ROM)
                {
                    #if defined( __C30__ ) || defined __XC16__
                        char __prog__   *ptr;
                    #elif defined( __PIC32MX__ )
                        const char      *ptr;
                    #endif
                    DWORD   i;

                    ptr = ((USB_DATA_POINTER)data).pointerROM;
                    for (i=0; i<size; i++)
                    {
                        buffer[i] = ~(*ptr++);
                    }
                }
                else
                {
                    char    *ptr;
                    DWORD   i;

                    ptr = ((USB_DATA_POINTER)data).pointerRAM;
                    for (i=0; i<size; i++)
                    {
                        buffer[i] = ~(*ptr++);
                    }
                }
            }
            else
            {
                buffer = ((USB_DATA_POINTER)data).pointerRAM;
            }

            return USBHostPrinterWrite( address, buffer, size, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_STOP:
            // No termination required.
            //return USB_PRINTER_SUCCESS;
            buffer = (char *)USB_MALLOC( 3 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            i = 0;

            // Set line spacing back to the default
            buffer[i++] = ESC_CHAR;
            buffer[i++] = '2';

            // Feed after the last line.
            buffer[i++] = LF_CHAR;

            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, i, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_POS_STANDARD_MODE:
            printerListESCPOS[printer].printerFlags.inPageMode = 0;
            return _PrintStaticCommand( printer, COMMAND_MODE_STANDARD, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_POS_FEED:
            // Make sure the data pointer is NULL.
            data.pointerRAM = NULL;
            // Fall through the USB_PRINTER_POS_TEXT_LINE command processing.

        //---------------------------------------------------------------------
        case USB_PRINTER_POS_TEXT_LINE:
            if (transferFlags & USB_PRINTER_TRANSFER_FROM_ROM)
            {
                return USB_PRINTER_BAD_PARAMETER;
            }

            {
                WORD    length;
                char    *ptr;

                length  = 0;
                ptr     = ((USB_DATA_POINTER)data).pointerRAM;
                if (ptr != NULL)
                {
                    length = strlen( ptr );
                }
                buffer  = (char *)USB_MALLOC( length + 3 );
                if (buffer == NULL)
                {
                    return USB_PRINTER_OUT_OF_MEMORY;
                }

                for (i=0; i<length; i++)
                {
                    buffer[i] = *ptr++;
                }
                buffer[i++] = ESC_CHAR;
                buffer[i++] = 'd';
                buffer[i++] = (BYTE)size;

                USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
                return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, length + 3, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        #ifdef USB_PRINTER_POS_CUTTER_SUPPORT
        case USB_PRINTER_POS_CUT:
        case USB_PRINTER_POS_CUT_PARTIAL:
            // Using Function B
            buffer = (char *)USB_MALLOC( 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            // Copy cut command.
            buffer[0] = GS_CHAR;
            buffer[1] = 'V';
            buffer[2] = 65;   // Full cut

            // Replace partial cut if necessary.
            if (command == USB_PRINTER_POS_CUT_PARTIAL)
            {
                buffer[2] = 66;
            }

            // Fill in feed amount. The default vertical motion unit varies
            // between printers (often 1/360 inch).  Not all printers can
            // change this value.
            buffer[3] = (char)size;

            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, 4, transferFlags );
            break;
        #endif

        //---------------------------------------------------------------------
        case USB_PRINTER_POS_JUSTIFICATION_CENTER:
            return _PrintStaticCommand( printer, COMMAND_JUSTIFY_CENTER, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_POS_JUSTIFICATION_LEFT:
            return _PrintStaticCommand( printer, COMMAND_JUSTIFY_LEFT, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_POS_JUSTIFICATION_RIGHT:
            return _PrintStaticCommand( printer, COMMAND_JUSTIFY_RIGHT, transferFlags );
            break;

        //---------------------------------------------------------------------
        #ifdef USB_PRINTER_POS_REVERSE_TEXT_SUPPORT
        case USB_PRINTER_POS_FONT_REVERSE:
            printerListESCPOS[printer].printerFlags.reversePrint = size & 0x01;
            if (size == 0)
            {
                return _PrintStaticCommand( printer, COMMAND_FONT_REVERSE_OFF, transferFlags );
            }
            else
            {
                return _PrintStaticCommand( printer, COMMAND_FONT_REVERSE_ON, transferFlags );
            }
            break;
        #endif

        //---------------------------------------------------------------------
        case USB_PRINTER_POS_FONT_UNDERLINE:
            printerListESCPOS[printer].printerFlags.isUnderlined = size & 0x01;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        #ifdef USB_PRINTER_POS_TWO_COLOR_SUPPORT
        case USB_PRINTER_POS_COLOR_BLACK:
            return _PrintStaticCommand( printer, COMMAND_COLOR_BLACK, transferFlags );
            break;
        #endif

        //---------------------------------------------------------------------
        #ifdef USB_PRINTER_POS_TWO_COLOR_SUPPORT
        case USB_PRINTER_POS_COLOR_RED:
            return _PrintStaticCommand( printer, COMMAND_COLOR_RED, transferFlags );
            break;
        #endif

        //---------------------------------------------------------------------
        #ifdef USB_PRINTER_POS_BARCODE_SUPPORT
        case USB_PRINTER_POS_BARCODE:
            {
                #define BCD (BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.data

                WORD    j;
                BYTE    dataLength; // The length of the bar code data, possibly adjusted for a checkdigit.

                dataLength = ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.dataLength;

                // Do any preprocessing required for the particular bar code type.
                switch ((BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.type)
                {
                    case USB_PRINTER_POS_BARCODE_UPC_A:
                        // Validate the data length.
                        if (dataLength != 11)
                        {
                            return USB_PRINTER_BAD_PARAMETER;
                        }
                        break;

                    case USB_PRINTER_POS_BARCODE_UPC_E:
                        // Validate the data length.
                        if (!((dataLength == 6) || (dataLength == 7) || (dataLength == 11)))
                        {
                            return USB_PRINTER_BAD_PARAMETER;
                        }
                        // If the length is not 6, then the first data byte must be '0'.
                        if ((dataLength != 6) && (BCD[0] != '0'))
                        {
                            return USB_PRINTER_BAD_PARAMETER;
                        }
                        break;

                    case USB_PRINTER_POS_BARCODE_EAN13:
                        // Validate the data length.
                        if (dataLength != 12)
                        {
                            return USB_PRINTER_BAD_PARAMETER;
                        }
                        break;

                    case USB_PRINTER_POS_BARCODE_EAN8:
                        // Validate the data length, and that the first byte is '0'.
                        if ((dataLength != 7) || (BCD[0] != '0'))
                        {
                            return USB_PRINTER_BAD_PARAMETER;
                        }
                        break;

                    case USB_PRINTER_POS_BARCODE_CODE39:
                    case USB_PRINTER_POS_BARCODE_CODABAR:
                        if ((BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.flags.bits.bPrintCheckDigit)
                        {
                            // Add one for the check digit that we will add.
                            dataLength ++;
                        }    
                        break;

                    case USB_PRINTER_POS_BARCODE_ITF:
                        // Validate that the data length is an even number
                        if (dataLength & 0x01)
                        {
                            return USB_PRINTER_BAD_PARAMETER;
                        }
                        break;

#ifdef USE_PRINTER_POS_EXTENDED_BARCODE_FORMAT
                    case USB_PRINTER_POS_BARCODE_CODE93:
                    case USB_PRINTER_POS_BARCODE_CODE128:
                        // No special checks are required.
                        break;

                    case USB_PRINTER_POS_BARCODE_EAN128:
                        // NOT YET SUPPORTED
                        return USB_PRINTER_BAD_PARAMETER;
                        break;
#endif

                    default:
                        // Invalid bar code format.
                        return USB_PRINTER_BAD_PARAMETER;
                        break;
                }

                buffer = (char *)USB_MALLOC( 21 + dataLength );
                if (buffer == NULL)
                {
                    return USB_PRINTER_OUT_OF_MEMORY;
                }


                i = 0;

                // Set barcode height.
                buffer[i++] = GS_CHAR;
                buffer[i++] = 'h';
                buffer[i++] = (BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.height;

                // Set readable characters position.
                buffer[i++] = GS_CHAR;
                buffer[i++] = 'H';
                buffer[i++] = (BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.textPosition;

                // Set readable characters font.
                buffer[i++] = GS_CHAR;
                buffer[i++] = 'f';
                buffer[i++] = (BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.textFont;

                // Print the bar code.
                buffer[i++] = GS_CHAR;
                buffer[i++] = 'k';

                #ifdef USE_PRINTER_POS_EXTENDED_BARCODE_FORMAT
                    // Use format 2
                    buffer[i++] = barCodeFormats[(BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.type];
                    buffer[i++] = dataLength;
                #else
                    // Use format 1
                    buffer[i++] = (BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.type;
                #endif

                // Put in the data as required for each particular bar code type.
                switch ((BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.type)
                {
                    case USB_PRINTER_POS_BARCODE_UPC_A:
                    case USB_PRINTER_POS_BARCODE_UPC_E:
                    case USB_PRINTER_POS_BARCODE_EAN13:
                    case USB_PRINTER_POS_BARCODE_EAN8:
                        // Copy the data for printing.
                        for (j=0; j<dataLength; j++)
                        {
                            buffer[i++] = BCD[j];
                        }

                        // UPC and EAN bar codes have a check digit, but we will let the
                        // printer calculate the check digit for us!
                        break;

                    case USB_PRINTER_POS_BARCODE_CODE39:
                        if ((BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.flags.bits.bPrintCheckDigit)
                        {
                            // These must all be ASCII, so copy them straight over.
                            // Our length already accounts for the check digit that
                            // we will add.
                            for (j=0; j<dataLength-1; j++)
                            {
                                buffer[i++] = BCD[j];
                            }

                            // Calculate the check digit.  The printer will not automatically
                            // calculate and print the checkdigit for this format.
                            {
                                WORD    sum = 0;
    
                                for (j=0; j<dataLength-1; j++)
                                {
                                    sum += _BarcodeCharacterValueCode39( BCD[j] );
                                }
    
                                sum %= 43;
    
                                buffer[i++] = _BarcodeValueCharacterCode39( sum );
                            }
                        }    
                        else
                        {
                            // These must all be ASCII, so copy them straight over.
                            for (j=0; j<dataLength; j++)
                            {
                                buffer[i++] = BCD[j];
                            }
                        }                                    
                        break;

                    case USB_PRINTER_POS_BARCODE_CODABAR:
                        if ((BYTE)((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBarCode.flags.bits.bPrintCheckDigit)
                        {
                            // These must all be ASCII, so copy them straight over.
                            // Don't copy the stop character yet - it goes after the
                            // checkdigit.  Our length already accounts for the
                            // checkdigit.
                            for (j=0; j<dataLength-2; j++)
                            {
                                buffer[i++] = BCD[j];
                            }
    
                            // Calculate the check digit.  The printer will not automatically
                            // calculate and print the checkdigit for this format.
                            {
                                WORD    sum = 0;
    
                                for (j=0; j<dataLength-1; j++)
                                {
                                    sum += _BarcodeCharacterValueCodabar( BCD[j] );
                                }
    
                                sum &= 0x0F;
                                if (sum != 0)
                                {
                                    sum = 16 - sum;
                                }
    
                                buffer[i++] = _BarcodeValueCharacterCodabar( sum );
                            }
    
                            // Now copy the stop character.
                            buffer[i++] = BCD[dataLength-2];
                        }
                        else
                        {
                            // These must all be ASCII, so copy them straight over.
                            for (j=0; j<dataLength; j++)
                            {
                                buffer[i++] = BCD[j];
                            }
                        }        
                        break;

                    case USB_PRINTER_POS_BARCODE_ITF:
                    #ifdef USE_PRINTER_POS_EXTENDED_BARCODE_FORMAT
                    case USB_PRINTER_POS_BARCODE_CODE93:
                    #endif
                    default:
                        for (j=0; j<dataLength; j++)
                        {
                            buffer[i++] = BCD[j];
                        }
                        break;
                }

                #ifndef USE_PRINTER_POS_EXTENDED_BARCODE_FORMAT
                    buffer[i++] = 0;
                #endif
            }

            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, i, transferFlags );
            break;
        #endif

        //---------------------------------------------------------------------
        default:
            return USB_PRINTER_UNKNOWN_COMMAND;
            break;
    }
    return USB_PRINTER_UNKNOWN_COMMAND;
}


/****************************************************************************
  Function:
    BOOL USBHostPrinterLanguageESCPOSIsSupported( char *deviceID,
                USB_PRINTER_FUNCTION_SUPPORT *support )

  Description:
    This function determines if the printer with the given device ID string
    supports the ESC/POS printer language.

  Preconditions:
    None

  Parameters:
    char *deviceID  - Pointer to the "COMMAND SET:" portion of the device ID
                        string of the attached printer.
    USB_PRINTER_FUNCTION_SUPPORT *support   - Pointer to returned information
                        about what types of functions this printer supports.

  Return Values:
    TRUE    - The printer supports ESC/POS.
    FALSE   - The printer does not support ESC/POS.

  Remarks:
    The caller must first locate the "COMMAND SET:" section of the device ID
    string.  To ensure that only the "COMMAND SET:" section of the device ID
    string is checked, the ";" at the end of the section should be temporarily
    replaced with a NULL.  Otherwise, this function may find the printer
    language string in the comments or other section, and incorrectly indicate
    that the printer supports the language.

    Device ID strings are case sensitive.

    POS printers use specialized strings to indicate the deviations from the
    strict ESC/POS specification.  Also, many printers indicate a custom USB
    Peripheral Device class rather than the Printer Class, and therefore do
    not support the standard device request used to obtain this string.
    Therefore, dynamic language determination is not recommended for POS
    printers.  Instead, indicate specific VID, PID, and printer language via
    the USB Configuration Tool.
  ***************************************************************************/

BOOL USBHostPrinterLanguageESCPOSIsSupported( char *deviceID,
                USB_PRINTER_FUNCTION_SUPPORT *support )
{
    if (strstr( deviceID, LANGUAGE_ID_STRING_ESCPOS ))
    {
        support->val = LANGUAGE_SUPPORT_FLAGS_ESCPOS;
        return TRUE;
    }
    return FALSE;
}


/****************************************************************************
  Function:
    USB_DATA_POINTER USBHostPrinterPOSImageDataFormat( USB_DATA_POINTER image,
        BYTE imageLocation, WORD imageHeight, WORD imageWidth, WORD *currentRow,
        BYTE byteDepth, BYTE *imageData )

  Summary:
    This function formats data for a bitmapped image into the format required
    for sending to a POS printer.

  Description:
    This function formats data for a bitmapped image into the format required
    for sending to a POS printer.  Bitmapped images are stored one row of pixels
    at a time.  Suppose we have an image with vertical black bars, eight pixels
    wide and eight pixels deep.  The image would appear as the following pixels,
    where 0 indicates a black dot and 1 indicates a white dot:
    <code>
    0 1 0 1 0 1 0 1
    0 1 0 1 0 1 0 1
    0 1 0 1 0 1 0 1
    0 1 0 1 0 1 0 1
    0 1 0 1 0 1 0 1
    0 1 0 1 0 1 0 1
    0 1 0 1 0 1 0 1
    0 1 0 1 0 1 0 1
    </code>
    The stored bitmap of the data would contain the data bytes, where each byte
    is one row of data:
    <code>
    0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55
    </code>
    When printing to a full sheet printer, eight separate
    USB_PRINTER_IMAGE_DATA_HEADER / USB_PRINTER_IMAGE_DATA command combinations
    are required to print this image.

    POS printers, however, require image data formated either 8 dots or 24 dots
    deep, depending on the desired (and supported) vertical print density.  For
    a POS printer performing an 8-dot vertical density print, the data needs to
    be in this format:
    <code>
    0x00 0xFF 0x00 0xFF 0x00 0xFF 0x00 0xFF
    </code>
    When printing to a POS printer, only one
    USB_PRINTER_IMAGE_DATA_HEADER / USB_PRINTER_IMAGE_DATA command combination
    is required to print this image.

    This function supports 8-dot and 24-dot vertical densities by specifying
    the byteDepth parameter as either 1 (8-dot) or 3 (24-dot).

  Precondition:
    None

  Parameters:
    USB_DATA_POINTER image  Pointer to the image bitmap data.
    BYTE imageLocation      Location of the image bitmap data.  Valid values
                            are USB_PRINTER_TRANSFER_FROM_ROM and
                            USB_PRINTER_TRANSFER_FROM_RAM.
    WORD imageHeight        Height of the image in pixels.
    WORD imageWidth         Width of the image in pixels.
    WORD *currentRow        The current pixel row.  Upon return, this value is
                            updated to the next pixel row to print.
    BYTE byteDepth          The byte depth of the print.  Valid values are 1
                            (8-dot vertical density) and 3 (24-dot vertical
                            density).
    BYTE *imageData         Pointer to a RAM data buffer that will receive the
                            manipulated data to send to the printer.

  Returns:
    The function returns a pointer to the next byte of image data.

  Example:
    The following example code will send a complete bitmapped image to a POS
    printer.
    <code>
        WORD                    currentRow;
        BYTE                    depthBytes;
        BYTE                    *imageDataPOS;
        USB_PRINTER_IMAGE_INFO  imageInfo;
        BYTE                    returnCode;
        #if defined (__C30__) || defined __XC16__
            BYTE __prog__       *ptr;
            ptr = (BYTE __prog__ *)logoMCHP.address;
        #elif defined (__PIC32MX__)
            const BYTE          *ptr;
            ptr = (const BYTE *)logoMCHP.address;
        #endif

        imageInfo.densityVertical   = 24;   // 24-dot density
        imageInfo.densityHorizontal = 2;    // Double density

        // Extract the image height and width
        imageInfo.width    = ((WORD)ptr[5] << 8) + ptr[4];
        imageInfo.height   = ((WORD)ptr[3] << 8) + ptr[2];

        depthBytes         = imageInfo.densityVertical / 8;
        imageDataPOS       = (BYTE *)USB_MALLOC( imageInfo.width *
                                       depthBytes );

        if (imageDataPOS == NULL)
        {
            // Error - not enough heap space
        }

        USBHostPrinterCommandWithReadyWait( &returnCode,
              printerInfo.deviceAddress, USB_PRINTER_IMAGE_START,
              USB_DATA_POINTER_RAM(&imageInfo),
              sizeof(USB_PRINTER_IMAGE_INFO ),
              0 );

        ptr += 10; // skip the header info

        currentRow = 0;
        while (currentRow < imageInfo.height)
        {
            USBHostPrinterCommandWithReadyWait( &returnCode,
              printerInfo.deviceAddress,
              USB_PRINTER_IMAGE_DATA_HEADER, USB_NULL,
              imageInfo.width, 0 );

            ptr = USBHostPrinterPOSImageDataFormat(
              USB_DATA_POINTER_ROM(ptr),
              USB_PRINTER_TRANSFER_FROM_ROM, imageInfo.height,
              imageInfo.width, &currentRow, depthBytes,
              imageDataPOS ).pointerROM;

            USBHostPrinterCommandWithReadyWait( &returnCode,
              printerInfo.deviceAddress, USB_PRINTER_IMAGE_DATA,
              USB_DATA_POINTER_RAM(imageDataPOS), imageInfo.width,
              USB_PRINTER_TRANSFER_COPY_DATA);
        }

        USB_FREE( imageDataPOS );

        USBHostPrinterCommandWithReadyWait( &returnCode,
              printerInfo.deviceAddress, USB_PRINTER_IMAGE_STOP,
              USB_NULL, 0, 0 );
      </code>

  Remarks:
    This routine currently does not support 36-dot density printing.  Since
    the output for 36-dot vertical density is identical to 24-dot vertical
    density, 24-dot vertical density should be used instead.

    This routine does not yet support reading from external memory.
  ***************************************************************************/

USB_DATA_POINTER USBHostPrinterPOSImageDataFormat( USB_DATA_POINTER image,
        BYTE imageLocation, WORD imageHeight, WORD imageWidth, WORD *currentRow,
        BYTE byteDepth, BYTE *imageData )
{
    BYTE                currentByte = 0x00;
    WORD                j;
    WORD                k;
    WORD                m;
    BYTE                maskHorizontal;
    BYTE                maskVertical;
    BYTE                *ptrRAM = NULL;
    #if defined( __C30__ ) || defined __XC16__
        BYTE __prog__   *ptrROM = NULL;
    #elif defined( __PIC32MX__ )
        const BYTE      *ptrROM = NULL;
    #endif
    WORD                widthBytes;


    maskHorizontal  = 0x80;
    switch( imageLocation )
    {
        case USB_PRINTER_TRANSFER_FROM_ROM:
            ptrROM      = ((USB_DATA_POINTER)image).pointerROM;
            break;

        case USB_PRINTER_TRANSFER_FROM_RAM:
            ptrRAM      = ((USB_DATA_POINTER)image).pointerRAM;
            break;

        //case USB_PRINTER_TRANSFER_FROM_EXTERNAL:
        default:
            return USB_NULL;
    }

    widthBytes      = (imageWidth + 7) / 8;

    // 0=dot, 1=no dot
    for (j=0; j<imageWidth * byteDepth; j++)
    {
        imageData[j] = 0xFF;
    }

    for (j=0; j<imageWidth; j++)
    {
        for (m=0; m<byteDepth; m++)
        {
            maskVertical = 0x80;
            for (k=0; k<8; k++)
            {
                if ((*currentRow + (m*8) + k) < imageHeight)
                {
                    switch( imageLocation )
                    {
                        case USB_PRINTER_TRANSFER_FROM_ROM:
                            currentByte = ptrROM[(widthBytes*(m*8 + k)) + (j/8)];
                            break;

                        case USB_PRINTER_TRANSFER_FROM_RAM:
                            currentByte = ptrRAM[(widthBytes*(m*8 + k)) + (j/8)];
                            break;

                        //case USB_PRINTER_TRANSFER_FROM_EXTERNAL:
                        //    break;
                    }
                    if (!(currentByte & maskHorizontal))
                    {
                        imageData[j*byteDepth + m] &= ~maskVertical;
                    }
                }
                maskVertical >>= 1;
            }
        }
        maskHorizontal >>= 1;
        if (maskHorizontal == 0)
        {
            maskHorizontal = 0x80;
        }
    }

    *currentRow += 8*byteDepth;
    switch( imageLocation )
    {
        case USB_PRINTER_TRANSFER_FROM_ROM:
            return (USB_DATA_POINTER)(image.pointerROM + (imageWidth * byteDepth));
            break;

        case USB_PRINTER_TRANSFER_FROM_RAM:
            return (USB_DATA_POINTER)(image.pointerRAM + (imageWidth * byteDepth));
            break;

        //case USB_PRINTER_TRANSFER_FROM_EXTERNAL:
        default:
            return USB_NULL;
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************

#ifdef USB_PRINTER_POS_BARCODE_SUPPORT

#define NUM_OTHERS_CODABAR      6
#define NUM_OTHERS_CODE39       7

static char _OthersCodabar[NUM_OTHERS_CODABAR]  = { '-', '$', ':', '/', '.', '+' };
static char _OthersCode39[NUM_OTHERS_CODE39]    = { '-', '.', ' ', '$', '/', '+', '%' };

#endif

/****************************************************************************
  Function:
    static BYTE _BarcodeCharacterValueCodabar( char c )

  Description:
    This function determines the value of a character when determining a
    Codabar checksum digit.

  Preconditions:
    None

  Parameters:
    char c  - Character in the bar code.

  Returns:
    The value to use for the indicated character when determining the check
    digit.

  Remarks:
    Use the function _BarcodeValueCharacterCodabar() to perform the reverse
    procedure.
  ***************************************************************************/

#ifdef USB_PRINTER_POS_BARCODE_SUPPORT

static BYTE _BarcodeCharacterValueCodabar( char c )
{
    int         i;

    if (('0' <= c) && (c <= '9'))
    {
        return (c - '0');
    }

    if (('A' <= c) && (c <= 'D'))
    {
        return (c - 'A' + 16);
    }

    for (i=0; i<NUM_OTHERS_CODABAR; i++)
    {
        if (c == _OthersCodabar[i])
        {
            return (i + 10);
        }
    }

    return 0;
}

#endif

/****************************************************************************
  Function:
    static BYTE _BarcodeCharacterValueCode39( char c )

  Description:
    This function determines the value of a character when determining a
    Code39 checksum digit.

  Preconditions:
    None

  Parameters:
    char c  - Character in the bar code.

  Returns:
    The value to use for the indicated character when determining the check
    digit.

  Remarks:
    Use the function _BarcodeValueCharacterCode39() to perform the reverse
    procedure.
  ***************************************************************************/

#if defined( USB_PRINTER_POS_BARCODE_SUPPORT )

static BYTE _BarcodeCharacterValueCode39( char c )
{
    int         i;

    if (('0' <= c) && (c <= '9'))
    {
        return (c - '0');
    }

    if (('A' <= c) && (c <= 'Z'))
    {
        return (c - 'A' + 10);
    }

    for (i=0; i<NUM_OTHERS_CODE39; i++)
    {
        if (c == _OthersCode39[i])
        {
            return (i + 36);
        }
    }

    return 0;
}

#endif

/****************************************************************************
  Function:
    static char _BarcodeValueCharacterCodabar( BYTE v )

  Description:
    This function determines the character used to represent a particular
    value for a Codabar checksum digit.

  Preconditions:
    None

  Parameters:
    BYTE v  - Value of the required character.

  Returns:
    The character to use for the indicated value for the check digit.

  Remarks:
    Use the function _BarcodeCharacterValueCodabar() to perform the reverse
    procedure.
  ***************************************************************************/

#ifdef USB_PRINTER_POS_BARCODE_SUPPORT

static char _BarcodeValueCharacterCodabar( BYTE v )
{
    if (v > 19)
    {
        return '0';     // Error
    }

    if (v <= 9)
    {
        return (v + '0');
    }

    if ((16 <= v) && (v <= 19))
    {
        return (v - 16 + 'A');
    }

    return _OthersCodabar[v - 10];
}

#endif

/****************************************************************************
  Function:
    static char _BarcodeValueCharacterCode39( BYTE v )

  Description:
    This function determines the character used to represent a particular
    value for a Code39 checksum digit.

  Preconditions:
    None

  Parameters:
    BYTE v  - Value of the required character.

  Returns:
    The character to use for the indicated value for the check digit.

  Remarks:
    Use the function _BarcodeCharacterValueCode39() to perform the reverse
    procedure.
  ***************************************************************************/

#if defined( USB_PRINTER_POS_BARCODE_SUPPORT )

static char _BarcodeValueCharacterCode39( BYTE v )
{
    if (v > 42)
    {
        return '0';     // Error
    }

    if (v <= 9)
    {
        return (v + '0');
    }

    if ((10 <= v) && (v <= 35))
    {
        return (v - 10 + 'A');
    }

    return _OthersCode39[v - 36];
}

#endif

/****************************************************************************
  Function:
    static BYTE _PrintFontCommand( BYTE printer, BYTE transferFlags )

  Description:
    This command sends a complete font command to the printer.  It is called
    whenever there is a change to the font, the weight, or the underlining.

  Preconditions:
    None

  Parameters:
    BYTE printer        - Index of the target printer.
    BYTE transferFlags  - Transfer control string.

  Return Values:
    USB_PRINTER_SUCCESS         - Command completed successfully.
    USB_PRINTER_OUT_OF_MEMORY   - Not enough dynamic memory to perform the
                                    command.
    others                      - See the return values for the function
                                    USBHostPrinterWrite().

  Remarks:
    The font command parameter is comprised of the following bits:
    <code>
        Bit Value   Meaning
        -------------------
        0   0       Font A
            1       Font B
        1           Reserved
        2           Reserved
        3   0       Medium weight
            1       Bold
        4   0       Normal height
            1       Double height
        5   0       Normal width
            1       Double width
        6           Reserved
        7   0       No underline
            1       Underline
    </code>
  ***************************************************************************/

static BYTE _PrintFontCommand( BYTE printer, BYTE transferFlags )
{
    char    *buffer;

    buffer = (char *)USB_MALLOC( 3 );
    if (buffer == NULL)
    {
        return USB_PRINTER_OUT_OF_MEMORY;
    }

    buffer[0]   = ESC_CHAR;
    buffer[1]   = '!';
    buffer[2]   = printerListESCPOS[printer].fontName;

    if (printerListESCPOS[printer].printerFlags.isBold)
    {
        buffer[2] |= 0x08;
    }
    if (printerListESCPOS[printer].printerFlags.isUnderlined)
    {
        buffer[2] |= 0x80;
    }

    USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
    return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, 3, transferFlags );
}


/****************************************************************************
  Function:
    static BYTE _PrintStaticCommand( BYTE printer, char *command, BYTE transferFlags )

  Description:
    This function sends a hard-coded command to the printer.  Many printer
    commands are constant text strings that require no parameters.  This
    routine allocates dynamic memory for a copy of the command, copies the
    command into the string, and sends the command to the printer.

  Preconditions:
    None

  Parameters:
    BYTE printer        - Index of the target printer.
    char *command       - Printer command string.
    BYTE transferFlags  - Transfer control string.

  Return Values:
    USB_PRINTER_SUCCESS         - Command completed successfully.
    USB_PRINTER_OUT_OF_MEMORY   - Not enough dynamic memory to perform the
                                    command.
    others                      - See the return values for the function
                                    USBHostPrinterWrite().

  Remarks:
    None
  ***************************************************************************/

static BYTE _PrintStaticCommand( BYTE printer, char *command, BYTE transferFlags )
{
    char    *buffer;

    USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );

    buffer = (char *)USB_MALLOC( strlen(command) + 1 );
    if (buffer == NULL)
    {
        return USB_PRINTER_OUT_OF_MEMORY;
    }
    strcpy( buffer, command );
    return USBHostPrinterWrite( printerListESCPOS[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
}


#endif

