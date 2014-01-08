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

/**
 * Interface that defines the API for all of the demos.
 * The demos send their information to the GUI through the
 * handler that was setup when they were created.
 */
public interface DemoInterface {
	/**
	 * Closes the connection to the device, kills the thread, 
	 * and cleans up all variables.
	 */
	public void close();
	
	/**
	 * Indicates if the connection to the USB device was
	 * successful.
	 * 
	 * @return boolean Status of the connection.
	 */
	public boolean isConnected();
	
	/** 
	 * Gets the title for the device.
	 * 
	 * @return String The title of the attached device.
	 */
	public String getDeviceTitle();
	
	/**
	 * Request to toggle the LEDs on the attached board
	 */
	public void toggleLEDs();
	
}
