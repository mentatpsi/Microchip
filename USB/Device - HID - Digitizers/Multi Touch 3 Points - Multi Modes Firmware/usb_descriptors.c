/********************************************************************
 FileName:     	usb_descriptors.c
 Dependencies:	See INCLUDES section
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

********************************************************************************

 Change History:
  Rev   Description
  ----  -----------------------------------------
  2.7   Updated descriptors to fix some WHQL certification related
        issues
  2.7b  Minor change to enable and demonstrate USB remote wakeup.

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

/** CONSTANTS ******************************************************/
#if defined(__18CXX)
#pragma romdata
#endif

/* Device Descriptor */
ROM USB_DEVICE_DESCRIPTOR device_dsc=
{
    0x12,    // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,  // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_config.h
    MY_VID,                 // Vendor ID
    MY_PID,                 // Product ID
    0x0001,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
ROM BYTE configDescriptor1[]={
    /* Configuration Descriptor */
    0x09,//sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,  // CONFIGURATION descriptor type
    DESC_CONFIG_WORD(0x0022),   // Total length of data for this cfg
    1,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    0,                      // Configuration string index
    _DEFAULT | _SELF | _RWU,// Attributes, see usb_device.h
    50,                     // Max power consumption (2X mA)

    /* Interface Descriptor */
    0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,      // INTERFACE descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    1,                      // Number of endpoints in this intf
    HID_INTF,               // Class code
    0,					    // Subclass code
    0,    // Protocol code
    0,                      // Interface string index

    /* HID Class-Specific Descriptor */
    0x09,//sizeof(USB_HID_DSC)+3,    // Size of this descriptor in bytes 
    DSC_HID,                // HID descriptor type
    DESC_CONFIG_WORD(0x0112),        // HID Spec Release Number in BCD format (1.12)
    0x00,                   // Country Code (0x00 for Not supported)
    HID_NUM_OF_DSC,         // Number of class descriptors, see usbcfg.h
    DSC_RPT,                // Report descriptor type
    DESC_CONFIG_WORD(HID_RPT01_SIZE),   //sizeof(hid_rpt01),  // Size of the report descriptor
    
    /* Endpoint Descriptor */
    0x07,/*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    HID_EP | _EP_IN,            //EndpointAddress
    _INTERRUPT,                 //Attributes
    DESC_CONFIG_WORD(64),       //size
    0x04                        //Interval in ms.  4ms = up to 250Hz update rate.
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
ROM struct{BYTE bLength;BYTE bDscType;WORD string[20];}sd002={
sizeof(sd002),USB_DESCRIPTOR_STRING,
{'H','I','D',' ','M','u','l','t','i','-','T','o','u','c','h',' ','D','e','m','o'}};


//Class specific descriptor - HID report descriptor:

//In order to understand the HID report descriptor, it is helpful to become familiar
//with the HID specifications (HID1_11.pdf was used to develop this example), and the
//"HUT" (HID Usage Tables) specifications (Hut1_12.pdf was used during development of this example).
//These documents can be found at the USB implementers forum website: www.usb.org 

//Additionally, Microsoft provides a few reference documents pertaining to the development of
//HID touch/multi-touch devices for Windows Touch enabled operating systems.
//The below example HID report descriptor is almost identical to one of Microsoft's example
//descriptors.  Microsoft provides some example descriptors and background information in
//the following documents:

//"Digitizer Drivers for Windows Touch and Pen-Based Computers" (publicly available download: DigitizerDrvs_touch.docx)
//"Human Interface Device (HID) Extensions and Windows/Device Protocol for Multi-Touch Digitizers" (not 
	//currently a public document[but might be in the future??], contact Microsoft if a 
	//public version is unavailable.  However, this document isn't critical to developing 
	//a touch/multi-touch device.  Most of the relevant content is already in the "Digitizer
	//Drivers for Windows Touch and Pen-Based Computers" document, and in Microsoft's developer 
	//blogs relating to Windows 7 Touch)


//At a minimum a multi-touch capable finger input digitizer
//must support the following report information (one set of data for each simultaneous contact supported):
//"Tip Switch" (ex: a soft button, indicating that the tip of the human's finger is touching the screen)
//"In-range" indicator: For example, if the touch screen can detect when the user's finger is 
//		hovering over an area, but not in direct contact (ex: it uses optical detection 
//		methods), it may set the in-range indicator bit, but not necessarily simultanously 
//		set the tip switch.  If a hard contact is detected, both the tip switch and in 
//		range bits should be set.
//X coordinate of contact point (see note)
//Y coordinate of contact point (see note)
//Other input usages are optional.

//NOTE (X/Y Coordinates): The origin is the upper left corner of the screen.  For the X coordinates,
//the leftmost part of the screen has X coordinate = 0.  The rightmost edge of the screen has 
//X coordinate = 4800 (for this example HID report descriptor).  The topmost part of the screen 
//has Y coordinate = 0.  The bottom most part of the screen has Y coordinate = 3000 for this 
//example HID report descriptor.  

ROM struct{BYTE report[HID_RPT01_SIZE];}hid_rpt01={
	{
//Below is the report descriptor for an example multi-touch device, which can support 
//up to two simultaneous contacts.  This report descriptor also supports multiple device modes.
//The OS may select the device mode (ex: mouse, single-touch digitizer, multi-touch digitizer)
//by sending a SET_REPORT (feature) control transfer.  In each mode, the device firmware
//should behave differently and send input packets on the HID IN endpoint, formatted
//based on the report descriptor collections associated with that mode.

//Each IN packet (HID report) sent to the host (in multi-touch digitizer mode) is 14 bytes long (Bytes 0-13).  
//The data is sent in little endian format.  To send the report to the host, verify 
//that the HID IN endpoint buffer is not already busy, then place the data in the HID 
//IN endpoint buffer SRAM, and then call the HIDTxPacket() function.  The data will 
//then get sent to the host, at the next opportunity when the host polls that endpoint 
//(by sending an IN token packet to the endpoint).

//NOTE (Contact identifier number): For a multi-touch device, the firmware needs to 
//keep track of each contact point separately and independantly.  For example, suppose 
//a human first presses one finger to the screen.  The firmware would detect this, and 
//it should arbitrarily assign a contact identifier number for this contact. Typically 
//it would be assigned something like contact ID = "1". As the human moves their finger 
//around on the contact surface the firmware will report new X and Y coordinate 
//information for this contact, but the contact ID byte should always be = "1" for
//this contact.
//
//Now suppose the human places a second finger onto the touch surface, so there are 
//two simultanous contacts.  The firmware should assign this second contact a new 
//contact ID, for instance, contact ID = "2".  So long as the first contact is still
//valid, the firmware needs to continue reporting ID = "1" for the first contact.  
//The firmware will continue to report information using these contact ID numbers 
//until such time as a contact becomes invalid (human removes their finger from the 
//screen).


    //Top level collection for use in Multi-Touch Digitizer device mode.
	//-------------------------------------------------------------------
	//Format for multi-touch digitizer input reports using this report descriptor:
    //Byte[0] = Report ID == MULTI_TOUCH_DATA_REPORT_ID
	//First contact point info in bytes 1-6.
	//Byte[1] = Bits7-3: pad bits (unused), Bit1:In Range, Bit0:Tip Switch
	//Byte[2] = Contact identifier number (see note above)
	//Byte[3] = X-coordinate LSB
	//Byte[4] = X-coordinate MSB
	//Byte[5] = Y-coordinate LSB
	//Byte[6] = Y-coordinate MSB
	
	//Second contact point info in bytes 7-12
	//Byte[7] = Bits7-3: pad bits (unused), Bit1:In Range, Bit0:Tip Switch
	//Byte[8] = Contact identifier number (see note above)
	//Byte[9] = X-coordinate LSB
	//Byte[10]= X-coordinate MSB
	//Byte[11]= Y-coordinate LSB
	//Byte[12]= Y-coordinate MSB
	
	//Byte[13]= 8-bit number indicating how many of the above contact points are valid.  
	//			If only the first contact is valid, send "1" here.  If both are valid, send "2".

	//HID Report descriptor for an example Multi-Touch (2 simultaneous contacts), multi-modes device.
	//------------------------------------------------------------------------------------------------
	//Note: In a real application, at a minimum, certain terms in the report descriptor 
	//(ex: UNIT, UNIT_EXPONENT, PHYSICAL_MAXIMUM and LOGICAL_MAXIMUM) will need to be modified to
	//match the characteristics (ex: size) of the actual application being developed.  See the HID1_11.pdf
	//specifications regarding these terms.
    0x05, 0x0D,	          // USAGE_PAGE (Digitizers)          
    0x09, 0x04,	          // USAGE (Touch Screen)             
    0xA1, 0x01,           // COLLECTION (Application)         
    0x85, MULTI_TOUCH_DATA_REPORT_ID,	//   REPORT_ID (Touch)    //When the firmware wants to send a HID report (containing multi-touch contact information), the Report ID byte should match this number
    0x09, 0x22,           //   USAGE (Finger)                 
    0xA1, 0x02,           //   COLLECTION (Logical)  
    0x09, 0x42,           //       USAGE (Tip Switch)           
    0x15, 0x00,           //       LOGICAL_MINIMUM (0)          
    0x25, 0x01,           //       LOGICAL_MAXIMUM (1)          
    0x75, 0x01,           //       REPORT_SIZE (1)              
    0x95, 0x01,           //       REPORT_COUNT (1)             
    0x81, 0x02,           //       INPUT (Data,Var,Abs) 		//Makes one, 1-bit field for tip switch
    0x09, 0x32,	          //       USAGE (In Range)             
    0x81, 0x02,           //       INPUT (Data,Var,Abs)         //Makes one, 1-bit field for In Range indicator
    0x95, 0x06,           //       REPORT_COUNT (6)  
    0x81, 0x03,           //       INPUT (Cnst,Ary,Abs)			//Makes six, 1-bit fields, which are pad bits (no valid data)
    0x75, 0x08,           //       REPORT_SIZE (8)
    0x09, 0x51,           //       USAGE (Contact Identifier)
    0x95, 0x01,           //       REPORT_COUNT (1)        		
    0x81, 0x02,           //       INPUT (Data,Var,Abs) 		//Makes one, 8-bit field for the contact identifier number.  Firmware arbitrarily assigns a contact ID, which stays the same until the contact becomes invalid (finger removed from screen).
    0x05, 0x01,           //       USAGE_PAGE (Generic Desk..
    0x26, 0xC0, 0x12,     //       LOGICAL_MAXIMUM (4800)   	//This is the maximum value the firmware should send for the X coordinate contact point.  It can potentially make the firmware design mathematically simpler if this is set to some meaningful value, such as equal to the PHYSICAL_MAXIMUM (which specifies the dimensions of the actual touch screen, in terms of the UNITS parameter).      
    0x75, 0x10,           //       REPORT_SIZE (16)             
    0x55, 0x0E,           //       UNIT_EXPONENT (-2)       //10^(-2)           
    0x65, 0x33,           //       UNIT (Inches, English Linear)  //But exponent -2, so Physical Maximum is in 10’s of mils.                 
    0x09, 0x30,           //       USAGE (X)                    
    0x35, 0x00,           //       PHYSICAL_MINIMUM (0)         
    0x46, 0x40, 0x06,     //       PHYSICAL_MAXIMUM (0x640 = 1600)     //1600 * 10^(-2) = 16 inches X-dimension        
    0x81, 0x02,           //       INPUT (Data,Var,Abs)           //Makes one, 16-bit field for X coordinate info.  Valid values from: 0-4800 (4800 is the LOGICAL_MAXIMUM, which would correspond to far edge of the screen, which is 1600x10mil distance from X origin [left of screen])   
    0x26, 0xB8, 0x0B,     //       LOGICAL_MAXIMUM (3000)             //16:10 aspect ratio (X:Y) (ex: 4800/3000 = 16/10).  //This is the maximum value the firmware should send for the Y coordinate contact point.  It can potentially make the firmware design mathematically simpler if this is set to some meaningful value, such as equal to the PHYSICAL_MAXIMUM (which specifies the dimensions of the actual touch screen, in terms of the UNIT parameter). 
    0x46, 0xE8, 0x03,     //       PHYSICAL_MAXIMUM (0x3E8 = 1000)    //1000 * 10^(-2) = 10 inches Y-dimension       
    0x09, 0x31,           //       USAGE (Y)       
    0x81, 0x02,           //       INPUT (Data,Var,Abs)			//Makes one, 16-bit field for Y coordinate info.  Valid values from: 0-3000 (3000 is the LOGICAL_MAIXMUM, which would correspond to the far bottom of the screen, which is 1000x10mil distance from the Y origin [top of screen])   
    0xC0,                 //    END_COLLECTION
    0xA1, 0x02,           //    COLLECTION (Logical)  
    0x05, 0x0D,	          //       USAGE_PAGE (Digitizers)  
    0x09, 0x42,           //       USAGE (Tip Switch)           
    0x15, 0x00,           //       LOGICAL_MINIMUM (0)          
    0x25, 0x01,           //       LOGICAL_MAXIMUM (1)          
    0x75, 0x01,           //       REPORT_SIZE (1)              
    0x95, 0x01,           //       REPORT_COUNT (1)             
    0x81, 0x02,           //       INPUT (Data,Var,Abs) 		//Makes one, 1-bit field for Tip Switch
    0x09, 0x32,	          //       USAGE (In Range)             
    0x81, 0x02,           //       INPUT (Data,Var,Abs)         //Makes one, 1-bit field for In Range Indicator
    0x95, 0x06,           //       REPORT_COUNT (6)  
    0x81, 0x03,           //       INPUT (Cnst,Ary,Abs)			//Makes six, 1-bit fields that are pad bits (no valid data)
    0x75, 0x08,           //       REPORT_SIZE (8)
    0x09, 0x51,           //       USAGE (Contact Identifier)
    0x95, 0x01,           //       REPORT_COUNT (1)          		
    0x81, 0x02,           //       INPUT (Data,Var,Abs)			//Makes one, 8-bit field for the contact identifier number.    
    0x05, 0x01,           //       USAGE_PAGE (Generic Desk..
    0x26, 0xC0, 0x12,     //       LOGICAL_MAXIMUM (4800)         
    0x75, 0x10,           //       REPORT_SIZE (16)             
    0x55, 0x0E,           //       UNIT_EXPONENT (-2)       //10^(-2)           
    0x65, 0x33,           //       UNIT (Inches, English Linear)  //But exponent -2, so Physical Maximum is in 10’s of mils.                 
    0x09, 0x30,           //       USAGE (X)                    
    0x35, 0x00,           //       PHYSICAL_MINIMUM (0)         
    0x46, 0x40, 0x06,     //       PHYSICAL_MAXIMUM (0x640 = 1600)     //1600 * 10^(-2) = 16 inches X-dimension        
    0x81, 0x02,           //       INPUT (Data,Var,Abs)           //Makes one, 16-bit field for X coordinate info.  Valid values from: 0-4800    
    0x26, 0xB8, 0x0B,     //       LOGICAL_MAXIMUM (3000)             //16:10 aspect ratio (X:Y) 
    0x46, 0xE8, 0x03,     //       PHYSICAL_MAXIMUM (0x3E8 = 1000)    //1000 * 10^(-2) = 10 inches Y-dimension       
    0x09, 0x31,           //       USAGE (Y)                    
    0x81, 0x02,           //       INPUT (Data,Var,Abs)                 //Makes one, 16-bit field for Y coordinate info.  Valid values from: 0-3000
    0xC0,                 //   END_COLLECTION
    0xA1, 0x02,           //   COLLECTION (Logical)  
    0x05, 0x0D,	          //       USAGE_PAGE (Digitizers)  
    0x09, 0x42,           //       USAGE (Tip Switch)           
    0x15, 0x00,           //       LOGICAL_MINIMUM (0)          
    0x25, 0x01,           //       LOGICAL_MAXIMUM (1)          
    0x75, 0x01,           //       REPORT_SIZE (1)              
    0x95, 0x01,           //       REPORT_COUNT (1)             
    0x81, 0x02,           //       INPUT (Data,Var,Abs) 		//Makes one, 1-bit field for Tip Switch
    0x09, 0x32,	          //       USAGE (In Range)             
    0x81, 0x02,           //       INPUT (Data,Var,Abs)         //Makes one, 1-bit field for In Range Indicator
    0x95, 0x06,           //       REPORT_COUNT (6)  
    0x81, 0x03,           //       INPUT (Cnst,Ary,Abs)			//Makes six, 1-bit fields that are pad bits (no valid data)
    0x75, 0x08,           //       REPORT_SIZE (8)
    0x09, 0x51,           //       USAGE (Contact Identifier)
    0x95, 0x01,           //       REPORT_COUNT (1)          		
    0x81, 0x02,           //       INPUT (Data,Var,Abs)			//Makes one, 8-bit field for the contact identifier number.    
    0x05, 0x01,           //       USAGE_PAGE (Generic Desk..
    0x26, 0xC0, 0x12,     //       LOGICAL_MAXIMUM (4800)         
    0x75, 0x10,           //       REPORT_SIZE (16)             
    0x55, 0x0E,           //       UNIT_EXPONENT (-2)       //10^(-2)           
    0x65, 0x33,           //       UNIT (Inches, English Linear)  //But exponent -2, so Physical Maximum is in 10’s of mils.                 
    0x09, 0x30,           //       USAGE (X)                    
    0x35, 0x00,           //       PHYSICAL_MINIMUM (0)         
    0x46, 0x40, 0x06,     //       PHYSICAL_MAXIMUM (0x640 = 1600)     //1600 * 10^(-2) = 16 inches X-dimension        
    0x81, 0x02,           //       INPUT (Data,Var,Abs)           //Makes one, 16-bit field for X coordinate info.  Valid values from: 0-4800    
    0x26, 0xB8, 0x0B,     //       LOGICAL_MAXIMUM (3000)             //16:10 aspect ratio (X:Y) 
    0x46, 0xE8, 0x03,     //       PHYSICAL_MAXIMUM (0x3E8 = 1000)    //1000 * 10^(-2) = 10 inches Y-dimension       
    0x09, 0x31,           //       USAGE (Y)                    
    0x81, 0x02,           //       INPUT (Data,Var,Abs)                 //Makes one, 16-bit field for Y coordinate info.  Valid values from: 0-3000
    0xC0,                 //   END_COLLECTION
    0x05, 0x0D,	          //   USAGE_PAGE (Digitizers)    
    0x09, 0x54,	          //   USAGE (Actual (contact) count)
    0x95, 0x01,           //   REPORT_COUNT (1)
    0x75, 0x08,           //   REPORT_SIZE (8)    
    0x25, MAX_VALID_CONTACT_POINTS,  //   LOGICAL_MAXIMUM (3)				//Maximum number of valid contact points simutaneously supported.  Increase this number if supporting more than 3 simultaneous contacts.
    0x81, 0x02,           //   INPUT (Data,Var,Abs)				//Makes one, 8-bit field for the actual number of valid contacts reported (valid values: 1 and 2)

    //In addition to standard HID reports (containing contact X/Y/tip-switch/in-range 
	//information), the firmware can send feature report information to the host.  
	//Feature reports lets the host know information about the firmware/hardware 
	//application design.  The host will typically request a feature report by sending 
	//a GET_REPORT control transfer, with the bRequest = REPORT, but with wIndex (MSB) = 0x03
	//(get feature report request as indicated in the HID1_11.pdf specifications). If a 
	//feature report is sent to the host, it should be sent as a 2-byte packet (for 
	//this example project), formatted like follows:
	//Byte[0]= Report ID == VALID_CONTACTS_FEATURE_REPORT_ID == Feature report ID
	//Byte[1]= Maximum number of contacts simultaneously supported by this application.  Always == 2 in this demo, since this report descriptor only has input fields for 2 contact points
	//See the UserGetReportHandler() function that sends this type of packet in the MultiTouch.c file.
    0x85, VALID_CONTACTS_FEATURE_REPORT_ID,  //   REPORT_ID (Feature)   //When the firmware wants to send a feature report (containing the number of valid contacts that can be reported), the Report ID byte should match this number.     
    0x09, 0x55,           //   USAGE(Maximum Count)
    0xB1, 0x02,           //   FEATURE (Data,Var,Abs) 			//Makes one, 8-bit field for reporting the maximum number of simultaneous contacts supported by the device.  Since this report descriptor contains two logical collections (one for each contact point), the firmware should always send 0x02 in response to a get feature report request.
    0xC0,                 // END_COLLECTION


	//Top level collection describing the feature report that the host will use (SET_REPORT (feature) control transfer) to set the device mode (ex: mouse, single-touch digitizer, multi-touch digitizer)
	//-------------------------------------------------------------------
	//Format of the feature report output data that the host will send to the device (when it wants to change the device mode):
	//byte[0] = Report ID = DEVICE_MODE_FEATURE_REPORT_ID
	//byte[1] = Device Mode (that the device should switch into)
	//byte[2] = Device Identifier
    0x09, 0x0E,           // USAGE (Device Configuration)
    0xa1, 0x01,           // COLLECTION (Application)
    0x85, DEVICE_MODE_FEATURE_REPORT_ID,     //   REPORT_ID (Configuration)              
    0x09, 0x23,           //   USAGE (Device Settings)              
    0xa1, 0x02,           //   COLLECTION (logical)    
    0x09, 0x52,           //    USAGE (Device Mode)         
    0x09, 0x53,           //    USAGE (Device Identifier)
    0x15, 0x00,           //    LOGICAL_MINIMUM (0)      
    0x25, 0x0a,           //    LOGICAL_MAXIMUM (10)
    0x75, 0x08,           //    REPORT_SIZE (8)         
    0x95, 0x02,           //    REPORT_COUNT (2)         
    0xb1, 0x02,           //   FEATURE (Data,Var,Abs)    
    0xc0,                 //   END_COLLECTION (logical)
    0xc0,                 // END_COLLECTION (application)


	//Top level collection for use in Mouse device mode. 
	//-------------------------------------------------------------------
	//Format for mouse input reports using this report descriptor:
	//byte[0] = Report ID = MOUSE_DATA_REPORT_ID
	//byte[1] = contains bit fields for various input information typically generated by mouse buttons or touch pad tap operations. '1' is the active value (ex: pressed), '0' is the non active value
	//		bit0 = Button 1. 
	//		bit1 = Button 2.  
	//		bit2 = Button 3.
	//		bits3-7 = pad bits, no valid data.
	//byte[2] = Relative X coordinate change since the last report was sent
	//byte[3] = Relative Y coordinate change since the last report was sent
    0x05, 0x01, // Usage Page (Generic Desktop)             
    0x09, 0x02, // Usage (Mouse)                            
    0xA1, 0x01, // Collection (Application)                 
    0x85, MOUSE_DATA_REPORT_ID, //  REPORT_ID (mouse)
    0x09, 0x01, //  Usage (Pointer)                         
    0xA1, 0x00, //  Collection (Physical)                   
    0x05, 0x09, //      Usage Page (Buttons)                
    0x19, 0x01, //      Usage Minimum (01)                  
    0x29, 0x03, //      Usage Maximum (03)                  
    0x15, 0x00, //      Logical Minimum (0)                 
    0x25, 0x01, //      Logical Maximum (1)                 
    0x45, 0x00, //      Physical Maximum (0)
    0x65, 0x00, //      Unit (none)                  
    0x95, 0x03, //      Report Count (3)                    
    0x75, 0x01, //      Report Size (1)                     
    0x81, 0x02, //      Input (Data, Variable, Absolute)    
    0x95, 0x01, //      Report Count (1)                    
    0x75, 0x05, //      Report Size (5)                     
    0x81, 0x01, //      Input (Constant)    ;5 bit padding  
    0x05, 0x01, //      Usage Page (Generic Desktop)        
    0x09, 0x30, //      Usage (X)                           
    0x09, 0x31, //      Usage (Y)                           
    0x15, 0x81, //      Logical Minimum (-127)              
    0x25, 0x7F, //      Logical Maximum (127)               
    0x75, 0x08, //      Report Size (8)                     
    0x95, 0x02, //      Report Count (2)                    
    0x81, 0x06, //      Input (Data, Variable, Relative)    
    0xC0, 		//  End Collection
    0xC0,  	    // End Collection            

	//Top level collection for use in Single-Touch Digitizer device mode.
	//-------------------------------------------------------------------
	//Format for single-touch digitizer input reports using this report descriptor:
	//byte[0] = Report ID = SINGLE_TOUCH_DATA_REPORT_ID
	//byte[1] = contains bit fields for various input information typically generated by an input pen or human finger. '1' is the active value (ex: pressed), '0' is the non active value
	//		bit0 = Tip switch. At the end of a pen input device would normally be a pressure senstive switch.  Asserting this performs an operation analogous to a "left click" on a mouse
	//		bit2 = In range indicator.  
	//		bit3 though bit 7 = Pad bits.  Values not used for anything.
	//byte[2] = Pad byte.  Value not used for anything.
	//byte[3] = X coordinate LSB value of contact point
	//byte[4] = X coordinate MSB value of contact point
	//byte[5] = Y coordinate LSB value of contact point
	//byte[6] = Y coordinate MSB value of contact point
    0x05, 0x0D,             // USAGE_PAGE (Digitizers)          
    0x09, 0x02,             // USAGE (Pen)                      
    0xA1, 0x01,             // COLLECTION (Application)         
    0x85, SINGLE_TOUCH_DATA_REPORT_ID, 	//   REPORT_ID (Pen digitizer/single touch)  //To send a HID input report containing single-touch digitizer data, use the SINGLE_TOUCH_DATA_REPORT_ID report ID
    0x09, 0x20,             //   USAGE (Stylus)                 
    0xA1, 0x00,             //   COLLECTION (Physical)          
    0x09, 0x42, 			//     USAGE (Tip Switch)           //(byte[1] bit 0)
    0x09, 0x32, 			//     USAGE (In Range)             //(byte[1] bit 1)
    0x15, 0x00,             //     LOGICAL_MINIMUM (0)          
    0x25, 0x01,             //     LOGICAL_MAXIMUM (1)          
    0x75, 0x01,             //     REPORT_SIZE (1)              //1-bit		
    0x95, 0x02,             //     REPORT_COUNT (2)             //two
    0x81, 0x02, 			//     INPUT (Data,Var,Abs)         //Makes two, 1-bit IN packet fields (byte[1] bits 0-4)) for (USAGE) tip sw, barrel sw, invert sw, in range sw.  Send '1' here when switch is active.  Send '0' when switch not active.
    0x95, 0x0E, 			//     REPORT_COUNT (14)            //fourteen
    0x81, 0x03, 			//     INPUT (Cnst,Var,Abs)         //Makes fourteen, 1-bit IN packet fields (byte[1] bits 5-7, and byte[2] all bits) with no usage.  These are pad bits that don't contain useful data.
    0x05, 0x01,             //     USAGE_PAGE (Generic Desktop)
    0x26, 0xC0, 0x12,     //       LOGICAL_MAXIMUM (4800)         
    0x75, 0x10,           //       REPORT_SIZE (16)             
    0x95, 0x01,             //     REPORT_COUNT (1)   			//one
    0x55, 0x0E,           //       UNIT_EXPONENT (-2)       //10^(-2)           
    0x65, 0x33,           //       UNIT (Inches, English Linear)  //But exponent -2, so Physical Maximum is in 10’s of mils.                 
    0x09, 0x30,           //       USAGE (X)                    
    0x35, 0x00,           //       PHYSICAL_MINIMUM (0)         
    0x46, 0x40, 0x06,     //       PHYSICAL_MAXIMUM (0x640 = 1600)     //1600 * 10^(-2) = 16 inches X-dimension        
    0x81, 0x02,           //       INPUT (Data,Var,Abs)           //Makes one, 16-bit field for X coordinate info.  Valid values from: 0-4800    
    0x26, 0xB8, 0x0B,     //       LOGICAL_MAXIMUM (3000)             //16:10 aspect ratio (X:Y) 
    0x46, 0xE8, 0x03,     //       PHYSICAL_MAXIMUM (0x3E8 = 1000)    //1000 * 10^(-2) = 10 inches Y-dimension       
    0x09, 0x31,           //       USAGE (Y)       
    0x81, 0x02,             //     INPUT (Data,Var,Abs)		    //Makes one, 16-bit field for Y coordinate info.  Valid values from: 0-3000   
    0xC0,                   //   END_COLLECTION (Physical)                
    0xC0                    // END_COLLECTION (Application)                 
	}
};// end of HID report descriptor 


//Array of configuration descriptors
ROM BYTE *ROM USB_CD_Ptr[]=
{
    (ROM BYTE *ROM)&configDescriptor1
};

//Array of string descriptors
ROM BYTE *ROM USB_SD_Ptr[]=
{
    (ROM BYTE *ROM)&sd000,
    (ROM BYTE *ROM)&sd001,
    (ROM BYTE *ROM)&sd002
};

/** EOF usb_descriptors.c ***************************************************/

#endif
