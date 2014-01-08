/********************************************************************
* FileName:		P2P.c
* Dependencies:    
* Processor:	PIC18, PIC24, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher		
*               Microchip C32 v1.02 or higher
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice for P2P Software:
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
*  This is the P2P stack  
*
* Change History:
*  Rev   Date         Author    Description
*  0.1   1/03/2008    yfy       Initial revision
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  2.1   6/20/2009    yfy       Add LCD support
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

/************************ HEADERS **********************************/
#include "ConfigApp.h"

#if defined(PROTOCOL_P2P)

#include "WirelessProtocols/MSPI.h"
#include "WirelessProtocols/P2P/P2P.h"
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "WirelessProtocols/Console.h"
#include "WirelessProtocols/NVM.h"
#include "WirelessProtocols/SymbolTime.h"
#include "Transceivers/MCHP_MAC.h"
#include "Transceivers/Transceivers.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/EEPROM.h"


/************************ VARIABLES ********************************/
// permanent address definition
#if MY_ADDRESS_LENGTH == 8
    BYTE myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4, EUI_5,EUI_6,EUI_7};
#elif MY_ADDRESS_LENGTH == 7
    BYTE myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4, EUI_5,EUI_6};
#elif MY_ADDRESS_LENGTH == 6
    BYTE myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4, EUI_5};
#elif MY_ADDRESS_LENGTH == 5
    BYTE myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4};        
#elif MY_ADDRESS_LENGTH == 4
    BYTE myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3};  
#elif MY_ADDRESS_LENGTH == 3
    BYTE myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2};    
#elif MY_ADDRESS_LENGTH == 2
    BYTE myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1};    
#endif


#if defined(ENABLE_ED_SCAN) || defined(ENABLE_ACTIVE_SCAN) || defined(ENABLE_FREQUENCY_AGILITY)
    // Scan Duration formula for P2P Connection:
    //  60 * (2 ^ n + 1) symbols, where one symbol equals 16us
    #define SCAN_DURATION_0 SYMBOLS_TO_TICKS(120)
    #define SCAN_DURATION_1 SYMBOLS_TO_TICKS(180)
    #define SCAN_DURATION_2 SYMBOLS_TO_TICKS(300)
    #define SCAN_DURATION_3 SYMBOLS_TO_TICKS(540)
    #define SCAN_DURATION_4 SYMBOLS_TO_TICKS(1020)
    #define SCAN_DURATION_5 SYMBOLS_TO_TICKS(1980)
    #define SCAN_DURATION_6 SYMBOLS_TO_TICKS(3900)
    #define SCAN_DURATION_7 SYMBOLS_TO_TICKS(7740)
    #define SCAN_DURATION_8 SYMBOLS_TO_TICKS(15420)
    #define SCAN_DURATION_9 SYMBOLS_TO_TICKS(30780)
    #define SCAN_DURATION_10 SYMBOLS_TO_TICKS(61500)
    #define SCAN_DURATION_11 SYMBOLS_TO_TICKS(122940)
    #define SCAN_DURATION_12 SYMBOLS_TO_TICKS(245820)
    #define SCAN_DURATION_13 SYMBOLS_TO_TICKS(491580)
    #define SCAN_DURATION_14 SYMBOLS_TO_TICKS(983100)
    const ROM DWORD ScanTime[15] = {SCAN_DURATION_0,SCAN_DURATION_1,SCAN_DURATION_2,SCAN_DURATION_3,
        SCAN_DURATION_4,SCAN_DURATION_5,SCAN_DURATION_6,SCAN_DURATION_7,SCAN_DURATION_8,SCAN_DURATION_9,
        SCAN_DURATION_10, SCAN_DURATION_11, SCAN_DURATION_12, SCAN_DURATION_13,SCAN_DURATION_14
    };
#endif

/**********************************************************************
 * "#pragma udata" is used to specify the starting address of a 
 * global variable. The address may be MCU dependent on RAM available
 * If the size of the global variable is small, such manual assignment
 * may not be necessary. Developer can comment out the address
 * assignment.
 **********************************************************************/
#if defined(__18CXX)
    #pragma udata TRX_BUFFER
#endif
BYTE TxBuffer[TX_BUFFER_SIZE];
#if defined(__18CXX)
    #pragma udata
#endif

#ifdef ENABLE_INDIRECT_MESSAGE
    /**********************************************************************
     * "#pragma udata" is used to specify the starting address of a 
     * global variable. The address may be MCU dependent on RAM available
     * If the size of the global variable is small, such manual assignment
     * may not be necessary. Developer can comment out the address
     * assignment.
     **********************************************************************/
    #if defined(__18CXX)
        #pragma udata INDIRECT_BUFFER
    #endif
    INDIRECT_MESSAGE indirectMessages[INDIRECT_MESSAGE_SIZE];   // structure to store the indirect messages
                                                                // for nodes with radio off duing idle time
    #if defined(__18CXX)
        #pragma udata
    #endif
#endif

/**********************************************************************
 * "#pragma udata" is used to specify the starting address of a 
 * global variable. The address may be MCU dependent on RAM available
 * If the size of the global variable is small, such manual assignment
 * may not be necessary. Developer can comment out the address
 * assignment.
 **********************************************************************/
#if defined(__18CXX)
    #pragma udata BIGvariables1
#endif
CONNECTION_ENTRY    ConnectionTable[CONNECTION_SIZE];        // The peer device records for P2P connections
#if defined(__18CXX)
    #pragma udata
#endif

#if defined(IEEE_802_15_4)
    WORD_VAL        myPANID;                    // the PAN Identifier for the device
#endif
BYTE            currentChannel = 0;             // current operating channel for the device
BYTE            TxData;
BYTE            ConnMode = DISABLE_ALL_CONN;
BYTE            P2PCapacityInfo;
RECEIVED_MESSAGE  rxMessage;                    // structure to store information for the received packet
BYTE            LatestConnection;
volatile P2P_STATUS P2PStatus;
extern BYTE     AdditionalNodeID[];             // the additional information regarding the device
                                                // that would like to share with the peer on the 
                                                // other side of P2P connection. This information 
                                                // is applicaiton specific. 
#if defined(ENABLE_ACTIVE_SCAN)
    BYTE    ActiveScanResultIndex;
    ACTIVE_SCAN_RESULT ActiveScanResults[ACTIVE_SCAN_RESULT_SIZE];  // The results for active scan, including
                                                                    // the PAN identifier, signal strength and 
                                                                    // operating channel
#endif

#ifdef ENABLE_SLEEP
    MIWI_TICK DataRequestTimer;
#endif

MAC_RECEIVED_PACKET MACRxPacket;

#if defined(ENABLE_SECURITY)
    DWORD_VAL IncomingFrameCounter[CONNECTION_SIZE];  // If authentication is used, IncomingFrameCounter can prevent replay attack
#endif

#if defined(ENABLE_NETWORK_FREEZER)
    MIWI_TICK nvmDelayTick;
#endif


#if defined(ENABLE_TIME_SYNC)
    #if defined(ENABLE_SLEEP)
        WORD_VAL WakeupTimes;
        WORD_VAL CounterValue;
    #elif defined(ENABLE_INDIRECT_MESSAGE)
        BYTE TimeSyncSlot = 0;
        MIWI_TICK TimeSyncTick;
        MIWI_TICK TimeSlotTick;
    #endif
#endif

/************************ FUNCTION DEFINITION ********************************/
BYTE AddConnection(void);
BOOL isSameAddress(INPUT BYTE *Address1, INPUT BYTE *Address2);

#if defined(IEEE_802_15_4)
    BOOL SendPacket(INPUT BOOL Broadcast, INPUT WORD_VAL DestinationPANID, 
                    INPUT BYTE *DestinationAddress, INPUT BOOL isCommand, 
                    INPUT BOOL SecurityEnabled);
#else
    BOOL SendPacket(INPUT BOOL Broadcast,  
                    INPUT BYTE *DestinationAddress, INPUT BOOL isCommand, 
                    INPUT BOOL SecurityEnabled);
#endif                    

#ifdef ENABLE_FREQUENCY_AGILITY
    void StartChannelHopping(INPUT BYTE OptimalChannel);
#endif

BOOL CheckForData(void);

/************************ FUNCTIONS ********************************/

/******************************************************************/
// C18 compiler cannot optimize the code with a macro. Instead of 
// calling macro Nop in a big function, we define a wrapper to call
// Nop in order to be able to optimize the code efficiently.
/******************************************************************/
void MacroNop(void)
{
    Nop(); 
}    


/*********************************************************************
 * void P2PTasks( void )
 *
 * Overview:        This function maintains the operation of the stack
 *                  It should be called as often as possible. 
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    The stack receives, handles, buffers, and transmits 
 *                  packets.  It also handles all of the joining 
 * 
 ********************************************************************/
void P2PTasks(void)
{
    BYTE            i;
    MIWI_TICK            tmpTick;
    
    #ifdef ENABLE_INDIRECT_MESSAGE
        // check indirect message periodically. If an indirect message is not acquired within
        // time of INDIRECT_MESSAGE_TIMEOUT
        for(i = 0; i < INDIRECT_MESSAGE_SIZE; i++)
        {
            if( indirectMessages[i].flags.bits.isValid )
            {
                tmpTick = MiWi_TickGet();
                if( MiWi_TickGetDiff(tmpTick, indirectMessages[i].TickStart) > INDIRECT_MESSAGE_TIMEOUT )
                {
                    indirectMessages[i].flags.Val = 0x00;   
                    Printf("\r\nIndirect message expired");
                }    
            }    
        }
    #endif
    
    #ifdef ENABLE_SLEEP
        // check if a response for Data Request has been received with in 
        // time of RFD_DATA_WAIT, defined in P2P.h. Expire the Data Request
        // to let device goes to sleep, if no response is received. Save
        // battery power even if something wrong with associated device
        if( P2PStatus.bits.DataRequesting )
        {
            tmpTick = MiWi_TickGet();
            if( MiWi_TickGetDiff(tmpTick, DataRequestTimer) > RFD_DATA_WAIT )
            {
                Printf("Data Request Expired\r\n");
                P2PStatus.bits.DataRequesting = 0;
                #if defined(ENABLE_TIME_SYNC)
                    WakeupTimes.Val = RFD_WAKEUP_INTERVAL / 16;
                    CounterValue.Val = 0xFFFF - ((WORD)4000*(RFD_WAKEUP_INTERVAL % 16));
                #endif
            }
        }
    #endif

    #if defined(ENABLE_NETWORK_FREEZER)
        if( P2PStatus.bits.SaveConnection )
        {
            tmpTick = MiWi_TickGet();
            if( MiWi_TickGetDiff(tmpTick, nvmDelayTick) > (ONE_SECOND) )
            {
                P2PStatus.bits.SaveConnection = 0;
                nvmPutConnectionTable(ConnectionTable);  
                //Printf("\r\nSave Connection\r\n");   
            }
        }
    #endif

    #if defined(ENABLE_TIME_SYNC) && !defined(ENABLE_SLEEP) && defined(ENABLE_INDIRECT_MESSAGE)
        tmpTick = MiWi_TickGet();
        if( MiWi_TickGetDiff(tmpTick, TimeSyncTick) > ((ONE_SECOND) * RFD_WAKEUP_INTERVAL) )
        {
            TimeSyncTick.Val += ((DWORD)(ONE_SECOND) * RFD_WAKEUP_INTERVAL);
            if( TimeSyncTick.Val > tmpTick.Val )
            {
                TimeSyncTick.Val = tmpTick.Val;
            }    
            TimeSyncSlot = 0;
        }    
    #endif


    // Check if transceiver receive any message.
    if( MiMAC_ReceivedPacket() )
    { 
        rxMessage.flags.Val = 0;
        rxMessage.flags.bits.broadcast = MACRxPacket.flags.bits.broadcast;
        rxMessage.flags.bits.secEn = MACRxPacket.flags.bits.secEn;
        rxMessage.flags.bits.command = (MACRxPacket.flags.bits.packetType == PACKET_TYPE_COMMAND) ? 1:0;
        rxMessage.flags.bits.srcPrsnt = MACRxPacket.flags.bits.sourcePrsnt;
        if( MACRxPacket.flags.bits.sourcePrsnt )
        {
            rxMessage.SourceAddress = MACRxPacket.SourceAddress;
        }
        #if defined(IEEE_802_15_4) && !defined(TARGET_SMALL)
            rxMessage.SourcePANID.Val = MACRxPacket.SourcePANID.Val;
        #endif

        rxMessage.PayloadSize = MACRxPacket.PayloadLen;
        rxMessage.Payload = MACRxPacket.Payload;
      
        #ifndef TARGET_SMALL
            rxMessage.PacketLQI = MACRxPacket.LQIValue;
            rxMessage.PacketRSSI = MACRxPacket.RSSIValue;
        #endif

        if( rxMessage.flags.bits.command )
        {
            // if comes here, we know it is a command frame
            switch( rxMessage.Payload[0] )
            {
                #if defined(ENABLE_HAND_SHAKE)
                    case CMD_P2P_CONNECTION_REQUEST:
                        {
                            // if a device goes to sleep, it can only have one
                            // connection, as the result, it cannot accept new
                            // connection request
                            #ifdef ENABLE_SLEEP
                                MiMAC_DiscardPacket();
                                break;
                            #else
                                
                                BYTE status = STATUS_SUCCESS;
                                
                                // if channel does not math, it may be a 
                                // sub-harmonics signal, ignore the request
                                if( currentChannel != rxMessage.Payload[1] )
                                {
                                    MiMAC_DiscardPacket();
                                    break;
                                }
                                
                                // if new connection is not allowed, ignore 
                                // the request
                                if( ConnMode == DISABLE_ALL_CONN )
                                { 
                                    MiMAC_DiscardPacket();
                                    break;
                                }
                                
                                #if !defined(TARGET_SMALL) && defined(IEEE_802_15_4)
                                    // if PANID does not match, ignore the request
                                    if( rxMessage.SourcePANID.Val != 0xFFFF &&
                                        rxMessage.SourcePANID.Val != myPANID.Val &&
                                        rxMessage.PayloadSize > 2)
                                    {
                                        status = STATUS_NOT_SAME_PAN;
                                    }
                                    else
                                #endif
                                {
                                    // request accepted, try to add the requesting
                                    // device into P2P Connection Entry
                                    status = AddConnection();
                                }

                                if( (ConnMode == ENABLE_PREV_CONN) && (status != STATUS_EXISTS && status != STATUS_ACTIVE_SCAN) )
                                {
                                    status = STATUS_NOT_PERMITTED;
                                }

                                if( (status == STATUS_SUCCESS || status == STATUS_EXISTS ) &&
                                    MiApp_CB_AllowConnection(LatestConnection) == FALSE )
                                {
                                    ConnectionTable[LatestConnection].status.Val = 0;
                                    status = STATUS_NOT_PERMITTED;
                                }
                                    
                                
                                // prepare the P2P_CONNECTION_RESPONSE command
                                MiApp_FlushTx();
                                MiApp_WriteData(CMD_P2P_CONNECTION_RESPONSE);
                                MiApp_WriteData(status);
                                if( status == STATUS_SUCCESS ||
                                    status == STATUS_EXISTS )
                                {
                                    MiApp_WriteData(P2PCapacityInfo);
                                    #if ADDITIONAL_NODE_ID_SIZE > 0
                                        for(i = 0; i < ADDITIONAL_NODE_ID_SIZE; i++)
                                        {
                                            MiApp_WriteData(AdditionalNodeID[i]);
                                        }
                                    #endif
                                }
                                
                                MiMAC_DiscardPacket();
                                
                                // unicast the response to the requesting device
                                #ifdef TARGET_SMALL
                                    #if defined(IEEE_802_15_4)
                                        SendPacket(FALSE, myPANID, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);       
                                    #else
                                        SendPacket(FALSE, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn); 
                                    #endif
                                #else
                                    #if defined(IEEE_802_15_4)
                                        SendPacket(FALSE, rxMessage.SourcePANID, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);
                                    #else
                                        SendPacket(FALSE, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);
                                    #endif
                                #endif
                                
                                #if defined(ENABLE_NETWORK_FREEZER)
                                    if( status == STATUS_SUCCESS )
                                    {
                                        nvmPutConnectionTableIndex(&(ConnectionTable[LatestConnection]), LatestConnection);
                                    }
                                    
                                #endif
                            #endif  // end of ENABLE_SLEEP
                        }
                        break;
                    
                    case CMD_P2P_ACTIVE_SCAN_REQUEST:
                        {
                            if(ConnMode > ENABLE_ACTIVE_SCAN_RSP)
                            {
                                MiMAC_DiscardPacket();
                                break;
                            }
                            if( currentChannel != rxMessage.Payload[1] )
                            {
                                MiMAC_DiscardPacket();
                                break;
                            }
                            
                            MiApp_FlushTx();
                            MiApp_WriteData(CMD_P2P_ACTIVE_SCAN_RESPONSE);
                            MiApp_WriteData(P2PCapacityInfo);
                            #if ADDITIONAL_NODE_ID_SIZE > 0
                                for(i = 0; i < ADDITIONAL_NODE_ID_SIZE; i++)
                                {
                                    MiApp_WriteData(AdditionalNodeID[i]);
                                }
                            #endif
                            MiMAC_DiscardPacket();
                            
                            // unicast the response to the requesting device
                            #ifdef TARGET_SMALL
                                #if defined(IEEE_802_15_4)
                                    SendPacket(FALSE, myPANID, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);       
                                #else
                                    SendPacket(FALSE, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn); 
                                #endif
                            #else
                                #if defined(IEEE_802_15_4)
                                    SendPacket(FALSE, rxMessage.SourcePANID, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);
                                #else
                                    SendPacket(FALSE, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);
                                #endif
                            #endif
                        }
                        break;
                        
                    
                    
                    #ifndef TARGET_SMALL    
                        case CMD_P2P_CONNECTION_REMOVAL_REQUEST:
                            {         
                                MiApp_FlushTx();
                                MiApp_WriteData(CMD_P2P_CONNECTION_REMOVAL_RESPONSE);
                                    
                                for(i = 0; i < CONNECTION_SIZE; i++)
                                {
                                    // if the record is valid
                                    if( ConnectionTable[i].status.bits.isValid )
                                    {
                                        // if the record is the same as the requesting device
                                        if( isSameAddress(rxMessage.SourceAddress, ConnectionTable[i].Address) )
                                        {
                                            // find the record. disable the record and
                                            // set status to be SUCCESS
                                            ConnectionTable[i].status.Val = 0;
                                            #if defined(ENABLE_NETWORK_FREEZER)
                                                nvmPutConnectionTableIndex(&(ConnectionTable[i]), i);
                                            #endif
                                            MiApp_WriteData(STATUS_SUCCESS);
                                            break;
                                        }
                                    } 
                                }
                                
                                MiMAC_DiscardPacket();
                                
                                if( i == CONNECTION_SIZE ) 
                                {
                                    // not found, the requesting device is not my peer
                                    MiApp_WriteData(STATUS_ENTRY_NOT_EXIST);
                                }
                                #ifdef TARGET_SMALL
                                    #if defined(IEEE_802_15_4)
                                        SendPacket(FALSE, myPANID, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);
                                    #else
                                        SendPacket(FALSE, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);
                                    #endif
                                #else
                                    #if defined(IEEE_802_15_4)
                                        SendPacket(FALSE, rxMessage.SourcePANID, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);
                                    #else
                                        SendPacket(FALSE, rxMessage.SourceAddress, TRUE, rxMessage.flags.bits.secEn);
                                    #endif
                                #endif
                            }
                            break;
                    #endif
                    
                    case CMD_P2P_CONNECTION_RESPONSE:
                        {
                            switch( rxMessage.Payload[1] )
                            {
                                case STATUS_SUCCESS:
                                case STATUS_EXISTS:
                                    #if defined(IEEE_802_15_4)
                                        if( myPANID.Val == 0xFFFF )
                                        {
                                            myPANID.Val = rxMessage.SourcePANID.Val;
                                            {
                                                WORD tmp = 0xFFFF;
                                                MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);
                                            }    
                                            #if defined(ENABLE_NETWORK_FREEZER)
                                                nvmPutMyPANID(myPANID.v);
                                            #endif
                                        }
                                    #endif
                                    AddConnection();
                                    #if defined(ENABLE_NETWORK_FREEZER)
                                        P2PStatus.bits.SaveConnection = 1;
                                        nvmDelayTick = MiWi_TickGet();
                                    #endif
                                    break;
 
                                default:
                                    break;
                            }                        
                        }
                        MiMAC_DiscardPacket();
                        break; 
                    
                    
                    case CMD_P2P_ACTIVE_SCAN_RESPONSE:
                        {
                            if( P2PStatus.bits.Resync )
                            {
                                P2PStatus.bits.Resync = 0;   
                            }
                            #ifdef ENABLE_ACTIVE_SCAN 
                                else   
                                {
                                    i = 0;
                                    for(; i < ActiveScanResultIndex; i++)
                                    {
                                        if( (ActiveScanResults[i].Channel == currentChannel) &&
                                        #if defined(IEEE_802_15_4)
                                            (ActiveScanResults[i].PANID.Val == rxMessage.SourcePANID.Val) &&
                                        #endif
                                            isSameAddress(ActiveScanResults[i].Address, rxMessage.SourceAddress)
                                        )
                                        {
                                            break;
                                        }
                                    }
                                    if( i == ActiveScanResultIndex && (i < ACTIVE_SCAN_RESULT_SIZE))
                                    {
                                        ActiveScanResults[ActiveScanResultIndex].Channel = currentChannel;
                                        ActiveScanResults[ActiveScanResultIndex].RSSIValue = rxMessage.PacketRSSI;
                                        ActiveScanResults[ActiveScanResultIndex].LQIValue = rxMessage.PacketLQI;
                                        #if defined(IEEE_802_15_4)
                                            ActiveScanResults[ActiveScanResultIndex].PANID.Val = rxMessage.SourcePANID.Val;
                                        #endif
                                        for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                                        {
                                            ActiveScanResults[ActiveScanResultIndex].Address[i] = rxMessage.SourceAddress[i];
                                        }
                                        ActiveScanResults[ActiveScanResultIndex].Capability.Val = rxMessage.Payload[1];
                                        #if ADDITIONAL_NODE_ID_SIZE > 0
                                            for(i = 0; i < ADDITIONAL_NODE_ID_SIZE; i++)
                                            {
                                                ActiveScanResults[ActiveScanResultIndex].PeerInfo[i] = rxMessage.Payload[2+i];
                                            }
                                        #endif
                                        ActiveScanResultIndex++;
                                    }
                                }
                            #endif
                            MiMAC_DiscardPacket(); 
                        }
                        break;
                    
                    
                    #ifndef TARGET_SMALL
                        case CMD_P2P_CONNECTION_REMOVAL_RESPONSE:
                        {
                            if( rxMessage.Payload[1] == STATUS_SUCCESS )
                            {
                                for(i = 0; i < CONNECTION_SIZE; i++)
                                {
                                    // if the record is valid
                                    if( ConnectionTable[i].status.bits.isValid )
                                    {
                                        // if the record address is the same as the requesting device
                                        if( isSameAddress(rxMessage.SourceAddress, ConnectionTable[i].Address) )
                                        {
                                            // invalidate the record
                                            ConnectionTable[i].status.Val = 0;
                                            #if defined(ENABLE_NETWORK_FREEZER)
                                                nvmPutConnectionTableIndex(&(ConnectionTable[i]), i);
                                            #endif
                                            break;
                                        }
                                    } 
                                }
                            }
                        }
                        MiMAC_DiscardPacket();
                        break;
                    #endif
                #endif
                
                #ifdef ENABLE_INDIRECT_MESSAGE
                    case CMD_DATA_REQUEST:
                    case CMD_MAC_DATA_REQUEST: 
                        {
                            BOOL isCommand = FALSE;
                            WORD_VAL tmpW;
                            
                            MiApp_FlushTx();
                            
                            #if defined(ENABLE_TIME_SYNC) && !defined(ENABLE_SLEEP)
                                MiApp_WriteData(CMD_TIME_SYNC_DATA_PACKET);
                                isCommand = TRUE;
                                tmpTick = MiWi_TickGet();
                                //tmpW.Val = (((ONE_SECOND) * RFD_WAKEUP_INTERVAL) - (tmpTick.Val - TimeSyncTick.Val) + ( TimeSlotTick.Val * TimeSyncSlot ) ) / (ONE_SECOND * 16);
                                //tmpW.Val = (((ONE_SECOND) * RFD_WAKEUP_INTERVAL) - (tmpTick.Val - TimeSyncTick.Val) + ( TimeSlotTick.Val * TimeSyncSlot ) ) / SYMBOLS_TO_TICKS((DWORD)0xFFFF * MICRO_SECOND_PER_COUNTER_TICK / 16);
                                tmpW.Val = (((ONE_SECOND) * RFD_WAKEUP_INTERVAL) - (tmpTick.Val - TimeSyncTick.Val) + ( TimeSlotTick.Val * TimeSyncSlot ) ) / SYMBOLS_TO_TICKS((DWORD)0xFFFF * MICRO_SECOND_PER_COUNTER_TICK / 16);
                                MiApp_WriteData(tmpW.v[0]);
                                MiApp_WriteData(tmpW.v[1]);
                                //tmpW.Val = 0xFFFF - (WORD)((TICKS_TO_SYMBOLS((((ONE_SECOND) * RFD_WAKEUP_INTERVAL) - (tmpTick.Val - TimeSyncTick.Val)  + ( TimeSlotTick.Val * TimeSyncSlot ) + TimeSlotTick.Val/2 - (ONE_SECOND * tmpW.Val * 16) )) * 16 / 250));
                                //tmpW.Val = 0xFFFF - (WORD)((TICKS_TO_SYMBOLS((((ONE_SECOND) * RFD_WAKEUP_INTERVAL) - (tmpTick.Val - TimeSyncTick.Val)  + ( TimeSlotTick.Val * TimeSyncSlot ) + TimeSlotTick.Val/2 - ((DWORD)0xFFFF * tmpW.Val) )) * 16 / MICRO_SECOND_PER_COUNTER_TICK));
                                tmpW.Val = 0xFFFF - (WORD)((TICKS_TO_SYMBOLS((((ONE_SECOND) * RFD_WAKEUP_INTERVAL) - (tmpTick.Val - TimeSyncTick.Val)  + ( TimeSlotTick.Val * TimeSyncSlot ) + TimeSlotTick.Val/2 - SYMBOLS_TO_TICKS((DWORD)0xFFFF * MICRO_SECOND_PER_COUNTER_TICK / 16 * tmpW.Val) )) * 16 / MICRO_SECOND_PER_COUNTER_TICK));
                                if( TimeSyncSlot < TIME_SYNC_SLOTS )
                                {
                                    TimeSyncSlot++;
                                }    
                                MiApp_WriteData(tmpW.v[0]);
                                MiApp_WriteData(tmpW.v[1]);

                            #endif
                            
                            for(i = 0; i < INDIRECT_MESSAGE_SIZE; i++)
                            {
                                if( indirectMessages[i].flags.bits.isValid )
                                {    
                                    BYTE j;

                                    #ifdef ENABLE_BROADCAST
                                        if( indirectMessages[i].flags.bits.isBroadcast )
                                        {
                                            for(j = 0; j < CONNECTION_SIZE; j++)
                                            {
                                                if( indirectMessages[i].DestAddress.DestIndex[j] != 0xFF &&
                                                    isSameAddress(ConnectionTable[indirectMessages[i].DestAddress.DestIndex[j]].Address, rxMessage.SourceAddress) )
                                                {
                                                    indirectMessages[i].DestAddress.DestIndex[j] = 0xFF;
                                                    for(j = 0; j < indirectMessages[i].PayLoadSize; j++)
                                                    {
                                                        MiApp_WriteData(indirectMessages[i].PayLoad[j]);
                                                    }
                                                    #if defined(ENABLE_TIME_SYNC)
                                                        if( indirectMessages[i].flags.bits.isCommand )
                                                        {
                                                            TxBuffer[0] = CMD_TIME_SYNC_COMMAND_PACKET;
                                                        } 
                                                    #endif   
                                                    #if defined(IEEE_802_15_4)
                                                        SendPacket(FALSE, indirectMessages[i].DestPANID, rxMessage.SourceAddress, isCommand, indirectMessages[i].flags.bits.isSecured);
                                                    #else
                                                        SendPacket(FALSE, rxMessage.SourceAddress, isCommand, indirectMessages[i].flags.bits.isSecured);
                                                    #endif 
                                                    //goto DiscardPacketHere;
                                                    goto END_OF_SENDING_INDIRECT_MESSAGE;
                                                }
                                            }         
                                        }
                                        else 
                                    #endif
                                    if( isSameAddress(indirectMessages[i].DestAddress.DestLongAddress, rxMessage.SourceAddress) )
                                    {                          
                                        for(j = 0; j < indirectMessages[i].PayLoadSize; j++)
                                        {
                                            MiApp_WriteData(indirectMessages[i].PayLoad[j]);
                                        }
                                        #if defined(ENABLE_TIME_SYNC)
                                            if( indirectMessages[i].flags.bits.isCommand )
                                            {
                                                TxBuffer[0] = CMD_TIME_SYNC_COMMAND_PACKET;
                                            } 
                                        #endif 
                                        #if defined(IEEE_802_15_4)
                                            SendPacket(FALSE, indirectMessages[i].DestPANID, indirectMessages[i].DestAddress.DestLongAddress, isCommand, (BOOL)indirectMessages[i].flags.bits.isSecured);
                                        #else
                                            SendPacket(FALSE, indirectMessages[i].DestAddress.DestLongAddress, isCommand, (BOOL)indirectMessages[i].flags.bits.isSecured);
                                        #endif
                                        indirectMessages[i].flags.Val = 0;   
                                        goto END_OF_SENDING_INDIRECT_MESSAGE;    
                                    }    
                                }
                            }
                           
                            if( i == INDIRECT_MESSAGE_SIZE )
                            {
                                #ifdef TARGET_SMALL
                                    #if defined(IEEE_802_15_4)
                                        SendPacket(FALSE, myPANID, rxMessage.SourceAddress, isCommand, FALSE);
                                    #else
                                        SendPacket(FALSE, rxMessage.SourceAddress, isCommand, FALSE);
                                    #endif
                                #else
                                    #if defined(IEEE_802_15_4)
                                        SendPacket(FALSE, rxMessage.SourcePANID, rxMessage.SourceAddress, isCommand, FALSE);
                                    #else
                                        SendPacket(FALSE, rxMessage.SourceAddress, isCommand, FALSE);
                                    #endif
                                #endif
                            }
                            
END_OF_SENDING_INDIRECT_MESSAGE:
                            #if defined(ENABLE_ENHANCED_DATA_REQUEST)
                                if( MACRxPacket.PayloadLen > 1 )
                                {
                                    rxMessage.Payload = &(MACRxPacket.Payload[1]);
                                    rxMessage.PayloadSize--;
                                    P2PStatus.bits.RxHasUserData = 1;
                                }
                                else    
                            #endif                        
                            MiMAC_DiscardPacket();
                        }    
                        break;
                #endif
                
                
                #if defined(ENABLE_TIME_SYNC) && defined(ENABLE_SLEEP)
                    case CMD_TIME_SYNC_DATA_PACKET:
                    case CMD_TIME_SYNC_COMMAND_PACKET:
                        {
                            WakeupTimes.v[0] = rxMessage.Payload[1];
                            WakeupTimes.v[1] = rxMessage.Payload[2];
                            CounterValue.v[0] = rxMessage.Payload[3];
                            CounterValue.v[1] = rxMessage.Payload[4];

                            if( rxMessage.PayloadSize > 5 )
                            {
                                if( rxMessage.Payload[0] == CMD_TIME_SYNC_DATA_PACKET )
                                {
                                    rxMessage.flags.bits.command = 0;
                                }    
                                rxMessage.PayloadSize -= 5;
                                rxMessage.Payload = &(rxMessage.Payload[5]);
                                P2PStatus.bits.RxHasUserData = 1;
                            }  
                            else
                            {
                                P2PStatus.bits.DataRequesting = 0;
                                MiMAC_DiscardPacket();
                            }      
                        }
                        break;    
                #endif
                
                
                     
                #if defined(ENABLE_FREQUENCY_AGILITY) 
                    case CMD_CHANNEL_HOPPING:
                        if( rxMessage.Payload[1] != currentChannel )
                        {
                            MiMAC_DiscardPacket();
                            break;
                        }
                        StartChannelHopping(rxMessage.Payload[2]);
                        Printf("\r\nHopping Channel to ");
                        PrintDec(currentChannel);
                        MiMAC_DiscardPacket();
                        break;
                    
                #endif
                                          
                default:
                    // let upper application layer to handle undefined command frame
                    P2PStatus.bits.RxHasUserData = 1;
                    break;
            }
        }
        else
        {
            P2PStatus.bits.RxHasUserData = 1;
        }

        #ifdef ENABLE_SLEEP
            if( P2PStatus.bits.DataRequesting && P2PStatus.bits.RxHasUserData )
            {
                P2PStatus.bits.DataRequesting = 0;
            }
        #endif
        
        if( rxMessage.PayloadSize == 0 || P2PStatus.bits.SearchConnection || P2PStatus.bits.Resync )
        {
            P2PStatus.bits.RxHasUserData = 0;
            MiMAC_DiscardPacket();
        }   
    }   
}


BOOL MiApp_ProtocolInit(BOOL bNetworkFreezer)
{
    BYTE i;
    
    MACINIT_PARAM initValue;
    
    #if defined(ENABLE_NVM)
        #if defined(ENABLE_NVM_MAC)
            if( MY_ADDRESS_LENGTH > 6 )
            {
                for(i = 0; i < 3; i++)
                {
                    EEPROMRead(&(myLongAddress[MY_ADDRESS_LENGTH-1-i]), EEPROM_MAC_ADDR+i, 1);
                }
                myLongAddress[4] = 0xFF;
                if( MY_ADDRESS_LENGTH > 7 )
                {
                    myLongAddress[3] = 0xFE;
                }
                for(i = 0; i < 3; i++)
                {
                    EEPROMRead(&(myLongAddress[2-i]), EEPROM_MAC_ADDR+3+i, 1);
                }
            }
            else
            {
                for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                {
                    EEPROMRead(&(myLongAddress[MY_ADDRESS_LENGTH-1-i]), EEPROM_MAC_ADDR+i, 1);
                }   
            }
        #endif
        
    #endif
    
    #if defined(ENABLE_NETWORK_FREEZER)
        NVMInit();
    #endif
    
    //clear all status bits
    P2PStatus.Val = 0;

    for(i = 0; i < CONNECTION_SIZE; i++)
    {
        ConnectionTable[i].status.Val = 0;
    }
 
    InitSymbolTimer();
    
    TxData = 0;

    #ifdef ENABLE_INDIRECT_MESSAGE
        for(i = 0; i < INDIRECT_MESSAGE_SIZE; i++)
        {
            indirectMessages[i].flags.Val = 0;
        }
    #endif   
        
    #if defined(ENABLE_SECURITY)
        for(i = 0; i < CONNECTION_SIZE; i++)
        {
            IncomingFrameCounter[i].Val = 0;
        }
    #endif
    
    #if defined(ENABLE_NETWORK_FREEZER)
        if( bNetworkFreezer )
        {
            nvmGetCurrentChannel(&currentChannel);
            if( currentChannel >= 32 )
            {
                return FALSE;
            }
            
            #if defined(IEEE_802_15_4)
                nvmGetMyPANID(myPANID.v);
            #endif
            nvmGetConnMode(&ConnMode);
            nvmGetConnectionTable(ConnectionTable);
                        
            #if defined(IEEE_802_15_4)            
            Printf("\r\nPANID:");
            PrintChar(myPANID.v[1]);
            PrintChar( myPANID.v[0]);
            #endif
            Printf(" Channel:");
            PrintChar(currentChannel);
        }
        else
        {
            #if defined(IEEE_802_15_4)
                myPANID.Val = MY_PAN_ID;
                nvmPutMyPANID(myPANID.v);
            #endif
            nvmPutCurrentChannel(&currentChannel);
            nvmPutConnMode(&ConnMode);
            nvmPutConnectionTable(ConnectionTable);
        }
    #else
        #if defined(IEEE_802_15_4)
            myPANID.Val = MY_PAN_ID; 
        #endif
    #endif
    
    initValue.PAddress = myLongAddress;
    initValue.actionFlags.bits.CCAEnable = 1;
    initValue.actionFlags.bits.PAddrLength = MY_ADDRESS_LENGTH;
    initValue.actionFlags.bits.NetworkFreezer = bNetworkFreezer;
    initValue.actionFlags.bits.RepeaterMode = 0;

    MiMAC_Init(initValue);
    
    #if defined(IEEE_802_15_4)
        {
            WORD tmp = 0xFFFF;
            MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);
        }
    #endif
    
    MiApp_SetChannel(currentChannel);

    #if defined(ENABLE_TIME_SYNC)
        #if defined(ENABLE_SLEEP)
            WakeupTimes.Val = 0;
            CounterValue.Val = 61535;   // (0xFFFF - 4000) one second
        #elif defined(ENABLE_INDIRECT_MESSAGE)
            TimeSlotTick.Val = ((ONE_SECOND) * RFD_WAKEUP_INTERVAL) / TIME_SYNC_SLOTS;
        #endif
    #endif

    P2PCapacityInfo = 0;
    #if !defined(ENABLE_SLEEP)
        P2PCapacityInfo |= 0x01;
    #endif
    #if defined(ENABLE_SECURITY)
        P2PCapacityInfo |= 0x08;
    #endif
    P2PCapacityInfo |= (ConnMode << 4);

    RFIE = 1;
    return TRUE;
}

#ifdef ENABLE_SLEEP
    /************************************************************************************
     * Function:
     *      BYTE    MiApp_TransceiverPowerState(BYTE Mode)
     *
     * Summary:
     *      This function put the RF transceiver into different power state. i.e. Put the 
     *      RF transceiver into sleep or wake it up.
     *
     * Description:        
     *      This is the primary user interface functions for the application layer to 
     *      put RF transceiver into sleep or wake it up. This function is only available
     *      to those wireless nodes that may have to disable the transceiver to save 
     *      battery power.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters: 
     *      BYTE Mode - The mode of power state for the RF transceiver to be set. The possible
     *                  power states are following
     *                  * POWER_STATE_SLEEP     The deep sleep mode for RF transceiver
     *                  * POWER_STATE_WAKEUP    Wake up state, or operating state for RF transceiver
     *                  * POWER_STATE_WAKEUP_DR Put device into wakeup mode and then transmit a 
     *                                          data request to the device's associated device
     *
     * Returns: 
     *      The status of the operation. The following are the possible status
     *      * SUCCESS           Operation successful
     *      * ERR_TRX_FAIL      Transceiver fails to go to sleep or wake up
     *      * ERR_TX_FAIL       Transmission of Data Request command failed. Only available if the
     *                          input mode is POWER_STATE_WAKEUP_DR.
     *      * ERR_RX_FAIL       Failed to receive any response to Data Request command. Only available
     *                          if input mode is POWER_STATE_WAKEUP_DR.
     *      * ERR_INVLAID_INPUT Invalid input mode. 
     *
     * Example:
     *      <code>
     *      // put RF transceiver into sleep
     *      MiApp_TransceiverPowerState(POWER_STATE_SLEEP;
     *
     *      // Put the MCU into sleep
     *      Sleep();    
     *
     *      // wakes up the MCU by WDT, external interrupt or any other means
     *      
     *      // make sure that RF transceiver to wake up and send out Data Request
     *      MiApp_TransceiverPowerState(POWER_STATE_WAKEUP_DR);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/
    BYTE MiApp_TransceiverPowerState(INPUT BYTE Mode)
    {
        //BYTE status;
        
        switch(Mode)
        {
            case POWER_STATE_SLEEP:
                {
                    #if defined(ENABLE_NETWORK_FREEZER)
                        if( P2PStatus.bits.SaveConnection )
                        {
                            nvmPutConnectionTable(ConnectionTable);
                            P2PStatus.bits.SaveConnection = 0;
                        }
                    #endif
                    if( MiMAC_PowerState(POWER_STATE_DEEP_SLEEP) )
                    {
                        P2PStatus.bits.Sleeping = 1;
                        return SUCCESS;
                    }
                    return ERR_TRX_FAIL;
                }
                
            case POWER_STATE_WAKEUP:
                {
                    if( MiMAC_PowerState(POWER_STATE_OPERATE) )
                    {
                        P2PStatus.bits.Sleeping = 0;
                        return SUCCESS;
                    }
                    return ERR_TRX_FAIL;
                }
               
            case POWER_STATE_WAKEUP_DR:
                {
                    if( FALSE == MiMAC_PowerState(POWER_STATE_OPERATE) )
                    {
                        return ERR_TRX_FAIL;
                    }
                    P2PStatus.bits.Sleeping = 0;
                    if( CheckForData() == FALSE )
                    {
                        return ERR_TX_FAIL;
                    }
                    while( P2PStatus.bits.DataRequesting ) 
                    {
                        P2PTasks();
                    }
                    return SUCCESS;
                }
                
             default:
                break;

        }
        
        return ERR_INVALID_INPUT;    
    }

     
     /*********************************************************************
     * BOOL CheckForData(void)
     *
     * Overview:        This function sends out a Data Request to the peer
     *                  device of the first P2P connection. 
     *
     * PreCondition:    Transceiver is initialized and fully waken up
     *
     * Input:           None
     *
     * Output:          None
     *
     * Side Effects:    The P2P stack is waiting for the response from
     *                  the peer device. A data request timer has been
     *                  started. In case there is no response from the
     *                  peer device, the data request will time-out itself
     *
     ********************************************************************/
     BOOL CheckForData(void)
     {
        BYTE tmpTxData = TxData;
        BYTE firstByte = TxBuffer[0];
        
        MiApp_FlushTx();
        MiApp_WriteData(CMD_MAC_DATA_REQUEST);

        #if defined(ENABLE_ENHANCED_DATA_REQUEST)
            if( tmpTxData > 0 )
            {
                BYTE i;
                
                for( i = tmpTxData; i > 1; i--)
                {
                    TxBuffer[i] = TxBuffer[i-1];
                }
                TxBuffer[1] = firstByte;  
                TxData = tmpTxData + 1;  
            }    
        #endif
                    
        #if defined(IEEE_802_15_4)
            #if defined(ENABLE_ENHANCED_DATA_REQUEST)
                if( SendPacket(FALSE, myPANID, ConnectionTable[0].Address, TRUE, P2PStatus.bits.Enhanced_DR_SecEn) )
            #else
                if( SendPacket(FALSE, myPANID, ConnectionTable[0].Address, TRUE, FALSE) )
            #endif
        #else
            #if defined(ENABLE_ENHANCED_DATA_REQUEST)
                if( SendPacket(FALSE, ConnectionTable[0].Address, TRUE, P2PStatus.bits.Enhanced_DR_SecEn) )
            #else
                if( SendPacket(FALSE, ConnectionTable[0].Address, TRUE, FALSE) )
            #endif
        #endif
        {
            P2PStatus.bits.DataRequesting = 1; 
            #if defined(ENABLE_ENHANCED_DATA_REQUEST) 
                P2PStatus.bits.Enhanced_DR_SecEn = 0;
            #endif
            DataRequestTimer = MiWi_TickGet();
            TxBuffer[0] = firstByte;
            TxData = tmpTxData;
            #if defined(ENABLE_TIME_SYNC)
                #if defined(__18CXX)
                    TMR3H = 0;
                    TMR3L = 0;
                #elif defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__)
                    PR1 = 0xFFFF;
                    TMR1 = 0;
                #elif defined(__PIC32MX__)
                    PR1 = 0xFFFF;
                    while(T1CONbits.TWIP) ;
                    TMR1 = 0;
                #endif
            #endif
            return TRUE;
        }
        TxBuffer[0] = firstByte;
        TxData = tmpTxData;
        #if defined(ENABLE_ENHANCED_DATA_REQUEST)
            P2PStatus.bits.Enhanced_DR_SecEn = 0;
        #endif
        return FALSE;
     }
 #endif
 

#ifdef ENABLE_INDIRECT_MESSAGE
    
    /*********************************************************************
     * BOOL IndirectPacket(BOOL Broadcast, 
     *                     WORD_VAL DestinationPANID, 
     *                     BYTE *DestinationAddress, 
     *                     BOOL isCommand, 
     *                     BOOL SecurityEnabled)
     *
     * Overview:        This function store the indirect message for node
     *                  that turns off radio when idle     
     *
     * PreCondition:    None
     *
     * Input:           Broadcast           - Boolean to indicate if the indirect
     *                                        message a broadcast message
     *                  DestinationPANID    - The PAN Identifier of the 
     *                                        destination node
     *                  DestinationAddress  - The pointer to the destination
     *                                        long address
     *                  isCommand           - The boolean to indicate if the packet
     *                                        is command
     *                  SecurityEnabled     - The boolean to indicate if the 
     *                                        packet needs encryption
     *
     * Output:          boolean to indicate if operation successful
     *
     * Side Effects:    An indirect message stored and waiting to deliever
     *                  to sleeping device. An indirect message timer
     *                  has started to expire the indirect message in case
     *                  RFD does not acquire data in predefined interval
     *
     ********************************************************************/
    #if defined(IEEE_802_15_4)
        BOOL IndirectPacket(INPUT BOOL Broadcast, 
                            INPUT WORD_VAL DestinationPANID, 
                            INPUT BYTE *DestinationAddress, 
                            INPUT BOOL isCommand, 
                            INPUT BOOL SecurityEnabled)
    #else
        BOOL IndirectPacket(INPUT BOOL Broadcast, 
                            INPUT BYTE *DestinationAddress, 
                            INPUT BOOL isCommand, 
                            INPUT BOOL SecurityEnabled)
    #endif                            
    { 
        BYTE i;
        
        #ifndef ENABLE_BROADCAST
            if( Broadcast )
            {
                return FALSE;
            }
        #endif

        // loop through the available indirect message buffer and locate
        // the empty message slot
        for(i = 0; i < INDIRECT_MESSAGE_SIZE; i++)
        {
            if( indirectMessages[i].flags.bits.isValid == 0 )
            {
                BYTE j;
                
                // store the message
                indirectMessages[i].flags.bits.isValid          = TRUE;
                indirectMessages[i].flags.bits.isBroadcast      = Broadcast;
                indirectMessages[i].flags.bits.isCommand        = isCommand;
                indirectMessages[i].flags.bits.isSecured        = SecurityEnabled;
                #if defined(IEEE_802_15_4)
                    indirectMessages[i].DestPANID.Val           = DestinationPANID.Val;
                #endif
                if( DestinationAddress != NULL )
                {
                    for(j = 0; j < MY_ADDRESS_LENGTH; j++)
                    {
                        indirectMessages[i].DestAddress.DestLongAddress[j] = DestinationAddress[j];
                    }
                }
                #ifdef ENABLE_BROADCAST
                    else
                    {
                        BYTE k = 0;
    
                        for(j = 0; j < CONNECTION_SIZE; j++)
                        {
                            //if( (ConnectionTable[j].PeerInfo[0] & 0x83) == 0x82 )
                            if( ConnectionTable[j].status.bits.isValid &&
                                ConnectionTable[j].status.bits.RXOnWhenIdle == 0 )
                            {
                                indirectMessages[i].DestAddress.DestIndex[k++] = j;
                            }
                        }
                        for(; k < CONNECTION_SIZE; k++)
                        {
                            indirectMessages[i].DestAddress.DestIndex[k] = 0xFF;
                        }
                    }
                #endif
                
                indirectMessages[i].PayLoadSize = TxData;
                for(j = 0; j < TxData; j++)
                {
                    indirectMessages[i].PayLoad[j] = TxBuffer[j];
                }
                indirectMessages[i].TickStart = MiWi_TickGet();
                return TRUE;
            }
        }
        return FALSE;
    }
#endif


/*********************************************************************
 * BOOL SendPacket(BOOL Broadcast, 
 *                 WORD_VAL DestinationPANID, 
 *                 BYTE *DestinationAddress, 
 *                 BOOL isCommand, 
 *                 BOOL SecurityEnabled)
 *
 * Overview:        This function sends the packet  
 *
 * PreCondition:    Transceiver is initialized
 *
 * Input:     
 *          BOOL        Broadcast           If packet to send needs to be broadcast
 *          WORD_VAL    DestinationPANID    Destination PAN Identifier
 *          BYTE *      DestinationAddress  Pointer to destination long address 
 *          BOOL        isCommand           If packet to send is a command packet
 *          BOOL        SecurityEnabled     If packet to send needs encryption
 *                  
 * Output: 
 *          BOOL                            If operation successful
 *
 * Side Effects:    Transceiver is triggered to transmit a packet
 *
 ********************************************************************/
#if defined(IEEE_802_15_4)
    BOOL SendPacket(INPUT BOOL Broadcast, 
                    INPUT WORD_VAL DestinationPANID, 
                    INPUT BYTE *DestinationAddress, 
                    INPUT BOOL isCommand, 
                    INPUT BOOL SecurityEnabled)
#else
    BOOL SendPacket(INPUT BOOL Broadcast, 
                    INPUT BYTE *DestinationAddress, 
                    INPUT BOOL isCommand, 
                    INPUT BOOL SecurityEnabled)
#endif                                        
{ 
    MAC_TRANS_PARAM tParam;
    BOOL status;

    tParam.flags.Val = 0;
    tParam.flags.bits.packetType = (isCommand) ? PACKET_TYPE_COMMAND : PACKET_TYPE_DATA;
    tParam.flags.bits.ackReq = (Broadcast) ? 0 : 1;
    tParam.flags.bits.broadcast = Broadcast;
    tParam.flags.bits.secEn = SecurityEnabled;
    #if defined(IEEE_802_15_4)
        tParam.altSrcAddr = 0;
        tParam.altDestAddr = (Broadcast) ? TRUE : FALSE;
    #endif
    
    #if defined(INFER_DEST_ADDRESS)
        tParam.flags.bits.destPrsnt = 0;
    #else
        tParam.flags.bits.destPrsnt = (Broadcast) ? 0:1;
    #endif
    
    #if defined(SOURCE_ADDRESS_ABSENT)
        if( tParam.flags.bits.packetType == PACKET_TYPE_COMMAND )
        {
            tParam.flags.bits.sourcePrsnt = 1;
        }
        else
        {
            tParam.flags.bits.sourcePrsnt = 0;
        }
    #else
        tParam.flags.bits.sourcePrsnt = 1;
    #endif
    
    tParam.DestAddress = DestinationAddress;

    #if defined(IEEE_802_15_4)
        tParam.DestPANID.Val = DestinationPANID.Val;
    #endif

    status = MiMAC_SendPacket(tParam, TxBuffer, TxData); 

    TxData = 0;
    
    return status;
}

/************************************************************************************
 * Function:
 *      BOOL MiApp_BroadcastPacket(BOOL SecEn )
 *
 * Summary:
 *      This function broadcast a message in the TxBuffer.
 *
 * Description:        
 *      This is the primary user interface function for the application layer to 
 *      broadcast a message. The application payload is filled in the global char
 *      array TxBuffer.
 *
 * PreCondition:    
 *      Protocol initialization has been done. 
 *
 * Parameters:           
 *      BOOL SecEn -    The boolean indicates if the application payload needs to be
 *                      secured before transmission.
 *
 * Returns: 
 *      A boolean to indicates if the broadcast procedure is succcessful.
 *
 * Example:
 *      <code>
 *      // Secure and then broadcast the message stored in TxBuffer
 *      MiApp_BroadcastPacket(TRUE);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BOOL MiApp_BroadcastPacket( INPUT BOOL SecEn )                                                     
{
    #ifdef ENABLE_INDIRECT_MESSAGE
        BYTE i;
        
        for(i = 0; i < CONNECTION_SIZE; i++)
        {
            if( ConnectionTable[i].status.bits.isValid && ConnectionTable[i].status.bits.RXOnWhenIdle == 0 )
            {
                #if defined(IEEE_802_15_4)
                    IndirectPacket(TRUE, myPANID, NULL, FALSE, SecEn);
                #else
                    IndirectPacket(TRUE, NULL, FALSE, SecEn);
                #endif
                break;
            }     
        }
    #endif
    
    #if defined(ENABLE_ENHANCED_DATA_REQUEST) && defined(ENABLE_SLEEPING)
        if( P2PStatus.bits.Sleeping )
        {
            P2PStatus.bits.Enhanced_DR_SecEn = SecEn;
            return TRUE;
        }    
    #endif
    
    #if defined(IEEE_802_15_4)
        return SendPacket(TRUE, myPANID, NULL, FALSE, SecEn);
    #else
        return SendPacket(TRUE, NULL, FALSE, SecEn);
    #endif
}

/************************************************************************************
 * Function:
 *      BOOL MiApp_UnicastConnection(BYTE ConnectionIndex, BOOL SecEn)
 *
 * Summary:
 *      This function unicast a message in the TxBuffer to the device with the input 
 *      ConnectionIndex in the connection table. 
 *
 * Description:        
 *      This is one of the primary user interface functions for the application layer to 
 *      unicast a message. The destination device is in the connection table specified by 
 *      the input parameter ConnectionIndex. The application payload is filled in the 
 *      global char array TxBuffer.
 *
 * PreCondition:    
 *      Protocol initialization has been done. The input parameter ConnectionIndex points to
 *      a valid peer device in the connection table.
 *
 * Parameters: 
 *      BYTE ConnectionIndex -  The index of the destination device in the connection table.          
 *      BOOL SecEn -    The boolean indicates if the application payload needs to be
 *                      secured before transmission.
 *
 * Returns: 
 *      A boolean to indicates if the unicast procedure is succcessful.
 *
 * Example:
 *      <code>
 *      // Secure and then unicast the message stored in TxBuffer to the first device in 
 *      // the connection table
 *      MiApp_UnicastConnection(0, TRUE);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/  
BOOL MiApp_UnicastConnection( INPUT BYTE ConnectionIndex, 
                        INPUT BOOL SecEn)
{
    if( ConnectionTable[ConnectionIndex].status.bits.isValid )
    {
        #ifdef ENABLE_INDIRECT_MESSAGE
            // check if RX on when idle
            //if( (ConnectionTable[ConnectionIndex].PeerInfo[0] & 0x03) == 0x02 )
            if( ConnectionTable[ConnectionIndex].status.bits.RXOnWhenIdle == 0 )
            {
                #if defined(IEEE_802_15_4)
                    return IndirectPacket(FALSE, myPANID, ConnectionTable[ConnectionIndex].Address, FALSE, SecEn);
                #else
                    // Define the additional step to assign the address to
                    // bypass compiler bug in C18 v3.38.
                    BYTE address[MY_ADDRESS_LENGTH];
                    BYTE i;

                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        address[i] = ConnectionTable[ConnectionIndex].Address[i];  
                    }
                    
                    //return IndirectPacket(FALSE, ConnectionTable[ConnectionIndex].Address, FALSE, SecEn);
                    return IndirectPacket(FALSE, address, FALSE, SecEn);
                #endif
            }
        #endif
        
        #if defined(ENABLE_ENHANCED_DATA_REQUEST) 
            if( P2PStatus.bits.Sleeping )
            {
                P2PStatus.bits.Enhanced_DR_SecEn = SecEn;
                return TRUE;
            }    
        #endif
        
        return MiApp_UnicastAddress(ConnectionTable[ConnectionIndex].Address, TRUE, SecEn);
    }
    return FALSE;
}

/************************************************************************************
 * Function:
 *      BOOL MiApp_UnicastAddress(BYTE *DestinationAddress, BOOL PermanentAddr, BOOL SecEn)
 *
 * Summary:
 *      This function unicast a message in the TxBuffer to the device with DestinationAddress 
 *
 * Description:        
 *      This is one of the primary user interface functions for the application layer to 
 *      unicast a message. The destination device is specified by the input parameter 
 *      DestinationAddress. The application payload is filled in the global char array TxBuffer.
 *
 * PreCondition:    
 *      Protocol initialization has been done. 
 *
 * Parameters: 
 *      BYTE * DestinationAddress - The destination address of the unicast
 *      BOOL PermanentAddr -    The boolean to indicate if the destination address above is a
 *                              permanent address or alternative network address. This parameter
 *                              is only used in a network protocol.         
 *      BOOL SecEn -    The boolean indicates if the application payload needs to be
 *                      secured before transmission.
 *
 * Returns: 
 *      A boolean to indicates if the unicast procedure is succcessful.
 *
 * Example:
 *      <code>
 *      // Secure and then broadcast the message stored in TxBuffer to the permanent address
 *      // specified in the input parameter.
 *      MiApp_UnicastAddress(DestAddress, TRUE, TRUE);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/    
BOOL MiApp_UnicastAddress(INPUT BYTE *DestinationAddress, 
                    INPUT BOOL PermanentAddr, 
                    INPUT BOOL SecEn)                          
{
    #ifdef ENABLE_INDIRECT_MESSAGE
        BYTE i;
        
        for(i = 0; i < CONNECTION_SIZE; i++)
        {
            // check if RX on when idle
            if( ConnectionTable[i].status.bits.isValid && (ConnectionTable[i].status.bits.RXOnWhenIdle == 0) &&
                isSameAddress(DestinationAddress, ConnectionTable[i].Address) )
            {
                #if defined(IEEE_802_15_4)
                    return IndirectPacket(FALSE, myPANID, DestinationAddress, FALSE, SecEn);
                #else
                    return IndirectPacket(FALSE, DestinationAddress, FALSE, SecEn);
                #endif
            }
        }
    #endif
    
    #if defined(ENABLE_ENHANCED_DATA_REQUEST) && defined(ENABLE_SLEEPING)
        if( P2PStatus.bits.Sleeping )
        {
            P2PStatus.bits.Enhanced_DR_SecEn = SecEn;
            return TRUE;
        }    
    #endif
    
    #if defined(IEEE_802_15_4)
        return SendPacket(FALSE, myPANID, DestinationAddress, FALSE, SecEn);    
    #else
        return SendPacket(FALSE, DestinationAddress, FALSE, SecEn);
    #endif
}



/*********************************************************************
 * BOOL    isSameAddress(BYTE *Address1, BYTE *Address2)
 *
 * Overview:        This function compares two long addresses and returns
 *                  the boolean to indicate if they are the same
 *
 * PreCondition:    
 *
 * Input:  
 *          Address1    - Pointer to the first long address to be compared
 *          Address2    - Pointer to the second long address to be compared
 *                  
 * Output: 
 *          If the two address are the same
 *
 * Side Effects:    
 *
 ********************************************************************/
BOOL    isSameAddress(INPUT BYTE *Address1, INPUT BYTE *Address2)
{
    BYTE i;
    
    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
    {
        if( Address1[i] != Address2[i] )
        {
            return FALSE;
        }
    }
    return TRUE;
}

#if defined(ENABLE_HAND_SHAKE)
     
    BOOL MiApp_StartConnection(BYTE Mode, BYTE ScanDuration, DWORD ChannelMap)
    {
        switch(Mode)
        {
            case START_CONN_DIRECT:
                #if defined(IEEE_802_15_4)
                    #if MY_PAN_ID == 0xFFFF
                        myPANID.v[0] = TMRL;
                        myPANID.v[1] = TMRL+0x51;
                    #else
                        myPANID.Val = MY_PAN_ID;
                    #endif
                    {
                        WORD tmp = 0xFFFF;
                        MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);
                    }
                #endif
                #if defined(ENABLE_TIME_SYNC) && !defined(ENABLE_SLEEP) && defined(ENABLE_INDIRECT_MESSAGE)
                    TimeSyncTick = MiWi_TickGet();
                #endif
                return TRUE;
                
            case START_CONN_ENERGY_SCN:
                #if defined(ENABLE_ED_SCAN)
                {
                    BYTE channel;
                    BYTE RSSIValue;
                    
                    #if defined(IEEE_802_15_4)
                        #if MY_PAN_ID == 0xFFFF
                            myPANID.v[0] = TMRL;
                            myPANID.v[1] = TMRL+0x51;
                        #else
                            myPANID.Val = MY_PAN_ID;
                        #endif
                        {
                            WORD tmp = 0xFFFF;
                            MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);
                        }
                    #endif
                    channel = MiApp_NoiseDetection(ChannelMap, ScanDuration, NOISE_DETECT_ENERGY, &RSSIValue);
                    MiApp_SetChannel(channel);
                    Printf("\r\nStart Wireless Communication on Channel ");
                    PrintDec(channel);
                    Printf("\r\n");
                    #if defined(ENABLE_TIME_SYNC) && !defined(ENABLE_SLEEP) && defined(ENABLE_INDIRECT_MESSAGE)
                        TimeSyncTick = MiWi_TickGet();
                    #endif
                    return TRUE;
                }
                #else
                    return FALSE;
                #endif
                
            case START_CONN_CS_SCN:
                // Carrier sense scan is not supported for current available transceivers
                return FALSE;         
            
            default:
                break;
        }    
        return FALSE;
    }
    
    /************************************************************************************
     * Function:
     *      BYTE    MiApp_EstablishConnection(BYTE ActiveScanIndex, BYTE Mode)
     *
     * Summary:
     *      This function establish a connection with one or more nodes in an existing
     *      PAN.
     *
     * Description:        
     *      This is the primary user interface function for the application layer to 
     *      start communication with an existing PAN. For P2P protocol, this function
     *      call can establish one or more connections. For network protocol, this 
     *      function can be used to join the network, or establish a virtual socket
     *      connection with a node out of the radio range. There are multiple ways to
     *      establish connection(s), all depends on the input parameters.
     *
     * PreCondition:    
     *      Protocol initialization has been done. If only to establish connection with
     *      a predefined device, an active scan must be performed before and valid active
     *      scan result has been saved.
     *
     * Parameters:           
     *      BYTE ActiveScanIndex -  The index of the target device in the ActiveScanResults
     *                              array, if a predefined device is targeted. If the 
     *                              value of ActiveScanIndex is 0xFF, the protocol stack will
     *                              try to establish a connection with any device.
     *      BYTE Mode - The mode to establish a connection. This parameter is generally valid in
     *                  a network protocol. The possible modes are:
     *                  * CONN_MODE_DIRECT      Establish a connection without radio range.
     *                  * CONN_MODE_INDIRECT    Establish a virtual connection with a device 
     *                                          that may be in or out of the radio range. This
     *                                          mode sometimes is called cluster socket, which
     *                                          is only valid for network protocol. The PAN 
     *                                          Coordinator will be involved to establish a 
     *                                          virtual indirect socket connection.
     *                  
     * Returns: 
     *      The index of the peer device on the connection table.
     *
     * Example:
     *      <code>
     *      // Establish one or more connections with any device
     *      PeerIndex = MiApp_EstablishConnection(0xFF, CONN_MODE_DIRECT);
     *      </code>
     *
     * Remarks:    
     *      If more than one connections have been established through this function call, the
     *      return value points to the index of one of the peer devices.
     *
     *****************************************************************************************/  
    BYTE    MiApp_EstablishConnection(INPUT BYTE ActiveScanIndex, INPUT BYTE Mode)
    {
        BYTE    tmpConnectionMode = ConnMode;
        BYTE    retry = CONNECTION_RETRY_TIMES;
        BYTE    connectionInterval = 0;
        MIWI_TICK    t1, t2;
        if( Mode == CONN_MODE_INDIRECT )
        {
            return 0xFF;
        }
       
        t1 = MiWi_TickGet();
        t1.Val -= (ONE_SECOND);
        ConnMode = ENABLE_ALL_CONN;
        P2PStatus.bits.SearchConnection = 1;
        while( P2PStatus.bits.SearchConnection )
        {
            t2 = MiWi_TickGet();
            if( MiWi_TickGetDiff(t2, t1) > (ONE_SECOND) )
            {   
                t1 = t2;
                
                if( connectionInterval-- > 0 )
                {
                    continue;
                }
                connectionInterval = CONNECTION_INTERVAL-1;
                if( retry-- == 0 )
                {
                    P2PStatus.bits.SearchConnection = 0;
                    return 0xFF;
                }
                MiApp_FlushTx();
                MiApp_WriteData(CMD_P2P_CONNECTION_REQUEST);
                MiApp_WriteData(currentChannel);
                MiApp_WriteData(P2PCapacityInfo);
                #if ADDITIONAL_NODE_ID_SIZE > 0
                    {
                        BYTE i;
                        
                        for(i = 0; i < ADDITIONAL_NODE_ID_SIZE; i++)
                        {
                            MiApp_WriteData(AdditionalNodeID[i]);
                        }
                    }
                #endif

                #if defined(IEEE_802_15_4)
                    #if defined(ENABLE_ACTIVE_SCAN)
                        if( ActiveScanIndex == 0xFF )
                        {
                            SendPacket(TRUE, myPANID, NULL, TRUE, FALSE);
                        }
                        else
                        {
                            MiApp_SetChannel( ActiveScanResults[ActiveScanIndex].Channel );
                            SendPacket(FALSE, ActiveScanResults[ActiveScanIndex].PANID, ActiveScanResults[ActiveScanIndex].Address, TRUE, FALSE);
                        }
                    #else
                        SendPacket(TRUE, myPANID, NULL, TRUE, FALSE);
                    #endif
                #else
                    #if defined(ENABLE_ACTIVE_SCAN)
                        if( ActiveScanIndex == 0xFF )
                        {
                            SendPacket(TRUE, NULL, TRUE, FALSE);
                        }
                        else
                        {
                            MiApp_SetChannel( ActiveScanResults[ActiveScanIndex].Channel );
                            SendPacket(FALSE, ActiveScanResults[ActiveScanIndex].Address, TRUE, FALSE);
                        }
                    #else
                        SendPacket(TRUE, NULL, TRUE, FALSE);
                    #endif
                #endif
            }
    
            if( MiApp_MessageAvailable())
            {
                MiApp_DiscardMessage();
            }     
            //P2PTasks();
        }
      
        ConnMode = tmpConnectionMode;
        
        #if defined(ENABLE_TIME_SYNC) && !defined(ENABLE_SLEEP) && defined(ENABLE_INDIRECT_MESSAGE)
            TimeSyncTick = MiWi_TickGet();
        #endif
        return LatestConnection;
        
    }


#endif



 
void MiApp_DiscardMessage(void)
{
    P2PStatus.bits.RxHasUserData = 0;
    MiMAC_DiscardPacket();
}


 
BOOL MiApp_SetChannel(BYTE channel)
{
    if( MiMAC_SetChannel(channel, 0) )
    {
        currentChannel = channel;
        #if defined(ENABLE_NETWORK_FREEZER)
            nvmPutCurrentChannel(&currentChannel);
        #endif
        return TRUE;
    }
    return FALSE;
}


BOOL MiApp_MessageAvailable(void)
{ 
    P2PTasks(); 
    
    return P2PStatus.bits.RxHasUserData;
}

#ifdef ENABLE_DUMP
    /*********************************************************************
     * void DumpConnection(BYTE index)
     *
     * Overview:        This function prints out the content of the connection 
     *                  with the input index of the P2P Connection Entry
     *
     * PreCondition:    
     *
     * Input:  
     *          index   - The index of the P2P Connection Entry to be printed out
     *                  
     * Output:  None
     *
     * Side Effects:    The content of the connection pointed by the index 
     *                  of the P2P Connection Entry will be printed out
     *
     ********************************************************************/
    void DumpConnection(INPUT BYTE index)
    {
        BYTE i, j;
        
        if( index > CONNECTION_SIZE )
        {
            Printf("\r\n\r\nMy Address: 0x");
            for(i = 0; i < MY_ADDRESS_LENGTH; i++)
            {
                PrintChar(myLongAddress[MY_ADDRESS_LENGTH-1-i]);
            }
            #if defined(IEEE_802_15_4)
                Printf("  PANID: 0x");
                PrintChar(myPANID.v[1]);
                PrintChar(myPANID.v[0]);
            #endif
            Printf("  Channel: ");
            PrintDec(currentChannel);
        }
            
        if( index < CONNECTION_SIZE )
        {
            Printf("\r\nConnection     PeerLongAddress     PeerInfo\r\n");  
            if( ConnectionTable[index].status.bits.isValid )
            {
                PrintChar(index);
                Printf("             ");
                for(i = 0; i < 8; i++)
                {
                    if(i < MY_ADDRESS_LENGTH)
                    {
                        PrintChar( ConnectionTable[index].Address[MY_ADDRESS_LENGTH-1-i] );
                    }
                    else
                    {
                        Printf("  ");
                    }
                }
                Printf("    ");
                #if ADDITIONAL_NODE_ID_SIZE > 0
                    for(i = 0; i < ADDITIONAL_NODE_ID_SIZE; i++)
                    {
                        PrintChar( ConnectionTable[index].PeerInfo[i] );
                    }
                #endif
                Printf("\r\n");
            }
        }
        else
        {
            Printf("\r\n\r\nConnection     PeerLongAddress     PeerInfo\r\n");  
            for(i = 0; i < CONNECTION_SIZE; i++)
            {
                
                if( ConnectionTable[i].status.bits.isValid )
                {
                    PrintChar(i);
                    Printf("             ");
                    for(j = 0; j < 8; j++)
                    {
                        if( j < MY_ADDRESS_LENGTH )
                        {
                            PrintChar( ConnectionTable[i].Address[MY_ADDRESS_LENGTH-1-j] );
                        }
                        else
                        {
                            Printf("  ");
                        }
                    }
                    Printf("    ");
                    #if ADDITIONAL_NODE_ID_SIZE > 0
                        for(j = 0; j < ADDITIONAL_NODE_ID_SIZE; j++)
                        {
                            PrintChar( ConnectionTable[i].PeerInfo[j] );
                        }
                    #endif
                    Printf("\r\n");
                }  
            }
        }
    }
#endif


#if defined(ENABLE_HAND_SHAKE)
    /*********************************************************************
     * BYTE AddConnection(void)
     *
     * Overview:        This function create a new P2P connection entry
     *
     * PreCondition:    A P2P Connection Request or Response has been 
     *                  received and stored in rxMessage structure
     *
     * Input:  None
     *                  
     * Output: 
     *          The index of the P2P Connection Entry for the newly added
     *          connection
     *
     * Side Effects:    A new P2P Connection Entry is created. The search 
     *                  connection operation ends if an entry is added 
     *                  successfully
     *
     ********************************************************************/
    BYTE AddConnection(void)
    {
        BYTE i;
        BYTE status = STATUS_SUCCESS;
        BYTE connectionSlot = 0xFF;
    
        // if no peerinfo attached, this is only an active scan request,
        // so do not save the source device's info
        #ifdef ENABLE_ACTIVE_SCAN
            if( rxMessage.PayloadSize < 3 )
            {
                return STATUS_ACTIVE_SCAN;
            }
        #endif
        
        // loop through all entry and locate an proper slot
        for(i = 0; i < CONNECTION_SIZE; i++)
        {
            // check if the entry is valid
            if( ConnectionTable[i].status.bits.isValid )
            {
                // check if the entry address matches source address of current received packet
                if( isSameAddress(rxMessage.SourceAddress, ConnectionTable[i].Address) )
                {
                    connectionSlot = i;
                    status = STATUS_EXISTS;
                    break;
                }
            }
            else if( connectionSlot == 0xFF )
            {
                // store the first empty slot
                connectionSlot = i;
            }  
        }
            
        if( connectionSlot == 0xFF )
        {
            return STATUS_NOT_ENOUGH_SPACE;
        }
        else 
        {
            if( ConnMode >= ENABLE_PREV_CONN )
            {
                return status;
            }
    
            // store the source address
            for(i = 0; i < 8; i++)
            {
                ConnectionTable[connectionSlot].Address[i] = rxMessage.SourceAddress[i];
            }
            
            // store the capacity info and validate the entry
            ConnectionTable[connectionSlot].status.bits.isValid = 1;
            ConnectionTable[connectionSlot].status.bits.RXOnWhenIdle = (rxMessage.Payload[2] & 0x01);
            
            // store possible additional connection payload
            #if ADDITIONAL_NODE_ID_SIZE > 0
                for(i = 0; i < ADDITIONAL_NODE_ID_SIZE; i++)
                {
                    ConnectionTable[connectionSlot].PeerInfo[i] = rxMessage.Payload[3+i];
                }
            #endif
    
            #ifdef ENABLE_SECURITY
                // if security is enabled, clear the incoming frame control
                IncomingFrameCounter[connectionSlot].Val = 0;
            #endif
            LatestConnection = connectionSlot;
            P2PStatus.bits.SearchConnection = 0;   
        } 
        return status;
    }
#endif


#ifdef ENABLE_ACTIVE_SCAN
    /************************************************************************************
     * Function:
     *      BYTE    MiApp_SearchConnection(BYTE ScanDuartion, DWORD ChannelMap)
     *
     * Summary:
     *      This function perform an active scan to locate operating PANs in the
     *      neighborhood.
     *
     * Description:        
     *      This is the primary user interface function for the application layer to 
     *      perform an active scan. After this function call, all active scan response
     *      will be stored in the global variable ActiveScanResults in the format of 
     *      structure ACTIVE_SCAN_RESULT. The return value indicates the total number
     *      of valid active scan response in the active scan result array.
     *
     * PreCondition:    
     *      Protocol initialization has been done.
     *
     * Parameters:           
     *      BYTE ScanDuration - The maximum time to perform scan on single channel. The
     *                          value is from 5 to 14. The real time to perform scan can
     *                          be calculated in following formula from IEEE 802.15.4 
     *                          specification 
     *                              960 * (2^ScanDuration + 1) * 10^(-6) second
     *      DWORD ChannelMap -  The bit map of channels to perform noise scan. The 32-bit
     *                          double word parameter use one bit to represent corresponding
     *                          channels from 0 to 31. For instance, 0x00000003 represent to 
     *                          scan channel 0 and channel 1. 
     *                  
     * Returns: 
     *      The number of valid active scan response stored in the global variable ActiveScanResults.
     *
     * Example:
     *      <code>
     *      // Perform an active scan on all possible channels
     *      NumOfActiveScanResponse = MiApp_SearchConnection(10, 0xFFFFFFFF);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/
    BYTE MiApp_SearchConnection(INPUT BYTE ScanDuration, INPUT DWORD ChannelMap)
    {
        BYTE i;
        DWORD channelMask = 0x00000001;    
        BYTE backupChannel = currentChannel;
        MIWI_TICK t1, t2;
        
        for(i = 0; i < ACTIVE_SCAN_RESULT_SIZE; i++)
        {
            ActiveScanResults[i].Channel = 0xFF;
        }
        
        ActiveScanResultIndex = 0;
        i = 0;
        while( i < 32 )
        {
            if( ChannelMap & FULL_CHANNEL_MAP & (channelMask << i) )
            {
                #if defined(IEEE_802_15_4)
                    WORD_VAL tmpPANID;
                #endif

                Printf("\r\nScan Channel ");
                PrintDec(i);
                /* choose appropriate channel */
                MiApp_SetChannel( i );
     
                MiApp_FlushTx();
                MiApp_WriteData(CMD_P2P_ACTIVE_SCAN_REQUEST);
                MiApp_WriteData(currentChannel);
                #if defined(IEEE_802_15_4)
                    tmpPANID.Val = 0xFFFF;
                    SendPacket(TRUE, tmpPANID, NULL, TRUE, FALSE);
                #else
                    SendPacket(TRUE, NULL, TRUE, FALSE);
                #endif
                
                t1 = MiWi_TickGet();
                while(1)
                {
                    if( MiApp_MessageAvailable())
                    {
                        MiApp_DiscardMessage();
                    }                     
                    //P2PTasks();
                    t2 = MiWi_TickGet();
                    if( MiWi_TickGetDiff(t2, t1) > ((DWORD)(ScanTime[ScanDuration])) )
                    {
                        // if scan time exceed scan duration, prepare to scan the next channel
                        break;
                    }
                }          
            }  
            i++;
        }
        
        MiApp_SetChannel(backupChannel);
        
        return ActiveScanResultIndex;
    }   

#endif

#ifdef ENABLE_ED_SCAN

    /************************************************************************************
     * Function:
     *      BYTE MiApp_NoiseDetection(  DWORD ChannelMap, BYTE ScanDuration, 
     *                                  BYTE DetectionMode, BYTE *NoiseLevel)
     *
     * Summary:
     *      This function perform a noise scan and returns the channel with least noise
     *
     * Description:        
     *      This is the primary user interface functions for the application layer to 
     *      perform noise detection on multiple channels.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters: 
     *      DWORD ChannelMap -  The bit map of channels to perform noise scan. The 32-bit
     *                          double word parameter use one bit to represent corresponding
     *                          channels from 0 to 31. For instance, 0x00000003 represent to 
     *                          scan channel 0 and channel 1. 
     *      BYTE ScanDuration - The maximum time to perform scan on single channel. The
     *                          value is from 5 to 14. The real time to perform scan can
     *                          be calculated in following formula from IEEE 802.15.4 
     *                          specification 
     *                              960 * (2^ScanDuration + 1) * 10^(-6) second
     *      BYTE DetectionMode -    The noise detection mode to perform the scan. The two possible
     *                              scan modes are
     *                              * NOISE_DETECT_ENERGY   Energy detection scan mode
     *                              * NOISE_DETECT_CS       Carrier sense detection scan mode
     *      BYTE *NoiseLevel -  The noise level at the channel with least noise level 
     *
     * Returns: 
     *      The channel that has the lowest noise level
     *
     * Example:
     *      <code>
     *      BYTE NoiseLevel;
     *      OptimalChannel = MiApp_NoiseDetection(0xFFFFFFFF, 10, NOISE_DETECT_ENERGY, &NoiseLevel);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/
    BYTE MiApp_NoiseDetection(INPUT DWORD ChannelMap, INPUT BYTE ScanDuration, INPUT BYTE DetectionMode, OUTPUT BYTE *RSSIValue)
    {
        BYTE i;
        BYTE OptimalChannel;
        BYTE minRSSI = 0xFF;
        DWORD channelMask = 0x00000001;
        MIWI_TICK t1, t2;
        
        if( DetectionMode != NOISE_DETECT_ENERGY )
        {
            return 0xFF;
        }
        
        ConsolePutROMString((ROM char*)"\r\nEnergy Scan Results:");
        i = 0;
        while( i < 32 )
        {
            if( ChannelMap & FULL_CHANNEL_MAP & (channelMask << i) )
            {
                BYTE RSSIcheck;
                BYTE maxRSSI = 0;
                BYTE j, k;

                /* choose appropriate channel */
                MiApp_SetChannel(i);
                
                t1 = MiWi_TickGet();
                
                while(1)
                {
                    RSSIcheck = MiMAC_ChannelAssessment(CHANNEL_ASSESSMENT_ENERGY_DETECT);
                    if( RSSIcheck > maxRSSI )
                    {
                        maxRSSI = RSSIcheck;
                    }

                    t2 = MiWi_TickGet();
                    if( MiWi_TickGetDiff(t2, t1) > ((DWORD)(ScanTime[ScanDuration])) )
                    {
                        // if scan time exceed scan duration, prepare to scan the next channel
                        break;
                    }
                } 
                
                Printf("\r\nChannel ");
                PrintDec(i);
                Printf(": ");
                j = maxRSSI/5;
                for(k = 0; k < j; k++)
                {
                    ConsolePut('-');
                }
                Printf(" ");
                PrintChar(maxRSSI);
                
                if( maxRSSI < minRSSI )
                {
                    minRSSI = maxRSSI;
                    OptimalChannel = i;
                    if( RSSIValue )
                    {
                        *RSSIValue = minRSSI;
                    }   
                }              
            }  
            i++;
        }        
       
        return OptimalChannel;
    }
   
#endif


#ifdef ENABLE_FREQUENCY_AGILITY

    /*********************************************************************
     * void StartChannelHopping(BYTE OptimalChannel)
     *
     * Overview:        This function broadcast the channel hopping command
     *                  and after that, change operating channel to the 
     *                  input optimal channel     
     *
     * PreCondition:    Transceiver has been initialized
     *
     * Input:           OptimalChannel  - The channel to hop to
     *                  
     * Output: 
     *          None
     *
     * Side Effects:    The operating channel for current device will change
     *                  to the specified channel
     *
     ********************************************************************/
    void StartChannelHopping(INPUT BYTE OptimalChannel)
    {
        BYTE i;
        MIWI_TICK t1, t2;
        
        for( i = 0; i < FA_BROADCAST_TIME; i++)
        {
            t1 = MiWi_TickGet();
            while(1)
            {
                t2 = MiWi_TickGet();
                if( MiWi_TickGetDiff(t2, t1) > SCAN_DURATION_9 )
                {
                    MiApp_FlushTx();
                    MiApp_WriteData(CMD_CHANNEL_HOPPING);
                    MiApp_WriteData(currentChannel);
                    MiApp_WriteData(OptimalChannel);
                    #if defined(IEEE_802_15_4)
                        SendPacket(TRUE, myPANID, NULL, TRUE, FALSE);
                    #else
                        SendPacket(TRUE, NULL, TRUE, FALSE);
                    #endif
                    break;
                }
            }
        }
        MiApp_SetChannel(OptimalChannel);  
    }
    
    
    /********************************************************************************************
     * Function:
     *      BOOL MiApp_ResyncConnection(BYTE ConnectionIndex, DWORD ChannelMap)
     *
     * Summary:
     *      This function tries to resynchronize the lost connection with 
     *      peers, probably due to channel hopping
     *
     * Description:        
     *      This is the primary user interface function for the application to resynchronize a 
     *      lost connection. For a RFD device that goes to sleep periodically, it may not 
     *      receive the channel hopping command that is sent when it is sleep. The sleeping 
     *      RFD device depends on this function to hop to the channel that the rest of
     *      the PAN has jumped to. This function call is usually triggered by continously 
     *      communication failure with the peers.
     *
     * PreCondition:    
     *      Transceiver has been initialized
     *
     * Parameters:      
     *      DWORD ChannelMap -  The bit map of channels to perform noise scan. The 32-bit
     *                          double word parameter use one bit to represent corresponding
     *                          channels from 0 to 31. For instance, 0x00000003 represent to 
     *                          scan channel 0 and channel 1. 
     *                  
     * Returns: 
     *                  a boolean to indicate if resynchronization of connection is successful
     *
     * Example:
     *      <code>
     *      // Sleeping RFD device resync with its associated device, usually the first peer
     *      // in the connection table
     *      MiApp_ResyncConnection(0, 0xFFFFFFFF);
     *      </code>
     *
     * Remark:          
     *      If operation is successful, the wireless node will be hopped to the channel that 
     *      the rest of the PAN is operating on.
     *
     *********************************************************************************************/ 
    BOOL MiApp_ResyncConnection(INPUT BYTE ConnectionIndex, INPUT DWORD ChannelMap)
    {
        BYTE i;
        BYTE j;
        BYTE backupChannel = currentChannel;
        MIWI_TICK t1, t2;
        
        t1 = MiWi_TickGet();
        P2PStatus.bits.Resync = 1;
        for(i = 0; i < RESYNC_TIMES; i++)
        {
            DWORD ChannelMask = 0x00000001;
            
            j = 0;
            while(P2PStatus.bits.Resync)
            {
                t2 = MiWi_TickGet();
                
                if( MiWi_TickGetDiff(t2, t1) > SCAN_DURATION_9 )
                {
                    t1.Val = t2.Val;
                    
                    if( j > 31 )
                    {
                        break;
                    }
                    while( (ChannelMap & FULL_CHANNEL_MAP & (ChannelMask << j)) == 0 )
                    {
                        if( ++j > 31 )
                        {
                            goto GetOutOfLoop;
                        }
                    }
                    
                    Printf("\r\nChecking Channel ");
                    PrintDec(j);
                    MiApp_SetChannel(j);
                    j++;
                    
                    MiApp_FlushTx();
                    MiApp_WriteData(CMD_P2P_ACTIVE_SCAN_REQUEST);
                    MiApp_WriteData(currentChannel);
        
                    #if defined(IEEE_802_15_4)
                        SendPacket(FALSE, myPANID, ConnectionTable[ConnectionIndex].Address, TRUE, FALSE);
                    #else
                        SendPacket(FALSE, ConnectionTable[ConnectionIndex].Address, TRUE, FALSE);
                    #endif
                }
                if( MiApp_MessageAvailable())
                {
                    MiApp_DiscardMessage();
                }        
                //P2PTasks();
            }
            if( P2PStatus.bits.Resync == 0 )
            {
                Printf("\r\nResynchronized Connection to Channel ");
                PrintDec(currentChannel);
                Printf("\r\n");
                return TRUE;
            }
GetOutOfLoop:
            MacroNop();         
        }
        
        MiApp_SetChannel(backupChannel);
        P2PStatus.bits.Resync = 0;
        return FALSE;
    }        

    #ifdef FREQUENCY_AGILITY_STARTER
        /*******************************************************************************************
         * Function:
         *      BOOL MiApp_InitChannelHopping(DWORD ChannelMap)
         *
         * Summary:
         *      
         *      This function tries to start a channel hopping (frequency agility) procedure
         *
         * Description:        
         *      This is the primary user interface function for the application to do energy 
         *      scan to locate the channel with least noise. If the channel is not current 
         *      operating channel, process of channel hopping will be started.
         *
         * PreCondition:    
         *      Transceiver has been initialized
         *
         * Parameters:      
         *      DWORD ChannelMap -  The bit map of the candicate channels
         *                          which can be hopped to
         *                  
         * Returns: 
         *                  a boolean to indicate if channel hopping is initiated
         *
         * Example:
         *      <code>
         *      // if condition meets, scan all possible channels and hop 
         *      // to the one with least noise
         *      MiApp_InitChannelHopping(0xFFFFFFFF);
         *      </code>
         *
         * Remark:          The operating channel will change to the optimal 
         *                  channel with least noise
         *
         ******************************************************************************************/
        BOOL MiApp_InitChannelHopping( INPUT DWORD ChannelMap)
        {
            BYTE RSSIValue;
            BYTE backupChannel = currentChannel;
            BYTE backupConnMode = ConnMode;
            BYTE optimalChannel;
            
            MiApp_ConnectionMode(DISABLE_ALL_CONN);
            optimalChannel = MiApp_NoiseDetection(ChannelMap, 10, NOISE_DETECT_ENERGY, &RSSIValue);
            MiApp_ConnectionMode(backupConnMode);
            
            MiApp_SetChannel(backupChannel);
            if( optimalChannel == backupChannel )
            {
                return FALSE;
            }
            
            Printf("\r\nHopping to Channel ");
            PrintDec(optimalChannel);
            Printf("\r\n");
            StartChannelHopping(optimalChannel);
            return TRUE;
        }
    #endif

#endif



#if !defined(TARGET_SMALL)
    /*********************************************************************
     * Function:
     *      void MiApp_RemoveConnection(BYTE ConnectionIndex)
     *
     * Summary:
     *      This function remove connection(s) in connection table
     *
     * Description:        
     *      This is the primary user interface function to disconnect connection(s).
     *      For a P2P protocol, it simply remove the connection. For a network protocol,
     *      if the device referred by the input parameter is the parent of the device
     *      calling this function, the calling device will get out of network along with
     *      its children. If the device referred by the input parameter is children of
     *      the device calling this function, the target device will get out of network.
     * 
     * PreCondition:    
     *      Transceiver has been initialized. Node has establish
     *      one or more connections
     *
     * Parameters:           
     *      BYTE ConnectionIndex -  The index of the connection in the 
     *                              connection table to be removed
     *                  
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      MiApp_RemoveConnection(0x00);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     ********************************************************************/
    void MiApp_RemoveConnection(INPUT BYTE ConnectionIndex)
    {   
        if( ConnectionIndex == 0xFF )
        {
            BYTE i;
            for(i = 0; i < CONNECTION_SIZE; i++)
            {
                WORD j;
                if( ConnectionTable[i].status.bits.isValid )
                {
                    MiApp_FlushTx();
                    MiApp_WriteData(CMD_P2P_CONNECTION_REMOVAL_REQUEST);   
                    #if defined(IEEE_802_15_4)
                        SendPacket(FALSE, myPANID, ConnectionTable[i].Address, TRUE, FALSE);
                    #else
                        SendPacket(FALSE, ConnectionTable[i].Address, TRUE, FALSE);
                    #endif
                    for(j = 0; j < 0xFFF; j++) {}   // delay
                }
                ConnectionTable[i].status.Val = 0;
                #if defined(ENABLE_NETWORK_FREEZER)
                    nvmPutConnectionTableIndex(&(ConnectionTable[i]), i);
                #endif
            } 
        }
        else if( ConnectionTable[ConnectionIndex].status.bits.isValid )
        {
            WORD j;
            
            MiApp_FlushTx();
            MiApp_WriteData(CMD_P2P_CONNECTION_REMOVAL_REQUEST);   
            #if defined(IEEE_802_15_4)
                SendPacket(FALSE, myPANID, ConnectionTable[ConnectionIndex].Address, TRUE, FALSE);
            #else
                SendPacket(FALSE, ConnectionTable[ConnectionIndex].Address, TRUE, FALSE);
            #endif
            for(j = 0; j < 0xFFF; j++) {}   // delay
            ConnectionTable[ConnectionIndex].status.Val = 0; 
            #if defined(ENABLE_NETWORK_FREEZER)
                nvmPutConnectionTableIndex(&(ConnectionTable[ConnectionIndex]), ConnectionIndex);
            #endif
        }
    }
#endif

/************************************************************************************
 * Function:
 *      void    MiApp_ConnectionMode(BYTE Mode)
 *
 * Summary:
 *      This function set the current connection mode.
 *
 * Description:        
 *      This is the primary user interface function for the application layer to 
 *      configure the way that the host device accept connection request.
 *
 * PreCondition:    
 *      Protocol initialization has been done. 
 *
 * Parameters:           
 *      BYTE Mode -     The mode to accept connection request. The privilege for those modes
 *                      decreases gradually as defined. The higher privilege mode has all the 
 *                      rights of the lower privilege modes.
 *                      The possible modes are
 *                      * ENABLE_ALL_CONN       Enable response to all connection request
 *                      * ENABLE_PREV_CONN      Enable response to connection request
 *                                              from device already in the connection
 *                                              table.
 *                      * ENABLE_ACTIVE_SCAN_RSP    Enable response to active scan only
 *                      * DISABLE_ALL_CONN      Disable response to connection request, including
 *                                              an acitve scan request.
 *
 * Returns: 
 *      None
 *
 * Example:
 *      <code>
 *      // Enable all connection request
 *      MiApp_ConnectionMode(ENABLE_ALL_CONN);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
void MiApp_ConnectionMode(INPUT BYTE Mode)
{
    if( Mode > 3 )
    {
        return;
    }
    ConnMode = Mode;
    P2PCapacityInfo = (P2PCapacityInfo & 0x0F) | (ConnMode << 4);
    
    #if defined(ENABLE_NETWORK_FREEZER)
        nvmPutConnMode(&ConnMode);
    #endif
}

#else  // defined PROTOCOL_P2P
    /*******************************************************************
     * C18 compiler cannot compile an empty C file. define following 
     * bogus variable to bypass the limitation of the C18 compiler if
     * a different protocol is chosen.
     ******************************************************************/
    extern char bogusVariable;
#endif

