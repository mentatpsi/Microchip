/********************************************************************
* FileName:		MiWiPRO.h
* Dependencies: SymbolTime.h
* Processor:	PIC18, PIC24F, PIC24H, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher	
* Company:		Microchip Technology, Inc.
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
*  This file provides the interface into the MiWi PRO stack
*
* Change History:
*  Rev   Date         Author    Description
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#ifndef __MIWI_PRO_H_
#define __MIWI_PRO_H_

/************************ HEADERS **********************************/
#include "ConfigApp.h"
#include "WirelessProtocols/SymbolTime.h"
#include "Transceivers/Transceivers.h"
#include "WirelessProtocols/MiWiPRO/ConfigMiWiPRO.h"


#define INPUT
#define OUTPUT
#define IOPUT

#define MAX_HOPS    (NUM_COORDINATOR+1)

#if !defined(ENABLE_NETWORK_FREEZER)
    #error "Network Freezer feature must be enabled for MiWi PRO protocol. Check ConfigApp.h"
#endif

/************************ FUNCTION PROTOTYPES **********************/
void MiWiPROTasks(void);	
BYTE findNextNetworkEntry(void);
void ClearNetworkTable(void);
BYTE SearchForLongAddress(void);
BYTE SearchForShortAddress(void);   
void SendIndirectPacket(BYTE *Address, BYTE *AltAddress, BOOL isAltAddress);
BYTE AddNodeToNetworkTable(void);
void DiscoverNodeByEUI(void);
void OpenSocket(void);
BOOL isSameAddress(INPUT BYTE *Address1, INPUT BYTE *Address2);
void DumpConnection(BYTE index);


/************************ DEFINITIONS ******************************/
#define FRAME_TYPE_BEACON   0x00
#define FRAME_TYPE_DATA     0x01
#define FRAME_TYPE_ACK      0x02
#define FRAME_TYPE_COMMAND  0x03

#define MAC_COMMAND_ASSOCIATION_REQUEST             0x01
#define MAC_COMMAND_ASSOCIATION_RESPONSE            0x02
    #define ASSOCIATION_SUCCESSFUL                  0x00
    #define ASSOCIATION_PAN_FULL                    0x01
    #define ASSOCIATION_ACCESS_DENIED               0x02
#define MAC_COMMAND_DISASSOCIATION_NOTIFICATION     0x03
#define MAC_COMMAND_DATA_REQUEST                    0x04
#define MAC_COMMAND_PAN_ID_CONFLICT_NOTIFICATION    0x05
#define MAC_COMMAND_ORPHAN_NOTIFICATION             0x06
#define MAC_COMMAND_BEACON_REQUEST                  0x07
#define MAC_COMMAND_COORDINATOR_REALIGNMENT         0x08


#define MAC_COMMAND_TIME_SYNC_DATA_PACKET               0x8A
#define MAC_COMMAND_TIME_SYNC_COMMAND_PACKET            0x8B


#define MIWI_PRO_PROTOCOL_ID 0x5D
#define MIWI_PROTOCOL_ID 0x4D
#define MIWI_PRO_VERSION_NUM 0x41               //v4.1
#define MIWI_PRO_ACK_REQ 0x04

#define PAYLOAD_START       11

#define FAMILY_NONE         0
#define FAMILY_PARENT       0x01
#define FAMILY_CHILD        0x02
#define FAMILY_SIBLING      0x03


#define ROLE_FFD_END_DEVICE 0x00
#define ROLE_COORDINATOR    0x01
#define ROLE_PAN_COORDINATOR 0x02

#define DATA_REQUEST_ASSOCIATION_RESPONSE 0x00
#define DATA_REQUEST_SHORT_ADDRESSES 0x01


#define MIWI_PRO_HEADER_LEN     11
/* Report type and ID definitions */
/* as a user you are able to use Report types 0x10 - 0xFF */

#define MIWI_PRO_STACK_REPORT_TYPE              0x00
    #define OPEN_SOCKET_REQUEST             0x10
    #define OPEN_SOCKET_RESPONSE            0x11
    #define EUI_ADDRESS_SEARCH_REQUEST      0x20
    #define EUI_ADDRESS_SEARCH_RESPONSE     0x21
    #define ACK_REPORT_TYPE                 0x30
    
    #define CHANNEL_HOPPING_REQUEST         0x40
    #define RESYNCHRONIZATION_REQUEST       0x41
    #define RESYNCHRONIZATION_RESPONSE      0x42
    
    #define COORDINATOR_REPORT              0xA0
    #define COORDINATOR_RESPONSE            0xA1
    #define FAMILY_TREE_REQUEST             0xA2
    #define FAMILY_TREE_REPORT              0xA3
    #define ROUTING_TABLE_REPORT            0xA4
    
    #define FA_COORDINATOR_REQUEST          0xB0
    #define FA_ENERGY_SCAN_REQUEST          0xB1
    #define FA_SUGGEST_CHANNEL              0xB2
    #define FA_DISPUTE_CHANNEL              0xB3

#define COORDINATOR_REPORT_STATUS_JOIN      0x00
#define COORDINATOR_REPORT_STATUS_LEAVE     0x01

/************************ DATA TYPE *******************************/
typedef union _MIWI_PRO_STATE_MACHINE
{
    BYTE Val;
    struct {
        BYTE searchingForNetwork    :1;
        BYTE memberOfNetwork        :1;
        BYTE RxHasUserData          :1;
        BYTE MiWiPROAckInProgress   :1;
        BYTE EUISearching           :1;
        BYTE DataRequesting         :1;
        BYTE Resynning              :1;
        BYTE Sleeping               :1;
    } bits;
} MIWI_PRO_STATE_MACHINE;

typedef union _MIWI_PRO_STATUS
{
    BYTE Val;
    struct
    {
        BYTE COMM_RANDOM_SEND       :1;
        BYTE COMM_FAMILY_TREE       :3;
        BYTE COMM_ROUTING_TABLE     :3;
    } bits;
} MIWI_PRO_STATUS;


typedef struct _OPEN_SOCKET
{
    union _OPEN_SOCKET_STATUS
    {
        struct _OPEN_SOCKET_STATUS_bits
        {
            BYTE matchFound :1;
            BYTE requestIsOpen :1;
            BYTE itIsMe         :1;
        } bits;
        BYTE Val;
    } status;
    BYTE socketHandle;
    WORD_VAL ShortAddress1;
    BYTE LongAddress1[MY_ADDRESS_LENGTH];
    #if ADDITIONAL_NODE_ID_SIZE > 0
        BYTE AdditionalNodeID1[ADDITIONAL_NODE_ID_SIZE];
    #endif
    #ifdef NWK_ROLE_COORDINATOR
        WORD_VAL ShortAddress2;
        BYTE LongAddress2[MY_ADDRESS_LENGTH];
    #endif
    MIWI_TICK socketStart;
} OPEN_SOCKET;



/******************************************************************
 * Overview: The structure to store indirect messages for devices turn off 
 * radio when idle
 *****************************************************************/
typedef struct 
{
    MIWI_TICK        TickStart;      // start time of the indirect message. Used for checking 
                                // indirect message time out
    #if defined(IEEE_802_15_4)                                
        WORD_VAL    DestPANID;      // the PAN identifier for the destination node
    #endif
    BYTE        DestAddress[MY_ADDRESS_LENGTH];             // unicast destination long address
    union 
    {
        BYTE    Val;                        // value for the flags
        struct 
        {
            BYTE    isValid         : 1;    // if this indirect message is valid
            BYTE    isBroadcast     : 1;    // if this indirect message is for broadcasting
            BYTE    isCommand       : 1;    // if this indirect message a command
            BYTE    isSecured       : 1;    // if this indirect message requires encryption
            BYTE    isAltAddr       : 1;    // if the destination address a short address
        } bits;                             // bit map of the flags
    } flags;                                // flags for indirect message
    BYTE        PayLoadSize;                // the indirect message pay load size
    #if (TX_BUFFER_SIZE + MIWI_PRO_HEADER_LEN) > 112
        BYTE        PayLoad[112];
    #else
        BYTE        PayLoad[TX_BUFFER_SIZE+MIWI_PRO_HEADER_LEN];    // the indirect message pay load
    #endif
} INDIRECT_MESSAGE;


/************************ EXTERNAL VARIABLES **********************/

extern MIWI_PRO_STATE_MACHINE MiWiPROStateMachine;
extern BYTE TxBuffer[];
extern BYTE TxData;
extern WORD_VAL myPANID;
extern WORD_VAL myShortAddress;
extern BYTE myParent;
extern BYTE tempLongAddress[MY_ADDRESS_LENGTH];
extern WORD_VAL tempShortAddress;
extern OPEN_SOCKET openSocketInfo;

/************************ MACROS **********************************/
#define MAC_FlushTx() {TxData = 0;}
 
#define MSK_SRC_PRSNT           0x01
#define MSK_DST_PRSNT           0x02
#define MSK_ALT_SRC_ADDR        0x04
#define MSK_ALT_DST_ADDR        0x08

#define PACKET_TYPE_BEACON      0x03

#define MICRO_SECOND_PER_COUNTER_TICK   (1000000ul / ( COUNTER_CRYSTAL_FREQ / 8 ))
    
#endif

    
    
