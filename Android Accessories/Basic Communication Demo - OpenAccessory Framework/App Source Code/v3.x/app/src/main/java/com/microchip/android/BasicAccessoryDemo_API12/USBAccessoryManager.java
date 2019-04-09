/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Companys customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN AS IS CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ********************************************************************/

package com.microchip.android.BasicAccessoryDemo_API12;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.ParcelFileDescriptor;
import android.util.Log;

import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbManager;

/**
 * A class created to assist in making accessing a USB accessory easier for
 * those that are less familiar with programming in Java, working with
 * threads/handlers/synchronization, and those that are not familiar with the
 * Open Accessory framework interface
 * 
 * @author Microchip Technology Inc.
 * 
 */
public class USBAccessoryManager {

	private String actionString = null;
	private Handler handler;
	private int what;

	private boolean enabled = false;
	private boolean permissionRequested = false;
	private boolean open = false;

	private FileOutputStream outputStream = null;
	private ParcelFileDescriptor parcelFileDescriptor = null;
	private ReadThread readThread = null;

	private ArrayList<byte[]> readData = new ArrayList<byte[]>();
	
	private String TAG = "MICROCHIP";

	/***********************************************************************/
	/** Public API **/
	/***********************************************************************/

	/**
	 * Creates new USB Accessory Manager
	 * 
	 * @param handler
	 *            The handler where to send USB accessory event messages
	 * @param what
	 *            The "what" value to use for USB accessory event messages
	 */
	public USBAccessoryManager(Handler handler, int what) {
		this.handler = handler;
		this.what = what;
	}

	/**
	 * Enumeration of possible return values for the enable function
	 */
	public enum RETURN_CODES {
		DEVICE_MANAGER_IS_NULL, ACCESSORIES_LIST_IS_EMPTY, FILE_DESCRIPTOR_WOULD_NOT_OPEN, PERMISSION_PENDING, SUCCESS
	}

	/**
	 * Enables the
	 * 
	 * @param context
	 *            The context that the USB manager should register to
	 * @return RETURN_CODES - the status of the enable request
	 */
	public RETURN_CODES enable(Context context, Intent intent) {
		// Grab the packageName to use for an attach Intent
		actionString = context.getPackageName() + ".action.USB_PERMISSION";

		PendingIntent permissionIntent = PendingIntent.getBroadcast(context, 0,
				new Intent(actionString), 0);

		// If the USB manager isn't already enabled
		if (enabled == false) {
			// Create a new filter with the package name (for the accessory
			// attach)

			try {
				Thread.sleep(500);
			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
			IntentFilter filter = new IntentFilter(actionString);
			// Also add a few other actions to the intent...
			filter.addAction(UsbManager.ACTION_USB_ACCESSORY_ATTACHED);
			filter.addAction(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
			// and register the intent with the specified context
			context.registerReceiver(receiver, filter);

			UsbManager deviceManager = null;
			UsbAccessory[] accessories = null;
			UsbAccessory accessory = null;

			// Get a UsbManager object from the specified intent (only works for
			// v3.1+ devices)
			deviceManager = (UsbManager) context
					.getSystemService(Context.USB_SERVICE);

			// If we were unable to get a UsbManager, return an error
			if (deviceManager == null) {
				return RETURN_CODES.DEVICE_MANAGER_IS_NULL;
			}

			// Get a list of all of the accessories from the UsbManager
			accessories = deviceManager.getAccessoryList();

			// If the list of accessories is empty, then exit
			if (accessories == null) {
				return RETURN_CODES.ACCESSORIES_LIST_IS_EMPTY;
			}

			// Get the first accessory in the list (currently the Android OS
			// only
			// supports one accessory, so this is it)
			accessory = accessories[0];

			// If the accessory isn't null, then let's try to attach to it.
			if (accessory != null) {
				// If we have permission to access the accessory,
				if (deviceManager.hasPermission(accessory)) {
					// Try to open a ParcelFileDescriptor by opening the
					// accessory
					parcelFileDescriptor = deviceManager
							.openAccessory(accessory);

					if (parcelFileDescriptor != null) {
						// Create a new read thread to handle reading data from
						// the accessory
						readThread = new ReadThread(parcelFileDescriptor);
						readThread.start();

						deviceManager.requestPermission(accessory,
								permissionIntent);
						
						if(parcelFileDescriptor == null) {
							Log.d(TAG, "USBAccessoryManager:enable() parcelFileDescriptor == null");
							return RETURN_CODES.FILE_DESCRIPTOR_WOULD_NOT_OPEN;
						}
						
						// Open the output file stream for writing data out to
						// the accessory
						outputStream = new FileOutputStream(
								parcelFileDescriptor.getFileDescriptor());
						
						if(outputStream == null) {
							Log.d(TAG, "USBAccessoryManager:enable() outputStream == null");
							
							try {
								parcelFileDescriptor.close();
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
							return RETURN_CODES.FILE_DESCRIPTOR_WOULD_NOT_OPEN;
						}

						Log.d(TAG,
								"USBAccessoryManager:enable() outputStream open");

						// If the ParcelFileDescriptor was successfully opened,
						// mark the accessory as enabled and open
						enabled = true;
						open = true;

						handler.obtainMessage(
								what,
								new USBAccessoryManagerMessage(
										USBAccessoryManagerMessage.MessageType.READY,
										accessory)).sendToTarget();

						Log.d(TAG,
								"USBAccessoryManager:enable() device ready");

						return RETURN_CODES.SUCCESS;
					} else {
						/*
						 * If we weren't able to open the ParcelFileDescriptor,
						 * then we will not be able to talk to the device. Due
						 * to a bug in the Android v2.3.4 OS this situation may
						 * occur if a user presses the "home" or "back" buttons
						 * while an accessory is still attached. In this case
						 * the attempt to close the ReadThread will fail if a
						 * read() is in progress on the FileInputStream. This
						 * results in the ParcelFileDescriptor not being freed
						 * up for later access. A future attempt to connect to
						 * the accessory (via reopening the app) will end up
						 * having the openAccessory() request return null,
						 * ending up in this section of code.
						 */
						return RETURN_CODES.FILE_DESCRIPTOR_WOULD_NOT_OPEN;
					}
				} else {
					/*
					 * If we don't currently have permission to access the
					 * accessory, then we need to request it. If we haven't
					 * requested it already...
					 */
					if (permissionRequested == false) {
						// Then go ahead and request it...
						deviceManager.requestPermission(accessory,
								permissionIntent);

						permissionRequested = true;
						return RETURN_CODES.PERMISSION_PENDING;
					}
				}
			}

			return RETURN_CODES.ACCESSORIES_LIST_IS_EMPTY;
		}

		return RETURN_CODES.SUCCESS;
	}

	/**
	 * Disables the USB manager and releases all resources
	 * 
	 * @param context
	 *            The context that the manager was enabled with
	 */
	public void disable(Context context) {
		// Free up all of the required resources
		closeAccessory();

		// Unregister the broadcast receiver
		try {
			context.unregisterReceiver(receiver);
		} catch (Exception e) {
		}
	}

	/**
	 * Describes if an accessory is attached or not
	 * 
	 * @return boolean - true if one is attached, false otherwise
	 */
	public boolean isConnected() {
		return open;
	}

	/** I/O API *****************************************************/

	/**
	 * discards the specified number of bytes from the internal read buffer
	 * 
	 * @param num
	 *            the number of bytes to discard
	 */
	void ignore(int num) {
		int amountRead = 0;

		// If the accessory is not connected, then we can't really do anything
		if (isConnected() == false) {
			// throw new USBAccessoryManagerException(
			// USB_ACCESSORY_NOT_CONNECTED );
			return;
		}

		// Must request to ignore 1 or more bytes, otherwise do nothing
		if (num <= 0) {
			return;
		}

		// synchronizing to the readData object so that the ReadThread and this
		// don't try to access the readData object at the same time. This will
		// block whoever tries to access it second until it is available.
		synchronized (readData) {
			// Keep reading through the data objects until one of the many
			// internal
			// checks decides to exit. This each entry in this loop either
			// returns
			// or removed objects from the list (which ultimately causes a
			// return),
			// this loop should always exit.
			while (true) {
				// If there are no more data objects in the list, exit
				if (readData.size() == 0) {
					return;
				}

				// If we have ignored the requested amount of data from the
				// buffer, then exit
				if (amountRead == num) {
					return;
				}

				if ((num - amountRead) >= readData.get(0).length) {
					// We need to ignore equal to or greater than the size of
					// data in this entry

					// then ignore the whole size of the entry
					amountRead += readData.get(0).length;

					// and remove it from the list.
					readData.remove(0);
				} else {
					// We need to ignore only a portion of the data in this
					// entry

					// only remove a portion of the entry, leaving part of the
					// data
					int amountRemoved = num - amountRead;
					byte[] newData = new byte[readData.get(0).length
							- amountRemoved];

					// Copy the remaining data in to a new buffer
					for (int i = 0; i < newData.length; i++) {
						newData[i] = readData.get(0)[i + amountRemoved];
					}

					// remove the old entry
					readData.remove(0);

					// add a new entry to the front of the buffer with the
					// remaining data
					readData.add(0, newData);

					// since we have now read all of the data that we need, exit
					return;
				}
			}
		}
	}

	/**
	 * fills the array with data from the read buffer without discarding it.
	 * 
	 * @param array
	 *            the buffer to fill
	 * @return the number of bytes copied from the buffer
	 */
	int peek(byte[] array) {
		int amountRead = 0;
		int currentNode = 0;

		// If an accessory is not connected, this request is invalid
		if (isConnected() == false) {
			// throw new USBAccessoryManagerException(
			// USB_ACCESSORY_NOT_CONNECTED );
			return 0;
		}

		// If there is no data available, then return 0 bytes read
		if (array.length == 0) {
			return 0;
		}

		/*
		 * Synchronize to the readData object so that the ReadThread doesn't
		 * attempt to add more data to the readData list while we are accessing
		 * it.
		 */
		synchronized (readData) {
			/* While we still have data to read */
			while (true) {
				/*
				 * if we have run out of list objects to peek from... (no data
				 * left to peek)
				 */
				if (currentNode >= readData.size()) {
					/* then return the amount that we have read */
					return amountRead;
				}

				/*
				 * if the amount that we have read exactly matches the amount
				 * that we need
				 */
				if (amountRead == array.length) {
					return amountRead;
				}

				if ((array.length - amountRead) >= readData.get(currentNode).length) {
					// If the amount we need to read is larger than or equal to
					// the data buffer for this node
					for (int i = 0; i < readData.get(currentNode).length; i++) {
						array[amountRead++] = readData.get(currentNode)[i];
					}
					currentNode++;
				} else {
					// If the amount we need to read is less than the data
					// buffer for this node
					for (int i = 0; i < (array.length - amountRead); i++) {
						array[amountRead++] = readData.get(currentNode)[i];
					}

					return amountRead;
				}
			}
		}
	}

	/**
	 * Indicates the number of bytes that are currently in the read buffer.
	 * There will be at least this many bytes to read from the buffer (as long
	 * as the accessory has not detach or been closed since the call to this
	 * function.
	 * 
	 * @return the number of bytes available in the read queue
	 */
	int available() {
		int amount = 0;

		// If the accessory is not connected, then this request is invalid
		if (isConnected() == false) {
			// throw new USBAccessoryManagerException(
			// USB_ACCESSORY_NOT_CONNECTED );
			return 0;
		}

		/*
		 * Synchronize to the readData object so that the ReadThread doesn't try
		 * to add data the list while we are accessing it.
		 */
		synchronized (readData) {
			for (byte[] b : readData) {
				amount += b.length;
			}
		}

		return amount;
	}

	/**
	 * Reads bytes from the read buffer, removing them from the buffer once read
	 * 
	 * @param array
	 *            where to copy the data
	 * @return the number of bytes copied (maximum will be the length of the
	 *         array param
	 */
	int read(byte[] array) {
		int amountRead = 0;

		/* If an accessory is not connected, this request is not valid */
		if (isConnected() == false) {
			// throw new USBAccessoryManagerException(
			// USB_ACCESSORY_NOT_CONNECTED );
			return 0;
		}

		/* if there is no data to read, return 0 instead of blocking */
		if (array.length == 0) {
			return 0;
		}

		/*
		 * Synchronize to the readData object so that the ReadThread doesn't try
		 * to add data to the list while we are accessing it.
		 */
		synchronized (readData) {
			/* while we still have data to read */
			while (true) {
				/*
				 * if there is no data left in the list, exit with the amount
				 * read already
				 */
				if (readData.size() == 0) {
					return amountRead;
				}

				/*
				 * if we have read all of the data that we can store in the
				 * buffer provided, then exit.
				 */
				if (amountRead == array.length) {
					return amountRead;
				}

				if ((array.length - amountRead) >= readData.get(0).length) {
					/*
					 * If the amount we need to read is larger than or equal to
					 * the data buffer for this node, then copy what is here and
					 * remove it from the list
					 */
					for (int i = 0; i < readData.get(0).length; i++) {
						array[amountRead++] = readData.get(0)[i];
					}
					readData.remove(0);
				} else {
					// If the amount we need to read is less than the data
					// buffer for this node
					int amountRemoved = 0;

					/* then copy what we need */
					for (int i = 0; i < (array.length - amountRead); i++) {
						array[amountRead++] = readData.get(0)[i];
						amountRemoved++;
					}

					/* create a new buffer the size of the remaining data */
					byte[] newData = new byte[readData.get(0).length
							- amountRemoved];

					/* copy the remaining data to that buffer */
					for (int i = 0; i < newData.length; i++) {
						newData[i] = readData.get(0)[i + amountRemoved];
					}

					/*
					 * remove the old object and add a new object with the
					 * remaining data
					 */
					readData.remove(0);
					readData.add(0, newData);

					return amountRead;
				}
			}
		}
	}

	/**
	 * Writes data to the accessory
	 * 
	 * @param data
	 *            the data to write
	 * @throws InterruptedException
	 */
	public void write(byte[] data) {
		int tries = 2;

		if (isConnected() == true) {
			if (outputStream != null) {
				/*
				 * we try here a few times because on first attachment the
				 * Android pipe doesn't appear to be completely open yet and
				 * calling write causes an exception in the first ~1 second
				 * after the devices attachment. Data sent in that first second
				 * may not get sent without retrying.
				 */
				while (tries-- > 0) {
					try {
						outputStream.write(data);
						return;
					} catch (IOException e) {
						Log.d(TAG,
								"USBAccessoryManager:write():IOException: "
										+ e.toString());
						try {
							Thread.sleep(2000);
						} catch (InterruptedException e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}
					}
				}
			}
		}
	}

	/***********************************************************************/
	/** Private section **/
	/***********************************************************************/
	// Create a BroadcastReceiver for Bluetooth events
	private final BroadcastReceiver receiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			/* get the action for this event */
			String action = intent.getAction();

			/*
			 * if it corresponds to the packageName, then it was a permissions
			 * grant request
			 */
			if (actionString.equals(action)) {
				/* see if we got permission */
				if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED,
						false)) {
					/* if so, then try to open the accessory */
					UsbManager deviceManager = null;
					UsbAccessory accessory = null;

					deviceManager = (UsbManager) context
							.getSystemService(Context.USB_SERVICE);

					if (deviceManager == null) {
						// TODO: error. report to user?
						return;
					}

					accessory = (UsbAccessory) intent
							.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);

					parcelFileDescriptor = deviceManager
							.openAccessory(accessory);

					if (parcelFileDescriptor != null) {
						enabled = true;
						open = true;

						outputStream = new FileOutputStream(
								parcelFileDescriptor.getFileDescriptor());

						readThread = new ReadThread(parcelFileDescriptor);
						readThread.start();

						Log.d(TAG,
								"USBAccessoryManager:BroadcastReceiver()-1");
						handler.obtainMessage(
								what,
								new USBAccessoryManagerMessage(
										USBAccessoryManagerMessage.MessageType.READY,
										accessory)).sendToTarget();
					} else {
						// TODO: error. report to user?
						return;
					}
				}
			}

			if (UsbManager.ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
				/*
				 * if it was a device attach notice, then try to open the
				 * accessory
				 */
				UsbManager deviceManager = null;
				UsbAccessory[] accessories = null;
				UsbAccessory accessory = null;

				deviceManager = (UsbManager) context
						.getSystemService(Context.USB_SERVICE);

				if (deviceManager == null) {
					// TODO: error. report to user?
					return;
				}

				accessories = deviceManager.getAccessoryList();

				if (accessories == null) {
					// TODO: error. report to user?
					return;
				}

				accessory = accessories[0];

				parcelFileDescriptor = deviceManager.openAccessory(accessory);

				if (parcelFileDescriptor != null) {
					enabled = true;
					open = true;

					outputStream = new FileOutputStream(
							parcelFileDescriptor.getFileDescriptor());

					readThread = new ReadThread(parcelFileDescriptor);
					readThread.start();

					Log.d(TAG, "USBAccessoryManager:BroadcastReceiver()-2");
					handler.obtainMessage(
							what,
							new USBAccessoryManagerMessage(
									USBAccessoryManagerMessage.MessageType.READY,
									accessory)).sendToTarget();
				} else {
					// TODO: error. report to user?
					return;
				}
			} else if (UsbManager.ACTION_USB_ACCESSORY_DETACHED.equals(action)) {
				/*
				 * if it was a detach notice, then close the accessory and
				 * notify the user
				 */
				closeAccessory();
				handler.obtainMessage(
						what,
						new USBAccessoryManagerMessage(
								USBAccessoryManagerMessage.MessageType.DISCONNECTED))
						.sendToTarget();
			} else if (UsbManager.EXTRA_PERMISSION_GRANTED.equals(action)) {

			}
		}

	};

	/**
	 * Closes the accessory and cleans up all lose ends
	 * 
	 */
	private void closeAccessory() {

		if (open == false) {
			return;
		}

		open = false;
		enabled = false;
		permissionRequested = false;

		if (readThread != null) {
			readThread.cancel();
		}

		if (outputStream != null) {
			try {
				outputStream.close();
			} catch (IOException e) {
			}
		}

		if (parcelFileDescriptor != null) {
			try {
				parcelFileDescriptor.close();
			} catch (IOException e) {
			}
		}

		outputStream = null;
		// readThread = null;
		parcelFileDescriptor = null;
	}

	public boolean isClosed() {
		boolean isAlive;

		if (readThread != null) {
			isAlive = readThread.isAlive();

			if (isAlive == false) {
				readThread = null;
			}

			return isAlive;
		}

		return true;
	}

	/**
	 * The thread for reading data from the FileInputStream so the main thread
	 * doesn't get blocked.
	 */
	private class ReadThread extends Thread {
		private boolean continueRunning = true;

		private FileInputStream inputStream;
		private ParcelFileDescriptor myparcelFileDescriptor;

		public ReadThread(ParcelFileDescriptor p) {
			myparcelFileDescriptor = p;
			inputStream = new FileInputStream(p.getFileDescriptor());
		}

		@Override
		public void run() {
			byte[] buffer = new byte[1024]; // buffer store for the stream
			int bytes; // bytes returned from read()

			while (continueRunning) {
				try {
					// Read from the InputStream
					bytes = inputStream.read(buffer);

					// Send the obtained bytes to the UI Activity
					byte[] data = new byte[bytes];
					System.arraycopy(buffer, 0, data, 0, bytes);

					/*
					 * Synchronize to the readData object to make sure that no
					 * user API is accessing it at the moment
					 */
					synchronized (readData) {
						readData.add(data);
					}

					handler.obtainMessage(
							what,
							bytes,
							-1,
							new USBAccessoryManagerMessage(
									USBAccessoryManagerMessage.MessageType.READ,
									data)).sendToTarget();
				} catch (IOException e) {
					// Exiting read thread
					break;
				}
			}
		}

		public void cancel() {
			continueRunning = false;
			try {
				inputStream.close();
			} catch (IOException e) {
			}

			try {
				myparcelFileDescriptor.close();
			} catch (IOException e) {
			}
		}

	}


	/***********************************************************************/
	/** Exception definition section **/
	/***********************************************************************/
	// private final static String USB_ACCESSORY_NOT_CONNECTED =
	// "USB Accessory is not attached.";

	/**
	 * Exception that can be thrown by the manager - currently not used
	 * 
	 */
	class USBAccessoryManagerException extends RuntimeException {
		private static final long serialVersionUID = 2329617898883120248L;
		String errorMessage;

		public USBAccessoryManagerException(String message) {
			errorMessage = message;
		}

		public String toString() {
			return errorMessage;
		}
	}
}