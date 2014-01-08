/****************************************************************************
* FileName:         Demo.c
* Dependencies:     none   
* Processor:        PIC18F46J50	
* Complier:         Microchip C18 v3.04 or higher
* Company:          Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice for MiWi DE Software:
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
****************************************************************************
* File Description:

*   This simple demo for the 8-bit Wireless Development Kit 
*   demonstrates uses MiWi P2P to create a P2P network. 
*   For the ping-pong, one node acts as a transmitter, the
*   other as the receiver and each time the node(transmitter) 
*   will transmit 10 packets before switching the roles 
*   (Transmitter -> Receiver and *   Receiver -> transmitter) 
*   Ping Pong Demo can be used multiple ways
*   - Range Test
*       Users can carry one board with them and check "Over the
*       Air Transmission Range"
*   - RSSI
*       The RSSI of the packets is printed on LCD screen. 
*   - PER (Packet Error Rate)
*      Packet count is printed on the screen, this can be used to 
*      estimate PER.
*     
* Change History:
*  Rev   Date         Author    Description
*  0.1	09/29/2010    mynenis	Initial Version
*  1.0  03/23/2010    mynenis   Updated to work with latest version 
*                               of MiWi Stack (4.1 Beta)
**************************************************************************/

/************************ HEADERS ****************************************/
#include "WirelessProtocols/Console.h"
#include "ConfigApp.h"
#include "HardwareProfile.h"
#include "LcdDisplay.h"
#include "WirelessProtocols/MSPI.h"
#include "WirelessProtocols/MCHP_API.h"
#include "TimeDelay.h"
#include "WirelessProtocols/SymbolTime.h"
#include "Transceivers/Transceivers.h"

/*************************************************************************/

// AdditionalNodeID variable array defines the additional
// information to identify a device on a PAN. This array
// will be transmitted when initiate the connection between
// the two devices. This  variable array will be stored in
// the Connection Entry structure of the partner device. The
// size of this array is ADDITIONAL_NODE_ID_SIZE, defined in
// ConfigApp.h.
// In this demo, this variable array is set to be empty.

/*************************************************************************/
#if ADDITIONAL_NODE_ID_SIZE > 0
BYTE        AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x00};
#endif
#if defined(MRF24J40)
ROM BYTE    RSSIlookupTable[] ={90,89,88,88,88,87,87,87,87,86,86,86,86,85,85,85,85,85,84,84,84,84,84,83,83,83,83,82,82,82,82,82,81,81,81,81,81,80,80,80,80,80,80,79,79,79,79,79,78,78,78,78,78,77,77,77,77,77,76,76,76,76,76,75,75,75,75,75,74,74,74,74,74,73,73,73,73,73,72,72,72,72,72,71,71,71,71,71,71,70,70,70,70,70,70,69,69,69,69,69,68,68,68,68,68,68,68,67,67,67,67,66,66,66,66,66,66,65,65,65,65,64,64,64,64,63,63,63,63,62,62,62,62,61,61,61,61,61,60,60,60,60,60,59,59,59,59,59,58,58,58,58,58,57,57,57,57,57,57,56,56,56,56,56,56,55,55,55,55,55,54,54,54,54,54,54,53,53,53,53,53,53,53,52,52,52,52,52,51,51,51,51,51,50,50,50,50,50,49,49,49,49,49,48,48,48,48,47,47,47,47,47,46,46,46,46,45,45,45,45,45,44,44,44,44,43,43,43,42,42,42,42,42,41,41,41,41,41,41,40,40,40,40,40,40,39,39,39,39,39,38,38,38,37,36,35};
#endif

//PingPong Variables
BOOL    PingPongSend = FALSE;
BOOL    PingPongReceive = FALSE;
BYTE    result = TRUE;
BYTE    case_value = 0;
BYTE    previous_state = 0;
BYTE    PingPong_Count = 0;
BYTE    PingPong_RxCount = 0;
BYTE    PingPong_Package = 10;
/*************************************************************************/

// The variable myChannel defines the channel that the device
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).

/*************************************************************************/

BYTE        myChannel = 11; 
BOOL        enSelfTest = FALSE;
ROM char    PingPongPacket[] = {0x01, 0xFF, 0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF};
                               //Random payload

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
* Overview:		    This is the main function that runs the simple 
*                   example demo. The purpose of this example is to
*                   demonstrate the simple application programming
*                   interface for the MiWi(TM) Development 
*                   Environment. By virtually total of less than 30 
*                   lines of code, we can develop a complete 
*                   application using MiApp interface. The 
*                   application will first try to establish a P2P 
*                   link with another device and then process the 
*                   received information as well as transmit its own 
*                   information.
*                   MiWi(TM) DE also support a set of rich 
*                   features. Example code FeatureExample will
*                   demonstrate how to implement the rich features 
*                   through MiApp programming interfaces.
*
* Note:			    
**********************************************************************/
void main (void)
{
    BYTE    i;
    BYTE    TxSynCount = 0;
    BYTE    TxSynCount2 = 0;
    BoardInit();            //Has LCDInit() also covered in this, need to make this separate to ensure that if there was a problem with I2C the board hangs up
    ConsoleInit();

    // Initialize the system

    /*******************************************************************/
    
    LCDBacklightON();
    LCDDisplay((char *)"8-Bit Wireless  Development Kit ", 0, TRUE);
    DelayMs(5000);
    LCDBacklightOFF();


    Printf("\r\nInput Configuration:");
    Printf("\r\n           Button 1: RB0");
    Printf("\r\n           Button 2: RB2");
    Printf("\r\nOutput Configuration:");
    Printf("\r\n              LED 1: RA2");
    Printf("\r\n              LED 2: RA3");
    Printf("\r\n              LED 3: RB1");
            #if defined(MRF24J40)
    Printf("\r\n     RF Transceiver: MRF24J40");
            #elif defined(MRF49XA)
    Printf("\r\n     RF Transceiver: MRF49XA");
            #elif defined(MRF89XA)
    Printf("\r\n     RF Transceiver: MRF89XA");
            #endif
    Printf("\r\n   Demo Instruction:");
    Printf("\r\n                     Power on the board until LED 1 lights up  ");
    Printf("\r\n                     to indicate connecting with peer.         ");
    Printf("\r\n                     Ping Pong Results will be displayed on LCD");
    Printf("\r\n                     Use MCLR + RB2 to switch to Self Test Mode. ");
    Printf("\r\n\r\n");
    

    LED_1 = 0;
    LED_2 = 0;

    MiApp_ProtocolInit(FALSE);

    // Set default channel
    if(MiApp_SetChannel(myChannel) == FALSE)
    {
        #if defined(__18CXX)
        return;
        #else
        return (0);
        #endif
    }

    /*******************************************************************/

    // Function MiApp_ConnectionMode defines the connection mode. The
    // possible connection modes are:
    //  ENABLE_ALL_CONN:    Enable all kinds of connection
    //  ENABLE_PREV_CONN:   Only allow connection already exists in
    //                      connection table
    //  ENABL_ACTIVE_SCAN_RSP:  Allow response to Active scan
    //  DISABLE_ALL_CONN:   Disable all connections.

    /*******************************************************************/
    MiApp_ConnectionMode(ENABLE_ALL_CONN);

    /*******************************************************************/

    // Display current opertion on LCD of demo board, if applicable

    /*******************************************************************/
    LCDDisplay((char *)"Connecting Peer  on Channel %d ", myChannel, TRUE);

    /*******************************************************************/

    // Function MiApp_EstablishConnection try to establish a new
    // connection with peer device.
    // The first parameter is the index to the active scan result,
    //      which is acquired by discovery process (active scan). If
    //      the value of the index is 0xFF, try to establish a
    //      connection with any peer.
    // The second parameter is the mode to establish connection,
    //      either direct or indirect. Direct mode means connection
    //      within the radio range; indirect mode means connection
    //      may or may not in the radio range.

    /*******************************************************************/
    #ifdef ENABLE_HAND_SHAKE
    i = 0xFF;
    while(i == 0xFF)
    {
        i = MiApp_EstablishConnection(0xFF, CONN_MODE_DIRECT);
    }
    #endif

    /*******************************************************************/

    // Display current opertion on LCD of demo board, if applicable

    /*******************************************************************/
    
    LCDDisplay((char *)"Joined  Network Successfully..", 0, TRUE);
    
    /*******************************************************************/

    // Function DumpConnection is used to print out the content of the
    //  Connection Entry on the hyperterminal. It may be useful in
    //  the debugging phase.
    // The only parameter of this function is the index of the
    //  Connection Entry. The value of 0xFF means to print out all
    //  valid Connection Entry; otherwise, the Connection Entry
    //  of the input index will be printed out.

    /*******************************************************************/
    #ifdef ENABLE_DUMP
    DumpConnection(0xFF);
    #endif
    #ifndef ENABLE_POWERSAVE

    // Turn on LED 1 to indicate P2P connection established
    LED_1 = 1;
    #endif
    DelayMs(20);
    LCDDisplay((char *)"Ping Pong Demo  RB0(TX) RB2(RX)", 0, TRUE);

    /*******************************************************************/

    // Following block display demo instructions on LCD based on the
    // demo board used.

    /*******************************************************************/
    /*Configure the device in transmit or Receive Mode*/
   
        
        ReadButtonPress();   

        while(1)
        {
            
            PingPongStateMachine();
            
        }                   //end of while(1)
    
}                           //end of main

/*********************************************************************
* Function:         void PingPongStateMachine()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Switches from TX to RX based on the state machine
*
* Side Effects:	    none
*
* Overview:
*
* Note:			    
**********************************************************************/


void PingPongStateMachine()
{
    BYTE    i;
    WORD    j, k;
    volatile BYTE    packagerssi;
    switch(case_value)
            {
                case 0:     //Send 0x01 frame to initiate ping - pong test
                    MiApp_FlushTx();
                    MiApp_WriteData(0x01);
                    for(i = 0; i < sizeof(PingPongPacket); i++)
                    {
                        MiApp_WriteData(PingPongPacket[i]);
                    }

                    /*******************************************************************/

                    // Function MiApp_BroadcastPacket is used to broadcast a message
                    //    The only parameter is the boolean to indicate if we need to
                    //       secure the frame

                    /*******************************************************************/
                    MiApp_BroadcastPacket(FALSE);
                    PingPong_Count = 0;
                    previous_state = 0;
                    case_value = 8;
                    LCDDisplay((char *)"Transmitting...", 0, TRUE);

                    //Printf("\r\nIn case 0");
                    break;

                case 1:     //Send 0x04 frames - indicating data transmission
                    MiApp_FlushTx();
                    MiApp_WriteData(0x04);
                    for(i = 0; i < sizeof(PingPongPacket); i++)
                    {
                        MiApp_WriteData(PingPongPacket[i]);
                    }

                    if(PingPong_Count != PingPong_Package)
                    {
                        MiApp_BroadcastPacket(FALSE);
                        case_value = 1;
                        PingPong_Count++;
                        LED_1 ^= 1;
                        LCDErase();
                        sprintf((char *)LCDText, (far rom char *) "Transmitting...");
                        sprintf((char *) &(LCDText[16]), (far rom char *) "Count: %d", PingPong_Count);
                        LCDUpdate();
                    }
                    else
                    {
                        Printf("\r\nSent Packet Count: ");
                        PrintDec(PingPong_Package);
                        PingPong_Count = 0;
                        case_value = 2;
                    }

                    previous_state = 1;

                    //Printf("\r\nIn case 1");
                    break;

                case 2:

                    //Send 0x02 frame to get status response
                    MiApp_FlushTx();
                    MiApp_WriteData(0x02);
                    for(i = 0; i < sizeof(PingPongPacket); i++)
                    {
                        MiApp_WriteData(PingPongPacket[i]);
                    }

                    MiApp_BroadcastPacket(FALSE);
                    previous_state = 2;
                    case_value = 8;

                    //Printf("\r\n In case 2");
                    break;

                case 3:

                    //Ping Pong Receive Mode
                    if(MiApp_MessageAvailable())
                    {
                        if(rxMessage.Payload[0] == 0x01)
                        {
                            BYTE    rssi = rxMessage.PacketRSSI;
                            #if defined(MRF24J40)
                            rssi = RSSIlookupTable[rssi];
                            #endif
                            MiApp_DiscardMessage();
                            MiApp_FlushTx();
                            MiApp_WriteData(0x03);
                            for(i = 0; i < sizeof(PingPongPacket); i++)
                            {
                                MiApp_WriteData(PingPongPacket[i]);
                            }

                            MiApp_BroadcastPacket(FALSE);
                            PingPong_RxCount = 0;
                            if((rssi > 0) & (rssi < 80))
                                packagerssi = rssi;
                            //LCDDisplay((char *)"Ping Pong Test  Rcvng.. RSSI:", rssi, TRUE);
                            LCDErase();
                            sprintf((char *)LCDText, (far rom char *) "Receiving.. ");
                            #if defined(MRF24J40)
                            sprintf((char *) &(LCDText[16]), (far rom char *) "RSSI (dB): --");
                            #else
                            sprintf((char *) &(LCDText[16]), (far rom char *) "RSSI (dB): --");
                            #endif
                            LCDUpdate();
                            
                        }
                        else if(rxMessage.Payload[0] == 0x04)
                        {
                            BYTE    rssi = rxMessage.PacketRSSI;
                            
                            #if defined(MRF24J40)
                            rssi = RSSIlookupTable[rssi];
                            #endif
                            MiApp_DiscardMessage();
                            PingPong_RxCount++;
                            previous_state = 3;
                            LED_2 ^= 1;

                            LCDErase();
                            sprintf((char *)LCDText, (far rom char *) "Receiving..  %d", PingPong_RxCount);
                            #if defined(MRF24J40)
                            sprintf((char *) &(LCDText[16]), (far rom char *) "RSSI (dB): -%d", packagerssi);
                            #else
                            sprintf((char *) &(LCDText[16]), (far rom char *) "RSSI (dB):  -%d", packagerssi);
                            #endif
                            LCDUpdate();

                            case_value++;
                        }
                        else if(rxMessage.Payload[0] == 0x02)
                        {
                            Printf("\r\nReceived Packet Count:");
                            PrintDec(PingPong_RxCount);
                            MiApp_DiscardMessage();
                            case_value = case_value + 2;
                        }
                        else
                        {

                            //Printf("\r\nIllegal packet received with payload first byte set to - ");
                            //PrintDec(rxMessage.Payload[0]);
                            MiApp_DiscardMessage();
                        }
                    }

                    //Printf("\r\n In case 3");
                    break;

                case 4:
                    if(MiApp_MessageAvailable())
                    {
                        if(rxMessage.Payload[0] == 0x04)
                        {
                            BYTE    rssi = rxMessage.PacketRSSI;
                            MiApp_DiscardMessage();
                            PingPong_RxCount++;
                            LED_2 ^= 1;

                            LCDErase();
                            sprintf((char *)LCDText, (far rom char *) "Receiving..  %d", PingPong_RxCount);
                            #if defined(MRF24J40)
                            sprintf((char *) &(LCDText[16]), (far rom char *) "RSSI (dB): -%d", packagerssi);
                            #else
                            sprintf((char *) &(LCDText[16]), (far rom char *) "RSSI (dB): -%d", packagerssi);
                            #endif
                            LCDUpdate();
                        }
                        else if(rxMessage.Payload[0] == 0x02)
                        {
                            Printf("\r\nReceived Packet Count:");
                            PrintDec(PingPong_RxCount);
                            MiApp_DiscardMessage();
                            case_value++;
                        }
                        else
                        {

                            //Printf("\r\nIllegal packet received with payload first byte set to - ");
                            //PrintDec(rxMessage.Payload[0]);
                            //Printf("\r\n");
                            MiApp_DiscardMessage();
                        }
                    }

                    //Printf("\r\n In case 4");
                    break;

                case 5:
                    MiApp_FlushTx();
                    MiApp_WriteData(0x03);
                    for(i = 0; i < sizeof(PingPongPacket); i++)
                    {
                        MiApp_WriteData(PingPongPacket[i]);
                    }

                    MiApp_BroadcastPacket(FALSE);
                    previous_state = 0;
                    case_value = 8;

                    //Printf("\r\n In case 5");
                    break;

                case 8:
                    case_value = previous_state;
                    DelayMs(40);
                    if(MiApp_MessageAvailable())
                    {
                        if(rxMessage.Payload[0] == 0x03)
                        {
                            case_value = (previous_state + 1);
                        }

                        if(rxMessage.Payload[0] == 0x01)
                        {
                            case_value = (previous_state + 1);
                        }

                        if(rxMessage.Payload[0] == 0x02)
                        {
                            case_value = 5;
                        }

                        MiApp_DiscardMessage();
                    }

                    //Printf("\r\nIn case 8");
                    break;

                default:
                    break;
            }               //end of switch statement
}
/*********************************************************************
* Function:         void ReadButtonPress()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Waits until user hits either RB0 or RB2
*
* Side Effects:	    none
*
* Overview:
*
* Note:			    
**********************************************************************/

void ReadButtonPress()
{
    while(result == TRUE)
        {
            if(PUSH_BUTTON_RB0 == 0)
            {

                //Configure the device as Ping Pong Sender
                while(PUSH_BUTTON_RB0== 0);
                PingPongSend = TRUE;
                case_value = 0;
                previous_state = 0;
                PingPong_RxCount = 0;
                PingPong_Count = 0;
                result = FALSE;
            }

            if(PUSH_BUTTON_RB2 == 0)
            {

                //Configure the device as Ping Pong Receiver
                while(PUSH_BUTTON_RB2 == 0);
                PingPongReceive = TRUE;
                case_value = 3;
                previous_state = 0;
                PingPong_RxCount = 0;
                PingPong_Count = 0;
                result = FALSE;
            }
        };
}        
    
