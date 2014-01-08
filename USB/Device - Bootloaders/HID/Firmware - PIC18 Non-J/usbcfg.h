/*********************************************************************
 * FileName:        usbcfg.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 3.45+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") for its PICmicro(R) Microcontroller is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
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
 ********************************************************************/

#ifndef USBCFG_H
#define USBCFG_H

//------------------------------------------------------------------------------
//Configurable Options
//------------------------------------------------------------------------------
#define ENABLE_IO_PIN_CHECK_BOOTLOADER_ENTRY  //Uncomment if you wish to enable I/O pin entry method into bootloader mode
                                              //Make sure proper sw2() macro definition is provided in io_cfg.h                                
//------------------------------------------------------------------------------
//If you don't uncomment the above, the only entry method into bootloader mode,
//is by software, ex:
//1.  Booting up initially into application run mode.
//2.  From the application run mode, execute a "goto 0x001C" instruction
//    in order to jump into bootloader mode.
//------------------------------------------------------------------------------

//Option to allow blinking of LEDs to show USB bus state.  May be optionally
//commented out to save code space.
#define ENABLE_USB_LED_BLINK_STATUS     //Comment out to save code space

//USB VBUS sensing and USB Bus/Self power sensing options.
//---------------------------------------------------------
//#define USE_SELF_POWER_SENSE_IO   //Leave commented if device is bus powered only (or self powered only, uncomment for some types of dual powered devices)
//#define USE_USB_BUS_SENSE_IO      //If the device is self powered, this needs to uncommented if making a fully compliant USB design




//------------------------------------------------------------------------------
//Usually constants, no modification typically needed
//------------------------------------------------------------------------------
#define MAX_NUM_INT             1   // For tracking Alternate Setting
#define EP0_BUFF_SIZE           8   // Valid Options: 8, 16, 32, or 64 bytes.
									// There is little advantage in using 
									// more than 8 bytes on EP0 IN/OUT in most cases.

/* Parameter definitions are defined in usbdrv.h */
#define MODE_PP                 _PPBM0
#define UCFG_VAL                _PUEN|_TRINT|_FS|MODE_PP


/* Make sure the proper hardware platform is being used*/
#if defined(__18F4550) || defined(__18F4455) || defined(__18F4450) || defined(__18F2550) || defined(__18F2455) || defined(__18F2450) || defined(__18F2458) || defined(__18F2553) || defined(__18F4458) || defined(__18F4553)
	#define PIC18F4550_PICDEM_FS_USB
#elif defined(__18F45K50) || defined(__18LF45K50) || defined(__18F25K50) || defined(__18LF25K50) || defined(__18F24K50) || defined(__18LF24K50)
	#define PIC18F4550_PICDEM_FS_USB_K50
#elif defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50)
	#define LOW_PIN_COUNT_USB_DEVELOPMENT_KIT
#else
    #error Unsupported processor type for this firmware.  See other firmware projects.
#endif




/** D E V I C E  C L A S S  U S A G E *******************************/
#define USB_USE_HID

/*
 * MUID = Microchip USB Class ID
 * Used to identify which of the USB classes owns the current
 * session of control transfer over EP0
 */
#define MUID_NULL               0
#define MUID_USB9               1
#define MUID_HID                2
#define MUID_CDC                3

/** E N D P O I N T S  A L L O C A T I O N **************************/
/*
 * See usbmmap.c for an explanation of how the endpoint allocation works
 */

/* HID */
#define HID_INTF_ID             0x00
#define HID_UEP                 UEP1
#define HID_BD_OUT              ep1Bo
#define HID_INT_OUT_EP_SIZE     64
#define HID_BD_IN               ep1Bi
#define HID_INT_IN_EP_SIZE      64
#define HID_NUM_OF_DSC          1		//Just the Report descriptor (no physical descriptor present)
#define HID_RPT01_SIZE          29

/* HID macros */
#define mUSBGetHIDDscAdr(ptr)               \
{                                           \
    if(usb_active_cfg == 1)                 \
        ptr = (rom byte*)&cfg01.hid_i00a00; \
}

#define mUSBGetHIDRptDscAdr(ptr)            \
{                                           \
    if(usb_active_cfg == 1)                 \
        ptr = (rom byte*)&hid_rpt01;        \
}

#define mUSBGetHIDRptDscSize(count)         \
{                                           \
    if(usb_active_cfg == 1)                 \
        count = sizeof(hid_rpt01);          \
}

#define MAX_EP_NUMBER           1           // UEP1

#endif //USBCFG_H
