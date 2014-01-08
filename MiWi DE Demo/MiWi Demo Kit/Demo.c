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
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/02/2011    ccs       Initial revision
*  1.1   7/08/2011    mynenis   Added multi channel support
*  1.2   7/12/2011    mynenis   Range Demo Retry Fix
*  1.3   2/28/2013    mynenis   Fixed the addressing issue in non IEEE 802.15.4 and reduced the
*                               latecy caused by LCD Display
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
#include "SelfTestMode.h"


// Demo Version
#define MAJOR_REV       1
#define MINOR_REV       3

/*************************************************************************/
// The variable myChannel defines the channel that the device
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).
/*************************************************************************/

// Possible channel numbers are from 0 to 31
BYTE myChannel = 26;

#define MiWi_CHANNEL        0x04000000                          //Channel 26 bitmap

#define EXIT_DEMO           1
#define RANGE_DEMO          2
#define TEMP_DEMO           3
#define IDENTIFY_MODE       4
#define EXIT_IDENTIFY_MODE  5

#define NODE_INFO_INTERVAL  5

BYTE ConnectionEntry = 0;
			
BOOL NetFreezerEnable = FALSE;

extern BYTE myLongAddress[];

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
	BOOL result = TRUE;
	BYTE switch_val;
	WORD VBG_Result;
	
	NetFreezerEnable = FALSE;

    /*******************************************************************/
    // Initialize Hardware
    /*******************************************************************/
	BoardInit();
   
    /*******************************************************************/
    // Initialize the LCD
    /*******************************************************************/
    LCDInit();
 	
 	LED0 = LED1 = LED2 = 1;
 	
 	Read_MAC_Address();
    
    /*******************************************************************/
    // If Network Freezer Is Not Enabled User Must Configure the Network
    /*******************************************************************/
    if( !NetFreezerEnable )
	{
    	/*******************************************************************/
    	// Display Start-up Splash Screen
    	/*******************************************************************/
        LCDBacklightON();
        
        LCDErase();
        sprintf((char *)LCDText, (far rom char*)"    Microchip   "  );
        sprintf((char *)&(LCDText[16]), (far rom char*)" MiWi Demo Board");
        LCDUpdate();

        /*******************************************************************/
	    // Initialize the MiWi Protocol Stack. The only input parameter indicates
	    // if previous network configuration should be restored.
	    /*******************************************************************/		
	    MiApp_ProtocolInit(FALSE);
      
        
        {
            MIWI_TICK tick1, tick2;
            tick1 = MiWi_TickGet();
            while(1)
            {
                tick2 = MiWi_TickGet();
                if(MiWi_TickGetDiff(tick2, tick1) > (ONE_SECOND * 4))
                    break;
               switch_val = ButtonPressed();
               if(switch_val == SW2)
                    SelfTest(myChannel);
               
            }
        }
        LCDBacklightOFF();

        /*******************************************************************/
        // Set-up PAN_ID
        /*******************************************************************/
CreateorJoin:
		
	    
 
        /*******************************************************************/
        //Ask Use to select channel
        /*******************************************************************/
        LCDErase();
		sprintf((char *)LCDText, (far rom char*)"SW1:<Sel Ch:%02d>", myChannel);
        sprintf((char *)&(LCDText[16]), (far rom char*)"SW2: Chnge Chnl");
        LCDUpdate();
        DelayMs(1000);

        while(1)
		{
    		switch_val = ButtonPressed();
            if(switch_val == SW1)
			{
                //User selected Default Channel
                break;
            }
            
            else if(switch_val == SW2)
			{
                BYTE select_channel = 0;
                BOOL update_channel = TRUE;

                #if defined (MRF24J40)
                    select_channel = 11;
                #else
                    select_channel = 27;
                #endif
                while(update_channel == TRUE)
                {
                    //User Selected Change Channel
                    LCDErase();
    		        sprintf((char *)LCDText, (far rom char*)"SW1:<Sel Ch:%02d>", select_channel);
                    sprintf((char *)&(LCDText[16]), (far rom char*)"SW2: Chnge Chnl");
                    LCDUpdate();
    
                    while(1)
                    {
                        switch_val = ButtonPressed();
                        if(switch_val == SW1)
                            {
                                myChannel = select_channel;
                                update_channel = FALSE;
                                break;
                            }
                        else if(switch_val == SW2)
                            {
                                select_channel = select_channel+1;
                                #if defined(MRF24J40)
                                    if(select_channel == 27) select_channel = 11;
                                #elif defined(MRF89XA)
                                    if(select_channel == 32) select_channel = 0;                                
                                #else
                                     #error "MiWi Demo is not supported for this transceiver"
                                #endif
                                break;
                            }
                    
                    } //End of while(1)
                } //End of while(result==TRUE)
                break;
            }   //End of Change Channel (switch_val == SW2)

        }           

        

        /*******************************************************************/
        // Set Device Communication Channel
        /*******************************************************************/

	    if( MiApp_SetChannel(myChannel) == FALSE )
	    {
	        LCDDisplay((char *)"ERROR: Unable toSet Channel..", 0, TRUE);
	        return;
	    }
	    
	    /*******************************************************************/
	    //  Set the connection mode. The possible connection modes are:
	    //      ENABLE_ALL_CONN:    Enable all kinds of connection
	    //      ENABLE_PREV_CONN:   Only allow connection already exists in 
	    //                          connection table
	    //      ENABL_ACTIVE_SCAN_RSP:  Allow response to Active scan
	    //      DISABLE_ALL_CONN:   Disable all connections. 
	    /*******************************************************************/
	    MiApp_ConnectionMode(ENABLE_ALL_CONN);
	    
	    /*******************************************************************/
	    // Ask User to Create or Join a Network
	    /*******************************************************************/
	    LCDDisplay((char *)"SW1: Create NtwkSW2: Join Ntwk  ", 0, FALSE);
	    DelayMs(1000);
	    	
		while(result == TRUE)
		{
    		switch_val = ButtonPressed();
    		
			/*******************************************************************/
			// Create a New Network
			/*******************************************************************/
			if(switch_val == SW1)
			{
				LCDDisplay((char *)"Creating Network", 0, TRUE);
				
				MiApp_ProtocolInit(FALSE);
				MiApp_StartConnection(START_CONN_DIRECT, 0, 0);

				LCDDisplay((char *)"Created Network Successfully", 0, TRUE);
				
				LCDErase();
                sprintf((char *)&(LCDText), (far rom char*)"PANID:%02x%02x Ch:%02d",myPANID.v[1],myPANID.v[0],myChannel);
                sprintf((char *)&(LCDText[16]), (far rom char*)"Address: %02x%02x", myShortAddress.v[1], myShortAddress.v[0]);
                LCDUpdate();
                
     			/*******************************************************************/
    			// Wait for a Node to Join Network then proceed to Demo's
    			/*******************************************************************/               
                while(!ConnectionTable[0].status.bits.isValid)
                {
                	if(MiApp_MessageAvailable())
                	    MiApp_DiscardMessage();
                }    
				result = FALSE;
				
				
			}
			
			/*******************************************************************/
			// Join a Network
			/*******************************************************************/
			if(switch_val == SW2)
			{
				volatile BYTE scanresult;
				
				LCDDisplay((char *)"  Scanning for    Networks....", 0, TRUE);
				LCDDisplay((char *)"Please Select   Network to Join ", 0, TRUE);
								
				MiApp_ProtocolInit(FALSE);

                /*******************************************************************/
                // Perform an active scan
                /*******************************************************************/

                if(myChannel < 8)
                    scanresult = MiApp_SearchConnection(10, (0x00000001 << myChannel));
                else if(myChannel < 16)
				    scanresult = MiApp_SearchConnection(10, (0x00000100 << (myChannel-8)));
                else if(myChannel < 24)
				    scanresult = MiApp_SearchConnection(10, (0x00010000 << (myChannel-16)));
                else 
				    scanresult = MiApp_SearchConnection(10, (0x01000000 << (myChannel-24)));
                
               

                /*******************************************************************/
                // Display Scan Results
                /*******************************************************************/
	            if(scanresult > 0)
	            {
                  BYTE k;  
                    
	                for(j = 0; j < scanresult; j++)
	                {
                        BYTE skip_print = FALSE;
                        if(j > 0)
                        {
                            
                            skip_print = FALSE;
                            for(k = 0; k < j; k++)
                            {
                                if((ActiveScanResults[j].PANID.v[1] == ActiveScanResults[k].PANID.v[1]) &
                                    (ActiveScanResults[j].PANID.v[0] == ActiveScanResults[k].PANID.v[0]))
                                    {
                                        skip_print = TRUE;
                                        break;
                                    }
                                
                            }
                            if(skip_print == TRUE)
                            { 
                                if(j == (scanresult-1)) j = -1;
                                continue;
                            }
                        }
                        
						// Display the index on LCD
						LCDErase();
						
						// Display if Network is Cordinator or PAN Cordinator
						
						sprintf((char *)LCDText, (far rom char*)"SW1:<PANID:%02x%02x>",ActiveScanResults[j].PANID.v[1], ActiveScanResults[j].PANID.v[0]);
						
						
						sprintf((char *)&(LCDText[16]), (far rom char*)"SW2: Additional");
						
						LCDUpdate();
					
						while(1)
						{
    						switch_val = ButtonPressed();
    						
    						/*******************************************************************/
    						// Connect to Display Network
    						/*******************************************************************/
							if(switch_val == SW1)
							{
								BYTE Status;
                                BYTE CoordCount = 0;
                                MiApp_FlushTx();
    	                        
                                for(k = 0 ; k < scanresult ; k++)
                                {
                                    if((ActiveScanResults[j].PANID.v[1] == ActiveScanResults[k].PANID.v[1]) &
                                        (ActiveScanResults[j].PANID.v[1] == ActiveScanResults[k].PANID.v[1]))
                                    {
                                        CoordCount++;
                                    }
                                }
                                if(CoordCount > 1)
                                {
                                    BYTE nodeIndex = 0;
                                    BYTE count = 0;
                                    MiApp_FlushTx();
                                    MiApp_WriteData(IDENTIFY_MODE);
                                    MiApp_WriteData(ActiveScanResults[j].PANID.v[1]);
                                    MiApp_WriteData(ActiveScanResults[j].PANID.v[0]);
    	                            MiApp_BroadcastPacket(FALSE);
                                    for(k = 0 ; k < scanresult ; k++)
                                    {
                                        if((ActiveScanResults[j].PANID.v[1] == ActiveScanResults[k].PANID.v[1]) &
                                        (ActiveScanResults[j].PANID.v[0] == ActiveScanResults[k].PANID.v[0]))
                                        {
                                                count++;
                                            	LCDErase();
						
                        						// Display Network information
                        						
                        					    sprintf((char *)LCDText, (far rom char*)"SW1:<Addr:%02x%02x>",ActiveScanResults[k].Address[1], ActiveScanResults[k].Address[0]);
                        						
                        						
                        						sprintf((char *)&(LCDText[16]), (far rom char*)"SW2: Additional");
                        						
                        						LCDUpdate();
                                                nodeIndex = k;

                                                while(1)
                                                {
                                                    switch_val = ButtonPressed();
                                                    if(switch_val == SW1)
                                                    {
                                                       //Establish connection with the node
                                                       j = nodeIndex;
                                                       k = scanresult-1;
                                                       break;
                                                    }
                                                    else if(switch_val == SW2)
                                                    {
                                                        //Display Additional node information
                                                        if((k == (scanresult - 1)) || (count == CoordCount))
                                                          {
                                                             k = -1;
                                                             count = 0;
                                                          }  
                                                       break;
                                                    }
                                                } //End of while(1) loop
                                        }
                                         
                                          
                                    } //End of for(k = 0; ....)
                                } //End of if (CoordCount > ...)							
        						/*******************************************************************/
        						// Establish Connection and Display results of connection
        						/*******************************************************************/								    
								Status = MiApp_EstablishConnection(j, CONN_MODE_DIRECT);
								if(Status == 0xFF)
								{
							     	LCDDisplay((char *)"Join Failed!!!", 0, TRUE); 
									goto CreateorJoin;
								}
								else
								{
									LCDDisplay((char *)"Joined  Network Successfully..", 0, TRUE);
									result = FALSE;
								}
                                MiApp_FlushTx();
                                MiApp_WriteData(EXIT_IDENTIFY_MODE);
                                MiApp_WriteData(myPANID.v[1]);
                                MiApp_WriteData(myPANID.v[0]);
    	                        MiApp_BroadcastPacket(FALSE);				
								break;	
                                
							}
							
							/*******************************************************************/
    						// Display Next Network in Scan List
    						/*******************************************************************/	
							else if(switch_val == SW2)
							{
								if((scanresult-j-1) == 0)
								    j = -1;
								    
								break;
							}
						}
					    /*******************************************************************/
						// If Connected to a Network Successfully bail out
						/*******************************************************************/
						if(result == FALSE)
						    break;									
                    }
							
					result = FALSE;
            	}
				else
				{
					LCDDisplay((char *)"No Network FoundSW2: Re-Scan", 0, FALSE);
				}
			}
		}
	}
	
	while(1)
	{
    	BYTE pktCMD = 0;
    	BYTE switch_val, menu_choice = 0;
    	result = FALSE;
    	
    	
    	LED0 = LED1 = LED2 = 0;
    	
        /*******************************************************************/
        // Ask User which Demo to Run
        /*******************************************************************/
        LCDDisplay((char *)"SW1: Range Demo SW2: Other Apps  ", 0, FALSE);
	   	
        /*******************************************************************/
        // Wait for User to Select Demo to Run 
        /*******************************************************************/
        while(result == FALSE)
        {  	
            // Read Switches
            switch_val = ButtonPressed();
            
        	// Check if MiMAC has any pkt's to processes
        	if(MiApp_MessageAvailable())
        	{
            	pktCMD = rxMessage.Payload[0];
                if(pktCMD == IDENTIFY_MODE)
                    {
                        if((rxMessage.Payload[1] != myPANID.v[1]) || (rxMessage.Payload[2] != myPANID.v[0]))
                            pktCMD = 0;
                            
                    }
                if(pktCMD == RANGE_DEMO)
                    {
                        for(i = 0 ; i< CONNECTION_SIZE ; i++)
                        {
                            if(ConnectionTable[i].status.bits.isValid)
                            {
                                if((ConnectionTable[i].AltAddress.v[1] == rxMessage.SourceAddress[1]) && (ConnectionTable[i].AltAddress.v[0] == rxMessage.SourceAddress[0]))
                                {
                                    ConnectionEntry = i;
                                    
                                    break;                           
                                }
                            }
                        }
                    }
            	MiApp_DiscardMessage();
            }
            if(pktCMD == RANGE_DEMO)
            {
                MiApp_FlushTx();
    	        MiApp_WriteData(RANGE_DEMO);
    	        MiApp_UnicastConnection(ConnectionEntry, FALSE);
    	        // Run Range Test Demo
                RangeDemo();
                result = TRUE;
            }
        	
        	if((switch_val == SW1) && (menu_choice == 0))
    		{	
                BYTE NumberOfConnections = 0;
                
                //Reset ConnectionEntry
                ConnectionEntry = 0;
                
                for(i = 0; i < CONNECTION_SIZE; i++)
                {
                    if((ConnectionTable[i].status.bits.isValid) && (ConnectionTable[i].status.bits.isFamily))
                        NumberOfConnections++;
                }
                if(NumberOfConnections > 1)
                {
                    BOOL result = FALSE;
                    BYTE ConnectionsCount = 0;
                    //Select Peer for Range Demo

                    LCDDisplay((char *)"Select Peer Node for Range Test", 0, FALSE);

                    for(i = 0; i < CONNECTION_SIZE; i++)
    			    {
    				    if(ConnectionTable[i].status.bits.isValid)
                        {
                            // Display Peer information
                            LCDErase();
                            sprintf((char *)LCDText, (far rom char*)"SW1:<Addr:%02x%02x>",ConnectionTable[i].AltAddress.v[1], ConnectionTable[i].AltAddress.v[0]);				
                            sprintf((char *)&(LCDText[16]), (far rom char*)"SW2: Additional");
                            LCDUpdate();
                            ConnectionsCount++;
                            while(1)
                            {
                                switch_val = ButtonPressed();
                                if(switch_val == SW1)
                                    {
                                        ConnectionEntry = i;
                                        result = TRUE;
                                        break;
                                    }
                                else if(switch_val == SW2)
                                    {
                                        if(ConnectionsCount == NumberOfConnections) 
                                        {
                                            i = -1;
                                            ConnectionsCount = 0;
                                        }
                                        break;
                                    }
                            }
                        }
                        if(result == TRUE) break;
                    }  
                }       
        		MiApp_FlushTx();
    	        MiApp_WriteData(RANGE_DEMO);
    	        MiApp_UnicastConnection(ConnectionEntry, FALSE);
    	        
        		// Run Range Test Demo
                RangeDemo();
                result = TRUE;
    		}
            if((switch_val == SW2))
            {
                menu_choice++;
                if(menu_choice == 3) menu_choice = 0;
                if(menu_choice == 0)
                {
                    LCDDisplay((char *)"SW1: Range Demo SW2: Other Apps  ", 0, FALSE);
                }
                else if(menu_choice == 1)
                {
                    LCDDisplay((char *)"SW1: Temp Demo  SW2: Other Apps  ", 0, FALSE);
                }    
                else if(menu_choice == 2)
                {
                    LCDDisplay((char *)"SW1: Node Info  SW2: Other Apps  ", 0, FALSE);
                }
                
            }
            if((pktCMD == TEMP_DEMO) || ((switch_val == SW1) && (menu_choice == 1)))
            {
                MiApp_FlushTx();
                MiApp_WriteData(TEMP_DEMO);
                MiApp_BroadcastPacket(FALSE);

                    // Run Temp Sensor Demo
                TempDemo();
                result = TRUE;
            }
            if((pktCMD == IDENTIFY_MODE) || ((switch_val == SW1) && (menu_choice == 2)))
            {
                MIWI_TICK tick1 = MiWi_TickGet();
                LCDErase();
                sprintf((char *)&(LCDText), (far rom char*)"PANID:%02x%02x Ch:%02d",myPANID.v[1],myPANID.v[0],myChannel);
                #if defined(IEEE_802_15_4)
                    sprintf((char *)&(LCDText[16]), (far rom char*)"Address: %02x%02x", myShortAddress.v[1], myShortAddress.v[0]);
                #else
                    sprintf((char *)&(LCDText[16]), (far rom char*)"Addr:%02x%02x/%02x%02x", myShortAddress.v[1], myShortAddress.v[0], myLongAddress[1],myLongAddress[0]);
                #endif
                LCDUpdate();
                pktCMD = 0;
            }
            
            if(pktCMD == EXIT_IDENTIFY_MODE)
            {
               
                result = TRUE;
               
            }
    	}
	}
}
