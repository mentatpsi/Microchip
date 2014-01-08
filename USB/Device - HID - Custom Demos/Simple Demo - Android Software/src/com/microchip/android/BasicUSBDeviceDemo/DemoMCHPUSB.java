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

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.Handler;

/**
 * 
 * DemoMCHPUSB class is a demo class that interfaces to the MCHPUSB demos
 * located in the Microchip Application Library USB framework.
 *   www.microchip.com/usb
 *
 * This example shows how to connect an Android device with USB host 
 * capability to a device using a custom class driver.
 */
public class DemoMCHPUSB extends Demo implements Runnable, DemoInterface{
	private UsbDevice device = null;
	private UsbManager manager = null;
	private Handler handler = null;
	private Integer toggleLEDCount = 0;
	private Boolean closeRequested = new Boolean(false);
	private UsbDeviceConnection connection;
	private UsbInterface intf;
	private boolean connected = false;
	Thread thread;
	
	/**
	 * Constructor - creates connection to device and launches the
	 *   thread that runs the actual demo.
	 * @param context Context requesting to run the demo.
	 * @param device The USB device to attach to.
	 * @param handler The Handler where demo Messages should be sent.
	 */
	DemoMCHPUSB(Context context, UsbDevice device, Handler handler) {
		/* Save the device and handler information for later use. */
		this.device = device;
		this.handler = handler;
		
		/* Get the USB manager from the requesting context */
		this.manager = (UsbManager) context.getSystemService(Context.USB_SERVICE);
		
		/*
		 * Get the required interface from the USB device.  In this case
		 * we are hard coding the interface number to 0.  In a dynamic example
		 * the code could scan through the interfaces to find the right
		 * interface.  In this case since we know the exact device we are connecting
		 * to, we can hard code it. 
		 */ 
		intf = device.getInterface(0);
		
		/* Open a connection to the USB device */
		connection = manager.openDevice(device);
		
		if(connection == null) {
			return;
		}
		
		/* Claim the required interface to gain access to it */
		if(connection.claimInterface(intf, true) == true) {
			thread = new Thread(this);
			thread.start();
			connected = true;
		} else {
			/* if the interface claim failed, we should close the
			 * connection and exit.
			 */
			connection.close();
		}
	}
	
	/**
	 * @return boolean Indicates if the connection to the USB device
	 * was successfully made.
	 */
	public boolean isConnected() {
		return connected;
	}
	
	/**
	 * @return String Returns the title/description of the device
	 */
	public String getDeviceTitle() {
		if(device != null) {
			return "Microchip Custom Class Device Demo (VID = 0x" + Integer.toHexString(device.getVendorId()) + 
				" PID = 0x" + Integer.toHexString(device.getProductId()) + ")";
		}
		
		return null;
	}
	
	/**
	 * Requests that the attached USB device toggle its LEDs.
	 */
	public void toggleLEDs() {
		/* We need to synchronize to the toggleLEDCount object to make sure that the
		 * running thread and this request aren't changing the variable at the same
		 * time.  
		 */
		synchronized(toggleLEDCount) {
			if(toggleLEDCount < Integer.MAX_VALUE) {
				toggleLEDCount++;
			}
		}
	}
	
	/**
	 * Request that the demo close itself.  
	 */
	public void close(){
		connected = false;
		
		/* We should synchronize to the closeRequested object here to insure that the 
		 * running thread isn't in the middle of checking this object when we change it.
		 */
		synchronized(closeRequested) {
			closeRequested = true;
		}
	}

	/**
	 * The main thread for the demo
	 */
	public void run() {	
		/* Get the OUT endpoint.  It is the first endpoint in the interface */
		UsbEndpoint endpointOUT = intf.getEndpoint(0);
		
		/* Get the IN endpoint.  It is the second endpoint in the interface */
		UsbEndpoint endpointIN = intf.getEndpoint(1);
		
		/* Create the packets that we are going to send to the attached USB
		 * device.  
		 */
		/* The GetPotentiometer command is 0x37 */
		byte[] getPotentiometerRequest = new byte[]{(byte)0x37};
		
		/* The SetLED command is 0x32 and has two parameters:
		 *  1) LED number
		 *  2) State (1 = on, 0 = off)
		 */
		byte[] toggleLEDsPacket = new byte[]{(byte)0x32, 0 , 0};
		
		/* Create a buffer to store the GetPotentiometer command results */
		byte[] getPotentiometerResults = new byte[64];
		
		int potentiometerLastResults = Integer.MAX_VALUE;
		int result = 0;
		
		boolean needToToggleLEDs = false;
		
		while(true) {
			/* If the connection was closed, destroy the connections and variables
			 * and exit this thread.
			 */
			if(wasCloseRequested() == true) {
				destroy();
				return;
			}
			
			/* Sleep the thread for a while */
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
			/* Check to see if we need to toggle the LEDs.  We are synchronizing on
			 * The toggleLEDCount object here to insure that the user isn't changing
			 * the value as we are checking it.
			 */
			synchronized(toggleLEDCount){
				if(toggleLEDCount > 0) {
					needToToggleLEDs = true;
					toggleLEDCount--;
				}
			}
			
			if(needToToggleLEDs == true) {
				/* If we do need to toggle the LEDs, then try to send the SetLED command for the LEDs.
				 */
				do{
					toggleLEDsPacket[1] = 3;
					result = connection.bulkTransfer(endpointOUT, toggleLEDsPacket, toggleLEDsPacket.length, 1000);
				} while((result < 0) && (wasCloseRequested() == false));

				do{
					toggleLEDsPacket[1] = 4;
					result = connection.bulkTransfer(endpointOUT, toggleLEDsPacket, toggleLEDsPacket.length, 1000);
				} while((result < 0) && (wasCloseRequested() == false));

				/* Toggle the on/off bit so that the next time we send the command, it will
				 * toggle the light.
				 */
				toggleLEDsPacket[2] ^= 1;
				
				needToToggleLEDs = false;
			}
		
			/* Send the request to read the Potentiometer */
			do{
				result = connection.bulkTransfer(endpointOUT, getPotentiometerRequest, getPotentiometerRequest.length, 1000);
			} while((result < 0) && (wasCloseRequested() == false));

			/* Read the results of that request */ 
			do{
				result = connection.bulkTransfer(endpointIN, getPotentiometerResults, getPotentiometerResults.length, 1000);
			} while((result < 0) && (wasCloseRequested() == false));
			
			/* Convert the resulting data to an int */
			byte[] potentiometerBuffer = new byte[]{0,0,0,0};
			potentiometerBuffer[0] = getPotentiometerResults[1];
			potentiometerBuffer[1] = getPotentiometerResults[2];
			
			ByteBuffer buf = ByteBuffer.wrap(getPotentiometerResults);
			buf.order(ByteOrder.LITTLE_ENDIAN);
			int potentiometerResults = buf.getInt();
			
			/* If the new results are different from the previous results, then send
			 * a message to the specified handler containing the new data.
			 */
			if(potentiometerResults != potentiometerLastResults) {
				handler.obtainMessage(0,new MessagePotentiometer((potentiometerResults * 100)/0x3FF)).sendToTarget();
				potentiometerLastResults = potentiometerResults;
			}
		}
	}
	
	/***********************************************************************
	 * Private methods
	 ***********************************************************************/
	
	/**
	 * @return boolean Indicates if someone has requested to close the demo
	 */
	private boolean wasCloseRequested()
	{
		synchronized(closeRequested){
			return closeRequested;
		}
	}
	
	/**
	 * Closes connections, releases resources, cleans up variables
	 */
	private void destroy(){
		/* Release the interface that was previously claimed and close
		 * the connection.
		 */
		connection.releaseInterface(intf);
		connection.close();
		
		/* Clear up all of the locals */
		device = null;
		manager = null;
		handler = null;
		toggleLEDCount = 0;
		closeRequested = false;
		connection = null;
		intf = null;
	}
}
