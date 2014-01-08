/******************************************************************************
 
  USB Host MIDI Client Driver

Description:
    This is the MIDI client driver file for a USB Embedded Host device.  This
    driver should be used in a project with usb_host.c to provided the USB
    hardware interface.

    To interface with USB Embedded Host layer, the routine USBHostMIDIInit()
    should be specified as the Initialize() function, and
    USBHostMICIEventHandler() should be specified as the EventHandler()
    function in the usbClientDrvTable[] array declared in usb_config.c.

    This driver can be configured to either use transfer events from usb_host.c
    or use a polling mechanism.  If USB_ENABLE_TRANSFER_EVENT is defined, this
    driver will utilize transfer events.  Otherwise, this driver will utilize
    polling.

Since the MIDI class is performed with bulk transfers,
USB_SUPPORT_BULK_TRANSFERS must be defined.

This driver has been tested with the following USB keyboards:
Akai Professional Synth Station 25
Korg Nano Key
eKeys 37
Ion Discover Keyboard USB
M-Audio Oxygen 25
Prodipe MIDI USB Keyboard Controller 25C
Alesis Q49

Summary:
    This is the MIDI client driver file for a USB Embedded Host device.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* FileName:        usb_host_midi.c
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
TL       17-Oct-2011    Preliminary release

*******************************************************************************/
//DOM-IGNORE-END

#include <stdlib.h>
#include <string.h>
#include "GenericTypeDefs.h"
#include "USB/usb.h"
#include "USB/usb_host_midi.h"

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
#ifndef USB_MAX_MIDI_DEVICES
    #define USB_MAX_MIDI_DEVICES     1
#endif

#if USB_MAX_MIDI_DEVICES != 1
    #error The MIDI client driver supports only one attached device.
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************
     
static MIDI_DEVICE devices[USB_MAX_MIDI_DEVICES];

// *****************************************************************************
// *****************************************************************************
// Section: Host Stack Interface Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL USBHostMIDIInit ( BYTE address, DWORD flags, BYTE clientDriverID )

  Summary:
    This function is called by the USB Embedded Host layer when a MIDI
    device attaches.

  Description:
    This routine is a call out from the USB Embedded Host layer to the USB
    MIDI client driver.  It is called when a MIDI device has been connected
    to the host.  Its purpose is to initialize and activate the USB
    MIDI client driver.

  Preconditions:
    The device has been configured.

  Parameters:
    BYTE address        - Device's address on the bus
    DWORD flags         - Initialization flags
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

BOOL USBHostMIDIInit ( BYTE address, DWORD flags, BYTE clientDriverID )
{
    BYTE *config_descriptor;
    BYTE *ptr;
    BYTE bDescriptorType;
    BYTE bLength;
    BYTE bNumEndpoints;
    BYTE bNumInterfaces;
    BYTE bInterfaceNumber;
    BYTE bAlternateSetting;
    BYTE Class;
    BYTE SubClass;
    BYTE Protocol;
    BYTE currentEndpoint;
    WORD wTotalLength;
    
    BYTE index = 0;
    BOOL error = FALSE;
    
    MIDI_DEVICE *device = &devices[0];
    
    config_descriptor = USBHostGetCurrentConfigurationDescriptor(address);
    ptr = config_descriptor;
    
    // Load up the values from the Configuration Descriptor
    bLength              = *ptr++;
    bDescriptorType      = *ptr++;
    wTotalLength         = *ptr++;           // In case these are not word aligned
    wTotalLength        += (*ptr++) << 8;
    bNumInterfaces       = *ptr++;
    
    // Skip over the rest of the Configuration Descriptor
    index += bLength;
    ptr    = &config_descriptor[index];

    while (!error && (index < wTotalLength))
    {
        // Check the descriptor length and type
        bLength         = *ptr++;
        bDescriptorType = *ptr++;

        // Find an interface descriptor
        if (bDescriptorType != USB_DESCRIPTOR_INTERFACE)
        {
            // Skip over the rest of the Descriptor
            index += bLength;
            ptr = &config_descriptor[index];
        }
        else
        {
            // Read some data from the interface descriptor
            bInterfaceNumber  = *ptr++;
            bAlternateSetting = *ptr++;
            bNumEndpoints     = *ptr++;
            Class             = *ptr++;
            SubClass          = *ptr++;
            Protocol          = *ptr++;

            // Check to see if this is a MIDI inteface descripter
            if (Class != AUDIO_CLASS || SubClass != MIDI_SUB_CLASS || Protocol != MIDI_PROTOCOL)
            {
                // If we cannot support this interface, skip it.
                index += bLength;
                ptr = &config_descriptor[index];
                continue;
            }

            // Initialize the device
            device->deviceAddress = address;
            device->clientDriverID = clientDriverID;
            device->numEndpoints = bNumEndpoints;
            
            
            // Allocate enough memory for each endpoint
            if ((device->endpoints = (MIDI_ENDPOINT_DATA*)malloc( sizeof(MIDI_ENDPOINT_DATA) * bNumEndpoints)) == NULL)
            {
                // Out of memory
                error = TRUE;   
            }
             
            if (!error)   
            {
                // Skip over the rest of the Interface Descriptor
                index += bLength;
                ptr = &config_descriptor[index];

                // Find the Endpoint Descriptors.  There might be Class and Vendor descriptors in here
                currentEndpoint = 0;
                while (!error && (index < wTotalLength) && (currentEndpoint < bNumEndpoints))
                {
                    bLength = *ptr++;
                    bDescriptorType = *ptr++;

                    if (bDescriptorType != USB_DESCRIPTOR_ENDPOINT)
                    {
                        // Skip over the rest of the Descriptor
                        index += bLength;
                        ptr = &config_descriptor[index];
                    }
                    else
                    {
                        device->endpoints[currentEndpoint].endpointAddress = *ptr++;
                        ptr++;
                        device->endpoints[currentEndpoint].endpointSize = *ptr++;
                        device->endpoints[currentEndpoint].endpointSize += (*ptr++) << 8;
                        device->endpoints[currentEndpoint].busy = FALSE;
                        
                        if(device->endpoints[currentEndpoint].endpointSize > 64)
                        {
                            // For full speed bulk endpoints, only 8, 16, 32, and 64 byte packets are supported
                            // But we will accept anything less than or equal to 64.
                            error = TRUE;
                        }
                        
                        // Get ready for the next endpoint.
                        currentEndpoint++;
                        index += bLength;
                        ptr = &config_descriptor[index];
                    }
                }
            }    

            // Ensure that we found all the endpoints for this interface.
            if (currentEndpoint != bNumEndpoints)
            {
                error = TRUE;
            }
        }
    }

    if (error)
    {
        // Destroy whatever list of interfaces, settings, and endpoints we created.
        // The "new" variables point to the current node we are trying to remove.
        if (device->endpoints != NULL)
        {           
            free( device->endpoints );
            device->endpoints = NULL;
        }    
        return FALSE;
    }
    
    #ifdef DEBUG_MODE
        UART2PrintString( "USB MIDI Client Initalized: " );
        UART2PrintString( " address=" );
        UART2PutDec( address );
        UART2PrintString( " Number of Endpoings=" );
        UART2PutHex( bNumEndpoints );
        UART2PrintString( "\r\n" );
    #endif

    // Notify that application that we've been attached to a device.
    USB_HOST_APP_EVENT_HANDLER(address, EVENT_MIDI_ATTACH, device, sizeof(MIDI_DEVICE) );

    return TRUE;

} // USBHostMIDIInit


/****************************************************************************
  Function:
    BOOL USBHostMIDIEventHandler ( BYTE address, USB_EVENT event,
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

BOOL USBHostMIDIEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    unsigned char i;
    
    // Make sure it was for one of our devices
    for( i = 0; i < USB_MAX_MIDI_DEVICES; i++)
    {
        if ( address == devices[i].deviceAddress)
        {
            break;
        }
    
    }
    if(i == USB_MAX_MIDI_DEVICES)
    {
        return FALSE;
    }    
    
    // Handle specific events
    switch (event)
    {
        case EVENT_DETACH:
            // Notify that application that the device has been detached.
            USB_HOST_APP_EVENT_HANDLER(devices[i].deviceAddress, EVENT_MIDI_DETACH, &devices[i], sizeof(MIDI_DEVICE) );
            devices[i].deviceAddress = 0;
            free(devices[i].endpoints);
            devices[i].endpoints = NULL;
            #ifdef DEBUG_MODE
                UART2PrintString( "USB MIDI Client Device Detached: address=" );
                UART2PutDec( address );
                UART2PrintString( "\r\n" );
            #endif
            return TRUE;
    
        #ifdef USB_ENABLE_TRANSFER_EVENT
        case EVENT_TRANSFER:
            if ( (data != NULL) && (size == sizeof(HOST_TRANSFER_DATA)) )
            {
                unsigned char currentEndpoint;
                //DWORD dataCount = ((HOST_TRANSFER_DATA *)data)->dataCount;
    
                for(currentEndpoint = 0; currentEndpoint < devices[i].numEndpoints; currentEndpoint++)
                {
                    if ( ((HOST_TRANSFER_DATA *)data)->bEndpointAddress == devices[i].endpoints[currentEndpoint].endpointAddress )
                    {
                        devices[i].endpoints[currentEndpoint].busy = 0;
                        USB_HOST_APP_EVENT_HANDLER(devices[i].deviceAddress, EVENT_MIDI_TRANSFER_DONE, &devices[i].endpoints[currentEndpoint], sizeof(MIDI_ENDPOINT_DATA));
                        return TRUE;
                    }
                }    
            }
            return FALSE;
        #endif
    
        case EVENT_SUSPEND:
        case EVENT_RESUME:
        case EVENT_BUS_ERROR:
        default:
            break;
    }

    return FALSE;
} // USBHostMIDIEventHandler


// *****************************************************************************
// *****************************************************************************
// Section: Application Callable Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL USBHostMIDIDeviceDetached( void* handle )

  Description:
    This interface is used to check if the device has been detached from the
    bus.

  Preconditions:
    None

  Parameters:
    void* handle - Pointer to a structure containing the Device Info

  Return Values:
    TRUE    - The device has been detached, or an invalid handle is given.
    FALSE   - The device is attached

  Example:
    <code>
    if (USBHostMIDIDeviceDetached( deviceAddress ))
    {
        // Handle detach
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

 // Implemented as a macro. See usb_host_midi.h


/****************************************************************************
  Function:
    MIDI_ENDPOINT_DIRECTION USBHostMIDIEndpointDirection( void* handle, BYTE endpointIndex )

  Description:
    This function retrieves the endpoint direction of the endpoint at
    endpointIndex for device that's located at handle.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    void* handle       - Pointer to a structure containing the Device Info
    BYTE endpointIndex - the index of the endpoint whose direction is requested

  Returns:
    MIDI_ENDPOINT_DIRECTION - Returns the direction of the endpoint (IN or OUT)

  Remarks:
    None
  ***************************************************************************/

 // Implemented as a macro. See usb_host_midi.h


/****************************************************************************
  Function:
    DWORD USBHostMIDISizeOfEndpoint( void* handle, BYTE endpointIndex )

  Description:
    This function retrieves the endpoint size of the endpoint at 
    endpointIndex for device that's located at handle.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    void* handle       - Pointer to a structure containing the Device Info
    BYTE endpointIndex - the index of the endpoint whose direction is requested

  Returns:
    DWORD - Returns the number of bytes for the endpoint (4 - 64 bytes per USB spec)

  Remarks:
    None
  ***************************************************************************/

// Implemented as a macro. See usb_host_midi.h


/****************************************************************************
  Function:
    BYTE USBHostMIDINumberOfEndpoints( void* handle )

  Description:
    This function retrieves the number of endpoints for the device that's
    located at handle.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    void* handle - Pointer to a structure containing the Device Info

  Returns:
    BYTE - Returns the number of endpoints for the device at handle.

  Remarks:
    None
  ***************************************************************************/
  
  // Implemented as a macro. See usb_host_midi.h


/****************************************************************************
  Function:
    BYTE USBHostMIDIRead( void* handle, BYTE endpointIndex, void *buffer, WORD length)

  Description:
    This function will attempt to read length number of bytes from the attached MIDI
    device located at handle, and will save the contents to ram located at buffer.

  Preconditions:
    The device must be connected and enumerated. The array at *buffer should have
    at least length number of bytes available.

  Parameters:
    void* handle       - Pointer to a structure containing the Device Info
    BYTE endpointIndex - the index of the endpoint whose direction is requested
    void* buffer       - Pointer to the data buffer
    WORD length        - Number of bytes to be read

  Return Values:
    USB_SUCCESS         - The Read was started successfully
    (USB error code)    - The Read was not started.  See USBHostRead() for
                            a list of errors.

  Example:
    <code>
    if (!USBHostMIDITransferIsBusy( deviceHandle, currentEndpoint )
    {
        USBHostMIDIRead( deviceHandle, currentEndpoint, &buffer, sizeof(buffer));
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BYTE USBHostMIDIRead( void* handle, BYTE endpointIndex, void *buffer, WORD length)
{
    MIDI_DEVICE *device = (MIDI_DEVICE*)handle;
    BYTE RetVal;
    
    RetVal = USBHostRead( device->deviceAddress, device->endpoints[endpointIndex].endpointAddress, (BYTE *)buffer, length );
    
    if (RetVal == USB_SUCCESS)
    {
        // Set the busy flag
        device->endpoints[endpointIndex].busy = TRUE;
    }

    return RetVal;

} // USBHostMIDIRead

/****************************************************************************
  Function:
    BOOL USBHostMIDITransferIsBusy( void* handle, BYTE endpointIndex )

  Summary:
    This interface is used to check if the client driver is currently busy
    transferring data over endponitIndex for the device at handle.

  Description:
    This interface is used to check if the client driver is currently busy
    receiving or sending data from the device at the endpoint with number
    endpointIndex.  This function is intended for use with transfer events.
    With polling, the function USBHostMIDITransferIsComplete()
    should be used.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    void* handle       - Pointer to a structure containing the Device Info
    BYTE endpointIndex - the index of the endpoint whose direction is requested

  Return Values:
    TRUE    - The device is receiving data or an invalid handle is
                given.
    FALSE   - The device is not receiving data

  Example:
    <code>
    if (!USBHostMIDITransferIsBusy( handle, endpointIndex ))
    {
        USBHostMIDIRead( handle, endpointIndex, &buffer, sizeof( buffer ) );
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

 // Implemented as a macro. See usb_host_midi.h


/****************************************************************************
  Function:
    BOOL USBHostMIDITransferIsComplete( void* handle, BYTE endpointIndex,
                                        BYTE *errorCode, DWORD *byteCount );

  Summary:
    This routine indicates whether or not the last transfer over endpointIndex
    is complete.

  Description:
    This routine indicates whether or not the last transfer over endpointIndex
    is complete. If it is, then the returned errorCode and byteCount are valid,
    and reflect the error code and the number of bytes received.

    This function is intended for use with polling.  With transfer events,
    the function USBHostMIDITransferIsBusy() should be used.

  Preconditions:
    None

  Parameters:
    void* handle        - Pointer to a structure containing the Device Info
    BYTE endpointIndex  - index of endpoint in endpoints array
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
BOOL USBHostMIDITransferIsComplete(void* handle, BYTE endpointIndex, BYTE* errorCode, DWORD *byteCount )
{
    MIDI_DEVICE* device = (MIDI_DEVICE*)handle;
    
    if (USBHostTransferIsComplete(device->deviceAddress, endpointIndex, errorCode, byteCount) == TRUE)
    {
        device->endpoints[endpointIndex].busy = 0;
        return TRUE;
    }
    
    // Then this transfer is not complete
    return FALSE;    
}
#endif


/****************************************************************************
  Function:
    BYTE USBHostMIDIWrite(void* handle, BYTE endpointIndex, void *buffer, WORD length)

  Description:
    This function will attempt to write length number of bytes from memory at location
    buffer to the attached MIDI device located at handle.

  Preconditions:
    The device must be connected and enumerated. The array at *buffer should have
    at least length number of bytes available.

  Parameters:
    handle          - Pointer to a structure containing the Device Info
    endpointIndex   - Index of the endpoint
    buffer          - Pointer to the data being transferred
    length          - Size of the data being transferred

  Return Values:
    USB_SUCCESS         - The Write was started successfully
    (USB error code)    - The Write was not started.  See USBHostWrite() for
                            a list of errors.

  Example:
    <code>
    if (!USBHostMIDITransferIsBusy( deviceHandle, currentEndpoint )
    {
        USBHostMIDIWrite( deviceAddress, &buffer, sizeof(buffer) );
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BYTE USBHostMIDIWrite(void* handle, BYTE endpointIndex, void *buffer, WORD length)
{
    MIDI_DEVICE *device = (MIDI_DEVICE*)handle;
    BYTE RetVal;
    
    RetVal = USBHostWrite( device->deviceAddress, device->endpoints[endpointIndex].endpointAddress, (BYTE *)buffer, length );
    if (RetVal == USB_SUCCESS)
    {
        // Set the busy flag
        device->endpoints[endpointIndex].busy = TRUE;
    }

    return RetVal;

} // USBHostMIDIWrite


/*************************************************************************
 * EOF usb_client_midi.c
 */
