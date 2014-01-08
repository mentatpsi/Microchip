/******************************************************************************

  USB Host Printer Client Driver

  Summary:
    This is the Printer client driver file for a USB Embedded Host device.

  Description:
    This is the Printer client driver file for a USB Embedded Host device.
    It allows an embedded application to utilize a USB printer to provide
    printed output.

    USB printers utilize the USB Printer Class to communicate with a USB
    Host.  This class defines the USB transfer type, the endpoint structure,
    a device requests that can be performed.  The actual commands sent to
    the printer, however, are dictated by the printer language used by the
    particular printer.

    Many different printer languages are utilized by the wide variety of
    printers on the market.  Typically, low end printers receive printer-specific
    binary data, utilizing the processing power of the USB Host to perform
    all of the complex calculations required to translate text and graphics to
    a simple binary representation.  This works well when a PC is the USB Host,
    but it is not conducive to an embedded application with limited resources.

    Many printers on the market use a command based printer language, relying
    on the printer itself to interpret commands to produce the desired output.
    Some languages are standardized across printers from a particular
    manufacturer, and some are used across multiple manufacturer.  This method
    lends itself better to embedded applications by allowing the printer to
    take on some of the computational overhead.  Microchip provides support for
    some printer languages, including PostScript and PCL 5.  Additional printer
    language can be implemented.  Refer to the USB Embedded Host Printer Class
    application notes for more details on implementing printer language support.


  Remarks:
    This driver should be used in a project with usb_host.c to provided the USB
    Embedded Host and hardware interfaces, plus one or more language support
    files.

    To interface with USB Embedded Host layer, the routine USBHostPrinterInitialize()
    should be specified as the Initialize() function, and
    USBHostPrinterEventHandler() should be specified as the EventHandler()
    function in the usbClientDrvTable[] array declared in usb_config.c.

    This driver requires transfer events from usb_host.c, so
    USB_ENABLE_TRANSFER_EVENT must be defined.

    Since the printer class is performed with bulk transfers,
    USB_SUPPORT_BULK_TRANSFERS must be defined.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

FileName:        usb_host_printer.c
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
  Rev           Description
  ----------    -----------
  2.6 - 2.6a    No chance except stack revision number
  2.7           Minor updates to USBHostPrinterGetStatus() header
                to better describe the function requirements and
                operation.

                Changed how transfer queues are handled to do a peek
                now before removing the item from the queue.
********************************************************************/

//DOM-IGNORE-END

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "GenericTypeDefs.h"
#include "usb_config.h"
#include "struct_queue.h"
#include "USB/usb.h"
#include "USB/usb_host_printer.h"

#ifdef USB_PRINTER_LANGUAGE_PCL_5
    #include "USB/usb_host_printer_pcl_5.h"
#endif

#ifdef USB_PRINTER_LANGUAGE_POSTSCRIPT
    #include "USB/usb_host_printer_postscript.h"
#endif

#ifndef USB_MALLOC
    #define USB_MALLOC(size) malloc(size)
#endif

#ifndef USB_FREE
    #define USB_FREE(ptr) free(ptr)
#endif

#define USB_FREE_AND_CLEAR(ptr) {USB_FREE(ptr); ptr = NULL;}

//#define DEBUG_MODE
//#define DEBUG_PRINT_COMMANDS
#if defined( DEBUG_MODE ) || defined( DEBUG_PRINT_COMMANDS )
    #include "uart2.h"
#endif


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

// *****************************************************************************
// Section: Interface and Protocol Constants
// *****************************************************************************

#define DEVICE_CLASS_PRINTER                0x07    // Class code for Printers

#define DEVICE_SUBCLASS_PRINTERS            0x01    // SubClass code for Printers

#define DEVICE_INTERFACE_UNIDIRECTIONAL     0x01    // Protocol code for unidirectional interface
#define DEVICE_INTERFACE_BIDIRECTIONAL      0x02    // Protocol code for bidirectional interface
#define DEVICE_INTERFACE_IEEE1284_4         0x03    // Protocol code for IEEE 1284.4 interface


// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Printer Transfer Queue Information

This structure contains the information needed for one entry in the transfer
queue.
*/
typedef struct _USB_PRINTER_QUEUE_ITEM
{
    DWORD   size;
    BYTE    *data;
    BYTE    flags;
} USB_PRINTER_QUEUE_ITEM;


// *****************************************************************************
/* Printer Transfer Queue

This is the structure for the printer transfer queue.
*/
typedef struct _USB_PRINTER_QUEUE
{
    int                     head;
    int                     tail;
    int                     count;
    USB_PRINTER_QUEUE_ITEM  buffer[USB_PRINTER_TRANSFER_QUEUE_SIZE];
} USB_PRINTER_QUEUE;


// *****************************************************************************
/* Printer Device Information

This structure contains information about an attached device, including
status flags and device identification.
*/
typedef struct _USB_PRINTER_DEVICE
{
    USB_PRINTER_DEVICE_ID           ID;             // Identification information about the device
    BYTE                            clientDriverID;
    DWORD                           rxLength;       // Number of bytes received in the last IN transfer
    #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
        char*                       deviceIDString;
        WORD                        deviceIDStringLength;
        WORD                        deviceIDStringIndex;
    #endif
    USB_PRINTER_LANGUAGE_HANDLER    languageHandler;
    BYTE                            endpointIN;     // Bulk IN endpoint
    BYTE                            endpointOUT;    // Bulk OUT endpoint

    USB_PRINTER_QUEUE               transferQueueIN;
    USB_PRINTER_QUEUE               transferQueueOUT;

    union
    {
        BYTE value;                     // BYTE representation of device status flags
        struct
        {
            BYTE inUse                          : 1;    // This array member is in use
            BYTE initialized                    : 1;    // Driver has been initialized
            BYTE txBusy                         : 1;    // Driver busy transmitting data
            BYTE rxBusy                         : 1;    // Driver busy receiving data
            #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
                BYTE deviceIDStringLengthValid  : 1;    // Device ID string length is valid
            #endif
        };
    } flags;                            // Printer client driver status flags

} USB_PRINTER_DEVICE;


// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

BYTE                                    currentPrinterRecord;
extern USB_PRINTER_INTERFACE            usbPrinterClientLanguages[];
USB_PRINTER_DEVICE                      usbPrinters[USB_MAX_PRINTER_DEVICES];
extern USB_PRINTER_SPECIFIC_INTERFACE   usbPrinterSpecificLanguage[];


// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************

BOOL _USBHostPrinter_FindDevice( BYTE address );
#ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
    BOOL _USBHostPrinter_GetDeviceIDString( void );
#endif
BYTE _USBHostPrinter_ReadFromQueue( BYTE deviceAddress );
BYTE _USBHostPrinter_WriteFromQueue( BYTE deviceAddress );

// *****************************************************************************
// *****************************************************************************
// Section: Host Stack Interface Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL USBHostPrinterInitialize ( BYTE address, DWORD flags, BYTE clientDriverID )

  Summary:
    This function is called by the USB Embedded Host layer when a printer
    attaches.

  Description:
    This routine is a call out from the USB Embedded Host layer to the USB
    printer client driver.  It is called when a "printer" device has been
    connected to the host.  Its purpose is to initialize and activate the USB
    Printer client driver.

  Preconditions:
    The device has been configured.

  Parameters:
    BYTE address    - Device's address on the bus
    DWORD flags     - Initialization flags
    BYTE clientDriverID - Client driver identification for device requests

  Return Values:
    TRUE    - Initialization was successful
    FALSE   - Initialization failed

  Remarks:
    Multiple client drivers may be used in a single application.  The USB
    Embedded Host layer will call the initialize routine required for the
    attached device.
  ***************************************************************************/

BOOL USBHostPrinterInitialize ( BYTE address, DWORD flags, BYTE clientDriverID )
{
    BYTE        endpointIN;
    BYTE        endpointOUT;
    #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
        BYTE    errorCode;
    #endif
    WORD        i;
    WORD        j;
    BYTE        *pDesc;
    BYTE        *pDescriptor;

    #ifdef DEBUG_MODE
        UART2PrintString( "PRN: Printer Client Init called\r\n" );
    #endif

    for (currentPrinterRecord=0; currentPrinterRecord<USB_MAX_PRINTER_DEVICES; currentPrinterRecord++)
    {
        if (!usbPrinters[currentPrinterRecord].flags.inUse) break;
    }
    if (currentPrinterRecord == USB_MAX_PRINTER_DEVICES)
    {
        #ifdef DEBUG_MODE
            UART2PrintString( "PRN: No more space\r\n" );
        #endif
        return FALSE;   // We have no more room for a new device.
    }

    pDesc  = USBHostGetDeviceDescriptor(address);

    #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
        usbPrinters[currentPrinterRecord].deviceIDString = (char *)USB_MALLOC( ((USB_DEVICE_DESCRIPTOR*)pDesc)->bMaxPacketSize0 );
        if (usbPrinters[currentPrinterRecord].deviceIDString == NULL)
        {
            #ifdef DEBUG_MODE
                UART2PrintString( "PRN: Out of memory for device ID string length\r\n" );
            #endif
            return FALSE;   // Out of memory
        }
    #endif

    // Initialize state
    usbPrinters[currentPrinterRecord].rxLength                  = 0;
    usbPrinters[currentPrinterRecord].flags.value               = 0x01; // Set the inUse flag.
    #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
        usbPrinters[currentPrinterRecord].deviceIDStringIndex   = 0;
    #endif

    // Save device the address, VID, & PID, and client driver ID.
    usbPrinters[currentPrinterRecord].ID.deviceAddress          = address;
    pDesc += 8;
    usbPrinters[currentPrinterRecord].ID.vid                    =  (WORD)*pDesc;        pDesc++;
    usbPrinters[currentPrinterRecord].ID.vid                    |= ((WORD)*pDesc) << 8; pDesc++;
    usbPrinters[currentPrinterRecord].ID.pid                    =  (WORD)*pDesc;        pDesc++;
    usbPrinters[currentPrinterRecord].ID.pid                    |= ((WORD)*pDesc) << 8; pDesc++;
    usbPrinters[currentPrinterRecord].clientDriverID            = clientDriverID;

    #ifdef DEBUG_MODE
        UART2PrintString( "PRN: USB Printer Client Initalized: flags=0x" );
        UART2PutHex(      flags );
        UART2PrintString( " address=" );
        UART2PutDec( address );
        UART2PrintString( " VID=0x" );
        UART2PutHex(      usbPrinters[currentPrinterRecord].ID.vid >> 8   );
        UART2PutHex(      usbPrinters[currentPrinterRecord].ID.vid & 0xFF );
        UART2PrintString( " PID=0x"      );
        UART2PutHex(      usbPrinters[currentPrinterRecord].ID.pid >> 8   );
        UART2PutHex(      usbPrinters[currentPrinterRecord].ID.pid & 0xFF );
        UART2PrintString( "\r\n"         );
    #endif

    //  Extract the bulk IN and OUT endpoint that the printer uses.

    pDescriptor = (BYTE *)USBHostGetCurrentConfigurationDescriptor( address );

    i = 0;

    #ifdef DEBUG_MODE
        UART2PrintString( "PRN: Checking descriptor " );
        UART2PutDec( pDescriptor[i+5] );
        UART2PrintString( " ...\r\n" );
    #endif

    // Find the next interface descriptor.
    while (i < ((USB_CONFIGURATION_DESCRIPTOR *)pDescriptor)->wTotalLength)
    {
        #ifdef DEBUG_MODE
            UART2PrintString( "PRN:  Checking interface...\r\n" );
        #endif
        // See if we are pointing to an interface descriptor.
        if (pDescriptor[i+1] == USB_DESCRIPTOR_INTERFACE)
        {
            if (USBHostDeviceSpecificClientDriver( address ) ||
                ((pDescriptor[i+5] == DEVICE_CLASS_PRINTER) &&
                 (pDescriptor[i+6] == DEVICE_SUBCLASS_PRINTERS) &&
                 ((pDescriptor[i+7] == DEVICE_INTERFACE_UNIDIRECTIONAL) ||
                  (pDescriptor[i+7] == DEVICE_INTERFACE_BIDIRECTIONAL))))
            {
                // Either this client driver was specified in the TPL for this
                // exact device, or the interface has the correct class,
                // subclass, and protocol values for the printer class.
                #ifdef DEBUG_MODE
                    if (USBHostDeviceSpecificClientDriver( address ))
                    {
                        UART2PrintString( "PRN: Explicit client driver support in TPL.\r\n" );
                    }
                #endif

                // Look for bulk IN and OUT endpoints.
                endpointIN  = 0;
                endpointOUT = 0;

                // Scan for endpoint descriptors.
                i += pDescriptor[i];
                while (pDescriptor[i+1] == USB_DESCRIPTOR_ENDPOINT)
                {
                    if (pDescriptor[i+3] == 0x02) // Bulk
                    {
                        if (((pDescriptor[i+2] & 0x80) == 0x80) && (endpointIN == 0))
                        {
                            endpointIN = pDescriptor[i+2];
                        }
                        if (((pDescriptor[i+2] & 0x80) == 0x00) && (endpointOUT == 0))
                        {
                            endpointOUT = pDescriptor[i+2];
                        }
                    }
                    i += pDescriptor[i];
                }

                if ((endpointIN != 0) && (endpointOUT != 0))
                {
                    // Initialize the device endpoint information.
                    usbPrinters[currentPrinterRecord].endpointIN       = endpointIN;
                    usbPrinters[currentPrinterRecord].endpointOUT      = endpointOUT;
                    #ifdef DEBUG_MODE
                        UART2PrintString( "PRN: Bulk endpoint IN: " );
                        UART2PutHex( endpointIN );
                        UART2PrintString( " Bulk endpoint OUT: " );
                        UART2PutHex( endpointOUT );
                        UART2PrintString( "\r\n" );
                    #endif
                    USBHostSetNAKTimeout( address, endpointIN,  1, USB_NUM_BULK_NAKS );
                    USBHostSetNAKTimeout( address, endpointOUT, 1, USB_NUM_BULK_NAKS );

                    // See if the printer language has been specified explicitly.
                    j = 0;
                    usbPrinters[currentPrinterRecord].languageHandler = NULL;
                    while ((usbPrinterSpecificLanguage[j].vid != 0x0000) &&
                           (usbPrinters[currentPrinterRecord].languageHandler == NULL))
                    {
                        if ((usbPrinterSpecificLanguage[j].vid == usbPrinters[currentPrinterRecord].ID.vid) &&
                            (usbPrinterSpecificLanguage[j].pid == usbPrinters[currentPrinterRecord].ID.pid))
                        {
                            usbPrinters[currentPrinterRecord].languageHandler = usbPrinterClientLanguages[usbPrinterSpecificLanguage[j].languageIndex].languageCommandHandler;
                            usbPrinters[currentPrinterRecord].ID.support      = usbPrinterSpecificLanguage[j].support;
                            #ifdef DEBUG_MODE
                                UART2PrintString( "PRN: Explicit language support. Flags: " );
                                UART2PutHex( (BYTE)usbPrinters[currentPrinterRecord].ID.support.val );
                                UART2PrintString( "\r\n" );
                            #endif
                        }
                        j ++;
                    }

                    if (usbPrinters[currentPrinterRecord].languageHandler != NULL)
                    {
                        // We have a printer language that we can use with this printer.
                        // Complete the client driver attachment.
                        usbPrinters[currentPrinterRecord].flags.initialized = 1;
                        USBHostPrinterCommand( usbPrinters[currentPrinterRecord].ID.deviceAddress, USB_PRINTER_ATTACHED,
                                USB_DATA_POINTER_RAM(&(usbPrinters[currentPrinterRecord].ID.support)), sizeof(USB_PRINTER_FUNCTION_SUPPORT), 0 );

                        // Tell the application layer that we have a device.
                        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_ATTACH, &(usbPrinters[currentPrinterRecord].ID), sizeof(USB_PRINTER_DEVICE_ID) );
                    }
                    else
                    {
                        #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
                            // No printer language has been specified for this printer.
                            // Get the printer device ID string, so we can try to determine
                            // what printer languages it supports.
                            errorCode = USBHostIssueDeviceRequest( address, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE,
                                            PRINTER_DEVICE_REQUEST_GET_DEVICE_ID, 0, 0, ((USB_DEVICE_DESCRIPTOR*)pDeviceDescriptor)->bMaxPacketSize0,
                                            (BYTE *)usbPrinters[currentPrinterRecord].deviceIDString, USB_DEVICE_REQUEST_GET,
                                            usbPrinters[currentPrinterRecord].clientDriverID );
                            if (errorCode)
                            {
                                #ifdef DEBUG_MODE
                                    UART2PrintString( "PRN: Cannot get device ID string length - " );
                                    UART2PutHex( errorCode );
                                    UART2PrintString( "\r\n" );
                                #endif
                                // If we cannot read the ID string, then we cannot determine the required printer language.
                                usbPrinters[currentPrinterRecord].flags.value = 0;
                                return FALSE;
                            }
                            #ifdef DEBUG_MODE
                                UART2PrintString( "PRN: Getting device ID string length...\r\n" );
                            #endif
                        #else
                            // No printer language has been specified for this printer.
                            // We cannot enumerate it.
                            #ifdef DEBUG_MODE
                                UART2PrintString( "PRN: No printer language specified.\r\n" );
                            #endif
                            usbPrinters[currentPrinterRecord].flags.value = 0;
                            return FALSE;
                        #endif
                    }

                    return TRUE;
                }
            }
        }

        // Jump to the next descriptor in this configuration.
        i += pDescriptor[i];
    }

    // This client driver could not initialize the device
    #ifdef DEBUG_MODE
        UART2PrintString( "PRN: Device is not a printer.\r\n" );
    #endif
    usbPrinters[currentPrinterRecord].flags.value = 0;
    return FALSE;

} // USBHostPrinterInitialize


/****************************************************************************
  Function:
    BOOL USBHostPrinterEventHandler ( BYTE address, USB_EVENT event,
                            void *data, DWORD size )

  Summary:
    This routine is called by the Host layer to notify the general client of
    events that occur.

  Description:
    This routine is called by the Host layer to notify the general client of
    events that occur.  If the event is recognized, it is handled and the
    routine returns TRUE.  Otherwise, it is ignored and the routine returns
    FALSE.

    This routine can notify the application with the following events:
        * EVENT_PRINTER_ATTACH
        * EVENT_PRINTER_DETACH
        * EVENT_PRINTER_TX_DONE
        * EVENT_PRINTER_RX_DONE
        * EVENT_PRINTER_REQUEST_DONE
        * EVENT_PRINTER_UNSUPPORTED

  Preconditions:
    None

  Parameters:
    BYTE address    - Address of device with the event
    USB_EVENT event - The bus event that occured
    void *data      - Pointer to event-specific data
    DWORD size      - Size of the event-specific data

  Return Values:
    TRUE    - The event was handled
    FALSE   - The event was not handled

  Remarks:
    None
  ***************************************************************************/

BOOL USBHostPrinterEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    USB_PRINTER_QUEUE_ITEM  *transfer;

    #ifdef DEBUG_MODE
        //UART2PrintString( "PRN: Receiving event\r\n" );
    #endif

    // Make sure it was for our device
    if (!_USBHostPrinter_FindDevice( address ))
    {
        return FALSE;   // The device was not found.
    }

    // Handle specific events.
    switch (event)
    {
        case EVENT_DETACH:
            // Purge the IN and OUT transfer queues.
            while (StructQueueIsNotEmpty( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
            {
                transfer = StructQueueRemove( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE );
            }
            while (StructQueueIsNotEmpty( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
            {
                transfer = StructQueueRemove( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE );
                if (transfer->flags & USB_PRINTER_TRANSFER_COPY_DATA)
                {
                    USB_FREE( transfer->data );
                }
            }

            // Tell the printer language support that the device has been detached.
            USBHostPrinterCommand( usbPrinters[currentPrinterRecord].ID.deviceAddress, USB_PRINTER_DETACHED, USB_NULL, 0, 0 );
            // Notify that application that the device has been detached.
            USB_HOST_APP_EVENT_HANDLER(usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_DETACH,
                    &usbPrinters[currentPrinterRecord].ID.deviceAddress, sizeof(BYTE) );
            #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
                USB_FREE( usbPrinters[currentPrinterRecord].deviceIDString );
            #endif
            usbPrinters[currentPrinterRecord].flags.value = 0;
            #ifdef DEBUG_MODE
                UART2PrintString( "PRN: USB Printer Client Device Detached: address=" );
                UART2PutDec( address );
                UART2PrintString( "\r\n" );
            #endif
            return TRUE;

        case EVENT_TRANSFER:
            #ifdef DEBUG_MODE
                //UART2PrintString( "PRN: TRANSFER event occurred.\r\n" );
            #endif

            if ( (data != NULL) && (size == sizeof(HOST_TRANSFER_DATA)) )
            {
                DWORD                   dataCount = ((HOST_TRANSFER_DATA *)data)->dataCount;

                if ( ((HOST_TRANSFER_DATA *)data)->bEndpointAddress == (USB_IN_EP | usbPrinters[currentPrinterRecord].endpointIN) )
                {
                    usbPrinters[currentPrinterRecord].flags.rxBusy   = 0;
                    usbPrinters[currentPrinterRecord].rxLength       = dataCount;

                    transfer = StructQueueRemove( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE );
                    if (transfer->flags & USB_PRINTER_TRANSFER_COPY_DATA)
                    {
                        USB_FREE( transfer->data );
                    }

                    if (StructQueueIsNotEmpty( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
                    {
                        _USBHostPrinter_ReadFromQueue( usbPrinters[currentPrinterRecord].ID.deviceAddress );
                    }

                    if (transfer->flags & USB_PRINTER_TRANSFER_NOTIFY)
                    {
                        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_RX_DONE, &dataCount, sizeof(DWORD) );
                    }
                }
                else if ( ((HOST_TRANSFER_DATA *)data)->bEndpointAddress == (USB_OUT_EP | usbPrinters[currentPrinterRecord].endpointOUT) )
                {
                    usbPrinters[currentPrinterRecord].flags.txBusy   = 0;

                    transfer = StructQueueRemove( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE );
                    if (transfer->flags & USB_PRINTER_TRANSFER_COPY_DATA)
                    {
                        USB_FREE( transfer->data );
                    }

                    if (StructQueueIsNotEmpty( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
                    {
                        _USBHostPrinter_WriteFromQueue( usbPrinters[currentPrinterRecord].ID.deviceAddress );
                    }

                    if (transfer->flags & USB_PRINTER_TRANSFER_NOTIFY)
                    {
                        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_TX_DONE, &dataCount, sizeof(DWORD) );
                    }
                }
                else if ((((HOST_TRANSFER_DATA *)data)->bEndpointAddress & 0x7F) == 0)
                {
                    #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
                    if (!usbPrinters[currentPrinterRecord].flags.initialized)
                    {
                        #ifdef DEBUG_MODE
                            UART2PrintString( "PRN: Check Device ID string...\r\n" );
                        #endif
                        if (!_USBHostPrinter_GetDeviceIDString())
                        {
                            #ifdef DEBUG_MODE
                                UART2PrintString( "PRN: Sending Unsupported Printer event...\r\n" );
                            #endif
                            USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_UNSUPPORTED, NULL, 0 );
                        }
                    }
                    else
                    #endif
                    {
                        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_REQUEST_DONE, &dataCount, sizeof(DWORD) );
                    }
                }
                else
                {
                    // Event is on an unknown endpoint.
                    return FALSE;
                }
                return TRUE;
            }
            else
            {
                // The data does not match what was expected for this event.
                return FALSE;
            }
            break;

        case EVENT_BUS_ERROR:
            // A bus error occurred. Clean up the best we can.
            #ifdef DEBUG_MODE
                UART2PrintString( "PRN: BUS_ERROR event occurred.\r\n" );
            #endif

            if ( (data != NULL) && (size == sizeof(HOST_TRANSFER_DATA)) )
            {
                if ( ((HOST_TRANSFER_DATA *)data)->bEndpointAddress == (USB_IN_EP | usbPrinters[currentPrinterRecord].endpointIN) )
                {
                    usbPrinters[currentPrinterRecord].flags.rxBusy   = 0;
                    usbPrinters[currentPrinterRecord].rxLength       = 0;

                    transfer = StructQueueRemove( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE );
                    if (transfer->flags & USB_PRINTER_TRANSFER_COPY_DATA)
                    {
                        USB_FREE( transfer->data );
                    }

                    if (StructQueueIsNotEmpty( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
                    {
                        _USBHostPrinter_ReadFromQueue( usbPrinters[currentPrinterRecord].ID.deviceAddress );
                    }

//                    if (transfer->flags & USB_PRINTER_TRANSFER_NOTIFY)
                    {
                        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_RX_ERROR, NULL, ((HOST_TRANSFER_DATA *)data)->bErrorCode );
                    }
                }
                else if ( ((HOST_TRANSFER_DATA *)data)->bEndpointAddress == (USB_OUT_EP | usbPrinters[currentPrinterRecord].endpointOUT) )
                {
                    usbPrinters[currentPrinterRecord].flags.txBusy   = 0;

                    transfer = StructQueueRemove( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE );
                    if (transfer->flags & USB_PRINTER_TRANSFER_COPY_DATA)
                    {
                        USB_FREE( transfer->data );
                    }

                    if (StructQueueIsNotEmpty( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
                    {
                        _USBHostPrinter_WriteFromQueue( usbPrinters[currentPrinterRecord].ID.deviceAddress );
                    }

//                    if (transfer->flags & USB_PRINTER_TRANSFER_NOTIFY)
                    {
                        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_TX_ERROR, NULL, ((HOST_TRANSFER_DATA *)data)->bErrorCode );
                    }
                }
                else if ((((HOST_TRANSFER_DATA *)data)->bEndpointAddress & 0x7F) == 0)
                {
                    #ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
                    if (!usbPrinters[currentPrinterRecord].flags.initialized &&
                        !usbPrinters[currentPrinterRecord].flags.deviceIDStringLengthValid)
                    {
                        // The printer does not support the GET_DEVICE_ID printer class request.
                        // We cannot determine a printer language for this printer.
                        #ifdef DEBUG_MODE
                            UART2PrintString( "PRN: Required specific printer language not found\r\n" );
                        #endif
                        usbPrinters[currentPrinterRecord].flags.value = 0;
                        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_UNSUPPORTED, NULL, 0 );

                    }
                    else
                    #endif
                    {
                        // The printer gave an error during an application control transfer.
                        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_REQUEST_ERROR, NULL, ((HOST_TRANSFER_DATA *)data)->bErrorCode );
                    }
                }
                else
                {
                    // Event is on an unknown endpoint.
                    return FALSE;
                }
                return TRUE;
            }
            else
            {
                // The data does not match what was expected for this event.
                return FALSE;
            }
            break;

        case EVENT_SUSPEND:
        case EVENT_RESUME:
        default:
            break;
    }

    return FALSE;
} // USBHostPrinterEventHandler


// *****************************************************************************
// *****************************************************************************
// Section: Application Callable Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BYTE USBHostPrinterCommand( BYTE deviceAddress, USB_PRINTER_COMMAND command,
                    USB_DATA_POINTER data, DWORD size, BYTE flags )

  Summary:
    This is the primary user interface function for the printer client
    driver.  It is used to issue all printer commands.

  Description:
    This is the primary user interface function for the printer client
    driver.  It is used to issue all printer commands.  Each generic printer
    command is translated to the appropriate command for the supported
    language and is enqueued for transfer to the printer.  Before calling
    this routine, it is recommended to call the function
    USBHostPrinterCommandReady() to determine if there is space available
    in the printer's output queue.

  Preconditions:
    None

  Parameters:
    BYTE address                - Device's address on the bus
    USB_PRINTER_COMMAND command - Command to execute.  See the enumeration
                                    USB_PRINTER_COMMAND for the list of
                                    valid commands and their requirements.
    USB_DATA_POINTER data       - Pointer to the required data.  Note that
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

  Returns:
    See the USB_PRINTER_ERRORS enumeration.  Also, refer to the printer
    language command handler function, such as
    USBHostPrinterLanguagePostScript().

  Example:
    <code>
    if (USBHostPrinterCommandReady( address ))
    {
        USBHostPrinterCommand( address, USB_PRINTER_JOB_START, NULL, 0, 0 );
    }
    </code>

  Remarks:
    Use the definitions USB_DATA_POINTER_RAM() and USB_DATA_POINTER_ROM()
    to cast data pointers.  For example:
    <code>
        USBHostPrinterCommand( address, USB_PRINTER_TEXT, USB_DATA_POINTER_RAM(buffer), strlen(buffer), 0 );
    </code>

    When developing new commands, keep in mind that the function
    USBHostPrinterCommandReady() will often be used before calling this
    function to see if there is space available in the output transfer queue.
    USBHostPrinterCommandReady() will routine TRUE if a single space is
    available in the output queue.  Therefore, each command can generate only
    one output transfer.
  ***************************************************************************/

BYTE USBHostPrinterCommand( BYTE deviceAddress, USB_PRINTER_COMMAND command,
                    USB_DATA_POINTER data, DWORD size, BYTE flags )
{
    if (!_USBHostPrinter_FindDevice( deviceAddress ))
    {
        // The device was not found.
        return USB_PRINTER_UNKNOWN_DEVICE;
    }

    return usbPrinters[currentPrinterRecord].languageHandler( deviceAddress, command, data, size, flags );
}


/****************************************************************************
  Function:
    BOOL USBHostPrinterCommandReady( BYTE deviceAddress )

  Description:
    This interface is used to check if the client driver has space available
    to enqueue another transfer.

  Preconditions:
    None

  Parameters:
    deviceAddress     - USB Address of the device

  Return Values:
    TRUE    - The printer client driver has room for at least one more
                transfer request, or the device is not attached.  The latter
                allows this routine to be called without generating an
                infinite loop if the device detaches.
    FALSE   - The transfer queue is full.

  Example:
    <code>
    if (USBHostPrinterCommandReady( address ))
    {
        USBHostPrinterCommand( address, USB_PRINTER_JOB_START, NULL, 0, 0 );
    }
    </code>

  Remarks:
    This routine will return TRUE if a single transfer can be enqueued.  Since
    this routine is the check to see if USBHostPrinterCommand() can be called,
    every command can generate at most one transfer.
  ***************************************************************************/

BOOL USBHostPrinterCommandReady( BYTE deviceAddress )
{
    if (!_USBHostPrinter_FindDevice( deviceAddress ))
    {
        // The device was not found.
        return TRUE;
    }

    if (StructQueueSpaceAvailable( 1, &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
    {
        return TRUE;
    }
    return FALSE;
}


/****************************************************************************
  Function:
    BOOL USBHostPrinterDeviceDetached( BYTE deviceAddress )

  Description:
    This interface is used to check if the device has been detached from the
    bus.

  Preconditions:
    None

  Parameters:
    BYTE deviceAddress  - USB Address of the device.

  Return Values:
    TRUE    - The device has been detached, or an invalid deviceAddress is given.
    FALSE   - The device is attached

  Example:
    <code>
    if (USBHostPrinterDeviceDetached( deviceAddress ))
    {
        // Handle detach
    }
    </code>

  Remarks:
    The event EVENT_PRINTER_DETACH can also be used to detect a detach.
  ***************************************************************************/

BOOL USBHostPrinterDeviceDetached( BYTE deviceAddress )
{
    if (_USBHostPrinter_FindDevice( deviceAddress ))
    {
        return FALSE;
    }
    return TRUE;
}


/****************************************************************************
  Function:
    DWORD USBHostPrinterGetRxLength( BYTE deviceAddress )

  Description:
    This function retrieves the number of bytes copied to user's buffer by
    the most recent call to the USBHostPrinterRead() function.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    BYTE deviceAddress  - USB Address of the device

  Returns:
    Returns the number of bytes most recently received from the Generic
    device with address deviceAddress.

  Remarks:
    None
  ***************************************************************************/

DWORD USBHostPrinterGetRxLength( BYTE deviceAddress )
{
    if (!_USBHostPrinter_FindDevice( deviceAddress ))
    {
        return 0;
    }
    return usbPrinters[currentPrinterRecord].rxLength;
}


/****************************************************************************
  Function:
    BYTE USBHostPrinterGetStatus( BYTE deviceAddress, BYTE *status )

  Summary:
    This function issues the Printer class-specific Device Request to
    obtain the printer status.

  Description:
    This function issues the Printer class-specific Device Request to
    obtain the printer status.  The returned status should have the following
    format, per the USB specification.  Any deviation will be due to the
    specific printer implementation.
    * Bit 5 - Paper Empty; 1 = paper empty, 0 = paper not empty
    * Bit 4 - Select; 1 = selected, 0 = not selected
    * Bit 3 - Not Error; 1 = no error, 0 = error
    * All other bits are reserved.

    The *status parameter is not updated until the EVENT_PRINTER_REQUEST_DONE
    event is thrown.  Until that point the value of *status is unknown.

    The *status parameter will only be updated if this function returns
    USB_SUCCESS.  If this function returns with any other error code then the
    EVENT_PRINTER_REQUEST_DONE event will not be thrown and the status field
    will not be updated.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    deviceAddress   - USB Address of the device
    *status         - pointer to the returned status byte

  Returns:
    See the return values for the USBHostIssueDeviceRequest() function.

  Remarks:
    None
  ***************************************************************************/

BYTE USBHostPrinterGetStatus( BYTE deviceAddress, BYTE *status )
{
    if (!_USBHostPrinter_FindDevice( deviceAddress ))
    {
        return USB_UNKNOWN_DEVICE;
    }

   return USBHostIssueDeviceRequest( deviceAddress,
        USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE,
        PRINTER_DEVICE_REQUEST_GET_PORT_STATUS,
        0, 0x0000, 1,
        status, USB_DEVICE_REQUEST_GET, usbPrinters[currentPrinterRecord].clientDriverID );
}


/****************************************************************************
  Function:
    BYTE USBHostPrinterRead( BYTE deviceAddress, BYTE *buffer, DWORD length,
                BYTE transferFlags )

  Description:
    Use this routine to receive from the device and store it into memory.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    deviceAddress  - USB Address of the device.
    buffer         - Pointer to the data buffer
    length         - Number of bytes to be transferred
    transferFlags  - Flags for how to perform the operation

  Return Values:
    USB_SUCCESS         - The Read was started successfully
    (USB error code)    - The Read was not started.  See USBHostRead() for
                            a list of errors.

  Example:
    <code>
    if (!USBHostPrinterRxIsBusy( deviceAddress ))
    {
        USBHostPrinterRead( deviceAddress, &buffer, sizeof(buffer), 0 );
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BYTE USBHostPrinterRead( BYTE deviceAddress, void *buffer, DWORD length,
            BYTE transferFlags )
{
    USB_PRINTER_QUEUE_ITEM  *transfer;

    // Validate the call
    if (!_USBHostPrinter_FindDevice( deviceAddress ) ||
        !usbPrinters[currentPrinterRecord].flags.initialized)
    {
        return USB_INVALID_STATE;
    }

    // Check transfer path
    if (StructQueueIsFull( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
    {
        return USB_PRINTER_BUSY;
    }

    // Add the newest transfer to the queue

    transfer = StructQueueAdd( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE);
    transfer->data  = buffer;
    transfer->size  = length;
    transfer->flags = transferFlags;

    if (usbPrinters[currentPrinterRecord].flags.rxBusy)
    {
        // The request has been queued.  We'll execute it when the current transfer is complete.
        return USB_SUCCESS;
    }
    else
    {
        return _USBHostPrinter_ReadFromQueue( deviceAddress );
    }
} // USBHostPrinterRead


/****************************************************************************
  Function:
    BYTE USBHostPrinterReset( BYTE deviceAddress )

  Description:
    This function issues the Printer class-specific Device Request to
    perform a soft reset.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    BYTE deviceAddress  - USB Address of the device

  Returns:
    See the return values for the USBHostIssueDeviceRequest() function.

  Remarks:
    Not all printers support this command.
  ***************************************************************************/

BYTE USBHostPrinterReset( BYTE deviceAddress )
{
    if (!_USBHostPrinter_FindDevice( deviceAddress ))
    {
        return USB_UNKNOWN_DEVICE;
    }

    return USBHostIssueDeviceRequest( deviceAddress,
        USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE,
        PRINTER_DEVICE_REQUEST_SOFT_RESET,
        0, 0x0000, 0,
        NULL, USB_DEVICE_REQUEST_SET, usbPrinters[currentPrinterRecord].clientDriverID );
}


/****************************************************************************
  Function:
    BOOL USBHostPrinterRxIsBusy( BYTE deviceAddress )

  Summary:
    This interface is used to check if the client driver is currently busy
    receiving data from the device.

  Description:
    This interface is used to check if the client driver is currently busy
    receiving data from the device.  This function is intended for use with
    transfer events.  With polling, the function USBHostPrinterRxIsComplete()
    should be used.

  Preconditions:
    None

  Parameters:
    deviceAddress     - USB Address of the device

  Return Values:
    TRUE    - The device is receiving data or an invalid deviceAddress is
                given.
    FALSE   - The device is not receiving data

  Example:
    <code>
    if (!USBHostPrinterRxIsBusy( deviceAddress ))
    {
        USBHostPrinterRead( deviceAddress, &Buffer, sizeof( Buffer ) );
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BOOL USBHostPrinterRxIsBusy( BYTE deviceAddress )
{
    if (!_USBHostPrinter_FindDevice( deviceAddress ))
    {
        // The device was not found.
        return TRUE;
    }

    if (usbPrinters[currentPrinterRecord].flags.rxBusy)
    {
        return TRUE;
    }
    return FALSE;
}


/****************************************************************************
  Function:
    BYTE USBHostPrinterWrite( BYTE deviceAddress, void *buffer, DWORD length,
                BYTE transferFlags )

  Description:
    Use this routine to transmit data from memory to the device.  This
    routine will not usually be called by the application directly.  The
    application will use the USBHostPrinterCommand() function, which will
    call the appropriate printer language support function, which will
    utilize this routine.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    BYTE deviceAddress  - USB Address of the device.
    void *buffer        - Pointer to the data buffer
    DWORD length        - Number of bytes to be transferred
    BYTE transferFlags  - Flags for how to perform the operation

  Return Values:
    USB_SUCCESS                 - The Write was started successfully.
    USB_PRINTER_UNKNOWN_DEVICE  - Device not found or has not been initialized.
    USB_PRINTER_BUSY            - The printer's output queue is full.
    (USB error code)            - The Write was not started.  See USBHostWrite() for
                                    a list of errors.

  Remarks:
    None
  ***************************************************************************/


BYTE USBHostPrinterWrite( BYTE deviceAddress, void *buffer, DWORD length,
            BYTE transferFlags)
{
    USB_PRINTER_QUEUE_ITEM  *transfer;

    // Validate the call
    if (!_USBHostPrinter_FindDevice( deviceAddress ) ||
        !usbPrinters[currentPrinterRecord].flags.initialized)
    {
        return USB_PRINTER_UNKNOWN_DEVICE;
    }

    // Check transfer path
    if (StructQueueIsFull( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE ))
    {
        // The queue is full.  If the caller was using heap space for the data,
        // deallocate the memory.
        if (transferFlags & USB_PRINTER_TRANSFER_COPY_DATA)
        {
            USB_FREE( buffer );
        }
        return USB_PRINTER_BUSY;
    }

    // Add the newest transfer to the queue

    transfer = StructQueueAdd( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE );
    transfer->data  = buffer;
    transfer->size  = length;
    transfer->flags = transferFlags;

    if (usbPrinters[currentPrinterRecord].flags.txBusy)
    {
        // The request has been queued.  We'll execute it when the current transfer is complete.
        return USB_SUCCESS;
    }
    else
    {
        // We can send the request now.  We still have to put it in the
        // queue, because that's where the event handler gets its information.
        return _USBHostPrinter_WriteFromQueue( deviceAddress );
    }
}


/****************************************************************************
  Function:
    BOOL USBHostPrinterWriteComplete( BYTE deviceAddress )

  Description:
    This interface is used to check if the client driver is currently
    transmitting data to the printer, or if it is between transfers but still
    has transfers queued.

  Preconditions:
    None

  Parameters:
    deviceAddress     - USB Address of the device

  Return Values:
    TRUE    - The device is done transmitting data or an invalid deviceAddress
                is given.
    FALSE   - The device is transmitting data or has a transfer in the queue.

  Remarks:
    None
  ***************************************************************************/

BOOL USBHostPrinterWriteComplete( BYTE deviceAddress )
{
    if (!_USBHostPrinter_FindDevice( deviceAddress ))
    {
        // The device was not found.
        return TRUE;
    }

    if (usbPrinters[currentPrinterRecord].flags.txBusy ||
        !(StructQueueIsEmpty( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE )))
    {
        return FALSE;
    }
    return TRUE;
}


// *****************************************************************************
// *****************************************************************************
// Section: Internal Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL _USBHostPrinter_FindDevice( BYTE address )

  Description:
    This function looks in the array of printers to see if there is currently
    a printer attached at the indicated device.  If so, it returns TRUE, and
    the global variable currentPrinterRecord is changed to the index of the
    printer.  If not, it returns FALSE.  Note that the function implies
    nothing about the status of the printer; it may not be ready to use.

  Preconditions:
    None

  Parameters:
    deviceAddress   - USB Address of the device.

  Return Values:
    TRUE    - The indicated device is attached
    FALSE   - The indicated device is not attached

  Remarks:
    None
  ***************************************************************************/

BOOL _USBHostPrinter_FindDevice( BYTE address )
{
    for (currentPrinterRecord=0; currentPrinterRecord<USB_MAX_PRINTER_DEVICES; currentPrinterRecord++)
    {
        if (usbPrinters[currentPrinterRecord].flags.inUse &&
            (usbPrinters[currentPrinterRecord].ID.deviceAddress == address))
        {
            return TRUE;
        }
    }
    return FALSE;   // The device was not found.
}


/****************************************************************************
  Function:
    BOOL _USBHostPrinter_GetDeviceIDString( void )

  Description:
    This routine performs most of the process required to get the device ID
    string.  The initial request to get the length is performed in the
    initialization handler.  This routine handles the processing of the
    length, the request for the entire string, and processing of the string.
    The format of this string is specified by IEEE 1284.

  Preconditions:
    None

  Parameters:
    None - None

  Return Values:
    TRUE    - Processing is proceeding normally
    FALSE   - Cannot read the device ID string, or cannot find a printer
                language to use to communicate with the printer.

  Remarks:
    This function is available only if USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION
    has been defined.
  ***************************************************************************/

#ifdef USB_PRINTER_ALLOW_DYNAMIC_LANGUAGE_DETERMINATION

BOOL _USBHostPrinter_GetDeviceIDString( void )
{
    BYTE errorCode;

    if (!usbPrinters[currentPrinterRecord].flags.deviceIDStringLengthValid)
    {
        // This transfer was to get the length of the string.
        // The length is a 2-byte value, MSB first.
        usbPrinters[currentPrinterRecord].deviceIDStringLength = ( (BYTE)usbPrinters[currentPrinterRecord].deviceIDString[0] << 8) +  (BYTE)usbPrinters[currentPrinterRecord].deviceIDString[1];
        usbPrinters[currentPrinterRecord].flags.deviceIDStringLengthValid = 1;

        #ifdef DEBUG_MODE
            UART2PrintString( "PRN: Got device ID string length: " );
            UART2PutHex( usbPrinters[currentPrinterRecord].deviceIDStringLength >> 8);
            UART2PutHex( usbPrinters[currentPrinterRecord].deviceIDStringLength );
            UART2PrintString( "\r\n" );
        #endif

        USB_FREE( usbPrinters[currentPrinterRecord].deviceIDString );
        usbPrinters[currentPrinterRecord].deviceIDString = (char *)USB_MALLOC( usbPrinters[currentPrinterRecord].deviceIDStringLength + 3 );
        if (usbPrinters[currentPrinterRecord].deviceIDString == NULL)
        {
            #ifdef DEBUG_MODE
                UART2PrintString( "PRN: Out of memory for the device ID string.\r\n" );
            #endif
            usbPrinters[currentPrinterRecord].flags.value = 0;
            return FALSE;
        }
        else
        {
            // Get the full string
            errorCode = USBHostIssueDeviceRequest(  usbPrinters[currentPrinterRecord].ID.deviceAddress,
                            USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE,
                            PRINTER_DEVICE_REQUEST_GET_DEVICE_ID, 0, 0, usbPrinters[currentPrinterRecord].deviceIDStringLength,
                            (BYTE *)usbPrinters[currentPrinterRecord].deviceIDString, USB_DEVICE_REQUEST_GET,
                            usbPrinters[currentPrinterRecord].clientDriverID );
            if (errorCode)
            {
                #ifdef DEBUG_MODE
                    UART2PrintString( "PRN: Cannot get device ID string  - " );
                    UART2PutHex( errorCode );
                    UART2PrintString( "\r\n" );
                #endif
                // If we cannot read the ID string, then we cannot determine the required printer language.
                usbPrinters[currentPrinterRecord].flags.value = 0;
                return FALSE;
            }
            #ifdef DEBUG_MODE
                UART2PrintString( "PRN: Getting device ID string ...\r\n" );
            #endif
        }
    }
    else
    {
        char                            *commandSet;
        WORD                            i;
        WORD                            semicolonLocation;

        // Null terminate the device ID string so we can do string manipulation on it.
        usbPrinters[currentPrinterRecord].deviceIDString[usbPrinters[currentPrinterRecord].deviceIDStringLength + 2] = 0;

        #ifdef DEBUG_MODE
            UART2PrintString( "PRN: Got device ID string: (Length " );
            UART2PutHex( usbPrinters[currentPrinterRecord].deviceIDString[0] );
            UART2PutHex( usbPrinters[currentPrinterRecord].deviceIDString[1] );
            UART2PrintString( ")\r\n" );
            for (i=2; i<usbPrinters[currentPrinterRecord].deviceIDStringLength; i++)
            {
                UART2PutChar( usbPrinters[currentPrinterRecord].deviceIDString[i] );
            }
            UART2PrintString( "\r\n" );
        #endif

        // Determine if one of the languages we are currently using can support this
        // printer.  Languages should be listed in ascending order of preference
        // in the usbPrinterClientLanguages array, so we can stop looking as soon
        // as we find a match.
        commandSet = strstr( &(usbPrinters[currentPrinterRecord].deviceIDString[2]), "COMMAND SET:" );
        if (!commandSet)
        {
            commandSet = strstr( &(usbPrinters[currentPrinterRecord].deviceIDString[2]), "CMD:" );
        }
        if (!commandSet)
        {
            // No printer language support is indicated.
            #ifdef DEBUG_MODE
                UART2PrintString( "PRN: Device ID string did not contain command set\r\n" );
            #endif
            usbPrinters[currentPrinterRecord].flags.value = 0;
            return FALSE;
        }

        // Replace the semicolon at the end of the COMMAND SET: specification with a
        // null, so we don't get a false positive based on some other portion of the
        // device ID string.  If we don't find a semicolon, set the location to 0.
        for (semicolonLocation = 0;
             (commandSet[semicolonLocation] != 0) && (commandSet[semicolonLocation] != ';');
             semicolonLocation ++ ) {};
        if (commandSet[semicolonLocation] == 0)
        {
            semicolonLocation = 0;
        }
        else
        {
            commandSet[semicolonLocation] = 0;
        }

        // Convert the command set to all upper case.
        for (i=0; i<semicolonLocation; i++)
        {
            commandSet[i] = toupper( commandSet[i] );
        }

        // Look for a supported printer language in the array of available languages.
        i = 0;
        usbPrinters[currentPrinterRecord].languageHandler = NULL;
        while ((usbPrinterClientLanguages[i].isLanguageSupported != NULL) &&
               (usbPrinters[currentPrinterRecord].languageHandler == NULL))
        {
            if (usbPrinterClientLanguages[i].isLanguageSupported( commandSet, &(usbPrinters[currentPrinterRecord].ID.support) ))
            {
                usbPrinters[currentPrinterRecord].languageHandler = usbPrinterClientLanguages[i].languageCommandHandler;
                #ifdef DEBUG_MODE
                    UART2PrintString( "PRN: Printer language support: " );
                    UART2PutHex( usbPrinters[currentPrinterRecord].ID.support.val );
                    UART2PrintString( "\r\n" );
                #endif
            }
            i ++;
        }

        // Restore the device ID string to its original state.
        if (semicolonLocation != 0)
        {
            commandSet[semicolonLocation] = ';';
        }

        // See if we were able to find a printer language.
        if (usbPrinters[currentPrinterRecord].languageHandler == NULL)
        {
            #ifdef DEBUG_MODE
                UART2PrintString( "PRN: Required printer language not found\r\n" );
            #endif
            usbPrinters[currentPrinterRecord].flags.value = 0;
            return FALSE;
        }

        // We have a printer language that we can use with this printer.
        #ifdef DEBUG_MODE
            UART2PrintString( "PRN: Required printer language found\r\n" );
        #endif
        usbPrinters[currentPrinterRecord].flags.initialized = 1;
        USBHostPrinterCommand( usbPrinters[currentPrinterRecord].ID.deviceAddress, USB_PRINTER_ATTACHED,
                USB_DATA_POINTER_RAM(&(usbPrinters[currentPrinterRecord].ID.support)), sizeof(USB_PRINTER_FUNCTION_SUPPORT), 0 );

        // Tell the application layer that we have a device.
        USB_HOST_APP_EVENT_HANDLER( usbPrinters[currentPrinterRecord].ID.deviceAddress, EVENT_PRINTER_ATTACH,
                &(usbPrinters[currentPrinterRecord].ID), sizeof(USB_PRINTER_DEVICE_ID) );
    }

    return TRUE;
}

#endif


/****************************************************************************
  Function:
    void _USBHostPrinter_ReadFromQueue( BYTE deviceAddress )

  Description:
    This routine initiates the IN transfer described by the first entry
    in the queue.

  Preconditions:
    * The receive path must be clear before calling this routine.
    * The queue must contain at least one valid entry.
    * currentPrinterRecord must be valid.

  Parameters:
    deviceAddress  - USB Address of the device.

  Return Values:
    USB_SUCCESS         - The Read was started successfully
    (USB error code)    - The Read was not started.  See USBHostRead() for
                            a list of errors.

  Remarks:
    None
  ***************************************************************************/

BYTE _USBHostPrinter_ReadFromQueue( BYTE deviceAddress )
{
    BYTE                    returnValue;
    USB_PRINTER_QUEUE_ITEM  *transfer;

    transfer = StructQueuePeekTail( &(usbPrinters[currentPrinterRecord].transferQueueIN), USB_PRINTER_TRANSFER_QUEUE_SIZE );

    usbPrinters[currentPrinterRecord].flags.rxBusy = 1;
    usbPrinters[currentPrinterRecord].rxLength     = 0;
    returnValue = USBHostRead( deviceAddress, USB_IN_EP|usbPrinters[currentPrinterRecord].endpointIN, (BYTE *)(transfer->data), transfer->size );
    if (returnValue != USB_SUCCESS)
    {
        usbPrinters[currentPrinterRecord].flags.rxBusy = 0;    // Clear flag to allow re-try
    }

    return returnValue;
}


/****************************************************************************
  Function:
    void _USBHostPrinter_WriteFromQueue( BYTE deviceAddress )

  Description:
    This routine initiates the OUT transfer described by the first entry
    in the queue.

  Preconditions:
    * The transmit path must be clear before calling this routine.
    * The queue must contain at least one valid entry.
    * currentPrinterRecord must be valid.

  Parameters:
    deviceAddress  - USB Address of the device.

  Return Values:
    USB_SUCCESS         - The Read was started successfully
    (USB error code)    - The Read was not started.  See USBHostWrite() for
                            a list of errors.

  Remarks:
    None
  ***************************************************************************/

BYTE _USBHostPrinter_WriteFromQueue( BYTE deviceAddress )
{
    BYTE                    returnValue;
    USB_PRINTER_QUEUE_ITEM  *transfer;
    #if defined( DEBUG_MODE ) || defined( DEBUG_PRINT_COMMANDS )
        //BYTE                i;
    #endif

    transfer = StructQueuePeekTail( &(usbPrinters[currentPrinterRecord].transferQueueOUT), USB_PRINTER_TRANSFER_QUEUE_SIZE );

    #if defined( DEBUG_MODE )
        //UART2PrintString( "PRN: OUT, " );
    #endif
    #if defined( DEBUG_MODE ) || defined( DEBUG_PRINT_COMMANDS )
        //for (i=0; i<transfer->size; i++) UART2PutChar(transfer->data[i]);
        //UART2PutHex( transfer->size >> 8 );
        //UART2PutHex( transfer->size );
        //UART2PrintString( "\r\n" );
    #endif

    usbPrinters[currentPrinterRecord].flags.txBusy = 1;
    returnValue = USBHostWrite( deviceAddress, USB_OUT_EP|usbPrinters[currentPrinterRecord].endpointOUT, (BYTE *)(transfer->data), transfer->size );
    if (returnValue != USB_SUCCESS)
    {
        usbPrinters[currentPrinterRecord].flags.txBusy = 0;    // Clear flag to allow re-try
    }

    return returnValue;
}


/*************************************************************************
 * EOF usb_client_generic.c
 */


