/********************************************************************
* FileName:		ConfigRF4CE.h
* Dependencies: 
* Processor:	PIC18, PIC24F, PIC24H, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
*
* Copyright © 2007-2008 Microchip Technology Inc.  All rights reserved.
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
*  This file provides configuration settings for RF4CE protocol stack
*
* Change History:
*  Rev      Date         Author        Description
*  1.0-1.1  12/10/2009   yfy           Initial Release
********************************************************************/

#ifndef __CONFIGURE_RF4CE_H

    #define __CONFIGURE_RF4CE_H
    
    #include "ConfigApp.h"
    #include "WirelessProtocols/SymbolTime.h" 


    /*********************************************************************/
    // TX_BUFFER_SIZE defines the maximum size of application payload
    // which is to be transmitted
    /*********************************************************************/
    #define TX_BUFFER_SIZE 127    

    // The maximum number of discovery record to be saved
    #define DISCOVERY_RESULT_SIZE       1
    
    // The number of key seed packet to be exchanged between pairing devices during
    // a secured join procedure.
    #define KEY_EXCHANGE_COUNT          0xFF
    
    // Network scan duration time in symbols, this value is used for both energy detection
    // scan as well as active scan
    #define NWK_SCAN_DURATION           ONE_SECOND
    
    // The total time in symbols that a target stay awake in one ducy cycle in power save mode  
    #define NWK_ACTIVE_PERIOD           (SYMBOLS_TO_TICKS(0x0000041A))              // 16.8ms

    // The total time in symbols that a target sleeps in a duty cycle in power save mode
    #define NWK_SLEEP_PERIOD            (SYMBOLS_TO_TICKS(61450))                   // 1 second - 16.8ms
    
    // The total time in symbols that a target spends in a duty cycle (sleep and wakes up) in 
    // power save mode. This value should NOT be higher than the network constant of 
    // NWKC_MAX_DUTY_CYCLE. The rule is (NWK_ACTIVE_PERIOD + NWK_SLEEP_PERIOD) = NWK_DUCY_CYCLE
    #define NWK_DUTY_CYCLE              ONE_SECOND    
    
    // The total number of retries of discovery process within each channel, before the number of
    // discovered different nodes reach DISCOVERY_RESULT_SIZE 
    #define NWK_MAX_DISCOVERY_REPETITIONS       5       //0x1E
    
    // The time interval in symbols that two seperate discovery request can be performed in the
    // same channel 
    #define NWK_DISCOVERY_REPETITION_INTERVAL   ONE_SECOND
    
    // The minimum signal strength threshold that a discovery request command can be accepted
    #define NWK_DISCOVERY_LQI_THRESHOLD         0x01
    
    // The maximum time period in symbols that the RF4CE stack will wait for a response from the 
    // peer device
    #define NWK_RESPONSE_WAIT_TIME              (SYMBOLS_TO_TICKS(0x00186A))        // 100 ms   
    
    // The boolean to indicate if the RF4CE target device accepts normal discovery request. When this
    // boolean is set to be FALSE, only auto discovery can be used to accept pair request.
    #define NWK_INDICATE_DISCOVERY_REQUEST      FALSE
    
    /*****************************************************************************
      This definition defines how MCU behaves in power saving mode. If this
      definition is defined, MCU will go to sleep during idle time of duty
      cycle and be waken up by watch dog timer (WDT). Due to the fact that
      the WDT can only be configured to timeout at certain interval, it
      cannot be sure that it will wake up every 983.2 ms (1 second - 16.8ms)
      specified by the specification. The result is that there is a small
      chance (1.68% + inacuracy of WDT), that the packet cannot be received
      by the target device. User has two options:
        1. Keep the MCU awake, where the MCU keep track of timing precisely.
           In this case, just comment out this definition
        2. Tolerate the small chance of failure, or reduce the duty cycle to
           be 500ms to ensure 100% delivery of message in power save mode. Even if
           duty cycle is reduced to be 500ms the power consumption is still much
           lower than the option 1. In this case, uncomment this definition.      
      *****************************************************************************/
    #define TARGET_POWERSAVE_MCU_SLEEP
#endif

