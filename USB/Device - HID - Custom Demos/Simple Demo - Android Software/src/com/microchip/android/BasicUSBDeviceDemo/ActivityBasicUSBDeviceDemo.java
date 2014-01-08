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

import java.util.HashMap;
import java.util.Iterator;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.method.KeyListener;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.TextView;

/**
 * 
 * The main class the implements the GUI
 * for the USB device demos
 *
 */
public class ActivityBasicUSBDeviceDemo extends Activity{
	
	Demo demo = null;
	PendingIntent pendingIntent = null;
	
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.no_device);
    }
    
    @Override
    public void onResume(){
    	super.onResume();

    	/* Check to see if it was a USB device attach that caused the app to
    	 * start or if the user opened the program manually. 
    	 */
    	Intent intent = getIntent();        
        String action = intent.getAction();        
       
        if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) {
        	/* This application is starting as a result of a device being attached.  Get
        	 * the device information that caused the app opening from the intent, and
        	 * load the demo that corresponds to that device. */        	
            UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE); 
            demo = loadDemo(device);
        } else {
        	/* This application is starting up by a user opening the app manually.  We
        	 * need to look through to see if there are any devices that are already
        	 * attached.
        	 */        	
        	UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
        	HashMap<String, UsbDevice> deviceList = manager.getDeviceList();
        	Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
        	
        	while(deviceIterator.hasNext()){
        		/* For each device that we found attached, see if we are able to load
        		 * a demo for that device.
        		 */
        		demo = loadDemo(deviceIterator.next());
        		if(demo != null) {
        			break;
        		}
        	}
        }
        
		//Create a new filter to detect USB device events
		IntentFilter filter = new IntentFilter();
		
		filter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
		filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
		
        registerReceiver(receiver, filter);
        
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(getPackageName() + ".USB_PERMISSION"), 0);
    }
    
    @Override
    public void onPause(){
    	/* If there is a demo running, close it */
        if(demo != null){
        	demo.close();
        }
		demo = null;
		
		/* unregister any receivers that we have */
    	unregisterReceiver(receiver);
    	
    	super.onPause();
    }
    
    /**
     * loadDemo - tries to load the demo for the specified USB device,
     * including changing the view and title.
     * @param device The USB device whos demo to run
     * @return Demo - The resulting demo, null if none
     */
    public Demo loadDemo(UsbDevice device) {
    	/* Try to load a demo for the specified USB device */
		Demo tempDemo = Demo.loadDemo(this.getApplicationContext(), device, handler);
		
		if(tempDemo != null) {
			setContentView(R.layout.demo);
			
			/* Change the GUI view to match the loaded demo */
			if(	(tempDemo.getClass().equals(DemoLibUSB.class) == true) ||  
				(tempDemo.getClass().equals(DemoWinUSB.class) == true)) {
				findViewById(R.id.layout_toggle_led).setVisibility(View.VISIBLE);
				findViewById(R.id.layout_button_status).setVisibility(View.VISIBLE);
				findViewById(R.id.layout_potentiometer_status).setVisibility(View.GONE);
				findViewById(R.id.layout_MCP2200).setVisibility(View.GONE);
				
			} else if(tempDemo.getClass().equals(DemoMCHPUSB.class)) {
				findViewById(R.id.layout_toggle_led).setVisibility(View.VISIBLE);
				findViewById(R.id.layout_button_status).setVisibility(View.GONE);
				findViewById(R.id.layout_potentiometer_status).setVisibility(View.VISIBLE);
				findViewById(R.id.layout_MCP2200).setVisibility(View.GONE);
				
			} else if(tempDemo.getClass().equals(DemoCustomHID.class)) {
				findViewById(R.id.layout_toggle_led).setVisibility(View.VISIBLE);
				findViewById(R.id.layout_button_status).setVisibility(View.VISIBLE);
				findViewById(R.id.layout_potentiometer_status).setVisibility(View.VISIBLE);
				findViewById(R.id.layout_MCP2200).setVisibility(View.GONE);
				
			} else if(tempDemo.getClass().equals(DemoMCP2200.class)) {
				findViewById(R.id.layout_toggle_led).setVisibility(View.GONE);
				findViewById(R.id.layout_button_status).setVisibility(View.GONE);
				findViewById(R.id.layout_potentiometer_status).setVisibility(View.GONE);
				findViewById(R.id.layout_MCP2200).setVisibility(View.VISIBLE);
				
		        Spinner s = (Spinner) findViewById(R.id.baud_rate_select);
		        ArrayAdapter adapter = ArrayAdapter.createFromResource(
		                this, R.array.baud_rates, android.R.layout.simple_spinner_item);
		        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		        s.setAdapter(adapter);
		        
		        s.setOnItemSelectedListener(
	        	    new AdapterView.OnItemSelectedListener() {
	
						public void onItemSelected(AdapterView<?> arg0, View arg1,
								int arg2, long arg3) {
								((DemoMCP2200)demo).setBaudRate( arg0.getItemAtPosition(arg2).toString() );
						}
	
						public void onNothingSelected(AdapterView<?> arg0) {
							// TODO Auto-generated method stub
							
						}
        	    });
			}
			
			/* Set the title of the view to show the device information */
			setTitle(tempDemo.getDeviceTitle());
		}
		
		return tempDemo;
    }
    
    /**
     * Toggles the LEDs on the attached board, if supported.
     * @param v View that caused the toggle request
     */
    public void toggleLEDsOnClick(View v) {
    	/* Synchronized on demo here just in case a broadcast
    	 * event occurs and attempts to modify the demo
    	 * local variable while we are issuing the toggle LED
    	 * request.
    	 */
    	synchronized(demo) {
	    	if(demo != null) {
	    		demo.toggleLEDs();
	    	}
    	}
    }
    
    /**
     * Toggles the LEDs on the attached board, if supported.
     * @param v View that caused the toggle request
     */
    public void sendStringOnClick(View v) {
    	/* Synchronized on demo here just in case a broadcast
    	 * event occurs and attempts to modify the demo
    	 * local variable while we are issuing the toggle LED
    	 * request.
    	 */
    	synchronized(demo) {
	    	if(demo != null) {
	    		EditText box = (EditText)findViewById(R.id.entry);
	    		((DemoMCP2200)demo).sendString( box.getText().toString() );
	    		box.getText().clear();
	    	}
    	}
    }
    
    
    /**
     * Sets the R.id.title object of the current view with the
     * specified String value.
     * @param s The new title for the current view.
     */
    private void setTitle(String s) {
		TextView title = (TextView)findViewById(R.id.title);
		title.setText(s);
    }
    
	/***********************************************************************/
	/** Private section **/
	/***********************************************************************/
    
    /**
     * New BroadcastReceiver object that will handle all of the USB device
     * attach and detach events.
     */
    BroadcastReceiver receiver = new BroadcastReceiver() {    
    	public void onReceive(Context context, Intent intent) {
    		/* Get the information about what action caused this event */
    		String action = intent.getAction();       
    		
    		if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {       
    			/* If it was a USB device detach event, then get the USB device
    			 * that cause the event from the intent.
    			 */
    			UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);  
    	        
    			if (device != null) {
    				/* Synchronize to demo here to make sure that the main GUI isn't
    				 * doing something with the demo at the moment.  
    				 */
    				synchronized(demo) {
    					/* If the demo exits, close it down and free it up */
    					if(demo != null) {
    						demo.close();
		    				demo = null;
    					}
    				}
    		        setContentView(R.layout.no_device);
    			}
    		}
    	}
    };
    	
    /** 
     * Handler for receiving messages from the USB Manager thread or
     *   the LED control modules
     */
    private Handler handler = new Handler() {
    	@Override
    	public void handleMessage(Message msg) {
    		/* Determine what type of message was sent. And process it accordingly */
			if(msg.obj.getClass().equals(MessageButton.class)) {
				updateButton(R.id.button_status, ((MessageButton)msg.obj).isPressed);
			} else if(msg.obj.getClass().equals(MessagePotentiometer.class)) {
				updatePotentiometer(R.id.potentiometer_status, ((MessagePotentiometer)msg.obj).percentage);
			} else if(msg.obj.getClass().equals(MessageText.class)) {
				updateTextView(((MessageText)msg.obj).message);
			}
    	} //handleMessage
    }; //handler
    
    /**
     * Updates the GUI to show the status of the button press events
     * @param id The resource id of the TextView that needs to be changed.
     * @param pressed The state of the button
     */
    private void updateButton(int id, boolean pressed) {
		TextView textviewToUpdate;
		LinearLayout layoutToUpdate;
		
		/* Get the requested TextView and its parent layout using
		 * the passed in id.
		 */
		textviewToUpdate = (TextView)findViewById(id);
		layoutToUpdate = (LinearLayout)textviewToUpdate.getParent();
		
		/* Set the background resource to 0 to clear out the existing
		 * background information (removes artifacts).  
		 */
		layoutToUpdate.setBackgroundResource(0);
		
		if(pressed == true)
		{
			textviewToUpdate.setText(R.string.button_pressed);
			textviewToUpdate.setBackgroundResource(R.color.button_pressed);

		} else {
			textviewToUpdate.setText(R.string.button_not_pressed);
			textviewToUpdate.setBackgroundResource(R.color.button_not_pressed);
		}
    }
    
    /**
     * Updates the specified ProgressBar resource with the specified status
     * @param id The resource id of the ProgressBar that needs to be updated
     * @param percentage The new status of the progress bar.
     */
    private void updatePotentiometer(int id, int percentage) {
		ProgressBar bar;
		
		bar = (ProgressBar)findViewById(id);
		bar.setProgress(percentage);
    }
    
    /**
     * Updates the text box with the specified text
     * @param Text to place in the textbox
     */
    private void updateTextView(String text) {
		TextView box;
		
		box = (TextView)findViewById(R.id.basic_text_view);
		box.setText(box.getText()+text);
    }
   
    
}