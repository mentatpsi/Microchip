/****************************************************************************
* FileName:		DemoOutput.c
* Dependencies: none   
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher	
*               Microchip C32 v1.02 or higher
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice for MiWi DE Software:
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
****************************************************************************
* File Description:
*
*  This is the output for the demo. The output information is displayed on
*  hyper terminal and LCD screen if demo board has one.
*
* Change History:
*  Rev   Date         Author    Description
*  4.1   1/31/2012    yfy       MiWi DE 4.2, simplified demo interface
**************************************************************************/

#include "WirelessProtocols/Console.h"
#include "ConfigApp.h"
#include "HardwareProfile.h"
#include "WirelessProtocols/LCDBlocking.h"
#include "WirelessProtocols/MCHP_API.h"

/*************************************************************************/
// the following two variable arrays are the data to be transmitted
// in this demo. They are bit map of English word "MiWi" and "DE"
// respectively.
/*************************************************************************/
ROM const BYTE MiWi[6][21] = 
{
    {0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x20,0x20,0xB2,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x0D,0x0A},
    {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0D,0x0A}
    
};   

ROM const BYTE DE[6][11] = 
{
    {0xB2,0xB2,0xB2,0x20,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x0D,0x0A},
    {0xB2,0xB2,0xB2,0x20,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A},
    {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0D,0x0A}
}; 
void DemoOutput_Greeting(void)
{
    #if defined(MRF49XA)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Simple P2P Demo  MRF49XA Node 2", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Simple MiWi Demo MRF49XA Node 2", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI_PRO)
            LCDDisplay((char *)"Simple MiWi PRO  MRF49XA Node 2", 0, TRUE); 
        #endif
    #elif defined(MRF24J40)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Simple P2P Demo MRF24J40 Node 2", 0, TRUE);
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Simple MiWi DemoMRF24J40 Node 2", 0, TRUE);
        #endif
        #if defined(PROTOCOL_MIWI_PRO)
            LCDDisplay((char *)"Simple MiWi PRO MRF24J40 Node 2", 0, TRUE);
        #endif
    #elif defined(MRF89XA)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Simple P2P Demo  MRF89XA Node 2", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Simple MiWi Demo MRF89XA Node 2", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI_PRO)
            LCDDisplay((char *)"Simple MiWi PRO  MRF89XA Node 2", 0, TRUE); 
        #endif
    #endif

    #if defined(PROTOCOL_P2P)
        Printf("\r\nStarting Node 2 of Simple Demo for MiWi(TM) P2P Stack ...");  
    #endif
    #if defined(PROTOCOL_MIWI)
        Printf("\r\nStarting Node 2 of Simple Demo for MiWi(TM) Stack ...");  
    #endif 
    #if defined(PROTOCOL_MIWI_PRO)
        Printf("\r\nStarting Node 2 of Simple Demo for MiWi(TM) PRO Stack ...");  
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
    Printf("\r\n                     to indicate connecting with peer. Push");
    Printf("\r\n                     Button 1 to broadcast message. Push Button");
    Printf("\r\n                     2 to unicast encrypted message. LED 2 will");
    Printf("\r\n                     be toggled upon receiving messages. ");
    Printf("\r\n\r\n");    
    
}        

void DemoOutput_Channel(BYTE channel, BYTE Step)
{
    if( Step == 0 )
    {
        LCDDisplay((char *)"Connecting Peer  on Channel %d ", channel, TRUE);
        Printf("\r\nConnecting Peer on Channel ");
        PrintDec(channel);
        Printf("\r\n");
    }
    else
    {    
        LCDDisplay((char *)" Connected Peer  on Channel %d", channel, TRUE);
        Printf("\r\nConnected Peer on Channel ");
        PrintDec(channel);
        Printf("\r\n");
    }
}    

void DemoOutput_Instruction(void)
{
    #if defined(EXPLORER16)
        LCDDisplay((char *)"RD6: Broadcast  RD7: Unicast", 0, FALSE); 
    #elif defined(PIC18_EXPLORER)
        LCDDisplay((char *)"RB0: Broadcast  RA5: Unicast", 0, FALSE); 
    #elif defined(EIGHT_BIT_WIRELESS_BOARD)
        LCDDisplay((char *)"RB0: Broadcast  RB2: Unicast", 0, FALSE); 
    #endif
}    


void DemoOutput_HandleMessage(BYTE TxNum, BYTE RxNum)
{
    BYTE i;
    
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
            PrintChar( rxMessage.SourceAddress[1]);
            PrintChar( rxMessage.SourceAddress[0]);
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
}    

void DemoOutput_UpdateTxRx(BYTE TxNum, BYTE RxNum)
{
    LCDTRXCount(TxNum, RxNum);  
}    

void DemoOutput_ChannelError(BYTE channel)
{
    Printf("\r\nSelection of channel ");
    PrintDec(channel);
    Printf(" is not supported in current configuration.\r\n");
}    

void DemoOutput_UnicastFail(void)
{
    Printf("\r\nUnicast Failed\r\n");                  
    LCDDisplay((char *)" Unicast Failed", 0, TRUE);
}    

