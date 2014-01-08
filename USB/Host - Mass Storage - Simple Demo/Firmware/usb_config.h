/*
********************************************************************************
                                                                                
Software License Agreement                                                      
                                                                                
Copyright � 2007-2008 Microchip Technology Inc.  All rights reserved.           
                                                                                
Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital signal controller  
that is integrated into your product or third party product (pursuant to the    
sublicense terms in the accompanying license agreement).                        
                                                                                
You should refer to the license agreement accompanying this Software for        
additional information regarding your rights and obligations.                   
                                                                                
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,   
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF        
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.  
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER       
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR    
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES         
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR     
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF        
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES          
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.     
                                                                                
********************************************************************************
*/

// Created by the Microchip USBConfig Utility, Version 2.0.0.0, 11/18/2008, 8:08:56

#ifndef _usb_config_h_
#define _usb_config_h_

#if defined(__PIC24F__)
    #include <p24Fxxxx.h>
#elif defined(__18CXX)
    #include <p18cxxx.h>
#elif defined(__PIC32MX__)
    #include <p32xxxx.h>
    #include "plib.h"
#elif defined (__dsPIC33EP512MU810__)
    #include <p33Exxxx.h>
#elif defined (__PIC24EP512GU810__)
    #include <p24Exxxx.h>
#else

    #error No processor header file.
#endif

#define _USB_CONFIG_VERSION_MAJOR 2
#define _USB_CONFIG_VERSION_MINOR 0
#define _USB_CONFIG_VERSION_DOT   0
#define _USB_CONFIG_VERSION_BUILD 0

// Supported USB Configurations

#define USB_SUPPORT_HOST

// Hardware Configuration

//USB_PING_PONG__FULL_PING_PONG
#define USB_PING_PONG_MODE  USB_PING_PONG__FULL_PING_PONG 

// Host Configuration

#define NUM_TPL_ENTRIES 2
#define USB_NUM_CONTROL_NAKS 200
#define USB_SUPPORT_INTERRUPT_TRANSFERS
#define USB_NUM_INTERRUPT_NAKS 3
#define USB_SUPPORT_BULK_TRANSFERS
#define USB_NUM_BULK_NAKS 20000
//#define USB_SUPPORT_ISOCHRONOUS_TRANSFERS
#define USB_INITIAL_VBUS_CURRENT (100/2)
#define USB_INSERT_TIME (250+1)
#define USB_HOST_APP_EVENT_HANDLER USB_ApplicationEventHandler

// Host Mass Storage Client Driver Configuration

//#define USB_ENABLE_TRANSFER_EVENT

#define USB_MAX_MASS_STORAGE_DEVICES 1

// Helpful Macros

#define USBTasks()                  \
    {                               \
        USBHostTasks();             \
        USBHostMSDTasks();          \
    }

#define USBInitialize(x)            \
    {                               \
        USBHostInit(x);             \
    }


#endif

