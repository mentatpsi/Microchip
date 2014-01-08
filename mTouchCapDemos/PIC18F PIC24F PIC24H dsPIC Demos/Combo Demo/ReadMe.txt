*******************************************************************************
********************** Readme File for Combo Demo *****************************
*******************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

******************************************************************************

This file contains the information related to the mTouch Capacitive Touch
Software Library used for Combo Demo. 

*******************************************************************************
                            Release Information
**********************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16

**********************************************************************************
    mTouch Capacitive Touch Library Testing Details for Combo Demo
**********************************************************************************

The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. 2 Channel Slider plugin module must be connected to channels 0
and 1. 8 buttons plugin module must be connected to channels from 4 to 11.

*******************************************************************************
    	   mTouch Capacitive Touch Library Set-Up Details  
*******************************************************************************

1)  Combo project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure mTouch controls?

Step 1: Assign required number of sensors and controls in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 3: Assign sensors used for controls with MTouchSet2ChSlider(...),
        MTouchSet4ChSlider(...), MTouchSetButton(...) and
        MTouchSetMatrixButton(...).

Step 4: Call pereodically MTouchAcquisition() acquisition and MTouchDecode()
        decoding functions.

Step 5: Get states and values from controls using MTouchGet2ChSliderState(...),
        MTouchGet2ChSliderValue(...), MTouchGet4ChSliderState(...),
        MTouchGet4ChSliderValue(...), MTouchGetButtonState(...) and
        MTouchGetMatrixButtonState(...) functions.

*******************************************************************************
    	mTouch Capacitive Touch Library Demonstration for Combo Demo
*******************************************************************************

Operation of combination of 2-ch Slider and 8 Buttons is demonstrated.

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