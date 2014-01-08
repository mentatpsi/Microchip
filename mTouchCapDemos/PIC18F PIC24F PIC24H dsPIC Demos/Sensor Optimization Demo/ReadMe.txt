**********************************************************************************
******************** Readme File for Sensor Optimization Demo ********************
**********************************************************************************

Copyright (c) Microchip Technology Inc.  All rights reserved.

*********************************************************************************

This file contains the information related to the mTouch Capacitive Touch Software
Library used for Sensor Optimization Demo. 

**********************************************************************************
                             Release Information
**********************************************************************************
mTouch Cap Library v1.43:         February-2013
MPLAB version:                    8.83 or Later
MPLAB-X Version:                  1.00 or Later 
Compiler Version:                 C18 3.40 or Later, HI-TECH 9.66 or Later, XC8, C30 3.31 or Later, XC16

**********************************************************************************
        mTouch Capacitive Touch Library Testing Details for Basic Button Demo
**********************************************************************************
The mTouch Capacitive Touch Library software is tested using Evaluation
boards DM183026-2. 8 buttons plugin module must be connected to the low part of the
sensors connector (channels from 0 to 7).

**********************************************************************************
   mTouch Capacitive Touch Library Demonstration for Sensor Optimization Demo
**********************************************************************************

The purpose of this demo is to show how to optimize the sensor parameters to
provide the best noise immunity. The program calculates optimal values for
CTMU current, CTMU charge delay and press detection threshold. 
Refer to "Sensor Optimization (Debug Module)" chapter in the mTouch library 
documentation to get more details.
 
**********************************************************************************
                                   Help File
**********************************************************************************

For more information about mTouch Software Library, refer "mTouch Cap Library Help.chm"
file located in "...\Microchip\Help" folder.