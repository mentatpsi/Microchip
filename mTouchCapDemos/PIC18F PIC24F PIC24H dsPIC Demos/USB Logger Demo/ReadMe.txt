*******************************************************************************
********************** Readme File for USB Logger demo ************************
*******************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

This file contains information related to the mTouch Capacitive Touch Software
Library using the USB Logger Demo.

*******************************************************************************
                            Release Information
*******************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, XC8, C30 3.31 or Later, XC16

*******************************************************************************
     mTouch Capacitive Touch Library testing Details for USB Logger Demo
*******************************************************************************

The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. 8 buttons plugin module must be connected to the low part of the
sensors connector (channels from 0 to 7).

*******************************************************************************
     mTouch Capacitive Touch Library Set-up Details for USB Logger Demo
Note : Since the USB Logger Demo uses the 8 Buttons plug-in board, all the instructions
       related to the Basic Button demo applies to USB Logger demo as well. 
*******************************************************************************

1)  USB Logger project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure Basic Button?

Step 1: Assign required number of sensors in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 3: Call pereodically MTouchAcquisition() acquisition functions.

Step 4: Get state using MTouchGetSensorState(...) function.

*******************************************************************************
               Demonstration of the USB Logger demo using
                 the mTouch Capacitive Touch Library
*******************************************************************************

The main purpose of the USB Logge Demo is to demonstrate the behaviour of the
capacitive touch buttons when they are in the pressed state and the released state.
It will also help in detemining the optimal overasampling factor and threshold
level for the application.

When the demo board is connected to PC with USB cable the Windows OS will prompt
to install driver. The driver INF file mchpcdc.inf is located in 
"...\mTouchCapDemos\Utilities\Pickit Serial Loader".

The demo works with mTouch GUI utility "mTouch GUI.exe" located in 
"...\mTouchCapDemos\Utilities\mTouch One-Way GUI".
Please read "mTouch Library GUI Help.chm" to get started.

The GUI utility draws graphs for difference (delta) between average and current
value for each sensor. Also it displays current states (pressed/released) for
the sensors.


*******************************************************************************
                   Limitations of the USB Logger Demo
*******************************************************************************
HI-TECH compiler is not supported for this demo.

*******************************************************************************
                                 Help File
*******************************************************************************

For more information about mTouch Software Library, refer "mTouch Cap Library Help.chm"
file located in "...\Microchip\Help" folder.