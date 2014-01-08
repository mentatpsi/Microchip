/********************************************************************
* FileName:		ConfigMRF24J40_v2.h
* Dependencies: 
* Processor:	PIC18, PIC24F, PIC24H, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16, PIC18 Explorer
* Complier:     Microchip C18 v3.30 or higher
*				Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
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
*********************************************************************
* File Description:
*
*  This file provides configuration settings for MRF24J40
*
* Change History:
*  Rev          Date         Author        Description
*  2.0          4/15/2009    yfy           MiMAC and MiApp revision
*  2.1          06/20/2009   yfy           Add LCD support
*  1.0-1.1.2    12/10/2009   yfy           Modified for RF4CE
********************************************************************/

#ifndef __CONFIG_MRF24J40_H

    #define __CONFIG_MRF24J40_H
    
    /*********************************************************************/
    // TURBO_MODE enables MRF24J40 transceiver to perform the communication
    // in proprietary modulation, which is not compliant to IEEE 802.15.4
    // specification. The data rate at turbo mode is 625Kbps.
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
    // SECURITY_LEVEL defines the security mode used in the application
    /*********************************************************************/
    #define SECURITY_LEVEL SEC_LEVEL_CCM_32
    
    /*********************************************************************/
    // If MRF24J40MB module with external power amplifier and low noise
    // amplifier has been used, the stack needs to do output power adjustment
    // according to MRF24J40MB data sheet.
    // Comment this part if used other design of MRF24J40 with external PA
    // and/or LNA
    /*********************************************************************/
    //#define MRF24J40MB 
    
    #if defined(MRF24J40MB)
        #define UNDEFINED_LOCATION  0x00
        #define UNITED_STATES       0x01
        #define CANADA              0x02
        #define EUROPE              0x03
        /*********************************************************************/
        // If MRF24J40MB module is used, the output power setting depends on 
        // the country where the application is used. Define one of the 
        // locations that this appliation will be applied. If none of the location
        // is set, US FCC setting for MRF24J40MB module will be used in the stack.
        // Check MRF24J40MB data sheet for details.
        /*********************************************************************/
        #define APPLICATION_SITE    UNITED_STATES
    #endif
    

#endif

