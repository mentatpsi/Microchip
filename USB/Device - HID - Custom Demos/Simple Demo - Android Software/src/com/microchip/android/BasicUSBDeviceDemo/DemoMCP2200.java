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
import android.util.Log;

/**
 * 
 * DemoCustomHID class is a demo class that interfaces to the HID custom HID 
 * demos located in the Microchip Application Library USB framework.
 *   www.microchip.com/usb
 *
 * This example shows how to connect an Android device with USB host 
 * capability to a device using a custom class driver.
 */
public class DemoMCP2200 extends Demo implements Runnable, DemoInterface {
	private UsbDevice device = null;
	private UsbManager manager = null;
	private Handler handler = null;
	private Integer toggleLEDCount = 0;
	private Boolean closeRequested = new Boolean(false);
	private UsbDeviceConnection connection;
	private UsbInterface intf;
	private boolean connected = false;
	private String toSend = new String();
	Thread thread;
	
	/**
	 * Constructor - creates connection to device and launches the
	 *   thread that runs the actual demo.
	 * @param context Context requesting to run the demo.
	 * @param device The USB device to attach to.
	 * @param handler The Handler where demo Messages should be sent.
	 */
	DemoMCP2200(Context context, UsbDevice device, Handler handler) {
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
		intf = device.getInterface(1);
		
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
			return "MCP2200EV-VCP Demo (VID = 0x" + Integer.toHexString(device.getVendorId()) + 
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
	 * The man thread for the demo
	 */
	public void run() {	
		/* Get the OUT endpoint.  It is the second endpoint in the interface */
		UsbEndpoint endpointOUT = null;
		
		/* Get the IN endpoint.  It is the first endpoint in the interface */
		UsbEndpoint endpointIN = null;
		
		int i = intf.getEndpointCount();
		int j;
		
		for(j=0; j<i; j++)
		{
			UsbEndpoint currentEndpoint = intf.getEndpoint(j);
			
			if(currentEndpoint.getEndpointNumber() == 3)
			{
				if(currentEndpoint.getDirection() == android.hardware.usb.UsbConstants.USB_DIR_OUT)
				{
					endpointOUT = currentEndpoint;
				}
				else
				{
					endpointIN = currentEndpoint;
				}
			}
		}
		
		if((endpointOUT == null) || (endpointIN == null)){
			Log.e("MCHP","Endpoint null in MCP2200 demo.");
			destroy();
			return;
		}
		
		/* Create the packets that we are going to send to the attached USB
		 * device.  
		 */

		byte[] RxData = new byte[64];
		int result = 0;
		byte[] TxData = null;
		
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
			
			/* Send the request to get the push button status */
			synchronized(toSend)
			{
				if((toSend.length() > 0) && (TxData == null))
				{
					TxData = toSend.getBytes();
					toSend = new String();
				}
			}
			
			if(TxData != null)
			{
				result = connection.bulkTransfer(endpointOUT, TxData, TxData.length, 100);
			
				if(result != 0)
				{
					TxData = null;
				}
			}
				
			/* Read the push button status */
			result = connection.bulkTransfer(endpointIN, RxData, RxData.length, 100);
						
			/* If there was data successfully read,... */
			if(result > 0) {
				handler.obtainMessage(0, new MessageText(new String(RxData).substring(0, result))).sendToTarget();	
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
		closeRequested = false;
		connection = null;
		intf = null;
	}

	public void sendString(String s) {	
		synchronized(toSend)
		{
			toSend = toSend + s;
		}		
	}
	
	public void setBaudRate(String s) {	
		byte[] line_coding = new byte[0];
		
		Integer baudRate = Integer.parseInt( s );
			
		line_coding = new byte[]{(byte)(baudRate.intValue()&0xFF), (byte)((baudRate.intValue()>>8)&0xFF), (byte)((baudRate.intValue()>>16)&0xFF), (byte)((baudRate.intValue()>>24)&0xFF), (byte)0x00, (byte)0x00, (byte)0x08};
		connection.controlTransfer(0x21, 0x20, 0x0000, 0x0000, line_coding, line_coding.length, 20);
	}
}
