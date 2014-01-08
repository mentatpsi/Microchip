/******************************************************************************

  USB Host Communication Device Class(CDC) driver

This is the Communication device Class driver file for a USB Embedded Host device.
This file should be used in a project with usb_host.c to provide the USB
hardware interface.

Acronyms/abbreviations used by this class:
    * CDC - Communication Device Class
    * ACM - Abstract Control Module

To interface with usb_host.c, the routine USBHostCDCInitialize() should be
specified as the Initialize() function, and USBHostCDCEventHandler() should
be specified as the EventHandler() function in the usbClientDrvTable[] array
declared in usb_config.h.

This driver can be configured to use transfer events from usb_host.c.  Transfer
events require more RAM and ROM than polling, but it cuts down or even
eliminates the required polling of the various USBxxxTasks functions.  For this
class, USBHostCDCTasks() is compiled out if transfer events from usb_host.c
are used.  However, USBHostTasks() still must be called to provide attach,
enumeration, and detach services.  If transfer events from usb_host.c
are going to be used, USB_ENABLE_TRANSFER_EVENT should be defined.  If transfer
status is going to be polled, USB_ENABLE_TRANSFER_EVENT should not be defined.
CDC client layer transfer events are also enables once USB_ENABLE_TRANSFER_EVENT
is defined. The application must define USB_HOST_APP_EVENT_HANDLER in file usb_config.h.
All the CDC client layer event are called back using this function handler. This eliminates
the need of polling for transfer status by the application, however handing of these events
should be taken care by the application.

Transfer of Data Class Interface can be performed with BULK transfers,
hence USB_SUPPORT_BULK_TRANSFERS must be defined. Data Class Interface can also use
ISOCHRONOUS transfers,however the CDC client is not tested for ISOCHRONOUS transfers.

* FileName:        usb_host_cdc.c
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
ADG          15-Sep-2008 First release
********************************************************************************
 Change History:
 Revision     Description
 v2.6         Removed state machine 'SUBSTATE_SET_CONTROL_LINE_STATE' from normal
              enumeration process.
              Modified code to accommodate CDC devices that do not have distinct
              Communication and Data interfaces.
 v2.7         Modified the code to allow connection of USB-RS232 dongles that do
              not fully comply with CDC specifications
              Modified API USBHostCDC_Api_Send_OUT_Data to allow data transfers
              more than 256 bytes
 v2.9         Fixed bug with sending SET_CONTROL_LINE_STATE command to device.
              The bit map settings were being sent as data stage bytes, rather
			  than as the wValue of the SETUP packet.
********************************************************************************/


#include <stdlib.h>
#include <string.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_cdc.h"
#include "USB/usb_host.h"

//#define DEBUG_MODE
#ifdef DEBUG_MODE
    #include "uart2.h"
#endif

#if !defined(USB_NUM_BULK_NAKS)
    #define USB_HOST_CDC_NAK_TIMEOUT 1
#else
    #define USB_HOST_CDC_NAK_TIMEOUT 0
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
#ifndef USB_MAX_CDC_DEVICES
    #define USB_MAX_CDC_DEVICES        1
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

#define STATE_MASK                                0x00F0 //
#define SUBSTATE_MASK                             0x000F //
                                                   
#define NEXT_STATE                                0x0010 //
#define NEXT_SUBSTATE                             0x0001 //


#define STATE_DETACHED                            0x0000 //
                                                   
#define STATE_INITIALIZE_DEVICE                   0x0010 //
#define SUBSTATE_WAIT_FOR_ENUMERATION             0x0000 //
#define SUBSTATE_DEVICE_ENUMERATED                0x0001 //
#define SUBSTATE_GET_LINE_CODING                  0x0002 //
#define SUBSTATE_WAIT_FOR_GET_LINE_CODING         0x0003 //
#define SUBSTATE_SET_LINE_CODING                  0x0004 //
#define SUBSTATE_WAIT_FOR_SET_LINE_CODING         0x0005 //
#define SUBSTATE_SET_CONTROL_LINE_STATE           0x0006 //
#define SUBSTATE_WAIT_FOR_SET_CONTROL_LINE_STATE  0x0007 //


#define STATE_RUNNING                             0x0020 //
#define SUBSTATE_WAITING_FOR_REQ                  0x0000 //
#define SUBSTATE_SEND_READ_REQ                    0x0001 //
#define SUBSTATE_READ_REQ_WAIT                    0x0002 //
#define SUBSTATE_SEND_WRITE_REQ                   0x0003 //
#define SUBSTATE_WRITE_REQ_WAIT                   0x0004 //

#define STATE_CDC_RESET_RECOVERY                  0x0030 //
#define SUBSTATE_SEND_RESET                       0x0000 //
#define SUBSTATE_WAIT_FOR_RESET                   0x0001 //
#define SUBSTATE_RESET_COMPLETE                   0x0002 //
                                                  
#define STATE_HOLDING                             0x0040 //


#else


#define STATE_DETACHED                            0x0000 //

#define STATE_INITIALIZE_DEVICE                   0x0001 //
#define STATE_WAIT_FOR_GET_LINE_CODING            0x0002 //
#define STATE_WAIT_FOR_SET_LINE_CODING            0x0003 //
#define STATE_WAIT_FOR_SET_CONTROL_LINE_STATE     0x0004 //

#define STATE_RUNNING                             0x0005 //
#define STATE_READ_REQ_WAIT                       0x0006 //
#define STATE_WRITE_REQ_WAIT                      0x0007 //

#define STATE_WAIT_FOR_RESET                      0x0008 //
#define STATE_RESET_COMPLETE                      0x0009 //

#define STATE_HOLDING                             0x000A //

#endif

// *****************************************************************************
// Other Constants
// *****************************************************************************

#define USB_CDC_RESET           (0xFF)  // Device Request code to reset the device.
#define MARK_RESET_RECOVERY     (0x0E)  // 


// Default values for RS232 configuration data
#ifndef USB_CDC_BAUDRATE_SUPPORTED 
    #define USB_CDC_BAUDRATE_SUPPORTED 19200UL      // default BAUDRATE for the device
#endif

#ifndef USB_CDC_PARITY_TYPE 
    #define USB_CDC_PARITY_TYPE 0                   // default Parity type
#endif

#ifndef USB_CDC_STOP_BITS 
    #define USB_CDC_STOP_BITS 0                     // default Stop bits
#endif

#ifndef USB_CDC_NO_OF_DATA_BITS 
    #define USB_CDC_NO_OF_DATA_BITS 8               // default value of data width
#endif

//******************************************************************************
//******************************************************************************
// Data Structures
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
// Section: Local Prototypes
//******************************************************************************
//******************************************************************************
void _USBHostCDC_ResetStateJump( BYTE i );
void USBHostCDC_Init_CDC_Buffers(void);

//******************************************************************************
//******************************************************************************
// Macros
//******************************************************************************
//******************************************************************************

#ifndef USB_ENABLE_TRANSFER_EVENT

#define _USBHostCDC_SetNextState()              { deviceInfoCDC[i].state = (deviceInfoCDC[i].state & STATE_MASK) + NEXT_STATE; }
#define _USBHostCDC_SetNextSubState()           { deviceInfoCDC[i].state += NEXT_SUBSTATE; }
#define _USBHostCDC_TerminateTransfer( error )  {                                                                       \
                                                    deviceInfoCDC[i].errorCode    = error;                                 \
                                                    deviceInfoCDC[i].state        = STATE_RUNNING;\
                                                }

#else
    #define _USBHostCDC_TerminateTransfer( error )  {                                                                       \
                                                        deviceInfoCDC[i].errorCode    = error;                                 \
                                                        deviceInfoCDC[i].state        = STATE_RUNNING;\
                                                    }
#endif


//******************************************************************************
//******************************************************************************
// Section: CDC Host Global Variables
//******************************************************************************
//******************************************************************************

USB_CDC_LINE_CODING           CDC_DEV_LINE_CODING_Buffer; // Stores LINE CODING data received from device
USB_CDC_LINE_CODING           CDC_LINE_CODING_Buffer;     // Stores LINE CODING data configured at host
USB_CDC_CONTROL_SIGNAL_BITMAP CDC_CONTROL_SIGNAL_Buffer;  // Stores CONTROL SIGNAL data configured at host

USB_CDC_DEVICE_INFO           deviceInfoCDC[USB_MAX_CDC_DEVICES] __attribute__ ((aligned)); // This structure holds all the information of device collected during enumeration

BYTE CDCdeviceAddress = 0; // Holds address of the attached device

//******************************************************************************
//******************************************************************************
// Section: CDC Host External Variables
//******************************************************************************
//******************************************************************************

#define PTR_HOST_LINE_CODING_BUFFER  (&CDC_LINE_CODING_Buffer)    // Address of Line Coding buffer
#define PTR_HOST_CONTROL_LINE_BUFFER (&CDC_CONTROL_SIGNAL_Buffer) // Address of control signal buffer


extern CLIENT_DRIVER_TABLE usbDeviceInterfaceTable;

//******************************************************************************
//******************************************************************************
// Section: CDC Host Global Variables
//******************************************************************************
//******************************************************************************

// *****************************************************************************
// *****************************************************************************
// CDC Host Stack Callback Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    BOOL USBHostCDCInitAddress( BYTE address, DWORD flags, BYTE clientDriverID )

  Precondition:
    The device has been enumerated without any errors.

  Summary:
    This function intializes the address of the attached CDC device.

  Description:
    This function intializes the address of the attached CDC device. Once the
    device is enumerated without any errors, the CDC client call this function.
    For all the transfer requesets this address is used to indentify the CDC
    device.


  Parameters:
    BYTE address    -   Address of the new device
    DWORD flags     -   Initialization flags
    BYTE clientDriverID - Client driver identification for device requests

  Return Values:
    TRUE    -   We can support the device.
    FALSE   -   We cannot support the device.

  Remarks:
    None
*******************************************************************************/
BOOL USBHostCDCInitAddress( BYTE address, DWORD flags, BYTE clientDriverID )
{
    #ifndef MINIMUM_BUILD
//        UART2PrintString( "CDC: Device attached.\r\n" );
    #endif

    if (CDCdeviceAddress == 0)
    {
        // Save the address of the new device.
        CDCdeviceAddress = address;
        return TRUE;
    }
    else
    {
        // We can currently only handle one device.
        return FALSE;
    }        
}    


// *****************************************************************************
// *****************************************************************************
// Application Callable Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    BYTE    USBHostCDCDeviceStatus( BYTE deviceAddress )

  Summary:

  Description:
    This function determines the status of a CDC device.

  Preconditions:  None

  Parameters:
    BYTE deviceAddress - address of device to query

  Return Values:
    USB_CDC_DEVICE_NOT_FOUND           -  Illegal device address, or the
                                          device is not an CDC
    USB_CDC_INITIALIZING               -  CDC is attached and in the
                                          process of initializing
    USB_PROCESSING_REPORT_DESCRIPTOR   -  CDC device is detected and report 
                                          descriptor is being parsed
    USB_CDC_NORMAL_RUNNING             -  CDC Device is running normal, 
                                          ready to send and receive reports 
    USB_CDC_DEVICE_HOLDING             -  CDC device could not recover from the error
    USB_CDC_DEVICE_DETACHED            -  CDC detached.

  Remarks:
    None
*******************************************************************************/
BYTE    USBHostCDCDeviceStatus( BYTE deviceAddress )
{
    BYTE    i;
    BYTE    status;

    // Find the correct device.
    for (i=0; (i<USB_MAX_CDC_DEVICES) && (deviceInfoCDC[i].deviceAddress != deviceAddress); i++);
    if (i == USB_MAX_CDC_DEVICES)
    {
        return USB_CDC_DEVICE_NOT_FOUND;
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
           switch (deviceInfoCDC[i].state & STATE_MASK)
           {
               case STATE_INITIALIZE_DEVICE:
                   return USB_CDC_INITIALIZING;
                   break;

               case STATE_RUNNING:
                   return USB_CDC_NORMAL_RUNNING;
                   break;
               case STATE_HOLDING:
                   return USB_CDC_DEVICE_HOLDING;
                   break;

               case STATE_CDC_RESET_RECOVERY:
                   return USB_CDC_RESETTING_DEVICE;
                   break;
                   
               default:  
                   return USB_CDC_DEVICE_DETACHED;  
                   break;
           }
        #else
           switch (deviceInfoCDC[i].state)
           {
               case STATE_INITIALIZE_DEVICE:
                   return USB_CDC_INITIALIZING;
                   break;

               case STATE_RUNNING:
               case STATE_READ_REQ_WAIT:
               case STATE_WRITE_REQ_WAIT:
                   return USB_CDC_NORMAL_RUNNING;
                   break;

               case STATE_HOLDING:
                   return USB_CDC_DEVICE_HOLDING;
                   break;
                   
               case STATE_WAIT_FOR_RESET:
               case STATE_RESET_COMPLETE:
                   return USB_CDC_RESETTING_DEVICE;
                   break;

               default:  
                   return USB_CDC_DEVICE_DETACHED;  
                   break;
           }
        #endif
    }    
}
    
/*******************************************************************************
  Function:
    BYTE USBHostCDCResetDevice( BYTE deviceAddress )

  Summary:
    This function starts a CDC  reset.

  Description:
    This function starts a CDC reset.  A reset can be
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
BYTE USBHostCDCResetDevice( BYTE deviceAddress )
{
    BYTE    i;

    // Make sure a valid device is being requested.
    if ((deviceAddress == 0) || (deviceAddress > 127))
    {
        return USB_CDC_DEVICE_NOT_FOUND;
    }

    // Find the correct device.
    for (i=0; (i<USB_MAX_CDC_DEVICES) && (deviceInfoCDC[i].deviceAddress != deviceAddress); i++);
    if (i == USB_MAX_CDC_DEVICES)
    {
        return USB_CDC_DEVICE_NOT_FOUND;
    }

     #ifndef USB_ENABLE_TRANSFER_EVENT
       if (((deviceInfoCDC[i].state & STATE_MASK) != STATE_DETACHED) &&
            ((deviceInfoCDC[i].state & STATE_MASK) != STATE_INITIALIZE_DEVICE))
     #else
         if ((deviceInfoCDC[i].state != STATE_DETACHED) &&
             (deviceInfoCDC[i].state != STATE_INITIALIZE_DEVICE))
     #endif
        {
            deviceInfoCDC[i].flags.val |= MARK_RESET_RECOVERY;
            deviceInfoCDC[i].flags.bfReset = 1;
        #ifndef USB_ENABLE_TRANSFER_EVENT
            deviceInfoCDC[i].returnState = STATE_HOLDING;
        #else
            deviceInfoCDC[i].returnState = STATE_RUNNING;
        #endif
        
            _USBHostCDC_ResetStateJump( i );
            return USB_SUCCESS;
        }
        return USB_CDC_ILLEGAL_REQUEST;
}


/*******************************************************************************
  Function:
     void USBHostCDCTasks( void )

  Summary:
    This function performs the maintenance tasks required by CDC class

  Description:
    This function performs the maintenance tasks required by the CDC
    class.  If transfer events from the host layer are not being used, then
    it should be called on a regular basis by the application.  If transfer
    events from the host layer are being used, this function is compiled out,
    and does not need to be called.

  Precondition:
    USBHostCDCInitialize() has been called.

  Parameters:
    None - None

  Returns:
    None

  Remarks:
    None
*******************************************************************************/
void USBHostCDCTasks( void )
{
#ifndef USB_ENABLE_TRANSFER_EVENT
    DWORD   byteCount;
    BYTE    errorCode;
    BYTE    i;

//    BYTE    temp;

    for (i=0; i<USB_MAX_CDC_DEVICES; i++)
    {
        if (deviceInfoCDC[i].deviceAddress == 0) /* device address updated by lower layer */
        {
            deviceInfoCDC[i].state = STATE_DETACHED;
        }

        switch (deviceInfoCDC[i].state & STATE_MASK)
        {
            case STATE_DETACHED:
                // No device attached.
                break;

            case STATE_INITIALIZE_DEVICE:
                switch (deviceInfoCDC[i].state & SUBSTATE_MASK)
                {
                    case SUBSTATE_WAIT_FOR_ENUMERATION:
                        if (USBHostDeviceStatus( deviceInfoCDC[i].deviceAddress ) == USB_DEVICE_ATTACHED)
                        {
                            _USBHostCDC_SetNextSubState();
                        }
                        break;

                    case SUBSTATE_DEVICE_ENUMERATED:
                        USBHostCDCInitAddress( deviceInfoCDC[i].deviceAddress,0, deviceInfoCDC[i].clientDriverID ); // Initialize device address for application use 
                       _USBHostCDC_SetNextSubState(); /* need to add sub states to Set Config, Get LANGID & String Descriptors */
                        break;


                   case SUBSTATE_GET_LINE_CODING:
                         
                       if(!USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                            USB_CDC_GET_LINE_CODING, 0 , deviceInfoCDC[i].commInterface.interfaceNum , USB_CDC_LINE_CODING_LENGTH, (BYTE*)&CDC_DEV_LINE_CODING_Buffer,
                                            USB_DEVICE_REQUEST_GET , deviceInfoCDC[i].clientDriverID ))
                       {           
                            _USBHostCDC_SetNextSubState();
                       }
                        break;

                    case SUBSTATE_WAIT_FOR_GET_LINE_CODING:   
                        if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, 0, &errorCode, &byteCount ))
                        {
                            if (errorCode)
                            {
                                /* Set error code */
                                _USBHostCDC_TerminateTransfer(errorCode);
                            }
                            else
                            {
                                // Clear the STALL.  Since it is EP0, we do not have to clear the stall.
                                USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, 0 );
                                // Compare with Line coding expected at Host end with the one received from device
                                if(0 == memcmp(&CDC_DEV_LINE_CODING_Buffer, PTR_HOST_LINE_CODING_BUFFER, USB_CDC_LINE_CODING_LENGTH))
                                {
                                    // if fine goto set  Control line state
 //                                   deviceInfoCDC[i].state = STATE_INITIALIZE_DEVICE | SUBSTATE_SET_CONTROL_LINE_STATE;
                                      deviceInfoCDC[i].state = STATE_RUNNING;
                                }
                                else
                                {
                                    // if data not as expected goto Set Line coding
                                    deviceInfoCDC[i].state = STATE_INITIALIZE_DEVICE | SUBSTATE_SET_LINE_CODING;
                                }
                            }
                        }
                        break;

                   case SUBSTATE_SET_LINE_CODING:
                         
                       if(!USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                            USB_CDC_SET_LINE_CODING, 1 , deviceInfoCDC[i].commInterface.interfaceNum , USB_CDC_LINE_CODING_LENGTH, (BYTE*)PTR_HOST_LINE_CODING_BUFFER,
                                            USB_DEVICE_REQUEST_SET , deviceInfoCDC[i].clientDriverID ))
                       {           
                            _USBHostCDC_SetNextSubState();
                       }
                        break;

                    case SUBSTATE_WAIT_FOR_SET_LINE_CODING:   
                        if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, 0, &errorCode, &byteCount ))
                        {
                            if (errorCode)
                            {
                                /* Set error code */
                                _USBHostCDC_TerminateTransfer(errorCode);
                            }
                            else
                            {    // check again if line coding is correctly set
                                 deviceInfoCDC[i].state = STATE_INITIALIZE_DEVICE | SUBSTATE_GET_LINE_CODING;
                            }
                        }
                        break;

                   case SUBSTATE_SET_CONTROL_LINE_STATE:
                         
                       if(!USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                            USB_CDC_SET_CONTROL_LINE_STATE , *(WORD*)PTR_HOST_CONTROL_LINE_BUFFER , deviceInfoCDC[i].commInterface.interfaceNum , USB_CDC_CONTROL_LINE_LENGTH, NULL,
                                            USB_DEVICE_REQUEST_SET , deviceInfoCDC[i].clientDriverID ))
                       {           
                            _USBHostCDC_SetNextSubState();
                       }
                        break;

                    case SUBSTATE_WAIT_FOR_SET_CONTROL_LINE_STATE:   
                        if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, 0, &errorCode, &byteCount ))
                        {
                            if (errorCode)
                            {
                                /* Set error code */
                                _USBHostCDC_TerminateTransfer(errorCode);
                            }
                            else
                            {
                                _USBHostCDC_SetNextState();
                            }
                        }
                        break;

                    default : 
                        break;
                }
                break;

            case STATE_RUNNING:
                switch (deviceInfoCDC[i].state & SUBSTATE_MASK)
                {
                    case SUBSTATE_WAITING_FOR_REQ:   
                              /* waiting for request from application */
                        break;

                    case SUBSTATE_SEND_READ_REQ:   
                            // if endpoint 0 then use control transfer - Communication interface request
                            // otherwise request is for data interface transfer
                            USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );

                            if(deviceInfoCDC[i].endpointDATA == 0x00)
                            {
                                errorCode   = USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                                                    deviceInfoCDC[i].commRequest, 0, deviceInfoCDC[i].interface,deviceInfoCDC[i].reportSize, deviceInfoCDC[i].userData,
                                                                    USB_DEVICE_REQUEST_GET , deviceInfoCDC[i].clientDriverID );
                            }
                            else
                            {
                                errorCode = USBHostRead( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA,
                                                  deviceInfoCDC[i].userData, deviceInfoCDC[i].reportSize );
                            }

                            if (errorCode)
                            {
                                if(USB_ENDPOINT_STALLED == errorCode)
                                {
                                     USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                     deviceInfoCDC[i].returnState = STATE_HOLDING;
                                     deviceInfoCDC[i].flags.bfReset = 1;
                                     _USBHostCDC_ResetStateJump( i );
                                }
                                else
                                {
                                    _USBHostCDC_TerminateTransfer(errorCode);
                                }
                            }
                            else
                            {
                                _USBHostCDC_SetNextSubState();
                            }
                        break;

                    case SUBSTATE_READ_REQ_WAIT:   
                              if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA, &errorCode, &byteCount ))
                              {
                                if (errorCode)
                                {
                                    if(USB_ENDPOINT_STALLED == errorCode)
                                    {
                                         USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                         deviceInfoCDC[i].returnState = STATE_HOLDING;
                                         deviceInfoCDC[i].flags.bfReset = 1;
                                         _USBHostCDC_ResetStateJump( i );
                                    }else if(USB_ENDPOINT_NAK_TIMEOUT == errorCode)
                                    {
                                        USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_CDC_NAK_TIMEOUT,NULL, 0);
                                        deviceInfoCDC[i].state = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
                                        USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                    }
                                    else
                                    {
                                        _USBHostCDC_TerminateTransfer(errorCode);
                                    }
                                }
                                else
                                {
                                    // Clear the STALL.  Since it is EP0, we do not have to clear the stall.
                                    USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                    deviceInfoCDC[i].bytesTransferred = byteCount; /* Can compare with report size and flag error ???*/
                                    deviceInfoCDC[i].state = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
                                }
                              }
#ifdef DEBUG_MODE
                              UART2PrintString("|");
#endif                                
                        break;

                    case SUBSTATE_SEND_WRITE_REQ:   

                            // if endpoint 0 then use control transfer - Communication interface request
                            // otherwise request is for data interface transfer
                              if(deviceInfoCDC[i].endpointDATA == 0x00)
                              {
                                  errorCode   = USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                               deviceInfoCDC[i].commRequest, 0, deviceInfoCDC[i].interface,deviceInfoCDC[i].reportSize, deviceInfoCDC[i].userData,
                                               USB_DEVICE_REQUEST_SET , deviceInfoCDC[i].clientDriverID );
                              }
                              else
                              {   // if transfer size is more than 64 Bytes, multiple transactions are used to transfer the data
                                  if(deviceInfoCDC[i].remainingBytes > USB_CDC_MAX_PACKET_SIZE)
                                  {
                                    deviceInfoCDC[i].bytesTransferred = USB_CDC_MAX_PACKET_SIZE; // max transaction size is 64 bytes
                                    deviceInfoCDC[i].remainingBytes -= USB_CDC_MAX_PACKET_SIZE;
                                    errorCode = USBHostWrite( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA,
                                                    deviceInfoCDC[i].userData, deviceInfoCDC[i].bytesTransferred );
                                    deviceInfoCDC[i].userData = deviceInfoCDC[i].userData + USB_CDC_MAX_PACKET_SIZE;
                                  }
                                  else
                                  {
                                    deviceInfoCDC[i].bytesTransferred = deviceInfoCDC[i].remainingBytes;
                                    deviceInfoCDC[i].remainingBytes = 0;
                                    errorCode = USBHostWrite( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA,
                                                    deviceInfoCDC[i].userData, deviceInfoCDC[i].bytesTransferred );
                                  }

                              }

                              if (errorCode)
                              {
                                  if(USB_ENDPOINT_STALLED == errorCode)
                                  {
                                       USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                       deviceInfoCDC[i].returnState = STATE_HOLDING;
                                       deviceInfoCDC[i].flags.bfReset = 1;
                                       _USBHostCDC_ResetStateJump( i );
                                  }
                                  else
                                  {
                                      _USBHostCDC_TerminateTransfer(errorCode);
                                  }
                              }
                              else
                              {
                                  _USBHostCDC_SetNextSubState();
                              }

                        break;
                    case SUBSTATE_WRITE_REQ_WAIT:   
                              if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA, &errorCode, &byteCount ))
                              {
                                if (errorCode)
                                {
                                    if(USB_ENDPOINT_STALLED == errorCode)
                                    {
                                         USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                         deviceInfoCDC[i].returnState = STATE_HOLDING;
                                         deviceInfoCDC[i].flags.bfReset = 1;
                                         _USBHostCDC_ResetStateJump( i );
                                    }
                                    else
                                    {
                                        _USBHostCDC_TerminateTransfer(errorCode);
                                    }
                                }
                                else
                                {
                                    USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                    // this is to check if there are any remaining data bytes to be transferred
                                    if(deviceInfoCDC[i].remainingBytes != 0)
                                    {
                                        // set up another OUT request
                                        deviceInfoCDC[i].state = STATE_RUNNING | SUBSTATE_SEND_WRITE_REQ;
                                    }
                                    else
                                    {
                                        // else goto wait state
                                        deviceInfoCDC[i].state = STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ;
                                    }
                                }
                              }

                        break;

                    default : 
                        break;
                }
                break;

            case STATE_CDC_RESET_RECOVERY:
                switch (deviceInfoCDC[i].state & SUBSTATE_MASK)
                {
                    case SUBSTATE_SEND_RESET:   /* Not sure of rest request */
                            errorCode = USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE,
                                            USB_CDC_RESET, 0, deviceInfoCDC[i].interface, 0, NULL, USB_DEVICE_REQUEST_SET , deviceInfoCDC[i].clientDriverID );

                            if (errorCode)
                            {
                                    if(USB_ENDPOINT_STALLED == errorCode)
                                    {
                                         USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, 0 );
                                         deviceInfoCDC[i].returnState = STATE_HOLDING;
                                         deviceInfoCDC[i].flags.bfReset = 1;
                                         _USBHostCDC_ResetStateJump( i );
                                    }
                                    else
                                    {
                                        _USBHostCDC_TerminateTransfer(errorCode);
                                    }
                            }
                            else
                            {
                                _USBHostCDC_SetNextSubState();
                            }
                        break;
                    case SUBSTATE_WAIT_FOR_RESET:
                            if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, 0, &errorCode, &byteCount ))
                            {
                                if (errorCode)
                                {
                                    if(USB_ENDPOINT_STALLED == errorCode)
                                    {
                                         USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, 0 );
                                         deviceInfoCDC[i].returnState = STATE_HOLDING;
                                         deviceInfoCDC[i].flags.bfReset = 1;
                                         _USBHostCDC_ResetStateJump( i );
                                    }
                                    else
                                    {
                                        _USBHostCDC_TerminateTransfer(errorCode);
                                    }
                                }
                                else
                                {
                                    deviceInfoCDC[i].flags.bfReset = 0;
                                    _USBHostCDC_ResetStateJump( i );
                                }
                            }
                            break;

                    case SUBSTATE_RESET_COMPLETE:
                            _USBHostCDC_ResetStateJump( i );
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
    USBHostCDCTransfer( BYTE deviceAddress, BYTE direction, BYTE reportid,
                        BYTE size, BYTE *data)

  Summary:
    This function starts a CDC transfer.

  Description:
    This function starts a CDC transfer. A read/write wrapper is provided in
    application interface file to access this function. This function is used for
    transfer requests over both Communication interface and data interface.

  Preconditions:
    None

  Parameters:
    BYTE deviceAddress      - Device address
    BYTE direction          - 1=read, 0=write
    BYTE interfaceNum       - Interface number of the device 
    BYTE size               - Byte size of the data buffer
    BYTE *data              - Pointer to the data buffer
    BYTE endpointDATA       - Endpoint details over which transfer is requested

  Return Values:
    USB_SUCCESS                 - Request started successfully
    USB_CDC_DEVICE_NOT_FOUND    - No device with specified address
    USB_CDC_DEVICE_BUSY         - Device not in proper state for
                                  performing a transfer
  Remarks:
    None
*******************************************************************************/

BYTE USBHostCDCTransfer( BYTE deviceAddress,BYTE request , BYTE direction, BYTE interfaceNum, WORD size, BYTE *data , BYTE endpointDATA)
{
    BYTE    i;
    #ifdef USB_ENABLE_TRANSFER_EVENT
    BYTE    errorCode;
    #endif
    // Find the correct device.
    for (i=0; (i<USB_MAX_CDC_DEVICES) && (deviceInfoCDC[i].deviceAddress != deviceAddress); i++);
    if (i == USB_MAX_CDC_DEVICES)
    {
        return USB_CDC_DEVICE_NOT_FOUND;
    }

    // Make sure the device is in a state ready to read/write.
    #ifndef USB_ENABLE_TRANSFER_EVENT
        if (deviceInfoCDC[i].state != (STATE_RUNNING) &&
           (deviceInfoCDC[i].state & SUBSTATE_MASK) != (SUBSTATE_WAITING_FOR_REQ))
    #else
        if (deviceInfoCDC[i].state != STATE_RUNNING)
    #endif
        {
            return USB_CDC_DEVICE_BUSY;
        }
     
    // Initialize the transfer information.
    deviceInfoCDC[i].bytesTransferred  = 0;
    deviceInfoCDC[i].errorCode         = USB_SUCCESS;
    deviceInfoCDC[i].userData          = data;
    deviceInfoCDC[i].reportSize        = size;
    deviceInfoCDC[i].remainingBytes    = size;
    deviceInfoCDC[i].interface         = interfaceNum;
    deviceInfoCDC[i].endpointDATA      = endpointDATA;
    deviceInfoCDC[i].commRequest       = request;       // invalid entry if DATA transfer is requested
    #ifdef DEBUG_MODE
        UART2PrintString( "Data EP: " );
        UART2PutHex( deviceInfoCDC[i].endpointDATA );
        UART2PrintString( "\r\n" );
    #endif
    
    #ifndef USB_ENABLE_TRANSFER_EVENT
        // Jump to the transfer state.
        if(!direction)
            {
                /* send write req */
                deviceInfoCDC[i].state             = STATE_RUNNING | SUBSTATE_SEND_WRITE_REQ;
            }
        else
            {
                deviceInfoCDC[i].state             = STATE_RUNNING | SUBSTATE_SEND_READ_REQ;
            }
    #else
        USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
        if(!direction)
            {
                if(deviceInfoCDC[i].endpointDATA == 0x00)// if endpoint 0 then use control transfer for COMM interface request
                {
                    errorCode   = USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                                        request, 0, deviceInfoCDC[i].interface,deviceInfoCDC[i].reportSize, deviceInfoCDC[i].userData,
                                                        USB_DEVICE_REQUEST_SET , deviceInfoCDC[i].clientDriverID );
                    deviceInfoCDC[i].state         = STATE_WRITE_REQ_WAIT;
                }
                else
                {
                    errorCode                   = USBHostWrite( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA,
                                                            deviceInfoCDC[i].userData, deviceInfoCDC[i].reportSize );
                    deviceInfoCDC[i].state         = STATE_WRITE_REQ_WAIT;
                }
            }
        else
            {
                if(deviceInfoCDC[i].endpointDATA == 0x00)// if endpoint 0 then use control transfer for COMM interface request
                {
                    errorCode   = USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                                        request, 0, deviceInfoCDC[i].interface,deviceInfoCDC[i].reportSize, deviceInfoCDC[i].userData,
                                                        USB_DEVICE_REQUEST_GET , deviceInfoCDC[i].clientDriverID );
                    deviceInfoCDC[i].state         = STATE_READ_REQ_WAIT;
                }
                else
                {
                    errorCode                   = USBHostRead( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA, 
                                                               deviceInfoCDC[i].userData, deviceInfoCDC[i].reportSize );
                    deviceInfoCDC[i].state         = STATE_READ_REQ_WAIT;
                }
            }

        if(errorCode)
            {
                _USBHostCDC_TerminateTransfer( USB_CDC_RESET_ERROR );
            }
        else
            {
                deviceInfoCDC[i].flags.bfReset = 0;
            }
    #endif
    return USB_SUCCESS;
}
/*******************************************************************************
  Function:
    BOOL USBHostCDCTransferIsComplete( BYTE deviceAddress,
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

BOOL USBHostCDCTransferIsComplete( BYTE deviceAddress, BYTE *errorCode, BYTE *byteCount )
{
    BYTE    i;

    // Find the correct device.
    for (i=0; (i<USB_MAX_CDC_DEVICES) && (deviceInfoCDC[i].deviceAddress != deviceAddress); i++);
    if ((i == USB_MAX_CDC_DEVICES) || (deviceInfoCDC[i].state == STATE_DETACHED))
    {
        *errorCode = USB_CDC_DEVICE_NOT_FOUND;
        *byteCount = 0;
        return TRUE;
    }

    *byteCount = deviceInfoCDC[i].bytesTransferred;
    *errorCode = deviceInfoCDC[i].errorCode;

      #ifndef USB_ENABLE_TRANSFER_EVENT
            if(deviceInfoCDC[i].state == (STATE_RUNNING | SUBSTATE_WAITING_FOR_REQ))
      #else
            if(deviceInfoCDC[i].state == STATE_RUNNING)
      #endif
                {
                    return TRUE;
                }
            else
                {
                    return FALSE;
                }
}

// *****************************************************************************
// *****************************************************************************
// Host Stack Interface Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    BOOL USBHostCDCEventHandler( BYTE address, USB_EVENT event,
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
BOOL USBHostCDCEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    HOST_TRANSFER_DATA* transfer_data = data ;
    BYTE    i;
    #ifdef USB_ENABLE_TRANSFER_EVENT

    BYTE    errorCode = 0 ;
    DWORD   byteCount = 0;

    #endif

    switch (event)
    {
        case EVENT_NONE:             // No event occured (NULL event)
            USBTasks();
            return TRUE;
            break;

        case EVENT_DETACH:           // USB cable has been detached (data: BYTE, address of device)
            #ifdef DEBUG_MODE
                UART2PrintString( "CDC: Detach\r\n" );
            #endif
            // Find the device in the table.  If found, clear the important fields.
            for (i=0; (i<USB_MAX_CDC_DEVICES) && (deviceInfoCDC[i].deviceAddress != address); i++);
            if (i < USB_MAX_CDC_DEVICES)
            {
                deviceInfoCDC[i].deviceAddress    = 0;
                deviceInfoCDC[i].state            = STATE_DETACHED;
                CDCdeviceAddress = 0;
                /* Free the memory used by the CDC device */
                USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_DETACH,NULL, 0);
           }

            return TRUE;
            break;

        case EVENT_HUB_ATTACH:       // USB hub has been attached
        case EVENT_TRANSFER:         // A USB transfer has completed - NOT USED
            #if defined( USB_ENABLE_TRANSFER_EVENT )
                #ifdef DEBUG_MODE
                    UART2PrintString( "CDC: transfer event: " );
                    UART2PutHex( address );
                    UART2PrintString( "\r\n" );
                #endif

                for (i=0; (i<USB_MAX_CDC_DEVICES) && (deviceInfoCDC[i].deviceAddress != address); i++) {}
                if (i == USB_MAX_CDC_DEVICES)
                {
                    #ifdef DEBUG_MODE
                        UART2PrintString( "CDC: Unknown device\r\n" );
                    #endif
                    return FALSE;
                }
                #ifdef DEBUG_MODE
                    UART2PrintString( "CDC: Device state: " );
                    UART2PutHex( deviceInfoCDC[i].state );
                    UART2PrintString( "\r\n" );
                #endif
                switch (deviceInfoCDC[i].state)
                {

                    case STATE_WAIT_FOR_GET_LINE_CODING :
                            if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, 0, &errorCode, &byteCount ))
                            {
                                if (errorCode)
                                {
                                    /* Set error code */
                                    _USBHostCDC_TerminateTransfer(errorCode);
                                }
                                else
                                {
                                    // Clear the STALL.  Since it is EP0, we do not have to clear the stall.
                                    USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, 0 );
                                    // Compare with Line coding expected at Host end with the one received from device
                                    if(0 == memcmp(&CDC_DEV_LINE_CODING_Buffer, PTR_HOST_LINE_CODING_BUFFER, USB_CDC_LINE_CODING_LENGTH))
                                    {
                                        // if fine goto set  Control line state
                                       if(!USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                                            USB_CDC_SET_CONTROL_LINE_STATE , *(WORD*)PTR_HOST_CONTROL_LINE_BUFFER , deviceInfoCDC[i].commInterface.interfaceNum , USB_CDC_CONTROL_LINE_LENGTH, NULL,
                                                            USB_DEVICE_REQUEST_SET , deviceInfoCDC[i].clientDriverID ))    
                                       {           
                                            deviceInfoCDC[i].state = STATE_WAIT_FOR_SET_CONTROL_LINE_STATE;
                                       }
                                       else
                                       {
                                            _USBHostCDC_TerminateTransfer(USB_CDC_COMMAND_FAILED);
                                       }
                                    }
                                    else
                                    {
                                        // if data not as expected goto Set Line coding
                                       if(!USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                                            USB_CDC_SET_LINE_CODING, 1 , deviceInfoCDC[i].commInterface.interfaceNum , USB_CDC_LINE_CODING_LENGTH, (BYTE*)PTR_HOST_LINE_CODING_BUFFER,
                                                            USB_DEVICE_REQUEST_SET , deviceInfoCDC[i].clientDriverID ))
                                       {           
                                            deviceInfoCDC[i].state = STATE_WAIT_FOR_SET_LINE_CODING;
                                       }
                                       else
                                       {
                                            _USBHostCDC_TerminateTransfer(USB_CDC_COMMAND_FAILED);
                                       }
                                    }
                                }
                            }
                           
                        break;
                    case STATE_WAIT_FOR_SET_LINE_CODING :
                            if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, 0, &errorCode, &byteCount ))
                            {
                                if (errorCode)
                                {
                                    /* Set error code */
                                    _USBHostCDC_TerminateTransfer(errorCode);
                                }
                                else
                                {    // check again if line coding is correctly set
                                    // Send GET Line Coding request to the device 
                                    if(!USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                                                 USB_CDC_GET_LINE_CODING, 0 , deviceInfoCDC[i].commInterface.interfaceNum , USB_CDC_LINE_CODING_LENGTH, (BYTE*)&CDC_DEV_LINE_CODING_Buffer,
                                                                 USB_DEVICE_REQUEST_GET , deviceInfoCDC[i].clientDriverID ))
                                    {           
                                        // wait for transfer event for and then decide to alter the settings or not
                                        deviceInfoCDC[i].state     = STATE_WAIT_FOR_GET_LINE_CODING;
                                    }
                                    else
                                    {
                                        _USBHostCDC_TerminateTransfer(USB_CDC_COMMAND_FAILED);
                                    }
                                }
                            }

                        break;

                    case STATE_WAIT_FOR_SET_CONTROL_LINE_STATE :
                        if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, 0, &errorCode, &byteCount ))
                        {
                            if (errorCode)
                            {
                                /* Set error code */
                                _USBHostCDC_TerminateTransfer(errorCode);
                            }
                            else
                            {
                                // device is ready to TX/RX data 
                                USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_CDC_ATTACH,NULL, 0);
                                deviceInfoCDC[i].state     = STATE_RUNNING;
                            }
                        }   

                        break;
                        
                    case STATE_READ_REQ_WAIT :
                              if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA, &errorCode, &byteCount ))
                              {
                                if (errorCode)
                                {
                                    if(USB_ENDPOINT_STALLED == errorCode)
                                    {
                                         USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                         deviceInfoCDC[i].returnState = STATE_HOLDING;
                                         deviceInfoCDC[i].flags.bfReset = 1;
                                         _USBHostCDC_ResetStateJump( i );
                                    }
                                    else
                                    {
                                        _USBHostCDC_TerminateTransfer(errorCode);
                                    }
                                }
                                else
                                {
                                    // Clear the STALL.  Since it is EP0, we do not have to clear the stall.
                                    USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                    deviceInfoCDC[i].bytesTransferred = byteCount; /* Can compare with report size and flag error ???*/
                                    deviceInfoCDC[i].state = STATE_RUNNING;
                                    if(deviceInfoCDC[i].endpointDATA == 0x00)
                                    {
                                        USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_CDC_COMM_READ_DONE,NULL, 0);
                                    }
                                    else
                                    {   // read request was for data interface
                                        USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_CDC_DATA_READ_DONE,NULL, 0);
                                    }
                                }
                              }
#ifdef DEBUG_MODE
                              UART2PrintString("|");
#endif                                
                        break;
                    
                    case STATE_WRITE_REQ_WAIT :
                              if (USBHostTransferIsComplete( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA, &errorCode, &byteCount ))
                              {
                                if (errorCode)
                                {
                                    if(USB_ENDPOINT_STALLED == errorCode)
                                    {
                                         USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                         deviceInfoCDC[i].returnState = STATE_HOLDING;
                                         deviceInfoCDC[i].flags.bfReset = 1;
                                         _USBHostCDC_ResetStateJump( i );
                                    }
                                    else
                                    {
                                        _USBHostCDC_TerminateTransfer(errorCode);
                                    }
                                }
                                else
                                {
                                    // Clear the STALL.  Since it is EP0, we do not have to clear the stall.
                                    USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                                    deviceInfoCDC[i].bytesTransferred = byteCount; /* Can compare with report size and flag error ???*/
                                    deviceInfoCDC[i].state = STATE_RUNNING;
                                    if(deviceInfoCDC[i].endpointDATA == 0x00)
                                    {
                                        USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_CDC_COMM_WRITE_DONE,NULL, 0);
                                    }
                                    else
                                    {   // read request was for data interface
                                        USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_CDC_DATA_WRITE_DONE,NULL, 0);
                                    }
                                }
                              }
#ifdef DEBUG_MODE
                              UART2PrintString("|");
#endif                                
                        break;

                    default:
                            return FALSE;
                }
            #endif
        case EVENT_SOF:              // Start of frame - NOT NEEDED
        case EVENT_RESUME:           // Device-mode resume received
        case EVENT_SUSPEND:          // Device-mode suspend/idle event received
        case EVENT_RESET:            // Device-mode bus reset received
        case EVENT_STALL:            // A stall has occured
            return TRUE;
            break;
        case EVENT_BUS_ERROR:            // BUS error has occurred
            for (i=0; (i<USB_MAX_CDC_DEVICES) && (deviceInfoCDC[i].deviceAddress != address); i++);
            if (i < USB_MAX_CDC_DEVICES)
            {
                if(transfer_data->bErrorCode == USB_ENDPOINT_NAK_TIMEOUT)
                {
                    USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_CDC_NAK_TIMEOUT,NULL, 0);
                    deviceInfoCDC[i].state = STATE_RUNNING;
                    USBHostClearEndpointErrors( deviceInfoCDC[i].deviceAddress, deviceInfoCDC[i].endpointDATA );
                    return TRUE;
                }
            }
            break;

        default:
            return FALSE;
            break;
    }
    return FALSE;
}

/*******************************************************************************
  Function:
    BOOL USBHostCDCInitialize( BYTE address, DWORD flags, BYTE clientDriverID )

  Summary:
    This function is the initialization routine for this client driver.

  Description:
    This function is the initialization routine for this client driver.  It
    is called by the host layer when the USB device is being enumerated.For a 
    CDC device we need to look into CDC descriptor, interface descriptor and 
    endpoint descriptor. This will decode information for Communication and
    Data interface.

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
BOOL USBHostCDCInitialize( BYTE address, DWORD flags, BYTE clientDriverID )
{
    BYTE   *descriptor         = NULL;
    WORD    i                  = 0;
    WORD    endpointINsize     = 0;
    WORD    endpointOUTsize    = 0;
    BYTE    device             = 0;
    BYTE    endpointIN         = 0;
    BYTE    endpointOUT        = 0; 
    BYTE    validCommInterface = 0;
    BYTE    validDataInterface = 0;

    #ifdef DEBUG_MODE
        UART2PrintString( "CDC: USBHostCDCInitialize(0x" );
        UART2PutHex( flags );
        UART2PrintString( ")\r\n" );
    #endif

    // Find the device in the table.  If we cannot find it, return an error.
    #ifdef DEBUG_MODE
        UART2PrintString("CDC: Selecting configuration...\r\n" );
    #endif
    for (device = 0; (device < USB_MAX_CDC_DEVICES) ; device++)
    {
        if(deviceInfoCDC[device].deviceAddress == address)
            return TRUE;
    }

    for (device = 0; (device < USB_MAX_CDC_DEVICES) && (deviceInfoCDC[device].deviceAddress != 0); device++);
    if (device == USB_MAX_CDC_DEVICES)
    {
        #ifdef DEBUG_MODE
            UART2PrintString("CDC: Not in the table!\r\n" );
        #endif
        return FALSE;
    }

    USBHostCDC_Init_CDC_Buffers();
    descriptor = USBHostGetCurrentConfigurationDescriptor( address );

        i = 0;

        #ifdef DEBUG_MODE
            UART2PrintString("CDC: Checking descriptor " );
            UART2PutDec( descriptor[i+5] );
            UART2PrintString(" ...\r\n" );
        #endif

        // Total no of interfaces
        deviceInfoCDC[device].noOfInterfaces = descriptor[i+4] ;
       
        i += descriptor[i]; // skip configuration descriptor

        // Set current configuration to this configuration.  We can change it later.


        // Find the next interface descriptor.
        while (i < ((USB_CONFIGURATION_DESCRIPTOR *)descriptor)->wTotalLength)
        {
            #ifdef DEBUG_MODE
                UART2PrintString("CDC:  Checking interface...\r\n" );
            #endif
            // initialize device address
            deviceInfoCDC[device].deviceAddress        = address;
          
            // See if we are pointing to an interface descriptor.
            if (descriptor[i+1] == USB_DESCRIPTOR_INTERFACE)
            {
                // See if the interface is a CDC - Communicaton Interface.
                if ((descriptor[i+5] == USB_CDC_COMM_INTF) || (descriptor[i+5] == 0xFF))
                {
                    validCommInterface = 1;
                    deviceInfoCDC[device].commInterface.interfaceNum  = descriptor[i+2];
                    deviceInfoCDC[device].commInterface.noOfEndpoints = descriptor[i+4];
                    // Since control has reached here , implies subclass & protocol is supported by the host
                    
                    // look for class specific functional descriptors
                     i += descriptor[i]; // skip interface descriptor
                     
                    // parse all function descriptors
                     while(USB_CDC_CS_INTERFACE == descriptor[i+1] )
                    {
                         switch(descriptor[i+2])
                        {
                             case   USB_CDC_DSC_FN_HEADER   :  
                                         deviceInfoCDC[device].commInterface.Header_Fn_Dsc = 
                                                            (*((USB_CDC_HEADER_FN_DSC*)&descriptor[i]));
                                 break;

                             case   USB_CDC_DSC_FN_CALL_MGT : 
                                         deviceInfoCDC[device].commInterface.Call_Mgt_Fn_Desc = 
                                                            (*((USB_CDC_CALL_MGT_FN_DSC*)&descriptor[i]));
                                 break;

                             case   USB_CDC_DSC_FN_ACM      :
                                         deviceInfoCDC[device].commInterface.ACM_Fn_Desc = 
                                                            (*((USB_CDC_ACM_FN_DSC*)&descriptor[i]));
                                 break;

                             case   USB_CDC_DSC_FN_UNION    :
                                         deviceInfoCDC[device].commInterface.Union_Fn_Desc = 
                                                            (*((USB_CDC_UNION_FN_DSC*)&descriptor[i]));
                                 break;

                             default                        :  // function not supported -- flag error/ return false
                                 break;
                        }
                        
                        i += descriptor[i]; // goto next function descriptor
                    }

                    if(descriptor[i+1] == USB_DESCRIPTOR_ENDPOINT)
                    {
                        if(descriptor[i+3] == 0x03) // Communication Interface uses Interrupt endpoint
                        {
                            if (((descriptor[i+2] & 0x80) == 0x80) && (deviceInfoCDC[device].commInterface.endpointIN == 0))
                            {
                                deviceInfoCDC[device].commInterface.endpointIN           = descriptor[i+2];
                                deviceInfoCDC[device].commInterface.endpointMaxDataSize  = ((descriptor[i+4]) |
                                                                                        (descriptor[i+5] << 8));
                                deviceInfoCDC[device].commInterface.endpointPollInterval = descriptor[i+6];
                            }
                        }
                        i += descriptor[i];
                    }
                    else
                    {
                        // communication interface normaly has one endpoint. 
                        // Notification element is optional - currently not supported.
                    }

                    if (((descriptor[i+1] == USB_DESCRIPTOR_INTERFACE) && (descriptor[i+5] == USB_CDC_DATA_INTF)) ||
                        (descriptor[i+1] == USB_DESCRIPTOR_ENDPOINT))

                    {
                           if(descriptor[i+5] == USB_CDC_DATA_INTF)
                           {
                              validDataInterface = 1;
                              deviceInfoCDC[device].dataInterface.interfaceNum  = descriptor[i+2];
                              deviceInfoCDC[device].dataInterface.noOfEndpoints = descriptor[i+4];
                              
                              i += descriptor[i]; // goto endpoint descriptors
                           }
                           else
                           {
                              // Some CDC devices do not list DATA interface descriptor seperately
                              // DATA endpoints are mentioned along with Communicaton interface
                              validDataInterface = 1;
                              deviceInfoCDC[device].dataInterface.interfaceNum  = deviceInfoCDC[device].commInterface.interfaceNum;
                              deviceInfoCDC[device].dataInterface.noOfEndpoints = deviceInfoCDC[device].commInterface.noOfEndpoints - 1;
                           }
                           // Look for bulk or isochronous , IN and OUT endpoints.
                           endpointIN  = 0;
                           endpointOUT = 0;
                           
                           while(descriptor[i+1] == USB_DESCRIPTOR_ENDPOINT)
                           {   // Data Interface uses Bulk or Iscochronous endpoint
                               deviceInfoCDC[device].dataInterface.endpointType = descriptor[i+3];
                               if((descriptor[i+3] == 0x01)||(descriptor[i+3] == 0x02))
                               {
                                   if (((descriptor[i+2] & 0x80) == 0x80) && (endpointIN == 0))
                                   {
                                       endpointIN     = descriptor[i+2];
                                       endpointINsize = ((descriptor[i+4])|(descriptor[i+5] << 8));
                                   }
                                   if (((descriptor[i+2] & 0x80) == 0x00) && (endpointOUT == 0))
                                   {
                                       endpointOUT     = descriptor[i+2];
                                       endpointOUTsize = ((descriptor[i+4])|(descriptor[i+5] << 8));
                                   }
                           
                               }
                               i += descriptor[i];
                           }
                           
                           if ((endpointIN != 0) || (endpointOUT != 0)) // normally endpoint should be in pair of same type
                           {
                               deviceInfoCDC[device].dataInterface.endpointIN           = endpointIN;
                               deviceInfoCDC[device].dataInterface.endpointInDataSize   = endpointINsize;
                           
                               deviceInfoCDC[device].dataInterface.endpointOUT          = endpointOUT;
                               deviceInfoCDC[device].dataInterface.endpointOutDataSize  = endpointOUTsize;
                           
                               USBHostSetNAKTimeout( address, endpointIN,  USB_HOST_CDC_NAK_TIMEOUT, USB_NUM_BULK_NAKS );
                               USBHostSetNAKTimeout( address, endpointOUT, USB_HOST_CDC_NAK_TIMEOUT, USB_NUM_BULK_NAKS );
                           }
                           else
                           {
                               validDataInterface = 0;
                           }
                    } // if data interface
                } // communication interface loop

            } // if interface descriptor

           // Jump to the next descriptor in this configuration.
           i += descriptor[i];
        } // main loop - to parse descriptors
        // ACM subclass must support a valid communication and data interfaces
        if((validDataInterface == 0) ||(validCommInterface == 0))
        {
            return FALSE;
        }

        deviceInfoCDC[device].clientDriverID = clientDriverID;

        #ifndef USB_ENABLE_TRANSFER_EVENT
           deviceInfoCDC[device].state                = STATE_INITIALIZE_DEVICE;
        #else
           // Initialize device address for application use 
           USBHostCDCInitAddress( deviceInfoCDC[device].deviceAddress,0,deviceInfoCDC[device].clientDriverID );
           // Send GET Line Coding request to the device 
           if(USBHostIssueDeviceRequest( deviceInfoCDC[device].deviceAddress, USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE, 
                                     USB_CDC_GET_LINE_CODING, 0 , deviceInfoCDC[device].commInterface.interfaceNum , USB_CDC_LINE_CODING_LENGTH, (BYTE*)&CDC_DEV_LINE_CODING_Buffer,
                                     USB_DEVICE_REQUEST_GET , deviceInfoCDC[device].clientDriverID ))
           {           
                return FALSE;
           }
           // wait for transfer event for and then decide to alter the settings or not
           deviceInfoCDC[device].state                = STATE_WAIT_FOR_GET_LINE_CODING;
        #endif
       return TRUE;
}


// *****************************************************************************
// *****************************************************************************
// Internal Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void _USBHostCDC_ResetStateJump( BYTE i )

  Summary:

  Description:
    This function determines which portion of the reset processing needs to
    be executed next and jumps to that state.

Precondition:
    The device information must be in the deviceInfo array.

  Parameters:
    BYTE i  - Index into the deviceInfoMSD structure for the device to reset.

  Returns:
    None

  Remarks:
    None
*******************************************************************************/
void _USBHostCDC_ResetStateJump( BYTE i )
{
     #ifdef USB_ENABLE_TRANSFER_EVENT
        BYTE    errorCode;
     #endif
   if (deviceInfoCDC[i].flags.bfReset)
    {
        #ifndef USB_ENABLE_TRANSFER_EVENT
            deviceInfoCDC[i].state = STATE_CDC_RESET_RECOVERY;
        #else
             errorCode = USBHostIssueDeviceRequest( deviceInfoCDC[i].deviceAddress, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_CLASS | USB_SETUP_RECIPIENT_INTERFACE,
                            USB_CDC_RESET, 0, deviceInfoCDC[i].interface, 0, NULL, USB_DEVICE_REQUEST_SET , deviceInfoCDC[i].clientDriverID );

            if (errorCode)
            {
                //MCHP - Now what??
                _USBHostCDC_TerminateTransfer( USB_CDC_RESET_ERROR );
            }
            else
            {
                deviceInfoCDC[i].state = STATE_WAIT_FOR_RESET;
            }
        #endif
    }
    else
    {
         USB_HOST_APP_EVENT_HANDLER(deviceInfoCDC[i].deviceAddress,EVENT_CDC_RESET,NULL, 0);

         deviceInfoCDC[i].state = deviceInfoCDC[i].returnState;
    }
}



/*******************************************************************************
  Function:
    void USBHostCDC_Init_CDC_Buffers(void)

  Summary:

  Description:
    This function intializes the Line Coding buffer with values configured by
    application in file usb_config.h . In case application does not delcare the
    macros then the default values will be assigned. Baudrate, Stop Bits, Data
    bits & parity bits are the settings expected in the device. During enumeration
    if the CDC Client finds that the device is not configured to these values then
    CDC Client will configure the device to the required values.

  Precondition:
    Configuration macros should be defined in file usb_config.h. USBConfig tool
    can be used to configure the CDC in ACM mode.
  
  Parameters:
    None
  
  Returns:
    None

  Remarks:
    If the apllication does not define macros in file usb_config.h then default
    values defined in file usb_host_cdc.c are cosidered.
*******************************************************************************/
void USBHostCDC_Init_CDC_Buffers(void)
{
   CDC_LINE_CODING_Buffer._byte[0] = (BYTE)USB_CDC_BAUDRATE_SUPPORTED; // Initialize supported Bit rate
   CDC_LINE_CODING_Buffer._byte[1] = (BYTE)(USB_CDC_BAUDRATE_SUPPORTED >> 8);
   CDC_LINE_CODING_Buffer._byte[2] = (BYTE)(USB_CDC_BAUDRATE_SUPPORTED >> 16);
   CDC_LINE_CODING_Buffer._byte[3] = (BYTE)(USB_CDC_BAUDRATE_SUPPORTED >> 24);

   CDC_LINE_CODING_Buffer.bCharFormat = USB_CDC_STOP_BITS;
   CDC_LINE_CODING_Buffer.bParityType = USB_CDC_PARITY_TYPE;
   CDC_LINE_CODING_Buffer.bDataBits   = (BYTE)USB_CDC_NO_OF_DATA_BITS; // No of data bits
}
