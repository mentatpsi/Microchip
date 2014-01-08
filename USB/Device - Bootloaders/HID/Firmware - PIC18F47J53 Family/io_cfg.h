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



#if defined(PIC18F47J53_PIM)
	/** U S B ***********************************************************/
	// Bus sense pin is RB5 on PIC18F46J50 FS USB Plug-In Module.
	// Must put jumper JP1 in R-U position
	#if defined(USE_USB_BUS_SENSE_IO)
    	#define tris_usb_bus_sense  TRISCbits.TRISC2    // Input
		#define usb_bus_sense       PORTCbits.RC2
	#else
		#define usb_bus_sense       1
	#endif
	#define self_power          0

    /** LED ************************************************************/
	#define mInitAllLEDs()      LATE &= 0xFD; TRISE &= 0xFD;
	#define mLED_1              LATEbits.LATE1
	#define mLED_1_On()         mLED_1 = 1;
	#define mLED_1_Off()        mLED_1 = 0;
	#define mLED_1_Toggle()     mLED_1 = !mLED_1;
    /** SWITCH *********************************************************/
	#define mInitAllSwitches()  {mInitSwitch2();}
	#define mInitSwitch2()   	{ANCON1bits.PCFG8 = 1;}   
	#define sw2                 PORTBbits.RB2
	#define mDeInitSwitch2()    {ANCON1bits.PCFG8 = 0;} 

/********************************************************************/

//Uncomment below if using the YOUR_BOARD hardware platform
//#elif defined(YOUR_BOARD)
//Add your hardware specific I/O pin mapping here

#else
    #error Not a supported board (yet), add I/O pin mapping in __FILE__, line __LINE__
#endif

#endif //IO_CFG_H
