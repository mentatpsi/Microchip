/********************************************************************
 FileName:     	HardwareProfile - PIC18F Starter Kit.h
 Dependencies:  See INCLUDES section
 Processor:     PIC1846J50
 Hardware:      PIC18 Starter Kit
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

********************************************************************/

#ifndef HARDWARE_PROFILE_PIC18F_STARTER_KIT_H
#define HARDWARE_PROFILE_PIC18F_STARTER_KIT_H

#if 0
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
    //#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER	

    //If the application is going to be used with the HID bootloader
    //  then this will provide a function for the application to 
    //  enter the bootloader from the application (optional)
    #if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
        #define EnterBootloader() __asm__("goto 0x400")
    #endif   
#endif   
    /*******************************************************************/
    /******** MDD File System selection options ************************/
    /*******************************************************************/
    #define USE_SD_INTERFACE_WITH_SPI
    #define USE_PIC18F

    #define SD_CS				LATCbits.LATC6
    #define SD_CS_TRIS			TRISCbits.TRISC6
    
    #define SD_CD				PORTAbits.RA0
    #define SD_CD_TRIS			TRISAbits.TRISA0
    
    #define SD_WE				PORTAbits.RA0
    #define SD_WE_TRIS			TRISAbits.TRISA0


    // TRIS pins for the SCK/SDI/SDO lines
    #define SPICLOCK            TRISCbits.TRISC2
    #define SPIIN               TRISDbits.TRISD6
    #define SPIOUT              TRISBbits.TRISB3

    // Latch pins for SCK/SDI/SDO lines
    #define SPICLOCKLAT         LATCbits.LATC2
    #define SPIINLAT            LATDbits.LATD6
    #define SPIOUTLAT           LATBbits.LATB3

    // Port pins for SCK/SDI/SDO lines
    #define SPICLOCKPORT        PORTCbits.RC2
    #define SPIINPORT           PORTDbits.RD6

    // Registers for the SPI module you want to use
    #define SPICON1             SSP2CON1
    #define SPISTAT             SSP2STAT
    #define SPIBUF              SSP2BUF
    #define SPISTAT_RBF         SSP2STATbits.BF
    #define SPICON1bits         SSP2CON1bits
    #define SPISTATbits         SSP2STATbits

    #define SPI_INTERRUPT_FLAG  PIR3bits.SSP2IF 
    #define SPI_INTERRUPT_FLAG_ASM  PIR3, 7    
    #define SPIENABLE           SSP2CON1bits.SSPEN


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
    #define DEMO_BOARD PIC18_STARTER_KIT
    #define PIC18_STARTER_KIT
    #define CLOCK_FREQ 48000000
    #define GetSystemClock() CLOCK_FREQ
    #define GetInstructionClock() GetSystemClock()

   /** MDD File System error checking *********************************/
    // Will generate an error if the clock speed is too low to interface to the card
    #if (GetSystemClock() < 100000)
        #error Clock speed must exceed 100 kHz
    #endif 
#endif  //HARDWARE_PROFILE_PIC18F_STARTER_KIT_H
