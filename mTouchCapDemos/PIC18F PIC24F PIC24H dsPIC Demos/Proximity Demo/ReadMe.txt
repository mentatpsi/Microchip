*******************************************************************************
******************* Readme File for Proximity Demo ****************************
*******************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

This file contains information related to the mTouch Capacitive Touch Software
Library for Proximity Demo

*******************************************************************************
                            Release Information
*******************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16

*******************************************************************************
    mTouch Capacitive Touch Library testing Details for Proximity Demo
*******************************************************************************

The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. 8 buttons plugin module must be connected to the low part of the
sensors connector (channels from 0 to 7).

*******************************************************************************
     mTouch Capacitive Touch Library Set-up Details for Proximity Demo
Note : Since the Proximity Demo uses the 8 Buttons plug-in board, all the instructions
       related to the Basic Button demo applies to Proximity demo as well. 
*******************************************************************************

1)  Proximity project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure Button?

Step 1: Assign required number of sensors in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 4: Call pereodically MTouchAcquisition() acquisition function.

Step 5: Get state using MTouchGetSensorState(...) function.
 
*******************************************************************************
                        Proximity Demonstration
*******************************************************************************

The purpose is to demonstrate proximity detection (detect fingers, hand or other
objects approaching the button "8".)

Note:  In general, the Proximity mode can detect a person's hand within 1-2 inches.
However, the Evaluation board running the Proximity demo software may be
sensitive to the surface material on which it is placed or objects in its vicinity,
so results may vary.

LED 0 on the board will illuminate as a finger, hand or other object approaches
the button "8". At the startup the demo will execute an "autozero" procedure that
nullifies the capacitive coupling effects of nearby surrounding objects or surfaces.
This is indicated by LED 15 illuminated for a few seconds. After the LED 15
turns off, the demo is ready for Proximity detection.

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