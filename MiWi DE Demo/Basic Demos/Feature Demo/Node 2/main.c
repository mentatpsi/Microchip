/********************************************************************
* FileName:		main.c
* Dependencies: none   
* Processor:	PIC18, PIC24, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher	
*               Microchip C32 v1.02 or higher
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice for MiWi Software:
*
* Copyright © 2007-2012 Microchip Technology Inc.  All rights reserved.
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
*  This is the sample application that demonstrate the rich features
*  of MiWi(TM) Development Environment (DE). This demo should be used 
*  with FeatureDemoNode1 together. In this demo, following MiWi(TM) 
*  DE features have been implemented:
*   - Network Freezer
*       This application demonstrate how to invoke Network Freezer
*       feature that restore the previous network configurations
*       after reset or power cycle.
*   - Active Scan 
*       This application will do an active scan to allocate all PANs
*       running in the neighborhood and choose the PAN that share the 
*       same PAN identifier to establish connection.
*   - Sleep Feature 
*       This application is able to put the radio and MCU into sleep 
*       mode to conserve power. After waking up from the sleep, this
*       application is also capable of sending out Data Request command
*       to retrieve possible data from its associated node. The sleeping
*       period for this application is roughly 8 seconds, depends on 
*       the MCU family and operating frequency setting.
*   - Frequency Agility 
*       As a frequency agility follower, this application is able to 
*       change operating channel when receiving a Channel Hopping 
*       command. This application is also capable of resynchronize
*       the connection by scanning possible channels.
*
* Detailed demo flow chart and execution instructions can be found in
*   MiWi DE help file located at directory <MLA Install Directory>/
*   Microchip/Help. From the content tab, find the document about
*   Feature demo at <ROOT> -> "Demos" -> "Running Demos" -> 
*   "Basic Demos" -> "Feature Demo".
*
* Change History:
*  Rev   Date         Author    Description
*  0.1   03/01/2008   yfy       Initial revision
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.2   1/31/2012    yfy       Simplified demo interface
********************************************************************/

/************************ HEADERS **********************************/
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/Console.h"
#include "DemoOutput.h"

/************************ VARIABLES ********************************/
#define LIGHT   0x01
#define SWITCH  0x02

/*******************************************************************/
// AdditionalNodeID variable array defines the additional 
// information to identify a device on a P2P connection. This array
// will be transmitted with the P2P_CONNECTION_REQUEST command to 
// initiate the connection between the two devices. Along with the 
// long address of this device, this variable array will be stored 
// in the P2P Connection Entry structure of the partner device. The 
// size of this array is ADDITIONAL_CONNECTION_PAYLOAD, defined in 
// P2PDefs.h.
// In this demo, this variable array is set to be empty.
/******************************************************************/
#if ADDITIONAL_NODE_ID_SIZE > 0
    BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {SWITCH};
#endif

/*******************************************************************/
// The variable myChannel defines the channel that the P2P connection
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available 
// with least amount of energy (or noise).
/*******************************************************************/
BYTE myChannel = 25;

/***************************************************************************
* Function:         void main(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    This is the main function that runs the demo.  
*                   The device will first search for a P2P connection
*                   Once the connection is established, pressing 
*                   button one (RB5 on PICDEM Z or RD6 on Explorer 16) 
*                   or button two (RB4 on PICDEM Z or RD7 on Explorer16) 
*                   will send out broadcast and unicast packets.
*
* Note:			    
****************************************************************************/
#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{   
    BYTE i, j;
    BYTE OperatingChannel = 0xFF;
    BYTE TxSynCount = 0;
    BYTE TxSynCount2 = 0;
    BYTE TxPersistFailures = 0;
    BOOL ReadyToSleep = FALSE;
    BYTE TxNum = 0;
    BYTE RxNum = 0;
    BYTE PressedButton = 0;
    
    /*******************************************************************/
    // Initialize the system
    /*******************************************************************/
    BoardInit();         
    ConsoleInit();  
    DemoOutput_Greeting();

    /*********************************************************************/
    // Function MiApp_ProtocolInit intialize the protocol stack.
    // The return value is a boolean to indicate the status of the 
    //      operation.
    // The only parameter indicates if Network Freezer should be invoked.
    //      When Network Freezer feature is invoked, all previous network
    //      configurations will be restored to the states before the 
    //      reset or power cycle
    //
    // In this example, we assume that the user wants to apply Network
    //      Freezer feature and restore the network configuration if
    //      button 1 is pressed and hold when powering up.
    /*********************************************************************/
    if( (PUSH_BUTTON_1 == 0) && ( MiApp_ProtocolInit(TRUE) == TRUE ) )
    {  
        DemoOutput_NetworkFreezer();
        while(PUSH_BUTTON_1 == 0 );
    }
    else
    {
        /*********************************************************************/
        // Function MiApp_ProtocolInit intialize the protocol stack.
        // The return value is a boolean to indicate the status of the 
        //      operation.
        // The only parameter indicates if Network Freezer should be invoked.
        //      When Network Freezer feature is invoked, all previous network
        //      configurations will be restored to the states before the 
        //      reset or power cycle
        //
        // In this example, we assume that the user wants to start from
        // scratch and ignore previous network configuration if button 1
        // is released when powering up.
        /*********************************************************************/
        MiApp_ProtocolInit(FALSE);   
        DemoOutput_StartActiveScan();
            
        while(1)
        {
            /*********************************************************************/
            // Function MiApp_SearchConnection will return the number of existing 
            // connections in all channels. It will help to decide which channel 
            // to operate on and which connection to add
            // The return value is the number of connections. The connection data
            //     are stored in global variable ActiveScanResults. Maximum active
            //     scan result is defined as ACTIVE_SCAN_RESULT_SIZE
            // The first parameter is the scan duration, which has the same 
            //     definition in Energy Scan. 10 is roughly 1 second. 9 is a half 
            //     second and 11 is 2 seconds. Maximum scan duration is 14, or 
            //     roughly 16 seconds.
            // The second parameter is the channel map. Bit 0 of the double
            //     word parameter represents channel 0. For the 2.4GHz frequency
            //     band, all possible channels are channel 11 to channel 26.
            //     As the result, the bit map is 0x07FFF800. Stack will filter
            //     out all invalid channels, so the application only needs to pay
            //     attention to the channels that are not preferred.
            /*********************************************************************/
            j = MiApp_SearchConnection(10, 0xFFFFFFFF);
            OperatingChannel = DemoOutput_ActiveScanResults(j);
        
            if( OperatingChannel != 0xFF )
            {
                /*******************************************************************/
                // Function MiApp_SetChannel assign the operation channel(frequency)
                // for the transceiver. Channels 0 - 31 has been defined for the 
                // wireless protocols, but not all channels are valid for all 
                // transceivers, depending on their hardware design, operating
                // frequency band, data rate and other RF parameters. Check the
                // definition of FULL_CHANNEL_MAP for details.
                /*******************************************************************/
                MiApp_SetChannel(OperatingChannel);
                break;
            }
            DemoOutput_Rescan();
        }

        
        /*********************************************************************/
        // Function MiApp_ConnectionMode sets the connection mode for the 
        // protocol stack. Possible connection modes are:
        //  - ENABLE_ALL_CONN       accept all connection request
        //  - ENABLE_PREV_CONN      accept only known device to connect
        //  - ENABL_ACTIVE_SCAN_RSP do not accept connection request, but allow
        //                          response to active scan
        //  - DISABLE_ALL_CONN      disable all connection request, including
        //                          active scan request
        /*********************************************************************/
        MiApp_ConnectionMode(ENABLE_ALL_CONN);
        
        /*******************************************************************/
        // Function MiApp_EstablishConnection establish connections between
        // two devices. It has two input parameters:
        // The first parameter is the index of the target device in the 
        //     active scan table. It requires a MiApp_SearchConnection call
        //     before hand. If seraching connection is not performed in 
        //     advance, user can apply 0xFF to the first parameter to 
        //     indicate that it is OK to establish connection with any 
        //     device.
        // The second parameter is the connection mode, either directly or
        //     indirectly. Direct connection is a connection in the radio
        //     range. All protocol stack support this connection mode. 
        //     Indirect connection is the connection out of radio range. 
        //     An indirect connection has to rely on other device to route 
        //     the messages between two connected devices. Indirect 
        //     connection is also called "Socket" connection in MiWi 
        //     Protocol. Since MiWi P2P protocol only handles connection 
        //     of one hop, indirect connection is not supported in MiWi 
        //     P2P protocol, but supported in other networking protocols.
        // Function MiApp_EstablishConnection returns the index of the 
        //     connected device in the connection table. If no connection 
        //     is established after predefined retry times 
        //     CONNECTION_RETRY_TIMES, it will return 0xFF. If multiple 
        //     connections have been established, it will return the one 
        //     of the indexes of the connected device.
        /*******************************************************************/
        i = MiApp_EstablishConnection(0, CONN_MODE_DIRECT);
        DemoOutput_Connected();
    }
    
    // Turn on LED 1 to indicate connection established
    LED_1 = 1;
    
    /*******************************************************************/
    // Function DumpConnection is used to print out the content of the
    // P2P Connection Entry on the hyperterminal. It may be useful in 
    // the debugging phase.
    // The only parameter of this function is the index of the 
    // Connection Entry. The value of 0xFF means to print out all
    // valid Connection Entries; otherwise, the Connection Entry
    // of the input index will be printed out.
    /*******************************************************************/
    DumpConnection(0xFF);
    
    DemoOutput_Instruction();

    while(1)
    {   
        /*******************************************************************/
        // Function MiApp_MessageAvailable will return a boolean to indicate 
        // if a message for application layer has been received by the 
        // transceiver. If a message has been received, all information will 
        // be stored in rxMessage, structure of RECEIVED_MESSAGE.
        /*******************************************************************/
        if( MiApp_MessageAvailable() )
        {
            DemoOutput_HandleMessage();
            
            /*******************************************************************/
            // Function MiApp_DiscardMessage is used to release the current 
            // received message. After calling this function, the stack can 
            // start to process the next received message.
            /*******************************************************************/ 
            MiApp_DiscardMessage();
            
            // Toggle LED2 to indicate receiving a packet.
            LED_2 ^= 1;
            
            DemoOutput_UpdateTxRx(TxNum, ++RxNum);
        }
        else 
        {
            /***********************************************************************/
            // TxPersistFailures is the local variable to track the transmission 
            // failure because no acknowledgement frame is received. Typically,
            // this is the indication of either very strong noise, or the PAN
            // has hopped to another channel. 
            /***********************************************************************/
            if( TxPersistFailures > 3 )
            {
                DemoOutput_StartResync();

                /*******************************************************************/
                // Function MiApp_TransceiverPowerState is used to set the power state
                // of RF transceiver. There are three possible states:
                //   - POWER_STATE_SLEEP        Put transceiver into sleep
                //   - POWER_STATE_WAKEUP       Wake up the transceiver only
                //   - POWER_STATE_WAKEUP_DR    Wake up and send Data Request command
                /*******************************************************************/
                MiApp_TransceiverPowerState(POWER_STATE_WAKEUP);
                
                /***********************************************************************/
                // Function MiApp_ResyncConnection is used to synchronized connection
                // if one side of communication jumped to another channel, when 
                // frequency agility is performed. Usually, this is done by the 
                // sleeping device, since the sleeping device cannot hear the broadcast 
                // of channel hopping command.
                //
                // The first parameter is the index of connection table for the peer 
                // node, which we would like to resynchronize to.
                // The second parameter is the bit map of channels to be scanned
                /***********************************************************************/
                MiApp_ResyncConnection(0, 0xFFFFFFFF);    
                TxPersistFailures = 0;
                ReadyToSleep = FALSE;
                
                DemoOutput_EndResync();
            }
            else
            {
                ReadyToSleep = TRUE;
            }    
            
            /*******************************************************************/
            // If Data Request command and data transmision has been handled,
            // as the RFD device, it is time to consider put both the radio and 
            // MCU into sleep mode to conserve power.
            /*******************************************************************/
            if( ReadyToSleep )
            {   
                ReadyToSleep = FALSE;
                
                /*******************************************************************/
                // Function MiApp_TransceiverPowerState is used to set the power 
                // state of RF transceiver. There are three possible states:
                //   - POWER_STATE_SLEEP        Put transceiver into sleep
                //   - POWER_STATE_WAKEUP       Wake up the transceiver only
                //   - POWER_STATE_WAKEUP_DR    Wake up and send Data Request 
                //                              command
                /*******************************************************************/
                MiApp_TransceiverPowerState(POWER_STATE_SLEEP);
                                    
                /*******************************************************************/
                // Prepare the condition to wake up the MCU. The MCU can either be
                // waken up by the timeout of watch dog timer (Time Synchronization 
                // off), timeout of counter with external 32KHz crystal (Time 
                // Synchronization on), or by the pin change notification interrupt 
                // by pushing the button. MCU handling of sleep preparing for 
                // different demo boards can be found in HardwareProfile.c
                /*******************************************************************/
                PrepareWakeup();
                
                // Put MCU into sleep mode
                Sleep();
                
                /*******************************************************************/
                // Handling the wakeup procedure. The steps differ for different
                // demo boards and families of MCUs. Details can be found in 
                // HardwareProfile.c
                /*******************************************************************/
                AfterWakeup();
                
                PressedButton = ButtonPressed();   
                
                /*******************************************************************/
                // Function MiApp_TransceiverPowerState is used to set the power 
                // state of RF transceiver. There are three possible states:
                //   - POWER_STATE_SLEEP        Put transceiver into sleep
                //   - POWER_STATE_WAKEUP       Wake up the transceiver only
                //   - POWER_STATE_WAKEUP_DR    Wake up and send Data Request 
                //                              command
                /*******************************************************************/
                if( MiApp_TransceiverPowerState(POWER_STATE_WAKEUP_DR) > SUCCESS )
                {
                    TxPersistFailures++;
                    DemoOutput_ConnectionLost(TxPersistFailures);
                }
                else
                {
                    if( TxPersistFailures > 0 )
                    {
                        DemoOutput_UpdateTxRx(TxNum, RxNum);
                    }
                    TxPersistFailures = 0;
                }
                
                switch( PressedButton )
                {
                    case 1: 
                        /*******************************************************************/                
                        // Button 1 pressed.
                        // First use MiApp_FlushTx to reset the Transmit buffer. Then fill 
                        // the TX buffer by calling function MiApp_WriteData
                        /*******************************************************************/
                        MiApp_FlushTx();
                        for(i = 0; i < 21; i++)
                        {
                            MiApp_WriteData(MiWi[(TxSynCount%6)][i]);
                        }
                        TxSynCount++;
                        
                        /*******************************************************************/
                        // Function MiApp_BroadcastPacket is used to broadcast a message
                        //    The only parameter is the boolean to indicate if we need to
                        //    secure the message
                        /*******************************************************************/
                        MiApp_BroadcastPacket(FALSE);
                        DemoOutput_UpdateTxRx(++TxNum, RxNum);   
                        break;
      
                    case 2:
                        /*******************************************************************/                
                        // Button 2 pressed.
                        // First use MiApp_FlushTx to reset the Transmit buffer. Then fill 
                        // the TX buffer by calling function MiApp_WriteData
                        /*******************************************************************/
                        MiApp_FlushTx();                   
                        for(i = 0; i < 11; i++)
                        {
                            MiApp_WriteData(DE[(TxSynCount2%6)][i]);
                        }
                        TxSynCount2++;
                        
                        /*******************************************************************/
                        // Function MiApp_UnicastConnection is one of the functions to 
                        // unicast a message.
                        //    The first parameter is the index of connection table for 
                        //       the peer device. In this demo, since there are only two
                        //       devices involved, the peer device must be stored in the 
                        //       first Connection Entry in the connection table.
                        //    The second parameter is the boolean to indicate if we need 
                        //       to secure the frame. If encryption is applied, the 
                        //       security level and security key are defined in the 
                        //       configuration file for the transceiver
                        //
                        // Another way to unicast a message is by calling function
                        // MiApp_UnicastAddress. Instead of supplying the index of the 
                        // connection table of the peer device, this function requires the 
                        // input parameter of destination address directly.
                        /*******************************************************************/
                        if( MiApp_UnicastConnection(0, TRUE) == FALSE )
                        {
                            DemoOutput_UnicastFail();
                        }
                        else
                        {
                            TxNum++;
                        }

                        DemoOutput_UpdateTxRx(TxNum, RxNum);
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
}



