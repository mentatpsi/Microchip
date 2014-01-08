/********************************************************************
* FileName:		RF4CE.c
* Dependencies:    
* Processor:	PIC18, tested with 18F87J11	
* Hardware:		PIC18 Explorer
* Complier:     Microchip C18 v3.20 or higher		
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice for RF4CE Software:
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
*   This is a dummy file for ZigBee RF4CE stack. This is just for
*   demo and evaluation purposes of the Universal Remote Control Demo.
*
********************************************************************/

/************************ HEADERS **********************************/

#warning The Remote control demo source code does not include RF4CE source code. To evaluate RF4CE demo, please program the PIC with the pre-programmed HEX file ‘Combo - Remote Control PIC24_DA210_RCB_RF4CE.hex’ located in the Pre-compiled Hex folder. To develop your application with RF4CE stack, please contact local Microchip Sales to request the RF4CE source code (RF4CE.c; MRF24J40_RF4CE.c). 

#include "ConfigApp.h"

#include <stdlib.h>
#include "RF4CE.h"
#include "ConfigRF4CE.h"
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "WirelessProtocols/Console.h"  
#ifdef __PIC24FJ256DA210__
#include "NVM.h"
#else
#include "Common/NVM.h"
#endif
#include "WirelessProtocols/SymbolTime.h"
#include "MCHP_MAC.h"
#include "Transceivers.h"
#include "GraphicsUserInterface.h"

/************************ VARIABLES ********************************/
// random number generator, use standard library
#define Random()        rand()
#define RandomSeed(a)   srand(a)

// permanent address definition
BYTE rf4ceLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4, EUI_5,EUI_6,EUI_7};

// Beacon result, used by target in cold start process to acquire PANIDs of neighboring
// IEEE 802.15.4 networks. Using beacon is mainly to avoid PANID confliction at start up
#if defined(RF4CE_TARGET) 
    #define BEACON_RESULT_SIZE  10
    BEACON_RESULT BeaconResults[BEACON_RESULT_SIZE];
#endif

BYTE                rf4ceCurrentChannel;                 // current operating channel for the device
RF4CE_TICK          rf4ceCurrentTick;
RF4CE_TICK          rf4ceDiffTick;
RECEIVED_MESSAGE    rxMessage;                    // structure to store information for the received packet

BYTE                rf4ceDiscoveryResultIndex;
DISCOVERY_RESULT    rf4ceDiscoveryResults[DISCOVERY_RESULT_SIZE];  // The results for active scan, including
                                                                // the PAN identifier, signal strength and 
                                                                // operating channel

DWORD max_duty_cycle = NWKC_MAX_DUTY_CYCLE;

// Temporary variables
BYTE tempLongAddress[MY_ADDRESS_LENGTH];
WORD_VAL tempShortAddress;
WORD_VAL tempPANID;

// Transmission buffer
BYTE            TxData;    
#if defined(__18CXX)
    #pragma udata TRX_BUFFER=0x180
#endif
    BYTE TxBuffer[TX_BUFFER_SIZE];
#if defined(__18CXX)
    #pragma udata
#endif

// Incoming frame counter, used to avoid replay attack
DWORD_VAL IncomingFrameCounter[MAX_PAIR_SIZE];
// Output frame counter
DWORD_VAL RF4CEOutFrameCounter;

// My current short address, PAN Identifier and default channel
WORD_VAL myShortAddress;
WORD_VAL myPANID;
BYTE myDefaultChannel;

// State machine settings
RF4CE_STATE_MACHINE RF4CEStateMachine;
RF4CE_STATUS RF4CEStatus;

// Variables used in MiMAC layer
MAC_TRANS_PARAM MTP;
MAC_RECEIVED_PACKET MACRxPacket;


// Pairing table. Should be the same as the table stored
// in NVM
#if defined(__18CXX)
    #pragma udata BIGvariables1 = 0x300
#endif
PAIR_TABLE_ENTRY    PairTable[MAX_PAIR_SIZE]; 
#if defined(__18CXX)
    #pragma udata
#endif

// Discovery information, used in discovery process
typedef struct 
{
    BYTE            DiscoveryDeviceType;
    BYTE            NumDiscoveryProfiles;
    BYTE            DiscoveryProfiles[7];
} DISCOVERY_INFO;
DISCOVERY_INFO DiscoveryInfo;

// Pair information, used in pairing process
struct
{
    BOOL        isValid;
    BYTE        PairStatus;
    BYTE        PairIndex;
    RF4CE_TICK  PairTick;
} PairInfo;


#if defined(ENABLE_RF4CE_SECURITY)
    // temporary seed block, used to avoid random seed data drop
    // on both joining and reception ends
    #if defined(__18CXX)
        #pragma udata SeedData = 0x200
    #endif
    BYTE    SeedData[80];
    BYTE    BackupSeedData[80];
    BYTE    SeedProcessBlock[16];
    BYTE    KeyBlock[16];
    #if defined(__18CXX)
        #pragma udata
    #endif
    
    // Ping information, used in ping process
    struct
    {
        union
        {
            BYTE    Val;
            struct
            {
                BYTE isValid            :1;
                BYTE isSender           :1;
                BYTE PingRequestSent    :1;
            } bits;
        } status;
        BYTE        PairIndex;
        RF4CE_TICK  PingTick;
        BYTE        PingPayload[4];
    } PingInfo;
    
    // Key exchagne information, used in key seed exchange process.
    struct
    {
        union
        {
            BYTE Val;
            struct
            {
                BYTE isValid    :1;
                BYTE isSender   :1;
            } bits;
        } status;
        BYTE        PairTableIndex;
        BYTE        KeyExchangeCount;
        BYTE        KeySeedIndex;
        RF4CE_TICK  KeySeedTick;
    } KeyExchangeInfo;
    
    // temporary time count variable, used to roughly check timeout in a fast way
    // to keep up with key seed information, which is sent continuously with minimum
    // interval
    WORD TimeCount = 0;
#endif

// Ticker for target power save mode 
RF4CE_TICK PowerSaveTick;

BYTE PairIndex;
BYTE RspStatus;

/************************ FUNCTIONS ********************************/

BOOL SendMACPacket(BYTE *PANID, BYTE *Address, BYTE PacketType, BYTE ModeMask);
BOOL SendRF4CEPacket(BYTE *PANID, BYTE *Address, BYTE PacketType, BYTE ModeMask, BYTE ProfileID, WORD VendorID);
extern void RF4CEDiscoveryCallBack();

// C18 macros, such as ClrWdt and Sleep will affect the optimziation in the function
// that calls these macros. Here we use a separate functions to elimilate the impact of
// using macros on optimzation engine on big functions like RF4CETasks.
#if defined(RF4CE_TARGET)
    void MCUSleep(void)
    {
        ClrWdt();
        WDTCONbits.SWDTEN = 1;
        Sleep();
        WDTCONbits.SWDTEN = 0;
    }
#endif

/*********************************************************************
 * Function:        void RF4CETasks( void )
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
 * Overview:        This function maintains the operation of the stack
 *                  It should be called as often as possible.  
 ********************************************************************/
void RF4CETasks(void)
{
}



/************************************************************************************
 * Function:
 *      BOOL SendMACPacket(BYTE *PANID, BYTE *Address, BYTE PacketType, BYTE ModeMask)
 *
 * Summary:
 *      This function directly call MAC layer to send a packet
 *
 * Description:        
 *      This is the function that every packet go through to send the packet via the 
 *      interface similar to MiMAC.
 *
 * PreCondition:    
 *      Transceiver initialization has been done. 
 *
 * Parameters: 
 *      BYTE * PANID    - The pointer to PAN identifier of the destination
 *      BYTE * Address  - The pointer to the packet destination address
 *      BYTE PacketType - The type of the packet. There are three possible packet
 *                        type: data, command and beacon, which used RESERVED to
 *                        agree with MiMAC interface
 *      BYTE ModeMask   - The bit map of mode to transmit data. The possible modes are:
 *                          MSK_SRC_ABSNT                               
 *                          MSK_ACK_NOT_REQ 
 *                          MSK_ALT_SRC_ADDR
 *                          MSK_ALT_DST_ADDR
 *                          MSK_DISABLE_RETRANS
 *
 * Returns: 
 *      A boolean to indicates if transmission is successful.
 *
 * Example:
 *      <code>
 *      SendMACPacket(  DestPANID, DestAddress, PACKET_TYPE_DATA, 
 *                      (MSK_ALT_DST_ADDR | MSK_ALT_SRC_ADDR));
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BOOL SendMACPacket(BYTE *PANID, BYTE *Address, BYTE PacketType, BYTE ModeMask)
{   
    // act like a packet was sent
    return TRUE;
}



/************************************************************************************
 * Function:
 *      BOOL SendRF4CEPacket(BYTE *PANID, BYTE *Address, BYTE PacketType, BYTE ModeMask, 
 *                      BYTE ProfileID, WORD VendorID)
 *
 * Summary:
 *      This function construct a valid RF4CE packet by adding a RF4CE header and then 
 *      send it out by calling function SendMACPacket.
 *
 * Description:        
 *      This is the function that every valid RF4CE packet go through to be transmitted
 *
 * PreCondition:    
 *      Transceiver initialization has been done. 
 *
 * Parameters: 
 *      BYTE * PANID    - The pointer to PAN identifier of the destination
 *      BYTE * Address  - The pointer to the packet destination address
 *      BYTE PacketType - The type of the packet. There are three possible packet
 *                        type: data and command
 *      BYTE ModeMask   - The bit map of mode to transmit data. The possible modes are:
 *                          MSK_SRC_ABSNT                               
 *                          MSK_ACK_NOT_REQ 
 *                          MSK_ALT_SRC_ADDR
 *                          MSK_ALT_DST_ADDR
 *                          MSK_DISABLE_RETRANS
 *                          MSK_RF4CE_SECURITY
 *                          MSK_RF4CE_CHANNEL_DSG
 *                          MSK_RF4CE_ASIS
 *      BYTE ProfileID  - The profile identifier of the packet
 *      WORD VendorID   - The vendor identifier, only valid if the packet type is 
 *                        PACKET_TYPE_RF4CE_VENDOR_DATA
 *
 * Returns: 
 *      A boolean to indicates if transmission is successful.
 *
 * Example:
 *      <code>
 *      SendRF4CEPacket(DestPANID, DestAddress, PACKET_TYPE_DATA, 
 *                      (MSK_ALT_DST_ADDR | MSK_ALT_SRC_ADDR), 0x01, 0xFFF1);
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *****************************************************************************************/ 
BOOL SendRF4CEPacket(BYTE *PANID, BYTE *Address, BYTE PacketType, BYTE ModeMask, BYTE ProfileID, WORD VendorID)
{
    return TRUE;
}    



#if defined(RF4CE_TARGET)
    /************************************************************************************
     * Function:
     *      void BeaconRequest(DWORD ScanDuration, DWORD ChannelMap)
     *
     * Summary:
     *      This function tries to perform IEEE 802.15.4 beacon request to discover
     *      IEEE 802.15.4 PAN in the neighborhood to avoid PAN identifier confliction
     *
     * Description:        
     *      This function is function RF4CE_Start to perform IEEE 802.15.4 beacon
     *      request to avoid PAN identifier. Usually, application layer does not 
     *      need to worry to handle beacon in RF4CE stack.
     *
     * PreCondition:    
     *      Transceiver initialization has been finished. 
     *
     * Parameters: 
     *      DWORD ScanDuration      - The scan duration on each channel in symbols. One 
     *                                symbol is equal to 16us. 
     *      DWORD ChannelMap        - The bit map of channels to be scanned. The valid 
     *                                channels are 15, 20 and 25. Any other channels 
     *                                bitmap setting will be ignored.
     *
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      BeaconRequest(ONE_SECOND, 0x02108000);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    void BeaconRequest(INPUT DWORD ScanDuration, DWORD ChannelMap)
    {
    }
#endif



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
BYTE RF4CE_SendFrame(BYTE PairIndex, BYTE ProfileID, WORD VendorID, TX_OPTIONS TxOptions)
{
    
    return SUCCESS;
}    


#if defined(RF4CE_TARGET)
    /************************************************************************************
     * Function:
     *      void RF4CE_PowerSave(BOOL bPowerSave)
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
     *      None
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
    BYTE RF4CE_PowerSave(BOOL bPowerSave)
    {
        return SUCCESS;
    }    
#endif



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
BYTE RF4CE_TransceiverPowerState(BYTE Mode)
{
    return SUCCESS;
}   
    
 

/************************************************************************************
 * Function:
 *      BOOL RF4CE_SetChannel(BYTE Channel)
 *
 * Summary:
 *      This function set the operating channel for the RF transceiver
 *
 * Description:        
 *      This is the primary user interface function to set the operating
 *      channel for the RF transceiver. Valid channels are 15, 20 and 25.
 *      If input channel is not a valid value, the current operating channel 
 *      will not be change, and the return value will be FALSE. Otherwise, 
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
 *      a boolean to indicate if channel change has been performed successfully
 *
 * Example:
 *      <code>
 *      if( TRUE == RF4CE_SetChannel(15) )
 *      {
 *          // channel changes successfully
 *      }
 *      </code>
 *
 * Remarks:    
 *      None
 *
 *********************************************************************************/      
BYTE RF4CE_SetChannel(BYTE channel)
{
    return SUCCESS;
}    




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
BYTE RF4CE_UnPair(BYTE PairIndex)
{
    return SUCCESS;
}



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
BYTE RF4CE_Pair(BYTE DiscoveryIndex, BYTE *PairIndex)
{
    return SUCCESS;
}

#if defined(RF4CE_TARGET)
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
    BYTE RF4CE_AutoDiscovery(DWORD AutoDiscoveryDuration, BYTE *DiscoveryIndex)
    {
        return SUCCESS;
    } 
#endif   


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
 *      results will be stored in the rf4ceDiscoveryResults structure array.
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
BYTE RF4CE_Discovery(WORD DestPANID, WORD DestAddr, DWORD ScanDuration, DWORD ChannelMap, BYTE RequestDevType)
{
    return 0;
} 



#if defined(ENABLE_RF4CE_SECURITY)
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
    BYTE RF4CE_UpdateKey(BYTE PairIndex, BYTE *Key)
    {
        return SUCCESS;            
    }    
#endif


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
BOOL RF4CE_MessageAvailable(void)
{
    RF4CETasks();
    
    return FALSE;
}



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
void RF4CE_DiscardMessage(void)
{
}    



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
BYTE RF4CE_Start(BOOL bKeepPairTable)
{
    InitSymbolTimer();
    return REMOTE_NORF_MODE; // also means SUCCESS to emulate RF4CE but RF4CE is not present
}




#if defined(RF4CE_TARGET)
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
    BYTE RF4CE_NoiseDetection(INPUT DWORD ChannelMap, INPUT DWORD ScanDuration, OUTPUT BYTE *RSSIValue)
    {
        return 0;
    }

#endif



















