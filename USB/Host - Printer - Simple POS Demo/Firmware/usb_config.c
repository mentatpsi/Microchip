/*
********************************************************************************
                                                                                
Software License Agreement                                                      
                                                                                
Copyright © 2007-2008 Microchip Technology Inc. and its licensors.  All         
rights reserved.                                                                
                                                                                
Microchip licenses to you the right to: (1) install Software on a single        
computer and use the Software with Microchip 16-bit microcontrollers and        
16-bit digital signal controllers ("Microchip Product"); and (2) at your        
own discretion and risk, use, modify, copy and distribute the device            
driver files of the Software that are provided to you in Source Code;           
provided that such Device Drivers are only used with Microchip Products         
and that no open source or free software is incorporated into the Device        
Drivers without Microchip's prior written consent in each instance.             
                                                                                
You should refer to the license agreement accompanying this Software for        
additional information regarding your rights and obligations.                   
                                                                                
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY         
KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY              
WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A          
PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE             
LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY,               
CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY           
DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY         
INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR         
LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,                 
SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY         
DEFENSE THEREOF), OR OTHER SIMILAR COSTS.                                       
                                                                                
********************************************************************************
*/

// Created by the Microchip USBConfig Utility, Version 1.0.11.0, 8/26/2008, 12:47:20

#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "USB/usb.h"
#include "USB/usb_host_printer.h"
#include "USB/usb_host_printer_esc_pos.h"

// *****************************************************************************
// USB Embedded Host Printer Client Driver Table of Supported Printer Languages
//
// This table lists the interfaces to all of the supported printer languages.
// The client driver will select the first match, so the preferred language should
// be placed at the beginning of the table.  The last entry must be all NULL.
// *****************************************************************************

USB_PRINTER_INTERFACE usbPrinterClientLanguages[] =
{
    {   // ESC/POS
        USBHostPrinterLanguageESCPOS,
        USBHostPrinterLanguageESCPOSIsSupported
    }
    ,
    {
        NULL,
        NULL 
    }
};

// *****************************************************************************
// USB Embedded Host Printer Client Driver Printer Languages Reference 
// 
// This table is required to provide support for printers that do not support
// the GET_DEVICE_ID printer class request.  The printer language required to
// support these printers must be indicated explicitly by this table, which
// cross references the printer's VID and PID with the entry in the
// usbPrinterClientLanguages[] array above that provides the required support.
// The last entry must be all 0's.
// *****************************************************************************

USB_PRINTER_SPECIFIC_INTERFACE usbPrinterSpecificLanguage[] = 
{ 
    { 0x04B8ul, 0x0202ul, 0, {LANGUAGE_SUPPORT_FLAGS_ESCPOS} }   // Epson TM-T88IV
    ,
    { 0x0000, 0x0000, 0, {0} }
};

// *****************************************************************************
// Client Driver Function Pointer Table for the USB Embedded Host foundation
// *****************************************************************************

CLIENT_DRIVER_TABLE usbClientDrvTable[] =
{                                        
    {
        USBHostPrinterInitialize,
        USBHostPrinterEventHandler,
        0
    }
};

// *****************************************************************************
// USB Embedded Host Targeted Peripheral List (TPL)
// *****************************************************************************

USB_TPL usbTPL[] =
{
    { INIT_VID_PID( 0x04B8ul, 0x0202ul ), 0, 0, {0} } // Epson TM-T88IV
};

