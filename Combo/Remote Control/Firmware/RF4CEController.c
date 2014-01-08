/*****************************************************************************
 * FileName:        main.c
 * Dependencies:    see Include Section
 * Processor:       PIC24F
 * Compiler:       	See Release Notes
 * Linker:          See Release Notes
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 *****************************************************************************/

/*****************************************************************************
 * Section:  Includes
 *****************************************************************************/
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "RF4CEController.h"

// this is used to store the current device selection. It is used both 
// as a record at run time and also saved in NVM in case the power of the
// remote is removed. The selected device must be retained until the user
// changes the selection.
RF4CE_SELECTED_DEVICE CurrentActiveDevice = {"MCHPRF4CE", 0xFF};

/*****************************************************************************
 * BYTE RF4CEControllerInit(void)
 *****************************************************************************/
BYTE RF4CEControllerInit(void)
{
    BYTE validEntryIndex = 0xFF;   
    RF4CE_SELECTED_DEVICE savedDevice;
    WORD i;
    BYTE status;
    
    status = RF4CE_Start(TRUE);

    // verify first if the table is really valid
    if (RF4CEControllerCheckPairTable(&validEntryIndex) != 0)
    {

        // check if the NVM is fresh
        RF4CE_ControllerNVM_GetSelectedDevice((BYTE*)&savedDevice);

        // get the saved device index in case NVM is valid
        CurrentActiveDevice.activeDevice = savedDevice.activeDevice;

        for(i = 0; i < RF4CE_NVM_SIGNATURE_LENGTH; i++)
        {
            if (savedDevice.signature[i] != CurrentActiveDevice.signature[i])
            {
                // NVM is not valid, assign the first valid entry as selected device
                CurrentActiveDevice.activeDevice = validEntryIndex;
                break;
            }
        }
    }
    else
    {
        RF4CE_Start(FALSE);
        CurrentActiveDevice.activeDevice = 0xFF;
    }  

    if (CurrentActiveDevice.activeDevice != savedDevice.activeDevice)
    {
        // clear the NVM and write the initialized device index
        RF4CE_ControllerNVM_Clear();
        RF4CE_ControllerNVM_PutSelectedDevice((BYTE*)&CurrentActiveDevice);
    }

    // this if (status) check is not required in actual application
    // this is added here to know if the RF4CE code is implemented or not
    // and to show in the GUI the status of RF4CE implementation.
    if (status == REMOTE_NORF_MODE)
    {
        // this means RF4CE mode is disabled or RF4CE code is not present in the project.
        return REMOTE_NORF_MODE;
    }
    else
        return REMOTE_RF4CE_MODE;
}

/*****************************************************************************
 * BYTE RF4CEControllerCheckPairTable(BYTE *pFirstValidEntryIndex)
 * 
 * Overview: Checks the pair table.
 * 
 * Input: Pointer to byte variable that will contain the index
 *        number of the first valid entry. 
 * 
 * Output: returns the number of valid entries, when table is empty
 *         value returned is zero.
 * 
 *****************************************************************************/
BYTE RF4CEControllerCheckPairTable(BYTE *pFirstValidEntryIndex)
{
    UINT i, count = 0;
    BOOL validEntryFound = FALSE;
    
    // verify first if the table is really valid
    // to do this check that the channels are valid (should only be channels: 15, 20, 25)
    for(i = 0; i < MAX_PAIR_SIZE; i++)
    {
        if (RF4CEControllerCheckPairTableEntry(i) == TRUE)
        {
            if (validEntryFound == FALSE)
            {
                *pFirstValidEntryIndex = i;
                validEntryFound = TRUE;
            }
            count++;
        }
    }

    return (BYTE)count;
}

/*****************************************************************************
 * BOOL RF4CEControllerCheckPairTableEntry(BYTE index)
 * 
 * Overview: Checks if the pair table entry is valid.
 * 
 * Input: index number of the entry in the table. 
 * 
 * Output: returns TRUE if valid and FALSE if invalid.
 * 
 *****************************************************************************/
BOOL RF4CEControllerCheckPairTableEntry(BYTE index)
{
    // verify first if index is valid
    if (index < MAX_PAIR_SIZE)
    {
        // check if the entry is using the valid channels
        if( (PairTable[index].Channel == 15) ||  \
            (PairTable[index].Channel == 20) ||  \
            (PairTable[index].Channel == 25))
        {
            // now check if the entry is valid
            if( PairTable[index].NodeCap.bits.isValid == TRUE)
                return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************
 * BOOL RF4CEControllerParing(void)
 *****************************************************************************/
BOOL RF4CEControllerPairing(void)
{
    BYTE i;
    BYTE DiscoveryNum;
    BYTE status;

    /***********************************************************************************/
    // RF4CE API RF4CE_Discovery is the function call to discover other RF4CE target
    // device in the neighborhood.
    // The signature of function RF4CE_Discovery is
    //
    //      BYTE RF4CE_Discovery(   WORD DestPANID, WORD DestAddr, DWORD ScanDuration, 
    //                              DWORD ChannelMap, BYTE RequestDevType)    // 
    // DestPANID is the destination PAN identifier of the device to be discovered. 
    // If the application wants to discover all devices, put 0xFFFF here.
    //
    // DestAddr is the destination network address of the device to be discovered. 
    // If the application wants to discover all devices, put 0xFFFF here.
    //
    // ScanDuration is the scan time on each channel in symbol. One
    // symbol is 16us, according to IEEE 802.15.4 specification. The scan time is 
    // only valid if it is higher than 1/3 of NWK_DISCOVERY_REPETITION_INTERVAL.
    //
    // ChannelMap is the bit map of channels to perform the discovery.
    // For the RF4CE protocol, only channel 15, 20 and 25 are valid channels, so the
    // bit map can only be a subset of 0x02108000.
    // 
    // RequestDevType is the device type of the device to be discovered.
    // If application wants to discover all devices, put 0xFF here.
    // 
    // The return value is the number of discovered nodes.
    /***********************************************************************************/
    DiscoveryNum = RF4CE_Discovery(0xFFFF, 0xFFFF, (0x016E36/2), (0x02108000), 0xFF);
    //DiscoveryNum = RF4CE_Discovery(0xFFFF, 0xFFFF, (0x016E36), (0x02000000), 0xFF);
    if( DiscoveryNum > 0 )
    {

        /**********************************************************************************/
        // RF4CE API RF4CE_Pair is the function call the pair two devices together
        // The signature of function RF4CE_Pair is
        //
        //      BYTE RF4CE_Pair(BYTE DiscoveryIndex, BYTE *PairIndex)
        // 
        // Input parameter DiscoveryIndex is the index of the discovery table for the 
        // device to be paired. Usually, pair operation happens after a successful
        // discovery
        // Output parameter PairIndex is the index of the pair table for the device paired,
        // if the pairing status is successful.
        // 
        // The return value is the status byte to indicate if pairing procedure successful
        /**********************************************************************************/
        status = RF4CE_Pair(0, &i); 
        if((status == SUCCESS) || (status == DUPLICATE_PAIRING))
        {
            // check if pairing was really a success
            if (i == 0xFF)
            {
                // pair failed
                return FALSE;
            }
            // set the newly discovered item as the active device
            RF4CEControllerSetActiveDevice(i);
            return TRUE;
        }   
    }   
   
    return FALSE;
}

/*****************************************************************************
 * BOOL RF4CEControllerParing(void)
 *****************************************************************************/
void RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_CMD cmd)
{
    TX_OPTIONS TxOptions;

    TxData = 0;
    TxBuffer[TxData++] = ZRC_USER_CONTROL_PRESSED;
    TxBuffer[TxData++] = (BYTE)cmd;
    
    
    /*************************************************************************************/
    // TxOptions decides how to transmit the data
    // The defintion of TxOptions structure is
    //
    //      typedef union
    //      {
    //          BYTE Val;
    //          struct
    //          {
    //              BYTE    broadcast       :1;     // 1: broadcast, 0: unicast
    //              BYTE    IEEEAddress     :1;     // 1: IEEE destination address, 
    //                                              // 0: Network destination address
    //              BYTE    ackReq          :1;     // 1: required acknowledgement, 
    //                                              // 0: acknowledgement not required
    //              BYTE    secEn           :1;     // 1: secure the outgoing packet, only 
    //                                              // valid in unicast
    //                                              // 0: not secure the outgoing packet
    //              BYTE    singleChannel   :1;     // 1: single channel transmission
    //                                              // 0: transmit on all 3 channels in 
    //                                              // broadcast, or transmit on all 3 
    //                                              // channels if acknowledgement is not 
    //                                              // received in unicast with acknowledgement
    //              BYTE    channelDsgtor   :1;     // 1: Channel designator on, 
    //                                              // 0: Channel designator off
    //              BYTE    vendorFrame     :1;     // 1: Vendor specific frame, 
    //                                              // 0: normal profile frame
    //          } bits;        
    //      } TX_OPTIONS;
    //
    /************************************************************************************/
    TxOptions.Val = 0;
    TxOptions.bits.ackReq = 1;
    
    /************************************************************************************/
    // RF4CE API RF4CE_SendFrame is the function to send message
    // The sigunature of function RF4CE_SendFrame is
    //
    //      BYTE RF4CE_SendFrame(   BYTE PairIndex, BYTE ProfileID, WORD VendorID, 
    //                              TX_OPTIONS TxOptions )
    //
    // Input parameter PairIndex is the index of the pair table that represents the 
    // destination node.
    // Input parameter ProfileID is the profile identifier of the message belongs to
    // Input parameter VendorID is the vendor identifier. It is only valid for vendor
    // specific messages.
    // Input parameter TxOptions is the options to notify RF4CE stack how to transmit
    // the message.
    //
    // The return value is the status byte to indicate the transmission status.
    /*************************************************************************************/
    RF4CE_SendFrame(CurrentActiveDevice.activeDevice, ZRC_PROFILE_ID, VENDOR_IDENTIFIER, TxOptions);

}

/*****************************************************************************
 * BOOL RF4CEControllerSetActiveDevice(BYTE index)
 *****************************************************************************/
BOOL RF4CEControllerSetActiveDevice(BYTE index)
{
    BYTE numOfEntries;
    BOOL status = TRUE;

    // check if we need to change the index
    if (CurrentActiveDevice.activeDevice != index)
    {
        // get max range
        numOfEntries = (sizeof(PairTable))/sizeof(PAIR_TABLE_ENTRY); 
        // check if outside the range
        if (index >= numOfEntries)
        {
            return FALSE;
        }
        // check if the no active device is being set 
        else if (index == 0xFF)
        {
            CurrentActiveDevice.activeDevice = index;
            status = FALSE;
        }
        else
        {
            // check if the index chosen has a valid entry
            if( PairTable[index].NodeCap.bits.isValid == TRUE)
            {             
                CurrentActiveDevice.activeDevice = index;
            }
            else
                return FALSE;
    
        }  
        RF4CE_ControllerNVM_Clear();
        RF4CE_ControllerNVM_PutSelectedDevice((BYTE*)&CurrentActiveDevice);
    }  
    return status;    
    
}

/*****************************************************************************
 * BYTE RF4CEControllerSetActiveDevice(void)
 *****************************************************************************/
BYTE RF4CEControllerGetActiveDevice(void)
{
    return CurrentActiveDevice.activeDevice;    
}


