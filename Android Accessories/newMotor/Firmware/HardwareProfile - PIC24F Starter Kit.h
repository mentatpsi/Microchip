/********************************************************************
 FileName:      HardwareProfile - PIC24F Starter Kit.h
 Dependencies:  See INCLUDES section
 Processor:     PIC24FJ256GB106
 Hardware:      PIC24F Starter Kit
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
********************************************************************/

#ifndef HARDWARE_PROFILE_PIC24F_STARTER_KIT_H
#define HARDWARE_PROFILE_PIC24F_STARTER_KIT_H

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
    #define DEMO_BOARD PIC24F_STARTER_KIT
    #define PIC24F_STARTER_KIT
    #define CLOCK_FREQ 32000000
    #define DEMO_BOARD_NAME_STRING "PIC24F Starter Kit 1"
   
    /** LED ************************************************************/
    #define InitAllLEDs()      LATG &= 0xFC3F; TRISG &= 0xFC3F; LATF &= 0xFFCF; TRISF &= 0xFFCF; //G6,7,8,9 and F4,5         
    
    #define LED0_On()         TRISF &= ~0x0030; 
    #define LED1_On()         TRISG &= ~0x0300;
    #define LED2_On()         TRISG &= ~0x00C0;
    #define LED3_On()         
    #define LED4_On()         
    #define LED5_On()         
    #define LED6_On()         
    #define LED7_On()         
    
    #define LED0_Off()        TRISF |= 0x0030;
    #define LED1_Off()        TRISG |= 0x0300;
    #define LED2_Off()        TRISG |= 0x00C0;
    #define LED3_Off()        
    #define LED4_Off()        
    #define LED5_Off()        
    #define LED6_Off()        
    #define LED7_Off()        
    
    /** SWITCH *********************************************************/
    #define mInitSwitch2()      TRISDbits.TRISD6=1;
    #define mInitSwitch3()      TRISDbits.TRISD7=1;
    #define InitAllSwitches()  mInitSwitch2();mInitSwitch3();

    #define Switch1Pressed()    FALSE
    #define Switch2Pressed()    FALSE
    #define Switch3Pressed()    FALSE
    #define Switch4Pressed()    FALSE

    /** POT ************************************************************/
    #define mInitPOT()  {AD1PCFGLbits.PCFG0 = 0;    AD1CON2bits.VCFG = 0x0;    AD1CON3bits.ADCS = 0xFF;    AD1CON1bits.SSRC = 0x0;    AD1CON3bits.SAMC = 0b00001;    AD1CON1bits.FORM = 0b00;    AD1CON2bits.SMPI = 0x0;    AD1CON1bits.ADON = 1;}
    
    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0

    /** Debug print interface ******************************************/
    #define DEBUG_Init(a)
    #define DEBUG_Error(a)          
    #define DEBUG_PrintString(a)    
    #define DEBUG_PrintHex(a)

#endif  //HARDWARE_PROFILE_PIC24F_STARTER_KIT_H
