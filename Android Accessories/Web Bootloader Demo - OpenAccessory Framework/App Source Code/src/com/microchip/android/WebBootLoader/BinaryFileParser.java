package com.microchip.android.WebBootLoader;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import android.os.Handler;
import android.os.Message;

/** This class parses the binary file that contains the new program memory contents
 *  that will be sent to the attached PIC android accessory. It breaks the contents
 *  up inside the binary file into packets that will be sent over USB.
 *  
 * @author Microchip Technology Inc.
 *
 */
public class BinaryFileParser implements Runnable {
	// Private variables
	private Handler handler = null;
	private int what = 0;
	private InputStream stream;
	private boolean running = false;
	
	// Constant for USB buffer size
	private static final int USB_BUFFER_SIZE = 8192;
	
	
	/** Creates a new BinaryFileParser object that saves the handler and message code
	 * 
	 *  @param handler The handler to send messages to
	 *  @param what    The message code to send to the handler
	 */
	public BinaryFileParser(Handler handler, int what) {
		this.handler = handler;
		this.what = what;
	}
	
	/** Starts the thread and parses the binary file
	 * 
	 *  @param file The file to parse
	 */
	public void ParseThreaded(File file) {
		// Create a stream and save it from the file
		try {
			stream = new BufferedInputStream(new FileInputStream(file));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		// Start parsing in new thread
		new Thread(this).start();
		
		// Set the running flag to true
		running = true;
	}
	
	/** Starts the thread and parses the binary file
	 * 
	 *  @param stream The stream to parse
	 */
	public void ParseThreaded(InputStream stream) {
		// Save stream and start parsing in new thread
		this.stream = stream;
		new Thread(this).start();
		
		// Set the running flag to true
		running = true;
	}
	
	/** Cancels the running thread
	 * 
	 */
	public void Cancel() {
		// Set the running flag to false so the run() function will stop executing
		running = false;
	}
	
	/** The running function of the thread. Parses through the binary file and sends it in packets to handler.
	 * 
	 **/
	@Override
	public void run() {
		byte[] buffer = new byte[USB_BUFFER_SIZE];
		int bytesRead = 0;
		
		// While the running flag is set to true
		while(running) {
			try {
				// Read bytes from the stream and save them to the buffer
				bytesRead = stream.read(buffer);
				
				// If bytesRead is -1
				if(bytesRead == -1) {
					// Then we're EOF and can exit thread
					break;
				} else {
					// Otherwise send the number of bytes that were read to the handler
					byte[] bytesToSend = new byte[bytesRead];
					System.arraycopy(buffer, 0, bytesToSend, 0, bytesRead);
					Message bytesReadMessage = Message.obtain(handler, what);
					bytesReadMessage.obj = bytesToSend;
					handler.obtainMessage(what, bytesToSend).sendToTarget();
				}
			} catch (IOException e) {
				e.printStackTrace();
				// If exception occurs, then exit thread
				break;
			}
		}
	}
}
