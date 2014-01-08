/******************************************************************************

  USB Host Human Interface Device Driver

This is the Human Interface Device Class driver file for a USB Embedded Host
device. This file should be used in a project with usb_host.c to provided the
USB hardware interface.

Acronyms/abbreviations used by this class:
    * HID - Human Interface Device

To interface with usb_host.c, the routine USBHostHIDInitialize() should be
specified as the Initialize() function, and USBHostHIDEventHandler() should
be specified as the EventHandler() function in the usbClientDrvTable[] array
declared in usb_config.h.

This driver can be configured to use transfer events from usb_host.c.  Transfer
events require more RAM and ROM than polling, but it cuts down or even
eliminates the required polling of the various USBxxxTasks functions.  For this
class, USBHostHIDTasks() is compiled out if transfer events from usb_host.c
are used.  However, USBHostTasks() still must be called to provide attach,
enumeration, and detach services.  If transfer events from usb_host.c
are going to be used, USB_ENABLE_TRANSFER_EVENT should be defined.  If transfer
status is going to be polled, USB_ENABLE_TRANSFER_EVENT should not be defined.

This driver can also be configured to provide HID transfer events to
the next layer. Generating these events requires a small amount of extra ROM,
but no extra RAM.  The layer above this driver must be configured to receive
and respond to the events.  If HID transfer events are going to be
sent to the next layer, USB_HID_ENABLE_TRANSFER_EVENT should be defined. If
HID transfer status is going to be polled, USB_HID_ENABLE_TRANSFER_EVENT 
should not be defined. In any case transfer event EVENT_HID_RPT_DESC_PARSED
will be sent to interface layer. Application must provide a function
to collect the report descriptor information. Report descriptor information will
be overwritten with new report descriptor(in case multiple interface are present)
information when cotrol returns to HID driver . This is done to avoid using 
extra RAM.

Since HID transfers are performed with interrupt taransfers, 
USB_SUPPORT_INTERRUPT_TRANSFERS must be defined.

FileName:        usb_host_hid.c
Dependencies:    None
Processor:       PIC24/dsPIC30/dsPIC33/PIC32MX
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

Author          Date    Comments
--------------------------------------------------------------------------------
ADG          9-Apr-2008 First release
*******************************************************************************/

/********************************************************************
 File Description:

 Change History:
  Rev       Description
  ----      -----------
  2.2       Initial Release
  2.3-2.5a  No changes
  2.5b      Corrected heap allocation issue
  2.6-2.6a  No changes
  2.7       Fixed issue where deviceInfoHID[i].rptDescriptor was freed
            twice.  The second free results in possible issues in future
            malloc() calls in the C32 compiler.
            Fixed Issue with heap not freeing up for the variable 
            allocated in other file. 'parsedDataMem' needs to be freed in
            the file 'usb_host_hid_parser.c'.
********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_hid.h"
#include "USB/usb_host_hid_parser.h"

//#define DEBUG_MODE
#ifdef DEBUG_MODE
    #include "uart2.h"
#endif


// *****************************************************************************
// *****************************************************************************
// Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Max Number of Supported Devices

This value represents the maximum number of attached devices this class driver
can support.  If the user does not define a value, it will be set to 1.
Currently this must be set to 1, due to limitations in the USB Host layer.
*/
#ifndef USB_MAX_HID_DEVICES
    #define USB_MAX_HID_DEVICES        1
#endif

// *****************************************************************************
// *****************************************************************************
// Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// State Machine Constants
// *****************************************************************************

#ifndef USB_ENABLE_TRANSFER_EVENT

#define STATE_MASK                          0x00F0 //
#define SUBSTATE_MASK                       0x000F //

#define NEXT_STATE                          0x0010 //
#define NEXT_SUBSTATE                       0x0001 //


#define STATE_DETACHED                      0x0000 //

#define STATE_INITIALIZE_DEVICE             0x0010 //
#define SUBSTATE_WAIT_FOR_ENUMERATION       0x0000 //
#define SUBSTATE_DEVICE_ENUMERATED          0x0001 //

/* Get Report Descriptor & parse */                //
#define STATE_GET_REPORT_DSC                0x0020 //
#define SUBSTATE_SEND_GET_REPORT_DSC        0x0000 //
#define SUBSTATE_WAIT_FOR_REPORT_DSC        0x0001 //
#define SUBSTATE_GET_REPORT_DSC_COMPLETE    0x0002 //
#define SUBSTATE_PARSE_REPORT_DSC           0x0003 //
#define SUBSTATE_PARSING_COMPLETE           0x0004 //

#define STATE_RUNNING                       0x0030 //
#define SUBSTATE_WAITING_FOR_REQ            0x0000 //
#define SUBSTATE_SEND_READ_REQ              0x0001 //
#define SUBSTATE_READ_REQ_WAIT              0x0002 //
#define SUBSTATE_READ_REQ_DONE              0x0003 //
#define SUBSTATE_SEND_WRITE_REQ             0x0004 //
#define SUBSTATE_WRITE_REQ_WAIT             0x0005 //
#define SUBSTATE_WRITE_REQ_DONE             0x0006 //

#define STATE_HID_RESET_RECOVERY            0x0040 //
#define SUBSTATE_SEND_RESET                 0x0000 //
#define SUBSTATE_WAIT_FOR_RESET             0x0001 //

#define STATE_HOLDING                       0x0060 //


#else


#define STATE_DETACHED                      0x0000 //

#define STATE_INITIALIZE_DEVICE             0x0001 //
#define STATE_WAIT_FOR_REPORT_DSC           0x0002 //
#define STATE_PARSING_COMPLETE              0x0003 //

#define STATE_RUNNING                       0x0004 //
#define STATE_READ_REQ_WAIT                 0x0005 //
#define STATE_WRITE_REQ_WAIT                0x0006 //

#define STATE_WAIT_FOR_RESET                0x0007 //
#define STATE_RESET_COMPLETE                0x0008 //

#define STATE_HOLDING                       0x0009 //

#endif

// *****************************************************************************
// Other Constants
// *****************************************************************************

#define USB_HID_RESET           (0xFF)  // Device Request code to reset the device.
#define MARK_RESET_RECOVERY     (0x0E)  // Maintain with USB_MSD_DEVICE_INFO

/* Class-Specific Requests */
#define USB_HID_GET_REPORT      (0x01)  //
#define USB_HID_GET_IDLE        (0x02)  //
#define USB_HID_GET_PROTOCOL    (0x03)  //
#define USB_HID_SET_REPORT      (0x09)  //
#define USB_HID_SET_IDLE        (0x0A)  //
#define USB_HID_SET_PROTOCOL    (0x0B)  //

#define USB_HID_INPUT_REPORT    (0x01)  //
#define USB_HID_OUTPUT_REPORT   (0x02)  //
#define USB_HID_FEATURE_REPORT  (0x03)  //


//******************************************************************************
//******************************************************************************
// Data Structures
//******************************************************************************
//******************************************************************************
/*  USB HID Device Information
   This structure is used to hold information of all the interfaces in a device that unique
*/
typedef struct _USB_HID_INTERFACE_DETAILS
{
    struct _USB_HID_INTERFACE_DETAILS   *next;                // Pointer to next interface in the list.
    WORD                                sizeOfRptDescriptor;  // Size of report descriptor of a particular interface.
    WORD                                endpointMaxDataSize;  // Max data size for a interface.
    BYTE                                endpointIN;           // HID IN endpoint for corresponding interface.
    BYTE                                endpointOUT;          // HID OUT endpoint for corresponding interface.
    BYTE                                interfaceNumber;      // Interface number.
    BYTE                                endpointPollInterval; // Polling rate of corresponding interface.
}   USB_HID_INTERFACE_DETAILS;

/*
   This structure is used to hold information about device common to all the interfaces
*/
typedef struct _USB_HID_DEVICE_INFO
{
    USB_HID_DEVICE_ID                   ID;                    // Information about the device.
    WORD                                reportId;              // Report ID of the current transfer.
    BYTE*                               userData;              // Data pointer to application buffer.
    BYTE*                               rptDescriptor;         // Common pointer to report descritor for all the interfaces.
    USB_HID_RPT_DESC_ERROR              HIDparserError;        // Error code incase report descriptor is not in proper format.
    union
    {
        struct
        {
            BYTE                        bfDirection          : 1;   // Direction of current transfer (0=OUT, 1=IN).
            BYTE                        bfReset              : 1;   // Flag indicating to perform HID Reset.
            BYTE                        bfClearDataIN        : 1;   // Flag indicating to clear the IN endpoint.
            BYTE                        bfClearDataOUT       : 1;   // Flag indicating to clear the OUT endpoint.
            BYTE                        breportDataCollected : 1;   // Flag indicationg report data is collected ny application
        };
        BYTE                            val;
    }                                   flags;
    BYTE                                driverSupported;       // If HID driver supports requested Class,Subclass & Protocol.
    BYTE                                errorCode;             // Error code of last error.
    BYTE                                state;                 // State machine state of the device.
    BYTE                                returnState;           // State to return to after performing error handling.
    BYTE                                noOfInterfaces;        // Total number of interfaces in the device.
    BYTE                                interface;             // Interface number of current transfer.
    BYTE                                bytesTransferred;      // Number of bytes transferred to/from the user's data buffer.
    BYTE                                reportSize;            // Size of report currently requested for transfer.
    BYTE                                endpointDATA;          // Endpoint to use for the current transfer.
} USB_HID_DEVICE_INFO;




//******************************************************************************
//******************************************************************************
// Section: Local Prototypes
//******************************************************************************
//******************************************************************************
void _USBHostHID_FreeRptDecriptorDataMem(BYTE deviceAddress);
void _USBHostHID_ResetStateJump( BYTE i );


//******************************************************************************
//******************************************************************************
// Macros
//******************************************************************************
//******************************************************************************
#ifndef USB_MALLOC
    #define USB_MALLOC(size) malloc(size)
#endif

#ifndef USB_FREE
    #define USB_FREE(ptr) free(ptr)
#endif

#define USB_FREE_AND_CLEAR(ptr) {USB_FREE(ptr); ptr = NULL;}

#define _USBHostHID_LockDevice(x)                   {                                                   \
                                                        deviceInfoHID[i].errorCode  = x;                \
                                                        deviceInfoHID[i].state      = STATE_HOLDING;    \
                                                    }

#ifndef USB_ENABLE_TRANSFER_EVENT

    #define _USBHostHID_SetNextState()                  { deviceInfoHID[i].state = (deviceInfoHID[i].state & STATE_MASK) + NEXT_STATE; }
    #define _USBHostHID_SetNextSubState()               { deviceInfoHID[i].state += NEXT_SUBSTATE; }
    #define _USBHostHID_TerminateReadTransfer( error )  {                                                                       \
                                                            deviceInfoHID[i].errorCode    = error;                                 \
                                                            deviceInfoHID[i].state        = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;\
                                                        }
    #define _USBHostHID_TerminateWriteTransfer( error ) {                                                                       \
                                                            deviceInfoHID[i].errorCode    = error;                                 \
                                                            deviceInfoHID[i].state        = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;\
                                                        }
#else
    #ifdef USB_HID_ENABLE_TRANSFER_EVENT
        #define _USBHostHID_TerminateReadTransfer( error )  {                                                                       \
                                                                deviceInfoHID[i].errorCode    = error;                                 \
                                                                deviceInfoHID[i].state        = STATE_RUNNING;\
                                                                USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_READ_DONE, &transferEventData, sizeof(HID_TRANSFER_DATA) );\
                                                            }
        #define _USBHostHID_TerminateWriteTransfer( error ) {                                                                       \
                                                                deviceInfoHID[i].errorCode    = error;                                 \
                                                                deviceInfoHID[i].state        = STATE_RUNNING;\
                                                                USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_WRITE_DONE, &transferEventData, sizeof(HID_TRANSFER_DATA) );\
                                                            }
    #else
        #define _USBHostHID_TerminateReadTransfer( error )  {                                                                       \
                                                                deviceInfoHID[i].errorCode    = error;                                 \
                                                                deviceInfoHID[i].state        = STATE_RUNNING;\
                                                            }
        #define _USBHostHID_TerminateWriteTransfer( error ) {                                                                       \
                                                                deviceInfoHID[i].errorCode    = error;                                 \
                                                                deviceInfoHID[i].state        = STATE_RUNNING;\
                                                            }
    #endif
#endif

//******************************************************************************
//******************************************************************************
// Section: HID Host Global Variables
//******************************************************************************
//******************************************************************************

static USB_HID_DEVICE_INFO          deviceInfoHID[USB_MAX_HID_DEVICES] __attribute__ ((aligned));
static USB_HID_INTERFACE_DETAILS*   pInterfaceDetails = NULL;
static USB_HID_INTERFACE_DETAILS*   pCurrInterfaceDetails = NULL;

#ifdef USB_HID_ENABLE_TRANSFER_EVENT
    static HID_TRANSFER_DATA            transferEventData;
#endif

//******************************************************************************
//******************************************************************************
// Section: HID Host External Variables
//******************************************************************************
//******************************************************************************

extern BYTE* parsedDataMem;

extern USB_HID_RPT_DESC_ERROR _USBHostHID_Parse_Report(BYTE*, WORD, WORD, BYTE);

// *****************************************************************************
// *****************************************************************************
// Application Callable Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    BOOL USBHostHIDDeviceDetect( BYTE deviceAddress )

  Description:
    This function determines if a HID device is attached and ready to use.

  Precondition:
    None

  Parameters:
    BYTE deviceAddress  - Address of the attached device.

  Return Values:
    TRUE   -  HID present and ready
    FALSE  -  HID not present or not ready

  Remarks:
    This function replaces the USBHostHID_ApiDeviceDetect() function.
*******************************************************************************/
BOOL USBHostHIDDeviceDetect( BYTE deviceAddress )
{
    BYTE    i;
    
    // Find the correct device.
    for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != deviceAddress); i++);
    if (i == USB_MAX_HID_DEVICES)
    {
        return FALSE;
    }
    
    if ((USBHostHIDDeviceStatus(deviceAddress) == USB_HID_NORMAL_RUNNING) &&
        (deviceAddress != 0))
    {
        return TRUE;
    }
    
    return FALSE;
}


/*******************************************************************************
  Function:
    BYTE    USBHostHIDDeviceStatus( BYTE deviceAddress )

  Summary:

  Description:
    This function determines the status of a HID device.

  Preconditions:  None

  Parameters:
    BYTE deviceAddress - address of device to query

  Return Values:
    USB_HID_DEVICE_NOT_FOUND           -  Illegal device address, or the
                                          device is not an HID
    USB_HID_INITIALIZING               -  HID is attached and in the
                                          process of initializing
    USB_PROCESSING_REPORT_DESCRIPTOR   -  HID device is detected and report 
                                          descriptor is being parsed
    USB_HID_NORMAL_RUNNING             -  HID Device is running normal, 
                                          ready to send and receive reports 
    USB_HID_DEVICE_HOLDING             -  Driver has encountered error and
                                          could not recover
    USB_HID_DEVICE_DETACHED            -  HID detached.

  Remarks:
    None
*******************************************************************************/
BYTE    USBHostHIDDeviceStatus( BYTE deviceAddress )
{
    BYTE    i;
    BYTE    status;

    // Find the correct device.
    for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != deviceAddress); i++);
    if (i == USB_MAX_HID_DEVICES)
    {
        return USB_HID_DEVICE_NOT_FOUND;
    }

    status = USBHostDeviceStatus( deviceAddress );
    if (status != USB_DEVICE_ATTACHED)
    {
        return status;
    }
    else
    {
        // The device is attached and done enumerating.  We can get more specific now.
        #ifndef USB_ENABLE_TRANSFER_EVENT
           switch (deviceInfoHID[i].state & STATE_MASK)
           {
               case STATE_INITIALIZE_DEVICE:
                   return USB_HID_INITIALIZING;
                   break;
               case STATE_GET_REPORT_DSC:
                   return USB_PROCESSING_REPORT_DESCRIPTOR;
                   break;

               case STATE_RUNNING:
                   return USB_HID_NORMAL_RUNNING;
                   break;
               case STATE_HOLDING:
                   return USB_HID_DEVICE_HOLDING;
                   break;

               case STATE_HID_RESET_RECOVERY:
                   return USB_HID_RESETTING_DEVICE;
                   break;

               default:
                   return USB_HID_DEVICE_DETACHED;
                   break;
           }
        #else
           switch (deviceInfoHID[i].state)
           {
               case STATE_INITIALIZE_DEVICE:
               case STATE_PARSING_COMPLETE:
                   return USB_HID_INITIALIZING;
                   break;

               case STATE_RUNNING:
               case STATE_READ_REQ_WAIT:
               case STATE_WRITE_REQ_WAIT:
                   return USB_HID_NORMAL_RUNNING;
                   break;

               case STATE_HOLDING:
                   return USB_HID_DEVICE_HOLDING;
                   break;

               case STATE_WAIT_FOR_RESET:
               case STATE_RESET_COMPLETE:
                   return USB_HID_RESETTING_DEVICE;
                   break;

               default:
                   return USB_HID_DEVICE_DETACHED;
                   break;
           }
        #endif
    }
}

/*******************************************************************************
  Function:
    BYTE USBHostHIDRead( BYTE deviceAddress,BYTE reportid, BYTE interface, 
                BYTE size, BYTE *data)

  Summary:
     This function starts a Get report transfer reuest from the device,
     utilizing the function USBHostHIDTransfer();

  Precondition:
    None

  Parameters:
    BYTE deviceAddress      - Device address
    BYTE reportid           - Report ID of the requested report
    BYTE interface          - Interface number
    BYTE size               - Byte size of the data buffer
    BYTE *data              - Pointer to the data buffer

  Return Values:
    USB_SUCCESS                 - Request started successfully
    USB_HID_DEVICE_NOT_FOUND    - No device with specified address
    USB_HID_DEVICE_BUSY         - Device not in proper state for
                                  performing a transfer
    Others                      - Return values from USBHostRead()

  Remarks:
    None
*******************************************************************************/
 // Implemented as a macro. See usb_host_hid.h


/*******************************************************************************
  Function:
    BYTE USBHostHIDResetDevice( BYTE deviceAddress )

  Summary:
    This function starts a HID  reset.

  Description:
    This function starts a HID reset.  A reset can be
    issued only if the device is attached and not being initialized.

  Precondition:
    None

  Parameters:
    BYTE deviceAddress - Device address

  Return Values:
    USB_SUCCESS                 - Reset started
    USB_MSD_DEVICE_NOT_FOUND    - No device with specified address
    USB_MSD_ILLEGAL_REQUEST     - Device is in an illegal state for reset

  Remarks:
    None
*******************************************************************************/
BYTE USBHostHIDResetDevice( BYTE deviceAddress )
{
    BYTE    i;

    // Find the correct device.
    for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != deviceAddress); i++);
    if (i == USB_MAX_HID_DEVICES)
    {
        return USB_HID_DEVICE_NOT_FOUND;
    }

    #ifndef USB_ENABLE_TRANSFER_EVENT
    if (((deviceInfoHID[i].state & STATE_MASK) != STATE_DETACHED) &&
        ((deviceInfoHID[i].state & STATE_MASK) != STATE_INITIALIZE_DEVICE))
    #else
    if ((deviceInfoHID[i].state != STATE_DETACHED) &&
        (deviceInfoHID[i].state != STATE_INITIALIZE_DEVICE))
    #endif
    {
        deviceInfoHID[i].flags.val |= MARK_RESET_RECOVERY;
        deviceInfoHID[i].flags.bfReset = 1;
        #ifndef USB_ENABLE_TRANSFER_EVENT
            deviceInfoHID[i].returnState = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
        #else
            deviceInfoHID[i].returnState = STATE_RUNNING;
        #endif

        _USBHostHID_ResetStateJump( i );
        return USB_SUCCESS;
    }
    
    return USB_HID_ILLEGAL_REQUEST;
}


/****************************************************************************
  Function:
    BOOL USBHostHIDResetDeviceWithWait( BYTE deviceAddress  )

  Description:
    This function resets a HID device, and waits until the reset is complete.

  Precondition:
    None

  Parameters:
    BYTE deviceAddress  - Address of the device to reset.

  Return Values:
    USB_SUCCESS                 - Reset successful
    USB_HID_RESET_ERROR         - Error while resetting device
    Others                      - See return values for USBHostHIDResetDevice()
                                    and error codes that can be returned
                                    in the errorCode parameter of
                                    USBHostHIDTransferIsComplete();
                                    
  Remarks:
    None
  ***************************************************************************/

BYTE USBHostHIDResetDeviceWithWait( BYTE deviceAddress  )
{
    BYTE    byteCount;
    BYTE    errorCode;

    errorCode = USBHostHIDResetDevice( deviceAddress );
    if (errorCode)
    {
        return errorCode;
    }

    do
    {
        USBTasks();
        errorCode = USBHostHIDDeviceStatus( deviceAddress );
    } while (errorCode == USB_HID_RESETTING_DEVICE);


    if (USBHostHIDTransferIsComplete( deviceAddress, &errorCode, (BYTE*)&byteCount ))
    {
        return errorCode;
    }

    return USB_HID_RESET_ERROR;
}

/*******************************************************************************
  Function:
     void USBHostHIDTasks( void )

  Summary:
    This function performs the maintenance tasks required by HID class

  Description:
    This function performs the maintenance tasks required by the HID
    class.  If transfer events from the host layer are not being used, then
    it should be called on a regular basis by the application.  If transfer
    events from the host layer are being used, this function is compiled out,
    and does not need to be called.

  Precondition:
    USBHostHIDInitialize() has been called.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
*******************************************************************************/
void USBHostHIDTasks( void )
{
#ifndef USB_ENABLE_TRANSFER_EVENT
    DWORD   byteCount;
    BYTE    errorCode;
    BYTE    i;

    for (i=0; i<USB_MAX_HID_DEVICES; i++)
    {
        if (deviceInfoHID[i].ID.deviceAddress == 0) /* device address updated by lower layer */
        {
            deviceInfoHID[i].state = STATE_DETACHED;
        }

        switch (deviceInfoHID[i].state & STATE_MASK)
        {
            case STATE_DETACHED:
                // No device attached.
                break;

            case STATE_INITIALIZE_DEVICE:
                switch (deviceInfoHID[i].state & SUBSTATE_MASK)
                {
                    case SUBSTATE_WAIT_FOR_ENUMERATION:
                        if (USBHostDeviceStatus( deviceInfoHID[i].ID.deviceAddress ) == USB_DEVICE_ATTACHED)
                        {
                            _USBHostHID_SetNextSubState();
                            pCurrInterfaceDetails = pInterfaceDetails; // assign current interface to top of list
                        }
                        break;

                    case SUBSTATE_DEVICE_ENUMERATED:
                        _USBHostHID_SetNextState(); /* need to add sub states to Set Config, Get LANGID & String Descriptors */
                        break;

                    default :
                        break;
                }
                break;

            case STATE_GET_REPORT_DSC:
                switch (deviceInfoHID[i].state & SUBSTATE_MASK)
                {
                    case SUBSTATE_SEND_GET_REPORT_DSC:
                        // If we are currently sending a token, we cannot do anything.
                        if (U1CONbits.TOKBUSY)
                            break;

                        if(pCurrInterfaceDetails != NULL) // end of interface list
                        {
                            if(pCurrInterfaceDetails->sizeOfRptDescriptor !=0) // interface must have a Report Descriptor
                            {
                                if((deviceInfoHID[i].rptDescriptor = (BYTE *)USB_MALLOC(pCurrInterfaceDetails->sizeOfRptDescriptor)) == NULL)
                                {
                                    _USBHostHID_LockDevice( USB_MEMORY_ALLOCATION_ERROR );
                                    break;
                                }
                                // send new interface request
                                if (!USBHostIssueDeviceRequest( deviceInfoHID[i].ID.deviceAddress, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_STANDARD | USB_SETUP_RECIPIENT_INTERFACE,
                                     USB_REQUEST_GET_DESCRIPTOR, DSC_RPT, pCurrInterfaceDetails->interfaceNumber,pCurrInterfaceDetails->sizeOfRptDescriptor, deviceInfoHID[i].rptDescriptor,
                                     USB_DEVICE_REQUEST_GET, deviceInfoHID[i].ID.clientDriverID ))
                                {
                                     _USBHostHID_SetNextSubState();
                                }
                                else
                                {
                                    USB_FREE_AND_CLEAR(deviceInfoHID[i].rptDescriptor);
                                }
                            }
                        }
                        else
                        {
                            _USBHostHID_LockDevice( USB_HID_INTERFACE_ERROR );
                        }
                        break;

                    case SUBSTATE_WAIT_FOR_REPORT_DSC:
                        if (USBHostTransferIsComplete( deviceInfoHID[i].ID.deviceAddress, 0, &errorCode, &byteCount ))
                        {
                            if (errorCode)
                            {
                                /* Set error code */
                                _USBHostHID_LockDevice( errorCode );
                            }
                            else
                            {
                                // Clear the STALL.  Since it is EP0, we do not have to clear the stall.
                                USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, 0 );
                            }
                            _USBHostHID_SetNextSubState();
                        }
                        break;

                    case SUBSTATE_GET_REPORT_DSC_COMPLETE:
                        _USBHostHID_SetNextSubState();
                        break;

                    case SUBSTATE_PARSE_REPORT_DSC:
                        /* Invoke HID Parser ,, validate for all the errors in report Descriptor */
                        deviceInfoHID[i].HIDparserError = _USBHostHID_Parse_Report((BYTE*)deviceInfoHID[i].rptDescriptor , (WORD)pCurrInterfaceDetails->sizeOfRptDescriptor,
                                                                               (WORD)pCurrInterfaceDetails->endpointPollInterval, pCurrInterfaceDetails->interfaceNumber);
                        if(deviceInfoHID[i].HIDparserError)
                        {
                            /* Report Descriptor is flawed , flag error and free memory ,
                               retry by requesting again */
                            #ifdef DEBUG_MODE
                                UART2PrintString("\r\nHID Error Reported :  ");
                                UART2PutHex(deviceInfoHID[i].HIDparserError);
                            #endif
                            _USBHostHID_FreeRptDecriptorDataMem(deviceInfoHID[i].ID.deviceAddress);
                            _USBHostHID_LockDevice( USB_HID_REPORT_DESCRIPTOR_BAD );
                            USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_BAD_REPORT_DESCRIPTOR, NULL, 0 );
                        }
                        else
                        {
                            _USBHostHID_SetNextSubState();
                        }
                        break;

                    case SUBSTATE_PARSING_COMPLETE:
                        if (USB_HOST_APP_EVENT_HANDLER(deviceInfoHID[i].ID.deviceAddress, EVENT_HID_RPT_DESC_PARSED, NULL, 0 ))
                        {
                            deviceInfoHID[i].flags.breportDataCollected = 1;
                        }
                        else
                        {
                            // At least once report must be present.  If not, flag error.
                            if((pCurrInterfaceDetails->interfaceNumber == (deviceInfoHID[i].noOfInterfaces-1)) &&
                                   (deviceInfoHID[i].flags.breportDataCollected == 0))
                            {
                                _USBHostHID_FreeRptDecriptorDataMem(deviceInfoHID[i].ID.deviceAddress);
                                _USBHostHID_LockDevice( USB_HID_REPORT_DESCRIPTOR_BAD );
                                USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_BAD_REPORT_DESCRIPTOR, NULL, 0 );
                            }
                        }
                        // free the previous allocated memory, reallocate for new interface if needed
                        USB_FREE_AND_CLEAR(deviceInfoHID[i].rptDescriptor);
                        pCurrInterfaceDetails = pCurrInterfaceDetails->next;
                        if(pCurrInterfaceDetails != NULL)
                        {
                            deviceInfoHID[i].state = STATE_GET_REPORT_DSC;
                        }
                        else
                        {
                            if(deviceInfoHID[i].flags.breportDataCollected == 0)
                            {
                                _USBHostHID_FreeRptDecriptorDataMem(deviceInfoHID[i].ID.deviceAddress);
                                _USBHostHID_LockDevice( USB_HID_REPORT_DESCRIPTOR_BAD );
                                USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_BAD_REPORT_DESCRIPTOR, NULL, 0 );
                            }
                            else
                            {
                                deviceInfoHID[i].state = STATE_RUNNING;
                            }
                        }
                        break;

                    default :
                        break;
                }
                break;

            case STATE_RUNNING:
                switch (deviceInfoHID[i].state & SUBSTATE_MASK)
                {
                    case SUBSTATE_WAITING_FOR_REQ:
                        /* waiting for request from application */
                        break;

                    case SUBSTATE_SEND_READ_REQ:
                        // State removed with new architecture.  Functionality is consolidated elsewhere.
                        break;

                    case SUBSTATE_READ_REQ_WAIT:
                        if (USBHostTransferIsComplete( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA, &errorCode, &byteCount ))
                        {
                            if (errorCode)
                            {
                                if(USB_ENDPOINT_STALLED == errorCode)
                                {
                                     USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
                                     deviceInfoHID[i].returnState = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
                                     deviceInfoHID[i].flags.bfReset = 1;
                                     _USBHostHID_ResetStateJump( i );
                                }
                                else
                                {
                                    /* Set proper error code as per HID guideline */
                                    #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
                                        transferEventData.dataCount         = byteCount;
                                        transferEventData.bErrorCode        = errorCode;
                                    #endif
                                    _USBHostHID_TerminateReadTransfer(errorCode);
                                }
                            }
                            else
                            {
                                // Clear the STALL.  Since it is EP0, we do not have to clear the stall.
                                USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
                                deviceInfoHID[i].bytesTransferred = byteCount; /* Can compare with report size and flag error ???*/
                               _USBHostHID_SetNextSubState();
                            }
                        }
                        #ifdef DEBUG_MODE
                            UART2PrintString("|");
                        #endif
                        break;

                    case SUBSTATE_READ_REQ_DONE:
                        /* Next transfer */
                        deviceInfoHID[i].state = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
                        #ifdef USB_HID_ENABLE_TRANSFER_EVENT
                            USB_HOST_APP_EVENT_HANDLER(deviceInfoHID[i].ID.deviceAddress, EVENT_HID_READ_DONE, NULL, 0 );
                        #endif
                        #ifdef DEBUG_MODE
                            UART2PrintString("}");
                        #endif
                        break;

                    case SUBSTATE_SEND_WRITE_REQ:
                        // State removed with new architecture.  Functionality is consolidated elsewhere.
                        break;

                    case SUBSTATE_WRITE_REQ_WAIT:
                        if (USBHostTransferIsComplete( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA, &errorCode, &byteCount ))
                        {
                            if (errorCode)
                            {
                                if(USB_ENDPOINT_STALLED == errorCode)
                                {
                                     USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
                                     deviceInfoHID[i].returnState = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
                                     deviceInfoHID[i].flags.bfReset = 1;
                                     _USBHostHID_ResetStateJump( i );
                                }
                                else
                                {
                                    /* Set proper error code as per HID guideline */
                                    #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
                                        transferEventData.dataCount         = byteCount;
                                        transferEventData.bErrorCode        = errorCode;
                                    #endif
                                    _USBHostHID_TerminateWriteTransfer(errorCode);
                                }
                            }
                            else
                            {
                                // MCHP - assuming only a STALL here
                                // Clear the STALL.  Since it is EP0, we do not have to clear the stall.
                                USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA);
                                _USBHostHID_SetNextSubState();
                            }
                        }
                        break;

                    case SUBSTATE_WRITE_REQ_DONE:
                        deviceInfoHID[i].state = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
                        #ifdef USB_HID_ENABLE_TRANSFER_EVENT
                            USB_HOST_APP_EVENT_HANDLER(deviceInfoHID[i].ID.deviceAddress, EVENT_HID_WRITE_DONE, NULL, 0 );
                        #endif
                    break;

                }
                break;

            case STATE_HID_RESET_RECOVERY:
                switch (deviceInfoHID[i].state & SUBSTATE_MASK)
                {
                    case SUBSTATE_SEND_RESET:   /* Not sure of rest request */
                        // State removed with new architecture.  Functionality is consolidated elsewhere.
                        break;

                    case SUBSTATE_WAIT_FOR_RESET:
                        if (USBHostTransferIsComplete( deviceInfoHID[i].ID.deviceAddress, 0, &errorCode, &byteCount ))
                        {
                            deviceInfoHID[i].flags.bfReset  = 0;
                            if (errorCode)
                            {
                                if(USB_ENDPOINT_STALLED == errorCode)
                                {
                                    // If it stalled, try again.
                                     USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, 0 );
                                     deviceInfoHID[i].returnState = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
                                     deviceInfoHID[i].flags.bfReset = 1;
                                }
                            }
                            deviceInfoHID[i].errorCode = errorCode;
                            _USBHostHID_ResetStateJump( i );
                        }
                        break;
                }
                break;

            case STATE_HOLDING:
                break;

            default :
                break;
        }
    }
#endif
}


/*******************************************************************************
  Function:
     BYTE USBHostHIDTerminateTransfer( BYTE deviceAddress, BYTE direction, BYTE interfaceNum )

  Summary:
    This function terminates a transfer that is in progress.

  Description:
    This function terminates a transfer that is in progress.

  Precondition:
    None

  Parameters:
    BYTE deviceAddress  - Device address
    BYTE direction      - Transfer direction.  Valid values are:
                            * 1 = In (Read)
                            * 0 = Out (Write)
    BYTE interfaceNum   - Interface number
                            
  Return Values:
    USB_SUCCESS                 - Transfer terminated
    USB_HID_DEVICE_NOT_FOUND    - No device with specified address
    
  Remarks:
    None
*******************************************************************************/
BYTE USBHostHIDTerminateTransfer( BYTE deviceAddress, BYTE direction, BYTE interfaceNum )
{
    BYTE    i;
    BYTE    endpoint;

    // Find the correct device.
    for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != deviceAddress); i++);
    if (i == USB_MAX_HID_DEVICES)
    {
        return USB_HID_DEVICE_NOT_FOUND;
    }

    pCurrInterfaceDetails = pInterfaceDetails;
    while((pCurrInterfaceDetails != NULL) && (pCurrInterfaceDetails->interfaceNumber != interfaceNum))
    {
        pCurrInterfaceDetails = pCurrInterfaceDetails->next;
    }
    
    endpoint = pCurrInterfaceDetails->endpointIN;
    if (direction == 0)
    {
        endpoint = pCurrInterfaceDetails->endpointOUT;
    }
        
    USBHostTerminateTransfer( deviceAddress, endpoint );
    
    // Reset the state.
    deviceInfoHID[i].state = STATE_RUNNING;
    
    return USB_SUCCESS;
}

    
/*******************************************************************************
  Function:
    USBHostHIDTransfer( BYTE deviceAddress, BYTE direction, BYTE interfaceNum, 
                BYTE reportid, BYTE size, BYTE *data)

  Summary:
    This function starts a HID transfer.

  Description:
    This function starts a HID transfer. A read/write wrapper is provided in application
    interface file to access this function.

  Preconditions:
    None

  Parameters:
    BYTE deviceAddress      - Device address
    BYTE direction          - 1=read, 0=write
    BYTE interfaceNum       - Interface number
    BYTE reportid           - Report ID of the requested report
    BYTE size               - Byte size of the data buffer
    BYTE *data              - Pointer to the data buffer

  Return Values:
    USB_SUCCESS                 - Request started successfully
    USB_HID_DEVICE_NOT_FOUND    - No device with specified address
    USB_HID_DEVICE_BUSY         - Device not in proper state for
                                  performing a transfer
    Others                      - Return values from USBHostIssueDeviceRequest(),
                                    USBHostRead(), and USBHostWrite()

  Remarks:
    None
*******************************************************************************/

BYTE USBHostHIDTransfer( BYTE deviceAddress, BYTE direction, BYTE interfaceNum, WORD reportid, WORD size, BYTE *data)
{
    BYTE    i;
    BYTE    errorCode;

    #ifdef DEBUG_MODE
//        UART2PrintString( "HID: Transfer: " );
        if (direction)
        {
//            UART2PrintString( "Read, " );
        }
        else
        {
 //           UART2PrintString( "Write, " );
        }
    #endif

    // Find the correct device.
    for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != deviceAddress); i++);
    if (i == USB_MAX_HID_DEVICES)
    {
        return USB_HID_DEVICE_NOT_FOUND;
    }

    pCurrInterfaceDetails = pInterfaceDetails;
    while((pCurrInterfaceDetails != NULL) && (pCurrInterfaceDetails->interfaceNumber != interfaceNum))
    {
        pCurrInterfaceDetails = pCurrInterfaceDetails->next;
    }
    // Make sure the device is in a state ready to read/write.
    // Make sure the device is in a state ready to read/write.
    #ifndef USB_ENABLE_TRANSFER_EVENT
        if (deviceInfoHID[i].state != (STATE_RUNNING) &&
           (deviceInfoHID[i].state & SUBSTATE_MASK) != (SUBSTATE_WAITING_FOR_REQ))
    #else
        if (deviceInfoHID[i].state != STATE_RUNNING)
    #endif
        {
            return USB_HID_DEVICE_BUSY;
        }

    // Initialize the transfer information.
    deviceInfoHID[i].bytesTransferred  = 0;
    deviceInfoHID[i].errorCode         = USB_SUCCESS;
    deviceInfoHID[i].userData          = data;
    deviceInfoHID[i].endpointDATA      = pCurrInterfaceDetails->endpointIN;
    deviceInfoHID[i].reportSize        = size;
    deviceInfoHID[i].reportId          = reportid;
    deviceInfoHID[i].interface         = interfaceNum;

    if (!direction) // OUT
    {
        deviceInfoHID[i].endpointDATA  = pCurrInterfaceDetails->endpointOUT;
        deviceInfoHID[i].reportId      = (reportid |((WORD)USB_HID_OUTPUT_REPORT<<8));
    }
    else
    {
        deviceInfoHID[i].endpointDATA      = pCurrInterfaceDetails->endpointIN;
    }    
    #ifdef DEBUG_MODE
//        UART2PrintString( "Data EP: " );
//        UART2PutHex( deviceInfoHID[i].endpointDATA );
//        UART2PrintString( "\r\n" );
    #endif

    if(!direction)
    {
        if(deviceInfoHID[i].endpointDATA == 0x00)// if endpoint 0 then use control transfer
        {
            errorCode = USBHostIssueDeviceRequest( deviceInfoHID[i].ID.deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE,
                                                     USB_HID_SET_REPORT, deviceInfoHID[i].reportId, deviceInfoHID[i].interface,deviceInfoHID[i].reportSize, deviceInfoHID[i].userData,
                                                     USB_DEVICE_REQUEST_SET, deviceInfoHID[i].ID.clientDriverID );
        }
        else
        {
            errorCode = USBHostWrite( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA,
                                      deviceInfoHID[i].userData, deviceInfoHID[i].reportSize );
        }

        if (!errorCode)
        {
            #ifndef USB_ENABLE_TRANSFER_EVENT
                deviceInfoHID[i].state  = STATE_RUNNING | SUBSTATE_WRITE_REQ_WAIT;
            #else
                deviceInfoHID[i].state  = STATE_WRITE_REQ_WAIT;
            #endif
        }
        else
        {
            deviceInfoHID[i].errorCode    = errorCode;
            deviceInfoHID[i].state        = STATE_RUNNING;
        }
    }
    else
    {
        errorCode = USBHostRead( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA,
                                 deviceInfoHID[i].userData, deviceInfoHID[i].reportSize );
        if (!errorCode)
        {
            #ifndef USB_ENABLE_TRANSFER_EVENT
                deviceInfoHID[i].state  = STATE_RUNNING | SUBSTATE_READ_REQ_WAIT;
            #else
                deviceInfoHID[i].state  = STATE_READ_REQ_WAIT;
            #endif
        }
        else
        {
            deviceInfoHID[i].errorCode    = errorCode;
            deviceInfoHID[i].state        = STATE_RUNNING;
        }
    }

    return errorCode;
}


/*******************************************************************************
  Function:
    BOOL USBHostHIDTransferIsComplete( BYTE deviceAddress,
                        BYTE *errorCode, DWORD *byteCount )

  Summary:
    This function indicates whether or not the last transfer is complete.

  Description:
    This function indicates whether or not the last transfer is complete.
    If the functions returns TRUE, the returned byte count and error
    code are valid. Since only one transfer can be performed at once
    and only one endpoint can be used, we only need to know the
    device address.

  Precondition:
    None

  Parameters:
    BYTE deviceAddress  - Device address
    BYTE *errorCode     - Error code from last transfer
    DWORD *byteCount    - Number of bytes transferred

  Return Values:
    TRUE    - Transfer is complete, errorCode is valid
    FALSE   - Transfer is not complete, errorCode is not valid
*******************************************************************************/

BOOL USBHostHIDTransferIsComplete( BYTE deviceAddress, BYTE *errorCode, BYTE *byteCount )
{
    BYTE    i;

    // Find the correct device.
    for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != deviceAddress); i++);
    if ((i == USB_MAX_HID_DEVICES) || (deviceInfoHID[i].state == STATE_DETACHED))
    {
        *errorCode = USB_HID_DEVICE_NOT_FOUND;
        *byteCount = 0;
        return TRUE;
    }

    *byteCount = deviceInfoHID[i].bytesTransferred;
    *errorCode = deviceInfoHID[i].errorCode;

    #ifndef USB_ENABLE_TRANSFER_EVENT
    if(deviceInfoHID[i].state == (STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ))
    #else
    if(deviceInfoHID[i].state == STATE_RUNNING)
    #endif
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************************************
  Function:
    BYTE USBHostHIDWrite( BYTE deviceAddress,BYTE reportid, BYTE interface,
                BYTE size, BYTE *data)

  Summary:
    This function starts a Set report transfer request to the device,
    utilizing the function USBHostHIDTransfer();

  Precondition:
    None

  Parameters:
    BYTE deviceAddress      - Device address
    BYTE reportid           - Report ID of the requested report
    BYTE interface          - Interface number
    BYTE size               - Byte size of the data buffer
    BYTE *data              - Pointer to the data buffer

  Return Values:
    USB_SUCCESS                 - Request started successfully
    USB_HID_DEVICE_NOT_FOUND    - No device with specified address
    USB_HID_DEVICE_BUSY         - Device not in proper state for
                                  performing a transfer
    Others                      - Return values from USBHostIssueDeviceRequest(),
                                    and USBHostWrite()
                                    
  Remarks:
    None
*******************************************************************************/
 // Implemented as a macro. See usb_host_hid.h

/*******************************************************************************
  Function:
    BOOL USBHostHID_ApiFindBit(WORD usagePage,WORD usage,HIDReportTypeEnum type,
                          BYTE* Report_ID, BYTE* Report_Length, BYTE* Start_Bit)

  Description:
    This function is used to locate a specific button or indicator.
    Once the report descriptor is parsed by the HID layer without any error,
    data from the report descriptor is stored in pre defined dat structures.
    This function traverses these data structure and exract data required
    by application

  Precondition:
    None

  Parameters:
    WORD usagePage         - usage page supported by application
    WORD usage             - usage supported by application
    HIDReportTypeEnum type - report type Input/Output for the particular
                             usage
    BYTE* Report_ID        - returns the report ID of the required usage
    BYTE* Report_Length    - returns the report length of the required usage
    BYTE* Start_Bit        - returns  the start bit of the usage in a
                             particular report

  Return Values:
    TRUE    - If the required usage is located in the report descriptor
    FALSE   - If the application required usage is not supported by the 
              device(i.e report descriptor).

  Remarks:
    Application event handler with event 'EVENT_HID_RPT_DESC_PARSED' is called.
    Application is suppose to fill in data details in structure 'HID_DATA_DETAILS'.
    This function can be used to the get the details of the required usages.
*******************************************************************************/
BOOL USBHostHID_ApiFindBit(WORD usagePage,WORD usage,HIDReportTypeEnum type,BYTE* Report_ID,
                    BYTE* Report_Length, BYTE* Start_Bit)
{
    WORD iR;
    WORD index;
    WORD reportIndex;
    HID_REPORTITEM *reportItem;
    BYTE* count;

//  Disallow Null Pointers

    if((Report_ID == NULL)|(Report_Length == NULL)|(Start_Bit == NULL))
        return FALSE;

//  Search through all the report items

    for (iR=0; iR < deviceRptInfo.reportItems; iR++)
        {
            reportItem = &itemListPtrs.reportItemList[iR];

//      Search only reports of the proper type

            if ((reportItem->reportType==type))// && (reportItem->globals.reportsize == 1))
                {
                    if (USBHostHID_HasUsage(reportItem,usagePage,usage,(WORD*)&index,(BYTE*)&count))
                        {
                             reportIndex = reportItem->globals.reportIndex;
                             *Report_ID = itemListPtrs.reportList[reportIndex].reportID;
                             *Start_Bit = reportItem->startBit + index;
                             if (type == hidReportInput)
                                 *Report_Length = (itemListPtrs.reportList[reportIndex].inputBits + 7)/8;
                             else if (type == hidReportOutput)
                                 *Report_Length = (itemListPtrs.reportList[reportIndex].outputBits + 7)/8;
                             else
                                 *Report_Length = (itemListPtrs.reportList[reportIndex].featureBits + 7)/8;
                             return TRUE;
                         }
                }
        }
    return FALSE;
}

/*******************************************************************************
  Function:
    BOOL USBHostHID_ApiFindValue(WORD usagePage,WORD usage,
                HIDReportTypeEnum type,BYTE* Report_ID,BYTE* Report_Length,BYTE*
                Start_Bit, BYTE* Bit_Length)

  Description:
    Find a specific Usage Value. Once the report descriptor is parsed by the HID
    layer without any error, data from the report descriptor is stored in
    pre defined dat structures. This function traverses these data structure and
    exract data required by application.

  Precondition:
    None

  Parameters:
    WORD usagePage         - usage page supported by application
    WORD usage             - usage supported by application
    HIDReportTypeEnum type - report type Input/Output for the particular
                             usage
    BYTE* Report_ID        - returns the report ID of the required usage
    BYTE* Report_Length    - returns the report length of the required usage
    BYTE* Start_Bit        - returns  the start bit of the usage in a
                             particular report
    BYTE* Bit_Length       - returns size of requested usage type data in bits

  Return Values:
    TRUE    - If the required usage is located in the report descriptor
    FALSE   - If the application required usage is not supported by the 
              device(i.e report descriptor).

  Remarks:
    Application event handler with event 'EVENT_HID_RPT_DESC_PARSED' is called.
    Application is suppose to fill in data details structure 'HID_DATA_DETAILS'
    This function can be used to the get the details of the required usages.
*******************************************************************************/
BOOL USBHostHID_ApiFindValue(WORD usagePage,WORD usage,HIDReportTypeEnum type,BYTE* Report_ID,
                    BYTE* Report_Length,BYTE* Start_Bit, BYTE* Bit_Length)
{
    WORD index;
    WORD reportIndex;
    BYTE iR;
    BYTE count;
    HID_REPORTITEM *reportItem;

//  Disallow Null Pointers

     if((Report_ID == NULL)|(Report_Length == NULL)|(Start_Bit == NULL)|(Bit_Length == NULL))
        return FALSE;

//  Search through all the report items

    for (iR=0; iR < deviceRptInfo.reportItems; iR++)
    {
        reportItem = &itemListPtrs.reportItemList[iR];

//      Search only reports of the proper type

        if ((reportItem->reportType==type)&& ((reportItem->dataModes & HIDData_ArrayBit) != HIDData_Array)
             && (reportItem->globals.reportsize != 1))
        {
            if (USBHostHID_HasUsage(reportItem,usagePage,usage,&index,&count))
            {
                 reportIndex = reportItem->globals.reportIndex;
                 *Report_ID = itemListPtrs.reportList[reportIndex].reportID;
                 *Bit_Length = reportItem->globals.reportsize;
                 *Start_Bit = reportItem->startBit + index * (reportItem->globals.reportsize);
                 if (type == hidReportInput)
                     *Report_Length = (itemListPtrs.reportList[reportIndex].inputBits + 7)/8;
                 else if (type == hidReportOutput)
                     *Report_Length = (itemListPtrs.reportList[reportIndex].outputBits + 7)/8;
                 else
                     *Report_Length = (itemListPtrs.reportList[reportIndex].featureBits + 7)/8;
                 return TRUE;
             }
        }
    }
    return FALSE;
}


/*******************************************************************************
  Function:
    BYTE USBHostHID_ApiGetCurrentInterfaceNum(void)

  Description:
    This function reurns the interface number of the cuurent report descriptor
    parsed. This function must be called to fill data interface detail data
    structure and passed as parameter when requesinf for report transfers.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    TRUE    - Transfer is complete, errorCode is valid
    FALSE   - Transfer is not complete, errorCode is not valid

  Remarks:
    None
*******************************************************************************/
BYTE USBHostHID_ApiGetCurrentInterfaceNum(void)
{
    return(deviceRptInfo.interfaceNumber);
}

/****************************************************************************
  Function:
    BYTE* USBHostHID_GetCurrentReportInfo(void)

  Description:
    This function returns a pointer to the current report info structure.

  Precondition:
    None

  Parameters:
    None

  Returns:
    BYTE * - Pointer to the report Info structure.

  Remarks:
    None
  ***************************************************************************/
 // Implemented as a macro. See usb_host_hid.h


/****************************************************************************
  Function:
    BYTE* USBHostHID_GetItemListPointers()

  Description:
    This function returns a pointer to list of item pointers stored in a
    structure.

  Precondition:
    None

  Parameters:
    None

  Returns:
    BYTE * - Pointer to list of item pointers structure.

  Remarks:
    None
  ***************************************************************************/
 // Implemented as a macro. See usb_host_hid.h


/*******************************************************************************
  Function:
    BOOL USBHostHID_ApiImportData(BYTE *report, WORD reportLength, 
                     HID_USER_DATA_SIZE *buffer,HID_DATA_DETAILS *pDataDetails)
  Description:
    This function can be used by application to extract data from the input 
    reports. On receiving the input report from the device application can call
    the function with required inputs 'HID_DATA_DETAILS'.

  Precondition:
    None

  Parameters:
    BYTE *report                    - Input report received from device
    WORD reportLength               - Length of input report report
    HID_USER_DATA_SIZE *buffer      - Buffer into which data needs to be
                                      populated
    HID_DATA_DETAILS *pDataDetails  - data details extracted from report
                                      descriptor
  Return Values:
    TRUE    - If the required data is retrieved from the report
    FALSE   - If required data is not found.

  Remarks:
    None
*******************************************************************************/
BOOL USBHostHID_ApiImportData(BYTE *report, WORD reportLength, HID_USER_DATA_SIZE *buffer, HID_DATA_DETAILS *pDataDetails)
{
    WORD data;
    WORD signBit;
    WORD mask;
    WORD extendMask;
    WORD start;
    WORD startByte;
    WORD startBit;
    WORD lastByte;
    WORD i;

//  Report must be ok

    if (report == NULL) return FALSE;

//  Must be the right report

    if ((pDataDetails->reportID != 0) && (pDataDetails->reportID != report[0])) return FALSE;

//  Length must be ok

    if (pDataDetails->reportLength != reportLength) return FALSE;
    lastByte = (pDataDetails->bitOffset + (pDataDetails->bitLength * pDataDetails->count) - 1)/8;
    if (lastByte > reportLength) return FALSE;

//  Extract data one count at a time

    start = pDataDetails->bitOffset;
    for (i=0; i<pDataDetails->count; i++) {
        startByte = start/8;
        startBit = start&7;
        lastByte = (start + pDataDetails->bitLength - 1)/8;

//      Pick up the data bytes backwards

        data = 0;
        do {
            data <<= 8;
            data |= (int) report[lastByte];
        }
        while (lastByte-- > startByte);

//      Shift to the right to byte align the least significant bit

        if (startBit > 0) data >>= startBit;

//      Done if 16 bits long

        if (pDataDetails->bitLength < 16) {

//          Mask off the other bits

            mask = 1 << pDataDetails->bitLength;
            mask--;
            data &= mask;

//          Sign extend the report item

            if (pDataDetails->signExtend) {
                signBit = 1;
                if (pDataDetails->bitLength > 1) signBit <<= (pDataDetails->bitLength-1);
                extendMask = (signBit << 1) - 1;
                if ((data & signBit)==0) data &= extendMask;
                else data |= ~extendMask;
            }
        }

//      Save the value

        *buffer++ = data;

//      Next one

        start += pDataDetails->bitLength;
    }
    return TRUE;
}


// *****************************************************************************
// *****************************************************************************
// Section: Host Stack Interface Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    BOOL USBHostHIDEventHandler( BYTE address, USB_EVENT event,
                        void *data, DWORD size )

  Precondition:
    The device has been initialized.

  Summary:
    This function is the event handler for this client driver.

  Description:
    This function is the event handler for this client driver.  It is called
    by the host layer when various events occur.

  Parameters:
    BYTE address    - Address of the device
    USB_EVENT event - Event that has occurred
    void *data      - Pointer to data pertinent to the event
    DWORD size       - Size of the data

  Return Values:
    TRUE   - Event was handled
    FALSE  - Event was not handled

  Remarks:
    None
*******************************************************************************/
BOOL USBHostHIDEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    BYTE    i;
    switch (event)
    {
        case EVENT_NONE:             // No event occured (NULL event)
            return TRUE;
            break;

        case EVENT_DETACH:           // USB cable has been detached (data: BYTE, address of device)
            #ifdef DEBUG_MODE
                UART2PrintString( "HID: Detach\r\n" );
            #endif
            // Find the device in the table.  If found, clear the important fields.
            for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != address); i++);
            if (i < USB_MAX_HID_DEVICES)
            {
                // Notify that application that the device has been detached.
                USB_HOST_APP_EVENT_HANDLER( address, EVENT_HID_DETACH,
                    &deviceInfoHID[i].ID.deviceAddress, sizeof(BYTE) );

                /* Free the memory used by the HID device */
                _USBHostHID_FreeRptDecriptorDataMem(address);
                deviceInfoHID[i].ID.deviceAddress   = 0;
                deviceInfoHID[i].state              = STATE_DETACHED;
            }

            return TRUE;
            break;

        case EVENT_TRANSFER:         // A USB transfer has completed
            #if defined( USB_ENABLE_TRANSFER_EVENT )
                #ifdef DEBUG_MODE
                    UART2PrintString( "HID: transfer event\r\n" );
                #endif

                for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != address); i++) {}
                if (i == USB_MAX_HID_DEVICES)
                {
                    #ifdef DEBUG_MODE
                        UART2PrintString( "HID: Unknown device\r\n" );
                    #endif
                    return FALSE;
                }
                #ifdef DEBUG_MODE
 //                   UART2PrintString( "HID: Device state: " );
 //                   UART2PutHex( deviceInfoHID[i].state );
 //                   UART2PrintString( "\r\n" );
                #endif

                switch (deviceInfoHID[i].state)
                {
                    case STATE_WAIT_FOR_REPORT_DSC:
                        #ifdef DEBUG_MODE
                            UART2PrintString( "HID: Got Report Descriptor\r\n" );
                        #endif
                        deviceInfoHID[i].bytesTransferred = ((HOST_TRANSFER_DATA *)data)->dataCount;
                        if ((!((HOST_TRANSFER_DATA *)data)->bErrorCode) && (deviceInfoHID[i].bytesTransferred == pCurrInterfaceDetails->sizeOfRptDescriptor ))
                        {
                            /* Invoke HID Parser ,, validate for all the errors in report Descriptor */
//                             deviceInfoHID[i].bytesTransferred = ((HOST_TRANSFER_DATA *)data)->dataCount;
                             deviceInfoHID[i].HIDparserError = _USBHostHID_Parse_Report((BYTE*)deviceInfoHID[i].rptDescriptor , (WORD)pCurrInterfaceDetails->sizeOfRptDescriptor,
                                                                                    (WORD)pCurrInterfaceDetails->endpointPollInterval, pCurrInterfaceDetails->interfaceNumber);

                            if(deviceInfoHID[i].HIDparserError)
                            {
                                /* Report Descriptor is flawed , flag error and free memory ,
                                   retry by requesting again */
                                #ifdef DEBUG_MODE
                                    UART2PrintString("\r\nHID Error Reported :  ");
                                    UART2PutHex(deviceInfoHID[i].HIDparserError);
                                #endif

                                _USBHostHID_FreeRptDecriptorDataMem(deviceInfoHID[i].ID.deviceAddress);
                                _USBHostHID_LockDevice( USB_HID_REPORT_DESCRIPTOR_BAD );
                                USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_BAD_REPORT_DESCRIPTOR, NULL, 0 );
                            }
                            else
                            {
                                /* Inform Application layer of new device attached */
                                #ifdef DEBUG_MODE
                                    UART2PrintString( "HID: Sending Report Descriptor Parsed event\r\n" );
                                #endif
                                if (USB_HOST_APP_EVENT_HANDLER(deviceInfoHID[i].ID.deviceAddress, EVENT_HID_RPT_DESC_PARSED, NULL, 0 ))
                                {
                                    deviceInfoHID[i].flags.breportDataCollected = 1;
                                }
                                else
                                {
                                    if ((pCurrInterfaceDetails->interfaceNumber == (deviceInfoHID[i].noOfInterfaces-1)) &&
                                        (deviceInfoHID[i].flags.breportDataCollected == 0))
                                    {
                                        #ifdef DEBUG_MODE
                                            UART2PrintString( "HID: Error parsing descriptor\r\n" );
                                        #endif
                                        _USBHostHID_FreeRptDecriptorDataMem(deviceInfoHID[i].ID.deviceAddress);
                                        _USBHostHID_LockDevice( USB_HID_REPORT_DESCRIPTOR_BAD );
                                        USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_BAD_REPORT_DESCRIPTOR, NULL, 0 );
                                    }
                                }
                                USB_FREE_AND_CLEAR(deviceInfoHID[i].rptDescriptor);
                                pCurrInterfaceDetails = pCurrInterfaceDetails->next;

                                if(pCurrInterfaceDetails != NULL)
                                {
                                    if(pCurrInterfaceDetails->sizeOfRptDescriptor !=0)
                                    {
                                        if((deviceInfoHID[i].rptDescriptor = (BYTE *)USB_MALLOC(pCurrInterfaceDetails->sizeOfRptDescriptor)) == NULL)
                                        {
                                            #ifdef DEBUG_MODE
                                                UART2PrintString( "HID: Out of memory\r\n" );
                                            #endif
                                            _USBHostHID_LockDevice( USB_MEMORY_ALLOCATION_ERROR );
                                            break;
                                        }
                                    }
                                    if(USBHostIssueDeviceRequest( deviceInfoHID[i].ID.deviceAddress, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_STANDARD | USB_SETUP_RECIPIENT_INTERFACE,
                                            USB_REQUEST_GET_DESCRIPTOR, DSC_RPT, pCurrInterfaceDetails->interfaceNumber, pCurrInterfaceDetails->sizeOfRptDescriptor, deviceInfoHID[i].rptDescriptor,
                                            USB_DEVICE_REQUEST_GET, deviceInfoHID[i].ID.clientDriverID ))
                                    {
                                        #ifdef DEBUG_MODE
                                            UART2PrintString( "HID: Error getting descriptor\r\n" );
                                        #endif
                                        USB_FREE_AND_CLEAR(deviceInfoHID[i].rptDescriptor);
                                        break;
                                    }
                                    deviceInfoHID[i].state = STATE_WAIT_FOR_REPORT_DSC;
                                }
                                else
                                {
                                    if(deviceInfoHID[i].flags.breportDataCollected == 0)
                                    {
                                        #ifdef DEBUG_MODE
                                            UART2PrintString( "HID: Problem collecting report data\r\n" );
                                        #endif
                                        _USBHostHID_FreeRptDecriptorDataMem(deviceInfoHID[i].ID.deviceAddress);
                                        _USBHostHID_LockDevice( USB_HID_REPORT_DESCRIPTOR_BAD );
                                        USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_BAD_REPORT_DESCRIPTOR, NULL, 0 );
                                    }
                                    else
                                    {
                                        #ifdef DEBUG_MODE
                                            UART2PrintString( "HID: Proceeding to run state\r\n" );
                                        #endif
                                        deviceInfoHID[i].state = STATE_RUNNING;

                                        // Tell the application layer that we have a device.
                                        USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_ATTACH, &(deviceInfoHID[i].ID), sizeof(USB_HID_DEVICE_ID) );
                                    }
                                }
                            }
                        }
                        else
                        {
                            // Assuming only a STALL here.  Since it is EP0, we do not have to clear the stall.
                            USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, 0 );
                        }
                        break;

                    case STATE_RUNNING:
                        // These will be events from application issued requests.  Just pass them up.
                        #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
                            transferEventData.dataCount         = ((HOST_TRANSFER_DATA *)data)->dataCount;
                            transferEventData.bErrorCode        = ((HOST_TRANSFER_DATA *)data)->bErrorCode;
                            if (((HOST_TRANSFER_DATA *)data)->bEndpointAddress & 0x80)
                            {
                                USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_READ_DONE, &transferEventData, sizeof(HID_TRANSFER_DATA) );
                            }
                            else
                            {
                                USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_WRITE_DONE, &transferEventData, sizeof(HID_TRANSFER_DATA) );
                            }
                        #endif
                        break;

                    case STATE_READ_REQ_WAIT:
                        #ifdef DEBUG_MODE
                            UART2PrintString( "HID: Read Event\r\n" );
                        #endif
                        if (((HOST_TRANSFER_DATA *)data)->bErrorCode)
                        {
                            if (USB_ENDPOINT_STALLED == ((HOST_TRANSFER_DATA *)data)->bErrorCode)
                            {
                                 USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
                                 deviceInfoHID[i].returnState = STATE_RUNNING;
                                 deviceInfoHID[i].flags.bfReset = 1;
                                 _USBHostHID_ResetStateJump( i );
                            }
                            else
                            {
                                /* Set proper error code as per HID guideline */
                                #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
                                    transferEventData.dataCount         = ((HOST_TRANSFER_DATA *)data)->dataCount;
                                    transferEventData.bErrorCode        = ((HOST_TRANSFER_DATA *)data)->bErrorCode;
                                #endif
                                _USBHostHID_TerminateReadTransfer(((HOST_TRANSFER_DATA *)data)->bErrorCode);
                            }
                        }
                        else
                        {
                            USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
                            deviceInfoHID[i].bytesTransferred = ((HOST_TRANSFER_DATA *)data)->dataCount; /* Can compare with report size and flag error ???*/
                            #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
                                transferEventData.dataCount         = ((HOST_TRANSFER_DATA *)data)->dataCount;
                                transferEventData.bErrorCode        = ((HOST_TRANSFER_DATA *)data)->bErrorCode;
                            #endif
                            _USBHostHID_TerminateReadTransfer(USB_SUCCESS);
                            return TRUE;
                        }
                        break;

                    case STATE_WRITE_REQ_WAIT:
                        #ifdef DEBUG_MODE
                            UART2PrintString( "HID: Write Event\r\n" );
                        #endif
                        if (((HOST_TRANSFER_DATA *)data)->bErrorCode)
                        {
                            if (USB_ENDPOINT_STALLED == ((HOST_TRANSFER_DATA *)data)->bErrorCode)
                            {
                                 USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
                                 deviceInfoHID[i].returnState = STATE_RUNNING ;
                                 deviceInfoHID[i].flags.bfReset = 1;
                                 _USBHostHID_ResetStateJump( i );
                            }
                            else
                            {
                                /* Set proper error code as per HID guideline */
                                #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
                                    transferEventData.dataCount         = ((HOST_TRANSFER_DATA *)data)->dataCount;
                                    transferEventData.bErrorCode        = ((HOST_TRANSFER_DATA *)data)->bErrorCode;
                                #endif
                                _USBHostHID_TerminateWriteTransfer(((HOST_TRANSFER_DATA *)data)->bErrorCode);
                            }
                        }
                        else
                        {
                            USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
                            deviceInfoHID[i].bytesTransferred = ((HOST_TRANSFER_DATA *)data)->dataCount; /* Can compare with report size and flag error ???*/
                            #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
                                transferEventData.dataCount         = ((HOST_TRANSFER_DATA *)data)->dataCount;
                                transferEventData.bErrorCode        = ((HOST_TRANSFER_DATA *)data)->bErrorCode;
                            #endif
                            _USBHostHID_TerminateWriteTransfer(USB_SUCCESS);
                            return TRUE;
                        }
                        break;

                    case STATE_WAIT_FOR_RESET:
                        #ifdef DEBUG_MODE
                            UART2PrintString( "HID: Reset Event\r\n" );
                        #endif
                        deviceInfoHID[i].errorCode = ((HOST_TRANSFER_DATA *)data)->bErrorCode;
                        deviceInfoHID[i].flags.bfReset = 0;
                        _USBHostHID_ResetStateJump( i );
                        return TRUE;
                        break;

                    case STATE_HOLDING:
                        return TRUE;
                        break;

                    default:
                        return FALSE;
                }
            #endif

// This addition has not been fully tested yet.  It may be included in the future.
//        #if defined( USB_ENABLE_TRANSFER_EVENT )
//        case EVENT_BUS_ERROR:
//            // We will get this error if we have a problem, like too many NAK's 
//            // on a write, so we know the write failed.
//            switch (deviceInfoHID[i].state)
//            {
//                case STATE_READ_REQ_WAIT:
//                    USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
//                    deviceInfoHID[i].bytesTransferred = 0;
//                    #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
//                        transferEventData.dataCount         = 0;
//                        transferEventData.bErrorCode        = ((HOST_TRANSFER_DATA *)data)->bErrorCode;
//                    #endif
//                    _USBHostHID_TerminateReadTransfer( ((HOST_TRANSFER_DATA *)data)->bErrorCode );
//                    return TRUE;
//                    break;
//                    
//                case STATE_WRITE_REQ_WAIT:
//                    USBHostClearEndpointErrors( deviceInfoHID[i].ID.deviceAddress, deviceInfoHID[i].endpointDATA );
//                    deviceInfoHID[i].bytesTransferred = 0;
//                    #ifdef USB_HID_ENABLE_TRANSFER_EVENT    
//                        transferEventData.dataCount         = 0;
//                        transferEventData.bErrorCode        = ((HOST_TRANSFER_DATA *)data)->bErrorCode;
//                    #endif
//                    _USBHostHID_TerminateWriteTransfer( ((HOST_TRANSFER_DATA *)data)->bErrorCode );
//                    return TRUE;
//                    break;
//            }            
//            break;
//        #endif
            
        case EVENT_SOF:              // Start of frame - NOT NEEDED
        case EVENT_RESUME:           // Device-mode resume received
        case EVENT_SUSPEND:          // Device-mode suspend/idle event received
        case EVENT_RESET:            // Device-mode bus reset received
        case EVENT_STALL:            // A stall has occured
            return TRUE;
            break;

        default:
            return FALSE;
            break;
    }
    return FALSE;
}

/*******************************************************************************
  Function:
    BOOL USBHostHIDInitialize( BYTE address, DWORD flags, BYTE clientDriverID )

  Summary:
    This function is the initialization routine for this client driver.

  Description:
    This function is the initialization routine for this client driver.  It
    is called by the host layer when the USB device is being enumerated.For a 
    HID device we need to look into HID descriptor, interface descriptor and 
    endpoint descriptor.

  Precondition:
    None

  Parameters:
    BYTE address        - Address of the new device
    DWORD flags          - Initialization flags
    BYTE clientDriverID - Client driver identification for device requests

  Return Values:
    TRUE   - We can support the device.
    FALSE  - We cannot support the device.

  Remarks:
    None
*******************************************************************************/
BOOL USBHostHIDInitialize( BYTE address, DWORD flags, BYTE clientDriverID )
{
    BYTE                        *descriptor;
    BOOL                        validConfiguration      = FALSE;
    BOOL                        rptDescriptorfound      = FALSE;
    WORD                        i;
    BYTE                        endpointIN              = 0;
    BYTE                        endpointOUT             = 0;
    BYTE                        device;
    BYTE                        numofinterfaces         = 0;
    BYTE                        temp_i                  = 0;
    USB_HID_INTERFACE_DETAILS   *pNewInterfaceDetails   = NULL;

    #ifdef DEBUG_MODE
        UART2PrintString( "HID: USBHostHIDInitialize(0x" );
        UART2PutHex( flags );
        UART2PrintString( ")\r\n" );
    #endif

    // Find the device in the table.  If it's there, we have already initialized this device.
    for (device = 0; (device < USB_MAX_HID_DEVICES) ; device++)
    {
        if(deviceInfoHID[device].ID.deviceAddress == address)
            return TRUE;
    }

    // See if we have room for another device
    for (device = 0; (device < USB_MAX_HID_DEVICES) && (deviceInfoHID[device].ID.deviceAddress != 0); device++);
    if (device == USB_MAX_HID_DEVICES)
    {
        #ifdef DEBUG_MODE
            UART2PrintString("HID: Out of space\r\n" );
        #endif
        return FALSE;
    }

    // Fill in the VID, PID, and client driver ID. They are not not valid unless deviceAddress is non-zero.
    descriptor = USBHostGetDeviceDescriptor( address );
    deviceInfoHID[device].ID.vid            = ((USB_DEVICE_DESCRIPTOR *)descriptor)->idVendor;
    deviceInfoHID[device].ID.pid            = ((USB_DEVICE_DESCRIPTOR *)descriptor)->idProduct;
    deviceInfoHID[device].ID.clientDriverID = clientDriverID;

    // Get ready to parse the configuration descriptor.
    descriptor = USBHostGetCurrentConfigurationDescriptor( address );

//    pCurrInterfaceDetails = pInterfaceDetails;
    i = 0;

    #ifdef DEBUG_MODE
        UART2PrintString("HID: Checking descriptor " );
        UART2PutDec( descriptor[i+5] );
        UART2PrintString(" ...\r\n" );
    #endif

    // Total no of interfaces
    deviceInfoHID[device].noOfInterfaces = descriptor[i+4] ;

    // Set current configuration to this configuration.  We can change it later.

    // MCHP - Check power requirement

    // Find the next interface descriptor.
    while (i < ((USB_CONFIGURATION_DESCRIPTOR *)descriptor)->wTotalLength)
    {
        #ifdef DEBUG_MODE
            UART2PrintString("HID:  Checking interface...\r\n" );
        #endif
        // See if we are pointing to an interface descriptor.
        if (descriptor[i+1] == USB_DESCRIPTOR_INTERFACE)
        {
            // See if the interface is a HID interface.
            if ((descriptor[i+5] == DEVICE_CLASS_HID)||(descriptor[i+5] == 0))
            {
                deviceInfoHID[device].driverSupported = 1;
                if (deviceInfoHID[device].driverSupported)
                {
                    if ((pNewInterfaceDetails = (USB_HID_INTERFACE_DETAILS*)USB_MALLOC(sizeof(USB_HID_INTERFACE_DETAILS))) == NULL)
                    {
                        #ifdef DEBUG_MODE
                            UART2PrintString("HID: Out of memory for interface details\r\n" );
                        #endif
                        return FALSE;
                    }
                    numofinterfaces ++ ;

                    // Create new entry into interface list
                    if(pInterfaceDetails == NULL)
                    {
                        pInterfaceDetails       = pNewInterfaceDetails;
                        pCurrInterfaceDetails   = pNewInterfaceDetails;
                        pInterfaceDetails->next = NULL;
                    }
                    else
                    {
                        pCurrInterfaceDetails->next             = pNewInterfaceDetails;
                        pCurrInterfaceDetails                   = pNewInterfaceDetails;
                        pCurrInterfaceDetails->next             = NULL;
                    }

//                       USB_FREE_AND_CLEAR( pNewInterfaceDetails );
                    pCurrInterfaceDetails->interfaceNumber   = descriptor[i+2];

                    // Scan for hid descriptors.
                    i += descriptor[i];
                    if(descriptor[i+1] == DSC_HID)
                    {
                        if(descriptor[i+5] == 0)
                        {
                            // At least one report descriptor must be present - flag error
                            #ifdef DEBUG_MODE
                                UART2PrintString("HID: No report descriptor\r\n" );
                            #endif
                            rptDescriptorfound = FALSE;
                        }
                        else
                        {
                            rptDescriptorfound = TRUE;
                            pCurrInterfaceDetails->sizeOfRptDescriptor = ((descriptor[i+7]) |
                                                                          ((descriptor[i+8]) << 8));

                            // Look for IN and OUT endpoints.
                            // MCHP - what if there are no endpoints?
                            endpointIN  = 0;
                            endpointOUT = 0;
                            temp_i = 0;
                            // Scan for endpoint descriptors.
                            i += descriptor[i];
                            while(descriptor[i+1] == USB_DESCRIPTOR_ENDPOINT)
                            {
                                if (descriptor[i+3] == 0x03) // Interrupt
                                {
                                    if (((descriptor[i+2] & 0x80) == 0x80) && (endpointIN == 0))
                                    {
                                        endpointIN = descriptor[i+2];
                                    }
                                    if (((descriptor[i+2] & 0x80) == 0x00) && (endpointOUT == 0))
                                    {
                                        endpointOUT = descriptor[i+2];
                                    }
                                }
                                temp_i = descriptor[i];
                                i += descriptor[i];
                            }

                            i -= temp_i;
//                                if ((endpointIN != 0) || (endpointOUT != 0))
                            // Some devices use EP0 as the OUT endpoint
                            if (endpointIN != 0)
                            {
                                // Initialize the remaining device information.
                                #ifdef DEBUG_MODE
                                    UART2PrintString("HID: Valid device info\r\n" );
                                #endif
                                deviceInfoHID[device].ID.deviceAddress        = address;
//
                                pCurrInterfaceDetails->endpointIN           = endpointIN;
                                pCurrInterfaceDetails->endpointOUT          = endpointOUT;
                                pCurrInterfaceDetails->endpointMaxDataSize  = ((descriptor[i+4]) |
                                                                           (descriptor[i+5] << 8));
                                pCurrInterfaceDetails->endpointPollInterval = descriptor[i+6];
                                validConfiguration = TRUE;

                                /* By default NAK time out is disabled for HID class */
                                /* If timeout is required then pass '1' instead '0' in below function call */
//                                    USBHostSetNAKTimeout( address, endpointIN,  0, USB_NUM_INTERRUPT_NAKS );
//                                    USBHostSetNAKTimeout( address, endpointOUT, 0, USB_NUM_INTERRUPT_NAKS );
                            }
//                            i -= temp_i;

                        }
                    }
                    else
                    {
                        /* HID Descriptor not found */
                    }
                }
            }
        }

        // Jump to the next descriptor in this configuration.
        i += descriptor[i];
    }

//    This check doesn't account for the fact that some of the interface descriptors are for
//    alternate settings of the same interface.  It's not really necessary - sometimes
//    descriptors have errors in them.
//    if(numofinterfaces != deviceInfoHID[device].noOfInterfaces)
//    {
//        #ifdef DEBUG_MODE
//            UART2PrintString("HID: Interface count mismatch\r\n" );
//        #endif
//        validConfiguration = FALSE;
//    }

    if(validConfiguration)
    {
        #ifndef USB_ENABLE_TRANSFER_EVENT
            deviceInfoHID[device].state                = STATE_INITIALIZE_DEVICE;
        #else
            pCurrInterfaceDetails = pInterfaceDetails;
            if (pCurrInterfaceDetails->sizeOfRptDescriptor !=0)
            {
                if (deviceInfoHID[device].rptDescriptor != NULL)
                {
                    USB_FREE_AND_CLEAR( deviceInfoHID[device].rptDescriptor );
                }

                if((deviceInfoHID[device].rptDescriptor = (BYTE *)USB_MALLOC(pCurrInterfaceDetails->sizeOfRptDescriptor)) == NULL)
                {
                    #ifdef DEBUG_MODE
                        UART2PrintString("HID: Out of memory for report descriptor\r\n" );
                    #endif
                    return FALSE;
                }
            }
            if (USBHostIssueDeviceRequest( deviceInfoHID[device].ID.deviceAddress, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_STANDARD | USB_SETUP_RECIPIENT_INTERFACE,
                 USB_REQUEST_GET_DESCRIPTOR, DSC_RPT, pCurrInterfaceDetails->interfaceNumber, pCurrInterfaceDetails->sizeOfRptDescriptor, deviceInfoHID[device].rptDescriptor,
                 USB_DEVICE_REQUEST_GET, deviceInfoHID[device].ID.clientDriverID ))
            {
                #ifdef DEBUG_MODE
                    UART2PrintString("HID: Error getting report descriptor\r\n" );
                #endif
                USB_FREE_AND_CLEAR(deviceInfoHID[device].rptDescriptor);
                return FALSE;
            }
            #ifdef DEBUG_MODE
                UART2PrintString("HID: Getting report descriptor\r\n" );
            #endif
            deviceInfoHID[device].state                = STATE_WAIT_FOR_REPORT_DSC;
        #endif

        #ifdef DEBUG_MODE
            UART2PrintString( "HID: Interrupt endpoint IN: " );
            UART2PutHex( endpointIN );
            UART2PrintString( " Interrupt endpoint OUT: " );
            UART2PutHex( endpointOUT );
            UART2PrintString( "\r\n" );
        #endif

        return TRUE;
    }
    else
    {
       return FALSE;
    }
}


// *****************************************************************************
// *****************************************************************************
// Internal Functions
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
  Function:
    void _USBHostHID_FreeRptDecriptorDataMem(BYTE deviceAddress)

  Summary:


  Description:
    This function is called in case of error is encountered . This function
    frees the memory allocated for report descriptor and interface
    descriptors.

  Precondition:
    None.

  Parameters:
    BYTE address        - Address of the device

  Returns:
    None

  Remarks:
    None
***************************************************************************/
void _USBHostHID_FreeRptDecriptorDataMem(BYTE deviceAddress)
{
    USB_HID_INTERFACE_DETAILS*           ptempInterface;
    BYTE    i;

    // Find the device in the table.  If found, clear the important fields.
    for (i=0; (i<USB_MAX_HID_DEVICES) && (deviceInfoHID[i].ID.deviceAddress != deviceAddress); i++);
    if (i < USB_MAX_HID_DEVICES)
    {
        if(deviceInfoHID[i].rptDescriptor != NULL)
        {
            USB_FREE_AND_CLEAR(deviceInfoHID[i].rptDescriptor);
        }

    /* free memory allocated to report descriptor in deviceInfoHID */
       while(pInterfaceDetails != NULL)
        {
            ptempInterface = pInterfaceDetails->next;
            USB_FREE_AND_CLEAR(pInterfaceDetails);
            pInterfaceDetails = ptempInterface;
        }
    }
}

/*******************************************************************************
  Function:
    void _USBHostHID_ResetStateJump( BYTE i )

  Summary:

  Description:
    This function determines which portion of the reset processing needs to
    be executed next and jumps to that state.

Precondition:
    The device information must be in the deviceInfoHID array.

  Parameters:
    BYTE i  - Index into the deviceInfoHIDMSD structure for the device to reset.

  Returns:
    None

  Remarks:
    None
*******************************************************************************/
void _USBHostHID_ResetStateJump( BYTE i )
{
    BYTE    errorCode;

    if (deviceInfoHID[i].flags.bfReset)
    {
         errorCode = USBHostIssueDeviceRequest( deviceInfoHID[i].ID.deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE,
                        USB_HID_RESET, 0, deviceInfoHID[i].interface, 0, NULL, USB_DEVICE_REQUEST_SET, deviceInfoHID[i].ID.clientDriverID );

        if (errorCode)
        {
            deviceInfoHID[i].errorCode    = USB_HID_RESET_ERROR;
            deviceInfoHID[i].state        = STATE_RUNNING;
            USB_HOST_APP_EVENT_HANDLER( deviceInfoHID[i].ID.deviceAddress, EVENT_HID_RESET_ERROR, NULL, 0 );
        }
        else
        {
            #ifndef USB_ENABLE_TRANSFER_EVENT
                deviceInfoHID[i].state = STATE_HID_RESET_RECOVERY | SUBSTATE_WAIT_FOR_RESET;
            #else
                deviceInfoHID[i].state = STATE_WAIT_FOR_RESET;
            #endif
        }
    }
    else
    {
        if (!deviceInfoHID[i].errorCode)
        {
            USB_HOST_APP_EVENT_HANDLER(deviceInfoHID[i].ID.deviceAddress, EVENT_HID_RESET, NULL, 0 );
        }
        else
        {
            USB_HOST_APP_EVENT_HANDLER(deviceInfoHID[i].ID.deviceAddress, EVENT_HID_RESET_ERROR, NULL, 0 );
        }        

        deviceInfoHID[i].state = deviceInfoHID[i].returnState;
    }
}


