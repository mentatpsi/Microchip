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
*  This is the sample application that demonstrate the rich features
*  of MiWi(TM) Development Evniroment (DE). This demo should be used 
*  with FeatureDemoNode2 together. In this demo, following MiWi(TM) 
*  DE features has been implemented:
*   - Network Freezer
*       This application demonstrate how to invoke Network Freezer
*       feature that restore the previous network configurations
*       after reset or power cycle.
*   - Active Scan 
*       This application will do an active scan to allocate all PANs
*       running in the neighborhood and choose the PAN that share the 
*       same PAN identifier to establish connection.
*   - Energy Scan
*       If no existing PAN that matches the desired PAN identifier, this
*       application will find a channel with least noise among all the
*       supported channels.
*   - Indirect Message Feature 
*       This application is able to store the message to the device
*       with radio off during idle and deliever the message when that 
*       device wakes up and asking for it. This application is also
*       capable of delivering broadcast message to each individual 
*       device with radio off during idle
*   - Frequency Agility 
*       As a frequency agility starter, this application is able to 
*       decide the optimal channel and change operating channel. It
*       also has to broadcast and let other devices to change channel.
*   
*
* Change History:
*  Rev   Date         Author    Description
*  0.1   03/01/2008   yfy       Initial revision
*  3.1   05/28/2010   yfy       MiWi DE 3.1
********************************************************************/

/************************ HEADERS **********************************/
#include "WirelessProtocols/Console.h"
#include "WirelessProtocols/P2P/P2P.h"
#include "WirelessProtocols/SymbolTime.h"
#include "Transceivers/Transceivers.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/NVM.h"
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
    BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {LIGHT};
#endif

/*******************************************************************/
// The variable myChannel defines the channel that the P2P connection
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).
/*******************************************************************/
BYTE myChannel = 25;

/*******************************************************************/
// the following two variable arrays are the data to be transmitted
// in this demo. They are bit map of English word "MiWi" and "DE"
// respectively.
/*******************************************************************/
ROM const BYTE MiWi[5][21] = 
{
    {0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x20,0x20,0xB2,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x0D,0x0A}
};   

ROM const BYTE DE[5][11] = 
{
    {0xB2,0xB2,0xB2,0x20,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x0D,0x0A},
    {0xB2,0xB2,0xB2,0x20,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A}
};  



/*********************************************************************
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
*                   The device will first search for an existing
*                   network.  If a network exists and the parameters 
*                   of the network are acceptable to the device (in 
*                   this example simple RSSI minimum) then the device
*                   will join the existing network.  If the device 
*                   does not find an acceptable network and is a 
*                   coordinator then the device will perform an 
*                   energy scan on all of the channels available and 
*                   determine which channel has the lowest noise.  
*                   It will form a new network on this channel as 
*                   the PAN coordinator.
*
* Note:			    
********************************************************************/
#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{   
    BYTE i, j;
    BYTE TxSynCount = 0;
    BYTE TxNum = 0;
    BYTE RxNum = 0;
    BOOL bReceivedMessage = FALSE;
    
    /*******************************************************************/
    // Initialize the system
    /*******************************************************************/
    BoardInit();         
    ConsoleInit();   
        
    
    #if defined(PROTOCOL_P2P)
        Printf("\r\nStarting Node 1 of Feature Demo for MiWi(TM) P2P Stack ...");
    #endif
    #if defined(PROTOCOL_MIWI)
        Printf("\r\nStarting Node 1 of Feature Demo for MiWi(TM) Stack ...");
    #endif
    #if defined(PROTOCOL_MIWI_PRO)
        Printf("\r\nStarting Node 1 of Feature Demo for MiWi(TM) PRO Stack ...");
    #endif
    #if defined(PICDEMZ) 
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RB5");
        Printf("\r\n           Button 2: RB4");
        Printf("\r\nOutput Configuration:");
        Printf("\r\n                     RS232 port");
        Printf("\r\n              LED 1: RA0");
        Printf("\r\n              LED 2: RA1");
    #endif
    #if defined(PIC18_EXPLORER) 
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RB0");
        Printf("\r\n           Button 2: RA5");
        Printf("\r\nOutput Configuration:");
        Printf("\r\n                     RS232 port");
        Printf("\r\n                     USB port");
        Printf("\r\n              LED 1: D8");
        Printf("\r\n              LED 2: D7");
    #endif
    #if defined(EIGHT_BIT_WIRELESS_BOARD) 
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RB0");
        Printf("\r\n           Button 2: RB2");
        Printf("\r\nOutput Configuration:");
        Printf("\r\n              LED 1: RA2");
        Printf("\r\n              LED 2: RA3");
    #endif
    #if defined(EXPLORER16) 
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RD6");
        Printf("\r\n           Button 2: RD7");
        Printf("\r\nOutput Configuration:");
        Printf("\r\n                     RS232 port");
        Printf("\r\n              LED 1: D10");
        Printf("\r\n              LED 2: D9");
    #endif
    #if defined(MRF24J40)
    Printf("\r\n     RF Transceiver: MRF24J40");
    #elif defined(MRF49XA)
    Printf("\r\n     RF Transceiver: MRF49XA");
    #elif defined(MRF89XA)
    Printf("\r\n     RF Transceiver: MRF89XA");
    #endif
    Printf("\r\n   Demo Instruction:");
    Printf("\r\n                     Power on the board until LED 1 lights up");
    Printf("\r\n                     to indicate it is ready to establish new");
    Printf("\r\n                     connections. Push Button 1 to perform");
    Printf("\r\n                     frequency agility procedure. Push Button");
    Printf("\r\n                     2 to unicast encrypted message. LED 2 will");
    Printf("\r\n                     be toggled upon receiving messages. ");
    Printf("\r\n\r\n");
   
    /*******************************************************************/
    // Following block display demo information on LCD of demo boards.
    /*******************************************************************/
    #if defined(MRF49XA)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Feature P2P Demo MRF49XA Node 1", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Feature MiWi Dem MRF49XA Node 1", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI_PRO)
            LCDDisplay((char *)"Feature MiWi PRO MRF49XA Node 1", 0, TRUE); 
        #endif
    #elif defined(MRF24J40)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Feature P2P DemoMRF24J40 Node 1", 0, TRUE);
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Feature MiWi DemMRF24J40 Node 1", 0, TRUE);
        #endif
        #if defined(PROTOCOL_MIWI_PRO)
            LCDDisplay((char *)"Feature MiWi PROMRF24J40 Node 1", 0, TRUE);
        #endif
    #elif defined(MRF89XA)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Feature P2P Demo MRF89XA Node 1", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Feature MiWi Dem MRF89XA Node 1", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI_PRO)
            LCDDisplay((char *)"Feature MiWi PRO MRF89XA Node 1", 0, TRUE); 
        #endif
    #endif
    
    if( (PUSH_BUTTON_1 == 0) && ( MiApp_ProtocolInit(TRUE) == TRUE ) )
    {   
        /*******************************************************************/
        // Function MiApp_ProtocolInit initialize the protocol stack. The
        // only input parameter indicates if previous network configuration
        // should be restored. In this example, if button 1 is pressed and
        // hold when powering up, we assume the user would like to enable
        // the Network Freezer and load previous network configuration
        // from NVM.
        /*******************************************************************/

        LCDDisplay((char *)" Network Freezer    ENABLED", 0, TRUE); 
        Printf("\r\nNetwork Freezer Feature is enabled. There will be no hand-shake process.\r\n");
        LED_1 = 1;
        while(PUSH_BUTTON_1 == 0);
    }
    else
    {
        /*******************************************************************/
        // Function MiApp_ProtocolInit initialize the protocol stack. The
        // only input parameter indicates if previous network configuration
        // should be restored. In this example, if button 1 is released 
        // when powering up, we assume that the user want the network to 
        // start from scratch.
        /*******************************************************************/
        MiApp_ProtocolInit(FALSE);   

        LED_1 = 0;
        LED_2 = 0;

        #ifdef ENABLE_ACTIVE_SCAN
        
            myChannel = 0xFF;
            ConsolePutROMString((ROM char *)"\r\nStarting Active Scan...");
            
            LCDDisplay((char *)"Active Scanning", 0, FALSE);
    
            /*******************************************************************/
            // Function MiApp_SearchConnection will return the number of 
            // existing connections in all channels. It will help to decide 
            // which channel to operate on and which connection to add.
            // The return value is the number of connections. The connection 
            //     data are stored in global variable ActiveScanResults. 
            //     Maximum active scan result is defined as 
            //     ACTIVE_SCAN_RESULT_SIZE
            // The first parameter is the scan duration, which has the same 
            //     definition in Energy Scan. 10 is roughly 1 second. 9 is a 
            //     half second and 11 is 2 seconds. Maximum scan duration is 14, 
            //     or roughly 16 seconds.
            // The second parameter is the channel map. Bit 0 of the 
            //     double word parameter represents channel 0. For the 2.4GHz 
            //     frequency band, all possible channels are channel 11 to 
            //     channel 26. As the result, the bit map is 0x07FFF800. Stack 
            //     will filter out all invalid channels, so the application 
            //     only needs to pay attention to the channels that are not 
            //     preferred.
            /*******************************************************************/
            i = MiApp_SearchConnection(10, 0xFFFFFFFF);
            
            if( i > 0 )
            {
                // now print out the scan result.
                Printf("\r\nActive Scan Results: \r\n");
                for(j = 0; j < i; j++)
                {
                    Printf("Channel: ");
                    PrintDec(ActiveScanResults[j].Channel );
                    Printf("   RSSI: ");
                    PrintChar(ActiveScanResults[j].RSSIValue);
                    Printf("\r\n");
                    myChannel = ActiveScanResults[j].Channel;
                }
            }
        #endif


        /*******************************************************************/
        // Function MiApp_ConnectionMode sets the connection mode for the 
        // protocol stack. Possible connection modes are:
        //  - ENABLE_ALL_CONN       accept all connection request
        //  - ENABLE_PREV_CONN      accept only known device to connect
        //  - ENABL_ACTIVE_SCAN_RSP do not accept connection request, but 
        //                          allow response to active scan
        //  - DISABLE_ALL_CONN      disable all connection request, including
        //                          active scan request
        /*******************************************************************/
        MiApp_ConnectionMode(ENABLE_ALL_CONN);
    
        if( i > 0 )
        {
            /*******************************************************************/
            // Function MiApp_EstablishConnection try to establish a new 
            // connection with peer device. 
            // The first parameter is the index to the active scan result, which 
            //      is acquired by discovery process (active scan). If the value
            //      of the index is 0xFF, try to establish a connection with any 
            //      peer.
            // The second parameter is the mode to establish connection, either 
            //      direct or indirect. Direct mode means connection within the 
            //      radio range; Indirect mode means connection may or may not 
            //      in the radio range. 
            /*******************************************************************/
            if( MiApp_EstablishConnection(0, CONN_MODE_DIRECT) == 0xFF )
            {
                Printf("\r\nJoin Fail");
            }
        }    
        else
        {
            /*******************************************************************/
            // Function MiApp_StartConnection tries to start a new network
            //
            // The first parameter is the mode of start connection. There are 
            // two valid connection modes:
            //   - START_CONN_DIRECT        start the connection on current 
            //                              channel
            //   - START_CONN_ENERGY_SCN    perform an energy scan first, 
            //                              before starting the connection on 
            //                              the channel with least noise
            //   - START_CONN_CS_SCN        perform a carrier sense scan 
            //                              first, before starting the 
            //                              connection on the channel with 
            //                              least carrier sense noise. Not
            //                              supported for current radios
            //
            // The second parameter is the scan duration, which has the same 
            //     definition in Energy Scan. 10 is roughly 1 second. 9 is a 
            //     half second and 11 is 2 seconds. Maximum scan duration is 
            //     14, or roughly 16 seconds.
            //
            // The third parameter is the channel map. Bit 0 of the 
            //     double word parameter represents channel 0. For the 2.4GHz 
            //     frequency band, all possible channels are channel 11 to 
            //     channel 26. As the result, the bit map is 0x07FFF800. Stack 
            //     will filter out all invalid channels, so the application 
            //     only needs to pay attention to the channels that are not 
            //     preferred.
            /*******************************************************************/
            LCDDisplay((char *)"Active Scanning Energy Scanning", 0, FALSE);
            MiApp_StartConnection(START_CONN_ENERGY_SCN, 10, 0xFFFFFFFF);
        }
        
        // Turn on LED 1 to indicate ready to accept new connections
        LED_1 = 1;
    }
    
    DumpConnection(0xFF);                    
    LCDDisplay((char *)"Start Connection on Channel %d", currentChannel, TRUE);

    /*******************************************************************/
    // Following block display demo instructions on LCD based on the
    // demo board used. 
    /*******************************************************************/
    #if defined(EXPLORER16)
        LCDDisplay((char *)"RD6: Freq Hop   RD7: Unicast", 0, FALSE); 
    #elif defined(PIC18_EXPLORER)
        LCDDisplay((char *)"RB0: Freq Hop   RA5: Unicast", 0, FALSE); 
    #elif defined(EIGHT_BIT_WIRELESS_BOARD)
        LCDDisplay((char *)"RB0: Freq Hop   RB2: Unicast", 0, FALSE);
    #endif


    while(1)
    {
        /*******************************************************************/
        // Function MiApp_MessageAvailable will return a boolean to indicate 
        // if a message for application layer has been received by the 
        // transceiver. If a message has been received, all information will 
        // be stored in the rxMessage, structure of RECEIVED_MESSAGE.
        /*******************************************************************/
        if( MiApp_MessageAvailable() )
        {
            /*******************************************************************/
            // If a packet has been received, following code prints out some of
            // the information available in rxFrame.
            /*******************************************************************/
            if( rxMessage.flags.bits.secEn )
            {
                ConsolePutROMString((ROM char *)"Secured ");
            }

            if( rxMessage.flags.bits.broadcast )
            {
                ConsolePutROMString((ROM char *)"Broadcast Packet with RSSI ");
            }
            else
            {
                ConsolePutROMString((ROM char *)"Unicast Packet with RSSI ");
            }
            PrintChar(rxMessage.PacketRSSI);
            if( rxMessage.flags.bits.srcPrsnt )
            {
                ConsolePutROMString((ROM char *)" from ");
                if( rxMessage.flags.bits.altSrcAddr )
                {
                    PrintChar(rxMessage.SourceAddress[1]);
                    PrintChar(rxMessage.SourceAddress[0]);
                }
                else
                {    
                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH-1-i]);
                    }
                }    
            }

            ConsolePutROMString((ROM char *)": ");
            
            for(i = 0; i < rxMessage.PayloadSize; i++)
            {
                ConsolePut(rxMessage.Payload[i]);
            }
            
            // Toggle LED2 to indicate receiving a packet.
            LED_2 ^= 1;
            
            /*******************************************************************/
            // Function MiApp_DiscardMessage is used to release the current 
            // received message. After calling this function, the stack can 
            // start to process the next received message.
            /*******************************************************************/          
            MiApp_DiscardMessage();
            
            bReceivedMessage = TRUE;
            
            /*******************************************************************/
            // Following block update the total received and transmitted messages
            // on the LCD of the demo board. 
            /*******************************************************************/
            LCDTRXCount(TxNum, ++RxNum);
        }
        else
        {
            /*******************************************************************/
            // If no packet received, now we can check if we want to send out
            // any information.
            // Function ButtonPressed will return if any of the two buttons
            // has been pushed.
            /*******************************************************************/
            BYTE PressedButton = ButtonPressed();
            switch( PressedButton )
            {
                case 1: 
                    {
                        DWORD ChannelMap = ~((DWORD)0x00000001 << currentChannel);
                        
                        /*******************************************************************/
                        // Following block display the frequency hopping precedure starts 
                        /*******************************************************************/
                        LCDDisplay((char *)" Init Freq Hop", 0, FALSE);
                        
                        /*******************************************************************/
                        // Function MiApp_InitChannelHopping will start the process of 
                        // channel hopping. This function can be only called by Frquency 
                        // Agility starter and the device must have the energy detection 
                        // feature turned on. This function will do an energy detection 
                        // scan of all input channels and start the process of jumping to 
                        // the channel with least noise
                        //
                        // The only parameter of this function is the bit map of the 
                        // allowed channels. Bit 0 of the double word parameter represents 
                        // channel 0. For the 2.4GHz frequency band, the possible channels 
                        // are channel 11 to channel 26. As the result, the bit map is 
                        // 0x07FFF800. 
                        //
                        // Microchip proprietary stack does not limit the application when to
                        // do channel hopping. The typical triggers for channel hopping are:
                        //  1.  Continuous data transmission failures
                        //  2.  Periodical try the channel hopping process every few hours
                        //      or once per day
                        //  3.  Receive a request to start the channel hopping process. This
                        //      demo is an example of manually issue the request. In the 
                        //      real application, a Frequency Agility follower can send a 
                        //      message to request channel hopping and the Frequency Agility
                        //      starter will decide if start the process.
                        /*******************************************************************/
                        if( MiApp_InitChannelHopping(ChannelMap & 0xFFFFFFFF) == TRUE )
                        {
                            /*******************************************************************/
                            // Following block display the frequency hopping precedure result on
                            // the LCD of demo board, if applicable. The new channel number that 
                            // the PAN has hopped to will be displayed.
                            /*******************************************************************/
                            LCDDisplay((char *)" Init Freq Hop   New Channel %d", currentChannel, FALSE);           
                            Printf("\r\nFrequency Agility to Channel ");
                            PrintDec(currentChannel);
                        }
                        else
                        {
                            LCDDisplay((char *)" Init Freq Hop   Freq Hop Failed", 0, FALSE);
                            Printf("\r\nFrequency Agility Failed");
                        }    
                    }
                    break;
                 
                case 2:
                    
                    /*******************************************************************/                
                    // Button 2 (RB4 on PICDEM Z or RD7 on Explorer 16) pressed. We need
                    //  to send out the bitmap of word "P2P" encrypted.
                    // First call function MiApp_FlushTx to reset the Transmit buffer. 
                    //  Then fill the buffer one byte by one byte by calling function 
                    //  MiApp_WriteData
                    /*******************************************************************/
                    MiApp_FlushTx();
                    if( (TxSynCount % 6 ) == 0 )
                    {
                        MiApp_WriteData(0x0D);
                        MiApp_WriteData(0x0A);
                    }
                    else
                    {                     
                        for(i = 0; i < 11; i++)
                        {
                            MiApp_WriteData(DE[(TxSynCount%6)-1][i]);
                        }
                    }
                    TxSynCount++;

                    /*******************************************************************/
                    // Function MiApp_UnicastConnection is one of the functions to 
                    // unicast a message.
                    //    The first parameter is the index of connection table for 
                    //       the peer device. In this demo, since there are only two
                    //       devices involved, the peer device must be stored in the 
                    //       first P2P Connection Entry of the connection table.
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
                        Printf("\r\nUnicast Failed\r\n");
                        /*******************************************************************/
                        // Following block display error message on LCD of demo board.
                        /*******************************************************************/
                        LCDDisplay((char *)" Unicast Failed", 0, TRUE);
                    }
                    else
                    {
                        TxNum++;
                    }
                    
                    /*******************************************************************/
                    // Following block update the total received and transmitted messages
                    // on the LCD of the demo board. 
                    /*******************************************************************/
                    LCDTRXCount(TxNum, RxNum);  
                    break;
                    
                default:
                    break;
            }   
        }
    }
}
