/******************************************************************************

  USB Host Charger Client Driver

Description:
    This is the Charger client driver file for a USB Embedded Host device.  This
    driver should be used in a project with usb_host.c to provided the USB
    hardware interface.

    To interface with USB Embedded Host layer, the routine USBHostChargerInit()
    should be specified as the Initialize() function, and
    USBHostChargerEventHandler() should be specified as the EventHandler()
    function in the usbClientDrvTable[] array declared in usb_config.c.

    This driver can be used in either the event driven or polling mechanism.

Summary:
    This is the Charger client driver file for a USB Embedded Host device.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

FileName:        usb_host_charger.c
Dependencies:    None
Processor:       PIC24/PIC32MX
Compiler:        C30, C32
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
  Rev        Description
  -----      -----------
  2.6-2.7a   No change

*******************************************************************************/
//DOM-IGNORE-END

#include <stdlib.h>
#include <string.h>
#include "GenericTypeDefs.h"
#include "USB/usb.h"
#include "USB/usb_host_charger.h"

//#define DEBUG_MODE
#ifdef DEBUG_MODE
    #include "uart2.h"
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

BYTE                currentChargingRecord;
USB_CHARGING_DEVICE usbChargingDevices[USB_MAX_CHARGING_DEVICES];


// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************

static BOOL _USBHostCharger_FindDevice( BYTE address );


// *****************************************************************************
// *****************************************************************************
// Section: Host Stack Interface Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL USBHostChargerInitialize ( BYTE address, DWORD flags, BYTE clientDriverID )

  Summary:
    This function is called by the USB Embedded Host layer when a device
    attaches for charging.

  Description:
    This routine is a call out from the USB Embedded Host layer to the USB
    charger client driver.  It is called when a device that is supported for
    charging only has been connected to the host.  Its purpose is to initialize
    and activate the USB Charger client driver.

  Preconditions:
    The device has been configured.

  Parameters:
    BYTE address    - Device's address on the bus
    DWORD flags     - Initialization flags
    BYTE clientDriverID - ID to send when issuing a Device Request via
                            USBHostIssueDeviceRequest() or USBHostSetDeviceConfiguration().

  Return Values:
    TRUE    - Initialization was successful
    FALSE   - Initialization failed

  Remarks:
    Multiple client drivers may be used in a single application.  The USB
    Embedded Host layer will call the initialize routine required for the
    attached device.
  ***************************************************************************/

BOOL USBHostChargerInitialize( BYTE address, DWORD flags, BYTE clientDriverID )
{
    BYTE    *pDesc;

    // Find a new entry
    for (currentChargingRecord=0; currentChargingRecord<USB_MAX_CHARGING_DEVICES; currentChargingRecord++)
    {
        if (!usbChargingDevices[currentChargingRecord].flags.inUse) break;
    }
    if (currentChargingRecord == USB_MAX_CHARGING_DEVICES)
    {
        #ifdef DEBUG_MODE
            UART2PrintString( "CHG: No more space\r\n" );
        #endif
        return FALSE;   // We have no more room for a new device.
    }

    // Initialize state - set the inUse flag.
    usbChargingDevices[currentChargingRecord].flags.val = 1;

    // Save device the address, VID, & PID
    usbChargingDevices[currentChargingRecord].ID.deviceAddress = address;
    usbChargingDevices[currentChargingRecord].ID.clientDriverID = clientDriverID;
    pDesc  = USBHostGetDeviceDescriptor(address);
    pDesc += 8;
    usbChargingDevices[currentChargingRecord].ID.vid  =  (WORD)*pDesc;       pDesc++;
    usbChargingDevices[currentChargingRecord].ID.vid |= ((WORD)*pDesc) << 8; pDesc++;
    usbChargingDevices[currentChargingRecord].ID.pid  =  (WORD)*pDesc;       pDesc++;
    usbChargingDevices[currentChargingRecord].ID.pid |= ((WORD)*pDesc) << 8; pDesc++;

    #ifdef DEBUG_MODE
        UART2PrintString( "CHG: USB Charging Client Initalized: flags=0x" );
        UART2PutHex(      flags );
        UART2PrintString( " address=" );
        UART2PutDec( address );
        UART2PrintString( " VID=0x" );
        UART2PutHex(      usbChargingDevices[currentChargingRecord].ID.vid >> 8   );
        UART2PutHex(      usbChargingDevices[currentChargingRecord].ID.vid & 0xFF );
        UART2PrintString( " PID=0x"      );
        UART2PutHex(      usbChargingDevices[currentChargingRecord].ID.pid >> 8   );
        UART2PutHex(      usbChargingDevices[currentChargingRecord].ID.pid & 0xFF );
        UART2PrintString( "\r\n"         );
    #endif

    // Notify that application that we've been attached to a device.
    USB_HOST_APP_EVENT_HANDLER(address, EVENT_CHARGER_ATTACH,
            &(usbChargingDevices[currentChargingRecord].ID), sizeof(USB_CHARGING_DEVICE_ID) );

    return TRUE;

} // USBHostChargerInit


/****************************************************************************
  Function:
    BOOL USBHostChargerEventHandler ( BYTE address, USB_EVENT event,
                            void *data, DWORD size )

  Summary:
    This routine is called by the Host layer to notify the charger client of
    events that occur.

  Description:
    This routine is called by the Host layer to notify the charger client of
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

BOOL USBHostChargerEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    // Make sure the event is for a connected device
    if (!_USBHostCharger_FindDevice( address ))
    {
        return FALSE;
    }

    // Handle specific events.
    switch (event)
    {
        case EVENT_DETACH:
            // Notify that application that the device has been detached.
            USB_HOST_APP_EVENT_HANDLER(usbChargingDevices[currentChargingRecord].ID.deviceAddress, EVENT_CHARGER_DETACH, &usbChargingDevices[currentChargingRecord].ID.deviceAddress, sizeof(BYTE) );
            usbChargingDevices[currentChargingRecord].flags.val        = 0;
            usbChargingDevices[currentChargingRecord].ID.deviceAddress = 0;
            #ifdef DEBUG_MODE
                UART2PrintString( "USB Charging Client Device Detached: address=" );
                UART2PutDec( address );
                UART2PrintString( "\r\n" );
            #endif
            return TRUE;
            break;

        case EVENT_SUSPEND:
        case EVENT_RESUME:
        case EVENT_BUS_ERROR:
        default:
            break;
    }

    return FALSE;
} // USBHostChargerEventHandler


// *****************************************************************************
// *****************************************************************************
// Section: Application Callable Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    BOOL USBHostChargerDeviceDetached( BYTE deviceAddress )

  Description:
    This interface is used to check if the device has been detached from the
    bus.

  Preconditions:
    None

  Parameters:
    deviceAddress     - USB Address of the device.

  Return Values:
    TRUE    - The device has been detached, or an invalid deviceAddress is given.
    FALSE   - The device is attached

  Example:
    <code>
    if (USBHostChargerDeviceDetached( deviceAddress ))
    {
        // Handle detach
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BOOL USBHostChargerDeviceDetached( BYTE deviceAddress )
{
    return !_USBHostCharger_FindDevice( deviceAddress );
}


/****************************************************************************
  Function:
    BOOL USBHostChargerGetDeviceAddress(USB_CHARGING_DEVICE_ID *pDevID)

  Description:
    This interface is used get the address of a specific generic device on
    the USB.

  Preconditions:
    The device must be connected and enumerated.

  Parameters:
    pDevID  - Pointer to a structure containing the Device ID Info (VID,
                    PID, and device address).

  Return Values:
    TRUE    - The device is connected
    FALSE   - The device is not connected.

  Example:
    <code>
    USB_CHARGING_DEVICE_ID  deviceID;
    BYTE                    deviceAddress;

    deviceID.vid          = 0x1234;
    deviceID.pid          = 0x5678;

    if (USBHostChargerGetDeviceAddress(&deviceID))
    {
        deviceAddress = deviceID.deviceAddress;
    }
    </code>

  Remarks:
    None
  ***************************************************************************/

BOOL USBHostChargerGetDeviceAddress(USB_CHARGING_DEVICE_ID *pDevID)
{
    if (pDevID == NULL)
    {
        return FALSE;
    }

    for (currentChargingRecord=0; currentChargingRecord<USB_MAX_CHARGING_DEVICES; currentChargingRecord++)
    {
        if (usbChargingDevices[currentChargingRecord].flags.inUse &&
            (usbChargingDevices[currentChargingRecord].ID.vid == pDevID->vid) && 
            (usbChargingDevices[currentChargingRecord].ID.pid == pDevID->pid))
        {
            pDevID->deviceAddress = usbChargingDevices[currentChargingRecord].ID.deviceAddress;
            return TRUE;
        }
    }

    return FALSE;   // The device was not found.

} // USBHostChargingGetDeviceAddress


// *****************************************************************************
// *****************************************************************************
// Section: Internal Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    static BOOL _USBHostCharger_FindDevice( BYTE address )

  Description:
    This function looks in the array of charging devices to see if there is
    currently a device attached at the indicated device.  If so, it returns
    TRUE, and the global variable currentChargingRecord is changed to the
    index of the device.  If not, it returns FALSE.

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

BOOL _USBHostCharger_FindDevice( BYTE address )
{
    for (currentChargingRecord=0; currentChargingRecord<USB_MAX_CHARGING_DEVICES; currentChargingRecord++)
    {
        if (usbChargingDevices[currentChargingRecord].flags.inUse &&
            (usbChargingDevices[currentChargingRecord].ID.deviceAddress == address))
        {
            return TRUE;
        }
    }
    return FALSE;   // The device was not found.
}


/*************************************************************************
 * EOF
 */


