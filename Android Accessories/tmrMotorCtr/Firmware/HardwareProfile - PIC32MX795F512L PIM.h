/********************************************************************
 FileName:      HardwareProfile - PIC32MX795F512L PIM.h
 Dependencies:  See INCLUDES section
 Processor:     PIC32 USB Microcontrollers
 Hardware:      PIC32MX460F512L PIM
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
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
  2.3   09/15/2008   Broke out each hardware platform into its own
                     "HardwareProfile - xxx.h" file
********************************************************************/

#ifndef HARDWARE_PROFILE_PIC32MX795F512L_PIM_H
#define HARDWARE_PROFILE_PIC32MX795F512L_PIM_H

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
    #define DEMO_BOARD PIC32MX795F512L_PIM
    #define EXPLORER_16
    #define CLOCK_FREQ 32000000

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

    #define DEMO_BOARD_NAME_STRING "PIC32MX795F512L PIM"

    /** LED ************************************************************/
    #define InitAllLEDs()      LATA &= 0xC0; TRISA &= 0xC0;  
    
    #define LED0_On()           LATAbits.LATA0 = 1;
    #define LED1_On()           LATAbits.LATA1 = 1;
    #define LED2_On()           LATAbits.LATA2 = 1;
    #define LED3_On()           LATAbits.LATA3 = 1;
    #define LED4_On()           LATAbits.LATA4 = 1;
    #define LED5_On()           LATAbits.LATA5 = 1;
    #define LED6_On()           
    #define LED7_On()           
     
    #define LED0_Off()          LATAbits.LATA0 = 0;
    #define LED1_Off()          LATAbits.LATA1 = 0;
    #define LED2_Off()          LATAbits.LATA2 = 0;
    #define LED3_Off()          LATAbits.LATA3 = 0;
    #define LED4_Off()          LATAbits.LATA4 = 0;
    #define LED5_Off()          LATAbits.LATA5 = 0;
    #define LED6_Off()          
    #define LED7_Off()     

    /** SWITCH *********************************************************/
    #define mInitSwitch2()      TRISDbits.TRISD6=1;
    #define mInitSwitch3()      TRISDbits.TRISD7=1;
    #define InitAllSwitches()  mInitSwitch2();mInitSwitch3();

    #define Switch1Pressed()    FALSE
    #define Switch2Pressed()    FALSE
    #define Switch3Pressed()    ((PORTDbits.RD7  == 0)? TRUE : FALSE)
    #define Switch4Pressed()    ((PORTDbits.RD6 == 0)? TRUE : FALSE)

    /** POT ************************************************************/
    #define mInitPOT()  {AD1PCFGbits.PCFG5 = 0;    AD1CON2bits.VCFG = 0x0;    AD1CON3bits.ADCS = 0xFF;    AD1CON1bits.SSRC = 0x0;    AD1CON3bits.SAMC = 0x10;    AD1CON1bits.FORM = 0x0;    AD1CON2bits.SMPI = 0x0;    AD1CON1bits.ADON = 1;}

    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0

    /** debugging UART port ********************************************/
    #define BAUDRATE2       57600UL
    #define BRG_DIV2        4 
    #define BRGH2           1
    
    #if defined(DEBUG_ENABLE)
        #include <uart2.h>
        #define DEBUG_Init(a)           UART2Init()
        // Define a debug printing interface
        #define DEBUG_Error(a)          Nop(); Nop(); Nop();
        #define DEBUG_PrintString(a)    UART2PrintString(a)
        #define DEBUG_PrintHex(a)       UART2PutHex(a)
    #else
        #define DEBUG_Init(a)
        #define DEBUG_Error(a)          
        #define DEBUG_PrintString(a)    
        #define DEBUG_PrintHex(a)       
    #endif


#endif  //HARDWARE_PROFILE_PIC32MX795F512L_PIM_H
