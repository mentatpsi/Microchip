//
//  HID PnP.h
//  HID PnP Demo
//
//  Created by Trey Lawrence on 10/11/10.
//  Copyright 2010 Microchip Technology. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <IOKit/hid/IOHIDManager.h>

// product and vendor IDs
#define productID 0x003F
#define vendorID 0x04D8

@interface HID_PnP : NSObject {
	float potentiometerVoltage;
	bool toggleLeds;
	bool pushButton;
	bool isConnected;
	
	IOHIDManagerRef hidManager;
	IOHIDDeviceRef myDevice;
}

// Read-write modifiers for class variables
@property(readwrite) float potentiometerVoltage;
@property(readwrite) bool toggleLeds, pushButton, isConnected;

// Objective C Functions
- (id) init;
- (void) StartHID;
- (void) DeviceRemoved;
- (void) PacketReceived;
- (void) NewDeviceDetected;

// Callback C functions
static void MyInputCallback(void *context, IOReturn result, void *sender, IOHIDReportType type, uint32_t reportID, uint8_t *report, CFIndex reportLength);
static void MyRemovalCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
static void MyNewDeviceCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);

@end
