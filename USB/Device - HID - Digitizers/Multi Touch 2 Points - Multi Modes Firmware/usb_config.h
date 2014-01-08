/********************************************************************
 FileName:     	usb_config.h
 Dependencies: 	Always: GenericTypeDefs.h, usb_device.h
               	Situational: usb_function_hid.h, usb_function_cdc.h, usb_function_msd.h, etc.
 Processor:		PIC18, PIC24, PIC32 USB Microcontrollers
 Hardware:		This demo is natively intended to be used on Microchip USB demo
 				boards supported by the MCHPFSUSB stack.  See release notes for
 				support matrix.  This demo can be modified for use on other hardware
 				platforms.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24) or C32 (for PIC32)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
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

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
 *******************************************************************/

/*********************************************************************
 * Descriptor specific type definitions are defined in: usbd.h
 ********************************************************************/

#ifndef USBCFG_H
#define USBCFG_H

/** DEFINITIONS ****************************************************/
#define USB_EP0_BUFF_SIZE		8	// Valid Options: 8, 16, 32, or 64 bytes.
								// Using larger options take more SRAM, but
								// does not provide much advantage in most types
								// of applications.  Exceptions to this, are applications
								// that use EP0 IN or OUT for sending large amounts of
								// application related data.
									
#define USB_MAX_NUM_INT     	1   // For tracking Alternate Setting
#define USB_MAX_EP_NUMBER	    1

//Device descriptor - if these two definitions are not defined then
//  a ROM USB_DEVICE_DESCRIPTOR variable by the exact name of device_dsc
//  must exist.
#define USB_USER_DEVICE_DESCRIPTOR &device_dsc
#define USB_USER_DEVICE_DESCRIPTOR_INCLUDE extern ROM USB_DEVICE_DESCRIPTOR device_dsc

//Configuration descriptors - if these two definitions do not exist then
//  a ROM BYTE *ROM variable named exactly USB_CD_Ptr[] must exist.
#define USB_USER_CONFIG_DESCRIPTOR USB_CD_Ptr
#define USB_USER_CONFIG_DESCRIPTOR_INCLUDE extern ROM BYTE *ROM USB_CD_Ptr[]

//Make sure only one of the below "#define USB_PING_PONG_MODE"
//is uncommented.
//#define USB_PING_PONG_MODE USB_PING_PONG__NO_PING_PONG
#define USB_PING_PONG_MODE USB_PING_PONG__FULL_PING_PONG
//#define USB_PING_PONG_MODE USB_PING_PONG__EP0_OUT_ONLY
//#define USB_PING_PONG_MODE USB_PING_PONG__ALL_BUT_EP0		//NOTE: This mode is not supported in PIC18F4550 family rev A3 devices


//#define USB_POLLING
#define USB_INTERRUPT

/* Parameter definitions are defined in usb_device.h */
#define USB_PULLUP_OPTION USB_PULLUP_ENABLE
//#define USB_PULLUP_OPTION USB_PULLUP_DISABLED

#define USB_TRANSCEIVER_OPTION USB_INTERNAL_TRANSCEIVER
//External Transceiver support is not available on all product families.  Please
//  refer to the product family datasheet for more information if this feature
//  is available on the target processor.
//#define USB_TRANSCEIVER_OPTION USB_EXTERNAL_TRANSCEIVER

#define USB_SPEED_OPTION USB_FULL_SPEED
//#define USB_SPEED_OPTION USB_LOW_SPEED //(not valid option for PIC24F devices)

#define MY_VID 0x04D8
#define MY_PID 0x0063

//------------------------------------------------------------------------------------------------------------------
//Option to enable auto-arming of the status stage of control transfers, if no
//"progress" has been made for the USB_STATUS_STAGE_TIMEOUT value.
//If progress is made (any successful transactions completing on EP0 IN or OUT)
//the timeout counter gets reset to the USB_STATUS_STAGE_TIMEOUT value.
//
//During normal control transfer processing, the USB stack or the application 
//firmware will call USBCtrlEPAllowStatusStage() as soon as the firmware is finished
//processing the control transfer.  Therefore, the status stage completes as 
//quickly as is physically possible.  The USB_ENABLE_STATUS_STAGE_TIMEOUTS 
//feature, and the USB_STATUS_STAGE_TIMEOUT value are only relevant, when:
//1.  The application uses the USBDeferStatusStage() API function, but never calls
//      USBCtrlEPAllowStatusStage().  Or:
//2.  The application uses host to device (OUT) control transfers with data stage,
//      and some abnormal error occurs, where the host might try to abort the control
//      transfer, before it has sent all of the data it claimed it was going to send.
//
//If the application firmware never uses the USBDeferStatusStage() API function,
//and it never uses host to device control transfers with data stage, then
//it is not required to enable the USB_ENABLE_STATUS_STAGE_TIMEOUTS feature.

#define USB_ENABLE_STATUS_STAGE_TIMEOUTS    //Comment this out to disable this feature.  

//Section 9.2.6 of the USB 2.0 specifications indicate that:
//1.  Control transfers with no data stage: Status stage must complete within 
//      50ms of the start of the control transfer.
//2.  Control transfers with (IN) data stage: Status stage must complete within 
//      50ms of sending the last IN data packet in fullfilment of the data stage.
//3.  Control transfers with (OUT) data stage: No specific status stage timing
//      requirement.  However, the total time of the entire control transfer (ex:
//      including the OUT data stage and IN status stage) must not exceed 5 seconds.
//
//Therefore, if the USB_ENABLE_STATUS_STAGE_TIMEOUTS feature is used, it is suggested
//to set the USB_STATUS_STAGE_TIMEOUT value to timeout in less than 50ms.  If the
//USB_ENABLE_STATUS_STAGE_TIMEOUTS feature is not enabled, then the USB_STATUS_STAGE_TIMEOUT
//parameter is not relevant.

#define USB_STATUS_STAGE_TIMEOUT     (BYTE)45   //Approximate timeout in milliseconds, except when
                                                //USB_POLLING mode is used, and USBDeviceTasks() is called at < 1kHz
                                                //In this special case, the timeout becomes approximately:
//Timeout(in milliseconds) = ((1000 * (USB_STATUS_STAGE_TIMEOUT - 1)) / (USBDeviceTasks() polling frequency in Hz))
//------------------------------------------------------------------------------------------------------------------

#define USB_SUPPORT_DEVICE

#define USB_NUM_STRING_DESCRIPTORS 3

//#define USB_INTERRUPT_LEGACY_CALLBACKS
#define USB_ENABLE_ALL_HANDLERS
//#define USB_ENABLE_SUSPEND_HANDLER
//#define USB_ENABLE_WAKEUP_FROM_SUSPEND_HANDLER
//#define USB_ENABLE_SOF_HANDLER
//#define USB_ENABLE_ERROR_HANDLER
//#define USB_ENABLE_OTHER_REQUEST_HANDLER
//#define USB_ENABLE_SET_DESCRIPTOR_HANDLER
//#define USB_ENABLE_INIT_EP_HANDLER
//#define USB_ENABLE_EP0_DATA_HANDLER
//#define USB_ENABLE_TRANSFER_COMPLETE_HANDLER

/** DEVICE CLASS USAGE *********************************************/
#define USB_USE_HID

/** ENDPOINTS ALLOCATION *******************************************/

/* HID */
#define HID_INTF_ID             0x00
#define HID_EP 					1
#define HID_INT_OUT_EP_SIZE     64
#define HID_INT_IN_EP_SIZE      64
#define HID_NUM_OF_DSC          1
#define HID_RPT01_SIZE          299u
#define USER_GET_REPORT_HANDLER UserGetReportHandler
#define USER_SET_REPORT_HANDLER UserSetReportHandler


/** OTHER SETTINGS *******************************************/

//#define DEFAULT_DEVICE_MODE       MOUSE_MODE					//Provides limited functionality even on legacy operating systems
#define DEFAULT_DEVICE_MODE       SINGLE_TOUCH_DIGITIZER_MODE	//Ideal if the hardware will be used on Vista or later
//#define DEFAULT_DEVICE_MODE       MULTI_TOUCH_DIGITIZER_MODE	//Pointless to select this option, since multi-touch capable OSes will use SET_FEATURE to select this during enumeration.

//The DEFAULT_DEVICE_MODE definition sets the default USB HID "DeviceMode" at
//power up.  After power up, the firmware will need to know what type of HID device 
//the firmware should behave as.  The HID report descriptor for this project 
//includes multiple top level collections, which allow the device to send HID 
//input reports enumulating a standard mouse, a single-touch/pen digitizer, and 
//a multi-touch digitizer.  Since ealier versions of Windows do not support 
//multi-touch HID digitizers, it is best to default to either mouse mode, or 
//single-touch digitizer mode.  During the enumeration sequence on an OS that 
//supports multi-touch digitizers (ex: Windows 7), the host will send a 
//SET_FEATURE request to set the new DeviceMode to MULTI_TOUCH_DIGITIZER_MODE 
//mode. This will allow it to function as a multi-touch capable digitizer, even 
//though it defaulted (at power up) to something else.

//If the device will always be plugged into machines running Vista or later 
//operating systems, it is suggested to default to SINGLE_TOUCH_DIGITIZER_MODE.
//If the device must provide some (limited) functionality in legacy operating 
//systems (ex: XP and older), then the MOUSE_MODE can be used.  On these OSes,
//the firmware can emulate the function of a standard mouse or touch pad (by 
//using the touch data generated by the touch surface area to create click and X/Y 
//movement info).  However, there is a penalty for defaulting to MOUSE_MODE.  On Vista 
//(SP1, future versions/patches might change this) the OS does not send the 
//SET_REPORT (feature) request to change the device mode to SINGLE_TOUCH_DIGITIZER_MODE,
//even though this operating system does support some single-touch capabilities.
//This could potentially be worked around by putting a mechanical switch on the device
//that allows the user to select the DEFAULT_DEVICE_MODE best for their 
//application.  Alternatively, a custom PC application could potentially be used 
//to send a software command to switch the device into the SINGLE_TOUCH_DIGITIZER_MODE 
//on a Vista based machine.


/** DEFINITIONS ****************************************************/

//Report ID Definitions.
#define MULTI_TOUCH_DATA_REPORT_ID			(BYTE)0x01
#define VALID_CONTACTS_FEATURE_REPORT_ID	(BYTE)0x02
#define DEVICE_MODE_FEATURE_REPORT_ID		(BYTE)0x03
#define MOUSE_DATA_REPORT_ID				(BYTE)0x04
#define SINGLE_TOUCH_DATA_REPORT_ID			(BYTE)0x05					





#endif //USBCFG_H
