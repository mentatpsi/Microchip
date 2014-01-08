/********************************************************************
* FileName:		MCHP_MAC.h
* Dependencies: P2PDefs.h
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
*  This file defines interface between MiMAC and Microchip proprietary
*  wireless protocol.
*
* Change History:
*  Rev   Date         Author       Description
*  2.0   4/15/2009    yfy          MiMAC and MiApp revision
*  3.1   5/28/2010    yfy          MiWi DE 3.1
*  4.1   6/3/2011     yfy          MAL v2011-06
********************************************************************/

#ifndef __MCHP_MAC_H
    #define __MCHP_MAC_H

    #include "ConfigApp.h"
    #include "GenericTypeDefs.h"
    #include "Transceivers/Transceivers.h"
    #if defined(PROTOCOL_MIWI)
        #include "WirelessProtocols/MiWi/ConfigMiWi.h"
    #endif
    #if defined(PROTOCOL_P2P)
        #include "WirelessProtocols/P2P/ConfigP2P.h"
    #endif
    #if defined(PROTOCOL_MIWI_PRO)
        #include "WirelessProtocols/MiWiPRO/ConfigMiWiPRO.h"
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
                BYTE        repeat          : 1;        // 1: allow repeaters to forward the message, 0: send message directly
                BYTE        ackReq          : 1;        // 1: acknowledgement required, 0: no acknowldgement
                BYTE        destPrsnt       : 1;        // 1: destination address in the packet, 0: destination address not in the packet
                BYTE        sourcePrsnt     : 1;        // 1: source address in the packet, 0: source address not in the packet
            } bits;
        } flags;
        
        BYTE        *DestAddress;           // destination address
        #if defined(IEEE_802_15_4)
			BOOL		altDestAddr;        // use the alternative network address as destination in the packet
            BOOL		altSrcAddr;         // use the alternative network address as source in the packet
            WORD_VAL    DestPANID;          // PAN identifier of the destination
        #endif

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
        #if defined(IEEE_802_15_4)
            BOOL        altSourceAddress;               // Source address is the alternative network address
            WORD_VAL    SourcePANID;                    // PAN ID of the sender
        #endif
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
                BYTE    NetworkFreezer  :1;
                BYTE    PAddrLength     :4;             // Length of the permanent address, range from 1 to 8.
            } bits;   
        } actionFlags;
        
        BYTE *PAddress;                                 // Permenet transceiver address
        
    } MACINIT_PARAM;
    
    extern MAC_RECEIVED_PACKET  MACRxPacket;
    
    /************************************************************************************
     * Function:
     *      BOOL MiMAC_SetChannel(BYTE channel, BYTE offsetFreq)
     *
     * Summary:
     *      This function set the operating channel for the RF transceiver
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      set the operating frequency of the RF transceiver. Valid channel
     *      number are from 0 to 31. For different frequency band, data rate
     *      and other RF settings, some channels from 0 to 31 might be
     *      unavailable. Paramater offsetFreq is used to fine tune the center
     *      frequency across the frequency band. For transceivers that follow
     *      strict definition of channels, this parameter may be discarded.
     *      The center frequency is calculated as 
     *      (LowestFrequency + Channel * ChannelGap + offsetFreq)
     *
     * PreCondition:    
     *      Hardware initialization on MCU has been done. 
     *
     * Parameters: 
     *      BYTE channel -  Channel number. Range from 0 to 31. Not all channels
     *                      are available under all conditions.
     *      BYTE offsetFreq -   Offset frequency used to fine tune the center 
     *                          frequency. May not apply to all RF transceivers
     *
     * Returns: 
     *      A boolean to indicates if channel setting is successful.
     *
     * Example:
     *      <code>
     *      // Set center frequency to be exactly channel 12
     *      MiMAC_SetChannel(12, 0);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/       
    BOOL MiMAC_SetChannel(BYTE channel, BYTE offsetFreq);
    
    
    /************************************************************************************
     * Function:
     *      BOOL MiMAC_SetPower(BYTE outputPower)
     *
     * Summary:
     *      This function set the output power for the RF transceiver
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      set the output power for the RF transceiver. Whether the RF
     *      transceiver can adjust output power depends on the hardware
     *      implementation.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      BYTE outputPower -  RF transceiver output power. 
     *
     * Returns: 
     *      A boolean to indicates if setting output power is successful.
     *
     * Example:
     *      <code>
     *      // Set output power to be 0dBm
     *      MiMAC_SetPower(TX_POWER_0_DB);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/  
    BOOL MiMAC_SetPower(BYTE outputPower);
    
    
    /************************************************************************************
     * Function:
     *      BOOL MiMAC_SetAltAddress(BYTE *Address, BYTE *PANID)
     *
     * Summary:
     *      This function set the alternative network address and PAN identifier if
     *      applicable
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      set alternative network address and/or PAN identifier. This function
     *      call applies to only IEEE 802.15.4 compliant RF transceivers. In case
     *      alternative network address is not supported, this function will return
     *      FALSE.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      BYTE * Address -    The alternative network address of the host device.
     *      BYTE * PANID -      The PAN identifier of the host device
     *
     * Returns: 
     *      A boolean to indicates if setting alternative network address is successful.
     *
     * Example:
     *      <code>
     *      WORD NetworkAddress = 0x0000;
     *      WORD PANID = 0x1234;
     *      MiMAC_SetAltAddress(&NetworkAddress, &PANID);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    BOOL MiMAC_SetAltAddress(BYTE *Address, BYTE *PANID);
    
    /************************************************************************************
     * Function:
     *      BOOL MiMAC_Init(MACINIT_PARAM initValue)
     *
     * Summary:
     *      This function initialize MiMAC layer
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      initialize the MiMAC layer. The initialization parameter is 
     *      assigned in the format of structure MACINIT_PARAM.
     *
     * PreCondition:    
     *      MCU initialization has been done. 
     *
     * Parameters: 
     *      MACINIT_PARAM initValue -   Initialization value for MiMAC layer
     *
     * Returns: 
     *      A boolean to indicates if initialization is successful.
     *
     * Example:
     *      <code>
     *      MiMAC_Init(initParameter);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    BOOL MiMAC_Init(MACINIT_PARAM initValue);
    
    /************************************************************************************
     * Function:
     *      BYTE MiMAC_ChannelAssessment(BYTE AssessmentMode)
     *
     * Summary:
     *      This function perform the noise detection on current operating channel
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      perform the noise detection scan. Not all assessment modes are supported
     *      for all RF transceivers.
     *
     * PreCondition:    
     *      MiMAC initialization has been done.  
     *
     * Parameters: 
     *      BYTE AssessmentMode -   The mode to perform noise assessment. The possible 
     *                              assessment modes are
     *                              * CHANNEL_ASSESSMENT_CARRIER_SENSE Carrier sense detection mode
     *                              * CHANNEL_ASSESSMENT_ENERGY_DETECT Energy detection mode
     *
     * Returns: 
     *      A byte to indicate the noise level at current channel.
     *
     * Example:
     *      <code>
     *      NoiseLevel = MiMAC_ChannelAssessment(CHANNEL_ASSESSMENT_CARRIER_SENSE);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    BYTE MiMAC_ChannelAssessment(BYTE AssessmentMode);
    
    
    /************************************************************************************
     * Function:
     *      void MiMAC_DiscardPacket(void)
     *
     * Summary:
     *      This function discard the current packet received from the RF transceiver
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      discard the current packet received from the RF transceiver.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      None
     *
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      if( TRUE == MiMAC_ReceivedPacket() )
     *      {
     *          // handle the raw data from RF transceiver
     * 
     *          // discard the current packet
     *          MiMAC_DiscardPacket();
     *      }
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    void MiMAC_DiscardPacket(void);
    
    
    /************************************************************************************
     * Function:
     *      BOOL MiMAC_ReceivedPacket(void)
     *
     * Summary:
     *      This function check if a new packet has been received by the RF transceiver
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      check if a packet has been received by the RF transceiver. When a packet has
     *      been received, all information will be stored in the global variable 
     *      MACRxPacket in the format of MAC_RECEIVED_PACKET;
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      None
     *
     * Returns: 
     *      A boolean to indicate if a packet has been received by the RF transceiver.
     *
     * Example:
     *      <code>
     *      if( TRUE == MiMAC_ReceivedPacket() )
     *      {
     *          // handle the raw data from RF transceiver
     * 
     *          // discard the current packet
     *          MiMAC_DiscardPacket();
     *      }
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    BOOL MiMAC_ReceivedPacket(void);
    
    
    /************************************************************************************
     * Function:
     *      BOOL MiMAC_SendPacket(  MAC_TRANS_PARAM transParam, 
     *                              BYTE *MACPayload, BYTE MACPayloadLen)
     *
     * Summary:
     *      This function transmit a packet
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      send a packet. Input parameter transParam configure the way
     *      to transmit the packet.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      MAC_TRANS_PARAM transParam -    The struture to configure the transmission way
     *      BYTE * MACPaylaod -             Pointer to the buffer of MAC payload
     *      BYTE MACPayloadLen -            The size of the MAC payload
     *
     * Returns: 
     *      A boolean to indicate if a packet has been received by the RF transceiver.
     *
     * Example:
     *      <code>
     *      MiMAC_SendPacket(transParam, MACPayload, MACPayloadLen);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    BOOL MiMAC_SendPacket(MAC_TRANS_PARAM transParam, BYTE *MACPayload, BYTE MACPayloadLen);
    
    
    /************************************************************************************
     * Function:
     *      BOOL MiMAC_PowerState(BYTE PowerState)
     *
     * Summary:
     *      This function puts the RF transceiver into sleep or wake it up
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      set different power state for the RF transceiver. There are minimal 
     *      power states defined as deep sleep and operating mode. Additional
     *      power states can be defined for individual RF transceiver depends
     *      on hardware design.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      BYTE PowerState -   The power state of the RF transceiver to be set to. 
     *                          The minimum definitions for all RF transceivers are
     *                          * POWER_STATE_DEEP_SLEEP RF transceiver deep sleep mode.
     *                          * POWER_STATE_OPERATE RF transceiver operating mode.
     * Returns: 
     *      A boolean to indicate if chaning power state of RF transceiver is successful.
     *
     * Example:
     *      <code>
     *      // Put RF transceiver into sleep
     *      MiMAC_PowerState(POWER_STATE_DEEP_SLEEP);
     *      // Put MCU to sleep
     *      Sleep();
     *      // Wake up the MCU by WDT, external interrupt or any other means
     *
     *      // Wake up the RF transceiver
     *      MiMAC_PowerState(POWER_STATE_OPERATE); 
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    BOOL MiMAC_PowerState(BYTE PowerState);
    
    #if defined(IEEE_802_15_4)
        #undef MY_ADDRESS_LENGTH
        #define MY_ADDRESS_LENGTH       8 
    #endif
    
    #if defined(ENABLE_SECURITY)
        extern DWORD_VAL IncomingFrameCounter[CONNECTION_SIZE];
    #endif
    
    void UserInterruptHandler(void);

#endif

