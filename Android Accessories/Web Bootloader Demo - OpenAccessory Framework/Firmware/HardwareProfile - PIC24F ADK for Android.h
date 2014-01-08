/********************************************************************
 FileName:     	HardwareProfile - PIC24F ADK for Android.h
 Dependencies:  See INCLUDES section
 Processor:     PIC24FJ256GB110
 Hardware:      PIC24F ADK for Android
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

#ifndef HARDWARE_PROFILE_PIC24F_ADK_FOR_ANDROID_H
#define HARDWARE_PROFILE_PIC24F_ADK_FOR_ANDROID_H  

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
    #define DEMO_BOARD PIC24F_ADK_FOR_ANDROID
    
    // Various clock values
    #define GetSystemClock()            32000000UL
    #define GetPeripheralClock()        (GetSystemClock())
    #define GetInstructionClock()       (GetSystemClock() / 2)

    #define DEMO_BOARD_NAME_STRING "Accessory Development Starter Kit (PIC24F) for Android(tm)"
    
    /** LED ************************************************************/
    #define mInitAllLEDs()  {TRISE = 0; LATE = 0;}
    
    #define mGetLED_0()              (LATEbits.LATE0)
    #define mGetLED_1()              (LATEbits.LATE1)
    #define mGetLED_2()              (LATEbits.LATE2)
    #define mGetLED_3()              (LATEbits.LATE3)
    #define mGetLED_4()              (LATEbits.LATE4)
    #define mGetLED_5()              (LATEbits.LATE5)
    #define mGetLED_6()              (LATEbits.LATE6)
    #define mGetLED_7()              (LATEbits.LATE7)
    
    #define mLED_0_On()         LATEbits.LATE0 = 1;
    #define mLED_1_On()         LATEbits.LATE1 = 1;
    #define mLED_2_On()         LATEbits.LATE2 = 1;
    #define mLED_3_On()         LATEbits.LATE3 = 1;
    #define mLED_4_On()         LATEbits.LATE4 = 1;
    #define mLED_5_On()         LATEbits.LATE5 = 1;
    #define mLED_6_On()         LATEbits.LATE6 = 1;
    #define mLED_7_On()         LATEbits.LATE7 = 1;
    
    #define mLED_0_Off()        LATEbits.LATE0 = 0;
    #define mLED_1_Off()        LATEbits.LATE1 = 0;
    #define mLED_2_Off()        LATEbits.LATE2 = 0;
    #define mLED_3_Off()        LATEbits.LATE3 = 0;
    #define mLED_4_Off()        LATEbits.LATE5 = 0;
    #define mLED_5_Off()        LATEbits.LATE4 = 0;
    #define mLED_6_Off()        LATEbits.LATE6 = 0;
    #define mLED_7_Off()        LATEbits.LATE7 = 0;
	
	#define mLED_0_Toggle()		LATEbits.LATE0 ^= 1;
	#define mLED_1_Toggle()		LATEbits.LATE1 ^= 1;
	#define mLED_2_Toggle()		LATEbits.LATE2 ^= 1;
	#define mLED_3_Toggle()		LATEbits.LATE3 ^= 1;
	#define mLED_4_Toggle()		LATEbits.LATE4 ^= 1;
	#define mLED_5_Toggle()		LATEbits.LATE5 ^= 1;
	#define mLED_6_Toggle()		LATEbits.LATE6 ^= 1;
	#define mLED_7_Toggle()		LATEbits.LATE7 ^= 1;
    
    /** SWITCH *********************************************************/
    #define mInitAllSwitches()  mInitSwitch1();mInitSwitch2();mInitSwitch3();mInitSwitch4();
    
    #define mInitSwitch1()		TRISAbits.TRISA1 = 1;
	#define mInitSwitch2()		{TRISBbits.TRISB11 = 1; AD1PCFGLbits.PCFG11 = 1;}
	#define mInitSwitch3()		TRISFbits.TRISF12 = 1;
	#define mInitSwitch4()		TRISFbits.TRISF13 = 1;
	
    #define sw1    				~(PORTFbits.RF13)
    #define sw2    				~(PORTFbits.RF12)
    #define sw3    				~(PORTBbits.RB11)
    #define sw4    				~(PORTAbits.RA1)

    /** POT ************************************************************/
    #define mInitPOT()  {AD1PCFGLbits.PCFG9 = 0;    AD1CON2bits.VCFG = 0x0;    AD1CON3bits.ADCS = 0xFF;    AD1CON1bits.SSRC = 0x0;    AD1CON3bits.SAMC = 0b10000;    AD1CON1bits.FORM = 0b00;    AD1CON2bits.SMPI = 0x0;    AD1CON1bits.ADON = 1;}

    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0
#endif  //HARDWARE_PROFILE_PIC24F_ADK_FOR_ANDROID_H
