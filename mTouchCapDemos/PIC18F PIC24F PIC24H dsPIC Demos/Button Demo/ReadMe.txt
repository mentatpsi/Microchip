**********************************************************************************
************************** Readme File for Button Demo ***************************
**********************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

*********************************************************************************

This file contains the information related to the mTouch Capacitive Touch Software
Library used for Button Demo. 

**********************************************************************************
                             Release Information
**********************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16

**********************************************************************************
    	mTouch Capacitive Touch Library Testing Details for Button Demo
**********************************************************************************
The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. 8 buttons plugin module must be connected to the low part of the
sensors connector (channels from 0 to 7).

**********************************************************************************
    	mTouch Capacitive Touch Library Set-up Details for Button Demo
**********************************************************************************

1)  Button project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure Button?

Step 1: Assign required number of sensors and buttons in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 3: Assign sensors used for the Buttons with MTouchSetButton(...).

Step 4: Call pereodically MTouchAcquisition() acquisition and MTouchDecode()
        decoding functions.

Step 5: Get state using MTouchGetButtonState(...) function.

**********************************************************************************
        mTouch Capacitive Touch Library Demonstration for Button Demo
**********************************************************************************

The purpose of this demo is to show the presence of touch on the Cap Touch Buttons. 
When the cap touch button is pressed state, the corresponding LED will lit-up
to indicate the touch. The buttons are decoded differently:

"8" and "9" - DECODE_MOST_PRESSED (only button with a bigger signal is indicated 
              as pressed),
"A" and "B" - DECODE_TOGGLE (toggled buttons),
"C" and "D" - DECODE_PRESS_REPEAT (if the buttons are held in pressed state they
              will toggle press/release state continuously),
"E" and "F" - DECODE_PRESS_REPEAT|DECODE_ONE_EVENT (if the buttons are held in
              pressed state they will toggle pressed/released state continuously,
              detect transition from press state to released and from released to
              pressed. If the transition is not detected will return CONTROL_IDLE
              flag).

**********************************************************************************
                                  mTouch GUI
**********************************************************************************

This demo sends data from all sensors to UART. Thus the states and the signals(deltas)
from the sensors can be displayed with an mTouch GUI utility "mTouch GUI.exe"
located in "...\mTouchCapDemos\Utilities\mTouch One-Way GUI" folder.
Please read "mTouch Library GUI Help.chm" in this folder to get started. RX line of the
PICkit Serial (pin 6) should be connected to pin 6 (RF0 label on silk screen)
of the J2 connector for "PIC18 Eval Board" and connected to pin 10 (RF3 label on
silk screen) of the J2 connector for "PIC24 Eval Board".
Also PICkit Serial pin 2 MUST be connected to Vdd on the board (pin 1 of J2) and
PICkit Serial pin 3 MUST be connected to ground on the board (pin 2 of J2).  
 
**********************************************************************************
                                   Help File
**********************************************************************************

For more information about mTouch Software Library, refer "mTouch Cap Library Help.chm"
file located in "...\Microchip\Help" folder.