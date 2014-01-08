**********************************************************************************
*********************** Readme File for Matrix Button Demo ***********************
**********************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

*********************************************************************************

This file contains the information related to the mTouch Capacitive Touch Software
Library used for Matrix Button Demo. 

**********************************************************************************
                             Release Information
**********************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16

**********************************************************************************
    mTouch Capacitive Touch Library Testing Details for Matrix Button Demo
**********************************************************************************
The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. mTouch Matrix Buttons plugin module must be connected to the low part
of the sensors connector (channels from 0 to 6).

**********************************************************************************
    mTouch Capacitive Touch Library Set-up Details for Matrix Button Demo
**********************************************************************************

1)  Matrix Button project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure Matrix Button?

Step 1: Assign required number of sensors and matrix buttons in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 3: Assign sensors used for the Matrix Buttons with MTouchSetMatrixButton(...).

Step 4: Call pereodically MTouchAcquisition() acquisition and MTouchDecode()
        decoding functions.

Step 5: Get state using MTouchGetMatrixButtonState(...) function.

**********************************************************************************
     mTouch Capacitive Touch Library Demonstration for Matrix Button Demo
**********************************************************************************

The purpose of this demo is to show the presence of touch on the Matrix Buttons. 

This demo uses 7 CTMU channels to perform 4x3 Matrix Buttons functionality. 
Each Matrix Button is multiplexed with 2 CTMU Channels.
The buttons are decoded differently:
"0","1","2" and "3"   - DECODE_MOST_PRESSED (only button
                        with a bigger signal is indicated as pressed).
"4","5","6" and "7"   - DECODE_TOGGLE (toggled buttons).
"8","9","10" and "11" - DECODE_PRESS_REPEAT (if the buttons are held they will
                        toggle pressed/released state continuously).


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
