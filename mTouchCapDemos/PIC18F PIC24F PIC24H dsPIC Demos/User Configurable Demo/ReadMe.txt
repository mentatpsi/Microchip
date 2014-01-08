**********************************************************************************
******************** Readme File for User Configurable Demo **********************
**********************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

*********************************************************************************

This file contains the information related to the mTouch Capacitive Touch Software
Library used for User Configurable Demo. 

**********************************************************************************
                             Release Information
**********************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16

**********************************************************************************
   mTouch Capacitive Touch Library Set-up Details for User Configurable Demo
**********************************************************************************
This demo can be used as a start point for the custom application.
All PIC18F and PIC24F devices are supported.
Hardware depended information such as PIC configuration bits and channels mapping
is located in ...\Configurations\User_Board folder. Also device specific ADC and CTMU
types selection is done in mTouchConfig.h file.
All analog channels information is predefined in system.h file.
Refer to the PIC datasheet for the available analog channels for the specific PIC device.

1)  User Configurable project can be configured as per user's application by
    selecting required PIC device and modifying header file "mTouchConfig.h".
  
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

**********************************************************************************
                                   Help File
**********************************************************************************

For more information about mTouch Software Library, refer "mTouch Cap Library Help.chm"
file located in "...\Microchip\Help" folder.