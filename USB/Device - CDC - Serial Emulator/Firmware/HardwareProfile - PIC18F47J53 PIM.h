/********************************************************************
 FileName:     	HardwareProfile - PIC18F47J53 PIM.h
 Dependencies:	See INCLUDES section
 Processor:	PIC18 USB Microcontrollers
 Hardware:	PIC18F47J53 PIM
 Compiler:  	Microchip C18
 Company:	Microchip Technology, Inc.

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
  Rev   Description
  ---   ----------------------------------------------
  2.6A  Initial release
********************************************************************/

#ifndef HARDWARE_PROFILE_PIC18F47J53_PIM_H
#define HARDWARE_PROFILE_PIC18F47J53_PIM_H

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
    #define tris_self_power     TRISCbits.TRISC2    // Input
    #define self_power          1

    //#define USE_USB_BUS_SENSE_IO
    #define tris_usb_bus_sense  TRISCbits.TRISC2    // Input
    #define USB_BUS_SENSE       1 

    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/
    /******** Application specific definitions *************************/
    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/

    #define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER

    /** Board definition ***********************************************/
    //These defintions will tell the main() function which board is
    //  currently selected.  This will allow the application to add
    //  the correct configuration bits as wells use the correct
    //  initialization functions for the board.  These defitions are only
    //  required in the stack provided demos.  They are not required in
    //  final application design.
    #define DEMO_BOARD PIC18F47J53_PIM
    #define PIC18F47J53_PIM
    #define CLOCK_FREQ 48000000
    #define GetSystemClock() CLOCK_FREQ   

    /** LED ************************************************************/
    #define mInitAllLEDs()      LATE &= 0xFC; TRISE &= 0xFC;
    
    #define mLED_1              LATEbits.LATE0
    #define mLED_2              LATEbits.LATE1
    #define mLED_3              
    #define mLED_4              
    
    #define mGetLED_1()         mLED_1
    #define mGetLED_2()         mLED_2
    #define mGetLED_3()         1
    #define mGetLED_4()         1

    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         
    #define mLED_4_On()        
    
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        
    #define mLED_4_Off()        
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     
    #define mLED_4_Toggle()     
    
    /** SWITCH *********************************************************/
    #define mInitSwitch2()      TRISBbits.TRISB2=1;
    #define mInitSwitch3()      mInitSwitch2();
    #define mInitAllSwitches()  mInitSwitch2();
    #define sw2                 PORTBbits.RB2
    #define sw3                 PORTBbits.RB2

    /** RS 232 lines ****************************************************/
    #define UART_TRISTx   TRISCbits.TRISC6
    #define UART_TRISRx   TRISCbits.TRISC7
    #define UART_Tx       PORTCbits.RC6
    #define UART_Rx       PORTCbits.RC7
    #define UART_ENABLE	  RCSTAbits.SPEN

    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0

    //These definitions are only relevant if the respective functions are enabled
    //in the usb_config.h file.
    //Make sure these definitions match the GPIO pins being used for your hardware
    //setup.
    #define UART_DTS PORTBbits.RB0
    #define UART_DTR LATDbits.LATD7
    #define UART_RTS LATBbits.LATB2
    #define UART_CTS PORTBbits.RB1
    
    #define mInitRTSPin() {TRISBbits.TRISB2 = 0;}   //Configure RTS as a digital output.  
    #define mInitCTSPin() {TRISBbits.TRISB1 = 1;}   //Configure CTS as a digital input.  (Make sure pin is digital if ANxx functions is present on the pin)
    #define mInitDTSPin() {TRISBbits.TRISB0 = 1;}   //Configure DTS as a digital input.  (Make sure pin is digital if ANxx functions is present on the pin)
    #define mInitDTRPin() {TRISDbits.TRISD7 = 0;}   //Configure DTR as a digital output.
#endif  //HARDWARE_PROFILE_PIC18F47J53_PIM_H
