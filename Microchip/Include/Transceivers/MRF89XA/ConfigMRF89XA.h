/********************************************************************
* FileName:		ConfigMRF89XA.h
* Dependencies: 
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16, PIC18 Explorer
* Complier:     Microchip C18 v3.30 or higher
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
*  Rev   Date         Author        Description
*  2.0   4/15/2009    SM            MiWi DE 3.1
*  4.1   6/3/2011     SM            MAL v2011-06
********************************************************************/

#ifndef __CONFIG_MRF89XA_H

    #define __CONFIG_MRF89XA_H
    
    #include "Transceivers/MRF89XA/MRF89XA.h"

    /*********************************************************************/
    // BAND_902, BAND_915 or BAND_863 (or BAND_950 - circuit dependent) are three supported frequency
    // band for Microchip MRF89XA. One and only one of the frequency
    // band must be defined 
    /*********************************************************************/
    //#define BAND_902      //Choose BAND_902 and BAND_915 for FCC and IC
                            //Supports frequencies between 902 - 915MHz
    //#define BAND_915      //Supports frequencies between 915 - 928MHz

    #define BAND_863        //Choose this for Europe ETSI 868MHz Frequency band
                            //Supports frequencies between 863MHz - 870MHz
    
    /*********************************************************************/
    // DATA_RATE_5, DATA_RATE_10, DATA_RATE_20, 
    // DATA_RATE_25, DATA_RATE_40, DATA_RATE_50, DATA_RATE_66,
    // DATA_RATE_100 and DATA_RATE_200 are 10 data rates supported 
    // by Microchip MRF89XA transceivers in MiMAC interface. One and only 
    // one of the data rate must be defined
    /*********************************************************************/  
    //#define DATA_RATE_5
    //#define DATA_RATE_10
    #define DATA_RATE_20
    //#define DATA_RATE_40
    //#define DATA_RATE_50
    //#define DATA_RATE_66
    //#define DATA_RATE_100
    //#define DATA_RATE_200
	
    
    
    /*********************************************************************/
    // LNA_GAIN defines the internal IF gain for
    // MRF89XA transceiver.
    /*********************************************************************/
    #define LNA_GAIN        LNA_GAIN_0_DB
    
    
    /*********************************************************************/
    // TX_POWER defines the output power for MRF89XA
    /*********************************************************************/
    #define TX_POWER        TX_POWER_1_DB
    
    
    /*********************************************************************/
    // RSSI_THRESHOLD defines the threshold for the RSSI digital output
    /*********************************************************************/
   	// #define RSSI_THRESHOLD  RSSI_THRESHOLD_79
    
    
    /*********************************************************************/
    // ENABLE_CCA enables MRF89XA to perform Clear Channel Assessement
    // before transmitting data in MiMAC layer. 
    /*********************************************************************/            
    #define ENABLE_CCA
    
    
    /*********************************************************************/
    // ENABLE_ACK enables MRF89XA to automatically send back an 
    // acknowledgement packet in MiMAC layer after receiving a packet, 
    // when such acknowledgement is requested by the packet sender.
    /*********************************************************************/
	#define ENABLE_ACK
    
    
    /*********************************************************************/
    // ENABLE_RETRANSMISSION enables MRF89XA to retransmit the packet
    // up to RETRANSMISSION_TIMES, if ENABLE_ACK is defined, and a proper
    // acknowledgement packet is not received by the sender in predefined
    // time period.
    /*********************************************************************/
    #define ENABLE_RETRANSMISSION


    /*********************************************************************/
    // SOURCE_ADDRESS_ABSENT disable the stack to transmit the source address
    // in the MAC layer, if the destination does not care where the message
    // comes from. This feature is highly application dependent. This feature
    // is only available for transceivers that support MiMAC frame format.
    /*********************************************************************/
    #define SOURCE_ADDRESS_ABSENT


    /*********************************************************************/
    // MAX_ALLOWED_TX_FAILURE defines the maximum number of tries to
    // transmit a packet before a transmission failure can be issued to
    // the upper protocol layer. Transmission failure under this condition
    // usually due to timeout from MRF89XA pin switch.
    /*********************************************************************/ 
    #define MAX_ALLOWED_TX_FAILURE  20
    
    
    /*********************************************************************/
    // RETRANSMISSION_TIMES defines the maximum retries that can be performed
    // if a proper acknowledgement packet is not received in predefined 
    // time period, if ENABLE_RETRANSMISSION is defined.
    /*********************************************************************/
    #define RETRANSMISSION_TIMES    3
    
    
    /*********************************************************************/
    // CCA_TIMES defines the total number of Clear Channel Assessment
    // in the CCA procedure. CCA procedure perform CCA for CCA_TIMES
    // and check if the times of CCA failure beyond the number defined
    // in CCA_THRESHOLD. In the case that CCA failure times is beyond 
    // CCA_THRESHOLD, the whole procedure must be repeated up to 
    // CCA_RETRIES times before transmission failure can be flagged.
    /*********************************************************************/
    #define CCA_TIMES               5
    
    
    /*********************************************************************/
    // CCA_THRESHOLD defines the threshold times of Clear Channel Assessment
    // failure in the CCA procedure. CCA procedure perform CCA for CCA_TIMES
    // and check if the times of CCA failure beyond the number defined
    // in CCA_THRESHOLD. In the case that CCA failure times is beyond 
    // CCA_THRESHOLD, the whole procedure must be repeated up to 
    // CCA_RETRIES times before transmission failure can be flagged.
    /*********************************************************************/
    #define CCA_THRESHOLD           65 	//-65dB limit for CCA threshold values (as sampling at data/preamble)
										//can use higher values for preamble (refer 802.11 standard)
    
    
    /*********************************************************************/
    // CCA_RETRIES defines the maximum retries can be performed in the case 
    // of Clear Channel Assessment failure in the CCA procedure. CCA 
    // procedure perform CCA for CCA_TIMES and check if the times of CCA 
    // failure beyond the number defined in CCA_THRESHOLD. In the case that 
    // CCA failure times is beyond CCA_THRESHOLD, the whole procedure must 
    // be repeated up to CCA_RETRIES times before transmission failure can 
    // be flagged.
    /*********************************************************************/
    #define CCA_RETRIES             4
    
    
    /*********************************************************************/
    // BANK_SIZE defines the number of packet can be received and stored
    // to wait for handling in MiMAC layer.
    /*********************************************************************/
    #define BANK_SIZE               2
    
    
    /*********************************************************************/
    // ACK_INFO_SIZE defines the number of acknowledgement information
    // structure can be stored to avoid duplicate packet to the protocol
    // layer.
    /*********************************************************************/
    #define ACK_INFO_SIZE           5
    
    /*********************************************************************/
    //USE_IRQ0_AS_INTERRUPT enable MRF89XA transceiver to use both the IRQ0
    //and IRQ1 interrupts. In MRF89XA.c file IRQ0 is received after 
    //preamble detection (for accurate RSSI) and IRQ1 received after CRC
    //correct packet is received.
    //If IRQ0 and IRQ1 are both connected to PIC interrupts pins then
    //enable this mode
    /*********************************************************************/
    //#define USE_IRQ0_AS_INTERRUPT
    #define RFIE        PHY_IRQ1_En
    #define RFIF        PHY_IRQ1
    #define RF_INT_PIN  IRQ1_INT_PIN
    #define RF_INT_TRIS IRQ1_INT_TRIS

    /***********************************************************************/
    //  SECURITY_KEY_xx defines xxth byte of security key used in the block
    //  cipher. The length of the key depends on the key size of the block
    //  cipher.                                                            
    /***********************************************************************/
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
    // SECURITY_LEVEL defines the security mode used in the application.
    /*********************************************************************/
    #define SECURITY_LEVEL SEC_LEVEL_CCM_16


    /*********************************************************************/
    // The interval to update the frame counter in the NVM
    /*********************************************************************/
    #define FRAME_COUNTER_UPDATE_INTERVAL 1024
    
    
    /*********************************************************************/
    // MRF89XA configuration verification
    /*********************************************************************/
    #if !defined(BAND_902) && !defined(BAND_915) && !defined(BAND_863)
        #error "At least one of the frequency band must be defined for MRF_89XA"
    #endif
    
    #if (defined(BAND_902) && defined(BAND_915)) || (defined(BAND_902) && defined(BAND_863)) || (defined(BAND_915) && defined(BAND_863))
        #error "Only one frequency band can be defined for MRF_89XA"
    #endif

    #if !defined(DATA_RATE_2) && !defined(DATA_RATE_5) && !defined(DATA_RATE_10) && !defined(DATA_RATE_20) && !defined(DATA_RATE_40) &&  !defined(DATA_RATE_50) && !defined(DATA_RATE_66) && !defined(DATA_RATE_100) && !defined(DATA_RATE_200)
        #error "At least one of the data rate must be defined for MRF_89XA"
    #endif
    
    #undef DATA_RATE_DEFINED
    #if defined(DATA_RATE_2)
        #define DATA_RATE_DEFINED
    #endif
    #if defined(DATA_RATE_5)
        #if defined(DATA_RATE_DEFINED)
            #error "Only one of the data rate can be defined for MRF_89XA"
        #endif
        #define DATA_RATE_DEFINED
    #endif
    #if defined(DATA_RATE_10)
        #if defined(DATA_RATE_DEFINED)
            #error "Only one of the data rate can be defined for MRF_89XA"
        #endif
        #define DATA_RATE_DEFINED
    #endif
    #if defined(DATA_RATE_20)
        #if defined(DATA_RATE_DEFINED)
            #error "Only one of the data rate can be defined for MRF_89XA"
        #endif
        #define DATA_RATE_DEFINED
    #endif
    #if defined(DATA_RATE_40)
        #if defined(DATA_RATE_DEFINED)
            #error "Only one of the data rate can be defined for MRF_89XA"
        #endif
        #define DATA_RATE_DEFINED
    #endif
    #if defined(DATA_RATE_50)
        #if defined(DATA_RATE_DEFINED)
            #error "Only one of the data rate can be defined for MRF_89XA"
        #endif
        #define DATA_RATE_DEFINED
    #endif
	#if defined(DATA_RATE_66)
        #if defined(DATA_RATE_DEFINED)
            #error "Only one of the data rate can be defined for MRF_89XA"
        #endif
        #define DATA_RATE_DEFINED
    #endif
    #if defined(DATA_RATE_100)
        #if defined(DATA_RATE_DEFINED)
            #error "Only one of the data rate can be defined for MRF_89XA"
        #endif
        #define DATA_RATE_DEFINED
    #endif
    #if defined(DATA_RATE_200)
        #if defined(DATA_RATE_DEFINED)
            #error "Only one of the data rate can be defined for MRF_89XA"
        #endif
        #define DATA_RATE_DEFINED
    #endif
    #undef DATA_RATE_DEFINED

#endif


