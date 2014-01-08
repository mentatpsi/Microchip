/********************************************************************
* FileName:		RF4CE.h
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
*  This file provides the interface into the RF4CE stack
*
* Change History:
*  Rev          Date        Author      Description
*  1.0-1.1.2    12/10/2009  yfy         Initial release
********************************************************************/

#ifndef __RF4CE_H_
#define __RF4CE_H_

/************************ HEADERS **********************************/
#include "ConfigApp.h"
#include "WirelessProtocols/SymbolTime.h"
#include "Transceivers.h"
#include "ConfigRF4CE.h"
#include "GenericTypeDefs.h"


/************************ DEFINITIONS ******************************/
#define INPUT
#define OUTPUT
#define IOPUT

#define RF4CE_TICK              MIWI_TICK
#define RF4CE_TickGet()         MiWi_TickGet()
#define RF4CE_TickGetDiff(a, b) MiWi_TickGetDiff(a, b)

#if defined(RF4CE_TARGET)
    #define CAP_NODE_TYPE   0x01
#else
    #define CAP_NODE_TYPE   0x00
#endif

#if defined(RF4CE_TARGET)
    #define CAP_POWER_SOURCE    0x02
#else
    #define CAP_POWER_SOURCE    0x00
#endif

#if defined(ENABLE_RF4CE_SECURITY)
    #define CAP_SECURITY        0x04
#else
    #define CAP_SECURITY        0x00
#endif

#if defined(ENABLE_RF4CE_CHANNEL_NORM)
    #define CAP_CHANNEL         0x08
#else
    #define CAP_CHANNEL         0x00
#endif

#define NODE_CAPABILITY     (CAP_NODE_TYPE | CAP_POWER_SOURCE | CAP_SECURITY | CAP_CHANNEL)

// Status: Success
#define SUCCESS                                     0

// Status: Error - Originator has no capacity
#define NO_ORG_CAPACITY                             0xb0

// Status: Error - Reception has no capacity
#define NO_REC_CAPACITY                             0xb1

// Status: Error - No existing pairing
#define NO_PAIRING                                  0xb2

// Status: Error - No response received
#define NO_RESPONSE                                 0xb3

// Status: Error - Not permitted
#define NOT_PERMITTED                               0xb4

// Status: Warning - Duplicate pairing
#define DUPLICATE_PAIRING                           0xb5

// Status: Warning - Outgoing frame counter has expired (overflow)
#define FRAME_COUNTER_EXPIRED                       0xb6

// Status: Error - Discovery Error
#define DISCOVERY_ERROR                             0xb7

// Status: Error - Discovery timeout
#define DISCOVERY_TIMEOUT                           0xb8

// Status: Error - Timeout in secured pairing
#define SECURITY_TIMEOUT                            0xb9

// Status: Error - Failure in secured pairing
#define SECURITY_FAILURE                            0xba

// Status: Error - Invalid parameter
#define INVALID_PARAMETER                           0xe8

// Status: Error - No acknowledgement
#define NO_ACK                                      0xe9

// Status: Error - Unsupported attribute
#define UNSUPPORTED_ARRIBUTE                        0xf4

// Status: Error - Invalid index
#define INVALID_INDEX                               0xf9

// Error code: Transceiver failure
#define ERR_TRX_FAIL                                0x01

// Error code: Transmitter failure
#define ERR_TX_FAIL                                 0x02

// Error code: Receiver failure
#define ERR_RX_FAIL                                 0x03

// Error code: Invalid input parameter
#define ERR_INVALID_INPUT                           0xFF

#define AUTO_DISC_SUCCESS                           0x80
#define PREV_AUTO_DISC                              0x81
#define AUTO_DISC_ERROR                             0x8F


#define MAC_COMMAND_BEACON_REQUEST                  0x07

#define RF4CE_COMMAND_DISCOVERY_REQUEST             0x01
#define RF4CE_COMMAND_DISCOVERY_RESPONSE            0x02
#define RF4CE_COMMAND_PAIR_REQUEST                  0x03
#define RF4CE_COMMAND_PAIR_RESPONSE                 0x04
#define RF4CE_COMMAND_UNPAIR_REQUEST                0x05
#define RF4CE_COMMAND_KEY_SEED                      0x06
#define RF4CE_COMMAND_PING_REQUEST                  0x07
#define RF4CE_COMMAND_PING_RESPONSE                 0x08

// Constant: RF4CE protocol identifier
#define RF4CE_PROTOCOL_ID                           0xCE

// Constant: RF4CE ZigBee Remote Control Profile ID
#define ZRC_PROFILE_ID                              0x01

// Constant: RF4CE protocol version number
#define RF4CE_VERSION_NUM                           0x01 
  
#define RF4CE_MAX_SEC_CMD_TX_POWER                  15      // -15dB


#define RF4CE_RESERVED_FRAME                        0x00
#define RF4CE_DATA_FRAME                            0x01
#define RF4CE_COMMAND_FRAME                         0x02
#define RF4CE_VENDOR_FRAME                          0x03

// Network constant: The RF4CE channel mask for available channels. The available channels
// are 15, 20 and 25
#define NWKC_CHANNEL_MASK                           0x02108000

// Network constant: The maximum time in symbols that multichannel unicast with acknowledgement will 
// try before declear failure. 
#define NWKC_MAX_DUTY_CYCLE                         (ONE_SECOND/10)                        //100ms  // 1 second

// Network constant: The maximum allowed time for each key seed command to be transmitted. Used
// mainly for timeout during key seed procedure                      
#define NWKC_MAX_KEY_SEED_WAIT_TIME                 (SYMBOLS_TO_TICKS(3750))            // 60 ms

// Network constant: The interval of frame counters between saving to non-volatile memory. After
// power cycle or reset, this is also the number used to be added to the frame counter read from 
// non-volatile memory. This definition is used to save the frame counter with reduced frequency
// to save flash memory endurance
#define NWKC_FRAME_COUNTER_WINDOW                   1024

#define MSK_SRC_ABSNT                               0x01
#define MSK_ACK_NOT_REQ                             0x02
#define MSK_ALT_SRC_ADDR                            0x04
#define MSK_ALT_DST_ADDR                            0x08
#define MSK_DISABLE_RETRANS                         0x10
#define MSK_RF4CE_SECURITY                          0x20
#define MSK_RF4CE_CHANNEL_DSG                       0x40
#define MSK_RF4CE_ASIS                              0x80

#define PACKET_TYPE_BEACON                          0x03

#define PACKET_TYPE_RESERVED                        0x00
#define PACKET_TYPE_RF4CE_DATA                      0x01
#define PACKET_TYPE_RF4CE_COMMAND                   0x02
#define PACKET_TYPE_RF4CE_VENDOR_DATA               0x03

#define POWER_STATE_SLEEP                           0x00
#define POWER_STATE_WAKEUP                          0x01

/************************ DATA TYPE *******************************/
typedef union 
{
    BYTE    Val;
    struct
    {
        BYTE    FrameType       :2;
        BYTE    SecEn           :1;
        BYTE    ProtocolVer     :2;
        BYTE    Reserved        :1;
        BYTE    Channel         :2;
    } bits;
} RF4CE_FRAME_CONTROL;


/*********************************************************************
  Application Capability
  
  This structure serves as the control bit map of capability for the 
  application layer. It is used to interpolate data immediately follow
  this control bit map. It is used in discovery and pairing process. 
  *********************************************************************/
typedef union
{
    BYTE    Val;
    struct
    {
        BYTE    UserString      : 1;    // 1: User string exist, 0: User string omitted
        BYTE    NumDeviceTypes  : 2;    // Total number of supported device types, range
                                        // is 1 to 3
        BYTE    Reserved_1      : 1;
        BYTE    NumProfiles     : 3;    // Total number of supported profiles, range is
                                        // 1 to 7
        BYTE    Reserved_2      : 1;
    } bits;
} APPLICATION_CAPABILITIES;


typedef union 
{
    BYTE Val;
    struct {
        BYTE beaconInProgress           :1;
        BYTE discoveryInProgress        :1;
        BYTE pairInProgress             :1;
        BYTE keySeedInProgress          :1;
        BYTE pingInProgress             :1; 
        BYTE autoDiscoveryInProgress    :1;
        BYTE powerSavingInProgress      :1;
    } bits;
} RF4CE_STATE_MACHINE;


typedef union
{
    BYTE Val;
    struct
    {
        BYTE RxHasUserData          :1;
        BYTE PowerSaveSleep         :1;
        BYTE memberOfNetwork        :1;
        BYTE nwkIndicateDiscovery   :1;
    } bits;
} RF4CE_STATUS;

typedef struct 
{
    BYTE    PairIndex;
    MIWI_TICK    SeedTick;
} KEY_SEED_INFO;


/***************************************************************************
 * Received Message information
 *  
 *      This structure contains information about the received application
 *      message.
 **************************************************************************/
typedef struct
{
    union
    {
        BYTE Val;
        struct
        {
            BYTE		broadcast:  1;      // 1: broadcast message
            BYTE		secEn:      1;      // 1: application payload has been secured
            BYTE        vendorFrame:1;      // 1: vendor specific frame
        } bits;
    }  RxFlags;

    BYTE        PairIndex;                  // the index of the source node in the pair table
    BYTE        ProfileID;                  // the profile identifier for this message
    WORD        VendorID;                   // vendor identifier, only valid for vendor specific frame
    BYTE		*Payload;                   // pointer to the application payload
    BYTE		PayloadSize;                // application payload length
    BYTE		PacketRSSI;                 // RSSI value of the receive message
    BYTE 		PacketLQI;                  // LQI value of the received message

} RECEIVED_MESSAGE;


/*********************************************************************
  Transmission Options
  
  This structure uses the bit map to specify how to transmit a packet.
  It is used in function call RF4CE_SendFrame, which is in application 
  layer for all data transmission.          
  *********************************************************************/
typedef union
{
    BYTE Val;
    struct
    {
        BYTE    broadcast       :1;     // 1: broadcast, 0: unicast
        BYTE    IEEEAddress     :1;     // 1: IEEE destination address, 0: Network destination address
        BYTE    ackReq          :1;     // 1: required acknowledgement, 0: acknowledgement not required
        BYTE    secEn           :1;     // 1: secure the outgoing packet, only valid in unicast
                                        // 0: not secure the outgoing packet
        BYTE    singleChannel   :1;     // 1: single channel transmission
                                        // 0: transmit on all 3 channels in broadcast, or transmit
                                        // on all 3 channels if acknowledgement is not received
                                        // in unicast with acknowledgement
        BYTE    channelDsgtor   :1;     // 1: Channel designator on, 0: Channel designator off
        BYTE    vendorFrame     :1;     // 1: Vendor specific frame, 0: normal profile frame
    } bits;        
} TX_OPTIONS;



typedef struct
{
    union
    {
        BYTE    Val;
        struct
        {
            BYTE    isValid : 1;
            BYTE    isRF4CE : 1;
            BYTE    protocolVersion : 6;
        } bits;
    } status;
    BYTE        Channel;
    WORD_VAL    Address;
    WORD_VAL    PANID;      
} BEACON_RESULT;


/*********************************************************************
  Discovery Result
  
  This structure contains peer device information in the discovery
  process. It is used both on the discovery request and response ends.
  When it is used in request end, it stores the information from the
  discovery response frame. When it is used in response end, it stores
  the information from the discovery request command frame.           
  *********************************************************************/
typedef struct
{
    BYTE		Channel;                        // Operating Channel of the PAN
    BYTE 		IEEEAddress[8];                 // Peer device IEEE address
    WORD_VAL	PANID;                          // PAN Identifier
    BYTE		RSSIValue;                      // RSSI value for the comand
                                                // that is received. Valid on
                                                // both ends of discovery process  
    BYTE		DiscoverRSSI;                   // RSSI value in the discovery 
                                                // response comamnd, used for the 
                                                // responding node to notify the 
                                                // discovery originator the RSSI 
                                                // value of discovery request command
                                                // frame. Valid only at the originator
                                                // end.
    BYTE        Status;                         // Status of the discovery process
    BYTE        NodeCap;                        // Node capability. It is a bit map
                                                // representation of the peer node
                                                // capability. Check structure
                                                // NODE_CAP for definition.
    BYTE        VendorID[2];                    // Vendor Identifier
    BYTE        VendorString[7];                // Vendor string
    APPLICATION_CAPABILITIES    AppCap;         // Application capability
    BYTE        UserString[15];                 // User string, optional, depends on
                                                // AppCap setting
    BYTE        DeviceTypeList[3];              // Device type list, up to 3 device 
                                                // types
    BYTE        ProfileIDList[7];               // Supported profile list, up to 7
                                                // supported profiles
    BYTE        SearchDeviceType;               // The device type that discovery
                                                // originator is asking for. Only
                                                // valid at the discovery reception end.
} DISCOVERY_RESULT; 



/*********************************************************************
  Node Capability
  
  This structure contains information about the capability of a RF4CE
  Node.
             
  *********************************************************************/
typedef union
{
    BYTE Val;
    struct
    {
        BYTE    isTarget        :1;     // 1: Target, 0: Controller
        BYTE    isMain          :1;     // 1: Main powered, 0: battery powered
        BYTE    isSecEn         :1;     // 1: Security enabled, 0: Security disabled
        BYTE    isChannelNorm   :1;     // 1: Channel normalization supported, 0: Channel normalization unsupported.
        BYTE    Reserved        :2;
        BYTE    isProvisional   :1;     // 1: Node record is Provisional, 0: Node record is not provisional
        BYTE    isValid         :1;     // 1: Node record is in use, 0: Node record is not in use
    } bits;
} NODE_CAP;


/*********************************************************************
  Pair Table Entry
  
  This structure contains information about the peer device of a pairing.
  This information is stored in non-volatile memory so that pairing 
  information will not get lost upon power cycle.          
  *********************************************************************/
typedef struct
{
    WORD_VAL    SrcNWKAddress;              // The network address for this pairing. 
                                            // For target, it is always 0xFFFE. Real
                                            // network address assigned for controller
    BYTE        Channel;                    // The operating channel that this pair 
                                            // communicate on
    BYTE        DstIEEEAddress[8];          // Peer device's IEEE address of this pair
    WORD_VAL    DstPANID;                   // Peer device's PAN Identifier of this pair
    WORD_VAL    DstNWKAddress;              // Peer device's network address of this pair
    NODE_CAP    NodeCap;                    // Peer device's capability
    #if defined(ENABLE_RF4CE_SECURITY)
        BYTE        SecurityLinkKey[16];    // Security link key of this pair, if security
                                            // is enabled on both ends of the pair     
    #endif  
} PAIR_TABLE_ENTRY;



/****************************** FUNCTION PROTOTYPES *******************************/

/******************** RF4CE stack APIs called by applications************************/

/************************************************************************************
 * Function:
 *      BYTE RF4CE_AutoDiscovery(DWORD AutoDiscoveryDuration, BYTE *DiscoveryIndex)
 *
 * Summary:
 *      This function starts automatic discovery procedure on target device
 *
 * Description:        
 *      This function is called by application layer of target device to start an
 *      auto discovery procedure. The auto discovery result status will be returned.
 *      In case the auto discovery return status is success, the DiscoveryIndex 
 *      parameter will be updated with the index of the discovered node in the 
 *      discovery result table; otherwise, the DiscoveryIndex parameter will be 
 *      updated with 0xFF
 *
 * PreCondition:    
 *      Transceiver initialization has been finished. 
 *
 * Parameters: 
 *      DWORD AutoDiscoveryDuration - The auto discovery duration time, in symbols. One
 *                                    symbol is equal to 16us.
 *      BYTE * DiscoveryIndex       - The index of the discovered node in the discovery
 *                                    table, if the auto discovery procedure is successful;
 *                                    otherwise, the DiscoveryIndex will be set to 0xFF;
 *
 * Returns: 
 *      The status code to indicate the operation result.
 *
 * Example:
 *      <code>
 *      RF4CE_AutoDiscovery(ONE_SECOND * 30, &DiscoveryIndex);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BYTE RF4CE_AutoDiscovery(DWORD AutoDiscoveryDuration, BYTE *DiscoveryIndex);

/************************************************************************************
 * Function:
 *      void    RF4CE_DiscardMessage(void)
 *
 * Summary:
 *      This function discard the current message for the application and notify the 
 *      protocol layer that it is ready to receive the next message.
 *
 * Description:        
 *      This is the primary user interface functions for the application layer to 
 *      discard the current active message, release the system resources used and 
 *      ready to receive the next message. It is must be called after finish handling
 *      the message, otherwise, no further message can be received.
 *
 * PreCondition:    
 *      Protocol initialization has been done. A message has been received by the 
 *      application layer.
 *
 * Parameters: 
 *      None
 *
 * Returns: 
 *      None
 *
 * Example:
 *      <code>
 *      if( TRUE == RF4CE_MessageAvailable() )
 *      {
 *          // handle the received message in global variable RxMessage
 *
 *          // discard the received message after processing
 *          RF4CE_DiscardMessage();
 *      }
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/      
void    RF4CE_DiscardMessage(void);

/************************************************************************************
 * Function:
 *      BYTE RF4CE_Discovery(WORD DestPANID, WORD DestAddr, DWORD ScanDuration, 
 *                           DWORD ChannelMap, BYTE RequestDevType)
 *
 * Summary:
 *      This function tries to discovery node in the neighborhood 
 *
 * Description:        
 *      This function is called by application layer to discover nodes in the neighborhood
 *      according to input criteria. The discovery status will be returned and the discovery
 *      results will be stored in the DiscoveryResults structure array.
 *
 * PreCondition:    
 *      Transceiver initialization has been finished. 
 *
 * Parameters: 
 *      WORD DestPANID          - The PAN identifier of the node to be discovered. When
 *                                trying to discover any device, 
 *      WORD DestAddr           - The network address of the node to be discovered
 *      DWORD ScanDuration      - The scan duration on each channel in symbols. One 
 *                                symbol is equal to 16us. The real scan duration in 
 *                                each channel decided by the value of ScanDuration 
 *                                or 1/3 of NWK_DISCOVERY_REPETITION_INTERVAL, which
 *                                ever is longer.
 *      DWORD ChannelMap        - The bit map of channels to be scanned. The valid 
 *                                channels are 15, 20 and 25. Any other channels 
 *                                bitmap setting will be ignored.
 *      BYTE RequestDevType     - The request type of the device to be discovered.
 *
 * Returns: 
 *      The number of discovered nodes.
 *
 * Example:
 *      <code>
 *      RF4CE_Discovery(DestPANID, DestAddr, ONE_SECOND, 0x02108000, 0x02);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BYTE RF4CE_Discovery(WORD DestPANID, WORD DestAddr, DWORD ScanDuration, DWORD ChannelMap, BYTE RequestDevType);

/************************************************************************************
 * Function:
 *      BOOL    RF4CE_MessageAvailable(void)
 *
 * Summary:
 *      This function return a boolean if a message is available for the application
 *
 * Description:        
 *      This is the primary user interface functions for the application layer to 
 *      call the Microchip proprietary protocol stack to check if a message is available
 *      for the application. The function will call the protocol stack state machine
 *      to keep the stack running. It is expected that this function should be called
 *      periodically in the application. In case a message is available, all information
 *      related to the recevied message will be stored in the global variable RxMessage
 *      in the format of RECEIVED_MESSAGE.
 *
 * PreCondition:    
 *      Protocol initialization has been done. 
 *
 * Parameters: 
 *      None
 *
 * Returns: 
 *      A boolean to indicates if a message is available for application.
 *
 * Example:
 *      <code>
 *      if( TRUE == RF4CE_MessageAvailable() )
 *      {
 *          // handle the received message in global variable RxMessage
 *
 *          // discard the received message after processing
 *          RF4CE_DiscardMessage();
 *      }
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/      
BOOL    RF4CE_MessageAvailable(void);

/************************************************************************************
 * Function:
 *      BYTE RF4CE_NoiseDetection(  DWORD ChannelMap, BYTE ScanDuration, 
 *                                  BYTE *NoiseLevel)
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
 *      DWORD ChannelMap -      The bit map of channels to perform noise scan. The 32-bit
 *                              double word parameter use one bit to represent corresponding
 *                              channels from 0 to 31. For instance, 0x00000003 represent to 
 *                              scan channel 0 and channel 1. 
 *      DWORD ScanDuration -    The maximum time to perform scan on single channel in symbol.
 *                              According to IEEE 802.15.4 specification, one symbol is 16 us.
 *      BYTE *RSSIValue -       The noise level at the channel with least noise level 
 *
 * Returns: 
 *      The channel that has the lowest noise level
 *
 * Example:
 *      <code>
 *      BYTE NoiseLevel;
 *      OptimalChannel = RF4CE_NoiseDetection(0x02108000, ONE_SECOND, &NoiseLevel);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/
BYTE    RF4CE_NoiseDetection(DWORD ChannelMap, DWORD ScanDuration, OUTPUT BYTE *NoiseLevel);

/************************************************************************************
 * Function:
 *      BYTE RF4CE_Pair(BYTE DiscoveryIndex, BYTE *PairIndex)
 *
 * Summary:
 *      This function creates a pair link between two devices
 *
 * Description:        
 *      This function is called by application layer establish a pair link between
 *      two devices. Usually, pairing procedure starts after a successful discovery
 *      procedure. The pair node information is stored in the discovery result
 *      data structure. The pair status will be returned and the pair index will be
 *      passed out by pointer.
 *
 * PreCondition:    
 *      Transceiver initialization has been finished. 
 *
 * Parameters: 
 *      BYTE DiscoveryIndex - The pair node index in the discovery result array. The
 *                            discovery result store all information regarding the node
 *                            to be paired.
 *      BYTE * PairIndex    - The index of the pair node in the pair table, if pairing
 *                            process is successful. Otherwise, 0xFF will be returned.
 *
 * Returns: 
 *      The status code to indicate the operation result.
 *
 * Example:
 *      <code>
 *      RF4CE_Pair(0x00, &PairIndex);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BYTE RF4CE_Pair(BYTE DiscoveryIndex, BYTE *PairIndex);

/************************************************************************************
 * Function:
 *      BYTE RF4CE_PowerSave(BOOL bPowerSave)
 *
 * Summary:
 *      This function puts transceiver in and out of power save mode for target
 *
 * Description:        
 *      This function set the transceiver in and out of power save state. This is
 *      function is usually called by application layer of target device to enter 
 *      or exit power save mode. When power save mode is changed, the state is 
 *      saved into NVM.
 *
 * PreCondition:    
 *      MCU initialization has been finished. 
 *
 * Parameters: 
 *      BOOL bPowerSave - The boolean to indicate in or out of power save mode.
 *
 * Returns: 
 *      Status code
 *
 * Example:
 *      <code>
 *      RF4CE_PowerSave(TRUE);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BYTE RF4CE_PowerSave(BOOL bPowerSave);

/************************************************************************************
 * Function:
 *      BYTE RF4CE_SendFrame(BYTE PairIndex, BYTE ProfileID, WORD VendorID, TX_OPTIONS TxOptions)
 *
 * Summary:
 *      This function send a RF4CE data frame 
 *
 * Description:        
 *      This is the function that application layer calls to transmit a RF4CE data packet
 *
 * PreCondition:    
 *      Transceiver initialization has been done. 
 *
 * Parameters: 
 *      BYTE PairIndex  - The index of the target device on the pair table
 *      BYTE ProfileID  - The profile identifier for this packet
 *      WORD VendorID   - The vendor identifier, only valid if the vendorFrame bit is set
 *                        in TxOptions parameter
 *      TX_OPTIONS TxOptions    - The options on how to send this packet. The structure
 *                                of the TX_OPTIONS structures are
 *                                  typedef union
 *                                  {
 *                                      BYTE Val;
 *                                      struct
 *                                      {
 *                                          BYTE    broadcast       :1;
 *                                          BYTE    IEEEAddress     :1;
 *                                          BYTE    ackReq          :1;
 *                                          BYTE    secEn           :1;
 *                                          BYTE    singleChannel   :1;
 *                                          BYTE    channelDsgtor   :1;
 *                                          BYTE    vendorFrame     :1;
 *                                      } bits;        
 *                                  } TX_OPTIONS;
 *
 * Returns: 
 *      A byte to indicate transmission status.
 *
 * Example:
 *      <code>
 *      BYTE transStatus = RF4CE_SendFrame(PairIndex, ProfileID, VendorID, TxOptions);
 *
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BYTE RF4CE_SendFrame(BYTE PairIndex, BYTE ProfileID, WORD VendorID, TX_OPTIONS TxOptions);


/************************************************************************************
 * Function:
 *      BYTE RF4CE_SetChannel(BYTE Channel)
 *
 * Summary:
 *      This function set the operating channel for the RF transceiver
 *
 * Description:        
 *      This is the primary user interface function to set the operating
 *      channel for the RF transceiver. Valid channels are 15, 20 and 25.
 *      If input channel is not a valid value, the current operating channel 
 *      will not be change, and the return value will be an error code. Otherwise, 
 *      the return value will be TRUE.
 *
 * PreCondition:    
 *      Protocol initialization has been done.
 *
 * Parameters:           
 *      BYTE Channel -  The channel of the transceiver to be operating on. The 
 *                      possible channels are 15, 20 and 25.
 *                  
 * Returns: 
 *      a status code to indicate operation status
 *
 * Example:
 *      <code>
 *      if( SUCCESS == RF4CE_SetChannel(15) )
 *      {
 *          // channel changes successfully
 *      }
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *********************************************************************************/        
BYTE    RF4CE_SetChannel(BYTE Channel);

/*************************************************************************
 * Function:
 *        BYTE RF4CE_Start(BOOL bKeepPairTable)
 *   
 * Summary:
 *   This function start the RF4CE protocol
 *
 * Description:
 *   This function is called by application layer to start the RF4CE stack.
 *   This is the first function to call before working with RF4CE stack.
 *
 * Conditions:
 *   MCU initialization has been finished.
 *
 * Input:
 *   BOOL bKeepPairTable -  The boolean to indicate if the stack loads the
 *                          persistent data from NVM or start from scratch.
 *
 * Return:
 *   The status to indicate if operation successful
 *
 * Example:
 *   <code>
 *   RF4CE_Start(TRUE);
 *   </code>
 *
 * Remarks:
 *   None                                                                  
 *
 **************************************************************************/    
BYTE    RF4CE_Start(BOOL);

/************************************************************************************
 * Function:
 *      BYTE    RF4CE_TransceiverPowerState(BYTE Mode)
 *
 * Summary:
 *      This function put the RF transceiver into different power state. i.e. Put the 
 *      RF transceiver into sleep or wake it up.
 *
 * Description:        
 *      This is the primary user interface functions for the application layer to 
 *      put RF transceiver into sleep or wake it up. This function is used to disable 
 *      the transceiver to save battery power.
 *
 * PreCondition:    
 *      Protocol initialization has been done. 
 *
 * Parameters: 
 *      BYTE Mode - The mode of power state for the RF transceiver to be set. The possible
 *                  power states are following
 *                  * POWER_STATE_SLEEP     The deep sleep mode for RF transceiver
 *                  * POWER_STATE_WAKEUP    Wake up state, or operating state for RF transceiver
 *
 * Returns: 
 *      The status of the operation. The following are the possible status
 *      * SUCCESS           Operation successful
 *      * ERR_TRX_FAIL      Transceiver fails to go to sleep or wake up
 *      * ERR_INVLAID_INPUT Invalid input mode. 
 *
 * Example:
 *      <code>
 *      // put RF transceiver into sleep
 *      MiApp_TransceiverPowerState(POWER_STATE_SLEEP);
 *
 *      // Put the MCU into sleep
 *      Sleep();    
 *
 *      // wakes up the MCU by WDT, external interrupt or any other means
 *      
 *      // make sure that RF transceiver to wake up
 *      RF4CE_TransceiverPowerState(POWER_STATE_WAKEUP);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/
BYTE    RF4CE_TransceiverPowerState(BYTE Mode);

/************************************************************************************
 * Function:
 *      BYTE RF4CE_UnPair(BYTE PairIndex)
 *
 * Summary:
 *      This function remove the pair from the pair table
 *
 * Description:        
 *      This function is called by application layer to remove a pair currently in
 *      the pair table and return a status code to indicate the result.
 *
 * PreCondition:    
 *      Transceiver initialization has been finished. 
 *
 * Parameters: 
 *      BYTE PairIndex  - The pair table index of the node to be removed 
 *
 * Returns: 
 *      The status code to indicate the operation result.
 *
 * Example:
 *      <code>
 *      RF4CE_UnPair(0x00);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BYTE RF4CE_UnPair(BYTE PairIndex);

/************************************************************************************
 * Function:
 *      BYTE RF4CE_UpdateKey(BYTE PairIndex, BYTE *Key)
 *
 * Summary:
 *      This function change the security link key of a paired device 
 *
 * Description:        
 *      This function is called by application layer to change the security link key
 *      of a paired device. Both ends of the pair must call this function and change
 *      to the same key to have secured communication. 
 *
 * PreCondition:    
 *      Transceiver initialization has been finished. 
 *
 * Parameters: 
 *      BYTE PairIndex      - The index of pair table for the device to update security
 *                            key
 *      BYTE * Key          - The pointer to the new key
 *
 * Returns: 
 *      The status code to indicate the operation result.
 *
 * Example:
 *      <code>
 *      RF4CE_UpdateKey(PairIndex, Key);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BYTE RF4CE_UpdateKey(BYTE PairIndex, BYTE *newKey);


/*********** Functions called in stack, May not necessary in applicationlayer ************/
void RF4CETasks(void);	
BYTE findNextPairEntry(void);
BYTE SearchForLongAddress(void);
BYTE SearchForShortAddress(void);   
BOOL isSameAddress(INPUT BYTE *Address1, INPUT BYTE *Address2);


/*********************************** CALLBACK FUNCTIONS *********************************/

/*******************************************************************************
  Function:
         BOOL RF4CE_CB_AllowDiscoveryResponse(DISCOVERY_RESULT *DiscoveryRequestInitiator)
    
  Summary:
    This is the callback function for target device to decide if respond to
    a discovery request.
  Description:
    For target only: this function is a callback function from stack to the
    application layer to let application layer to decide if it is necessary
    to respond to a discovery request by sending out discovery response
    command. The decision is passed back to the stack by the return boolean
    value.
  Conditions:
    Transceiver initialization has been finished. A Discovery request has
    been received by the RF4CE stack layer.
  Input:
    DISCOVERY_RESULT * DiscoveryRequestInitiator -  The pointer to the
                                          DISCOVERY_RESULT data structure
                                          that contains information about
                                          the node that sends out discovery
                                          request
  Return:
    The boolean from the application layer to RF4CE stack layer to indicate
    if allows to respond to discovery request. Boolean value TRUE indicates
    allowing respond.
  Example:
    <code>
    BOOL RF4CE_CB_AllowDiscoveryResponse(DISCOVERY_RESULT *DiscoveryRequestInitiator)
    {
        // only respond to discovery from controller device
        if( (DiscoveryRequestInitiator-\>NodeCap &amp; 0x01) == 0 )
        {
            \return TRUE;
        }
        \return FALSE;
    }
    </code>
  Remarks:
    None                                                                        
  *******************************************************************************/ 
BOOL RF4CE_CB_AllowDiscoveryResponse(DISCOVERY_RESULT *DiscoveryRequestInitiator);


/*******************************************************************************
  Function:
         BOOL RF4CE_CB_AllowPair(BYTE PairIndex, BYTE *PairPayload, BYTE Status)
    
  Summary:
    This is the callback function for target device to decide how to
    respond to a pair request.
  Description:
    For target only: this function is a callback function from stack to the
    application layer to let application layer to decide if it is necessary
    to grant permission to pair a device. If permission is granted, the
    \return value will be boolean TRUE and the status of pair reponse
    command will be SUCCESS. Otherwise, the status of pair response will be
    NOT_PERMITTED.
  Conditions:
    Transceiver initialization has been finished. A pair request has been
    received by the RF4CE stack layer.
  Input:
    BYTE PairIndex -           The index of the potentail pair device in the
                               pair table
    BYTE * PairPayload -       Pointer to additional payload to identify the
                               device that issue the pair request. The pair
                               payload is in following format
                               VendorID(2 Byte) | VendorString(7 Byte) | App
                               Capability (1 Byte) | Optional App User
                               String(15 Byte) | Device Type List (Variable)
                               | Profile List (Variable) | Key Exchange
                               Count
    BYTE Status -              The status of the potential pair device in
                               the pair table. The possible status are
                                SUCCESS 
                                DUPLICATE_PAIRING 
                                NO_REC_CAPACITY
  Return:
    The boolean from the application layer to RF4CE stack layer to indicate
    if allows to pair. A boolean value of TRUE indicates allowing to pair.
    If the return value of this call function is FALSE, the pair response
    will have the status of NOT_PERMITTED.
  Example:
    <code>
    BOOL RF4CE_CB_AllowPair(BYTE PairIndex, BYTE *PairPayload, BYTE Status)
    {
        // only allow to pair controller device
        if( PairTable[PairIndex].NodeCap.bits.isTarget == 0 )
        {
            \return TRUE;
        }
        \return FALSE;
    }
    </code>
  Remarks:
    None                                                                        
  *******************************************************************************/ 
BOOL RF4CE_CB_AllowPair(BYTE PairIndex, BYTE *PairPayload, BYTE Status);


/**************************************************************************
  Function:
         BOOL BOOL RF4CE_CB_AllowUnPair(BYTE PairIndex)
    
  Summary:
    This is the callback function for target device to decide if OK to
    remove a pair record from pair table
  Description:
    For target only: this function is a callback function from stack to the
    application layer to let application layer to decide if it is OK to
    remove a pair record from the pair table. This callback function call
    will only be issued upon receiving an unpair request from a device in
    the pair table. The return boolean value will decide how RF4CE stack
    handles the unpair request. If return value is TRUE, the unpair request
    will be honored and such pair record will be removed. Otherwise, the
    pair record will not be removed. Noticed that the return value of
    RF4CE_CB_AllowUnPair is used on the node at the receiving side of
    unpair request. Even the return value is FALSE and the pair record will
    be kept at the receiving side, the node at the requesting side may
    still have removed the pair record per its application layer request.
  Conditions:
    Transceiver initialization has been finished. An unpair request has
    been received by the RF4CE stack layer.
  Input:
    BYTE PairIndex -  The index of the potentail unpair device in the pair
                      table
  Return:
    The boolean from the application layer to RF4CE stack layer to indicate
    if allows to unpair. A boolean value of TRUE indicates allowing to
    unpair. If the return value of this call function is FALSE, the pair
    will be kept, but the pair may still be removed from the pair table of
    unpair requesting device.
  Example:
    <code>
    BOOL RF4CE_CB_AllowUnPair(BYTE PairIndex)
    {
        // only allow to unpair controller device
        if( PairTable[PairIndex].NodeCap.bits.isTarget == 0 )
        {
            \return TRUE;
        }
        \return FALSE;
    }
    </code>
  Remarks:
    None                                                                   
  **************************************************************************/ 
BOOL RF4CE_CB_AllowUnPair(BYTE PairIndex); 


/*************************************************************************
  Function:
           void RF4CE_CB_StatusReport(BYTE status, BYTE PairIndex)
    
  Summary:
    This is the callback function from the RF4CE stack to the application
    to report communication status, especially communication status in the
    pairing process.
  Description:
    This is the callback function from the RF4CE stack to the application
    to report communication status, especially in the pairing process.
    Application layer generally can ignore this status report, unless the
    status is not SUCCESS. In the case a communication fails during the
    pairing process, either because of timeout in the process of sending a
    command, or timeout to receive certain command, the status will be set
    to represent failure reason.
  Conditions:
    Transceiver initialization has been finished.
  Input:
    BYTE status -     The status of the communication status.
    BYTE PairIndex -  The index of the communication destination device in
                      the pair table
  Return:
    None
  Example:
    <code>
    void RF4CE_CB_StatusReport(BYTE status, BYTE PairIndex)
    {
        // handle the communication error if pairing process fail.
        if( status != SUCCESS )
        {
            // handle the error
        }
    }
    </code>
  Remarks:
    None                                                                  
  *************************************************************************/ 
void RF4CE_CB_StatusReport(BYTE status, BYTE PairIndex); 

/*************************************************************************
 * void RF4CE_CB_Get64BitNodeAddress(BYTE *address)
 *************************************************************************/ 
void RF4CE_CB_Get64BitNodeAddress(BYTE *address);
/*************************************************************************
DWORD RF4CE_CB_GetFrameCounter(void)
 *************************************************************************/ 
DWORD RF4CE_CB_GetFrameCounter(void);
/*************************************************************************
 * void RF4CE_CB_PutFrameCounter(DWORD frameCounter)
 *************************************************************************/ 
void RF4CE_CB_PutFrameCounter(DWORD frameCounter);
/*****************************************************************************
 * BYTE RF4CE_CB_GetDefaultChannel(void)
 *****************************************************************************/
BYTE RF4CE_CB_GetDefaultChannel(void);
/*****************************************************************************
 * void RF4CE_CB_PutDefaultChannel(BYTE defaultChannel)
 *****************************************************************************/
void RF4CE_CB_PutDefaultChannel(BYTE defaultChannel);
/*****************************************************************************
 * BYTE RF4CE_CB_GetPowerSaveMode(void)
 *****************************************************************************/
BYTE RF4CE_CB_GetPowerSaveMode(void);
/*****************************************************************************
 * void RF4CE_CB_PutPowerSaveMode(BYTE powerSaveMode)
 *****************************************************************************/
void RF4CE_CB_PutPowerSaveMode(BYTE powerSaveMode);
/*****************************************************************************
 * WORD RF4CE_CB_GetShortAddress(void)
 *****************************************************************************/
WORD RF4CE_CB_GetShortAddress(void);
/*****************************************************************************
 * void RF4CE_CB_PutShortAddress(WORD shortAddress)
 *****************************************************************************/
void RF4CE_CB_PutShortAddres(WORD shortAddress);
/*****************************************************************************
 * WORD RF4CE_CB_GetShortAddress(void)
 *****************************************************************************/
WORD RF4CE_CB_GetPanID(void);
/*****************************************************************************
 * void RF4CE_CB_PutShortAddress(WORD shortAddress)
 *****************************************************************************/
void RF4CE_CB_PutPanID(WORD panID);
/*****************************************************************************
 * void RF4CE_CB_GetPairTable(BYTE *pairTable)
 *****************************************************************************/
void RF4CE_CB_GetPairTable(BYTE *pairTable);
/*****************************************************************************
 * void RF4CE_CB_PutShortAddress(WORD shortAddress)
 *****************************************************************************/
void RF4CE_CB_PutPairTableIndex(BYTE *pairTable, WORD index);
/*****************************************************************************
 * void RF4CE_CB_NVMClear(void)
 *****************************************************************************/
void RF4CE_CB_NVMClear(void);

/************************************************************************************
 * Function:
 *      void EnablePLL(void)
 *
 * Summary:
 *      This is the callback function from the RF4CE stack to enable PLL of MCU to run
 *      it full speed during secured pairing process.
 *
 * Description:        
 *      According to ZigBee RF4CE specification, for secured pairing, the reception side needs
 *      to streaming security key count number of seed packets to the joining device. Depending
 *      on the target device, the data rate for this seed procedure may be very high. Although
 *      the RF4CE stack already has build-in modules to recover from packet overflow, it is 
 *      still recommended that the joining device enables PLL to run higher speed in the short
 *      period of key seeding process. The RF4CE stack will call EnablePLL to enable PLL
 *      temporarily. After key seed procedure is over, callback function RestorePLL will be 
 *      called to restore to the original PLL setting. These functions are only need to implement
 *      if the device needs to actively paired with another device in security mode. In addition, 
 *      if the device runs with PLL already enabled, these two functions can be left unimplemented.
 *
 * PreCondition:    
 *      MCU has been properly initialized. 
 *
 * Parameters: 
 *      None
 *
 * Returns: 
 *      None
 *
 * Example:
 *      <code>
 *      BYTE backupPLL;
 *
 *      void EnablePLL(void)
 *      {
 *          backupPLL = OSCTUNEbits.PLLEN;
 *          OSCTUNEbits.PLLEN = 1;   
 *      }    
 *
 *      void RestorePLL(void)
 *      {
 *          OSCTUNEbits.PLLEN = backupPLL;
 *      } 
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
void EnablePLL(void);


/************************************************************************************
 * Function:
 *      void RestorePLL(void)
 *
 * Summary:
 *      This is the callback function from the RF4CE stack to enable PLL of MCU to run
 *      it full speed during secured pairing process.
 *
 * Description:        
 *      According to ZigBee RF4CE specification, for secured pairing, the reception side needs
 *      to streaming security key count number of seed packets to the joining device. Depending
 *      on the target device, the data rate for this seed procedure may be very high. Although
 *      the RF4CE stack already has build-in modules to recover from packet overflow, it is 
 *      still recommended that the joining device enables PLL to run higher speed in the short
 *      period of key seeding process. The RF4CE stack will call EnablePLL to enable PLL
 *      temporarily. After key seed procedure is over, callback function RestorePLL will be 
 *      called to restore to the original PLL setting. These functions are only need to implement
 *      if the device needs to actively paired with another device in security mode. In addition, 
 *      if the device runs with PLL already enabled, these two functions can be left unimplemented.
 *
 * PreCondition:    
 *      MCU has been properly initialized. 
 *
 * Parameters: 
 *      None
 *
 * Returns: 
 *      None
 *
 * Example:
 *      <code>
 *      BYTE backupPLL;
 *
 *      void EnablePLL(void)
 *      {
 *          backupPLL = OSCTUNEbits.PLLEN;
 *          OSCTUNEbits.PLLEN = 1;   
 *      }    
 *
 *      void RestorePLL(void)
 *      {
 *          OSCTUNEbits.PLLEN = backupPLL;
 *      } 
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/
void RestorePLL(void); 


#define SetResponseTimeout(timeout) {max_duty_cycle=timeout;}

/************************ EXTERNAL VARIABLES **********************/

extern BYTE TxBuffer[];
extern BYTE TxData;
extern WORD_VAL myPANID;
extern WORD_VAL myShortAddress;
extern BYTE tempLongAddress[MY_ADDRESS_LENGTH];
extern WORD_VAL tempShortAddress;
extern PAIR_TABLE_ENTRY    PairTable[MAX_PAIR_SIZE]; 
extern RECEIVED_MESSAGE rxMessage;
extern DWORD max_duty_cycle;
    
#endif

    
    
