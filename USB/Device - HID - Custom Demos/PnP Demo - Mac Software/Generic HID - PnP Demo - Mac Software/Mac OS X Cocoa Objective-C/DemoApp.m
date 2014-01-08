//
//  DemoApp.m
//  HID PnP Demo
//
//  Created by Trey Lawrence on 10/11/10.
//  Copyright 2010 Microchip Technology. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "DemoApp.h"


@implementation DemoApp

- (id)init {
	// Init routine for parent class
	if(!(self = [super init])) {
		NSLog(@"DemoApp init Error");
		return nil;
	}
	
	// Allocate memory and initialize HID_PnP object to start HID USB communication
	plugNPlay = [[HID_PnP alloc]init];
	
	// Get current run loop, then create timer with 30ms (~30hz) interval
	NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
	NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:.015
													  target:self
													selector:@selector(UpdateAppWindow)
													userInfo:NULL
													 repeats:YES];
	
	// Add timer to run loop with common mode and event tracking mode
	[runLoop addTimer:timer forMode:NSRunLoopCommonModes];
	[runLoop addTimer:timer forMode:NSEventTrackingRunLoopMode];
	
	// Return self
	return self;
}

- (IBAction)ToggleledsButtonPressed: (id) sender {
	// When Toggle LED(s) pushbutton is pressed in application window, set toggle LEDs flag
	[plugNPlay setToggleLeds: TRUE];
}

- (void)UpdateAppWindow {
	// If device is plugged in
	if([plugNPlay isConnected] == TRUE) {
		// Enable pushbutton, darken text in labels to black
		[toggleButton setEnabled:TRUE];
		[pushbuttonState setAlphaValue:1];
		[progressBar setAlphaValue:1];
		[progressBarLabel setAlphaValue:1];
		
		// Display that device is attached
		[pnpStatus setStringValue:@"Device Found: AttachedState = TRUE"];
		
		// If pushbutton on device is pressed, display Pressed
		if([plugNPlay pushButton] == true) {
			[pushbuttonState setStringValue:@"Pushbutton State: Pressed"];
		}
		// Otherwise display Not Pressed
		else if([plugNPlay pushButton] == false) {
			[pushbuttonState setStringValue:@"Pushbutton State: Not Pressed"];
		}
	}
	// If device is not plugged in
	else {
		// Disable pushbutton, grey text in labels to light grey
		[toggleButton setEnabled:FALSE];
		[pushbuttonState setAlphaValue:.5];
		[progressBar setAlphaValue:.5];
		[progressBarLabel setAlphaValue:.5];
		
		// Display that device is not attached and pushbutton is in Unknown state
		[pnpStatus setStringValue:@"Device Not Detected: Verify Connection/Correct Firmware"];
		[pushbuttonState setStringValue:@"Pushbutton State: Unknown"];
	}
	
	// Update progress bar to display Potentiometer value
	[progressBar setDoubleValue:[plugNPlay potentiometerVoltage]];
}

@end
