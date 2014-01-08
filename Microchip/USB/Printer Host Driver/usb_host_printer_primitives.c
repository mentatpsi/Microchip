/*****************************************************************************

    Microchip USB Host Printer Client Driver, Graphics Library Interfaces Layer

  Summary:
    This file contains the routines needed to utilize the Microchip Graphics
    Library functions to create graphic images on a USB printer.

  Description:
    This file contains the routines needed to utilize the Microchip Graphics
    Library functions to create graphic images on a USB printer.

    The label USE_GRAPHICS_LIBRARY_PRINTER_INTERFACE must be defining in the
    USB configuration header file usb_config.h to utilize these functions.

  Remarks:
    None

******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

 FileName:        usb_host_printer_primitives.c
 Dependencies:    None
 Processor:       PIC24F/PIC32MX
 Compiler:        C30/C32
 Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the �Company�) for its PICmicro� Microcontroller is intended and
supplied to you, the Company�s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

Change History:
  Rev         Description
  ----------  ----------------------------------------------------------
  2.6 - 2.6a  No change

*******************************************************************************/
//DOM-IGNORE-END


#include <stdlib.h>
#include <string.h>
#include "Graphics/Graphics.h"
#include "USB/usb.h"
#include "USB/usb_host_printer_primitives.h"

//#define DEBUG_MODE
#ifdef DEBUG_MODE
    #include "uart2.h"
#endif

#ifndef USB_MALLOC
    #define USB_MALLOC(size) malloc(size)
#endif

#ifndef USB_FREE
    #define USB_FREE(ptr) free(ptr)
#endif

#define USB_FREE_AND_CLEAR(ptr) {USB_FREE(ptr); ptr = NULL;}

#ifdef USE_GRAPHICS_LIBRARY_PRINTER_INTERFACE


// *****************************************************************************
// *****************************************************************************
// Section: Subroutines
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    SHORT PrintScreen( BYTE address, USB_PRINT_SCREEN_INFO *printScreenInfo )

  Summary:
    This routine will extract the image that is currently on the specified
    portion of the graphics display, and print it at the specified location.

  Description:
    This routine is intended for use in an application that is using the
    Graphics Library to control a graphics display.  This routine will
    extract the image that is currently on the specified portion of
    the graphics display, and print it at the specified location.
    Since the display may be in color and the printer can print only black
    and white, the pixel color to interpret as black must be specified in the
    USB_PRINT_SCREEN_INFO structure.

    The function can be compiled as either a blocking function or a
    non-blocking function.  When compiled as a blocking function, the routine
    will wait to enqueue all printer instructions.  If an error occurs, then
    this function will return the error.  If all printer instructions are
    enqueued successfully, the function will return -1.  When compiled as a
    non-blocking function, this function will return 0 if the operation is
    proceeding correctly but has not yet completed.  The application must
    continue to call this function, with the same parameters, until a
    non-zero value is returned.  A value of -1 indicates that all printer
    instructions have been enqueued successfully.  Any other value is an error
    code, and the state machine will be set back to the beginning state.

  Precondition:
    None

  Parameters:
    BYTE address                            - USB address of the printer.
    USB_PRINT_SCREEN_INFO *printScreenInfo  - Information about the screen
                            area to print, how to interpret the screen image,
                            and how and where to print the image.  Note that
                            the width and height members of the structure do
                            not need to be filled in by the application.

  Return Values:
    0       -   Non-blocking configuration only.  Image output is not yet
                complete, but is proceeding normally.
    (-1)    -   Image output was completed successfully.
    other   -   Printing was aborted due to an error.  See the return values
                for USBHostPrinterCommand().  Note that the return code
                USB_PRINTER_SUCCESS will not be returned.  Instead, (-1) will
                be returned upon successful completion.

  Remarks:
    None
  ***************************************************************************/

#define PRINT_SCREEN_STATE_BEGIN        0
#define PRINT_SCREEN_STATE_SEND_START   1
#define PRINT_SCREEN_STATE_SEND_HEADER  2
#define PRINT_SCREEN_STATE_SEND_DATA    3
#define PRINT_SCREEN_STATE_SEND_STOP    4


SHORT PrintScreen( BYTE address, USB_PRINT_SCREEN_INFO *printScreenInfo )
{
    #ifdef USE_NONBLOCKING_CONFIG
        static WORD     imageLine;
        WORD            imagePixel;
        static BYTE     lineDepth;
        BYTE            mask;
        static BYTE     *oneRow     = NULL;
        WORD            size;
        static BYTE     state       = PRINT_SCREEN_STATE_BEGIN;
        BYTE            returnCode;

        if (!USBHostPrinterCommandReady( address ))
        {
            return 0;
        }

        switch (state)
        {
            case PRINT_SCREEN_STATE_BEGIN:
                printScreenInfo->printerInfo.width  = printScreenInfo->xR - printScreenInfo->xL + 1;
                printScreenInfo->printerInfo.height = printScreenInfo->yB - printScreenInfo->yT + 1;

                lineDepth = 1;
                if (printScreenInfo->printerType.supportFlags.supportsPOS)
                {
                    if (printScreenInfo->printerInfo.densityVertical == 24)
                    {
                        lineDepth = 3;
                    }
                    else if (printScreenInfo->printerInfo.densityVertical == 36)
                    {
                        lineDepth = 5;
                    }
                }

                imageLine  = 0;
                if (oneRow != NULL)
                {
                    USB_FREE( oneRow );
                }

                if (printScreenInfo->printerType.supportFlags.supportsPOS)
                {
                    size = printScreenInfo->printerInfo.width * lineDepth;
                }
                else
                {
                    size = (printScreenInfo->printerInfo.width + 7) / 8 ;
                }

                oneRow = (BYTE *)USB_MALLOC( size );
                if (oneRow == NULL)
                {
                    return USB_PRINTER_OUT_OF_MEMORY;
                }

                state++;
                // Fall through.

            case PRINT_SCREEN_STATE_SEND_START:
                returnCode = USBHostPrinterCommand( address, USB_PRINTER_IMAGE_START,
                            USB_DATA_POINTER_RAM(&(printScreenInfo->printerInfo)), sizeof(printScreenInfo->printerInfo), 0 );
                if (returnCode)
                {
                    USB_FREE( oneRow );
                    state = PRINT_SCREEN_STATE_BEGIN;
                    return returnCode;
                }
                break;

            case PRINT_SCREEN_STATE_SEND_HEADER:
                // Read the pixels for the current row of data.
                if (printScreenInfo->printerType.supportFlags.supportsPOS)
                {
                    WORD    i;
                    WORD    j;

                    memset( oneRow, 0xFF, printScreenInfo->printerInfo.width * lineDepth );

                    mask = 0x80;
                    for (imagePixel=0; imagePixel<printScreenInfo->printerInfo.width; imagePixel++)
                    {
                        for (i=0; i<lineDepth; i++)
                        {
                            for (j=0; j<8; j++)
                            {
                                if (((imageLine + (i*8) + j) < printScreenInfo->printerInfo.height) &&
                                    (GetPixel( printScreenInfo->xL + imagePixel, printScreenInfo->yT + imageLine + (i*8) + j) == printScreenInfo->colorBlack))
                                {
                                    oneRow[imagePixel*lineDepth + i] &= ~mask;
                                }
                                mask >>= 1;
                                if (mask == 0)
                                {
                                    mask = 0x80;
                                }
                            }
                        }
                    }
                }
                else
                {
                    memset( oneRow, 0, (printScreenInfo->printerInfo.width + 7) / 8 );

                    mask = 0x80;
                    for (imagePixel=0; imagePixel<printScreenInfo->printerInfo.width; imagePixel++)
                    {
                        if (GetPixel( printScreenInfo->xL + imagePixel, printScreenInfo->yT + imageLine ) != printScreenInfo->colorBlack)
                        {
                            oneRow[imagePixel/8] |= mask;
                        }
                        mask >>= 1;
                        if (mask == 0)
                        {
                            mask = 0x80;
                        }
                    }

                    #ifdef DEBUG_MODE
                        UART2PrintString( "\r\n" );
                        for (imagePixel=0; imagePixel<printScreenInfo->printerInfo.width; imagePixel+=8)
                        {
                            UART2PutHex( oneRow[imagePixel/8] );
                        }
                        UART2PrintString( "\r\n" );
                    #endif
                }

                returnCode = USBHostPrinterCommand( address, USB_PRINTER_IMAGE_DATA_HEADER, USB_NULL, printScreenInfo->printerInfo.width, 0 );
                if (returnCode)
                {
                    USB_FREE( oneRow );
                    state = PRINT_SCREEN_STATE_BEGIN;
                    return returnCode;
                }
                break;

            case PRINT_SCREEN_STATE_SEND_DATA:
                returnCode = USBHostPrinterCommand( address, USB_PRINTER_IMAGE_DATA, USB_DATA_POINTER_RAM(oneRow), printScreenInfo->printerInfo.width, USB_PRINTER_TRANSFER_COPY_DATA);
                if (returnCode)
                {
                    USB_FREE( oneRow );
                    state = PRINT_SCREEN_STATE_BEGIN;
                    return returnCode;
                }

                if (printScreenInfo->printerType.supportFlags.supportsPOS)
                {
                    if (printScreenInfo->printerInfo.densityVertical == 24)
                    {
                        imageLine += 24;
                    }
                    else if (printScreenInfo->printerInfo.densityVertical == 36)
                    {
                        imageLine += 36;
                    }
                    else
                    {
                        imageLine += 8;
                    }
                }
                else
                {
                    imageLine++;
                }
                if (imageLine < printScreenInfo->printerInfo.height)
                {
                    state = PRINT_SCREEN_STATE_SEND_HEADER;
                    return 0;
                }
                break;

            case PRINT_SCREEN_STATE_SEND_STOP:
                returnCode = USBHostPrinterCommand( address, USB_PRINTER_IMAGE_STOP, USB_NULL, 0, 0 );
                if (!returnCode)
                {
                    returnCode = -1;
                }

                USB_FREE( oneRow );
                state = PRINT_SCREEN_STATE_BEGIN;
                return returnCode;
                break;
        }

        state ++;
        return 0;

    #else

        WORD            imageLine;
        WORD            imagePixel;
        BYTE            lineDepth;
        BYTE            mask;
        BYTE            *oneRow;
        BYTE            returnCode;
        WORD            size;

        printScreenInfo->printerInfo.width  = printScreenInfo->xR - printScreenInfo->xL + 1;
        printScreenInfo->printerInfo.height = printScreenInfo->yB - printScreenInfo->yT + 1;

        lineDepth = 1;
        if (printScreenInfo->printerType.supportFlags.supportsPOS)
        {
            if (printScreenInfo->printerInfo.densityVertical == 24)
            {
                lineDepth = 3;
            }
            else if (printScreenInfo->printerInfo.densityVertical == 36)
            {
                lineDepth = 5;
            }
        }

        if (printScreenInfo->printerType.supportFlags.supportsPOS)
        {
            size = printScreenInfo->printerInfo.width * lineDepth;
        }
        else
        {
            size = (printScreenInfo->printerInfo.width + 7) / 8 ;
        }

        oneRow = (BYTE *)USB_MALLOC( size );
        if (oneRow == NULL)
        {
            return USB_PRINTER_OUT_OF_MEMORY;
        }

        USBHostPrinterCommandWithReadyWait( &returnCode, address, USB_PRINTER_IMAGE_START,
                    USB_DATA_POINTER_RAM(&(printScreenInfo->printerInfo)), sizeof(printScreenInfo->printerInfo), 0 );
        if (!returnCode)
        {
            imageLine = 0;
            while (!returnCode && (imageLine < printScreenInfo->printerInfo.height))
            {
                if (printScreenInfo->printerType.supportFlags.supportsPOS)
                {
                    WORD    i;
                    WORD    j;

                    // Read the pixels for the current wide row of data.
                    memset( oneRow, 0xFF, printScreenInfo->printerInfo.width * lineDepth );
                    mask = 0x80;
                    for (imagePixel=0; imagePixel<printScreenInfo->printerInfo.width; imagePixel++)
                    {
                        for (i=0; i<lineDepth; i++)
                        {
                            for (j=0; j<8; j++)
                            {
                                if (((imageLine + (i*8) + j) < printScreenInfo->printerInfo.height) &&
                                    (GetPixel( printScreenInfo->xL + imagePixel, printScreenInfo->yT + imageLine + (i*8) + j) == printScreenInfo->colorBlack))
                                {
                                    oneRow[imagePixel*lineDepth + i] &= ~mask;
                                }
                                mask >>= 1;
                                if (mask == 0)
                                {
                                    mask = 0x80;
                                }
                            }
                        }
                    }
                    imageLine += printScreenInfo->printerInfo.densityVertical;
                }
                else
                {
                    // Read the pixels for the current row of data.
                    memset( oneRow, 0, (printScreenInfo->printerInfo.width + 7) / 8 );
                    mask = 0x80;
                    for (imagePixel=0; imagePixel<printScreenInfo->printerInfo.width; imagePixel++)
                    {
                        if (GetPixel( printScreenInfo->xL + imagePixel, printScreenInfo->yT + imageLine ) != printScreenInfo->colorBlack)
                        {
                            oneRow[imagePixel/8] |= mask;
                        }
                        mask >>= 1;
                        if (mask == 0)
                        {
                            mask = 0x80;
                        }
                    }

                    #ifdef DEBUG_MODE
                        UART2PrintString( "\r\n" );
                        for (imagePixel=0; imagePixel<printScreenInfo->printerInfo.width; imagePixel+=8)
                        {
                            UART2PutHex( oneRow[imagePixel/8] );
                        }
                        UART2PrintString( "\r\n" );
                    #endif

                    imageLine ++;
                }

                USBHostPrinterCommandWithReadyWait( &returnCode, address, USB_PRINTER_IMAGE_DATA_HEADER, USB_NULL, printScreenInfo->printerInfo.width, 0 );
                if (!returnCode)
                {
                    USBHostPrinterCommandWithReadyWait( &returnCode, address, USB_PRINTER_IMAGE_DATA, USB_DATA_POINTER_RAM(oneRow), printScreenInfo->printerInfo.width, USB_PRINTER_TRANSFER_COPY_DATA);
                }
            }

            if (!returnCode)
            {
                USBHostPrinterCommandWithReadyWait( &returnCode, address, USB_PRINTER_IMAGE_STOP, USB_NULL, 0, 0 );
            }
        }

        USB_FREE( oneRow );
        if (!returnCode)
        {
            returnCode = -1;    // The image completed successfully.
        }
        return returnCode;

    #endif
}



#endif

