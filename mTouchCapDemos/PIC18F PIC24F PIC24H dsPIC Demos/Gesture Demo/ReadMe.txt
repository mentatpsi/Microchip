*******************************************************************************
********************** Readme File for Gesture Demo ***************************
*******************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

*******************************************************************************

This file contains the information related to the mTouch Capacitive Touch
Software Library used for Gesture Demo. 

*******************************************************************************
                            Release Information
*******************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16
 
*******************************************************************************
      mTouch Capacitive Touch Library Testing Details for Gesture Demo
*******************************************************************************

The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. The Gesture Demo uses the 8 Buttons plug-in board connected
to channels from 0 to 7. 

*******************************************************************************
      mTouch Capacitive Touch Library Set-up Details for Gesture Demo
Note : Since the Gesture Demo uses the 8 Buttons plug-in board, all the instructions
       related to the Basic Button demo applies to Gesture demo as well. 
*******************************************************************************
1)  Gesture project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure Basic Button?

Step 1: Assign required number of sensors and buttons in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 3: Call pereodically MTouchAcquisition() acquisition function.

Step 5: Get state using MTouchGetSensorState(...) function.


*******************************************************************************
    	mTouch Capacitive Touch Library Demonstration for Gesture Demo
*******************************************************************************
The purpose of this demo is to show the presence of swiping gesture on
the Cap Touch buttons. 

Function of the demo:
When the swiping gesture on any two or more buttons is performed, the LEDs show
the direction of the swipe.
The upper 8 LEDs(LED D8-D15) are used for Up-Down gesture. 
The lower 8 LEDs(LED D1-D7) are used for Right-Left gesture. 

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


*******************************************************************************
                                Help File
*******************************************************************************

For more information about mTouch Software Library, refer "mTouch Cap Library Help.chm"
file located in "...\Microchip\Help" folder.