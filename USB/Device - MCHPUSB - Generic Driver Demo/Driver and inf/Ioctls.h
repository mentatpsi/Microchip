// IOCTLS.H -- IOCTL code definitions for mchpusb driver
// Copyright (C) 2003 by Microchip Technology, Inc.
// All rights reserved

#pragma once

#ifndef CTL_CODE
	#pragma message("CTL_CODE undefined. Include winioctl.h or wdm.h")
#endif

///////////////////////////////////////////////////////////////////////////////
// Interface GUID for Microchip PIC18F4550 family driver

// {5354FA28-6D14-4E35-A1F5-75BB54E6030F}
DEFINE_GUID(GUID_DEVINTERFACE_MCHPUSB, 0x5354fa28L, 0x6d14, 0x4e35, 0xa1, 0xf5, 0x75, 0xbb, 0x54, 0xe6, 0x03, 0x0f);

// Suffix strings for forming device names. Take the "device detail" path string
// returned by a SetupDiXxx enumeration and add on one of these, followed by
// a zero-based DECIMAL integer denoting the endpoint address. Use the resulting 
// name string as the first argument in a call to CreateFile. Application 
// code should use constructs like this to specify names in either Unicode or 
// ANSI format: _T(MCHPUSB_PIPE_NAME). Driver code can use TEXT(MCHPUSB_PIPE_NAME), etc.
// For interrupt endpoints, using the "ASYNC" suffix means that you must use
// IOCTL_MCHPUPSB_WAIT_INTERRUPT to read input from the pipe. Using the EP
// suffix means you must use ReadFile or WriteFile, as appropriate to the endpoint's
// directionality.

#define MCHPUSB_PIPE_NAME			"\\MCHP_EP"
#define MCHPUSB_ASYNC_PIPE_SUFFIX	"_ASYNC"

///////////////////////////////////////////////////////////////////////////////
//	IOCTL definitions.
//
//	Note that "input" and "output" refer to the parameter designations in calls to
//	DeviceIoControl, which are the opposite of common sense from the perspective of
//	an application making the calls.
//
//	IOCTL_MCHPUSB_GET_VERSION -- get driver revision level
//	Input:
//		None
//	Output:
//		32-bit revision level MMMMmmmm
//
//	IOCTL_MCHPUSB_GET_DESCRIPTOR -- get descriptor
//	Input:
//		GET_DESCRIPTOR_PARAMETER structure (q.v.)
//	Output:
//		As much of specified descriptor as will fit in the buffer
//
//	IOCTL_MCHPUSB_GET_CONFIGURATION_INFO -- get simplified information about configuration
//	Input:
//		None
//	Output:
//		DEVICE_INFO structure (q.v.)
//
//	IOCTL_MCHPUSB_SET_CONFIGURATION -- configure device
//	Input:
//		SET_CONFIGURATION_PARAMETER structure (q.v.)
//	Output:
//		None.
//	Note that this IOCTL will fail with STATUS_ACCESS_DENIED if any pipe handles are presently
//	open for endpoints that are not in the new configuration.
//
//	IOCTL_MCHPUSB_CONTROL_OUT -- issue control pipe command with output data
//	Input:
//		CONTROL_PARAMETER structure (q.v.)
//	Output:
//		Data to be sent to device (at most 4096 bytes)
//	Note: you must use this IOCTL with care, so as not to put the device into a state other
//	than expected by the driver. E.g., sending a SET_ADDRESS request would be a really bad idea.
//
//	IOCTL_MCHPUSB_CONTROL_IN -- issue control pipe command with input data
//	Input:
//		CONTROL_PARAMETER structure (q.v.)
//	Output:
//		Buffer for data retrieved from device (at most 4096 bytes)
//
//	IOCTL_MCHPUSB_WAIT_INTERRUPT -- await input from interrupt endpoint
//	Input:
//		None
//	Output:
//		Interrupt data provided by the matching input transaction on the endpoint

#define IOCTL_MCHPUSB_GET_VERSION				CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MCHPUSB_GET_DESCRIPTOR			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_MCHPUSB_GET_CONFIGURATION_INFO	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_MCHPUSB_SET_CONFIGURATION			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_MCHPUSB_CONTROL_OUT				CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)
#define IOCTL_MCHPUSB_CONTROL_IN				CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_OUT_DIRECT, FILE_READ_ACCESS)
#define IOCTL_MCHPUSB_WAIT_INTERRUPT			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_BUFFERED, FILE_READ_ACCESS)

///////////////////////////////////////////////////////////////////////////////
// Parameter structure for IOCTL_MCHPUSB_GET_DESCRIPTOR:

typedef struct _GET_DESCRIPTOR_PARAMETER {
	UCHAR bType;				// type of descriptor
	UCHAR bIndex;				// index of descriptor
	USHORT wLangid;				// language id for string descriptor
	} GET_DESCRIPTOR_PARAMETER, *PGET_DESCRIPTOR_PARAMETER;

#define USB_DEVICE_DESCRIPTOR_TYPE                0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE         0x02
#define USB_STRING_DESCRIPTOR_TYPE                0x03

///////////////////////////////////////////////////////////////////////////////
// Output pseudo-structures for IOCTL_MCHPUSB_GET_CONFIGURATION_INFO. Note that as much
// of the structure is filled in as there is room in the output buffer. To retrieve
// the entire structure, make one call with a 4-byte buffer to get the length, then
// make another call after allocating sufficient memory.

typedef struct _ENDPOINT_INFO {
    UCHAR bEndpointAddress;
    UCHAR bmAttributes;
    USHORT wMaxPacketSize;
    UCHAR bInterval;
	} ENDPOINT_INFO, *PENDPOINT_INFO;

#define NEXT_ENDPOINT_INFO(pei) ((pei) + 1)

#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03

typedef struct _INTERFACE_INFO {
    UCHAR bInterfaceNumber;
    UCHAR bAlternateSetting;
    UCHAR bNumEndpoints;
    UCHAR bInterfaceClass;
    UCHAR bInterfaceSubClass;
    UCHAR bInterfaceProtocol;
	_ENDPOINT_INFO Endpoints[1];	// placeholder for array of endpoint info structures
	} INTERFACE_INFO, *PINTERFACE_INFO;

#define NEXT_INTERFACE_INFO(pii) ((PINTERFACE_INFO) ((pii)->Endpoints + (pii)->bNumEndpoints))

typedef struct _CONFIGURATION_INFO {
	ULONG ulSize;					// size of config descriptor and all interface descriptors
	UCHAR bConfigurationValue;
	UCHAR bNumInterfaces;			// number of interfaces in this configuration, including different alternate settings
	_INTERFACE_INFO Interfaces[1];	// placeholder for array of interface info structures
	} CONFIGURATION_INFO, *PCONFIGURATION_INFO;

#define NEXT_CONFIGURATION_INFO(pci) ((PCONFIGURATION_INFO) ((PUCHAR) (pci) + (pci)->ulSize))

typedef struct _DEVICE_INFO {
	ULONG ulSize;							// length of the entire structure
    UCHAR bDeviceClass;
    UCHAR bDeviceSubClass;
    UCHAR bDeviceProtocol;
    UCHAR bNumConfigurations;
    USHORT idVendor;
    USHORT idProduct;
    USHORT bcdDevice;
	_CONFIGURATION_INFO Configurations[1];	// placeholder for array of configuration info structures
	} DEVICE_INFO, *PDEVICE_INFO;

///////////////////////////////////////////////////////////////////////////////
// Parameter pseudo-structure for IOCTL_MCHPUSB_SET_CONFIGURATION:

typedef struct _SET_CONFIGURATION_PARAMETER {
	UCHAR bConfigurationValue;	// desired configuration number
	UCHAR bNumInterfaces;		// number of interfaces to enable
	struct _Interfaces {
		UCHAR bInterfaceNumber;	// interface number
		UCHAR bAlternateSetting;// alternate setting number
		} Interfaces[1];		// placeholder for array of interface specifications
	} SET_CONFIGURATION_PARAMETER, *PSET_CONFIGURATION_PARAMETER;

#define SET_CONFIGURATION_PARAMETER_SIZE(numint) (FIELD_OFFSET(SET_CONFIGURATION_PARAMETER, Interfaces) + numint * sizeof(SET_CONFIGURATION_PARAMETER::_Interfaces))

///////////////////////////////////////////////////////////////////////////////
// Parameter structure for IOCTL_MCHPUSB_CONTROL_xxx requests

typedef struct _CONTROL_PARAMETER {
	UCHAR bmRequestType;
	UCHAR bRequest;
	USHORT wValue;
	USHORT wIndex;
	} CONTROL_PARAMETER, *PCONTROL_PARAMETER;