/********************************************************************
 FileName:     	HardwareProfile - PIC24FJ64GB004 PIM.h
 Dependencies:  See INCLUDES section
 Processor:     PIC24FJ64GB004
 Hardware:      PIC24FJ64GB004 PIM
 Compiler:      Microchip C30
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
  2.3   09/15/2008   Broke out each hardware platform into its own
                     "HardwareProfile - xxx.h" file
  2.4b  04/08/2009   Initial support for PIC24FJ64GB004 family
********************************************************************/

#ifndef HARDWARE_PROFILE_PIC24FJ64GB004_PIM_H
#define HARDWARE_PROFILE_PIC24FJ64GB004_PIM_H

    /*******************************************************************/
    /******** USB stack hardware selection options *********************/
    /*******************************************************************/
    //This section is the set of definitions required by the MCHPFSUSB
    //  framework.  These definitions tell the firmware what mode it is
    //  running in, and where it can find the results to some information
    //  that the stack needs.
    //These definitions are required by every application developed with
    //  this revision of the MCHPFSUSB framework.  Please review each
    //  option carefully and determine which options are desired/required
    //  for your application.

    //#define USE_SELF_POWER_SENSE_IO
    #define tris_self_power     TRISAbits.TRISA2    // Input
    #define self_power          1

    //#define USE_USB_BUS_SENSE_IO
    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
    #define USB_BUS_SENSE       1 
   
    //Uncomment this to make the output HEX of this project 
    //   to be able to be bootloaded using the HID bootloader
    #define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER	

    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/
    /******** Application specific definitions *************************/
    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/

    /** Board definition ***********************************************/
    //These defintions will tell the main() function which board is
    //  currently selected.  This will allow the application to add
    //  the correct configuration bits as wells use the correct
    //  initialization functions for the board.  These defitions are only
    //  required in the stack provided demos.  They are not required in
    //  final application design.
    #define DEMO_BOARD YOUR_DEMO_BOARD
    #define YOUR_DEMO_BOARD
    #define CLOCK_FREQ 32000000
    
    /** LED ************************************************************/
    #define mInitAllLEDs()      
    
    #define mLED_1              
    #define mLED_2              
    #define mLED_3              
    #define mLED_4              

    #define mGetLED_1()         1
    #define mGetLED_2()         1
    #define mGetLED_3()         1
    #define mGetLED_4()         1
    
    #define mLED_1_On()         
    #define mLED_2_On()         
    #define mLED_3_On()         
    #define mLED_4_On()        
    
    #define mLED_1_Off()        
    #define mLED_2_Off()        
    #define mLED_3_Off()        
    #define mLED_4_Off()        
    
    #define mLED_1_Toggle()     
    #define mLED_2_Toggle()     
    #define mLED_3_Toggle()     
    #define mLED_4_Toggle() 

    #define mSetLED_1(in)         
    #define mSetLED_2(in)         
    #define mSetLED_3(in)         
    #define mSetLED_4(in)             
    
    /** SWITCH *********************************************************/
    #define mInitSwitch2()      
    #define mInitSwitch3()      
    #define mInitAllSwitches()  mInitSwitch2();mInitSwitch3();
    #define sw2                 1
    #define sw3                 1

    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0

#endif  //HARDWARE_PROFILE_PIC24FJ64GB004_PIM_H
