package com.microchip.android.WebBootLoader;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.Vector;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

/** A class that extends activity. This class handles inter-thread communication
 *  and updating the layout for the web bootloader demo.
 *  
 * @author Microchip Technology Inc.
 *
 */
public class WebBootLoader extends Activity {
	// Message commands from other threads 
	private static final int USB_EVENT_RECEIVED = 0;
	private static final int XML_PARSING_COMPLETE = 1;
	private static final int USB_PACKET_READY = 2;

	// Commands for USB Packets
	protected static final byte ENTER_BOOT_MODE_REQ = 0;
	protected static final byte START_BOOT_MODE_REQ = 1;
	protected static final byte READ_FILE_REQ = 2;
	protected static final byte LOAD_COMPLETE_RSP = 3;

	// Error codes for LOAD_COMPLETE_RSP
	protected static final byte BOOT_SUCCESS = 0;
	protected static final byte VERIFICATION_FAIL = 1;

	// Constants
	protected static final boolean updateFromInternet = true;
	protected static final String xml_file_name = "versions.xml";
	protected static final String file_system_dir = "sdcard/Android/data/com.microchip.android.WebBootLoader/";
	protected static final String xml_file_url = "http://ww1.microchip.com/downloads/en/SoftwareLibrary/versions.xml";

	// Private variables for WebBootLoader class
	private USBAccessoryManager accessoryManager;
	private VersionInfo accessoryInfo;
	private VersionInfo updatedAccessoryInfo;
	private BinaryFileParser binParser;

	
	// Function Override that is called when the application is started
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// Call super class function 
		super.onCreate(savedInstanceState);
		
		// Set Content view to layout defined in main.xml
		setContentView(R.layout.main);
		
		// Instantiate USB accessory manager with the handler inside WebBootLoader and it's Message command
		accessoryManager = new USBAccessoryManager(handler, USB_EVENT_RECEIVED);
	}

	// Function Override that is called after onCreate, a restart of the activity, or resuming from a pause
	@Override
	public void onResume() {
		// Call super class function
		super.onResume();
		
		// Enable the USB thread to start reading USB packets sent from host
		accessoryManager.enable(this);
	}

	// Function Override that is called when another activity comes in front of this one
	@Override
	public void onPause() {
		// Call super class function
		super.onPause();
		
		// Disable the USB thread
		accessoryManager.disable(this);
		
		// Disconnect accessory
		disconnectAccessory();
	}

	/** Callback function for when the Upgrade button is pressed 
	 * 
	 * @param view
	 */
	public void onClickUpgrade(View v) {
		TextView view;
		Button updateButton;
		byte[] enterBootloader = new byte[5];

		// Update title to show accessory is entering into bootloader mode
		view = (TextView) findViewById(R.id.new_title);
		view.setText("Accessory Entering Bootloader Mode..."); 

		// Hide update button
		updateButton = (Button) findViewById(R.id.update_button);
		updateButton.setVisibility(View.GONE);

		// Fill USB packet with 5 bytes, ENTER_BOOT_MODE_REQ followed by "BOOT"
		enterBootloader[0] = ENTER_BOOT_MODE_REQ;
		enterBootloader[1] = 'B';
		enterBootloader[2] = 'O';
		enterBootloader[3] = 'O';
		enterBootloader[4] = 'T';

		// Write five bytes to USB output buffer
		accessoryManager.write(enterBootloader);
	}

	/** Show disconnect of accessory in layout
	 * 
	 */
	public void disconnectAccessory() {
		TextView view;
		Button updateButton;

		// Stop parsing the binary file if that thread has been started
		if (binParser != null) {
			binParser.Cancel();
		}

		// Update the layout to show disconnect of layout
		view = (TextView) findViewById(R.id.description);
		view.setText("No device attached"); 

		view = (TextView) findViewById(R.id.model);
		view.setVisibility(View.GONE);

		view = (TextView) findViewById(R.id.version);
		view.setVisibility(View.GONE);

		view = (TextView) findViewById(R.id.new_title);
		view.setVisibility(View.GONE);

		updateButton = (Button) findViewById(R.id.update_button);
		updateButton.setVisibility(View.GONE);
	}

	/***********************************************************************/
	/** Private section **/
	/***********************************************************************/
    // Create a Handler to accept messages from other threads
	private Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			byte[] commandCode = new byte[1];
			TextView view;

			// Which thread sent message?
			switch (msg.what) {
			// If the USB Accessory Manager thread sent message
			case USB_EVENT_RECEIVED:
				// Then check to see what event occurred
				switch (((USBAccessoryManagerMessage) msg.obj).type) {
				// If an accessory has been connected, then do nothing
				case CONNECTED:
					break;
				// If an accessory has been connected, and is ready
				case READY:
					// Instantiate a new VersionInfo object to keep track of the accessory's information
					accessoryInfo = new VersionInfo(((USBAccessoryManagerMessage) msg.obj).accessory);

					// Update the view with the model, description, and version of the attached accessory
					view = (TextView) findViewById(R.id.model);
					view.setText(accessoryInfo.getModel());

					view = (TextView) findViewById(R.id.description);
					view.setText(accessoryInfo.getDescription());
					view.setVisibility(View.VISIBLE);
					
					view = (TextView) findViewById(R.id.version);
					view.setText("v" + accessoryInfo.getVersionString()); 
					view.setVisibility(View.VISIBLE);

					// If accessory is in Bootloader mode and we've gotten the updated application info
					if ((accessoryInfo.isInBootloadMode()) && (updatedAccessoryInfo != null)) {
						// Then we can start bootloading by sending the START_BOOT_MODE_REQ command to accessory
						byte[] startBootloaderPacket = new byte[1];
						startBootloaderPacket[0] = START_BOOT_MODE_REQ;
						accessoryManager.write(startBootloaderPacket);

						// And update the title showing bootloader is starting
						view = (TextView) findViewById(R.id.new_title);
						view.setText("Starting Bootloading..."); 
						view.setVisibility(View.VISIBLE);
					} else {
						// Otherwise update the title showing searching for updates
						view = (TextView) findViewById(R.id.new_title);
						view.setText("Searching for updates..."); 
						view.setVisibility(View.VISIBLE);
						
						// If we are updating from the Internet
						if(updateFromInternet)
						{
							// Check for connectivity
							ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
							NetworkInfo netInfo = cm.getActiveNetworkInfo();
							if (netInfo != null && netInfo.isConnectedOrConnecting()) {
								// If connected, then try to connect to versions.xml on microchip.com website
								try {
									URL xmlURL = new URL(xml_file_url); 
									URLConnection xmlConnection = xmlURL.openConnection();
									
									// If successfully connected, then instantiate a VersionsXMLParser thread to parse versions.xml 
									VersionsXMLParser parser = new VersionsXMLParser(handler, XML_PARSING_COMPLETE);
									parser.ParseThreaded(xmlConnection.getInputStream());
								} catch (MalformedURLException e) {
									// If the URL is incorrect, update title
									e.printStackTrace();
									view.setText("Unable to connect. Illegal URL."); 
								} catch (IOException e) {
									// If a file error occurs, update title
									e.printStackTrace();
									view.setText("Unable to connect. File Error."); 
								}
							} else {
								// If not connected, update title
								view.setText("Internet connection not available."); 
							}
						} else {
							// Otherwise, get xml file from Android file system, inside WebBootLoader directory.
							File file = new File(file_system_dir + xml_file_name);
							
							// Check to see if file exists
							if(file.exists()) {
								// If so, then instantiate a new VersionXMLParser thread to parse the file
								VersionsXMLParser parser = new VersionsXMLParser(handler, XML_PARSING_COMPLETE);
								parser.ParseThreaded(file);
							} else {
								// If the file doesn't exist, update title
								view.setText("File does not exist."); 
							}
						}
					}
					break;
				// If the accessory was disconnected
				case DISCONNECTED:
					// Then update the view
					disconnectAccessory();
					break;
				// If a USB packet was read
				case READ:
					// Then make sure that the accessory is still connected
					if (accessoryManager.isConnected() == false) {
						return;
					}

					// While data is available to read
					while (accessoryManager.available() != 0) {
						// Read the first byte to see what the command was
						accessoryManager.read(commandCode);
						switch (commandCode[0]) {
						// If READ_FILE_REQ command read
						case READ_FILE_REQ:
							// Then the accessory wants to be updated with new firmware
							view = (TextView) findViewById(R.id.new_title);
							view.setText("Updating Accessory..."); 

							// If we are updating from the Internet
							if(updateFromInternet)
							{
								// Check for connectivity
								ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
								NetworkInfo netInfo = cm.getActiveNetworkInfo();
								if (netInfo != null && netInfo.isConnectedOrConnecting()) {
									// If connected, then try to connect to the associated url for the binary file on microchip.com website.
									try {
										URL binaryURL = new URL(updatedAccessoryInfo.getURL());
										URLConnection binaryConnection = binaryURL.openConnection();
										
										// If successfully connected, then instantiate a BinaryFileParser thread to parse the binary file 
										binParser = new BinaryFileParser(handler, USB_PACKET_READY);
										binParser.ParseThreaded(binaryConnection.getInputStream());
									} catch (MalformedURLException e) {
										// If the URL is incorrect, update title
										e.printStackTrace();
										view.setText("Unable to connect. Illegal URL."); 
									} catch(IOException e) {
										// If a file error occurs, update title
										view.setText("Unable to connect. File error."); 
										e.printStackTrace();
									}
								} else {
									// If not connected, update title
									view.setText("Internet connection not available."); 
								}
							} else {
								// Otherwise, get binary file from Android file system, inside WebBootLoader directory.
								File file = new File(file_system_dir + updatedAccessoryInfo.getFileName()); 
								
								// Check to see if file exists
								if(file.exists()) {
									// If so, then instantiate a new BinaryFileParser thread to parse the file
									binParser = new BinaryFileParser(handler, USB_PACKET_READY);
									binParser.ParseThreaded(file);
								} else {
									// If the file doesn't exist, update title
									view.setText("File does not exist."); 
								}
							}
							break;
						// If LOAD_COMPLETE_RSP command read
						case LOAD_COMPLETE_RSP:
							byte[] errorCode = new byte[4];
							int numRead;
							
							// Cancel the binary file parser thread and read the next four bytes for the error code
							binParser.Cancel();
							view = (TextView) findViewById(R.id.new_title);
							numRead = accessoryManager.read(errorCode);

							// If less than four bytes received  
							if (numRead < 4) {
								// No error code received, Update title
								view.setText("No error code received."); 
							} else {
								// Otherwise Convert errorCode from 4 byte array to int
								ByteBuffer bb = ByteBuffer.wrap(errorCode);
								IntBuffer ib = bb.asIntBuffer();
								int errorCodeInt = ib.get(0);
	
								// Update title based on error code
								switch (errorCodeInt) {
								case BOOT_SUCCESS:
									view.setText("Update Complete. Resetting..."); 
									break;
								case VERIFICATION_FAIL:
									view.setText("Verification Failed."); 
									break;
								default:
									view.setText("Unknown Error: " + Integer.toString(errorCodeInt)); 
									break;
								}
							}
							break;
						default:
							break;
						}
					}
					break;
				}
				break;
			// If the XML parsing thread sent the message
			case XML_PARSING_COMPLETE:
				// Get the parser object
				VersionsXMLParser parser = (VersionsXMLParser) msg.obj;

				view = (TextView) findViewById(R.id.new_title);

				// If in bootloader mode
				if(accessoryInfo.isInBootloadMode()) {
					// That means that we started in bootloader mode and don't have an application, so just filter by board name (description)
					parser.filterResults(accessoryInfo.getDescription());
				} else {
					// Otherwise we can filter by both application (model) and board name (description)
					parser.filterResults(accessoryInfo.getModel(), accessoryInfo.getDescription());
				}
				
				// Also filter to get the latest version
				parser.filterResultsBestVersionOnly();

				// And save the results
				Vector<VersionInfo> versions = parser.GetResults();

				// If there are any versions available
				if ((versions != null) && (versions.size() != 0)) {
					// We'll just take the first one
					updatedAccessoryInfo = versions.elementAt(0);
					
					// If we're in bootloader mode
					if (accessoryInfo.isInBootloadMode()) {
						// Then we can start bootloading by sending the START_BOOT_MODE_REQ command to accessory
						byte[] startBootloaderPacket = new byte[1];
						startBootloaderPacket[0] = START_BOOT_MODE_REQ;
						accessoryManager.write(startBootloaderPacket);

						// And update the title showing bootloader is starting
						view = (TextView) findViewById(R.id.new_title);
						view.setText("Starting Bootloading..."); 
						view.setVisibility(View.VISIBLE);
					} else {
						// Otherwise, we'll see if this version is newer than the current one
						if (updatedAccessoryInfo.isNewerVersionThan(accessoryInfo) == true) {
							// if so, then we can show the available newer version with a button so they can choose if they want to update 
							view.setText("Update Availiable:"); 
							Button updateButton;
							updateButton = (Button) findViewById(R.id.update_button);
							updateButton.setText("Update to version v" + versions.elementAt(0).getVersionString()); 
							updateButton.setVisibility(View.VISIBLE);
						} else {
							// Otherwise show there are no updates
							view.setText("No updates found."); 
						}
					}
				} else {
					// Otherwise show there are no updates
					view.setText("No updates found."); 
				}
				break;
			// If the binary file parser thread sent the message
			case USB_PACKET_READY:
				// Then read the packet of bytes that will be sent
				byte[] packet = (byte[]) msg.obj;
				Log.d("WebBootloader", "Binary Message Received, size: " + Integer.toString(packet.length));  
				
				// And add that packet to the USB accessory manager's output buffer
				accessoryManager.write(packet);
				break;
			default:
				break;
			}
		}
	};
}