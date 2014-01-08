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
*   setups a application for chatting wirelessly. 
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
/************************** VARIABLES ************************************/

/*************************************************************************/

// The variable myChannel defines the channel that the device
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).

/*************************************************************************/
BYTE        myChannel = 11;

//Chat Window Application Variables
#define MAX_MESSAGE_LEN 35
BYTE        TxMessage[MAX_MESSAGE_LEN];
BYTE        TxMessageSize = 0;

//Uses ReadMacAddress routine to give unique ID to each node
extern BYTE myLongAddress[];

//Chat Application status variables
BOOL    messagePending = FALSE;
BOOL    transmitPending = FALSE;

//Timers used in the application to determine 'no activity'
MIWI_TICK    tickCurrent;
MIWI_TICK    tickPrevious;




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
* Overview:		    This is the main function that runs the Chat application
*
* Note:			    
**********************************************************************/

void main (void)
{
    BYTE    i;
    
    
    BoardInit();            
    ConsoleInit();

    /*******************************************************************/

    // Initialize the system

    /*******************************************************************/

    /*******************************************************************/

    // Following block display demo information on LCD of Explore 16 or
    // PIC18 Explorer demo board.

    /*******************************************************************/
    LCDDisplay((char *)"8-bit Wireless  Development Kit", 0, TRUE);

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
    Printf("\r\n                     Use Console to Chat with the Peer Devices");
    Printf("\r\n\r\n");

    LED_1 = 0;
    LED_2 = 0;

    ReadMacAddress();
    /*******************************************************************/

    // Initialize Microchip proprietary protocol. Which protocol to use
    // depends on the configuration in ConfigApp.h

    /*******************************************************************/

    /*******************************************************************/

    // Function MiApp_ProtocolInit initialize the protocol stack. The
    // only input parameter indicates if previous network configuration
    // should be restored. In this simple example, we assume that the
    // network starts from scratch.

    /*******************************************************************/
    MiApp_ProtocolInit(FALSE);

    // Set default channel
    if(MiApp_SetChannel(myChannel) == FALSE)
    {
        return;
        Printf("\r\nUnable to Program the Radio Channel\r\n");   
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
    i = MiApp_EstablishConnection(0xFF, CONN_MODE_DIRECT);
    #endif

    /*******************************************************************/

    // Display current opertion on LCD of demo board, if applicable

    /*******************************************************************/
    if(i != 0xFF)
    {
        LCDDisplay((char *)"Joined  Network Successfully..", 0, TRUE);
        Printf("\r\nConnection Established with the Peer Device\r\n");
    }
    else
    {
        MiApp_StartConnection(START_CONN_DIRECT, 10, 0);
    }

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
    DelayMs(100);
    LCDDisplay((char *)"Chat Window DemoUse Console App", 0, TRUE);

    /*******************************************************************/

    // Following block display demo instructions on LCD based on the
    // demo board used.

    /*******************************************************************/
    Printf("-------------------------------------------------------\r\n");
    Printf("Chat Window: \r\n");
    Printf("-------------------------------------------------------\r\n");
    Printf("$$");

    while(1)
    {
        if(MiApp_MessageAvailable())
        {  
            ProcessRxMessage();      
        }

        if(ConsoleIsGetReady())
        {
            FormatTxMessage();
            
        }

        if(messagePending)
        {
            tickCurrent = MiWi_TickGet();
            if
            (
                (MiWi_TickGetDiff(tickCurrent, tickPrevious) > (ONE_SECOND * 30)) ||
                (TxMessageSize >= MAX_MESSAGE_LEN) ||
                (transmitPending == TRUE)
            )
            {
                
                TransmitMessage();


            }
        }
    }

    //Enable device to foward the received packet information to the console
}                           //end of main

void ProcessRxMessage()
{
    BYTE index;
    if(rxMessage.flags.bits.broadcast)
    {
        ConsolePutROMString((ROM char *) "\n -------- From group:");
    }
    else
    {
        ConsolePutROMString((ROM char *) "\n -------- From [");
    }

    if(rxMessage.flags.bits.srcPrsnt)
    
    {
        if(rxMessage.flags.bits.altSrcAddr)
        {
            PrintChar(rxMessage.SourceAddress[1]);
            PrintChar(rxMessage.SourceAddress[0]);
        }
        else
        {
            for(index = 0; index < MY_ADDRESS_LENGTH; index++)
            {
                PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH - 1 - index]);
            }
        }

        ConsolePutROMString((ROM char *) "] : ");
    }

    for(index = 1; index < rxMessage.PayloadSize; index++)
    {
        ConsolePut(rxMessage.Payload[index]);
    }

    ConsolePutROMString((ROM char *) "\r\n$$");

    // Toggle LED2 to indicate receiving a packet.
    LED_2 ^= 1;

    /*******************************************************************/

    // Function MiApp_DiscardMessage is used to release the current
    //  received packet.
    // After calling this function, the stack can start to process the
    //  next received frame

    /*******************************************************************/
    MiApp_DiscardMessage();
}           

void FormatTxMessage()
{
    BYTE    inputChar;
    
    inputChar = ConsoleGet();
    ConsolePut(inputChar);
    if(inputChar == 0x0D)
    {
        ConsolePutROMString((ROM char *) "\r\n$$");
        messagePending = TRUE;
        transmitPending = TRUE;
    }
    else if(inputChar == 0x08)
    {
        TxMessageSize--;
    }
    else
    {
        if(TxMessageSize < MAX_MESSAGE_LEN)
        {
            TxMessage[TxMessageSize] = inputChar;
            TxMessageSize++;
            tickPrevious = MiWi_TickGet();
            messagePending = TRUE;
        }
    }
}
     
     
void TransmitMessage()
{
    BYTE index;
    //Send message

    /******************************************************************/

    // First call function MiApp_FlushTx to reset the Transmit buffer.
    //  Then fill the buffer one byte by one byte by calling function
    //  MiApp_WriteData

    /*******************************************************************/
    MiApp_FlushTx();

    MiApp_WriteData(TxMessageSize);
    for(index = 0; index < TxMessageSize; index++)
    {
        MiApp_WriteData(TxMessage[index]);
    }

    //Unicast Message

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
    if(MiApp_UnicastConnection(0, TRUE) == FALSE)
    {

        //Message Failed
    }
    else
    {
        messagePending = FALSE;
        transmitPending = FALSE;
        TxMessageSize = 0;
    }
}   


/*********************************************************************
* Function:         void ReadMacAddress()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Reads MAC Address from MAC Address EEPROM
*
* Side Effects:	    none
*
* Overview:		    Uses the MAC Address from the EEPROM for addressing
*
* Note:			    
**********************************************************************/
void ReadMacAddress(void)
{
    RF_EEnCS_TRIS = 0;
    {
        BYTE    Address0, Address1, Address2;
        RF_EEnCS = 0;
        SPIPut(0x03);   //Read Sequence to EEPROM
        SPIPut(0xFA);   //MAC address Start byte
        Address0 = SPIGet();
        Address1 = SPIGet();
        Address2 = SPIGet();
        RF_EEnCS = 1;
        if((Address0 == 0x00) && (Address1 == 0x04) && (Address2 == 0xA3))  //Compare the value against Microchip's OUI
        {
            RF_EEnCS = 0;
            SPIPut(0x03);
            SPIPut(0xFD);
            myLongAddress[2] = SPIGet();
            myLongAddress[1] = SPIGet();
            myLongAddress[0] = SPIGet();
            myLongAddress[3] = 0xFE;
            myLongAddress[4] = 0xFF;
            myLongAddress[5] = 0xA3;
            myLongAddress[6] = 0x04;
            myLongAddress[7] = 0x00;

            RF_EEnCS = 1;
        }
    }
}
 
