/********************************************************************
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
********************************************************************/

package com.microchip.android.BasicUSBDeviceDemo;

import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.os.Handler;

/**
 * Abstract class that is used to load the correct demo corresponding
 * to the USB device that was attached.
 */
public abstract class Demo implements DemoInterface {
	
	/**
	 * Loads the demo that corresponds to the attached USB device.
	 * 
	 * @param context The context associated with the request.
	 * @param device The USB device that should be connected to.
	 * @param handler The handler to the GUI where the demo should send messages. 
	 */
	static Demo loadDemo(Context context, UsbDevice device, Handler handler){
		//Check the USB device to see if it is a Microchip LibUSB demo device
		if((device.getVendorId() == (int)0x04D8) && (device.getProductId() == (int)0x0204)) {
			return new DemoLibUSB(context, device, handler);
		}
		
		//Check the USB device to see if it is a Microchip MCHPUSB demo device
		if((device.getVendorId() == (int)0x04D8) && (device.getProductId() == (int)0x000C)) {
			return new DemoMCHPUSB(context, device, handler);
		}
		
		//Check the USB device to see if it is a Microchip WinUSB demo device
		if((device.getVendorId() == (int)0x04D8) && (device.getProductId() == (int)0x0053)) {
			return new DemoWinUSB(context, device, handler);
		}
		
		//Check the USB device to see if it is a Microchip Custom HID demo device
		if((device.getVendorId() == (int)0x04D8) && (device.getProductId() == (int)0x003F)) {
			return new DemoCustomHID(context, device, handler);
		}
		
		//Check the USB device to see if it is a MCP2200EV-VCP demo board
		if((device.getVendorId() == (int)0x04D8) && (device.getProductId() == (int)0x00DF)) {
			return new DemoMCP2200(context, device, handler);
		}
	
		//If we don't know what type of demo this is, return null.
		return null;
	}
}
