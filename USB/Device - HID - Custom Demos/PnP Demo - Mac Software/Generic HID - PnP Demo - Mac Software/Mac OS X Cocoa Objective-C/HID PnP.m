//
//  HID PnP.m
//  HID PnP Demo
//
//  Created by Trey Lawrence on 10/11/10.
//  Copyright 2010 Microchip Technology. All rights reserved.
//

#import "HID PnP.h"

unsigned char OUTBuffer[64];
unsigned char INBuffer[64];

void *selfRef;

@implementation HID_PnP
@synthesize potentiometerVoltage, toggleLeds, pushButton, isConnected;

- (id) init {
	// Init routine for parent class
	if(!(self = [super init])) {
		NSLog(@"DemoApp init Error");
		return nil;
	}
	
	// Initialize class variables
	isConnected = FALSE;
	pushButton = FALSE;
	toggleLeds = FALSE;
	potentiometerVoltage = 0;
	
	// Save pointer reference to self for C callback functions
	selfRef = self;
	
	// Initialize HID Manager
	hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	
	// Initialize Dictionary with Microchip Product and Vendor IDs and find any devices matching them
	NSMutableDictionary *deviceDictionary = [NSMutableDictionary dictionary];
	[deviceDictionary setObject:[NSNumber numberWithLong:productID]
						 forKey:[NSString stringWithCString:kIOHIDProductIDKey encoding:NSUTF8StringEncoding]];
	[deviceDictionary setObject:[NSNumber numberWithLong:vendorID]
						 forKey:[NSString stringWithCString:kIOHIDVendorIDKey encoding:NSUTF8StringEncoding]];
	IOHIDManagerSetDeviceMatching(hidManager, (CFMutableDictionaryRef)deviceDictionary);	
	
	// Open HID Manager	
	IOHIDManagerOpen(hidManager, 0L);
	
	// See if new device is detected
	[self NewDeviceDetected];
	
	// Return self
	return self;
}

- (void) DeviceRemoved{
	// Remove device reference
	myDevice = nil;
	
	// Set connected, toggle LEDs, and pushbutton flags to false. Set potentiometer reading to 0
	isConnected = FALSE;
	pushButton = FALSE;
	toggleLeds = FALSE;
	potentiometerVoltage = 0;
	
	// Unregsiter device removed callback, register the callback handler when a new device is enumerated to check if it matches VID and PID
	IOHIDManagerRegisterDeviceRemovalCallback(hidManager, NULL, NULL);
	IOHIDManagerRegisterDeviceMatchingCallback(hidManager, MyNewDeviceCallback, NULL);
}

- (void) NewDeviceDetected {
	// Schedule HID Manger with current run loop with default mode and event tracking mode
	IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
	IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), (CFStringRef*)NSEventTrackingRunLoopMode);
	
	// Obtain current enumerated devices that match VID and PID; only take the first instance and throw away the rest
	NSSet *allDevices = (NSSet *)IOHIDManagerCopyDevices(hidManager);
	NSArray *myDevices = [allDevices allObjects];
	myDevice = ([myDevices count]) ? (IOHIDDeviceRef)[myDevices objectAtIndex:0] : nil;
		
	// If a device is attached
	if(myDevice) {
		// Set connected flag to true
		isConnected = TRUE;
		
		// Register the callback functions to handle when an IN data packet is received and when the device is removed
		IOHIDDeviceRegisterInputReportCallback(myDevice, (uint8_t*)&INBuffer, 64, MyInputCallback, NULL);
		IOHIDManagerRegisterDeviceRemovalCallback(hidManager, MyRemovalCallback, NULL);
		
		// Setup output buffer with get ADC value (0x37) as first byte of packet and fill the rest of the packet
		OUTBuffer[0] = 0x37;
		memset((void*)&OUTBuffer[1], 0xFF, 63);
		
		// Send Output buffer to device with Report ID = 0
		IOHIDDeviceSetReport(myDevice, kIOHIDReportTypeOutput, 0, (uint8_t*)&OUTBuffer, 64);
		
		// Unregister the callback function that handles new devices, since this example only supports one custom hid device at a time
		IOHIDManagerRegisterDeviceMatchingCallback(hidManager, NULL, NULL);
	}
	// If device is not attached
	else {
		[self  DeviceRemoved];
	}
}

- (void) PacketReceived {
	if(toggleLeds == TRUE)
	{
		// Clear toggle LEDs flag
		toggleLeds = FALSE;
		
		// Setup output buffer with Toggle LEDs Command (0x80) as first byte of packet
		OUTBuffer[0] = 0x80;
		
		// Fill the rest of the output buffer with 0xFF and send Output buffer to device with Report ID = 0
		memset((void*)&OUTBuffer[1], 0xFF, 63);
		IOHIDDeviceSetReport(myDevice, kIOHIDReportTypeOutput, 0, (uint8_t*)&OUTBuffer, 64);
	}
	if(INBuffer[0] == 0x37) {
		// Take second and third bytes of packet to calculate integer value of potentiometer reading
		potentiometerVoltage = (INBuffer[2] << 8) + INBuffer[1];
		
		// Setup output buffer with get Push Button Status value (0x81) as first byte of packet
		OUTBuffer[0] = 0x81;
	}
	if(INBuffer[0] == 0x81) {
		// Set pushbutton status to pressed if if INBuffer[1] is 0, otherwise not pressed
		pushButton = (INBuffer[1] == 0x00);
			
		// Setup output buffer with get ADC value (0x37) as first byte of packet
		OUTBuffer[0] = 0x37;
	}
		
	// Fill the rest of the output buffer with 0xFF, and send with Report ID = 0
	memset((void*)&OUTBuffer[1], 0xFF, 63);
	IOHIDDeviceSetReport(myDevice, kIOHIDReportTypeOutput, 0, (uint8_t*)&OUTBuffer, 64);
}

static void MyInputCallback(void *context, IOReturn result, void *sender, IOHIDReportType type, uint32_t reportID, uint8_t *report, CFIndex reportLength) {
	[selfRef PacketReceived];
}

static void MyRemovalCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device) {
	[selfRef DeviceRemoved];
}

static void MyNewDeviceCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device) {
	[selfRef NewDeviceDetected];
}

@end
