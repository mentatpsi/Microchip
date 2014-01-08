/****************************************************************************
* FileName:		Demo.c
* Dependencies: none   
* Processor:	PIC18F46J50	
* Complier:     Microchip C18 v3.04 or higher
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
* SOFTWARE AND DOCUMENTATION ARE PROVIDED ï¿½AS ISï¿½ WITHOUT WARRANTY OF ANY 
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
*  This is the simple example that demonstrate the simple programming
*  interface of MiWi Development Environment (DE). As you may see in
*  the demo code, besides application specific code, all wireless
*  communicate code is less than 30 lines. This simple example must be 
*  used with node 2 of simple example.
*  In this simple example, following features have been demonstrated:
*   - Hand Shake
*       A hand-shaking process has been demonstrated by establishing
*       connection with a peer device.
*   - Receiving Message
*       This example demonstrate how to check received message and 
*       available information for the received data. Finally, this
*       example also show how to process the message
*   - Transmitting Message
*       This example demonstrate how to transmit message by broadcast
*       or unicast
*   - Security
*       This example demonstrate how to require the protocol stack
*       to encrypt the outgoing message. It also shows how the 
*       protocol stack automatically decrypt the incoming message
*       and provide the security status to the application layer.
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/02/2011    ccs       Initial revision
*  1.1   7/08/2011    mynenis   Added multi channel support
*  1.2   7/12/2011    mynenis   Range Demo Retry Fix
********************************************************************/

/************************ HEADERS ****************************************/
#include "ConfigApp.h"
#include "HardwareProfile.h"
#include "WirelessProtocols/MSPI.h"
#include "WirelessProtocols/MCHP_API.h"
#include "LCD_ST7032.h"
#include "TimeDelay.h"
#include "RangeDemo.h"
#include "TempDemo.h"
#include "WirelessProtocols/SymbolTime.h"
#include "MAC_EEProm.h"
#include "Transceivers/MCHP_MAC.h"
#include "WirelessProtocols/NVM.h"

// Demo Version
#define MAJOR_REV       1
#define MINOR_REV       2	

/*************************************************************************/
// The variable myChannel defines the channel that the device
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).
/*************************************************************************/

// Possible channel numbers are from 0 to 31
BYTE myChannel = 15;



#define COMMISSION          0x01
#define CONTROL_CODE        0x02
#define TRACKER_SIZE        15
#define REBROADCAST_COUNT   2
#define TRACKER_EXPIRATION  ((ONE_SECOND)*2)
struct
{
    BOOL isValid;
    BYTE sourceAddr[MY_ADDRESS_LENGTH];
    BYTE seqNum;
    MIWI_TICK expireTick;
    BYTE counter;
} BroadcastTracker[TRACKER_SIZE];   


			
BOOL NetFreezerEnable = FALSE;
BYTE connectionAddress[3];

extern BYTE myLongAddress[];
extern BYTE defaultHops;       

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
    BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x00};
#endif

void MainDisplay(void)
{
    BYTE i;
    
        LCDText[0] = 'C';
	LCDText[1] = 'H';
	LCDText[2] = ':';
	LCDText[3] = currentChannel/10+'0';
	LCDText[4] = currentChannel%10+'0';
	LCDText[5] = ' ';
	LCDText[6] = 'P';
	LCDText[7] = 'A';
	LCDText[8] = 'N';
	LCDText[9] = 'I';
	LCDText[10] = 'D';
	LCDText[11] = ':';
	if( (myPANID.v[1]>>4) > 9 )
	{
    	LCDText[12] = (myPANID.v[1]>>4)-10+'A';
    }   	
    else
    {
        LCDText[12] = (myPANID.v[1]>>4)+'0';
    }    
	if( (myPANID.v[1]&0x0F) > 9 )
	{
    	LCDText[13] = (myPANID.v[1]&0x0F)-10+'A';
    }   	
    else
    {
        LCDText[13] = (myPANID.v[1]&0x0F)+'0';
    }  
    if( (myPANID.v[0]>>4) > 9 )
	{
    	LCDText[14] = (myPANID.v[0]>>4)-10+'A';
    }   	
    else
    {
        LCDText[14] = (myPANID.v[0]>>4)+'0';
    }    
	if( (myPANID.v[0]&0x0F) > 9 )
	{
    	LCDText[15] = (myPANID.v[0]&0x0F)-10+'A';
    }   	
    else
    {
        LCDText[15] = (myPANID.v[0]&0x0F)+'0';
    }  
    
    for(i = 0; i < 8; i++)
    {
        BYTE tmp = myLongAddress[7-i];
        if( (tmp>>4) > 9 )
        { 
            LCDText[16+i*2] = (tmp>>4)-10+'A';
        }
        else
        {
            LCDText[16+i*2] = (tmp>>4)+'0';
        }
        tmp &= 0x0F;
        if( tmp > 9 )
        {
            LCDText[16+i*2+1] = tmp-10+'A';
        }    
        else
        {
            LCDText[16+i*2+1] = tmp+'0';
        }
    }

    
    LCDUpdate();
}    
                                                                                               
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
void main(void)
{   
    BYTE i, j;
    MIWI_TICK startTick, currentTick;
    BYTE FlashCount = 0;

    /*******************************************************************/
    // Initialize Hardware
    /*******************************************************************/
	BoardInit();

    /*******************************************************************/
    // Initialize the LCD
    /*******************************************************************/
    LCDInit();
 	
 	for(i = 0; i < TRACKER_SIZE; i++)
 	{
     	BroadcastTracker[i].isValid = FALSE;
        }
 	
    MiApp_ProtocolInit(TRUE);
    
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
       
    MainDisplay();
    
    /*******************************************************************/
    //  Set the connection mode. The possible connection modes are:
    //      ENABLE_ALL_CONN:    Enable all kinds of connection
    //      ENABLE_PREV_CONN:   Only allow connection already exists in 
    //                          connection table
    //      ENABLE_ACTIVE_SCAN_RSP:  Allow response to Active scan
    //      DISABLE_ALL_CONN:   Disable all connections. 
    /*******************************************************************/
    MiApp_ConnectionMode(ENABLE_ACTIVE_SCAN_RSP);

	while(1)
	{
    	
    	if(SW0_PORT == 0)
    	{
        	MainDisplay();
        }   	
    	
    	if(SW1_PORT == 0)
    	{
            while(SW1_PORT == 0);
            LCDErase();
            sprintf((char *) LCDText, (far rom char*) "Commission Mode?");
            sprintf((char *) &(LCDText[16]), (far rom char*) "SW1:Yes  SW2:No");
            LCDUpdate();

            while (1) {
                if (SW0_PORT == 0) {

                    WORD tmp = 0xFFFF;

                    while(SW0_PORT == 0);
                    myPANID.Val = MY_PAN_ID;
                    MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);
                    nvmPutMyPANID(myPANID.v);
                    currentChannel = 11;
                    MiApp_SetChannel(currentChannel);
                    MainDisplay();
                    break;
                } else if (SW1_PORT == 0) {
                    while(SW1_PORT == 0);
                    MainDisplay();
                    break;
                }


            }
           
        }   	


        if( MiApp_MessageAvailable() )
        {
            BOOL validPacket = TRUE;
            BYTE index = 0xFF;
            
            if( rxMessage.flags.bits.broadcast )
            {
                for(i = 0; i < TRACKER_SIZE; i++)
                {
                    if( BroadcastTracker[i].isValid &&
                        BroadcastTracker[i].seqNum == rxMessage.Payload[2] &&
                        isSameAddress( BroadcastTracker[i].sourceAddr, rxMessage.SourceAddress ) == TRUE )
                    {
                        index = i;
                        BroadcastTracker[index].counter++;
                        break;
                    }    
                }
            }    
            
            
            // handling received message
            if( index > TRACKER_SIZE )
            {
                if( rxMessage.Payload[3] == 0 || 
                    TRUE == isSameAddress( myLongAddress, &(rxMessage.Payload[4])) )
                {
                    BYTE PayloadIndex;
                    
                    if( rxMessage.Payload[3] == 0 )
                    {
                        PayloadIndex = 4;
                    }
                    else
                    {
                        PayloadIndex = 12;
                    }        
                    switch( rxMessage.Payload[0] )
                    {
                        case COMMISSION:
                            {
                                switch( rxMessage.Payload[1] )
                                {
                                    case 0x01:  // flash light
                                        {
                                            FlashCount = rxMessage.Payload[PayloadIndex];
                                            if(FlashCount == 0)
                                               LED0 = LED1 = LED2 = 0;
                                        }
                                        break;
                                        
                                    case 0x02:  // set channel and PANID
                                        {
                                            WORD tmp = 0xFFFF;

                                            FlashCount = 0;
                                            LED0 = LED1 = LED2 = 0;
                                            
                                            currentChannel = rxMessage.Payload[PayloadIndex];
                                            MiApp_SetChannel(currentChannel);
                                            
                                        	myPANID.v[0] = rxMessage.Payload[PayloadIndex+1];
                                        	myPANID.v[1] = rxMessage.Payload[PayloadIndex+2];
                                            MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);
                                        	nvmPutMyPANID(myPANID.v);
                                        	MainDisplay();
                                        }
                                        break; 
                                        
                                    default:
                                        break;       
                                }    
                            }
                            break;    
                        
                        
                        
                        case CONTROL_CODE:
                            {
                                switch( rxMessage.Payload[1] )
                                {
                                    case 0x00:
                                        {
                                            BKLIGHT = 0;
                                            LED0 = LED1 = LED2 = 0;
                                            
                                        }
                                        break;
                                        
                                    case 0x01:
                                        {
                                            BKLIGHT = 1;
                                            LED0 = LED1 = LED2 = 1;
                                            
                                        }
                                        break;
                                        
                                    case 0xFF:
                                        {
                                            if( (rxMessage.Payload[PayloadIndex] == 'M') &&
                                                (rxMessage.Payload[PayloadIndex+1] == 'i') &&
                                                (rxMessage.Payload[PayloadIndex+2] == 'W') &&
                                                (rxMessage.Payload[PayloadIndex+3] == 'i') )
                                            {
                                                WORD tmp = 0xFFFF;
            
                                            	myPANID.Val = MY_PAN_ID;
                                                MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);
                                            	nvmPutMyPANID(myPANID.v);
                                            	currentChannel = 11;
                                            	MiApp_SetChannel(currentChannel);
                                            	MainDisplay();
                                            }    
                                        } 
                                        break;   
                                        
                                    default:
                                        
                                        break;        
                                }    
                            }
                            break;    
                        
                        default:
                           
                            validPacket = FALSE;
                            break;
                        
                    }  
                }
                
                if( validPacket )
                {
                    // save the broadcast tracker
                    for(j = 0; j < TRACKER_SIZE; j++)
                    {
                        if( BroadcastTracker[j].isValid == FALSE )
                        {
                            index = j;
                            break;
                        }    
                    }    
                    if( index < TRACKER_SIZE )
                    {
                        BroadcastTracker[index].isValid = TRUE;
                        for(j = 0; j < MY_ADDRESS_LENGTH; j++)
                        {
                            BroadcastTracker[index].sourceAddr[j] = rxMessage.SourceAddress[j];
                        }    
                        BroadcastTracker[index].seqNum = rxMessage.Payload[2];
                        BroadcastTracker[index].expireTick = MiWi_TickGet();
                        BroadcastTracker[index].counter = 0;
                    }    
                }    
            }    

            
            
            
            // handle rebroadcast
            if( validPacket && rxMessage.flags.bits.broadcast )
            {
                if( index > TRACKER_SIZE || BroadcastTracker[index].counter < REBROADCAST_COUNT )
                {
                    // rebroadcast
                    MiApp_FlushTx();
                    for(i = 0; i < rxMessage.PayloadSize; i++)
                    {
                        MiApp_WriteData(rxMessage.Payload[i]);
                    }         
                    MiApp_BroadcastPacket( rxMessage.flags.bits.secEn );
                }    
            } 
            
            MiApp_DiscardMessage();   
        }    
        
        


    	
        currentTick = MiWi_TickGet();
        if( FlashCount > 0 )
        {
            if( MiWi_TickGetDiff(currentTick, startTick) > ONE_SECOND/2 )
            {
                startTick.Val = currentTick.Val;
                LED0 ^= 1;
                LED1 ^= 1;
                LED2 ^= 1;
                
            }
        }
        
        for(i = 0; i < TRACKER_SIZE; i++)
        {
            if( BroadcastTracker[i].isValid )
            {
                if( MiWi_TickGetDiff(currentTick, BroadcastTracker[i].expireTick) > TRACKER_EXPIRATION )
                {
                    BroadcastTracker[i].isValid = FALSE;
                }    
            }    
        }    
	}
}
