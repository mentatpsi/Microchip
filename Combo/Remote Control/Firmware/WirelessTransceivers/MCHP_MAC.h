/********************************************************************
* FileName:		MCHP_MAC.h
* Dependencies: ConfigApp.h
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
*  This file defines interface between MiMAC and Microchip proprietary
*  wireless protocol.
*
* Change History:
*  Rev          Date         Author       Description
*  2.0          4/15/2009    yfy          MiMAC and MiApp revision
*  2.1          06/20/2009   yfy          Add LCD support
*  1.0-1.1.2    12/10/2009   yfy          Modified for RF4CE
********************************************************************/

#ifndef __MCHP_MAC_H
    #define __MCHP_MAC_H

    #include "ConfigApp.h"
    #include "GenericTypeDefs.h"
    #include "Transceivers.h"
    #if defined(PROTOCOL_P2P)
        #include "WirelessProtocols/P2P/ConfigP2P.h"
    #endif

    #define CHANNEL_ASSESSMENT_CARRIER_SENSE    0x00
    #define CHANNEL_ASSESSMENT_ENERGY_DETECT    0x01
    
    #define POWER_STATE_DEEP_SLEEP              0x00
    #define POWER_STATE_OPERATE                 0xFF


    #define PACKET_TYPE_MASK        0x03
    #define BROADCAST_MASK          0x04
    #define SECURITY_MASK           0x08
    #define REPEAT_MASK             0x10
    #define ACK_MASK                0x20
    #define DSTPRSNT_MASK           0x40
    #define SRCPRSNT_MASK           0x80
    
    #define PACKET_TYPE_DATA        0x00
    #define PACKET_TYPE_COMMAND     0x01
    #define PACKET_TYPE_ACK         0x02
    #define PACKET_TYPE_RESERVE     0x03



    /***************************************************************************
     * Parameters to Transmit a Packet
     *  
     *      This structure contains configurations to transmit a packet
     **************************************************************************/
    typedef struct
    {
        union
        {
            BYTE    Val;   
            struct
            {
                BYTE        packetType      : 2;        // type of packet. Possible types are
                                                        // * PACKET_TYPE_DATA - Data type
                                                        // * PACKET_TYPE_COMMAND -  Command type
                                                        // * PACKET_TYPE_ACK -  Acknowledgement type
                                                        // * PACKET_TYPE_RESERVE - Reserved type
                BYTE        broadcast       : 1;        // 1: broadcast, 0: unicast
                BYTE        secEn           : 1;        // 1: secure the MAC payload, 0: send plain text
                BYTE        retransOff      : 1;
                BYTE        ackReq          : 1;        // 1: acknowledgement required, 0: no acknowldgement
                BYTE        destPrsnt       : 1;        // 1: destination address in the packet, 0: destination address not in the packet
                BYTE        sourcePrsnt     : 1;        // 1: source address in the packet, 0: source address not in the packet
            } bits;
        } flags;
        
        BYTE        *DestAddress;           // destination address
		BOOL		altDestAddr;            // use the alternative network address as destination in the packet
        BOOL		altSrcAddr;             // use the alternative network address as source in the packet
        WORD_VAL    DestPANID;              // PAN identifier of the destination

    } MAC_TRANS_PARAM;
    
    
    /***************************************************************************
     * Content of the Received Message
     *  
     *      This structure contains all information of the received message
     **************************************************************************/
    typedef struct
    {
        union
        {
            BYTE        Val;  
            struct
            {
                BYTE    packetType      :2;             // type of packet. Possible types are
                                                        // * PACKET_TYPE_DATA - Data type
                                                        // * PACKET_TYPE_COMMAND -  Command type
                                                        // * PACKET_TYPE_ACK -  Acknowledgement type
                                                        // * PACKET_TYPE_RESERVE - Reserved type
                BYTE    broadcast       :1;             // 1: broadcast, 0: unicast
                BYTE    secEn           :1;             // 1: secure the MAC payload, 0: send plain text
                BYTE    repeat          :1;             // 1: allow repeaters to forward the message, 0: send message directly
                BYTE    ackReq          :1;             // 1: acknowledgement required, 0: no acknowldgement
                BYTE    destPrsnt       :1;             // 1: destination address in the packet, 0: destination address not in the packet
                BYTE    sourcePrsnt     :1;             // 1: source address in the packet, 0: source address not in the packet
            } bits;
        } flags;
            
        BYTE *      SourceAddress;                      // Address of the Sender
        BYTE *      Payload;                            // Pointer to the payload
        BYTE        PayloadLen;                         // Payload size
        BYTE        RSSIValue;                          // RSSI value for the received packet
        BYTE        LQIValue;                           // LQI value for the received packet
        BOOL        altSourceAddress;                   // Source address is the alternative network address
        WORD_VAL    SourcePANID;                        // PAN ID of the sender

    } MAC_RECEIVED_PACKET;
        
    /***************************************************************************
     * Initialization Parameters for MAC
     *  
     *      This structure contains parameters in the initialization
     **************************************************************************/
    typedef struct
    {
        union
        {
            BYTE        Val;
            struct
            {
                BYTE    RepeaterMode    :1;             // 1: Allow to act as repeater, 0: disable repeater function
                BYTE    CCAEnable       :1;             // 1: Enable Clear Channel Assessment, 0: Disable CCA
                BYTE    PAddrLength     :4;             // Length of the permanent address, range from 1 to 8.
            } bits;   
        } actionFlags;
        
        BYTE *PAddress;                                 // Permenet transceiver address
        
    } MACINIT_PARAM;
    
    extern MAC_RECEIVED_PACKET  MACRxPacket;
    
           
    BOOL MiMAC_SetChannel(BYTE channel, BYTE offsetFreq);
    
    
      
    BOOL MiMAC_SetPower(BYTE outputPower);
    
    
     
    BOOL MiMAC_SetAltAddress(BYTE *Address, BYTE *PANID);
    
     
    BOOL MiMAC_Init(MACINIT_PARAM initValue);
    
     
    BYTE MiMAC_ChannelAssessment(BYTE AssessmentMode);
    
    
     
    void MiMAC_DiscardPacket(void);
    
    
     
    BOOL MiMAC_ReceivedPacket(void);
    
    
     
    BOOL MiMAC_SendPacket(MAC_TRANS_PARAM transParam, BYTE *MACPayload, BYTE MACPayloadLen);
    
    
     
    BOOL MiMAC_PowerState(BYTE PowerState);
    

    #undef MY_ADDRESS_LENGTH
    #define MY_ADDRESS_LENGTH       8 
  
    
#endif

