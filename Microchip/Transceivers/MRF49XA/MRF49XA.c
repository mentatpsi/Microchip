/********************************************************************
* FileName:		MRF49XA.c
* Dependencies:    
* Processor:	PIC18, PIC24, PIC32, dsPIC30, dsPIC33
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
*  This file provides transceiver driver functionality for MRF49XA 
*  subGHz transceiver. The transceiver driver interfaces are based 
*  on Microchip MAC strategy. The transceiver driver interfaces works 
*  with all Microchip wireless protocols
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/15/2009    yfy       Initial revision
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  2.1   6/20/2009    yfy       Add LCD support
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#include "SystemProfile.h"


#if defined(MRF49XA)
    #include "Transceivers/MCHP_MAC.h"
    #include "Transceivers/MRF49XA/MRF49XA.h"
    #include "WirelessProtocols/SymbolTime.h"
    #include "WirelessProtocols/Console.h"
    #include "TimeDelay.h"
    #include "WirelessProtocols/NVM.h"
    #include "Transceivers/Security.h"
    #include "Transceivers/crc.h"
    #include "WirelessProtocols/MCHP_API.h"

    //==============================================================
    // Global variables:
    //==============================================================
        
    /**********************************************************************
     * "#pragma udata" is used to specify the starting address of a 
     * global variable. The address may be MCU dependent on RAM available
     * If the size of the global variable is small, such manual assignment
     * may not be necessary. Developer can comment out the address
     * assignment.
     **********************************************************************/
    #if defined(__18CXX)
        #pragma udata MAC_RX_BUFFER
    #endif
        volatile RX_PACKET          RxPacket[BANK_SIZE];
    #if defined(__18CXX)
        #pragma udata
    #endif
    volatile TRANSCEIVER_STATUS TransceiverStatus;
    
    MACINIT_PARAM               MACInitParams;
    BYTE                        TxMACSeq;
    BYTE                        MACSeq;
    BYTE                        ReceivedBankIndex;
    
    #if defined(ENABLE_ACK)
        volatile    BOOL hasAck = FALSE;
        #if defined(ENABLE_RETRANSMISSION)
            ACK_INFO    AckInfo[ACK_INFO_SIZE];
        #endif
    #endif
    
    #if defined(ENABLE_SECURITY)
        DWORD_VAL OutgoingFrameCounter; 
        BYTE key[KEY_SIZE];
    #endif
    
    #if defined(__18CXX)
        #pragma udata MAC_TX_BUFFER
    #endif
    volatile BYTE        MACTxBuffer[TX_PACKET_SIZE];     
    #if defined(__18CXX)
        #pragma udata
    #endif

    void SPIPut(BYTE v);
    BYTE SPIGet(void);
    
    /*********************************************************************
     * WORD getReceiverBW(void)
     *
     * Overview:        
     *              This function get the receiver band width setting
     *              based on RF deviation configuration
     *
     * PreCondition:    
     *              RF deviation configuration has been done in the 
     *              C preprocessor
     *
     * Input:       None
     *
     * Output:      
     *          WORD    The configuration setting for receiver band width.
     *                  This output needs to be ORed with receiver 
     *                  configuration command
     *
     * Side Effects:    None
     *
     ********************************************************************/
    WORD getReceiverBW(void)
    {
        BYTE bw_table[16] = {6,6,6,5,5,4,4,4,3,3,2,2,1,1,1,1};   
        
        return ( ((WORD)(bw_table[(BYTE)((WORD)RF_DEV/15)])) << 5);
    }

    
    /*********************************************************************
     * void RegisterSet(INPUT WORD setting)
     *
     * Overview:        
     *              This function access the control register of MRF49XA.
     *              The register address and the register settings are
     *              the input
     *
     * PreCondition:    
     *              None
     *
     * Input:       
     *          WORD    setting     The address of the register and its
     *                              corresponding settings
     *
     * Output:  None    
     *
     * Side Effects:    Register settings have been modified
     *
     ********************************************************************/
    void RegisterSet(WORD setting)
    {
        BYTE oldRFIE = RFIE;
        
        RFIE = 0;
        PHY_CS = 0;
        SPIPut((BYTE)(setting >> 8));
        SPIPut((BYTE)setting);
        PHY_CS = 1;
        RFIE = oldRFIE;
    }
    
    /*********************************************************************
     * void StatusRead(void)
     *
     * Overview:        
     *              This function read the status register and output the
     *              status in the global variable TransceiverStatus.
     *
     * PreCondition:    
     *              MRF49XA transceiver has been properly initialized
     *
     * Input:       None   
     *
     * Output:      None    
     *
     * Side Effects:    Global variable TransceiverStatus has been modified
     *
     ********************************************************************/
    void StatusRead(void)
    {
        BYTE preNFSEL = nFSEL;
        BYTE preNCS = PHY_CS;
        BYTE oldRFIE = RFIE;
        
        RFIE = 0;
        nFSEL = 1;
        PHY_CS = 0;
        
        #if defined(HARDWARE_SPI)
        
            TransceiverStatus.v[0] = SPIGet();
            TransceiverStatus.v[1] = SPIGet();
            
        #else
            SPI_SDO = 0;

            TransceiverStatus.bits.RG_FF_IT = SPI_SDI;    
            SPI_SCK = 1;
            SPI_SCK = 0;
           
            TransceiverStatus.bits.POR = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
            
            TransceiverStatus.bits.RGUR_FFOV = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
            
            TransceiverStatus.bits.WKUP = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
            
            TransceiverStatus.bits.EXT = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
            
            TransceiverStatus.bits.LBD = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
            
            TransceiverStatus.bits.FFEM = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
            
            TransceiverStatus.bits.RSSI_ATS = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
                
            TransceiverStatus.bits.DQD = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
            
            TransceiverStatus.bits.CRL = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
            
            TransceiverStatus.bits.ATGL = SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
        #endif
        
        nFSEL = preNFSEL;
        PHY_CS = preNCS;
        RFIE = oldRFIE;
    }
    
    
    /*********************************************************************
     * BOOL TxPacket(INPUT BYTE TxPacketLen, INPUT BOOL CCA)
     *
     * Overview:        
     *              This function send the packet in the buffer MACTxBuffer
     *
     * PreCondition:    
     *              MRF49XA transceiver has been properly initialized
     *
     * Input:       
     *              BYTE    TxPacketLen     The length of the packet to be
     *                                      sent.
     *              BOOL    CCA             The boolean to indicate if a 
     *                                      CCA operation needs to be done
     *                                      before sending the packet   
     *
     * Output:      
     *              BOOL    The boolean to indicate if packet sent successful
     *
     * Side Effects:    
     *              The packet has been sent out
     *
     ********************************************************************/
    BOOL TxPacket(INPUT BYTE TxPacketLen, INPUT BOOL CCA)
    {
        BOOL status;
        BYTE TxPacketPtr;
        MIWI_TICK t1, t2;
        BYTE i;
        WORD counter;
        
        #ifdef ENABLE_CCA
            BYTE CCARetries;
        #endif
    
        #if defined(ENABLE_ACK) && defined(ENABLE_RETRANSMISSION)
            BYTE reTry = RETRANSMISSION_TIMES;
        #endif
                
        #if defined(ENABLE_ACK) && defined(ENABLE_RETRANSMISSION)
            while( reTry-- )
        #endif
        {
            BYTE allowedTxFailure;
            BYTE synCount;
            BYTE DQDOK;
            
            allowedTxFailure = 0;
Start_Transmitting:
    
            #if defined(ENABLE_ACK)
                hasAck = FALSE;
            #endif
            
            #ifdef ENABLE_CCA
                CCARetries = 0;
        
                if( CCA )
                {
                    RegisterSet(PMCREG | 0x0080);
Start_CCA:    
                    DQDOK = 0;    
                    for(i = 0; i < CCA_TIMES; i++)
                    {
                        StatusRead();
                        if( TransceiverStatus.bits.DQD )
                        {
                            DQDOK++;
                        }
                        Delay10us(1);
                    }
                    
                    if( DQDOK > CCA_THRESHOLD )
                    {
                        if(CCARetries++ > CCA_RETRIES )
                        {
                            return FALSE;
                        }
                        goto Start_CCA;
                    }
                }
            #endif
            
            RFIE = 0;
            
            // Turn off receiver, enable the TX register
            RegisterSet(PMCREG);
            RegisterSet(GENCREG | 0x0080);
            
            // enable transmitter
            RegisterSet(PMCREG | 0x0020);
            
            DelayMs(1);
             
            TxPacketPtr = 0;
            synCount = 0;

            PHY_CS = 0;

            SPIPut(0xB8);
            SPIPut(0xAA);                 // 3rd preamble
            
            counter = 0;
            while(TxPacketPtr < TxPacketLen + 2 )   // 2 dummy byte
            {
                if( SPI_SDI == 1 ) 
                {
                    if( synCount < 3 )
                    {
                        switch(synCount)
                        {
                            case 0:
                                SPIPut(0x2D); // first syn byte
                                break;
                            case 1:
                                SPIPut(0xD4); // second syn byte
                                break;
                            case 2:
                                SPIPut(TxPacketLen);
                                break;
                            default:
                                break;
                        }
                        synCount++;
                    }
                    else
                    {
                        SPIPut(MACTxBuffer[TxPacketPtr]);
                        TxPacketPtr++;
                    }
                    counter = 0;
                }
                else 
                {
                    if( counter++ > 0xFFFE )
                    {
                        PHY_CS = 1;
                        
                        if( allowedTxFailure++ > MAX_ALLOWED_TX_FAILURE )
                        {
                            break;
                        }
                        
                        RegisterSet(PMCREG | 0x0080);
                        
                        RegisterSet(GENCREG | 0x0040 );
                        RegisterSet(FIFORSTREG | 0x0002);
                        DelayMs(5);
                        RFIE = 1;
    
                        goto Start_Transmitting;
                    }
                }  
            }
            PHY_CS = 1;

            // Turn off the transmitter, disable the Tx register
            RegisterSet(PMCREG | 0x0080);
            RegisterSet(GENCREG | 0x0040 );
            
            RegisterSet(FIFORSTREG | 0x0002);
            
            StatusRead();
            
            RFIE = 1;
    
            #if defined(ENABLE_ACK) 
                if( (MACTxBuffer[0] & ACK_MASK) > 0 )        // required acknowledgement
                {
                    TxMACSeq = MACTxBuffer[1];
                    t1 = MiWi_TickGet();
                    while(1)
                    {
                        if( hasAck )
                        {
                            status = TRUE;
                            goto TX_END_HERE;
                        }
                        t2 = MiWi_TickGet();
                        if( MiWi_TickGetDiff(t2, t1) > ONE_SECOND/20 )
                        {
                            break;
                        }
                    }
                }
                else
            #endif
            {
                status = TRUE;
                goto TX_END_HERE;
            }
        }
        
        status = FALSE;
        
TX_END_HERE:    
        return status;
    }
    
    
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
    BOOL MiMAC_SetAltAddress(INPUT BYTE *Address, INPUT BYTE *PANID)
    {
        return FALSE;
    }
    
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
    BOOL MiMAC_SetChannel(INPUT BYTE channel, INPUT BYTE offsetFreq)
    {
        if( channel >= CHANNEL_NUM )    
        {
            return FALSE;   
        }

        RegisterSet(0xA000 + FREQ_START + ((WORD)channel * FREQ_STEP) + offsetFreq);
        DelayMs(20);       // delay 20 ms to stablize the transceiver
        return TRUE;   
    }
    
    
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
    BOOL MiMAC_SetPower(INPUT BYTE outputPower)
    {
        if( outputPower > TX_POWER_N_17_5_DB )
        {
            return FALSE;
        }
        RegisterSet(0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | outputPower);    
        return TRUE;
    }
    
    
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
    BOOL MiMAC_Init(INPUT MACINIT_PARAM initValue)
    {
        BYTE i;
         
        MACInitParams = initValue;
           
        PHY_CS = 1;           // nSEL inactive
        nFSEL = 1;           // nFFS inactive
        SPI_SDO = 0;        
        SPI_SCK = 0;        
    
        MACSeq = TMRL;
        ReceivedBankIndex = 0xFF;
        
        for(i = 0; i < BANK_SIZE; i++)
        {
            RxPacket[i].flags.Val = 0;
        }
        
        #if defined(ENABLE_ACK) && defined(RETRANSMISSION)
            for(i = 0; i < ACK_INFO_SIZE; i++)
            {
                AckInfo[i].Valid = FALSE;
            }
        #endif
    
        #if defined(ENABLE_SECURITY)
            #if defined(ENABLE_NETWORK_FREEZER)
                if( initValue.actionFlags.bits.NetworkFreezer )
                {
                    nvmGetOutFrameCounter(OutgoingFrameCounter.v);
                    OutgoingFrameCounter.Val += FRAME_COUNTER_UPDATE_INTERVAL;
                    nvmPutOutFrameCounter(OutgoingFrameCounter.v);
                }
                else
                {
                    OutgoingFrameCounter.Val = 0;
                    nvmPutOutFrameCounter(OutgoingFrameCounter.v);
                    OutgoingFrameCounter.Val = 1;
                }        
            #else
        	    OutgoingFrameCounter.Val = 1;
        	#endif
        	
            for(i = 0; i < KEY_SIZE; i++)
            {
                key[i] = mySecurityKey[i];
            }
        #endif
    
    
        //----  configuring the RF link --------------------------------
        RegisterSet(FIFORSTREG);
        RegisterSet(FIFORSTREG | 0x0002);                               // enable synchron latch
        RegisterSet(GENCREG);
        RegisterSet(AFCCREG);
        RegisterSet(0xA7D0);                                            
        RegisterSet(DRVSREG);
        RegisterSet(PMCREG);
        RegisterSet(RXCREG | getReceiverBW());
        RegisterSet(TXCREG);
        RegisterSet(BBFCREG);
        // antenna tuning on startup
        RegisterSet(PMCREG | 0x0020);                                   // turn on the transmitter
        DelayMs(5);                                                    // ant.tuning time (~100us) + Xosc starup time (5ms)
        // end of antenna tuning
        RegisterSet(PMCREG | 0x0080);                                   // turn off transmitter, turn on receiver
        RegisterSet(GENCREG | 0x0040);                                  // enable the FIFO
        RegisterSet(FIFORSTREG);
        RegisterSet(FIFORSTREG | 0x0002);                               // enable synchron latch
        RegisterSet(0x0000);										    // read status byte (read ITs)
        
        
        return TRUE;
    }
    
    
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
    BOOL MiMAC_SendPacket( INPUT MAC_TRANS_PARAM transParam, 
                        INPUT BYTE *MACPayload, 
                        INPUT BYTE MACPayloadLen)
    {
        BYTE i;
        BYTE TxIndex;
        WORD crc;
      
        if( MACPayloadLen > TX_BUFFER_SIZE )
        {
            return FALSE;
        }

        #if defined(INFER_DEST_ADDRESS)
            transParam.flags.bits.destPrsnt = 0;
        #else
            transParam.flags.bits.destPrsnt = (transParam.flags.bits.broadcast) ? 0:1;
        #endif
        
        #if !defined(SOURCE_ADDRESS_ABSENT)
            transParam.flags.bits.sourcePrsnt = 1;
        #endif
        
        if( transParam.flags.bits.packetType == PACKET_TYPE_COMMAND )
        {
            transParam.flags.bits.sourcePrsnt = 1;
        }
      
        MACTxBuffer[0] = transParam.flags.Val;
        MACTxBuffer[1] = MACSeq++;
        crc = CRC16((BYTE *)MACTxBuffer, 2, 0);
        
        TxIndex = 2;
        
        if( transParam.flags.bits.destPrsnt )
        {   
            for(i = 0; i < MACInitParams.actionFlags.bits.PAddrLength; i++)
            {
                MACTxBuffer[TxIndex++] = transParam.DestAddress[i];
            }
        }
        if( transParam.flags.bits.broadcast == 0 )
        {
            crc = CRC16(transParam.DestAddress, MACInitParams.actionFlags.bits.PAddrLength, crc);
        }
    
        if( transParam.flags.bits.sourcePrsnt )
        {   
            for(i = 0; i < MACInitParams.actionFlags.bits.PAddrLength; i++)
            {
                MACTxBuffer[TxIndex++] = MACInitParams.PAddress[i];
            }
            crc = CRC16((BYTE *)&(MACTxBuffer[TxIndex-MACInitParams.actionFlags.bits.PAddrLength]), MACInitParams.actionFlags.bits.PAddrLength, crc);
        }
    
        #if defined(ENABLE_SECURITY)
            if( transParam.flags.bits.secEn )
            {
    
                for(i = 0; i < 4; i++)
                {
                    MACTxBuffer[TxIndex++] = OutgoingFrameCounter.v[i];
                }
                OutgoingFrameCounter.Val++;
                #if defined(ENABLE_NETWORK_FREEZER)
                    if( (OutgoingFrameCounter.v[0] == 0) && ((OutgoingFrameCounter.v[1] & 0x03) == 0) )
                    {
                        nvmPutOutFrameCounter(OutgoingFrameCounter.v);
                    }    
                #endif
                MACTxBuffer[TxIndex++] = KEY_SEQUENCE_NUMBER;
    
                {
                    BYTE headerLen;
                    
                    headerLen = TxIndex;
                    
                    for(i = 0; i < MACPayloadLen; i++)
                    {
                        MACTxBuffer[TxIndex++] = MACPayload[i];
                    }
                    
                    #if SECURITY_LEVEL == SEC_LEVEL_CTR
                        {
                            BYTE nounce[BLOCK_SIZE];
                            
                            for(i = 0; i < BLOCK_SIZE; i++)
                            {
                                if( i < TxIndex )
                                {
                                    nounce[i] = MACTxBuffer[i];
                                }
                                else
                                {
                                    nounce[i] = 0;
                                }
                            }
                            CTR(&(MACTxBuffer[headerLen]), MACPayloadLen, key, nounce);
                        }
                    #elif (SECURITY_LEVEL == SEC_LEVEL_CCM_64) || (SECURITY_LEVEL == SEC_LEVEL_CCM_32) || (SECURITY_LEVEL == SEC_LEVEL_CCM_16)
                        CCM_Enc((BYTE *)MACTxBuffer, headerLen, MACPayloadLen, key);
                        TxIndex += SEC_MIC_LEN;
                    #elif (SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_64) || (SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_32) || (SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_16)
                        CBC_MAC(MACTxBuffer, TxIndex, key, &(MACTxBuffer[TxIndex]));
                        TxIndex += SEC_MIC_LEN;
                    #endif
                    
                    crc = CRC16((BYTE *)&(MACTxBuffer[headerLen-5]), (MACPayloadLen + SEC_MIC_LEN + 5), crc);
                }
            }
            else
    
        #endif
    
        {
            for(i = 0; i < MACPayloadLen; i++)
            {
                MACTxBuffer[TxIndex++] = MACPayload[i];
            }
            
            crc = CRC16(MACPayload, MACPayloadLen, crc);
        }    
    
    
        MACTxBuffer[TxIndex++] = (BYTE)(crc>>8);
        MACTxBuffer[TxIndex++] = (BYTE)crc;
          
        return TxPacket(TxIndex, MACInitParams.actionFlags.bits.CCAEnable);
    }
    

    BOOL MiMAC_ReceivedPacket(void)
    {
        BYTE i;
        MIWI_TICK currentTick;
        
        if( RF_INT_PIN == 0 )
        {
            RFIF = 1;
        }
        
        #if defined(ENABLE_ACK) && defined(ENABLE_RETRANSMISSION)
            for(i = 0; i < ACK_INFO_SIZE; i++)
            {
                currentTick = MiWi_TickGet();
                if( AckInfo[i].Valid && (currentTick.Val > AckInfo[i].startTick.Val) && 
                    (MiWi_TickGetDiff(currentTick, AckInfo[i].startTick) > ONE_SECOND) )
                {
                    AckInfo[i].Valid = FALSE;
                }
            }
        #endif
            
        if( ReceivedBankIndex != 0xFF )
        {
            return FALSE;
        }
       
        for(i = 0; i < BANK_SIZE; i++)
        {
            if( RxPacket[i].flags.bits.Valid )
            {
                BYTE PayloadIndex;
                BYTE j;

                MACRxPacket.flags.Val = RxPacket[i].Payload[0];
                MACRxPacket.PayloadLen = RxPacket[i].PayloadLen;
                PayloadIndex = 2;
                
                if( MACRxPacket.flags.bits.destPrsnt )
                {
                    PayloadIndex += MACInitParams.actionFlags.bits.PAddrLength;
                }
                
                if( MACRxPacket.flags.bits.sourcePrsnt )
                {
                    MACRxPacket.SourceAddress = (BYTE *)&(RxPacket[i].Payload[PayloadIndex]);
                    PayloadIndex += MACInitParams.actionFlags.bits.PAddrLength;
                }
                else
                {
                    MACRxPacket.SourceAddress = NULL;
                }
                
                #if defined(ENABLE_SECURITY)
                    if( MACRxPacket.flags.bits.secEn )
                    {
                        // check key sequence number first
                        if( KEY_SEQUENCE_NUMBER != RxPacket[i].Payload[PayloadIndex+4] )    
                        {
                            RxPacket[i].flags.Val = 0;
                            return FALSE;
                        }
                        
                        // check frame counter now
                        if( MACRxPacket.flags.bits.sourcePrsnt )
                        {
    
                            for(j = 0; j < CONNECTION_SIZE; j++)
                            {
                                if( (ConnectionTable[j].status.bits.isValid) && 
                                    isSameAddress(ConnectionTable[j].Address, MACRxPacket.SourceAddress) )
                                {
                                    break;
                                }
                            }
                            if( j < CONNECTION_SIZE )
                            {
                                DWORD_VAL FrameCounter;
                                BYTE k;
                                
                                for(k = 0; k < 4; k++)
                                {
                                    FrameCounter.v[k] = RxPacket[i].Payload[PayloadIndex+k];    
                                }
                                
                                if( IncomingFrameCounter[j].Val > FrameCounter.Val )
                                {
                                    RxPacket[i].flags.Val = 0;
                                    return FALSE;
                                }
                                else
                                {
                                    IncomingFrameCounter[j].Val = FrameCounter.Val;
                                }
                            }
                        }
                        
                        // now decrypt the data
                        PayloadIndex += 5;      // bypass the frame counter and key sequence number
                        
                        #if SECURITY_LEVEL == SEC_LEVEL_CTR
                            {
                                BYTE nounce[BLOCK_SIZE];
                                
                                for(j = 0; j < BLOCK_SIZE; j++)
                                {
                                    if( j < PayloadIndex )
                                    {
                                        nounce[j] = RxPacket[i].Payload[j];
                                    }
                                    else
                                    {
                                        nounce[j] = 0;
                                    }
                                }
                                
                                CTR(&(RxPacket[i].Payload[PayloadIndex]), (RxPacket[i].PayloadLen - PayloadIndex), key, nounce); 
                            }
                        #elif (SECURITY_LEVEL == SEC_LEVEL_CCM_64) || (SECURITY_LEVEL == SEC_LEVEL_CCM_32) || (SECURITY_LEVEL == SEC_LEVEL_CCM_16)

                            if(CCM_Dec((BYTE *)RxPacket[i].Payload, PayloadIndex, RxPacket[i].PayloadLen-PayloadIndex, key) == FALSE)
                            {
                                RxPacket[i].flags.Val = 0;
                                return FALSE;
                            }

                        #elif (SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_64) || (SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_32) || (SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_16)
                            {
                                BYTE MIC[BLOCK_SIZE];
                                
                                CBC_MAC(RxPacket[i].Payload, (RxPacket[i].PayloadLen - SEC_MIC_LEN), key, MIC);
                                for(j = 0; j < SEC_MIC_LEN; j++)
                                {
                                    if( MIC[j] != RxPacket[i].Payload[RxPacket[i].PayloadLen-SEC_MIC_LEN+j] )
                                    {
                                        RxPacket[i].flags.Val = 0;
                                        return FALSE;
                                    }    
                                }
                            }
                        #endif
                        MACRxPacket.PayloadLen -= (PayloadIndex + SEC_MIC_LEN);
                    }   
                    else
                    {
                        MACRxPacket.PayloadLen -= PayloadIndex;
                    }
    
                #else
                
                    MACRxPacket.PayloadLen -= PayloadIndex;
                
                #endif
                
                MACRxPacket.Payload = (BYTE *)&(RxPacket[i].Payload[PayloadIndex]);
                #if !defined(TARGET_SMALL)
                    if( RxPacket[i].flags.bits.RSSI )
                    {
                        #if RSSI_THRESHOLD == RSSI_THRESHOLD_103
                            MACRxPacket.RSSIValue = 1;
                        #elif RSSI_THRESHOLD == RSSI_THRESHOLD_97
                            MACRxPacket.RSSIValue = 7;
                        #elif RSSI_THRESHOLD == RSSI_THRESHOLD_91
                            MACRxPacket.RSSIValue = 13;
                        #elif RSSI_THRESHOLD == RSSI_THRESHOLD_85
                            MACRxPacket.RSSIValue = 19;
                        #elif RSSI_THRESHOLD == RSSI_THRESHOLD_79
                            MACRxPacket.RSSIValue = 25;
                        #elif RSSI_THRESHOLD == RSSI_THRESHOLD_73
                            MACRxPacket.RSSIValue = 31;
                        #endif
                    }
                    else
                    {
                        MACRxPacket.RSSIValue = 0;
                    }
                    MACRxPacket.LQIValue = RxPacket[i].flags.bits.DQD;
                #endif
                ReceivedBankIndex = i;
                
                return TRUE;
            }
        }
    
        return FALSE;    
    }
    
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
    void MiMAC_DiscardPacket(void)
    {
        if( ReceivedBankIndex < BANK_SIZE )
        {
            RxPacket[ReceivedBankIndex].flags.Val = FALSE;
            ReceivedBankIndex = 0xFF;    
        }
    }
    
    
    #if defined(ENABLE_ED_SCAN)
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
        BYTE MiMAC_ChannelAssessment(BYTE AssessmentMode)
        {
            BYTE i;
            BYTE j;
            BYTE k;
            BYTE result[6] = {222, 186, 150, 114, 78, 42};
            BYTE count;
            
            RegisterSet(PMCREG | 0x0080);    // turn on the receiver 
            for(i = 0; i < 6; i++)
            {
                RegisterSet(((RXCREG & 0xFF80) + 5-i) | getReceiverBW());

                count = 0;
                for(j = 0; j < 0xFE; j++)
                {
                    StatusRead();
                    if( AssessmentMode == CHANNEL_ASSESSMENT_CARRIER_SENSE )
                    {
                        count += TransceiverStatus.bits.DQD;
                    }
                    else if( AssessmentMode == CHANNEL_ASSESSMENT_ENERGY_DETECT )
                    {
                        count += TransceiverStatus.bits.RSSI_ATS;
                    }
                    PHY_CS = 1;
                    for(k = 0; k < 0x1F; k++) {}
                }
                
                #if defined(__18CXX)
                    if( OSCTUNEbits.PLLEN )
                    {
                        k = 10;
                    }
                    else
                #endif
                {
                    k = 5;
                }
                
                if( count > k * (7-(getReceiverBW()>>5)) )
                {
                    RegisterSet(RXCREG | getReceiverBW());
                    return result[i];
                }
            }

            RegisterSet(RXCREG | getReceiverBW());
            return 0;
        }
    #endif
    
    
    #if defined(ENABLE_SLEEP)
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
        BOOL MiMAC_PowerState(INPUT BYTE PowerState)
        {
            switch(PowerState)
            {
                case POWER_STATE_DEEP_SLEEP:
                    {   
                        RegisterSet(FIFORSTREG);                  // turn off FIFO
                        RegisterSet(GENCREG);                   // disable FIFO, TX_latch
                        RegisterSet(PMCREG);                    // turn off both receiver and transmitter
                        StatusRead();                           // reset all non latched interrupts
                        nFSEL = 1;
                    }
                    break;
                
                case POWER_STATE_OPERATE:
                    {
                        BYTE i;
                        
                        RegisterSet(PMCREG | 0x0008);           // switch on oscillator
                        DelayMs(10);                            // oscillator start up time 2~7 ms. Use 10ms here
                        #if defined(ENABLE_ACK)
                            for(i = 0; i < ACK_INFO_SIZE; i++)
                            {
                                AckInfo[i].Valid = FALSE;
                            }
                        #endif
                    }
                    break;
                    
                default:
                    return FALSE;
            }
            return TRUE;    
        }
    #endif
    
    
    #if defined(__18CXX)
        #if defined(HITECH_C18)
            #pragma interrupt_level 0
            void interrupt HighISR(void)
        #else
            #pragma interruptlow HighISR
            void HighISR(void)
        #endif
    #elif defined(__dsPIC30F__) || defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__)
        void _ISRFAST __attribute__((interrupt, auto_psv)) _INT1Interrupt(void)
    #elif defined(__PICC__)
        #pragma interrupt_level 0
        void interrupt HighISR(void)
    #elif defined(__PIC32MX__)
        void __ISR(_EXTERNAL_1_VECTOR, ipl4) _INT1Interrupt(void)
    #else
        void _ISRFAST _INT1Interrupt(void)
    #endif
    {   
        if( RFIE && RFIF )
        {
            PHY_CS = 0;
            #if defined(__dsPIC30F__) || defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__) || defined(__PIC32MX__)
                Nop();          // add Nop here to make sure PIC24 family MCU can respond to the SPI_SDI change
            #endif        
                         
            if( SPI_SDI == 1 )
            {
                BYTE RxPacketPtr;
                BYTE PacketLen;
                BYTE BankIndex;
                WORD counter;
                BOOL bAck;
                BYTE ackPacket[4];
                
                // There is data in RX FIFO
                PHY_CS = 1;
                nFSEL = 0;                   // FIFO selected
            
                PacketLen = SPIGet();

                for(BankIndex = 0; BankIndex < BANK_SIZE; BankIndex++)
                {
                    if( RxPacket[BankIndex].flags.bits.Valid == FALSE )
                    {
                        break;
                    }
                }
                
                if( PacketLen == 4 )        // may be an acknowledgement
                {
                    bAck = TRUE;
                }
                else
                {
                    bAck = FALSE;
                }

                if( PacketLen >= RX_PACKET_SIZE || PacketLen == 0 || (BankIndex >= BANK_SIZE && (bAck==FALSE)) )
                {
IGNORE_HERE:       
                    nFSEL = 1;                                       // bad packet len received
                    RFIF = 0;
                    RegisterSet(PMCREG);                            // turn off the transmitter and receiver
                    RegisterSet(FIFORSTREG);                          // reset FIFO
                    RegisterSet(GENCREG);                           // disable FIFO, TX_latch
                    RegisterSet(GENCREG | 0x0040);                  // enable FIFO
                    RegisterSet(PMCREG | 0x0080);                   // turn on receiver
                    RegisterSet(FIFORSTREG | 0x0002);                 // FIFO synchron latch re-enabled
                    goto RETURN_HERE;
                }
                
                RxPacketPtr = 0;
                counter = 0;

                while(1)
                {
                    if(FINT == 1)
                    {
                        if( bAck )
                        {
                            ackPacket[RxPacketPtr++] = SPIGet();
                        }
                        else
                        {
                            RxPacket[BankIndex].Payload[RxPacketPtr++] = SPIGet();
                        }
                        
                        if( RxPacketPtr >= PacketLen ) //RxPacket[BankIndex].PayloadLen )
                        {
                            WORD received_crc;
                            WORD calculated_crc;
                            BYTE i;

                            StatusRead();

                            if( TransceiverStatus.bits.DQD == 0 )
                            {
                                goto IGNORE_HERE;
                            }
                            
                            nFSEL = 1;
                            RegisterSet(FIFORSTREG);
                           
                            
                            if( bAck )
                            {
                                #if defined(ENABLE_ACK)
                                if( ( ackPacket[0] & PACKET_TYPE_MASK ) == PACKET_TYPE_ACK )
                                {
                                    received_crc  = ((WORD)ackPacket[3]) + (((WORD)ackPacket[2]) << 8);
                                    calculated_crc = CRC16(ackPacket, 2, 0);
                                    if( received_crc != calculated_crc)
                                    {
                                        RxPacketPtr = 0;
                                        RegisterSet(FIFORSTREG | 0x0002);
                                        goto IGNORE_HERE;
                                    }
                                    if( ackPacket[1] == TxMACSeq )
                                    {
                                        hasAck = TRUE;
                                    }
                                    RxPacketPtr = 0;
                                    RegisterSet(FIFORSTREG | 0x0002);
                                    goto RETURN_HERE;
                                }
                                else
                                #endif
                                if( BankIndex >= BANK_SIZE )
                                {
                                    RxPacketPtr = 0;
                                    RegisterSet(FIFORSTREG | 0x0002);
                                    goto IGNORE_HERE;
                                }
                                RxPacket[BankIndex].Payload[0] = ackPacket[0];
                                RxPacket[BankIndex].Payload[1] = ackPacket[1];
                                RxPacket[BankIndex].Payload[2] = ackPacket[2];
                                RxPacket[BankIndex].Payload[3] = ackPacket[3];
                            }
                            
                            RxPacket[BankIndex].PayloadLen = PacketLen;

                            // checking CRC
                            received_crc = ((WORD)(RxPacket[BankIndex].Payload[RxPacket[BankIndex].PayloadLen-1])) + (((WORD)(RxPacket[BankIndex].Payload[RxPacket[BankIndex].PayloadLen-2])) << 8);
                            if( (RxPacket[BankIndex].Payload[0] & BROADCAST_MASK) || (RxPacket[BankIndex].Payload[0] & DSTPRSNT_MASK) )
                            {
                                calculated_crc = CRC16((BYTE *)RxPacket[BankIndex].Payload, RxPacket[BankIndex].PayloadLen-2, 0);
                            }
                            else
                            {
                                calculated_crc = CRC16((BYTE *)RxPacket[BankIndex].Payload, 2, 0);
                                
                                // try full address first
                                calculated_crc = CRC16(MACInitParams.PAddress, MACInitParams.actionFlags.bits.PAddrLength, calculated_crc);
                                calculated_crc = CRC16((BYTE *)&(RxPacket[BankIndex].Payload[2]), RxPacket[BankIndex].PayloadLen-4, calculated_crc);
                            }
                            
                            if( received_crc != calculated_crc )
                            {
                                RxPacketPtr = 0;
                                RxPacket[BankIndex].PayloadLen = 0;
                                RegisterSet(FIFORSTREG | 0x0002);            // FIFO synchron latch re-enable 

                                goto IGNORE_HERE;
                            }
                            
                            #if !defined(TARGET_SMALL)
                                RxPacket[BankIndex].flags.bits.DQD = 1;
                                RxPacket[BankIndex].flags.bits.RSSI = TransceiverStatus.bits.RSSI_ATS;
                            #endif
                            
                            // send ack / check ack
                            #if defined(ENABLE_ACK1)
                                if( ( RxPacket[BankIndex].Payload[0] & PACKET_TYPE_MASK ) == PACKET_TYPE_ACK )  // acknowledgement
                                {
                                    if( RxPacket[BankIndex].Payload[1] == TxMACSeq )
                                    {
                                        hasAck = TRUE;
                                    }
        
                                    RxPacketPtr = 0;
                                    RxPacket[BankIndex].PayloadLen = 0;
                                    RegisterSet(FIFORSTREG | 0x0002);
                                }
                                else 
                            #endif
                            {
                                BYTE ackInfoIndex = 0xFF;
                                
                                if( RxPacket[BankIndex].Payload[0] & DSTPRSNT_MASK )
                                {
                                    for(i = 0; i < MACInitParams.actionFlags.bits.PAddrLength; i++)
                                    {
                                        if( RxPacket[BankIndex].Payload[2+i] != MACInitParams.PAddress[i] )
                                        {
                                            RxPacketPtr = 0;
                                            RxPacket[BankIndex].PayloadLen = 0;
                                            RegisterSet(FIFORSTREG | 0x0002);
                                            goto IGNORE_HERE;
                                        }
                                    }
                                }
    
                                #if defined(ENABLE_ACK)
                                    if( (RxPacket[BankIndex].Payload[0] & ACK_MASK) )  // acknowledgement required
                                    {
                                        RegisterSet(FIFORSTREG | 0x0002);
                                        
                                        for(i = 0; i < 4; i++)
                                        {
                                            ackPacket[i] = MACTxBuffer[i];
                                        }
                                        MACTxBuffer[0] = PACKET_TYPE_ACK | BROADCAST_MASK;   // frame control, ack type + broadcast
                                        MACTxBuffer[1] = RxPacket[BankIndex].Payload[1];     // sequenece number
                                        calculated_crc = CRC16((BYTE *)MACTxBuffer, 2, 0);
                                        MACTxBuffer[2] = calculated_crc>>8;
                                        MACTxBuffer[3] = calculated_crc;
                                        DelayMs(2);
                                        TxPacket(4, FALSE);
                                        RegisterSet(FIFORSTREG);
                                        for(i = 0; i < 4; i++)
                                        {
                                            MACTxBuffer[i] = ackPacket[i];
                                        }
                                    }
                                #endif
                                    
                                #if defined(ENABLE_ACK) && defined(ENABLE_RETRANSMISSION)
                                    for(i = 0; i < ACK_INFO_SIZE; i++)
                                    {
                                        if( AckInfo[i].Valid && (AckInfo[i].Seq == RxPacket[BankIndex].Payload[1]) &&
                                            AckInfo[i].CRC == received_crc )
                                        {
                                            AckInfo[i].startTick = MiWi_TickGet();
                                            break;    
                                        }
                                        if( (ackInfoIndex == 0xFF) && (AckInfo[i].Valid == FALSE) )
                                        {
                                            ackInfoIndex = i;
                                        }
                                    }
                                    if( i >= ACK_INFO_SIZE )
                                    {
                                        if( ackInfoIndex < ACK_INFO_SIZE )
                                        {                                
                                            AckInfo[ackInfoIndex].Valid = TRUE;
                                            AckInfo[ackInfoIndex].Seq = RxPacket[BankIndex].Payload[1];
                                            AckInfo[ackInfoIndex].CRC = received_crc;   
                                            AckInfo[ackInfoIndex].startTick = MiWi_TickGet();
                                        }

                                        RxPacket[BankIndex].PayloadLen -= 2;            // remove CRC
                                        RxPacket[BankIndex].flags.bits.Valid = TRUE;
                                    }
                                #else
                                
                                    RxPacket[BankIndex].PayloadLen -= 2;            // remove CRC
                                    RxPacket[BankIndex].flags.bits.Valid = TRUE;
                                    #if !defined(TARGET_SMALL)
                                        RxPacket[BankIndex].flags.bits.RSSI = TransceiverStatus.bits.RSSI_ATS;
                                        RxPacket[BankIndex].flags.bits.DQD = TransceiverStatus.bits.DQD; 
                                    #endif                      
                                #endif
                                RegisterSet(FIFORSTREG | 0x0002);
    
                            }
                            goto RETURN_HERE;
                        }
                        counter = 0;
                    }
                    else if( counter++ > 0xFFFE )
                    {
                        goto IGNORE_HERE;
                    }
                }
            }
            else            // read out the rest IT bits in case of no FINT
            {
                
                #if defined(HARDWARE_SPI)
                
                    TransceiverStatus.v[0] = SPIGet();
                    
                #else
                
                    TransceiverStatus.bits.RG_FF_IT = SPI_SDI;
                    SPI_SCK = 1;
                    SPI_SCK = 0;
                    
                    TransceiverStatus.bits.POR = SPI_SDI;
                    SPI_SCK = 1;
                    SPI_SCK = 0;
                    
                    TransceiverStatus.bits.RGUR_FFOV = SPI_SDI;
                    SPI_SCK = 1;
                    SPI_SCK = 0;
                    
                    TransceiverStatus.bits.WKUP = SPI_SDI;
                    SPI_SCK = 1;
                    SPI_SCK = 0;
                    
                    TransceiverStatus.bits.EXT = SPI_SDI;
                    SPI_SCK = 1;
                    SPI_SCK = 0;
                    
                    TransceiverStatus.bits.LBD = SPI_SDI;
                    SPI_SCK = 1;
                    SPI_SCK = 0;
                #endif
                
                PHY_CS = 1;
            }
              
RETURN_HERE:     
            RFIF = 0;
        }   
   
        #if defined(__18CXX)
            //check to see if the symbol timer overflowed
            if(TMR_IF)
            {
                if(TMR_IE)
                {
                    /* there was a timer overflow */
                    TMR_IF = 0;
                    timerExtension1++;
                    if(timerExtension1 == 0)
                    {
                        timerExtension2++;
                    }
                }
            }
            
            UserInterruptHandler();
        #endif
    }
    
    
    #if defined(__18CXX) & !defined(HI_TECH_C)
        #pragma code highVector=0x08
        void HighVector (void)
        {
            _asm goto HighISR _endasm
        }
        #pragma code /* return to default code section */
    #endif
    
    #if defined(__18CXX) & !defined(HI_TECH_C)
        #pragma code lowhVector=0x18
        void LowVector (void)
        {
            _asm goto HighISR _endasm
        }
        #pragma code /* return to default code section */
    #endif

#else
    /*******************************************************************
     * C18 compiler cannot compile an empty C file. define following 
     * bogus variable to bypass the limitation of the C18 compiler if
     * a different transceiver is chosen.
     ******************************************************************/
    extern char bogusVariable;
#endif


