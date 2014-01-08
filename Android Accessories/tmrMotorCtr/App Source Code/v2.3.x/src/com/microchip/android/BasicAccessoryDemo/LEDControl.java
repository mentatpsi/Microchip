/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
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

package com.microchip.android.BasicAccessoryDemo;

import android.content.Context;
import android.graphics.Canvas;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;

/** Class to control a custom button representing an LED
 * 
 * @author Microchip Technology Inc.
 *
 */
public class LEDControl extends ImageButton implements OnClickListener{
	/* Constant value sent to the GUI handler in the "what" feild */
	public static final int UPDATE_LED_SETTING		 	= 1;
	
	/* Boolean indicating the current LED status */
	private boolean on;
	
	/* Boolean that indicates if the LED button should be active */
	private boolean enabled;
	
	/* Reference to the user interface handler */
	private Handler uiHandler;
	
	/** Creates instance of the LED button
	 * 
	 * @param context The associated context
	 */
	public LEDControl(Context context) {
		super(context);
		init();
	}

	/** Creates instance of the LED button
	 * 
	 * @param context The associated context
	 * @param attrs The associated AttributeSet for this item
	 */
	public LEDControl(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}
	
	/** Initialized the internal state variables and registers the
	 *  click listener for this button
	 */
	private void init() {
		on = false;
		enabled = false;
		this.setOnClickListener(this);
		uiHandler = null;
	}
	
	/** Set the user interface handler where this class should
	 * send messages to when a press event occurs
	 * @param handler If a GUI wants to be notified of this event, it can 
	 *                send it's handler here (only supports one handler).
	 */
	public void setHandler(Handler handler) {
		uiHandler = handler;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
	}
	
	public void onClick(View view) {
		if(enabled == true) {
			setState(!on);
		}
	}
	
	public void setEnabled(boolean state) {
		enabled = state;
	}
	
	/** Gets the current state of the button
	 * 
	 * @return boolean that indicates the current button state, true = on
	 */
	public boolean getState() {
		return on;
	}
	
	/** Sets the current state of the button
	 * 
	 * @param state true = on, false = off
	 */
	public void setState(boolean state) {
		
		//save the new value for the LED from the parameter passed in.
		on = state;
		
		if(state == false) {
			//If the user is turning off the LED, then
			//  set the image resource for the button to the "led_off" image
	        this.setImageResource(R.drawable.led_off);
		} else {
			//If the user is turning on the LED, then
			//  set the image resource for the button to the "led_on" image
			this.setImageResource(R.drawable.led_on);
		}
		
		Message ledUpdate = Message.obtain(uiHandler, UPDATE_LED_SETTING);
		
		if(uiHandler != null) {
			uiHandler.sendMessage(ledUpdate);
		}
	}
}
