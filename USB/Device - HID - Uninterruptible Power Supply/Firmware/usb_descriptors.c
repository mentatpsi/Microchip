/********************************************************************
 FileName:     	usb_descriptors.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
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

*********************************************************************
-usb_descriptors.c-
-------------------------------------------------------------------
Filling in the descriptor values in the usb_descriptors.c file:
-------------------------------------------------------------------

[Device Descriptors]
The device descriptor is defined as a USB_DEVICE_DESCRIPTOR type.  
This type is defined in usb_ch9.h  Each entry into this structure
needs to be the correct length for the data type of the entry.

[Configuration Descriptors]
The configuration descriptor was changed in v2.x from a structure
to a BYTE array.  Given that the configuration is now a byte array
each byte of multi-byte fields must be listed individually.  This
means that for fields like the total size of the configuration where
the field is a 16-bit value "64,0," is the correct entry for a
configuration that is only 64 bytes long and not "64," which is one
too few bytes.

The configuration attribute must always have the _DEFAULT
definition at the minimum. Additional options can be ORed
to the _DEFAULT attribute. Available options are _SELF and _RWU.
These definitions are defined in the usb_device.h file. The
_SELF tells the USB host that this device is self-powered. The
_RWU tells the USB host that this device supports Remote Wakeup.

[Endpoint Descriptors]
Like the configuration descriptor, the endpoint descriptors were 
changed in v2.x of the stack from a structure to a BYTE array.  As
endpoint descriptors also has a field that are multi-byte entities,
please be sure to specify both bytes of the field.  For example, for
the endpoint size an endpoint that is 64 bytes needs to have the size
defined as "64,0," instead of "64,"

Take the following example:
    // Endpoint Descriptor //
    0x07,                       //the size of this descriptor //
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    _EP02_IN,                   //EndpointAddress
    _INT,                       //Attributes
    0x08,0x00,                  //size (note: 2 bytes)
    0x02,                       //Interval

The first two parameters are self-explanatory. They specify the
length of this endpoint descriptor (7) and the descriptor type.
The next parameter identifies the endpoint, the definitions are
defined in usb_device.h and has the following naming
convention:
_EP<##>_<dir>
where ## is the endpoint number and dir is the direction of
transfer. The dir has the value of either 'OUT' or 'IN'.
The next parameter identifies the type of the endpoint. Available
options are _BULK, _INT, _ISO, and _CTRL. The _CTRL is not
typically used because the default control transfer endpoint is
not defined in the USB descriptors. When _ISO option is used,
addition options can be ORed to _ISO. Example:
_ISO|_AD|_FE
This describes the endpoint as an isochronous pipe with adaptive
and feedback attributes. See usb_device.h and the USB
specification for details. The next parameter defines the size of
the endpoint. The last parameter in the polling interval.

-------------------------------------------------------------------
Adding a USB String
-------------------------------------------------------------------
A string descriptor array should have the following format:

rom struct{byte bLength;byte bDscType;word string[size];}sdxxx={
sizeof(sdxxx),DSC_STR,<text>};

The above structure provides a means for the C compiler to
calculate the length of string descriptor sdxxx, where xxx is the
index number. The first two bytes of the descriptor are descriptor
length and type. The rest <text> are string texts which must be
in the unicode format. The unicode format is achieved by declaring
each character as a word type. The whole text string is declared
as a word array with the number of characters equals to <size>.
<size> has to be manually counted and entered into the array
declaration. Let's study this through an example:
if the string is "USB" , then the string descriptor should be:
(Using index 02)
rom struct{byte bLength;byte bDscType;word string[3];}sd002={
sizeof(sd002),DSC_STR,'U','S','B'};

A USB project may have multiple strings and the firmware supports
the management of multiple strings through a look-up table.
The look-up table is defined as:
rom const unsigned char *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002};

The above declaration has 3 strings, sd000, sd001, and sd002.
Strings can be removed or added. sd000 is a specialized string
descriptor. It defines the language code, usually this is
US English (0x0409). The index of the string must match the index
position of the USB_SD_Ptr array, &sd000 must be in position
USB_SD_Ptr[0], &sd001 must be in position USB_SD_Ptr[1] and so on.
The look-up table USB_SD_Ptr is used by the get string handler
function.

-------------------------------------------------------------------

The look-up table scheme also applies to the configuration
descriptor. A USB device may have multiple configuration
descriptors, i.e. CFG01, CFG02, etc. To add a configuration
descriptor, user must implement a structure similar to CFG01.
The next step is to add the configuration descriptor name, i.e.
cfg01, cfg02,.., to the look-up table USB_CD_Ptr. USB_CD_Ptr[0]
is a dummy place holder since configuration 0 is the un-configured
state according to the definition in the USB specification.

*********************************************************************

 Change History:
  Rev   Description
  ----  -----------------------------------------
  2.7   Fixed an error in the descriptors that specified that there
        was only one endpoint where there are actually two.  Minor
        changes to report descriptor and comments to make them more
        consistent.

********************************************************************/
 
/*********************************************************************
 * Descriptor specific type definitions are defined in:
 * usb_device.h
 *
 * Configuration options are defined in:
 * usb_config.h
 ********************************************************************/

#ifndef __USB_DESCRIPTORS_C
#define __USB_DESCRIPTORS_C

/** INCLUDES *******************************************************/
#include "./USB/usb.h"
#include "./USB/usb_function_hid.h"
#include "reports.h"

/** CONSTANTS ******************************************************/
#if defined(__18CXX)
#pragma romdata
#endif

/* This example was developed directly from pwr.hid example
 * distributed with the HID Descriptor Tool from the USB.org
 * found at the following link (valid as of 3/9/2009
 * http://www.usb.org/developers/hidpage
 */

/* Device Descriptor */
ROM USB_DEVICE_DESCRIPTOR device_dsc=
{
    0x12,    // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,                // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    USB_EP0_BUFF_SIZE,          // Max packet size for EP0, see usb_config.h
    MY_VID,                 // Vendor ID
    MY_PID,                 // Product ID
    0x0003,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x04,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
ROM BYTE configDescriptor1[]={
    /* Configuration Descriptor */
    0x09,//sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                // CONFIGURATION descriptor type
    DESC_CONFIG_WORD(41),   // Total length of data for this cfg
    1,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    0,                      // Configuration string index
    _DEFAULT | _SELF,// Attributes, see usb_device.h
    50,                     // Max power consumption (2X mA)

    /* Interface Descriptor */
    0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this intf
    HID_INTF,               // Class code
    0x00,                   // Subclass code
    0x00,					// Protocol code (None)
    0,                      // Interface string index

    /* HID Class-Specific Descriptor */
    0x09,//sizeof(USB_HID_DSC)+3,    // Size of this descriptor in bytes RRoj hack
    DSC_HID,                // HID descriptor type
    DESC_CONFIG_WORD(0x0111),                 // HID Spec Release Number in BCD format (1.11)
    0x00,                   // Country Code (0x00 for Not supported)
    HID_NUM_OF_DSC,         // Number of class descriptors, see usbcfg.h
    DSC_RPT,                // Report descriptor type
    DESC_CONFIG_WORD(HID_RPT01_SIZE),   //sizeof(hid_rpt01),      // Size of the report descriptor
    
    /* Endpoint Descriptor */
    0x07,/*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    HID_EP | _EP_IN,            //EndpointAddress
    _INTERRUPT,                       //Attributes
    DESC_CONFIG_WORD(HID_INT_IN_EP_SIZE),                  //size
    0x0A,                        //Interval

    /* Endpoint Descriptor */
    0x07,/*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    HID_EP | _EP_OUT,            //EndpointAddress
    _INTERRUPT,                       //Attributes
    DESC_CONFIG_WORD(HID_INT_OUT_EP_SIZE),                  //size
    0x0A                        //Interval
};


//Language code string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[1];}sd000={
sizeof(sd000),USB_DESCRIPTOR_STRING,{0x0409
}};

//Manufacturer string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[25];}sd001={
sizeof(sd001),USB_DESCRIPTOR_STRING,
{'M','i','c','r','o','c','h','i','p',' ',
'T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'
}};

//Product string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[21];}sd002={
sizeof(sd002),USB_DESCRIPTOR_STRING,
{'M','i','c','r','o','c','h','i','p',' ','U',
'P','S',' ','E','x','a','m','p','l','e'
}};

//Manufacturer string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[4];}sd003={
sizeof(sd003),USB_DESCRIPTOR_STRING,
{'L','i',' ',' '}};

//Manufacturer string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[12];}sd004={
sizeof(sd004),USB_DESCRIPTOR_STRING,
{'1','2','3','4','5','6','7','8','9','A','B','C'}};

//Array of string descriptors
ROM BYTE *ROM USB_SD_Ptr[]=
{
    (ROM BYTE *ROM)&sd000,
    (ROM BYTE *ROM)&sd001,
    (ROM BYTE *ROM)&sd002,
    (ROM BYTE *ROM)&sd003,
    (ROM BYTE *ROM)&sd004,
};

//Class specific descriptor
ROM struct{BYTE report[HID_RPT01_SIZE];}hid_rpt01={
    {
    0x05, 0x84,                                 // USAGE_PAGE (Power Device)
    0x09, 0x04,                                 // USAGE (UPS)
    0xa1, 0x01,                                 // COLLECTION (Application)
    0x09, 0x24,                                 //   USAGE (Sink)
    0xa1, 0x00,                                 //   COLLECTION (Physical)

    0x05, 0x84,                                 //     USAGE_PAGE (Power Device)
    0x85, POWER_SUMMARY_POWER_SUMMARY_ID,       //     REPORT_ID (POWER_SUMMARY_POWER_SUMMARY_ID)
    0x09, 0x25,                                 //     USAGE (SinkID)
    0x15, 0x00,                                 //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,                           //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                                 //     REPORT_SIZE (8)
    0x95, 0x01,                                 //     REPORT_COUNT (1)
    0xb1, 0x03,                                 //     FEATURE (Cnst,Var,Abs)

    0x05, 0x84,                                 //     USAGE_PAGE (Power Device)
    0x85, POWER_SUMMARY_I_NAME,                 //     REPORT_ID (POWER_SUMMARY_I_NAME)
    0x09, 0x01,                                 //     USAGE (iName)
    0x15, 0x00,                                 //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,                           //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                                 //     REPORT_SIZE (8)
    0x95, 0x03,                                 //     REPORT_COUNT (1)
    0x79, 0x02,                                 //     STRING_INDEX(2)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

    0x85, POWER_SUMMARY_I_PRODUCT,              //     REPORT_ID (POWER_SUMMARY_I_PRODUCT)
    0x09, 0xfe,                                 //     USAGE(iProduct)
    0x79, 0x02,                                 //     STRING_INDEX(2)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

    0x85, POWER_SUMMARY_I_SERIAL_NUMBER,        //     REPORT_ID (POWER_SUMMARY_I_SERIAL_NUMBER)
    0x09, 0xff,                                 //     USAGE(iSerialNumber)
    0x79, 0x04,                                 //     STRING_INDEX(4)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

    0x05, 0x85,                                 //     USAGE_PAGE (Battery System)
    0x85, POWER_SUMMARY_I_DEVICE_CHEMISTRY,     //     REPORT_ID (POWER_SUMMARY_I_DEVICE_CHEMISTRY)
    0x79, 0x03,                                 //     STRING_INDEX(3)
    0x09, 0x89,                                 //     USAGE (iDeviceChemistery)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

    0x85, POWER_SUMMARY_I_OEM_INFORMATION,      //     REPORT_ID (POWER_SUMMARY_I_OEM_INFORMATION)
    0x79, 0x01,                                 //     STRING_INDEX(1)
    0x09, 0x8F,                                 //     USAGE (iOEMInformation)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)


    0x05, 0x84,                                 //     USAGE_PAGE (Power Device)
    0x85, POWER_SUMMARY_I_MANUFACTURER_NAME,    //     REPORT_ID (POWER_SUMMARY_I_MANUFACTURER_NAME)
    0x79, 0x01,                                 //     STRING_INDEX(1)
    0x09, 0xFD,                                 //     USAGE (iManufacturerName)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

    0x85, POWER_SUMMARY_I_MANUFACTURER,         //     REPORT_ID (POWER_SUMMARY_I_MANUFACTURER)
    0x79, 0x01,                                 //     STRING_INDEX(1)
    0x09, 0x87,                                 //     USAGE (iManufacturer)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

	0x85, POWER_SUMMARY_VOLTAGE_INFO,           //     REPORT_ID (POWER_SUMMARY_VOLTAGE_INFO)
	0x09, 0x40,                                 //     USAGE (ConfigVoltage)
	0x67, 0x21, 0xd1, 0xf0, 0x00,               //     UNIT (SI Lin:Volts)
	0x55, 0x05,                                 //     UNIT_EXPONENT (5)
	0x75, 0x10,                                 //     REPORT_SIZE (16)
	0x27, 0xff, 0xff, 0x00, 0x00,               //     LOGICAL_MAXIMUM (65535)
	0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)
	0x09, 0x30,                                 //     USAGE (Voltage)
	0xb1, 0x82,                                 //     FEATURE (Data,Var,Abs,Vol)

	0x85, POWER_SUMMARY_CURRENT_INFO,           //     REPORT_ID (POWER_SUMMARY_CURRENT_INFO)
	0x09, 0x41,                                 //     USAGE (ConfigCurrent)
    0x67, 0x01, 0x00, 0x10, 0x00,               //     UNIT (SI Lin:Amps)
    0x55, 0x0e,                                 //     UNIT_EXPONENT (-2)
	0x75, 0x10,                                 //     REPORT_SIZE (16)
	0x27, 0xff, 0xff, 0x00, 0x00,               //     LOGICAL_MAXIMUM (65535)
	0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)
	0x09, 0x31,                                 //     USAGE (Current)
	0xb1, 0x82,                                 //     FEATURE (Data,Var,Abs,Vol)

    0x85, POWER_SUMMARY_STATUS_INFO,            //     REPORT_ID (POWER_SUMMARY_STATUS_INFO)
    0x09, 0x02,                                 //     USAGE (PresentStatus)
    0xa1, 0x02,                                 //     COLLECTION (Logical)
    0x09, 0x60,                                 //       USAGE (Present)
    0x09, 0x61,                                 //       USAGE (Good)
    0x09, 0x62,                                 //       USAGE (InternalFailure)
    0x09, 0x65,                                 //       USAGE (Overload)
    0x09, 0x66,                                 //       USAGE (OverCharged)
    0x09, 0x67,                                 //       USAGE (OverTemperature)
    0x09, 0x68,                                 //       USAGE (ShutdownRequested)
    0x09, 0x69,                                 //       USAGE (ShutdownImminent)
    0x05, 0x85,                                 //       USAGE_PAGE (Battery System)
    0x09, 0x42,                                 //       USAGE (BelowRemainingCapacityLimit)
    0x09, 0x43,                                 //       USAGE (RemainingTimeLimitExpired)
    0x09, 0x44,                                 //       USAGE (Charging)
    0x09, 0x45,                                 //       USAGE (Discharging)
    0x09, 0x48,                                 //       USAGE (ConditionningFlag)
    0x09, 0x4b,                                 //       USAGE (NeedReplacement)
    0x09, 0xd0,                                 //       USAGE (ACPresent)
    0x09, 0xd1,                                 //       USAGE (BatteryPresent)
    0x15, 0x00,                                 //       LOGICAL_MINIMUM (0)
    0x25, 0x01,                                 //       LOGICAL_MAXIMUM (1)
    0x75, 0x01,                                 //       REPORT_SIZE (1)
    0x95, 0x10,                                 //       REPORT_COUNT (16)
    0xb1, 0x83,                                 //       FEATURE (Cnst,Var,Abs,Vol)
    0xc0,                                       //     END_COLLECTION
    0xa1, 0x02,                                 //     COLLECTION (Logical)
    0x05, 0x84,                                 //       USAGE_PAGE (Power Device)
    0x09, 0x60,                                 //       USAGE (Present)
    0x09, 0x61,                                 //       USAGE (Good)
    0x09, 0x62,                                 //       USAGE (InternalFailure)
    0x09, 0x65,                                 //       USAGE (Overload)
    0x09, 0x66,                                 //       USAGE (OverCharged)
    0x09, 0x67,                                 //       USAGE (OverTemperature)
    0x09, 0x68,                                 //       USAGE (ShutdownRequested)
    0x09, 0x69,                                 //       USAGE (ShutdownImminent)
    0x05, 0x85,                                 //       USAGE_PAGE (Battery System)
    0x09, 0x42,                                 //       USAGE (BelowRemainingCapacityLimit)
    0x09, 0x43,                                 //       USAGE (RemainingTimeLimitExpired)
    0x09, 0x44,                                 //       USAGE (Charging)
    0x09, 0x45,                                 //       USAGE (Discharging)
    0x09, 0x48,                                 //       USAGE (ConditionningFlag)
    0x09, 0x4b,                                 //       USAGE (NeedReplacement)
    0x09, 0xd0,                                 //       USAGE (ACPresent)
    0x09, 0xd1,                                 //       USAGE (BatteryPresent)
    0x15, 0x00,                                 //       LOGICAL_MINIMUM (0)
    0x25, 0x01,                                 //       LOGICAL_MAXIMUM (1)
    0x75, 0x01,                                 //       REPORT_SIZE (1)
    0x95, 0x10,                                 //       REPORT_COUNT (16)
    0x81, 0x82,                                 //       INPUT (Data,Var,Abs,Vol)
    0xc0,                                       //     END_COLLECTION

    0x85, POWER_SUMMARY_CAPACITY_MODE,          //     REPORT_ID (POWER_SUMMARY_CAPACITY_MODE)
    0x09, 0x2c,                                 //     USAGE (CapacityMode)
    0x75, 0x08,                                 //     REPORT_SIZE (8)
    0x95, 0x01,                                 //     REPORT_COUNT (1)
    0x15, 0x00,                                 //     LOGICAL_MINIMUM (0)
    0x25, 0x03,                                 //     LOGICAL_MAXIMUM (3)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

    0x85, POWER_SUMMARY_DESIGN_CAPACITY,        //     REPORT_ID (POWER_SUMMARY_DESIGN_CAPACITY)
    0x09, 0x83,                                 //     USAGE (DesignCapacity)
    0x75, 0x08,                                 //     REPORT_SIZE(8)
    0x95, 0x01,                                 //     REPORT_COUNT(1)
    0x65, 0x00,                                 //     UNIT(None)
    0x55, 0x00,                                 //     UNIT_EXPONENT(0)
    0x15, 0x00,                                 //     LOGICAL_MINIMUM(0)
    0x25, 100,                                  //     LOGICAL_MAXIMUM(100)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

    0x85, POWER_SUMMARY_REMAINING_CAPACITY,     //     REPORT_ID (POWER_SUMMARY_REMAINING_CAPACITY)
    0x09, 0x66,                                 //     USAGE (RemainingCapacity)
    0x81, 0x82,                                 //     INPUT (Data,Var,Abs,Vol)
    0x09, 0x66,                                 //     USAGE (RemainingCapacity)
    0xb1, 0x82,                                 //     FEATURE (Data,Var,Abs,Vol)  

    0x85, POWER_SUMMARY_FULL_CHARGE_CAPACITY,   //     REPORT_ID (POWER_SUMMARY_FULL_CHARGE_CAPACITY)
    0x09, 0x67,                                 //     USAGE (FullChargeCapacity)
    0xb1, 0x02,                                 //     FEATURE (Data,Var,Abs)

    0x85, POWER_SUMMARY_REMAINING_CAPACITY_LIMIT,//     REPORT_ID (POWER_SUMMARY_REMAINING_CAPACITY_LIMIT)
    0x09, 0x29,                                 //     USAGE (RemainingCapacityLimit)
    0xb1, 0x82,                                 //     FEATURE (Data,Var,Abs,Vol)    

    0x85, POWER_SUMMARY_WARNING_CAPACITY_LIMIT, //     REPORT_ID (POWER_SUMMARY_WARNING_CAPACITY_LIMIT)
    0x09, 0x8c,                                 //     USAGE (WarningCapacityLimit)
    0xb1, 0x82,                                 //     FEATURE (Data,Var,Abs,Vol) 

    0x85, POWER_SUMMARY_CAPACITY_GRANULARITY_1, //     REPORT_ID (POWER_SUMMARY_CAPACITY_GRANULARITY_1)
    0x09, 0x8d,                                 //     USAGE (CapacityGranularity1)
    0xb1, 0x82,                                 //     FEATURE (Data,Var,Abs,Vol) 

    0x85, POWER_SUMMARY_CAPACITY_GRANULARITY_2, //     REPORT_ID (POWER_SUMMARY_CAPACITY_GRANULARITY_2)
    0x09, 0x8e,                                 //     USAGE (CapacityGranularity2)
    0xb1, 0x82,                                 //     FEATURE (Data,Var,Abs,Vol) 

    0x85, POWER_SUMMARY_RUN_TIME_TO_EMPTY,      //     REPORT_ID (POWER_SUMMARY_RUN_TIME_TO_EMPTY)
    0x09, 0x68,                                 //     USAGE (RunTimeToEmpty)
    0x75, 0x18,                                 //     REPORT_SIZE (24)
    0x95, 0x01,                                 //     REPORT_COUNT (1)
    0x66, 0x01, 0x10,                           //     UNIT (SI Lin:Time)
    0x55, 0x00,                                 //     UNIT_EXPONENT (0)
    0x15, 0x00,                                 //     LOGICAL_MINIMUM (0)
    0x27, 0xff, 0xff, 0xff, 0x00,               //     LOGICAL_MAXIMUM (16777215)
    0xb1, 0x82,                                 //     FEATURE (Data,Var,Abs,Vol) 
    0x09, 0x68,                                 //     USAGE (RunTimeToEmpty)
    0x81, 0x82,                                 //     INPUT (Data,Var,Abs,Vol) 
 
    0xc0,                                       //   END_COLLECTION

    0xc0,                                       // END_COLLECTION
    }
};

//********************************************************************
//********************Pure Spec***************************************
//********************************************************************
#if 0
    0x05, 0x84,                    // USAGE_PAGE (Power Device)
    0x09, 0x04,                    // USAGE (UPS)
    0xa1, 0x01,                    // COLLECTION (Application)

    //Main AC Flow
    0x05, 0x84,                    //   USAGE_PAGE (Power Device)
    0x09, 0x1e,                    //   USAGE (Flow)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x01,                    //     REPORT_ID (1)
    0x09, 0x1f,                    //     USAGE (FlowID)
    0x65, 0x00,                    //     UNIT (None)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //     LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //     UNIT (None)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x01,                    //     USAGE (iName)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x40,                    //     USAGE (ConfigVoltage)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x67, 0x21, 0xd1, 0xf0, 0x00,  //     UNIT (SI Lin:Volts)
    0x55, 0x07,                    //     UNIT_EXPONENT (7)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xfa, 0x00,              //     LOGICAL_MAXIMUM (250)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x42,                    //     USAGE (ConfigFrequency)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x66, 0x01, 0xf0,              //     UNIT (SI Lin:Hertz)
    0x55, 0x00,                    //     UNIT_EXPONENT (0)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x3c,                    //     LOGICAL_MAXIMUM (60)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0xc0,                          //   END_COLLECTION

    //Back DC Flow
    0x05, 0x84,                    //   USAGE_PAGE (Power Device)
    0x09, 0x1e,                    //   USAGE (Flow)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x02,                    //     REPORT_ID (2)
    0x09, 0x1f,                    //     USAGE (FlowID)
    0x65, 0x00,                    //     UNIT (None)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //     LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //     UNIT (None)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x01,                    //     USAGE (iName)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x40,                    //     USAGE (ConfigVoltage)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x67, 0x21, 0xd1, 0xf0, 0x00,  //     UNIT (SI Lin:Volts)
    0x55, 0x05,                    //     UNIT_EXPONENT (5)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x42,                    //     USAGE (ConfigFrequency)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x66, 0x01, 0xf0,              //     UNIT (SI Lin:Hertz)
    0x55, 0x00,                    //     UNIT_EXPONENT (0)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x3c,                    //     LOGICAL_MAXIMUM (60)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0xc0,                          //   END_COLLECTION

    //Output AC Flow
    0x05, 0x84,                    //   USAGE_PAGE (Power Device)
    0x09, 0x1e,                    //   USAGE (Flow)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x03,                    //     REPORT_ID (3)
    0x09, 0x1f,                    //     USAGE (FlowID)
    0x65, 0x00,                    //     UNIT (None)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //     LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //     UNIT (None)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x01,                    //     USAGE (iName)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x40,                    //     USAGE (ConfigVoltage)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x67, 0x21, 0xd1, 0xf0, 0x00,  //     UNIT (SI Lin:Volts)
    0x55, 0x07,                    //     UNIT_EXPONENT (7)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xfa, 0x00,              //     LOGICAL_MAXIMUM (250)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x42,                    //     USAGE (ConfigFrequency)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x66, 0x01, 0xf0,              //     UNIT (SI Lin:Hertz)
    0x55, 0x00,                    //     UNIT_EXPONENT (0)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x3c,                    //     LOGICAL_MAXIMUM (60)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x43,                    //     USAGE (ConfigApparentPower)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x66, 0x21, 0xd1,              //     UNIT (SI Lin:Power)
    0x55, 0x07,                    //     UNIT_EXPONENT (7)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0xc0,                          //   END_COLLECTION

    //Battery System
    //--Header
    0x05, 0x84,                    //   USAGE_PAGE (Power Device)
    0x09, 0x10,                    //   USAGE (BatterySystem)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x04,                    //     REPORT_ID (4)
    0x09, 0x11,                    //     USAGE (BatterySystemID)
    0x65, 0x00,                    //     UNIT (None)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //     LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //     UNIT (None)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)

    //--AC Input
    0x09, 0x1a,                    //     USAGE (Input)
    0xa1, 0x00,                    //     COLLECTION (Physical)

    0x09, 0x1b,                    //       USAGE (InputID)
    0x09, 0x1f,                    //       USAGE (FlowID)
    0x75, 0x04,                    //       REPORT_SIZE (4)
    0x95, 0x02,                    //       REPORT_COUNT (2)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //       LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //       UNIT (None)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)

    0x09, 0x02,                    //       USAGE (PresentStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x6d,                    //         USAGE (Used)
    0x09, 0x61,                    //         USAGE (Good)
    0x75, 0x01,                    //         REPORT_SIZE (1)
    0x95, 0x02,                    //         REPORT_COUNT (2)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0xb1, 0x83,                    //         FEATURE (Cnst,Var,Abs,Vol)
    0x95, 0x06,                    //         REPORT_COUNT (6)
    0xb1, 0x03,                    //         FEATURE (Cnst,Var,Abs)
    0xc0,                          //       END_COLLECTION

    0x09, 0x03,                    //       USAGE (ChangedStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x6d,                    //         USAGE (Used)
    0x09, 0x61,                    //         USAGE (Good)
    0x75, 0x01,                    //         REPORT_SIZE (1)
    0x95, 0x02,                    //         REPORT_COUNT (2)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0x81, 0x82,                    //         INPUT (Data,Var,Abs,Vol)
    0x95, 0x06,                    //         REPORT_COUNT (6)
    0x81, 0x03,                    //         INPUT (Const,Var,Abs)
    0xc0,                          //       END_COLLECTION
    0xc0,                          //     END_COLLECTION

    //--Charger
    0x09, 0x14,                    //     USAGE (Charger)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x85, 0x05,                    //       REPORT_ID (5)
    0x09, 0x15,                    //       USAGE (ChargerID)
    0x75, 0x04,                    //       REPORT_SIZE (4)
    0x95, 0x01,                    //       REPORT_COUNT (1)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //       LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //       UNIT (None)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0xc0,                          //     END_COLLECTION

    //--DC Output
    0x09, 0x1c,                    //     USAGE (Output)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x09, 0x1d,                    //       USAGE (OutputID)
    0x09, 0x1f,                    //       USAGE (FlowID)
    0x75, 0x04,                    //       REPORT_SIZE (4)
    0x95, 0x02,                    //       REPORT_COUNT (2)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //       LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //       UNIT (None)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0xc0,                          //     END_COLLECTION

    //--Battery 
    0x09, 0x12,                    //     USAGE (Battery)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x85, 0x06,                    //       REPORT_ID (6)
    0x09, 0x13,                    //       USAGE (BatteryID)
    0x75, 0x04,                    //       REPORT_SIZE (4)
    0x95, 0x01,                    //       REPORT_COUNT (1)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //       LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //       UNIT (None)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0x05, 0x85,                    //       USAGE_PAGE (Battery System)
    0x09, 0x2c,                    //       USAGE (CapacityMode)
    0x75, 0x01,                    //       REPORT_SIZE (1)
    0x95, 0x01,                    //       REPORT_COUNT (1)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //       LOGICAL_MAXIMUM (1)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0x75, 0x03,                    //       REPORT_SIZE (3)
    0x95, 0x01,                    //       REPORT_COUNT (1)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0x09, 0x83,                    //       USAGE (DesignCapacity)
    0x75, 0x18,                    //       REPORT_SIZE (24)
    0x95, 0x01,                    //       REPORT_COUNT (1)
    0x67, 0x01, 0x00, 0x10, 0x00,  //       UNIT (SI Lin:Amps)
    0x55, 0x00,                    //       UNIT_EXPONENT (0)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0xff, 0x00,  //       LOGICAL_MAXIMUM (16777214)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0x05, 0x84,                    //       USAGE_PAGE (Power Device)
    0x09, 0x40,                    //       USAGE (ConfigVoltage)
    0x75, 0x10,                    //       REPORT_SIZE (16)
    0x95, 0x01,                    //       REPORT_COUNT (1)
    0x67, 0x21, 0xd1, 0xf0, 0x00,  //       UNIT (SI Lin:Volts)
    0x55, 0x05,                    //       UNIT_EXPONENT (5)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //       LOGICAL_MAXIMUM (65534)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0x05, 0x85,                    //       USAGE_PAGE (Battery System)
    0x09, 0x66,                    //       USAGE (RemainingCapacity)
    0x75, 0x18,                    //       REPORT_SIZE (24)
    0x95, 0x01,                    //       REPORT_COUNT (1)
    0x67, 0x01, 0x00, 0x10, 0x00,  //       UNIT (SI Lin:Amps)
    0x55, 0x00,                    //       UNIT_EXPONENT (0)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0xff, 0x00,  //       LOGICAL_MAXIMUM (16777214)
    0xb1, 0x83,                    //       FEATURE (Cnst,Var,Abs,Vol)
    0x05, 0x84,                    //       USAGE_PAGE (Power Device)
    0x65, 0x00,                    //       UNIT (None)
    0x09, 0x02,                    //       USAGE (PresentStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x61,                    //         USAGE (Good)
    0x05, 0x85,                    //         USAGE_PAGE (Battery System)
    0x09, 0x42,                    //         USAGE (BelowRemainingCapacityLimit)
    0x09, 0x44,                    //         USAGE (Charging)
    0x09, 0x45,                    //         USAGE (Discharging)
    0x75, 0x01,                    //         REPORT_SIZE (1)
    0x95, 0x04,                    //         REPORT_COUNT (4)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0x65, 0x00,                    //         UNIT (None)
    0xb1, 0x83,                    //         FEATURE (Cnst,Var,Abs,Vol)
    0xb1, 0x03,                    //         FEATURE (Cnst,Var,Abs)
    0xc0,                          //       END_COLLECTION
    0x05, 0x84,                    //       USAGE_PAGE (Power Device)
    0x09, 0x03,                    //       USAGE (ChangedStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x61,                    //         USAGE (Good)
    0x05, 0x85,                    //         USAGE_PAGE (Battery System)
    0x09, 0x42,                    //         USAGE (BelowRemainingCapacityLimit)
    0x09, 0x44,                    //         USAGE (Charging)
    0x09, 0x45,                    //         USAGE (Discharging)
    0x75, 0x02,                    //         REPORT_SIZE (2)
    0x95, 0x04,                    //         REPORT_COUNT (4)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0x81, 0x82,                    //         INPUT (Data,Var,Abs,Vol)
    0xc0,                          //       END_COLLECTION
    0xc0,                          //     END_COLLECTION

    0xc0,                          //   END_COLLECTION

    //Power Converter
    0x05, 0x84,                    //   USAGE_PAGE (Power Device)
    0x09, 0x16,                    //   USAGE (PowerConverter)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x08,                    //     REPORT_ID (8)
    0x09, 0x17,                    //     USAGE (PowerConverterID)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //     LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //     UNIT (None)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x1a,                    //     USAGE (Input)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x09, 0x1b,                    //       USAGE (InputID)
    0x09, 0x1f,                    //       USAGE (FlowID)
    0x75, 0x04,                    //       REPORT_SIZE (4)
    0x95, 0x02,                    //       REPORT_COUNT (2)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //       LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //       UNIT (None)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0x09, 0x02,                    //       USAGE (PresentStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x6d,                    //         USAGE (Used)
    0x09, 0x61,                    //         USAGE (Good)
    0x75, 0x01,                    //         REPORT_SIZE (1)
    0x95, 0x02,                    //         REPORT_COUNT (2)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0xb1, 0x83,                    //         FEATURE (Cnst,Var,Abs,Vol)
    0x95, 0x06,                    //         REPORT_COUNT (6)
    0xb1, 0x03,                    //         FEATURE (Cnst,Var,Abs)
    0xc0,                          //       END_COLLECTION
    0x09, 0x03,                    //       USAGE (ChangedStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x6d,                    //         USAGE (Used)
    0x09, 0x61,                    //         USAGE (Good)
    0x75, 0x02,                    //         REPORT_SIZE (2)
    0x95, 0x02,                    //         REPORT_COUNT (2)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0x81, 0x82,                    //         INPUT (Data,Var,Abs,Vol)
    0x95, 0x02,                    //         REPORT_COUNT (2)
    0x81, 0x03,                    //         INPUT (Const,Var,Abs)
    0xc0,                          //       END_COLLECTION
    0xc0,                          //     END_COLLECTION
    0x09, 0x1c,                    //     USAGE (Output)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x85, 0x09,                    //       REPORT_ID (9)
    0x09, 0x1d,                    //       USAGE (OutputID)
    0x09, 0x1f,                    //       USAGE (FlowID)
    0x75, 0x04,                    //       REPORT_SIZE (4)
    0x95, 0x02,                    //       REPORT_COUNT (2)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //       LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //       UNIT (None)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0x09, 0x35,                    //       USAGE (PercentLoad)
    0x75, 0x08,                    //       REPORT_SIZE (8)
    0x95, 0x01,                    //       REPORT_COUNT (1)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //       LOGICAL_MAXIMUM (255)
    0x81, 0x83,                    //       INPUT (Cnst,Var,Abs,Vol)
    0x09, 0x02,                    //       USAGE (PresentStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x6d,                    //         USAGE (Used)
    0x09, 0x61,                    //         USAGE (Good)
    0x09, 0x65,                    //         USAGE (Overload)
    0x09, 0x69,                    //         USAGE (ShutdownImminent)
    0x75, 0x01,                    //         REPORT_SIZE (1)
    0x95, 0x04,                    //         REPORT_COUNT (4)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0xb1, 0x83,                    //         FEATURE (Cnst,Var,Abs,Vol)
    0x95, 0x04,                    //         REPORT_COUNT (4)
    0xb1, 0x03,                    //         FEATURE (Cnst,Var,Abs)
    0xc0,                          //       END_COLLECTION
    0x09, 0x03,                    //       USAGE (ChangedStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x6d,                    //         USAGE (Used)
    0x09, 0x61,                    //         USAGE (Good)
    0x09, 0x65,                    //         USAGE (Overload)
    0x09, 0x69,                    //         USAGE (ShutdownImminent)
    0x75, 0x02,                    //         REPORT_SIZE (2)
    0x95, 0x04,                    //         REPORT_COUNT (4)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0x81, 0x82,                    //         INPUT (Data,Var,Abs,Vol)
    0xc0,                          //       END_COLLECTION
    0xc0,                          //     END_COLLECTION
    0x09, 0x1a,                    //     USAGE (Input)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x85, 0x0a,                    //       REPORT_ID (10)
    0x09, 0x1b,                    //       USAGE (InputID)
    0x09, 0x1f,                    //       USAGE (FlowID)
    0x75, 0x04,                    //       REPORT_SIZE (4)
    0x95, 0x02,                    //       REPORT_COUNT (2)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //       LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //       UNIT (None)
    0xb1, 0x03,                    //       FEATURE (Cnst,Var,Abs)
    0x09, 0x02,                    //       USAGE (PresentStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x6d,                    //         USAGE (Used)
    0x09, 0x61,                    //         USAGE (Good)
    0x75, 0x01,                    //         REPORT_SIZE (1)
    0x95, 0x02,                    //         REPORT_COUNT (2)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0xb1, 0x83,                    //         FEATURE (Cnst,Var,Abs,Vol)
    0x95, 0x06,                    //         REPORT_COUNT (6)
    0xb1, 0x03,                    //         FEATURE (Cnst,Var,Abs)
    0xc0,                          //       END_COLLECTION
    0x09, 0x03,                    //       USAGE (ChangedStatus)
    0xa1, 0x02,                    //       COLLECTION (Logical)
    0x09, 0x6d,                    //         USAGE (Used)
    0x09, 0x61,                    //         USAGE (Good)
    0x75, 0x02,                    //         REPORT_SIZE (2)
    0x95, 0x02,                    //         REPORT_COUNT (2)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0x81, 0x82,                    //         INPUT (Data,Var,Abs,Vol)
    0x95, 0x02,                    //         REPORT_COUNT (2)
    0x81, 0x03,                    //         INPUT (Const,Var,Abs)
    0xc0,                          //       END_COLLECTION
    0xc0,                          //     END_COLLECTION
    0xc0,                          //   END_COLLECTION

    //Power Summary
    0x09, 0x24,                    //   USAGE (Sink)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x0b,                    //     REPORT_ID (11)

    0x09, 0x25,                    //     USAGE (SinkID)
    0x09, 0x1f,                    //     USAGE (FlowID)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x0f,                    //     LOGICAL_MAXIMUM (15)
    0x65, 0x00,                    //     UNIT (None)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)

    0x09, 0x01,                    //     USAGE (iName)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x65, 0x00,                    //     UNIT (None)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)

    0x05, 0x85,                    //     USAGE_PAGE (Battery System)
    0x09, 0xd1,                    //     USAGE (BatteryPresent)
    0x09, 0x2c,                    //     USAGE (CapacityMode)
    0x09, 0x8b,                    //     USAGE (Rechargeable)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)
    0x95, 0x05,                    //     REPORT_COUNT (5)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)

    0x05, 0x85,                    //     USAGE_PAGE (Battery System)
    0x09, 0x83,                    //     USAGE (DesignCapacity)
    0x09, 0x8c,                    //     USAGE (WarningCapacityLimit)
    0x09, 0x8d,                    //     USAGE (CapacityGranularity1)
    0x09, 0x8e,                    //     USAGE (CapacityGranularity2)
    0x75, 0x18,                    //     REPORT_SIZE (24)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x67, 0x01, 0x00, 0x10, 0x00,  //     UNIT (SI Lin:Amps)
    0x55, 0x00,                    //     UNIT_EXPONENT (0)
    0x27, 0xfe, 0xff, 0xff, 0x00,  //     LOGICAL_MAXIMUM (16777214)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)

    0x05, 0x84,                    //     USAGE_PAGE (Power Device)
    0x09, 0x40,                    //     USAGE (ConfigVoltage)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x67, 0x21, 0xd1, 0xf0, 0x00,  //     UNIT (SI Lin:Volts)
    0x55, 0x05,                    //     UNIT_EXPONENT (5)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)

    0x05, 0x84,                    //     USAGE_PAGE (Power Device)
    0x09, 0xFE,                    //     USAGE (iProduct)
    0x09, 0xFF,                    //     USAGE (iserialNumber)
    0x05, 0x85,                    //     USAGE_PAGE (Battery System)
    0x09, 0x89,                    //     USAGE (iDeviceChemistery)
    0x09, 0x87,                    //     USAGE (iManufacturerName)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x65, 0x00,                    //     UNIT (None)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)

    0x05, 0x84,                    //     USAGE_PAGE (Power Device)
    0x09, 0x35,                    //     USAGE (PercentLoad)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x26, 0xfe, 0x00,              //     LOGICAL_MAXIMUM (254)
    0x65, 0x00,                    //     UNIT (None)
    0x81, 0x82,                    //     INPUT (Data,Var,Abs,Vol)

    0x05, 0x84,                    //     USAGE_PAGE (Power Device)
    0x09, 0x30,                    //     USAGE (Voltage)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x09, 0x30,                    //     USAGE (Voltage)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x67, 0x21, 0xd1, 0xf0, 0x00,  //     UNIT (SI Lin:Volts)
    0x55, 0x05,                    //     UNIT_EXPONENT (5)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0xb1, 0x82,                    //     FEATURE (Data,Var,Abs,Vol)

    0x09, 0x31,                    //     USAGE (Current)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x67, 0x01, 0x00, 0x10, 0x00,  //     UNIT (SI Lin:Amps)
    0x55, 0x0e,                    //     UNIT_EXPONENT (-2)
    0xb1, 0x82,                    //     FEATURE (Data,Var,Abs,Vol)

    0x05, 0x85,                    //     USAGE_PAGE (Battery System)
    0x09, 0x67,                    //     USAGE (FullChargeCapacity)
    0x75, 0x18,                    //     REPORT_SIZE (24)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x66, 0x21, 0xd1,              //     UNIT (SI Lin:Power)
    0x55, 0x00,                    //     UNIT_EXPONENT (0)
    0x27, 0xfe, 0xff, 0xff, 0x00,  //     LOGICAL_MAXIMUM (16777214)
    0xb1, 0x82,                    //     FEATURE (Data,Var,Abs,Vol)

    0x09, 0x66,                    //     USAGE (RemainingCapacity)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x82,                    //     INPUT (Data,Var,Abs,Vol)
    0x09, 0x68,                    //     USAGE (RunTimeToEmpty)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x66, 0x01, 0x10,              //     UNIT (SI Lin:Time)
    0x55, 0x00,                    //     UNIT_EXPONENT (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0x81, 0x82,                    //     INPUT (Data,Var,Abs,Vol)

	0x05, 0x84,                    //     USAGE_PAGE (Power Device)
	0x09, 0x02,                    //     USAGE (PresentStatus)
	0xa1, 0x02,                    //     COLLECTION (Logical)
	0x05, 0x85,                    //       USAGE_PAGE (Battery)
	0x09, 0xd0,                    //       USAGE (ACPresent)
    0x09, 0x44,                    //       USAGE (Charging)
	0x09, 0x45,                    //       USAGE (Discharging)
	0x09, 0x42,                    //       USAGE (BelowRemainingCapacityLimit)
	0x05, 0x84,                    //       USAGE_PAGE (Power Device)
	0x09, 0x69,                    //       USAGE (ShutdownImminent)
	0x09, 0x65,                    //       USAGE (Overload)
	0x75, 0x01,                    //       REPORT_SIZE (1)
    0x95, 0x06,                    //       REPORT_COUNT (6)
	0x25, 0x01,                    //       LOGICAL_MAXIMUM (1)
	0x81, 0x82,                    //       INPUT (Data,Var,Abs,Vol)
    0x95, 0x02,                    //       REPORT_COUNT (2)
	0x81, 0x03,                    //       INPUT (Cnst,Var,Abs)
	0xc0,                          //     END_COLLECTION (PresentStatus)

    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
#endif

//Array of configuration descriptors
ROM BYTE *ROM USB_CD_Ptr[]=
{
    (ROM BYTE *ROM)&configDescriptor1
};

/** EOF usb_descriptors.c ***************************************************/

#endif
