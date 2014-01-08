/*********************************************************************
 *                                                                    
 * Software License Agreement                                         
 *                                                                    
 * Copyright © 2007-2009 Microchip Technology Inc.  All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute 
 * Software only when embedded on a Microchip microcontroller or digital 
 * signal controller and used with a Microchip radio frequency transceiver, 
 * which are integrated into your product or third party product (pursuant 
 * to the terms in the accompanying license agreement).   
 *
 * You should refer to the license agreement accompanying this Software for 
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
 * PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
 * LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
 * CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
 * DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
 * ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
 * LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
 * TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
 * NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.             
 *                                                                    
 *********************************************************************/
#ifndef __CONFIG_APP_H_
#define __CONFIG_APP_H_


/*********************************************************************/
// USE_DATA_EEPROM defines how to store RF4CE information in NVM.
// The available options are following:
//	USE_DATA_EEPROM         - Use data EEPROM on the MCU
//  USE_EXTERNAL_EEPROM     - USE external EEPROM via SPI interface
//  USE_PROGRAMMING_SPACE   - Use programming space if enhanced flash
//                            is used in MCU
/*********************************************************************/
#define USE_EXTERNAL_EEPROM
#define USE_25LC256
#define EXPLORER16

/*********************************************************************/
// ENABLE_CONSOLE will enable the print out on the hyper terminal
// this definition is very helpful in the debugging process
/*********************************************************************/
#define ENABLE_CONSOLE


/*********************************************************************/
// HARDWARE_SPI enables the hardware SPI implementation on MCU
// silicon. If HARDWARE_SPI is not defined, digital I/O pins will
// be used to bit-bang the RF transceiver
/*********************************************************************/
#define HARDWARE_SPI

// This definition indicates current device is a RF4CE controller, like a remote control
#define RF4CE_CONTROLLER

// This definition enables security feature in RF4CE stack
#define ENABLE_RF4CE_SECURITY

// This definition enables channel normalization 
//#define ENABLE_RF4CE_CHANNEL_NORM

// This is the definition of vendor identifier. End product manufacture can apply for
// a vendor identifier from ZigBee Alliance.
#define VENDOR_IDENTIFIER                   0xFFF1

// This seven bytes defines an ASCII string to identify the end product manufacture
#define VENDOR_STRING7                      0
#define VENDOR_STRING6                      0
#define VENDOR_STRING5                      0
#define VENDOR_STRING4                      'P'
#define VENDOR_STRING3                      'H'
#define VENDOR_STRING2                      'C'
#define VENDOR_STRING1                      'M'

// This is the definition of total number of supported device types for this RF4CE node
#define NUM_SUPPORTED_DEVICE_TYPES          1

// This is the definition of total number of supported profiles for this RF4CE node
#define NUM_SUPPORTED_PROFILES              1

// This is the definition of supported device types. The number of definition depends on 
// the value of NUM_SUPPORTED_DEVICE_TYPES
#define DEVICE_TYPE_1                       0x01        // remote control       


// This is the definition of supported profiles. The number of definition depends on 
// the value of NUM_SUPPORTED_PROFILES
#define DEVICE_PROFILE_1                    0x01        // ZRC profile
#define DEVICE_PROFILE_2                    0x01
#define DEVICE_PROFILE_3                    0x01
#define DEVICE_PROFILE_4                    0x01
#define DEVICE_PROFILE_5                    0x01
#define DEVICE_PROFILE_6                    0x01
#define DEVICE_PROFILE_7                    0x01

// This is the boolean to indicate if a 15 byte user string has been defined to be transmitted
// during the discovery and pairing procedure
#define USER_STRING_SPECIFIED               0

// This is the optional 15-byte user string to be used in discovery and pairing procedure, these
// definitions will be valid only if USER_STRING_SPECIFIED is not zero.
#define USER_STRING15                       0x00
#define USER_STRING14                       0x00
#define USER_STRING13                       0x00
#define USER_STRING12                       0x00
#define USER_STRING11                       0x00
#define USER_STRING10                       0x00
#define USER_STRING09                       0x00
#define USER_STRING08                       0x00
#define USER_STRING07                       0x00
#define USER_STRING06                       0x00
#define USER_STRING05                       0x00
#define USER_STRING04                       0x00
#define USER_STRING03                       0x00
#define USER_STRING02                       0x00
#define USER_STRING01                       0x00


/*********************************************************************/
// Definition of MRF24J40 enables the application to use Microchip
// MRF24J40 2.4GHz IEEE 802.15.4 compliant RF transceiver
/*********************************************************************/
#define MRF24J40


/*********************************************************************/
// EUI_x defines the xth byte of permanent address for the wireless
// node
/*********************************************************************/
#define EUI_7 0x00 //0xaa
#define EUI_6 0x01 //0xaa
#define EUI_5 0x02 //0xaa
#define EUI_4 0xFE //0xaa
#define EUI_3 0xFF //0xaa
#define EUI_2 0xA3 //0xaa
#define EUI_1 0x04 //0xaa
#define EUI_0 0x00 //0xaa

/*********************************************************************/
// MAX_PAIR_SIZE defines the maximum number of pair that the 
// device allowes at the same time. 
/*********************************************************************/
#define MAX_PAIR_SIZE             1


/*********************************************************************/
// ENABLE_PA_LNA enable the external power amplifier and low noise
// amplifier on the RF board to achieve longer radio communication 
// range. To enable PA/LNA on RF board without power amplifier and
// low noise amplifier may be harmful to the transceiver.
/*********************************************************************/
//#define ENABLE_PA_LNA


#include "HardwareProfile.h"

#endif
