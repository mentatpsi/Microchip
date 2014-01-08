*********************************************************************************
******************** Readme File for 2 Channel Slider Demo **********************
*********************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

*********************************************************************************
This file contains the information related to the mTouch Capacitive Touch Software
Library used for 2 Channel Slider Demo. 

**********************************************************************************
                             Release Information
**********************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16

**********************************************************************************
   mTouch Capacitive Touch Library Testing Details for 2 Channel Slider Demo
**********************************************************************************
The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. 2 Channel Slider plugin module must be connected to the low part
of the sensors connector (channels 0 and 1).

**********************************************************************************
   mTouch Capacitive Touch Library Set-up Details for 2 Channel Slider Demo
**********************************************************************************

1)  2 Channel Slider project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure 2-Channel Slider?

Step 1: Assign required number of sensors and sliders in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 3: Assign sensors used for 2-Channel Slider with MTouchSet2ChSlider(...).

Step 4: Call pereodically MTouchAcquisition() acquisition and MTouchDecode()
        decoding functions.

Step 5: Get state and value from slider using MTouchGet2ChSliderState(...) and
        MTouchGet2ChSliderValue(...) functions.

**********************************************************************************
   mTouch Capacitive Touch Library Demonstration for 2 Channel Slider Demo
**********************************************************************************

The purpose of this demo is to show the presence of touch on the 2 Channel Slider. 
Evaluation board will indicate a position of touch on the slider.
The output is ratio-metrically calculated from 0 to 1000 proportional to the finger
position on the slider.

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
