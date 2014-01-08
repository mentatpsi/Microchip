/********************************************************************
* FileName:		ConfigMRF24J40.h
* Dependencies: 
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
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
*********************************************************************
* File Description:
*
*  This file provides configuration settings for MRF24J40
*
* Change History:
*  Rev   Date         Author        Description
*  2.0   4/15/2009    yfy           MiMAC and MiApp revision
*  3.1   5/28/2010    yfy           MiWi DE 3.1
*  4.1   6/3/2011     yfy           MAL v2011-06
********************************************************************/

#ifndef __CONFIG_MRF24J40_H

    #define __CONFIG_MRF24J40_H
    
    /*********************************************************************/
    // TURBO_MODE enables MRF24J40 transceiver to perform the communication
    // in proprietary modulation, which is not compliant to IEEE 802.15.4
    // specification. The data rate at turbo mode is up to 625Kbps.
    /*********************************************************************/
    //#define TURBO_MODE
    
    
    /*********************************************************************/
    // VERIFY_TRANSMIT configures the MRF24J40 transceiver to transmit 
    // data in a block procedure, which ensures finish transmission before
    // continue other task. This block procedure ensures the delivery state
    // of transmitting known to the upper protocol layer, thus may be 
    // necessary to detect transmission failure. However, this block procedure
    // slightly lower the throughput
    /*********************************************************************/
    #define VERIFY_TRANSMIT
    
    
    /*********************************************************************/
    // SECURITY_KEY_xx defines xxth byte of security key used in the
    // block cipher
    /*********************************************************************/
    #define SECURITY_KEY_00 0x00
    #define SECURITY_KEY_01 0x01
    #define SECURITY_KEY_02 0x02
    #define SECURITY_KEY_03 0x03
    #define SECURITY_KEY_04 0x04
    #define SECURITY_KEY_05 0x05
    #define SECURITY_KEY_06 0x06
    #define SECURITY_KEY_07 0x07
    #define SECURITY_KEY_08 0x08
    #define SECURITY_KEY_09 0x09
    #define SECURITY_KEY_10 0x0a
    #define SECURITY_KEY_11 0x0b
    #define SECURITY_KEY_12 0x0c
    #define SECURITY_KEY_13 0x0d
    #define SECURITY_KEY_14 0x0e
    #define SECURITY_KEY_15 0x0f
    
    
    /*********************************************************************/
    // KEY_SEQUENCE_NUMBER defines the sequence number that is used to
    // identify the key. Different key should have different sequence
    // number, if multiple security keys are used in the application.
    /*********************************************************************/
    #define KEY_SEQUENCE_NUMBER 0x00
    
    
    /*********************************************************************/
    // SECURITY_LEVEL defines the security mode used in the application
    /*********************************************************************/
    #define SECURITY_LEVEL SEC_LEVEL_CCM_32


    /*********************************************************************/
    // FRAME_COUNTER_UPDATE_INTERVAL defines the NVM update interval for
    // frame counter, when security is enabled. The same interval will be
    // added to the frame counter read from NVM when Network Freezer
    // feature is enabled.
    /*********************************************************************/ 
    #define FRAME_COUNTER_UPDATE_INTERVAL 1024
    
	
	#define BANK_SIZE           2
	
    /*********************************************************************/
    // If MRF24J40MB module with external power amplifier and low noise
    // amplifier has been used, the stack needs to do output power adjustment
    // according to MRF24J40MB data sheet.
    // Comment this part if used other design of MRF24J40 with external PA
    // and/or LNA. This definition cannot be used with definition of
    // MRF24J40MC
    /*********************************************************************/
    #define MRF24J40MB

    /*********************************************************************/
    // If MRF24J40MC module with external power amplifier, low noise
    // amplifier and external antenna has been used, the stack needs to 
    // do output power adjustment according to MRF24J40MC data sheet.
    // Comment this part if used other design of MRF24J40 with external PA
    // and/or LNA. This definition cannot be used with definition of 
    // MRF24J40MB
    /*********************************************************************/
    //#define MRF24J40MC


    #if defined(MRF24J40MB) && defined(MRF24J40MC)
        #error "MRF24J40MB and MRF24J40MC cannot be defined at the same time"
    #endif

    #if defined(MRF24J40MB) || defined(MRF24J40MC)
        #define UNDEFINED_LOCATION  0x00
        #define UNITED_STATES       0x01
        #define CANADA              0x02
        #define EUROPE              0x03
        /*********************************************************************/
        // If MRF24J40MB/C module is used, the output power setting depends on
        // the country where the application is used. Define one of the 
        // locations that this appliation will be applied. If none of the location
        // is set, US FCC setting for MRF24J40MB/C module will be used in the stack.
        // Check MRF24J40MB/C data sheet for details.
        /*********************************************************************/
        #define APPLICATION_SITE    UNITED_STATES
    #endif
	
    
#endif

