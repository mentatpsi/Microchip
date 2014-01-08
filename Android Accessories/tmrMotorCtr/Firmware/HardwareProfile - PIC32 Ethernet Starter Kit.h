/********************************************************************
 FileName:      HardwareProfile - PIC32 Ethernet Starter Kit.h
 Dependencies:  See INCLUDES section
 Processor:     PIC32 USB Microcontrollers
 Hardware:      PIC32 Ethernet Starter Kit
 Compiler:      Microchip C32 (for PIC32)
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
  Rev   Description
  2.9   Initial Release
********************************************************************/

#ifndef HARDWARE_PROFILE_PIC32_ETHERNET_STARTER_KIT_H
#define HARDWARE_PROFILE_PIC32_ETHERNET_STARTER_KIT_H

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
    #define DEMO_BOARD PIC32_ETHERNET_STARTER_KIT


    //#define RUN_AT_48MHZ
    //#define RUN_AT_24MHZ
    #define RUN_AT_60MHZ
    
    // Various clock values
    #if defined(RUN_AT_48MHZ)
        #define GetSystemClock()            48000000UL
        #define GetPeripheralClock()        48000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_24MHZ)
        #define GetSystemClock()            24000000UL
        #define GetPeripheralClock()        24000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_60MHZ)    
        #define GetSystemClock()            60000000UL
        #define GetPeripheralClock()        60000000UL  // Will be divided down
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #else
        #error Choose a speed
    #endif  

    #define DEMO_BOARD_NAME_STRING "PIC32 Ethernet Starter Kit"

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
    #define InitAllSwitches()  TRISDbits.TRISD6=1; TRISDbits.TRISD7=1; TRISDbits.TRISD13=1; CNPUEbits.CNPUE19=1; CNPUEbits.CNPUE16=1; CNPUEbits.CNPUE15=1;

    #define Switch1Pressed()    ((PORTDbits.RD6 == 0)? TRUE : FALSE)
    #define Switch2Pressed()    ((PORTDbits.RD7  == 0)? TRUE : FALSE)
    #define Switch3Pressed()    ((PORTDbits.RD13  == 0)? TRUE : FALSE)
    #define Switch4Pressed()    FALSE

    /** POT ************************************************************/
    #define mInitPOT()  {AD1PCFGbits.PCFG5 = 0;    AD1CON2bits.VCFG = 0x0;    AD1CON3bits.ADCS = 0xFF;    AD1CON1bits.SSRC = 0x0;    AD1CON3bits.SAMC = 0x10;    AD1CON1bits.FORM = 0x0;    AD1CON2bits.SMPI = 0x0;    AD1CON1bits.ADON = 1;}

    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0

    /** Debug print interface ******************************************/
    #define DEBUG_Init(a)
    #define DEBUG_Error(a)          
    #define DEBUG_PrintString(a)    
    #define DEBUG_PrintHex(a)

#endif  //HARDWARE_PROFILE_PIC32_ETHERNET_STARTER_KIT_H
