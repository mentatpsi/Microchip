*******************************************************************************
*******************************************************************************
********************** Readme File for DA210 Graphics Demo ********************
*******************************************************************************
*******************************************************************************
Copyright (c) Microchip Technology Inc.  All rights reserved.

This file contains information related to the mTouch Capacitive Touch Software
Library for Cap Touch on the DA210 Demo Board, with integration with the 
Microchip Graphics Library.

**********************************************************************************
                             Release Information
**********************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C30 3.31 or Later, XC16

*******************************************************************************
  mTouch Capacitive Touch Library testing Details for DA210 Graphics Demo
*******************************************************************************
The mTouch Capacitive Touch Library software is tested using the PIC24FJ256DA210
Development Board and Graphics Display Truly 3.2" 240 x 320 Board.  

*******************************************************************************
    mTouch Capacitive Touch Library Set-up Details for DA210 Graphics Demo
*******************************************************************************

Please see the User's Guide for the DA210 Development Board for the correct 
jumper settings.  All settings are the default settings except for jumpers 
JP13, JP14, and JP15.  All three of these must have pins 1 & 2 connected for 
the cap touch pads to operate.  Note that this will disable pushbutton switches
S1, S2, and S3.  The DA210 Demo software requires that the Graphics Display 
Truly 3.2" 240 x 320 Board be connected to Display Connector V1 on the DA210
Development Board.

This Demo is an extension of the Application Note AN1136 code. Please see application 
note AN1136 for details of the Graphics Library implementation in this demo.  
Note that the Touchscreen located on the graphics display is also enabled for
this demo.

*******************************************************************************
                           Cap Touch Demonstration
*******************************************************************************
The purpose of this demo is to interact with the graphics display objects on the
display using either the Capacitive Touch Buttons located on the DA210 Demo Board,
or using the display Touch Screen.

Cap Touch input is done by reading the cap touch buttons, and then creating events
that are fed to the graphics library to change the display state.

Only 4 of 5 keys on the DA210 Demo Board are used; Left, Right, Up and Down.
These buttons are either single cap touch channels or a combination of two cap touch
channels.  These are as follows:
	Up      =       Cap Touch Channel 2
	Down	= 	Cap Touch Channel 3
	Left	=	Cap Touch Channel 1 + 2
	Right	=	Cap Touch Channel 1 + 3

Notice that when the Left or Right cap touch buttons are pressed, an indication LED
lights (LED D1 = Left Key, LED D3 = Right Key).

Each button on the DA210 Demo board has a specific action on the display.

      - The Left button sends a keyboard scan "CARRIAGE RETURN" pressed/release
        message to the "LEFT" button on the screen. This displays pressed
        or released button image and moves the slider on the screen. 

      - The Right button sends a keyboard scan "CARRIAGE RETURN" pressed/release
        message to the "RIGHT" button on the screen. This displays pressed
        or released button image and moves the slider on the screen. 

      - The Up button sends a keyboard scan "RIGHT ARROW" pressed message
        to the slider on the screen. This moves the slider on the display a fixed
        amount to the right and the bar graph is adjusted accordingly.

      - The Down button sends a keyboard scan "LEFT ARROW" pressed message
        to the slider on the screen. This moves the slider on the display a fixed
        amount to the left and the bar graph is adjusted accordingly.

*******************************************************************************
                     Features of the DA210 Graphics Demo
*******************************************************************************
The LEDs indicate when the UP and DOWN buttons are touched. These LEDs are actually 
connected to the cap touch pads.  The connections to the PIC24FJ256DA210 are pins
that can operate as standard I/O pins as well as A/D converter inputs.  When the
pin is not reading the cap touch pad using the CTMU to detect if it is touched, the
pin is switched to an output, and it displays the status of the cap touch pad.
This can be accomplished because the reading of the cap touch pad only takes a short
period. This small amount of time is not perceptible.  It should be noted that 
the LED and current limiting resistor reduce the sensitivity of the cap touch pad
sensors.  This would make this type of implementation not practical if a thicker
overlay was needed to be placed over the cap touch buttons.

The DA210 Graphics Cap Touch Demo is intended to show that a graphics display
may be used along with a touchscreen overlay on the display.  This can help with
applications that need both "hard" defined keys as well as "software" defined
keys (keys created by the display).

*******************************************************************************
                   Limitations of the DA210 Graphics Demo
*******************************************************************************
DA210 Graphics Demo Software is developed on PIC24FJ256DA210 and is not compatible
for other controllers.


*******************************************************************************
                                 Help File
*******************************************************************************
For more information about mTouch Software Library, refer "mTouch Cap Library Help.chm"
file located in "...\Microchip\Help" folder.