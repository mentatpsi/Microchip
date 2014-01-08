/******************************************************************************
 
  USB Host Generic Client Driver

Description:
    This is the Generic client driver file for a USB Embedded Host device.  This
    driver should be used in a project with usb_host.c to provided the USB
    hardware interface.

    To interface with USB Embedded Host layer, the routine USBHostGenericInit()
    should be specified as the Initialize() function, and
    USBHostGenericEventHandler() should be specified as the EventHandler()
    function in the usbClientDrvTable[] array declared in usb_config.c.

    This driver can be configured to either use transfer events from usb_host.c
    or use a polling mechanism.  If USB_ENABLE_TRANSFER_EVENT is defined, this
    driver will utilize transfer events.  Otherwise, this driver will utilize
    polling.

    Since the generic class is performed with interrupt transfers,
    USB_SUPPORT_INTERRUPT_TRANSFERS must be defined.

Summary:
    This is the Generic client driver file for a USB Embedded Host device.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* FileName:        usb_client_generic.c
* Dependencies:    None
* Processor:       PIC24/dsPIC30/dsPIC33/PIC32MX
* Compiler:        C30 v2.01/C32 v0.00.18
* Company:         Microchip Technology, Inc.

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

Author          Date    Comments
--------------------------------------------------------------------------------
BC/KO       25-Dec-2007 First release

*******************************************************************************/
//DOM-IGNORE-END

#include <stdlib.h>
#include <string.h>
#include "GenericTypeDefs.h"
#include "USB/usb.h"
#include "USB/usb_host_generic.h"

//#define DEBUG_MODE
#ifdef DEBUG_MODE
    #include "uart2.h"
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Max Number of Supported Devices

This value represents the maximum number of attached devices this class driver
can support.  If the user does not define a value, it will be set to 1.
Currently this must be set to 1, due to limitations in the USB Host layer.
*/
#ifndef USB_MAX_GENERIC_DEVICES
    #define USB_MAX_GENERIC_DEVICES     1
#endif

#if USB_MAX_GENERIC_DEVICES != 1
    #error The Generic client driver supports only one attached device.
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

GENERIC_DEVICE  gc_DevData;

#ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
    #ifndef USB_GENERIC_MAX_SERIAL_NUMBER
        #define USB_GENERIC_MAX_SERIAL_NUMBER   64
    #endif

    WORD    serialNumbers[USB_MAX_GENERIC_DEVICES][USB_GENERIC_MAX_SERIAL_NUMBER];
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Host Stack Interface Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL USBHostGenericInit ( BYTE address, DWORD flags, BYTE clientDriverID )

  Summary:
    This function is called by the USB Embedded Host layer when a "generic"
    device attaches.

  Description:
    This routine is a call out from the USB Embedded Host layer to the USB
    generic client driver.  It is called when a "generic" device has been
    connected to the host.  Its purpose is to initialize and activate the USB
    Generic client driver.

  Preconditions:
    The device has been configured.

  Parameters:
    BYTE address    - Device's address on the bus
    DWORD flags     - Initialization flags
    BYTE clientDriverID - ID to send when issuing a Device Request via
                            USBHostIssueDeviceRequest(), USBHostSetDeviceConfiguration(),
                            or USBHostSetDeviceInterface().  

  Return Values:
    TRUE    - Initialization was successful
    FALSE   - Initialization failed

  Remarks:
    Multiple client drivers may be used in a single application.  The USB
    Embedded Host layer will call the initialize routine required for the
    attached device.
  ***************************************************************************/

BOOL USBHostGenericInit ( BYTE address, DWORD flags, BYTE clientDriverID )
{
    BYTE *pDesc;

    // Initialize state
    gc_DevData.rxLength     = 0;
    gc_DevData.flags.val = 0;

    // Save device the address, VID, & PID
    gc_DevData.ID.deviceAddress = address;
    pDesc  = USBHostGetDeviceDescriptor(address);
    pDesc += 8;
    gc_DevData.ID.vid  =  (WORD)*pDesc;       pDesc++;
    gc_DevData.ID.vid |= ((WORD)*pDesc) << 8; pDesc++;
    gc_DevData.ID.pid  =  (WORD)*pDesc;       pDesc++;
    gc_DevData.ID.pid |= ((WORD)*pDesc) << 8; pDesc++;

    // Save the Client Driver ID
    gc_DevData.clientDriverID = clientDriverID;
    
    #ifdef DEBUG_MODE
        UART2PrintString( "GEN: USB Generic Client Initalized: flags=0x" );
        UART2PutHex(      flags );
        UART2PrintString( " address=" );
        UART2PutDec( address );
        UART2PrintString( " VID=0x" );
        UART2PutHex(      gc_DevData.ID.vid >> 8   );
        UART2PutHex(      gc_DevData.ID.vid & 0xFF );
        UART2PrintString( " PID=0x"      );
        UART2PutHex(      gc_DevData.ID.pid >> 8   );
        UART2PutHex(      gc_DevData.ID.pid & 0xFF );
        UART2PrintString( "\r\n"         );
    #endif

    #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
    {
        BYTE    *deviceDescriptor;
        BYTE    serialNumberIndex;

        // MCHP - when multiple devices are implemented, this init will change
        // to find a free slot
        gc_DevData.flags.serialNumberValid = 0;

        gc_DevData.ID.serialNumber = &(serialNumbers[0][0]);
        gc_DevData.ID.serialNumberLength = 0;

        deviceDescriptor = USBHostGetDeviceDescriptor( deviceAddress );
        serialNumberIndex = deviceDescriptor[16];

        if (serialNumberIndex)
        {
            #ifdef DEBUG_MODE
                UART2PrintString( "GEN: Getting serial number...\r\n" );
            #endif
        }
        else
        {
            serialNumberIndex = 1;
            #ifdef DEBUG_MODE
                UART2PrintString( "GEN: Getting string descriptor...\r\n" );
            #endif
        }

        if (USBHostGetStringDescriptor( address, serialNumberIndex, (BYTE *)gc_DevData.ID.serialNumber, USB_GENERIC_MAX_SERIAL_NUMBER*2 ))
        {
            // We can't get the serial number.  Just set the pointer to null
            // and call it good.  We have to call the SN valid so we don't get trapped
            // in the event handler.
            gc_DevData.ID.serialNumber          = NULL;
            gc_DevData.flags.initialized        = 1;
            gc_DevData.flags.serialNumberValid  = 1;

            // Tell the application layer that we have a device.
            USB_HOST_APP_EVENT_HANDLER(address, EVENT_GENERIC_ATTACH, &(gc_DevData.ID), sizeof(GENERIC_DEVICE_ID) );

            #ifdef DEBUG_MODE
                UART2PrintString( "GEN: Cannot get string descriptor!\r\n" );
            #endif
        }
    }
    #else
        // Generic Client Driver Init Complete.
        gc_DevData.flags.initialized = 1;

        // Notify that application that we've been attached to a device.
        USB_HOST_APP_EVENT_HANDLER(address, EVENT_GENERIC_ATTACH, &(gc_DevData.ID), sizeof(GENERIC_DEVICE_ID) );
    #endif

    // TBD

    return TRUE;

} // USBHostGenericInit


/****************************************************************************
  Function:
    BOOL USBHostGenericEventHandler ( BYTE address, USB_EVENT event,
                            void *data, DWORD size )

  Summary:
    This routine is called by the Host layer to notify the general client of
    events that occur.

  Description:
    This routine is called by the Host layer to notify the general client of
    events that occur.  If the event is recognized, it is handled and the
    routine returns TRUE.  Otherwise, it is ignored and the routine returns
    FALSE.

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

BOOL USBHostGenericEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    // Make sure it was for our device
    if ( address != gc_DevData.ID.deviceAddress)
    {
        return FALSE;
    }

    // Handle specific events.
    switch (event)
    {
    case EVENT_DETACH:
        // Notify that application that the device has been detached.
        USB_HOST_APP_EVENT_HANDLER(gc_DevData.ID.deviceAddress, EVENT_GENERIC_DETACH, &gc_DevData.ID.deviceAddress, sizeof(BYTE) );
        gc_DevData.flags.val        = 0;
        gc_DevData.ID.deviceAddress = 0;
        #ifdef DEBUG_MODE
            UART2PrintString( "USB Generic Client Device Detached: address=" );
            UART2PutDec( address );
            UART2PrintString( "\r\n" );
        #endif
        return TRUE;

    #ifdef USB_ENABLE_TRANSFER_EVENT
    case EVENT_TRANSFER:
        if ( (data != NULL) && (size == sizeof(HOST_TRANSFER_DATA)) )
        {
            DWORD dataCount = ((HOST_TRANSFER_DATA *)data)->dataCount;

            if ( ((HOST_TRANSFER_DATA *)data)->bEndpointAddress == (USB_IN_EP|USB_GENERIC_EP) )
            {
                gc_DevData.flags.rxBusy = 0;
                gc_DevData.rxLength = dataCount;
                USB_HOST_APP_EVENT_HANDLER(gc_DevData.ID.deviceAddress, EVENT_GENERIC_RX_DONE, &dataCount, sizeof(DWORD) );
            }
            else if ( ((HOST_TRANSFER_DATA *)data)->bEndpointAddress == (USB_OUT_EP|USB_GENERIC_EP) )
            {
                gc_DevData.flags.txBusy = 0;
                USB_HOST_APP_EVENT_HANDLER(gc_DevData.ID.deviceAddress, EVENT_GENERIC_TX_DONE, &dataCount, sizeof(DWORD) );
            }
            else
            #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
                if (((((HOST_TRANSFER_DATA *)data)->bEndpointAddress & 0x7F) == 0) && !gc_DevData.flags.serialNumberValid)
                {
                    #ifdef DEBUG_MODE
                        UART2PrintString( "GEN: Got serial number!\r\n" );
                    #endif
                    // Set the serial number information
                    gc_DevData.ID.serialNumberLength    = dataCount;
                    gc_DevData.flags.serialNumberValid  = 1;
                    gc_DevData.flags.initialized        = 1;

                    // Tell the application layer that we have a device.
                    USB_HOST_APP_EVENT_HANDLER(address, EVENT_GENERIC_ATTACH, &(gc_DevData.ID), sizeof(GENERIC_DEVICE_ID) );
                }
            else
            #endif
            {
                return FALSE;
            }

            return TRUE;

        }
        else
            return FALSE;
    #endif

    case EVENT_SUSPEND:
    case EVENT_RESUME:
    case EVENT_BUS_ERROR:
    default:
        break;
    }

    return FALSE;
} // USBHostGenericEventHandler


// *****************************************************************************
// *****************************************************************************
// Section: Application Callable Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL USBHostGenericDeviceDetached( BYTE deviceAddress )

  Description:
    This interface is used to check if the devich has been detached from the
    bus.

  Preconditions:
    None

  Parameters:
    BYTE deviceAddress	- USB Address of the device.

  Return Values:
    TRUE    - The device has been detached, or an invalid deviceAddress is given.
    FALSE   - The device is attached

  Example:
    <code>
    if (USBHostGenericDeviceDetached( deviceAddress ))
    {
        // Handle detach
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

 // Implemented as a macro. See usb_client_generic.h


/****************************************************************************
  Function:
    BOOL USBHostGenericGetDeviceAddress(GENERIC_DEVICE_ID *pDevID)

  Description:
    This interface is used get the address of a specific generic device on
    the USB.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    GENERIC_DEVICE_ID* pDevID  - Pointer to a structure containing the Device ID Info (VID,
                    		 PID, serial number, and device address).

  Return Values:
    TRUE    - The device is connected
    FALSE   - The device is not connected.

  Example:
    <code>
    GENERIC_DEVICE_ID   deviceID;
    WORD                serialNumber[] = { '1', '2', '3', '4', '5', '6' };
    BYTE                deviceAddress;

    deviceID.vid          = 0x1234;
    deviceID.pid          = 0x5678;
    deviceID.serialNumber = &serialNumber;

    if (USBHostGenericGetDeviceAddress(&deviceID))
    {
        deviceAddress = deviceID.deviceAddress;
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BOOL USBHostGenericGetDeviceAddress(GENERIC_DEVICE_ID *pDevID)
{
    if (!gc_DevData.flags.initialized) return FALSE;

    if (gc_DevData.ID.deviceAddress != 0 && pDevID != NULL)
    {
        if (pDevID->vid == gc_DevData.ID.vid && pDevID->pid == gc_DevData.ID.pid)
        {
#ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
            if (!strncmp( (char *)gc_DevData.ID.serialNumber, (char *)pDevID->serialNumber, gc_DevData.ID.serialNumberLength*2 ))
#endif
            {
                pDevID->deviceAddress = gc_DevData.ID.deviceAddress;
                return TRUE;
            }
        }
    }

    return FALSE;

} // USBHostGenericGetDeviceAddress


/****************************************************************************
  Function:
    DWORD USBHostGenericGetRxLength( BYTE deviceAddress )

  Description:
    This function retrieves the number of bytes copied to user's buffer by
    the most recent call to the USBHostGenericRead() function.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    BYTE deviceAddress	- USB Address of the device

  Returns:
    Returns the number of bytes most recently received from the Generic
    device with address deviceAddress.

  Remarks:
    This function can only be called once per transfer.  Subsequent calls will
    return zero until new data has been received.
  ***************************************************************************/

 // Implemented as a macro. See usb_client_generic.h


/****************************************************************************
  Function:
    void USBHostGenericRead( BYTE deviceAddress, BYTE *buffer, DWORD length )

  Description:
    Use this routine to receive from the device and store it into memory.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    BYTE deviceAddress  - USB Address of the device.
    BYTE *buffer        - Pointer to the data buffer
    DWORD length        - Number of bytes to be transferred

  Return Values:
    USB_SUCCESS         - The Read was started successfully
    (USB error code)    - The Read was not started.  See USBHostRead() for
                            a list of errors.

  Example:
    <code>
    if (!USBHostGenericRxIsBusy( deviceAddress ))
    {
        USBHostGenericRead( deviceAddress, &buffer, sizeof(buffer) );
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BYTE USBHostGenericRead( BYTE deviceAddress, void *buffer, DWORD length )
{
    BYTE RetVal;

    // Validate the call
    if (!API_VALID(deviceAddress)) return USB_INVALID_STATE;
    if (gc_DevData.flags.rxBusy)   return USB_BUSY;

    // Set the busy flag, clear the count and start a new IN transfer.
    gc_DevData.flags.rxBusy = 1;
    gc_DevData.rxLength = 0;
    RetVal = USBHostRead( deviceAddress, USB_IN_EP|USB_GENERIC_EP, (BYTE *)buffer, length );
    if (RetVal != USB_SUCCESS)
    {
        gc_DevData.flags.rxBusy = 0;    // Clear flag to allow re-try
    }

    return RetVal;

} // USBHostGenericRead

/****************************************************************************
  Function:
    BOOL USBHostGenericRxIsBusy( BYTE deviceAddress )

  Summary:
    This interface is used to check if the client driver is currently busy
    receiving data from the device.

  Description:
    This interface is used to check if the client driver is currently busy
    receiving data from the device.  This function is intended for use with
    transfer events.  With polling, the function USBHostGenericRxIsComplete()
    should be used.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    BYTE deviceAddress     - USB Address of the device

  Return Values:
    TRUE    - The device is receiving data or an invalid deviceAddress is
                given.
    FALSE   - The device is not receiving data

  Example:
    <code>
    if (!USBHostGenericRxIsBusy( deviceAddress ))
    {
        USBHostGenericRead( deviceAddress, &buffer, sizeof( buffer ) );
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

 // Implemented as a macro. See usb_client_generic.h


/****************************************************************************
  Function:
    BOOL USBHostGenericRxIsComplete( BYTE deviceAddress, BYTE *errorCode,
                DWORD *byteCount )

  Summary:
    This routine indicates whether or not the last IN transfer is complete.

  Description:
    This routine indicates whether or not the last IN transfer is complete.
    If it is, then the returned errorCode and byteCount are valid, and
    reflect the error code and the number of bytes received.

    This function is intended for use with polling.  With transfer events,
    the function USBHostGenericRxIsBusy() should be used.

  Preconditions:
    None

  Parameters:
    BYTE deviceAddress  - Address of the attached peripheral
    BYTE *errorCode     - Error code of the last transfer, if complete
    DWORD *byteCount    - Bytes transferred during the last transfer, if
                            complete

  Return Values:
    TRUE    - The IN transfer is complete.  errorCode and byteCount are valid.
    FALSE   - The IN transfer is not complete.  errorCode and byteCount are
                invalid.

  Remarks:
    None
  ***************************************************************************/

#ifndef USB_ENABLE_TRANSFER_EVENT
BOOL USBHostGenericRxIsComplete( BYTE deviceAddress,
                                    BYTE *errorCode, DWORD *byteCount )
{
    if (gc_DevData.flags.rxBusy)
    {
        return FALSE;
    }
    else
    {
        *byteCount = gc_DevData.rxLength;
        *errorCode = gc_DevData.rxErrorCode;
        return TRUE;
    }
}
#endif

/****************************************************************************
  Function:
    void USBHostGenericTasks( void )

  Summary:
    This routine is used if transfer events are not utilized. It monitors the
    host status and updates the transmit and receive flags.

  Description:
    This routine is used if transfer events are not utilized. It monitors the
    host status and updates the transmit and receive flags.  If serial
    numbers are supported, then this routine handles the reception of the
    serial number.

  Preconditions:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    This function is compiled only if USB_ENABLE_TRANSFER_EVENT is not
    defined.
  ***************************************************************************/

#ifndef USB_ENABLE_TRANSFER_EVENT
void USBHostGenericTasks( void )
{
    DWORD   byteCount;
    BYTE    errorCode;

    if (gc_DevData.ID.deviceAddress && gc_DevData.flags.initialized)
    {
        #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
            if (!gc_DevData.flags.serialNumberValid)
            {
                if (USBHostTransferIsComplete( gc_DevData.ID.deviceAddress, USB_IN_EP, &errorCode, &byteCount );
                {
                    #ifdef DEBUG_MODE
                        UART2PrintString( "GEN: Got serial number!\r\n" );
                    #endif
                    // Set the serial number information
                    gc_DevData.ID.serialNumberLength    = byteCount;
                    gc_DevData.flags.serialNumberValid  = 1;
                    gc_DevData.flags.initialized        = 1;

                    // Tell the application layer that we have a device.
                    USB_HOST_APP_EVENT_HANDLER(address, EVENT_GENERIC_ATTACH, &(gc_DevData.ID), sizeof(GENERIC_DEVICE_ID) );
                }
            }
        #endif

        if (gc_DevData.flags.rxBusy)
        {
            if (USBHostTransferIsComplete( gc_DevData.ID.deviceAddress, USB_IN_EP|USB_GENERIC_EP, &errorCode, &byteCount ))
            {
                gc_DevData.flags.rxBusy = 0;
                gc_DevData.rxLength     = byteCount;
                gc_DevData.rxErrorCode  = errorCode;
            }
        }

        if (gc_DevData.flags.txBusy)
        {
            if (USBHostTransferIsComplete( gc_DevData.ID.deviceAddress, USB_OUT_EP|USB_GENERIC_EP, &errorCode, &byteCount ))
            {
                gc_DevData.flags.txBusy = 0;
                gc_DevData.txErrorCode  = errorCode;
            }
        }
    }
}
#endif

/****************************************************************************
  Function:
    BOOL USBHostGenericTxIsBusy( BYTE deviceAddress )

  Summary:
    This interface is used to check if the client driver is currently busy
    transmitting data to the device.

  Description:
    This interface is used to check if the client driver is currently busy
    transmitting data to the device.  This function is intended for use with
    transfer events.  With polling, the function USBHostGenericTxIsComplete()
    should be used.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    BYTE deviceAddress	- USB Address of the device

  Return Values:
    TRUE    - The device is transmitting data or an invalid deviceAddress
                is given.
    FALSE   - The device is not transmitting data

  Example:
    <code>
    if (!USBHostGenericTxIsBusy( deviceAddress ) )
    {
        USBHostGenericWrite( deviceAddress, &buffer, sizeof( buffer ) );
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

 // Implemented as a macro. See usb_client_generic.h


/****************************************************************************
  Function:
    BOOL USBHostGenericTxIsComplete( BYTE deviceAddress, BYTE *errorCode )

  Summary:
    This routine indicates whether or not the last OUT transfer is complete.

  Description:
    This routine indicates whether or not the last OUT transfer is complete.
    If it is, then the returned errorCode is valid, and reflect the error
    code of the transfer.

    This function is intended for use with polling.  With transfer events,
    the function USBHostGenericTxIsBusy() should be used.

  Preconditions:
    None

  Parameters:
    BYTE deviceAddress  - Address of the attached peripheral
    BYTE *errorCode     - Error code of the last transfer, if complete

  Return Values:
    TRUE    - The OUT transfer is complete.  errorCode is valid.
    FALSE   - The OUT transfer is not complete.  errorCode is invalid.

  Remarks:
    None
  ***************************************************************************/

#ifndef USB_ENABLE_TRANSFER_EVENT
BOOL USBHostGenericTxIsComplete( BYTE deviceAddress, BYTE *errorCode )
{
    if (gc_DevData.flags.txBusy)
    {
        return FALSE;
    }
    else
    {
        *errorCode = gc_DevData.txErrorCode;
        return TRUE;
    }
}
#endif


/****************************************************************************
  Function:
    void USBHostGenericWrite( BYTE deviceAddress, BYTE *buffer, DWORD length )

  Description:
    Use this routine to transmit data from memory to the device.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    BYTE deviceAddress   - USB Address of the device.
    BYTE *buffer         - Pointer to the data buffer
    DWORD length         - Number of bytes to be transferred

  Return Values:
    USB_SUCCESS         - The Write was started successfully
    (USB error code)    - The Write was not started.  See USBHostWrite() for
                            a list of errors.

  Example:
    <code>
    if (!USBHostGenericTxIsBusy( deviceAddress ))
    {
        USBHostGenericWrite( deviceAddress, &buffer, sizeof(buffer) );
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BYTE USBHostGenericWrite( BYTE deviceAddress, void *buffer, DWORD length )
{
    BYTE RetVal;

    // Validate the call
    if (!API_VALID(deviceAddress)) return USB_INVALID_STATE;
    if (gc_DevData.flags.txBusy)   return USB_BUSY;

    // Set the busy flag and start a new OUT transfer.
    gc_DevData.flags.txBusy = 1;
    RetVal = USBHostWrite( deviceAddress, USB_OUT_EP|USB_GENERIC_EP, (BYTE *)buffer, length );
    if (RetVal != USB_SUCCESS)
    {
        gc_DevData.flags.txBusy = 0;    // Clear flag to allow re-try
    }

    return RetVal;

} // USBHostGenericWrite


/*************************************************************************
 * EOF usb_client_generic.c
 */


