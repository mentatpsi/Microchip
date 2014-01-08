/********************************************************************
 FileName:     reports.h
 Dependencies: See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18), C30 (for PIC24), C32 (for PIC32)
 Company:		Microchip Technology, Inc.

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
  2.5   4/1/2009     Initial release
********************************************************************/

#ifndef REPORTS_H
#define REPORTS_H

typedef enum HIDReportID
{
    //Power Summary Section
        //ID requests
        POWER_SUMMARY_POWER_SUMMARY_ID          = 0x01, 

        //String requests
        POWER_SUMMARY_I_NAME                    = 0x10, 
        POWER_SUMMARY_I_PRODUCT                 = 0x11, 
        POWER_SUMMARY_I_SERIAL_NUMBER           = 0x12, 
        POWER_SUMMARY_I_DEVICE_CHEMISTRY        = 0x13, 
        POWER_SUMMARY_I_OEM_INFORMATION         = 0x14, 
        POWER_SUMMARY_I_MANUFACTURER_NAME       = 0x15, 
        POWER_SUMMARY_I_MANUFACTURER            = 0x16, 
    
        //Voltage Information
        POWER_SUMMARY_VOLTAGE_INFO              = 0x20,

        //Current Information
        POWER_SUMMARY_CURRENT_INFO              = 0x30,

        //Status Information
        POWER_SUMMARY_STATUS_INFO               = 0x40,

        //Capacity Information
        POWER_SUMMARY_CAPACITY_MODE             = 0x50,
        POWER_SUMMARY_DESIGN_CAPACITY           = 0x51,
        POWER_SUMMARY_REMAINING_CAPACITY        = 0x52,
        POWER_SUMMARY_FULL_CHARGE_CAPACITY      = 0x53,
        POWER_SUMMARY_REMAINING_CAPACITY_LIMIT  = 0x54,
        POWER_SUMMARY_WARNING_CAPACITY_LIMIT    = 0x55,
        POWER_SUMMARY_CAPACITY_GRANULARITY_1    = 0x56,
        POWER_SUMMARY_CAPACITY_GRANULARITY_2    = 0x57,

        //Time Information
        POWER_SUMMARY_RUN_TIME_TO_EMPTY         = 0x60,

    NO_MORE_REPORTS = 0xFF
} HID_REPORT_ID;

#endif
