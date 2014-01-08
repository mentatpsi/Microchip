/******************************************************************************
    PCL 5 Printer Language Support

  Summary:
    This file provides support for the PCL 5 printer language when using the
    USB Embedded Host Printer Client Driver.

  Description:
    This file provides support for the PCL 5 printer language when using the
    USB Embedded Host Printer Client Driver.

    There are several versions of the PCL printer language.  This file is
    targetted to support PCL 5.  Unfortunately, printer language support is not
    always advertised correctly by the printer.  Some printers advertise only
    PCL 6 support when they also support PCL 5.  Therefore, the default value
    for the LANGUAGE_ID_STRING_PCL string used in the routine
    USBHostPrinterLanguagePCL5IsSupported() is set such that the routine will
    return TRUE if any PCL language support is advertised.  It is highly
    recommended to test the target application with the specific printer(s)
    that will be utilized, and, if possible, populate the
    usbPrinterSpecificLanguage[] array in usb_config.c via the configuration
    tool to manually select the printer language and its functional support.

  Notes:
    The PCL 5 coordinate origin is located at the top left corner of the paper.
    The HP-GL/2 coordinate origin, however, is located at the bottom left corner
    of the page.  For consistency for the user, HP-GL/2 coordinate system is
    adjusted to match the PCL coordinate system.   This also matches the
    coordinate system use by the Microchip Graphics library.

    The black and white bit image polarity is 0=white, 1=black, which is
    reversed from the Microchip Graphics Library polarity.  This driver will
    automatically convert the image data to the required format, as long as the
    image data is located in ROM (USB_PRINTER_TRANSFER_FROM_ROM) or it is
    copied from a RAM buffer (USB_PRINTER_TRANSFER_COPY_DATA).  If the data is
    to be sent directly from its original RAM location, the data must already
    be in the format required by the printer language.

    PCL 5 is not compatible with PCL 6; PCL 5 utilizes ASCII input, whereas
    PCL 6 utilizes binary data.  However, some printers that advertise support
    for only PCL 5 do support PCL 6.

    PCL 3 printers utilize many of the PCL 5 commands.  The following
    limitations exist with PCL 3:
        * PCL 3 does not support vector graphics
        * PCL 3 does not support image printing in landscape mode.  The
            page print will fail.
        * Items must be sent to the page in the order that they are to be
            printed from top to bottom.  The printer cannot return to a
            higher position on the page.
        * PCL 3 does not support the USB_PRINTER_EJECT_PAGE command.  Use the
            USB_PRINTER_JOB_STOP and USB_PRINTER_JOB_START commands instead.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

 FileName:        usb_host_printer_pcl_5.c
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
  2.6 - 2.6a  No change

  2.7         Changed the interface to _SetCurrentPosition to be able to
              take in the printer number as a parameter.
*******************************************************************************/
//DOM-IGNORE-END

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GenericTypeDefs.h"
#include "USB/usb.h"
#include "USB/usb_host_printer.h"
#include "USB/usb_host_printer_pcl_5.h"

//#define DEBUG_MODE
#if defined( DEBUG_MODE )
    #include "uart2.h"
#endif


#ifdef USB_PRINTER_LANGUAGE_PCL_5

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

#define PCL_PAGE_BOTTOM                     792
#define PCL_PAGE_LEFT                       0
#define PCL_PAGE_RIGHT                      612
#define PCL_PAGE_TOP                        0

#define ESCAPE                              "\033"
#define ESCAPE_CHAR                         '\033'

#define COMMAND_ASCII                       ESCAPE "(0U"            // Zero
#define COMMAND_FONT_BOLD                   ESCAPE "(s3B"
#define COMMAND_FONT_BOLD_VG                "6,3,7,"
#define COMMAND_FONT_ITALIC                 ESCAPE "(s1S"
#define COMMAND_FONT_ITALIC_VG              "5,1,"
#define COMMAND_FONT_MEDIUM                 ESCAPE "(s0B"
#define COMMAND_FONT_MEDIUM_VG              "6,0,7,"
#define COMMAND_FONT_UPRIGHT                ESCAPE "(s0S"
#define COMMAND_FONT_UPRIGHT_VG             "5,0,"
#define COMMAND_SELECT_STANDARD             "SS;"
#define COMMAND_STANDARD_FONT               "SD1,277,2,"
#define COMMAND_STANDARD_FONT_FIXED         "0,3,"
#define COMMAND_STANDARD_FONT_PROPORTIONAL  "1,4,"
#define COMMAND_TEXT_START                  "LB"
#define COMMAND_TEXT_STOP                   "\003;"

#define COMMAND_GRAPHICS_ANCHOR_CORNER      "AC%d,%d;"
#define COMMAND_GRAPHICS_ARC_RELATIVE       "AR%d,%d,%d;"
#define COMMAND_GRAPHICS_BAR                "RA%d,%d;"
#define COMMAND_GRAPHICS_CIRCLE             "CI%d;"
#define COMMAND_GRAPHICS_CIRCLE_FILLED      "WG%d,0,360;"
#define COMMAND_GRAPHICS_COLOR_BLACK        "SP1;"
#define COMMAND_GRAPHICS_COLOR_WHITE        "SP0;"
#define COMMAND_GRAPHICS_FILL_TYPE_SOLID    "FT1;"
#define COMMAND_GRAPHICS_FILL_TYPE_SHADE    "FT10,%d;"
#define COMMAND_GRAPHICS_FILL_TYPE_HATCH    "FT3,%d,%d;"
#define COMMAND_GRAPHICS_LINE               "PA%d,%d;PD%d,%d;"
#define COMMAND_GRAPHICS_LINE_END_BUTT      "LA1,1;"
#define COMMAND_GRAPHICS_LINE_END_ROUND     "LA1,4;"
#define COMMAND_GRAPHICS_LINE_END_SQUARE    "LA1,2;"
#define COMMAND_GRAPHICS_LINE_JOIN_BEVEL    "LA2,5;"
#define COMMAND_GRAPHICS_LINE_JOIN_MITER    "LA2,1;"
#define COMMAND_GRAPHICS_LINE_JOIN_ROUND    "LA2,4;"
#define COMMAND_GRAPHICS_LINE_TO            "PD%d,%d;"
#define COMMAND_GRAPHICS_LINE_TO_RELATIVE   "PD;PR%d,%d;"
#define COMMAND_GRAPHICS_LINE_TYPE_DASHED   "LT2,4,0;"
#define COMMAND_GRAPHICS_LINE_TYPE_DOTTED   "LT1,4,0;"
#define COMMAND_GRAPHICS_LINE_TYPE_SOLID    "LT;"
#define COMMAND_GRAPHICS_LINE_WIDTH_NORMAL  "PW;"
#define COMMAND_GRAPHICS_LINE_WIDTH_THICK   "PW1.059;"  // 3 points
#define COMMAND_GRAPHICS_MOVE_TO            "PA%d,%d;"
#define COMMAND_GRAPHICS_MOVE_RELATIVE      "PR%d,%d;"
#define COMMAND_GRAPHICS_ORIENT_LANDSCAPE   ESCAPE "*p0x0Y" ESCAPE "*c7920x5760Y" ESCAPE "*c0T" ESCAPE "%1B;IN;SP1;TR0;SC0,1.411,0,-1.411,2;IR0,100,0,100;" //"RO270;IP;"
#define COMMAND_GRAPHICS_ORIENT_PORTRAIT    ESCAPE "*p0x0Y" ESCAPE "*c5760x7920Y" ESCAPE "*c0T" ESCAPE "%1B;IN;SP1;TR0;SC0,1.411,0,-1.411,2;IR0,100,0,100;" //"RO;IP;"
#define COMMAND_GRAPHICS_PEN_UP             "PU;"
#define COMMAND_GRAPHICS_PEN_DOWN           "PD;"
#define COMMAND_GRAPHICS_RECTANGLE          "EA%d,%d;"
#define COMMAND_GRAPHICS_TERMINATE          ESCAPE "%1A"
#define COMMAND_GRAPHICS_WEDGE              "WG%d,%d,%d;"

#define COMMAND_JOB_START                   ESCAPE "%-12345X" ESCAPE "E" ESCAPE "&l0E" ESCAPE "&l0F" ESCAPE "&a0L" ESCAPE "&a0M" //ESCAPE "&a0N"
#define COMMAND_JOB_STOP                    ESCAPE "E" ESCAPE "%-12345X"
#define COMMAND_LANDSCAPE                   ESCAPE "&l1O"           // Little L, one, Capital o
#define COMMAND_PORTRAIT                    ESCAPE "&l0O"           // Little L, zero, Capital o
#define COMMAND_NEXT_PAGE                   ESCAPE "&a1G"
#define COMMAND_POSITION_HORIZONTAL         ESCAPE "&a%dH"
#define COMMAND_POSITION_VERTICAL           ESCAPE "&a%dV"
#define COMMAND_PROPORTIONAL                ESCAPE "(s1P"
#define COMMAND_FIXED                       ESCAPE "(s0P"
#define COMMAND_RASTER_COMPRESSION_ADAPT    ESCAPE "*b5M"
#define COMMAND_RASTER_COMPRESSION_DELTA    ESCAPE "*b3M"
#define COMMAND_RASTER_COMPRESSION_NONE     ESCAPE "*b0M"
#define COMMAND_RASTER_COMPRESSION_RES      ESCAPE "*b4M"
#define COMMAND_RASTER_COMPRESSION_RLE      ESCAPE "*b1M"
#define COMMAND_RASTER_COMPRESSION_TIFF     ESCAPE "*b2M"
#define COMMAND_RASTER_DATA                 ESCAPE "*b%dW"      // In bytes of data
#define COMMAND_RASTER_END                  ESCAPE "*rC"
#define COMMAND_RASTER_HEIGHT               ESCAPE "*r%dT"     // In pixels
#define COMMAND_RASTER_PRESENTATION         ESCAPE "*r0F"
#define COMMAND_RASTER_RESOLUTION           ESCAPE "*t%dR"
#define COMMAND_RASTER_START                ESCAPE "*r1A"
#define COMMAND_RASTER_WIDTH                ESCAPE "*r%dS"     // In pixels
//#define COMMAND_RASTER_Y_OFFSET             ESCAPE "*b%dY"      // In pixels

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

typedef struct _PRINTER_STATUS_PCL
{
    USB_PRINTER_FUNCTION_SUPPORT    support;    // The functions supported by this printer.

    WORD    currentHeight;  // The current height of the page in points.
    WORD    currentWidth;   // The current width of the page in points.

    WORD    currentX;       // Current X-axis position.
    WORD    currentY;       // Current Y-axis position.

    BYTE    deviceAddress;  // Address of the attached printer
    BYTE    fontName;       // Currently selected font
    BYTE    fontSize;       // Size of the current font

    BYTE    imageEndMask;   // Some PCL printers do not ignore bits after the specified
                            // image width.  We must explicitly set these to 0, or we
                            // will get a line down the right side of the image.

    union
    {
        BYTE    value;

        struct
        {
            BYTE    isLandscape     : 1;    // Landscape(1) or portrait(0)
            BYTE    isBold          : 1;    // If the font is bold
            BYTE    isItalic        : 1;    // If the font is italic
            BYTE    penIsWhite      : 1;    // Current selected pen is white.
        };
    } printerFlags;

} PRINTER_STATUS_PCL;


// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

const char          _pclFontNames[USB_PRINTER_FONT_MAX_FONT][10] = {
                    ESCAPE "(s24607T",      // Avant Garde
                    ESCAPE "(s24623T",      // Bookman
                    ESCAPE "(s3T",          // Courier
                    ESCAPE "(s24580T",      // Helvetica
                    ESCAPE "(s24580T",      // Helvetica Narrow
                    ESCAPE "(s24703T",      // New Century Schoolbook
                    ESCAPE "(s24591T",      // Palatino
                    ESCAPE "(s25093T" };    // Times Roman

const char          _pclFontNamesVG[USB_PRINTER_FONT_MAX_FONT][4] = {
                    "31",   // Avant Garde
                    "47",   // Bookman
                    "3",    // Courier
                    "4",    // Helvetica
                    "4",    // Helvetica Narrow
                    "23",   // New Century Schoolbook
                    "15",   // Palatino
                    "5" };  // Times Roman (277)

PRINTER_STATUS_PCL  printerListPCL[USB_MAX_PRINTER_DEVICES];


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define PAGE_IS_LANDSCAPE(x)        ((x & 0x01) == 0x01)
#define PAGE_IS_PORTRAIT(x)         ((x & 0x01) == 0x00)
#define FONT_IS_BOLD(x)             ((x & 0x02) == 0x02)
#define FONT_IS_ITALIC(x)           ((x & 0x04) == 0x04)

#define USING_VECTOR_GRAPHICS       (printerListPCL[printer].support.supportFlags.supportsVectorGraphics)

#define X_COORDINATE_IN_RANGE(x)    ((0 <= (x)) && ((x) <= printerListPCL[printer].currentWidth))
#define Y_COORDINATE_IN_RANGE(y)    ((0 <= (y)) && ((y) <= printerListPCL[printer].currentHeight))


#define _SetCurrentPosition(p,x,y)              \
    {                                           \
        printerListPCL[(p)].currentX = (x);     \
        printerListPCL[(p)].currentY = (y);     \
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

static BYTE _PrintFontCommand( BYTE printer, BYTE transferFlags );
static BYTE _PrintStaticCommand( BYTE address, char *command, BYTE transferFlags );


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BYTE USBHostPrinterLanguagePCL5( BYTE address,
        USB_PRINTER_COMMAND command, USB_DATA_POINTER data, DWORD size, BYTE transferFlags )

  Summary:
    This function executes printer commands for a PCL 5 printer.

  Description:
    This function executes printer commands for a PCL 5 printer.  When
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

BYTE USBHostPrinterLanguagePCL5( BYTE address,
        USB_PRINTER_COMMAND command, USB_DATA_POINTER data, DWORD size, BYTE transferFlags )
{
    char    *buffer;
    BYTE    printer = 0;

    if (command != USB_PRINTER_ATTACHED)
    {
        // Try to find the current printer.  If we can't find the printer in the list,
        // put it in the list at the first available location.
        for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListPCL[printer].deviceAddress != address); printer++ );
        if (printer == USB_MAX_PRINTER_DEVICES)
        {
            return USB_PRINTER_UNKNOWN_DEVICE;
        }
    }

    // The USB_PRINTER_SET_POSITION should be used only when not using vector graphics.
    if ((command == USB_PRINTER_SET_POSITION) && (USING_VECTOR_GRAPHICS))
    {
        command = USB_PRINTER_GRAPHICS_MOVE_TO;
    }

    // If we are connected to a PCL printer that does not support vector graphis,
    // such as PCL 3, return an error if the command is a vector graphics command.
    if ((command > USB_PRINTER_VECTOR_GRAPHICS_START) && (command < USB_PRINTER_VECTOR_GRAPHICS_END) && !USING_VECTOR_GRAPHICS)
    {
        return USB_PRINTER_UNKNOWN_COMMAND;
    }

    switch( command )
    {
        //---------------------------------------------------------------------
        case USB_PRINTER_ATTACHED:
            for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListPCL[printer].deviceAddress != 0); printer++ );
            if (printer != USB_MAX_PRINTER_DEVICES)
            {
                printerListPCL[printer].deviceAddress = address;
                printerListPCL[printer].support       = *((USB_PRINTER_FUNCTION_SUPPORT *)(data.pointerRAM));
                return USB_PRINTER_SUCCESS;
            }
            return USB_PRINTER_TOO_MANY_DEVICES;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_DETACHED:
            for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListPCL[printer].deviceAddress != address); printer++ );
            if (printer != USB_MAX_PRINTER_DEVICES)
            {
                printerListPCL[printer].deviceAddress = 0;
            }
            return USB_PRINTER_SUCCESS;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_JOB_START:
            printerListPCL[printer].fontName                = USB_PRINTER_FONT_HELVETICA;
            printerListPCL[printer].fontSize                = 12;
            printerListPCL[printer].printerFlags.value      = 0;
            printerListPCL[printer].currentHeight           = PRINTER_PAGE_PORTRAIT_HEIGHT;
            printerListPCL[printer].currentWidth            = PRINTER_PAGE_PORTRAIT_WIDTH;
            _SetCurrentPosition( printer, 0, 0 );
            if (USING_VECTOR_GRAPHICS)
            {
                return _PrintStaticCommand( address, COMMAND_JOB_START COMMAND_GRAPHICS_ORIENT_PORTRAIT, transferFlags );
            }
            else
            {
                return _PrintStaticCommand( address, COMMAND_JOB_START, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_JOB_STOP:
            if (USING_VECTOR_GRAPHICS)
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_TERMINATE COMMAND_JOB_STOP, transferFlags | USB_PRINTER_TRANSFER_NOTIFY);
            }
            else
            {
                return _PrintStaticCommand( address, COMMAND_JOB_STOP, transferFlags | USB_PRINTER_TRANSFER_NOTIFY);
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_ORIENTATION_PORTRAIT:
            printerListPCL[printer].printerFlags.isLandscape = 0;
            printerListPCL[printer].currentHeight            = PRINTER_PAGE_PORTRAIT_HEIGHT;
            printerListPCL[printer].currentWidth             = PRINTER_PAGE_PORTRAIT_WIDTH;
            if (USING_VECTOR_GRAPHICS)
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_TERMINATE COMMAND_PORTRAIT COMMAND_GRAPHICS_ORIENT_PORTRAIT, transferFlags );
            }
            else
            {
                return _PrintStaticCommand( address, COMMAND_PORTRAIT, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_ORIENTATION_LANDSCAPE:
            printerListPCL[printer].printerFlags.isLandscape = 1;
            printerListPCL[printer].currentHeight            = PRINTER_PAGE_LANDSCAPE_HEIGHT;
            printerListPCL[printer].currentWidth             = PRINTER_PAGE_LANDSCAPE_WIDTH;
            if (USING_VECTOR_GRAPHICS)
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_TERMINATE COMMAND_LANDSCAPE COMMAND_GRAPHICS_ORIENT_LANDSCAPE, transferFlags );
            }
            else
            {
                return _PrintStaticCommand( address, COMMAND_LANDSCAPE, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_NAME:
            // Font name is passed in the size parameter.
            printerListPCL[printer].fontName = (BYTE)size;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_SIZE:
            // Font size is passed in the size parameter.
            printerListPCL[printer].fontSize = (BYTE)size;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_ITALIC:
            printerListPCL[printer].printerFlags.isItalic = 1;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_UPRIGHT:
            printerListPCL[printer].printerFlags.isItalic = 0;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_BOLD:
            printerListPCL[printer].printerFlags.isBold = 1;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_MEDIUM:
            printerListPCL[printer].printerFlags.isBold = 0;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_EJECT_PAGE:
            _SetCurrentPosition( printer, 0, 0 );
            if (USING_VECTOR_GRAPHICS)
            {
                if ( printerListPCL[printer].printerFlags.isLandscape)
                {
                    return _PrintStaticCommand( address, COMMAND_GRAPHICS_TERMINATE COMMAND_NEXT_PAGE COMMAND_GRAPHICS_ORIENT_LANDSCAPE, transferFlags );
                }
                else
                {
                    return _PrintStaticCommand( address, COMMAND_GRAPHICS_TERMINATE COMMAND_NEXT_PAGE COMMAND_GRAPHICS_ORIENT_PORTRAIT, transferFlags );
                }
            }
            // If not using vector graphics, then this command is not supported.
            // Use USB_PRINTER_JOB_STOP and USB_PRINTER_JOB_START instead.
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_TEXT_START:
            if (USING_VECTOR_GRAPHICS)
            {
                return _PrintStaticCommand( address, COMMAND_TEXT_START, transferFlags );
            }
            else
            {
                // No processing required.
                return USB_PRINTER_SUCCESS;
            }
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
                    #if defined( __C30__ ) || defined __XC16__
                        char __prog__   *ptr;
                    #elif defined( __PIC32MX__ )
                        const char      *ptr;
                    #endif
                    DWORD   i;

                    ptr = ((USB_DATA_POINTER)data).pointerROM;
                    for (i=0; i<size; i++)
                    {
                        buffer[i] = *ptr++;
                    }
                }
                else
                {
                    char    *ptr;
                    DWORD   i;

                    ptr = ((USB_DATA_POINTER)data).pointerRAM;
                    for (i=0; i<size; i++)
                    {
                        buffer[i] = *ptr++;
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
            if (USING_VECTOR_GRAPHICS)
            {
                return _PrintStaticCommand( address, COMMAND_TEXT_STOP, transferFlags );
            }
            else
            {
                // No processing required.
                return USB_PRINTER_SUCCESS;
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_SET_POSITION:
            // Used only when not doing vector graphics.
            // This command sets the cursor to the specified position.  Note
            // that we must convert the specification to use decipoints.
            buffer = (char *)USB_MALLOC( 10 + 10 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_POSITION_HORIZONTAL COMMAND_POSITION_VERTICAL, (WORD)(size >> 16) * 10, (WORD)size * 10 );
            _SetCurrentPosition( printer, (WORD)(size >> 16), (WORD)size );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_START:
            // This command sets up the printer for printing raster data.
            buffer = (char *)USB_MALLOC( 4 + 8 + 8 + 6 + 11 + 11 + 11 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            {
                BYTE                    usedBits;
                USB_PRINTER_IMAGE_INFO  *info;
                BYTE                    mask;
                WORD                    resolution;

                info = (USB_PRINTER_IMAGE_INFO *)(data.pointerRAM);

                // Determine the mask for the last image byte, so we don't get a spurious
                // line on some printers.
                printerListPCL[printer].imageEndMask    = 0;
                mask                                    = 0x80;
                usedBits                                = info->width & 0x07;
                if (!usedBits)
                {
                    usedBits = 8;
                }
                while (usedBits > 0)
                {
                    printerListPCL[printer].imageEndMask |= mask;
                    mask >>= 1;
                    usedBits--;
                }

                // Set the resolution to a known good value.
                if (info->resolution >= 600)
                {
                    resolution = 600;
                }
                else if (info->resolution >= 300)
                {
                    resolution = 300;
                }
                else if (info->resolution >= 200)
                {
                    resolution = 200;
                }
                else if (info->resolution >= 150)
                {
                    resolution = 150;
                }
                else if (info->resolution >= 100)
                {
                    resolution = 100;
                }
                else
                {
                    resolution = 75;
                }

                if (USING_VECTOR_GRAPHICS)
                {
                    strcpy( buffer, COMMAND_GRAPHICS_TERMINATE );
                    sprintf( &(buffer[strlen(buffer)]),  COMMAND_POSITION_HORIZONTAL COMMAND_POSITION_VERTICAL COMMAND_RASTER_PRESENTATION COMMAND_RASTER_RESOLUTION COMMAND_RASTER_HEIGHT COMMAND_RASTER_WIDTH COMMAND_RASTER_START,
                        info->positionX * 10, info->positionY * 10, resolution, info->height, info->width );
                }
                else
                {
                    sprintf( buffer, COMMAND_POSITION_HORIZONTAL COMMAND_POSITION_VERTICAL COMMAND_RASTER_PRESENTATION COMMAND_RASTER_RESOLUTION COMMAND_RASTER_HEIGHT COMMAND_RASTER_WIDTH COMMAND_RASTER_START,
                        info->positionX * 10, info->positionY * 10, resolution, info->height, info->width );
                }
            }
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_DATA_HEADER:
            // This command sends the command for the raster data.  Therefore, the
            // command USB_PRINTER_IMAGE_DATA must follow this command.
            buffer = (char *)USB_MALLOC( 20 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_RASTER_COMPRESSION_NONE COMMAND_RASTER_DATA, (WORD)((size+7)/8) );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_DATA:
            // The polarity of PCL raster graphics (0=white, 1=black) is
            // backwards from other printer languages and the graphics library.
            // To maintain compatibility, the values will be flipped for all
            // copied data.

            // ??? Do we want to mandate the flip?  It will maintain consistency,
            // but will not allow direct access to RAM data, and will be slower.

            // Determine the number of data bytes.  We might have a size that is
            // not an even multiple of 8 bits.  To account for this, add 7 to the
            // size, and then divide by 8.
            size += 7;
            size /= 8;

            // If the user's data is in ROM, we have to copy it to RAM first,
            // so the USB Host routines can read it.
            if (transferFlags & USB_PRINTER_TRANSFER_FROM_ROM)
            {
                USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            }
            if (transferFlags & USB_PRINTER_TRANSFER_COPY_DATA)
            {
                DWORD   i;

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

                    ptr = ((USB_DATA_POINTER)data).pointerROM;
                    for (i=0; i<size; i++)
                    {
                        buffer[i] = ~(*ptr++);
                    }
                }
                else
                {
                    char    *ptr;

                    ptr = ((USB_DATA_POINTER)data).pointerRAM;
                    for (i=0; i<size; i++)
                    {
                        buffer[i] = ~(*ptr++);
                    }
                }
                buffer[i-1] &= printerListPCL[printer].imageEndMask;
            }
            else
            {
                buffer = ((USB_DATA_POINTER)data).pointerRAM;
            }

            return USBHostPrinterWrite( address, buffer, size, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_STOP:
            if (USING_VECTOR_GRAPHICS)
            {
                if (printerListPCL[printer].printerFlags.isLandscape)
                {
                   return _PrintStaticCommand( address, COMMAND_RASTER_END COMMAND_LANDSCAPE COMMAND_GRAPHICS_ORIENT_LANDSCAPE, transferFlags );
                }
                else
                {
                   return _PrintStaticCommand( address, COMMAND_RASTER_END COMMAND_PORTRAIT COMMAND_GRAPHICS_ORIENT_PORTRAIT, transferFlags );
                }
            }
            else
            {
                return _PrintStaticCommand( address, COMMAND_RASTER_END, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_TYPE:
            if (size == PRINTER_LINE_TYPE_SOLID)
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_TYPE_SOLID, transferFlags );
            }
            else //PRINTER_LINE_TYPE_DASHED
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_TYPE_DASHED, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_WIDTH:
            if (size == PRINTER_LINE_WIDTH_NORMAL)
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_WIDTH_NORMAL, transferFlags );
            }
            else //PRINTER_LINE_WIDTH_THICK
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_WIDTH_THICK, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_END:
            if (size == PRINTER_LINE_END_BUTT)
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_END_BUTT, transferFlags );
            else if (size == PRINTER_LINE_END_ROUND)
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_END_ROUND, transferFlags );
            else //PRINTER_LINE_END_SQUARE
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_END_SQUARE, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_JOIN:
            if (size == PRINTER_LINE_JOIN_BEVEL)
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_JOIN_BEVEL, transferFlags );
            if (size == PRINTER_LINE_JOIN_MITER)
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_JOIN_MITER, transferFlags );
            else // PRINTER_LINE_JOIN_ROUND
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_JOIN_ROUND, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_FILL_TYPE:
            if ((((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sFillType.fillType == PRINTER_FILL_SHADED) ||
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sFillType.fillType == PRINTER_FILL_HATCHED) ||
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sFillType.fillType == PRINTER_FILL_CROSS_HATCHED))
            {
                buffer = (char *)USB_MALLOC( 14 );
                if (buffer == NULL)
                {
                    return USB_PRINTER_OUT_OF_MEMORY;
                }

                if (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sFillType.fillType == PRINTER_FILL_SHADED)
                {
                    sprintf( buffer, COMMAND_GRAPHICS_FILL_TYPE_SHADE, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sFillType.shading );
                }
                else
                {
                    sprintf( buffer, COMMAND_GRAPHICS_FILL_TYPE_HATCH, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sFillType.spacing,
                            ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sFillType.angle );
                    if (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sFillType.fillType == PRINTER_FILL_CROSS_HATCHED)
                    {
                        buffer[2] = '4';
                    }
                }
                USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
                return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            }
            else // Default to PRINTER_FILL_SOLID.
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_FILL_TYPE_SOLID, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_COLOR:
            if (size == PRINTER_COLOR_BLACK)
            {
                 printerListPCL[printer].printerFlags.penIsWhite = 0;
                 return _PrintStaticCommand( address, COMMAND_GRAPHICS_COLOR_BLACK, transferFlags );
            }
            else //PRINTER_COLOR_WHITE
            {
                printerListPCL[printer].printerFlags.penIsWhite = 1;
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_COLOR_WHITE, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_MOVE_TO:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE((WORD)(size >> 16)) || !Y_COORDINATE_IN_RANGE((WORD)size ))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 14 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO, (WORD)(size >> 16) * 10, (WORD)size * 10);
            _SetCurrentPosition( printer, (WORD)(size >> 16), (WORD)size );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_MOVE_RELATIVE:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE( printerListPCL[printer].currentX + (int)(size >> 16) ) || !Y_COORDINATE_IN_RANGE( printerListPCL[printer].currentY + (int)(size & 0xFFFF) ))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 16 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_MOVE_RELATIVE, (int)(size >> 16) * 10, (int)(size & 0xFFFF) * 10 );
            _SetCurrentPosition( printer, printerListPCL[printer].currentX + (int)(size >> 16), printerListPCL[printer].currentY + (int)(size & 0xFFFF) );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x1) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y1) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x2) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y2))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 28 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_LINE COMMAND_GRAPHICS_PEN_UP,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x1 * 10, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y1 * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x2 * 10, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y2 * 10);
            _SetCurrentPosition( printer, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x2, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y2 );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_TO:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE( (int)(size >> 16) ) || !Y_COORDINATE_IN_RANGE( (int)(size & 0xFFFF) ))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif
            buffer = (char *)USB_MALLOC( 18 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_LINE_TO COMMAND_GRAPHICS_PEN_UP, (int)(size >> 16) * 10, (int)(size & 0xFFFF) * 10) ;
            _SetCurrentPosition( printer, (int)(size >> 16), (int)(size & 0xFFFF) );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_TO_RELATIVE:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE( printerListPCL[printer].currentX + (int)(size >> 16) ) || !Y_COORDINATE_IN_RANGE( printerListPCL[printer].currentY + (int)(size & 0xFFFF) ))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif
            buffer = (char *)USB_MALLOC( 22 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_LINE_TO_RELATIVE COMMAND_GRAPHICS_PEN_UP, (int)(size >> 16) * 10, (int)(size & 0xFFFF) * 10 ) ;
            _SetCurrentPosition( printer, printerListPCL[printer].currentX + (int)(size >> 16), printerListPCL[printer].currentY + (int)(size & 0xFFFF) );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_ARC:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xL) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yT) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xR) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yB))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( (30 + 3)* 2 + 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            {
                int     a1;
                int     a2;
                BYTE    mask;
                BYTE    maskPrevious;

                // Find the angles that define the arc.

                if (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.octant == 0)
                {
                    a1 = 0;
                    a2 = 0;
                }
                else if (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.octant == 0xFF)
                {
                    a1 = 0;
                    a2 = 360;
                }
                else
                {
                    // Look for the first edge of the arc.
                    a1 = 0;
                    mask = 0x02;
                    maskPrevious = 0x04;
                    while (!((mask & ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.octant) && !(maskPrevious & ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.octant)))
                    {
                        a1 += 45;
                        maskPrevious = mask;
                        mask >>= 1;
                        if (mask == 0)
                        {
                            mask = 0x80;
                        }
                    }

                    // Look for the second edge of the arc.  Note that the second angle is the sweep, not position.
                    a2 = 0;
                    while (mask & ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.octant)
                    {
                        a2 += 45;
                        mask >>= 1;
                        if (mask == 0)
                        {
                            mask = 0x80;
                        }
                    }
                }

                // Draw the outer circle of the arc.

                sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_WEDGE,
                    (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xR + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xL) * 10 / 2,
                    (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yT + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yB) * 10 / 2,
                     ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.r2 * 10, a1, a2 );

                if (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.r1 != 0)
                {
                    // Change pens

                    if (printerListPCL[printer].printerFlags.penIsWhite)
                    {
                        strcat( buffer, COMMAND_GRAPHICS_COLOR_BLACK );
                    }
                    else
                    {
                        strcat( buffer, COMMAND_GRAPHICS_COLOR_WHITE );
                    }

                    // Draw the inner circle to erase the interior.

                    sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_WEDGE,
                        (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xR + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xL) * 10 / 2,
                        (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yT + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yB) * 10 / 2,
                         ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.r1 * 10, a1, a2 );

                    // Put the original pen back.

                    if (printerListPCL[printer].printerFlags.penIsWhite)
                    {
                        strcat( buffer, COMMAND_GRAPHICS_COLOR_WHITE );
                    }
                    else
                    {
                        strcat( buffer, COMMAND_GRAPHICS_COLOR_BLACK );
                    }
                }

                strcat ( buffer, COMMAND_GRAPHICS_PEN_UP );
            }
            _SetCurrentPosition( printer, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xL, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yT );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_CIRCLE:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r))
                {
                    #ifdef DEBUG_MODE
                        UART2PutHexWord(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x);
                        UART2PutChar( ' ' );
                        UART2PutHexWord(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y);
                        UART2PutChar( ' ' );
                        UART2PutHexWord(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r);
                        UART2PutChar( ' ' );
                    #endif
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 12 + 8 + 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_CIRCLE COMMAND_GRAPHICS_PEN_UP,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x * 10, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r * 10 );
            _SetCurrentPosition( printer, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_CIRCLE_FILLED:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r))
                {
                    #ifdef DEBUG_MODE
                        UART2PutHexWord(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x);
                        UART2PutChar( ' ' );
                        UART2PutHexWord(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y);
                        UART2PutChar( ' ' );
                        UART2PutHexWord(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r);
                        UART2PutChar( ' ' );
                    #endif
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 12 + 12+ 14 + 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_ANCHOR_CORNER COMMAND_GRAPHICS_CIRCLE_FILLED COMMAND_GRAPHICS_PEN_UP,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x * 10, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x * 10, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r * 10 );
            _SetCurrentPosition( printer, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_BEVEL:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 12 + (24 + 18) * 4 + 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            // PAxL,yB-r;PDxL,yB-r,xL,yT+r;
            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_LINE,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10 );

            // ARr,0,-90;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC_RELATIVE,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r * 10, 0, 90 );

            // PDxL+r,yT,xR-r,yT;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_LINE,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT * 10 );

            // AR0,r,-90;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC_RELATIVE,
                0, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r * 10, 90 );

            // PDxR,yT+r,xR,yB-r;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_LINE,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10 );

            // AR-r,0,-90;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC_RELATIVE,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r * -1 * 10, 0, 90 );

            // PDxR-r,yB,xL+r;yB;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_LINE,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB * 10 );

            // AR0,-r,-90;PU;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC_RELATIVE COMMAND_GRAPHICS_PEN_UP,
                0, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r * -1 * 10, 90 );

            _SetCurrentPosition( printer, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_BEVEL_FILLED:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 12 + 2 * (12 + 12) + 4 * (12 + 13) + 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            // PAxL+r,yB; RAxR-r,yT;
            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_ANCHOR_CORNER COMMAND_GRAPHICS_BAR,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT * 10);

            // PAxL,yB-r; RAxR,yT+r;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_BAR,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10);

            // PAxL+r,yB-r; WGr,180,90;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_WEDGE,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r * 10, 180, -90 );

            // PAxL+r,yT+r; WGr,90,90;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_WEDGE,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r * 10, 270, -90 );

            // PAxR-r,yT+r; WGr,0,90;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_WEDGE,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r * 10, 0, -90 );

            // PAxR-r,yB-r; WGr,270,90; PU;
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_WEDGE,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r) * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r * 10, 90, -90 );

            _SetCurrentPosition( printer,  ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_RECTANGLE:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yT) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xR) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 12 + 12 + 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_RECTANGLE COMMAND_GRAPHICS_PEN_UP,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yT * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xR * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB * 10 );
            _SetCurrentPosition( printer, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_RECTANGLE_FILLED:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yT) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xR) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 12 + 12 + 12 + 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_ANCHOR_CORNER COMMAND_GRAPHICS_BAR COMMAND_GRAPHICS_PEN_UP,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yT * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yT * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xR * 10,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB * 10);
            _SetCurrentPosition( printer,  ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_POLYGON:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
            {
                WORD    i;

                for (i = 0; i < ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.numPoints * 2; i+=2)
                {
                    if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[i]) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[i+1]))
                    {
                        return USB_PRINTER_BAD_PARAMETER;
                    }
                }
            }
            #endif

            buffer = (char *)USB_MALLOC( 12 + (10 * (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.numPoints + 1)) + 4 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            {
                WORD    i = 0;

                sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO "PD",
                            ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[0] * 10,
                            ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[1] * 10 );

                // xn,yn,
                while (i < ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.numPoints * 2)
                {
                    sprintf( &(buffer[strlen(buffer)]), "%d,%d,",
                        ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[i]   * 10,
                        ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[i+1] * 10);
                    i += 2;
                }

                sprintf( &(buffer[strlen(buffer)]), "%d,%d;" COMMAND_GRAPHICS_PEN_UP,
                    ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[0] * 10,
                    ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[1] * 10);
            }

            _SetCurrentPosition( printer, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[0], ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[1] );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        default:
            return USB_PRINTER_UNKNOWN_COMMAND;
            break;
    }
    return USB_PRINTER_UNKNOWN_COMMAND;
}


/****************************************************************************
  Function:
    BOOL USBHostPrinterLanguagePCL5IsSupported( char *deviceID,
                USB_PRINTER_FUNCTION_SUPPORT *support )

  Summary:
    This function determines if the printer with the given device ID string
    supports the PCL 5 printer language.

  Description:
    This function determines if the printer with the given device ID string
    supports the PCL 5 printer language.

    Unfortunately, printer language support is not always advertised correctly
    by the printer.  Some printers advertise only PCL 6 support when they also
    support PCL 5.  Therefore, this routine will return TRUE if any PCL
    language support is advertised.  It is therefore highly recommended to test
    the target application with the specific printer(s) that will be utilized.

  Preconditions:
    None

  Parameters:
    char *deviceID  - Pointer to the "COMMAND SET:" portion of the device ID
                        string of the attached printer.
    USB_PRINTER_FUNCTION_SUPPORT *support   - Pointer to returned information
                        about what types of functions this printer supports.

  Return Values:
    TRUE    - The printer supports PCL 5.
    FALSE   - The printer does not support PCL 5.

  Remarks:
    The caller must first locate the "COMMAND SET:" section of the device ID
    string.  To ensure that only the "COMMAND SET:" section of the device ID
    string is checked, the ";" at the end of the section should be temporarily
    replaced with a NULL.  Otherwise, this function may find the printer
    language string in the comments or other section, and incorrectly indicate
    that the printer supports the language.

    Device ID strings are case sensitive.
  ***************************************************************************/

BOOL USBHostPrinterLanguagePCL5IsSupported( char *deviceID,
                USB_PRINTER_FUNCTION_SUPPORT *support )
{
    if (strstr( deviceID, LANGUAGE_ID_STRING_PCL ))
    {
        // When we add checks for PCL 3, this will have to change.  PCL3 does
        // not support vector graphics.
        support->val = LANGUAGE_SUPPORT_FLAGS_PCL5;
        return TRUE;
    }
    return FALSE;
}


// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    static BYTE _PrintFontCommand( BYTE printer, BYTE transferFlags )

  Description:
    This function generates the command needed to select the desired font
    and sends it to the printer.  The font specification includes typeface,
    size, and bold and italic indications.

  Preconditions:
    None

  Parameters:
    BYTE printer        - Index of the desired printer in the printer support array.
    BYTE transferFlags  - Transfer control flags.

  Return Values:
    USB_PRINTER_SUCCESS         - Command completed successfully.
    USB_PRINTER_OUT_OF_MEMORY   - Not enough dynamic memory to perform the
                                    command.
    others                      - See the return values for the function
                                    USBHostPrinterWrite().

  Remarks:
    HP-GL/2 requires font spacing awareness.  If the font has fixed spacing,
    the Font Spacing parameter must be set to 0.  If the font has proportional
    spacing, then the Font Spacing parameter must be set to 1.  Otherwise, the
    correct font may not be selected.

    If the font has fixed spacing, pitch must be specified to select the font
    size.  The formula is pitch = 120/points, and points is stored in the
    fontSize structure member.
  ***************************************************************************/

static BYTE _PrintFontCommand( BYTE printer, BYTE transferFlags )
{
    char    *buffer;
    BYTE    font;

    buffer = (char *)USB_MALLOC( 40 );
    if (buffer == NULL)
    {
        return USB_PRINTER_OUT_OF_MEMORY;
    }

    font = printerListPCL[printer].fontName;
    if (font > USB_PRINTER_FONT_MAX_FONT)
    {
        font = USB_PRINTER_FONT_COURIER;
    }

    if (USING_VECTOR_GRAPHICS)
    {
        strcpy( buffer, COMMAND_STANDARD_FONT );
        if (printerListPCL[printer].fontName == USB_PRINTER_FONT_COURIER)
        {
            strcat( buffer, COMMAND_STANDARD_FONT_FIXED );
            sprintf( &(buffer[strlen(buffer)]), "%d,", 120 / printerListPCL[printer].fontSize );
        }
        else
        {
            strcat( buffer, COMMAND_STANDARD_FONT_PROPORTIONAL );
            sprintf( &(buffer[strlen(buffer)]), "%d,", printerListPCL[printer].fontSize );
        }

        if (FONT_IS_ITALIC( printerListPCL[printer].printerFlags.value ))
        {
            strcat( buffer, COMMAND_FONT_ITALIC_VG );
        }
        else
        {
            strcat( buffer, COMMAND_FONT_UPRIGHT_VG );
        }

        if (FONT_IS_BOLD( printerListPCL[printer].printerFlags.value ))
        {
            strcat( buffer, COMMAND_FONT_BOLD_VG );
        }
        else
        {
            strcat( buffer, COMMAND_FONT_MEDIUM_VG );
        }

        strcat( buffer, &(_pclFontNamesVG[font][0]) );

        strcat( buffer, ";" COMMAND_SELECT_STANDARD );
    }
    else
    {
        strcpy( buffer, COMMAND_ASCII );
        if (printerListPCL[printer].fontName == USB_PRINTER_FONT_COURIER)
        {
            strcat( buffer, COMMAND_FIXED );
            strcat( buffer, ESCAPE "(s" );
            sprintf( &(buffer[strlen(buffer)]), "%d", 120 / printerListPCL[printer].fontSize );
            strcat( buffer, "H" );
        }
        else
        {
            strcat( buffer, COMMAND_PROPORTIONAL );
            strcat( buffer, ESCAPE "(s" );
            sprintf( &(buffer[strlen(buffer)]), "%d", printerListPCL[printer].fontSize );
            strcat( buffer, "V" );
        }

        if (FONT_IS_ITALIC( printerListPCL[printer].printerFlags.value ))
        {
            strcat( buffer, COMMAND_FONT_ITALIC );
        }
        else
        {
            strcat( buffer, COMMAND_FONT_UPRIGHT );
        }

        if (FONT_IS_BOLD( printerListPCL[printer].printerFlags.value ))
        {
            strcat( buffer, COMMAND_FONT_BOLD );
        }
        else
        {
            strcat( buffer, COMMAND_FONT_MEDIUM );
        }

        strcat( buffer, &(_pclFontNames[font][0]) );
    }

    USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );

    return USBHostPrinterWrite( printerListPCL[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
}


/****************************************************************************
  Function:
    static BYTE _PrintStaticCommand( BYTE address, char *command, BYTE transferFlags )

  Description:
    This function sends a hard-coded command to the printer.  Many printer
    commands are constant text strings that require no parameters.  This
    routine allocates dynamic memory for a copy of the command, copies the
    command into the string, and sends the command to the printer.

  Preconditions:
    None

  Parameters:
    BYTE address        - Address of the attached printer.
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

 static BYTE _PrintStaticCommand( BYTE address, char *command, BYTE transferFlags )
{
    char    *buffer;

    USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );

    buffer = (char *)USB_MALLOC( strlen(command) + 1 );
    if (buffer == NULL)
    {
        return USB_PRINTER_OUT_OF_MEMORY;
    }
    strcpy( buffer, command );
    return USBHostPrinterWrite( address, buffer, strlen(buffer), transferFlags );
}


#endif
