//
//  DemoApp.h
//  HID PnP Demo
//
//  Created by Trey Lawrence on 10/11/10.
//  Copyright 2010 Microchip Technology. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "HID PnP.h"


@interface DemoApp : NSObject {
	IBOutlet NSProgressIndicator *progressBar;
	IBOutlet NSTextField *progressBarLabel;
	IBOutlet NSTextField *pushbuttonState;
	IBOutlet NSTextField *pnpStatus;
	IBOutlet NSButton *toggleButton;
	
	HID_PnP *plugNPlay;
}

// Objective C functions
- (id) init;
- (IBAction) ToggleledsButtonPressed: (id) sender;
- (void) UpdateAppWindow;

@end
