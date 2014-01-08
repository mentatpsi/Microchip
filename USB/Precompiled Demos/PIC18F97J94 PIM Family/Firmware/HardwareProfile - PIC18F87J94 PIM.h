/********************************************************************
 FileName:     	HardwareProfile - PIC18F97J94 PIM.h
 Dependencies:	See INCLUDES section
 Processor:	    PIC18 USB Microcontrollers
 Hardware:      PIC18F97J94 PIM
 Compiler:  	Microchip C18
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC® Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
********************************************************************/

#ifndef HARDWARE_PROFILE_PIC18F87J94_PIM_H
#define HARDWARE_PROFILE_PIC18F87J94_PIM_H

    //** RTCC definition for PIM Demo
    #include "RTCC.h"
    
    //** LCD definition for PIM Demo
    #include "LCD.h"

    //** LCD communications for the PIC18 Explorer
    #include "LCDBlocking.h"


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

    /** USB ************************************************************/
    //The PIC18F87J94 FS USB Plug-In Module supports the USE_USB_BUS_SENSE_IO
    //feature.  The USE_SELF_POWER_SENSE_IO feature is not implemented on the
    //circuit board, so the USE_SELF_POWER_SENSE_IO define should always be
    //commented for this hardware platform.

    //#define USE_SELF_POWER_SENSE_IO
    #define tris_self_power     TRISAbits.TRISA2    // Input
    #if defined(USE_SELF_POWER_SENSE_IO)
    #define self_power          PORTAbits.RA2
    #else
    #define self_power          1
    #endif

    //#define USE_USB_BUS_SENSE_IO		//JP1 must be in 1-2 position to use this feature on this board	
    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
    #if defined(USE_USB_BUS_SENSE_IO)
    #define USB_BUS_SENSE       PORTBbits.RB5
    #else
    #define USB_BUS_SENSE       1
    #endif


   
    //Uncomment this to make the output HEX of this project 
    //   to be able to be bootloaded using the HID bootloader
	#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER		

    /*******************************************************************/
    /******** MDD File System selection options ************************/
    /*******************************************************************/
    #define USE_PIC18

    #define ERASE_BLOCK_SIZE        512
    #define WRITE_BLOCK_SIZE        64

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
    #define PIC18F97J94_FAMILY
    #define CLOCK_FREQ 64000000
    #define GetInstructionClock()	(CLOCK_FREQ/4)
  

    #define DEMO_BOARD PIC18F87J94_PIM
    #define PIC18F87J94_PIM

    #define mInitAllLEDs()      {LATE &= 0xFC; TRISE &= 0xFC;}
    
    #define mLED_1              LATEbits.LATE0
    #define mLED_2              LATEbits.LATE1
    #define mLED_3              mLED_1
    #define mLED_4              mLED_2
    
    #define mGetLED_1()         mLED_1
    #define mGetLED_2()         mLED_2
    #define mGetLED_3()         mLED_3
    #define mGetLED_4()         mLED_4

    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         mLED_3 = 1;
    #define mLED_4_On()         mLED_4 = 1;
    
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3 = 0;
    #define mLED_4_Off()        mLED_4 = 0;
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3 = !mLED_3;
    #define mLED_4_Toggle()     mLED_4 = !mLED_4;
    
    /** SWITCH *********************************************************/
    #define mInitAllSwitches()  {   TRISBbits.TRISB4    = 1; }
    #define mInitSwitch2()      TRISBbits.TRISB4 = 1;
    #define sw2                 PORTBbits.RB4

    /** POT ************************************************************/
    //Analog potentiometer is located on the PICDEM PIC18 Explorer Demo Board, 
    //which is connected to microcontroller pin RA0, when the PIM is operated
    //with the PIC18 Explorer.
    #define mInitPOT()          {TRISAbits.TRISA0 = 1; \
                                ANCON1bits.ANSEL0 = 1; \
                                ADCON1H = 0x00;                             \
                                ADCON1L = 0x70; /* auto convert mode */     \
                                ADCON2H = 0x08; /* BUFREGEN */              \
                                ADCON2L = 0x00;                             \
                                ADCON3H = 0x0A; /* 10 Tad acquisition */    \
                                ADCON3L = 0x0C; /* ADCS = 13 */             \
                                ADCON5H = 0x00;                             \
                                ADCON5L = 0x00;                             \
                                ADCHS0H = 0x00;                             \
                                ADCHS0L = 0x00;                             \
                                ADCON1Hbits.ADON = 1;}

    /** SPI ************************************************************/
    //The PIC18F87J94 is hosted on the PIC18 Explorer board.  The LCD
    //display on the PIC18 Explorer board is operated via a SPI interface.

    #define SPI_SDI             PORTCbits.RC4               
    #define SDI_TRIS            TRISCbits.TRISC4
    #define SPI_SDO             LATCbits.LATC5               
    #define SDO_TRIS            TRISCbits.TRISC5
    #define SPI_SCK             LATCbits.LATC3               
    #define SCK_TRIS            TRISCbits.TRISC3

    #define PIC18_EXPLORER
    #define SSPIF       SSP1IF
    #define SSPBUF      SSP1BUF
    #define SSPCON1bits SSP1CON1bits
  //#define RFIE            LATGbits.LATG0  // dummy to satisfy LCDBlocking code
    #define LCD_CS          LATAbits.LATA2
    #define LCD_CS_TRIS     TRISAbits.TRISA2
    #define LCD_RESET       LATFbits.LATF6
    #define LCD_RESET_TRIS  TRISFbits.TRISF6
 


    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0
    
#endif  //HARDWARE_PROFILE_PIC18F87J94_PIM_H
