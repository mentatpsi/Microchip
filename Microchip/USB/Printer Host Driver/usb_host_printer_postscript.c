/******************************************************************************
    PostScript Printer Language Support

  Summary:
    This file provides support for the PostScript printer language when using the
    USB Embedded Host Printer Client Driver.

  Description:
    This file provides support for the PostScript printer language when using the
    USB Embedded Host Printer Client Driver.

    In general, PostScript is recommended for use with the USB Embedded Host
    printer class when printing to a full sheet printer.  Implementation of the
    PostScript language across various printers is standard, ensuring uniform
    output from printers produced by different manufacturers.

  Notes:
    The PostScript coordinate origin is located at the bottom left corner of the
    paper.  For consistency for the user, the coordinates are adjusted so the
    origin is located at the top left corner.  This matches the coordinate
    system use by the Microchip Graphics library.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

 FileName:        usb_host_printer_postscript.c
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
#include "USB/usb_host_printer_postscript.h"

//#define DEBUG_MODE
#ifdef DEBUG_MODE
    #include "uart2.h"
#endif


#ifdef USB_PRINTER_LANGUAGE_POSTSCRIPT


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

#define ESCAPE                              "\033"

#define COMMAND_EJECT_PAGE                  "showpage "

#define COMMAND_GRAPHICS_COLOR_BLACK        "0 setgray "
#define COMMAND_GRAPHICS_COLOR_WHITE        "1 setgray "
#define COMMAND_GRAPHICS_LINE_TYPE_DASHED   "[3] 0 setdash "
#define COMMAND_GRAPHICS_LINE_TYPE_DOTTED   "[1 5] 0 setdash"
#define COMMAND_GRAPHICS_LINE_TYPE_SOLID    "[] 0 setdash "
#define COMMAND_GRAPHICS_LINE_WIDTH_NORMAL  "1 setlinewidth "
#define COMMAND_GRAPHICS_LINE_WIDTH_THICK   "3 setlinewidth "
#define COMMAND_GRAPHICS_LINE_END_BUTT      "0 setlinecap "
#define COMMAND_GRAPHICS_LINE_END_ROUND     "1 setlinecap "
#define COMMAND_GRAPHICS_LINE_END_SQUARE    "2 setlinecap "
#define COMMAND_GRAPHICS_LINE_JOIN_BEVEL    "2 setlinejoin "
#define COMMAND_GRAPHICS_LINE_JOIN_MITER    "0 setlinejoin "
#define COMMAND_GRAPHICS_LINE_JOIN_ROUND    "1 setlinejoin "
#define COMMAND_GRAPHICS_MOVE_TO            "%d %d moveto "
#define COMMAND_GRAPHICS_MOVE_RELATIVE      "%d %d rmoveto "
#define COMMAND_GRAPHICS_LINE_TO            "%d %d lineto "
#define COMMAND_GRAPHICS_LINE_TO_RELATIVE   "%d %d rlineto "
#define COMMAND_GRAPHICS_ARC                "%d %d %d %d %d arc "
#define COMMAND_GRAPHICS_ARC_TO             "%d %d %d %d %d arcto clear "
#define COMMAND_GRAPHICS_CIRCLE             "%d %d %d 0 360 arc "
#define COMMAND_GRAPHICS_CLOSEPATH          "closepath "

#define COMMAND_IMAGE_STOP                  ">} image grestore "
#define COMMAND_JOB_START                   ESCAPE "%-12345X"
#define COMMAND_LANDSCAPE                   "612 0 translate 90 rotate "
#define COMMAND_JOB_STOP                    "showpage " ESCAPE "%-12345X"
#define COMMAND_SET_POSITION                "%d %d moveto "
#define COMMAND_TEXT_START                  "("
#define COMMAND_TEXT_STOP                   ") show "
#define COMMAND_FILL                        "fill "
#define COMMAND_STROKE                      "stroke "

#define X_COORDINATE_IN_RANGE(x)            ((0 <= (x)) && ((x) <= printerListPostScript[printer].currentWidth))
#define Y_COORDINATE_IN_RANGE(y)            ((0 <= (y)) && ((y) <= printerListPostScript[printer].currentHeight))

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

typedef struct _PRINTER_STATUS_POSTSCRIPT
{
    USB_PRINTER_FUNCTION_SUPPORT    support;    // The functions supported by this printer.

    WORD    currentHeight;  // The current height of the page in points.
    WORD    currentWidth;   // The current width of the page in points.

    WORD    currentX;       // Current X-axis position.
    WORD    currentY;       // Current Y-axis position.

    BYTE    deviceAddress;  // Address of the attached printer
    BYTE    fontName;       // Currently selected font
    BYTE    fontSize;       // Size of the current font

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

} PRINTER_STATUS_POSTSCRIPT;


// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

const unsigned char         _psCharacterArray[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
const char                  _psFontNames[USB_PRINTER_FONT_MAX_FONT][4][30] = {
    { "AvantGarde-Book", "AvantGarde-Demi", "AvantGarde-Oblique", "AvantGarde-DemiOblique" },
    { "Bookman-Light", "Bookman-Demi", "Bookman-LightOblique", "Bookman-DemiOblique" },
    { "Courier", "Courier-Bold", "Courier-Oblique", "Courier-BoldOblique" },
    { "Helvetica", "Helvetica-Bold", "Helvetica-Oblique", "Helvetica-BoldOblique" },
    { "Helvetica-Narrow", "Helvetica-Narrow-Bold", "Helvetica-Narrow-Oblique", "Helvetica-Narrow-BoldOblique" },
    { "NewCenturySchlbk-Roman", "NewCenturySchlbk-Bold", "NewCenturySchlbk-Italic", "NewCenturySchlbk-BoldItalic" },
    { "Palatino-Roman", "Palatino-Bold", "Palatino-Italic", "Palatino-BoldItalic" },
    { "Times-Roman", "Times-Bold", "Times-Italic", "Times-BoldItalic" } };

PRINTER_STATUS_POSTSCRIPT   printerListPostScript[USB_MAX_PRINTER_DEVICES];


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define PAGE_IS_LANDSCAPE(x)    ((x & 0x01) == 0x01)
#define PAGE_IS_PORTRAIT(x)     ((x & 0x01) == 0x00)
#define FONT_IS_BOLD(x)         ((x & 0x02) == 0x02)
#define FONT_IS_ITALIC(x)       ((x & 0x04) == 0x04)

#define _SetCurrentPosition(p,x,y)                      \
    {                                                   \
        printerListPostScript[(p)].currentX = (x);      \
        printerListPostScript[(p)].currentY = (y);      \
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
static void _SetFontString( BYTE font, BYTE printerFlags, char *ptr );


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BYTE USBHostPrinterLanguagePostScript( BYTE address,
        USB_PRINTER_COMMAND command, USB_DATA_POINTER data, DWORD size, BYTE transferFlags )

  Summary:
    This function executes printer commands for a PostScript printer.

  Description:
    This function executes printer commands for a PostScript printer.  When
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

BYTE USBHostPrinterLanguagePostScript( BYTE address,
        USB_PRINTER_COMMAND command, USB_DATA_POINTER data, DWORD size, BYTE transferFlags )
{
    char    *buffer;
    BYTE    printer = 0;

    if (command != USB_PRINTER_ATTACHED)
    {
        // Try to find the current printer.  If we can't find the printer in the list,
        // put it in the list at the first available location.
        for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListPostScript[printer].deviceAddress != address); printer++ );
        if (printer == USB_MAX_PRINTER_DEVICES)
        {
            return USB_PRINTER_UNKNOWN_DEVICE;
        }
    }

    switch( command )
    {
        //---------------------------------------------------------------------
        case USB_PRINTER_ATTACHED:
            for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListPostScript[printer].deviceAddress != 0); printer++ );
            if (printer != USB_MAX_PRINTER_DEVICES)
            {
                printerListPostScript[printer].deviceAddress = address;
                printerListPostScript[printer].support       = *((USB_PRINTER_FUNCTION_SUPPORT *)(data.pointerRAM));
                return USB_PRINTER_SUCCESS;
            }
            return USB_PRINTER_TOO_MANY_DEVICES;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_DETACHED:
            for (printer=0; (printer<USB_MAX_PRINTER_DEVICES) && (printerListPostScript[printer].deviceAddress != address); printer++ );
            if (printer != USB_MAX_PRINTER_DEVICES)
            {
                printerListPostScript[printer].deviceAddress = 0;
            }
            return USB_PRINTER_SUCCESS;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_JOB_START:
            // Initialize page information.
            printerListPostScript[printer].fontName                 = USB_PRINTER_FONT_HELVETICA;
            printerListPostScript[printer].fontSize                 = 12;
            printerListPostScript[printer].printerFlags.value       = 0;
            printerListPostScript[printer].currentHeight            = PRINTER_PAGE_PORTRAIT_HEIGHT;
            printerListPostScript[printer].currentWidth             = PRINTER_PAGE_PORTRAIT_WIDTH;
            _SetCurrentPosition( printer, 0, 0 );
            return _PrintStaticCommand( address, COMMAND_JOB_START, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_JOB_STOP:
            return _PrintStaticCommand( address, COMMAND_JOB_STOP, transferFlags | USB_PRINTER_TRANSFER_NOTIFY );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_ORIENTATION_PORTRAIT:
            // PostScript resets all settings at the beginning of a page.  Portrait
            // is the default orientation, so we don't have to do anything.  This
            // command should be the first command sent after USB_PRINTER_JOB_START
            // or USB_PRINTER_EJECT_PAGE.
            printerListPostScript[printer].printerFlags.isLandscape = 0;
            printerListPostScript[printer].currentHeight            = PRINTER_PAGE_PORTRAIT_HEIGHT;
            printerListPostScript[printer].currentWidth             = PRINTER_PAGE_PORTRAIT_WIDTH;
            return USB_PRINTER_SUCCESS;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_ORIENTATION_LANDSCAPE:
            // PostScript resets all settings at the beginning of a page.  This
            // command should be the first command sent after USB_PRINTER_JOB_START
            // or USB_PRINTER_EJECT_PAGE.  It must be called only once, or the
            // paper will be rotated again.
            printerListPostScript[printer].printerFlags.isLandscape = 1;
            printerListPostScript[printer].currentHeight            = PRINTER_PAGE_LANDSCAPE_HEIGHT;
            printerListPostScript[printer].currentWidth             = PRINTER_PAGE_LANDSCAPE_WIDTH;
            return _PrintStaticCommand( address, COMMAND_LANDSCAPE, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_NAME:
            // Font name is passed in the size parameter.
            printerListPostScript[printer].fontName = (BYTE)size;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_SIZE:
            // Font size is passed in the size parameter.
            printerListPostScript[printer].fontSize = (BYTE)size;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_ITALIC:
            printerListPostScript[printer].printerFlags.isItalic = 1;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_UPRIGHT:
            printerListPostScript[printer].printerFlags.isItalic = 0;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_BOLD:
            printerListPostScript[printer].printerFlags.isBold = 1;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_FONT_MEDIUM:
            printerListPostScript[printer].printerFlags.isBold = 0;
            return _PrintFontCommand( printer, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_EJECT_PAGE:
            // When we eject a page, all of our settings are cleared.
            return _PrintStaticCommand( address, COMMAND_EJECT_PAGE, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_TEXT_START:
            return _PrintStaticCommand( address, COMMAND_TEXT_START, transferFlags );
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
            return _PrintStaticCommand( address, COMMAND_TEXT_STOP, transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_MOVE_TO:
            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE((WORD)(size >> 16)) || !Y_COORDINATE_IN_RANGE((WORD)size ))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif
        case USB_PRINTER_SET_POSITION:
            // This command sets the cursor to the specified position.  Note
            // that we must convert the specification to use PostScript's
            // orientation of the Y-axis.
            buffer = (char *)USB_MALLOC( 16 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_SET_POSITION, (WORD)(size >> 16), printerListPostScript[printer].currentHeight - (WORD)size );
            _SetCurrentPosition( printer, (WORD)(size >> 16), (WORD)size );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_START:
            buffer = (char *)USB_MALLOC( 146 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            {
                USB_PRINTER_IMAGE_INFO  *info;

                info = (USB_PRINTER_IMAGE_INFO *)(data.pointerRAM);

                sprintf( buffer, "gsave %d %d %d %7.2f mul sub translate %d %7.2f mul %d %7.2f mul scale %d %d 1 [%d 0 0 -%d 0 %d] {<",
                    info->positionX, printerListPostScript[printer].currentHeight - info->positionY, info->height, (double)info->scale,
                    info->width, (double)info->scale, info->height, (double)info->scale,
                    info->width, info->height,
                    info->width, info->height, info->height );
                #ifdef DEBUG_MODE
                    UART2PrintString( buffer );
                    UART2PrintString( "\r\n" );
                #endif
            }
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_DATA_HEADER:
            return USB_PRINTER_SUCCESS;
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_DATA:
            // We have to translate the data here from binary to an ASCII representation
            // of the binary.  For each four bits, we need one byte for the ASCII, plus one
            // for a NULL at the end.  We cannot do this in terms of bytes - PostScript will
            // ignore extra bits at the end of a single character only, not at the end of
            // a byte.

            // Determine the number of data nibbles.  We might have a size that is
            // not an even multiple of 4 bits.  To account for this, add 3 to the
            // size, and then divide by 4.
            size += 3;
            size /= 4;

            buffer = (char *)USB_MALLOC( size + 1 ); // Add one for possible truncation error.
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }
            else
            {
                WORD    i;
                BYTE    printVar;
                char    *tempNew;

                // If the data is being read from ROM, then we have to
                // use a different pointer type to get the data.
                if (transferFlags & USB_PRINTER_TRANSFER_FROM_ROM)
                {
                    #if defined( __C30__ ) || defined __XC16__
                        BYTE __prog__   *tempOld;
                    #elif defined( __PIC32MX__ )
                        const BYTE      *tempOld;
                    #endif

                    tempNew = (char *)buffer;
                    tempOld = ((USB_DATA_POINTER)data).pointerROM;

                    // PostScript will ignore extra bits at the end of a nibble, but
                    // not at the end of a byte.  Therefore, we have to print one
                    // nibble at a time.
                    for (i=0; i<size; i++)
                    {
                        if (!(i & 0x01))
                        {
                            // Most significant nibble.  Don't advance the byte pointer.
                            printVar = (*tempOld>>4) & 0x0F;
                        }
                        else
                        {
                            // Least significant nibble.  Advance the byte pointer.
                            printVar = *tempOld++ & 0x0F;
                        }
                        *tempNew++ = _psCharacterArray[printVar];
                    }
                }
                else
                {
                    BYTE    *tempOld;

                    tempNew = (char *)buffer;
                    tempOld = ((USB_DATA_POINTER)data).pointerRAM;

                    // PostScript will ignore extra bits at the end of a nibble, but
                    // not at the end of a byte.  Therefore, we have to print one
                    // nibble at a time.
                    for (i=0; i<size; i++)
                    {
                        if (!(i & 0x01))
                        {
                            // Most significant nibble.  Don't advance the byte pointer.
                            printVar = (*tempOld>>4) & 0x0F;
                        }
                        else
                        {
                            // Least significant nibble.  Advance the byte pointer.
                            printVar = *tempOld++ & 0x0F;
                        }
                        *tempNew++ = _psCharacterArray[printVar];
                    }
                }

                USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
                return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, size, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_IMAGE_STOP:
            return _PrintStaticCommand( address, COMMAND_IMAGE_STOP, transferFlags );
            break;


        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_TYPE:
            if (size == PRINTER_LINE_TYPE_SOLID)
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_TYPE_SOLID, transferFlags );
            }
            else if (size == PRINTER_LINE_TYPE_DASHED)
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_TYPE_DASHED, transferFlags );
            }
            else // PRINTER_LINE_TYPE_DOTTED
            {
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_LINE_TYPE_DOTTED, transferFlags );
            }
            break;

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
            // This command is not supported yet.  It's a bit complicated.
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_COLOR:
            if (size == PRINTER_COLOR_BLACK)
            {
                printerListPostScript[printer].printerFlags.penIsWhite = 0;
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_COLOR_BLACK, transferFlags );
            }
            else //PRINTER_COLOR_WHITE
            {
                printerListPostScript[printer].printerFlags.penIsWhite = 1;
                return _PrintStaticCommand( address, COMMAND_GRAPHICS_COLOR_WHITE, transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_MOVE_RELATIVE:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.
            {
                int x;
                int y;

                x = printerListPostScript[printer].currentX + (int)(size >> 16);
                y = printerListPostScript[printer].currentY + (int)(size & 0xFFFF);

                #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                    if (!X_COORDINATE_IN_RANGE( x ) || !Y_COORDINATE_IN_RANGE( y ))
                    {
                        return USB_PRINTER_BAD_PARAMETER;
                    }
                #endif

                buffer = (char *)USB_MALLOC( 16 );
                if (buffer == NULL)
                {
                    return USB_PRINTER_OUT_OF_MEMORY;
                }

                sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO, x, printerListPostScript[printer].currentHeight - y ) ;
                _SetCurrentPosition( printer,  x, y );
                USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
                return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            }
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.

            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x1) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y1) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x2) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y2))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 73 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_SET_POSITION COMMAND_GRAPHICS_LINE_TO COMMAND_STROKE,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x1, printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y1),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x2, printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y2) );
            _SetCurrentPosition( printer,  ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.x2, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sLine.y2 );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_TO:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.

            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE( (int)(size >> 16) ) || !Y_COORDINATE_IN_RANGE( (int)(size & 0xFFFF) ))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 46 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_LINE_TO COMMAND_STROKE,
                printerListPostScript[printer].currentX, printerListPostScript[printer].currentHeight - printerListPostScript[printer].currentY,
                (int)(size >> 16), printerListPostScript[printer].currentHeight - (int)(size & 0xFFFF) );
            _SetCurrentPosition( printer,  (int)(size >> 16), (int)(size & 0xFFFF) );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_LINE_TO_RELATIVE:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.

            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE( printerListPostScript[printer].currentX + (int)(size >> 16) ) || !Y_COORDINATE_IN_RANGE( printerListPostScript[printer].currentY + (int)(size & 0xFFFF) ))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 46 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_LINE_TO_RELATIVE COMMAND_STROKE,
                printerListPostScript[printer].currentX, printerListPostScript[printer].currentHeight - printerListPostScript[printer].currentY,
                (int)(size >> 16), (int)(size & 0xFFFF) * -1 ) ;
            _SetCurrentPosition( printer,  printerListPostScript[printer].currentX + (int)(size >> 16), printerListPostScript[printer].currentY + (int)(size & 0xFFFF) );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_ARC:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.  Also, the coordinate parameters do
            // not match the command parameters (x,y).
            // x y ro a1 a2 arc fill 1 setgray x y ri a1 a2 arc fill 0 setgray

            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xL) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yT) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xR) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yB))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 15 + (56 + 11)* 2 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            {
                int     a1;
                int     a2;
                int     x;
                int     y;
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

                    // Look for the second edge of the arc
                    a2 = a1;
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

                x = (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xR + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xL) / 2;
                y = (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yT + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yB) / 2;

                sprintf( buffer, COMMAND_GRAPHICS_MOVE_TO COMMAND_GRAPHICS_ARC COMMAND_FILL,
                    x, printerListPostScript[printer].currentHeight - y,
                    x, printerListPostScript[printer].currentHeight - y,
                    ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.r2, a1, a2 );

                if (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.r1 != 0)
                {
                    // Change pens

                    if (printerListPostScript[printer].printerFlags.penIsWhite)
                    {
                        strcat( buffer, COMMAND_GRAPHICS_COLOR_BLACK );
                    }
                    else
                    {
                        strcat( buffer, COMMAND_GRAPHICS_COLOR_WHITE );
                    }

                    // Draw the inner circle to erase the interior.

                    sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC COMMAND_FILL,
                        x, printerListPostScript[printer].currentHeight - y,
                        ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.r1, a1, a2 );

                    // Put the original pen back.

                    if (printerListPostScript[printer].printerFlags.penIsWhite)
                    {
                        strcat( buffer, COMMAND_GRAPHICS_COLOR_WHITE );
                    }
                    else
                    {
                        strcat( buffer, COMMAND_GRAPHICS_COLOR_BLACK );
                    }
                }
            }
            _SetCurrentPosition( printer,  ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.xL, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sArc.yT );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_CIRCLE:
        case USB_PRINTER_GRAPHICS_CIRCLE_FILLED:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.

            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y + ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r) ||
                    !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 50 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            sprintf( buffer, COMMAND_GRAPHICS_CIRCLE,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.r );

            if (command == USB_PRINTER_GRAPHICS_CIRCLE)
            {
                strcat( buffer, COMMAND_STROKE );
            }
            else // USB_PRINTER_GRAPHICS_CIRCLE_FILLED
            {
                strcat( buffer, COMMAND_FILL );
            }

            _SetCurrentPosition( printer,  ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.x, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sCircle.y );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_BEVEL:
        case USB_PRINTER_GRAPHICS_BEVEL_FILLED:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.
            // xL yB-r moveto xL yT xR yT r arcto clear xR yT xR yB r arcto clear xR yB xL yT r arcto clear xL yB xL yT r arcto clear stroke

            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 155 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            // xL yB+r moveto
            sprintf( buffer, COMMAND_SET_POSITION,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r ));

            // xL yT xR yT r arcto clear
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC_TO,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r );

            // xR yT xR yB r arcto clear
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC_TO,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r );

            // xR yB xL yB r arcto clear
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC_TO,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xR,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r );

            // xL yB xL yT r arcto clear
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_ARC_TO,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL,
                printerListPostScript[printer].currentHeight - (((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yT),
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.r );

            if (command == USB_PRINTER_GRAPHICS_BEVEL)
            {
                strcat( buffer, COMMAND_STROKE );
            }
            else // USB_PRINTER_GRAPHICS_BEVEL_FILLED
            {
                strcat( buffer, COMMAND_FILL );
            }

            _SetCurrentPosition( printer,  ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.xL, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sBevel.yB );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_RECTANGLE:
        case USB_PRINTER_GRAPHICS_RECTANGLE_FILLED:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.
            // xL yB moveto xL yT lineto xR yT lineto xR yB lineto closepath stroke

            #ifdef PRINTER_GRAPHICS_COORDINATE_CHECKING
                if (!X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yT) ||
                    !X_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xR) || !Y_COORDINATE_IN_RANGE(((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB))
                {
                    return USB_PRINTER_BAD_PARAMETER;
                }
            #endif

            buffer = (char *)USB_MALLOC( 80 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            // xL yB moveto
            sprintf( buffer, COMMAND_SET_POSITION,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL,
                printerListPostScript[printer].currentHeight - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB );

            // xL yT lineto
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_LINE_TO,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL,
                printerListPostScript[printer].currentHeight - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yT );

            // xR yT lineto
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_LINE_TO,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xR,
                printerListPostScript[printer].currentHeight - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yT );

            // xR yB lineto
            sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_LINE_TO,
                ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xR,
                printerListPostScript[printer].currentHeight - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB );

            strcat( buffer, COMMAND_GRAPHICS_CLOSEPATH );
            if (command == USB_PRINTER_GRAPHICS_RECTANGLE)
            {
                strcat( buffer, COMMAND_STROKE );
            }
            else // USB_PRINTER_GRAPHICS_RECTANGLE_FILLED
            {
                strcat( buffer, COMMAND_FILL );
            }

            _SetCurrentPosition( printer,  ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.xL, ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sRectangle.yB );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
            break;

        //---------------------------------------------------------------------
        case USB_PRINTER_GRAPHICS_POLYGON:
            // Note that we must convert the specification to use PostScript's
            // orientation of the Y-axis.

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

            buffer = (char *)USB_MALLOC( 15 + ( 15 * ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.numPoints) + 18 );
            if (buffer == NULL)
            {
                return USB_PRINTER_OUT_OF_MEMORY;
            }

            {
                WORD    i = 0;

                // x0 y0 moveto
                sprintf( buffer, COMMAND_SET_POSITION,
                    ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[i],
                    printerListPostScript[printer].currentHeight - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[i+1] );
                i += 2;

                // xn yn lineto
                while (i < ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.numPoints * 2)
                {
                    sprintf( &(buffer[strlen(buffer)]), COMMAND_GRAPHICS_LINE_TO,
                        ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[i],
                        printerListPostScript[printer].currentHeight - ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[i+1] );
                    i += 2;
                }

                strcat( buffer, COMMAND_GRAPHICS_CLOSEPATH COMMAND_STROKE );
            }

            _SetCurrentPosition( printer,  ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[0], ((USB_PRINTER_GRAPHICS_PARAMETERS *)(data.pointerRAM))->sPolygon.points[1] );
            USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );
            return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
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
    BOOL USBHostPrinterLanguagePostScriptIsSupported( char *deviceID,
                USB_PRINTER_FUNCTION_SUPPORT *support )

  Description:
    This function determines if the printer with the given device ID string
    supports the PostScript printer language.

  Preconditions:
    None

  Parameters:
    char *deviceID  - Pointer to the "COMMAND SET:" portion of the device ID
                        string of the attached printer.
    USB_PRINTER_FUNCTION_SUPPORT *support   - Pointer to returned information
                        about what types of functions this printer supports.

  Return Values:
    TRUE    - The printer supports PostScript.
    FALSE   - The printer does not support PostScript.

  Remarks:
    The caller must first locate the "COMMAND SET:" section of the device ID
    string.  To ensure that only the "COMMAND SET:" section of the device ID
    string is checked, the ";" at the end of the section should be temporarily
    replaced with a NULL.  Otherwise, this function may find the printer
    language string in the comments or other section, and incorrectly indicate
    that the printer supports the language.

    Device ID strings are case sensitive.
  ***************************************************************************/

BOOL USBHostPrinterLanguagePostScriptIsSupported( char *deviceID,
                USB_PRINTER_FUNCTION_SUPPORT *support )
{
    if (strstr( deviceID, LANGUAGE_ID_STRING_POSTSCRIPT ))
    {
        support->val = LANGUAGE_SUPPORT_FLAGS_POSTSCRIPT;
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
    None
  ***************************************************************************/

static BYTE _PrintFontCommand( BYTE printer, BYTE transferFlags )
{
    char    *buffer;
//    char    temp[6];

    buffer = (char *)USB_MALLOC( 1 + 30 + 10 + 5 + 20 );
    if (buffer == NULL)
    {
        return USB_PRINTER_OUT_OF_MEMORY;
    }

    buffer[0] = '/';
    _SetFontString( printerListPostScript[printer].fontName, printerListPostScript[printer].printerFlags.value, &(buffer[1]) );
    sprintf( &(buffer[strlen(buffer)]), " findfont %d scalefont setfont ", printerListPostScript[printer].fontSize );
//    strcat( buffer, " findfont " );
//    sprintf( temp, "%d", printerListPostScript[printer].fontSize );
//    strcat( buffer, temp );
//    strcat( buffer, " scalefont setfont " );

    USBHOSTPRINTER_SETFLAG_COPY_DATA( transferFlags );

    return USBHostPrinterWrite( printerListPostScript[printer].deviceAddress, buffer, strlen(buffer), transferFlags );
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


/****************************************************************************
  Function:
    static void _SetFontString( BYTE font, BYTE printerFlags, char *ptr )

  Description:
    This function puts the string that designates the selected font into
    the indicated character buffer.

  Preconditions:
    None

  Parameters:
    BYTE font           - Font indication.  See the enumeration
                            USB_PRINTER_FONTS for the list of valid values.
    BYTE printerFlags   - Flags that indicate the bold and italic condition
                            of the font.  These must match the structure of
                            printerFlags in the PRINTER_STATUS_POSTSCRIPT
                            structure
    char *ptr           -   Buffer to store the font string.

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

static void _SetFontString( BYTE font, BYTE printerFlags, char *ptr )
{
    BYTE    i;

    if (font > USB_PRINTER_FONT_MAX_FONT)
    {
        font = USB_PRINTER_FONT_COURIER;
    }

    i = 0;
    if (FONT_IS_BOLD( printerFlags ))   i |= 0x01;
    if (FONT_IS_ITALIC( printerFlags))  i |= 0x02;

    strcpy( ptr, &(_psFontNames[font][i][0]) );
}



#endif

