*******************************************************************************
********************** Readme File for Low Power Demo**************************
*******************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

*******************************************************************************

This file contains the information related to the mTouch Capacitive Touch
Software Library used for Low Power Demo using the Buttons. 

*******************************************************************************
                            Release Information
*******************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16

*******************************************************************************
      mTouch Capacitive Touch Library Testing Details for Low Power Demo
*******************************************************************************

The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. The Gesture Demo uses the 8 Buttons plug-in board connected
to channels from 0 to 7. 

*******************************************************************************
    mTouch Capacitive Touch Library Set-up Details for Low Power Demo
Note : Since the Low Power Demo uses the 8 Buttons plug-in board, all the instructions
       related to the Basic Button demo applies to Low Power demo as well. 
*******************************************************************************
1)  Low Power project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure Basic Button?

Step 1: Assign required number of sensors in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 3: Call pereodically MTouchAcquisition() acquisition function.

Step 4: Get buttons states using MTouchGetSensorState(...) function.

*******************************************************************************
      mTouch Capacitive Touch Library Demonstration for Low Power Demo
*******************************************************************************
The Low Power demo is used to demonstrate the power saving technique
while scanning the capsense buttons using a 8 Buttons plug-in board.
This will greatly help in reducing the power consumed when the device is Idle.
This can be acheived keeping the device in Sleep at regular intervals depending
on the need of the application.

There are 2 different modes that is implemented in the Low power logic.

ACTIVE Mode: This mode will execute the application logic.

SLEEP  Mode: In this mode, the device will be placed in Sleep for a duration which
             is determined by the WDT timeout.

The steps involved in the implementation of the logic is given below:

1). The Low Power Demo will initially scan all the buttons for any touch condition
    for a fixed period of time - timeout.
2). If any of the button is pressed, then the timeout counter will be reset and
    the application will stay in ACTIVE Mode.
3). If none of the buttons are pressed during this timeout period, the device will be placed
    in SLEEP mode.
4). The device will come out of Sleep condition using a WDT interrupt.
5). A LED D16 will be lit to indicate that the device is in SLEEP Mode.
6). If the logic senses any button press during the SLEEP Mode, the logic will shift
    to the ACTIVE mode and LED D16 will be switched off.

*******************************************************************************
                                   Help File
*******************************************************************************

For more information about mTouch Software Library, refer "mTouch Cap Library Help.chm"
file located in "...\Microchip\Help" folder.
