/********************************************************************
 FileName:      HardwareProfile - PIC32MX460F512L PIM.h
 Dependencies:  See INCLUDES section
 Processor:     dsPIC33E USB Microcontrollers
 Hardware:      dsPIC33E USB Starter Kit
 Compiler:      Microchip C30 (for dsPIC33E)
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

********************************************************************/

#ifndef HARDWARE_PROFILE_DSPIC33E_USB_STARTER_KIT
#define HARDWARE_PROFILE_DSPIC33E_USB_STARTER_KIT

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
    #define DEMO_BOARD DSPIC33E_USB_STARTER_KIT
    
    #define GetSystemClock()        60000000UL
    #define GetPeripheralClock()    60000000UL

    #define DEMO_BOARD_NAME_STRING "dsPIC33E USB Starter Kit"
    
    /** LED ************************************************************/
    #define InitAllLEDs()      LATD &= 0xFFF8; TRISD &= 0xFFF8;
    
    #define LED0_On()           LATDbits.LATD0 = 1;
    #define LED1_On()           LATDbits.LATD1 = 1;
    #define LED2_On()           LATDbits.LATD2 = 1;
    #define LED3_On()           
    #define LED4_On()           
    #define LED5_On()           
    #define LED6_On()           
    #define LED7_On()           
     
    #define LED0_Off()          LATDbits.LATD0 = 0;
    #define LED1_Off()          LATDbits.LATD1 = 0;
    #define LED2_Off()          LATDbits.LATD2 = 0;
    #define LED3_Off()          
    #define LED4_Off()          
    #define LED5_Off()          
    #define LED6_Off()          
    #define LED7_Off()          
    
    /** SWITCH *********************************************************/
    #define InitAllSwitches()  TRISDbits.TRISD6=1; TRISDbits.TRISD7=1; TRISDbits.TRISD13=1; ANSELDbits.ANSD7 = 0; ANSELDbits.ANSD6 = 0;

    #define Switch1Pressed()    ((PORTDbits.RD6  == 0)? TRUE : FALSE)
    #define Switch2Pressed()    ((PORTDbits.RD7  == 0)? TRUE : FALSE)
    #define Switch3Pressed()    ((PORTDbits.RD13  == 0)? TRUE : FALSE)
    #define Switch4Pressed()    (FALSE)

    /** POT ************************************************************/
    #define mInitPOT() {}

    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0
    
    /** Debug print interface ******************************************/
    #define DEBUG_Init(a)
    #define DEBUG_Error(a)          
    #define DEBUG_PrintString(a)    
    #define DEBUG_PrintHex(a)

#endif  
