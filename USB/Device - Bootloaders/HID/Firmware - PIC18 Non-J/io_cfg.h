/*********************************************************************
 *
 *                Microchip USB C18 Firmware 
 *
 *********************************************************************
 * FileName:        io_cfg.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 3.45+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") for its PIC(R) Microcontroller is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PIC Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * File Version  Date		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 1.0			 04/09/2008	Started from MCHPFSUSB v1.3 HID Mouse
 *							demo project.  Commented out items that
 *							are not particularly useful for the
 *							bootloader.
 ********************************************************************/

#ifndef IO_CFG_H
#define IO_CFG_H

/** I N C L U D E S *************************************************/
#include "usbcfg.h"

/** T R I S *********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

#if defined(PIC18F4550_PICDEM_FS_USB)   //Based on PIC18F4550, for PIC18F45K50 based board, see farther below
    /** U S B ***********************************************************/
    #define tris_usb_bus_sense  TRISAbits.TRISA1    // Input
    
    #if defined(USE_USB_BUS_SENSE_IO)
        #define usb_bus_sense       PORTAbits.RA1
    #else
        #define usb_bus_sense       1
    #endif
    
    #define tris_self_power     TRISAbits.TRISA2    // Input
    
    #if defined(USE_SELF_POWER_SENSE_IO)
        #define self_power          PORTAbits.RA2
    #else
        #define self_power          0
    #endif
    
    /** L E D ***********************************************************/
    #define mInitAllLEDs()      LATD &= 0xFE; TRISD &= 0xFE;
    #define mLED_1              LATDbits.LATD0
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    
    /** S W I T C H *****************************************************/
    #define mInitAllSwitches()  {mInitSwitch2();}
    #define mInitSwitch2()      {ADCON1 = 0x0F;}
    #define sw2                 PORTBbits.RB4
    #define mDeInitSwitch2()    {ADCON1 = 0x07;}
    /********************************************************************/

#elif defined(PIC18F4550_PICDEM_FS_USB_K50)  //Section for PIC18F45K50 when on the PICDEM FS USB Demo Board
    /** U S B ***********************************************************/
    #define tris_usb_bus_sense  TRISAbits.TRISA1    // Input

    #if defined(USE_USB_BUS_SENSE_IO)
        #define usb_bus_sense       PORTAbits.RA1
    #else
        #define usb_bus_sense       1
    #endif

    #define tris_self_power     TRISAbits.TRISA2    // Input

    #if defined(USE_SELF_POWER_SENSE_IO)
        #define self_power          PORTAbits.RA2
    #else
        #define self_power          0
    #endif

    /** L E D ***********************************************************/
    #define mInitAllLEDs()      LATD &= 0xFE; TRISD &= 0xFE;
    #define mLED_1              LATDbits.LATD0
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;

    /** S W I T C H *****************************************************/
    #define mInitAllSwitches()  TRISBbits.TRISB4=1;
    #define mInitSwitch2()      {ANSELBbits.ANSB4 = 0;}
    #define sw2                 PORTBbits.RB4
    #define mDeInitSwitch2()    {ANSELBbits.ANSB4 = 1;}
    /********************************************************************/

#elif defined(LOW_PIN_COUNT_USB_DEVELOPMENT_KIT)
    #define tris_usb_bus_sense  TRISAbits.TRISA1    // Input

	#if defined(USE_USB_BUS_SENSE_IO)
	    #define usb_bus_sense       PORTAbits.RA1
	#else
	    #define usb_bus_sense       1
	#endif
	
	#define tris_self_power     TRISAbits.TRISA2    // Input
	
	#if defined(USE_SELF_POWER_SENSE_IO)
		#define self_power          PORTAbits.RA2
	#else
		#define self_power          0
	#endif
    
    /** LED ************************************************************/
    #define mInitAllLEDs()      LATC &= 0xFE; TRISC &= 0xFE;
    #define mLED_1              LATCbits.LATC0
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    
    /** SWITCH *********************************************************/
    #define mInitAllSwitches()  mInitSwitch2();
    #define mInitSwitch2()      {ADCON1 = 0x0F;}
    #define sw2                 PORTAbits.RA3
    #define mDeInitSwitch2()    {ADCON1 = 0x07;}



#else
    #error Not a supported board (yet), add I/O pin mapping in __FILE__, line __LINE__
#endif


//Special register re-definitions, for accessing the USBIF and USBIE interrupt 
//related bits.  These bits are located in PIR3/PIE3 on PIC18F45K50 Family devices,
//but this fimware expects them to be in PIR2/PIE2 instead, like on previous devices.
#if defined(PIC18F4550_PICDEM_FS_USB_K50)
    #define PIR2bits  PIR3bits
    #define PIE2bits  PIE3bits
#endif


#endif //IO_CFG_H
