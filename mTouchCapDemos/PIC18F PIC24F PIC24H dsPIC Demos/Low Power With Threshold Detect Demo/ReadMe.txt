*******************************************************************************
*********** Readme File for Low Power With Threshold Detect Demo **************
*******************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

*******************************************************************************

This file contains the information related to the mTouch Capacitive Touch
Software Library used for Low Power With Threshold Detect Demo using the Basic
Buttons. 

*******************************************************************************
                            Release Information
*******************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C30 3.31 or Later, XC16

*******************************************************************************
mTouch Capacitive Touch Library Testing Details for Low Power With Threshold Detect Demo
*******************************************************************************

The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. The Gesture Demo uses the 8 Buttons plug-in board connected
to channels from 0 to 7. 

The mTouch Capacitive Touch Library software can be customized to customer's
application-specific boards using below mentioned set-up details.

*******************************************************************************
mTouch Capacitive Touch Library Set-up Details for Low Power With Threshold Detect Demo
Note : Since the Low Power Demo uses the 8 Buttons plug-in board, all the instructions
       related to the Basic Button demo applies to Low Power demo as well. 
*******************************************************************************
1)  Low Power With Threshold Detect project can be configured as per user's application by
    modifying header file "mTouchConfig.h".
  
2) How to configure Basic Button?

Step 1: Assign required number of sensors in mTouchConfig.h

Step 2: Initialize mTouch library with mTouchInit() and all sensors with
        MTouchSetSensor(...) functions.

Step 3: Call pereodically MTouchAcquisition() acquisition function.

Step 4: Get buttons states using MTouchGetSensorState(...) function.

*******************************************************************************
mTouch Capacitive Touch Library Demonstration for Low Power With Threshold Detect Demo
*******************************************************************************
The Low Power demo is used to demonstrate the power saving technique
using scanning of the capsense buttons by ADC hardware.
This will greatly help in reducing the power consumed when the device is Idle.
PIC24FJXXXGA3XX devices have a hardware implementation of mTouch functionality.
In sleep CTMU can charge sensor and ADC can get samples. If the sample value is
less than a threshold set in the hardware the PIC wakes up.

For additional inforamtion about this feature please read:
PIC24F Family Ref. Manual, "Section 51. 12-Bit A/D Converter with Threshold Detect"
and
PIC24F Family Ref. Manual, "Section 53. Charge Time Measurement Unit (CTMU) with Threshold Detect" 

There are 2 different modes that is implemented in the Low power logic.

ACTIVE Mode: This mode will execute the application logic.

SLEEP  Mode: In this mode, the device will be placed in Sleep for a duration which
             is determined by the WDT timeout.

The steps involved in the implementation of the logic is given below:

1). The Low Power Demo will initially scan all the buttons for any touch condition
    for a fixed period of time - timeout.
2). If any of the button is pressed, then the timeout counter will be reset and
    the application will stay in ACTIVE Mode.
3). If none of the buttons are pressed during this period, the device will be placed
    in SLEEP mode and LED D16 will be switched on.
4). If the hardware senses any button press during the SLEEP Mode, the logic will shift
    to the ACTIVE mode and LED D16 will be switched off.

*******************************************************************************
                                   Help File
*******************************************************************************

For more information on mTouch Software Library, refer "mTouchCap Library Help" file.
