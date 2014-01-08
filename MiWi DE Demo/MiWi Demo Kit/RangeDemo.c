/********************************************************************
/********************************************************************
* FileName:		RangeDemo.c
* Dependencies:    
* Processor:	PIC18	
* Complier:     Microchip C18 v3.04 or higher
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
*  This file runs the Range Test Demo
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/02/2011    ccs       Initial revision
********************************************************************/

#include "RangeDemo.h"

#define TX_PKT_INTERVAL             4
#define DISPLAY_RSSI_INTERVAL       4
#define EXIT_DEMO_TIMEOUT           6

#define EXIT_PKT    1
#define RANGE_PKT   2
#define ACK_PKT     4

#if defined(MRF24J40)
ROM BYTE RSSIlookupTable [] =   {90,89,88,88,88,87,87,87,87,86,86,86,86,85,85,85,85,85,84,84,84,
                                84,84,83,83,83,83,82,82,82,82,82,81,81,81,81,81,80,80,80,80,80,
                                80,79,79,79,79,79,78,78,78,78,78,77,77,77,77,77,76,76,76,76,76,
                                75,75,75,75,75,74,74,74,74,74,73,73,73,73,73,72,72,72,72,72,71,
                                71,71,71,71,71,70,70,70,70,70,70,69,69,69,69,69,68,68,68,68,68,
                                68,68,67,67,67,67,66,66,66,66,66,66,65,65,65,65,64,64,64,64,63,
                                63,63,63,62,62,62,62,61,61,61,61,61,60,60,60,60,60,59,59,59,59,
                                59,58,58,58,58,58,57,57,57,57,57,57,56,56,56,56,56,56,55,55,55,
                                55,55,54,54,54,54,54,54,53,53,53,53,53,53,53,52,52,52,52,52,51,
                                51,51,51,51,50,50,50,50,50,49,49,49,49,49,48,48,48,48,47,47,47,
                                47,47,46,46,46,46,45,45,45,45,45,44,44,44,44,43,43,43,42,42,42,
                                42,42,41,41,41,41,41,41,40,40,40,40,40,40,39,39,39,39,39,38,38,
                                38,37,36,35};
#elif defined(MRF89XA)
ROM BYTE RSSIlookupTable [] =   {100,89,88,88,88,87,87,87,87,86,86,86,86,85,85,85,85,85,84,84,84,
                                84,84,83,83,83,83,82,82,82,82,82,81,81,81,81,81,80,80,80,80,80,
                                80,79,79,79,79,79,78,78,78,78,78,77,77,77,77,77,76,76,76,76,76,
                                75,75,75,75,75,74,74,74,74,74,73,73,73,73,73,72,72,72,72,72,71,
                                71,71,71,71,71,70,70,70,70,70,70,69,69,69,69,69,68,68,68,68,68,
                                68,68,67,67,67,67,66,66,66,66,66,66,65,65,65,65,64,64,64,64,63,
                                63,63,63,62,62,62,62,61,61,61,61,61,60,60,60,60,60,59,59,59,59,
                                59,58,58,58,58,58,57,57,57,57,57,57,56,56,56,56,56,56,55,55,55,
                                55,55,54,54,54,54,54,54,53,53,53,53,53,53,53,52,52,52,52,52,51,
                                51,51,51,51,50,50,50,50,50,49,49,49,49,49,48,48,48,48,47,47,47,
                                47,47,46,46,46,46,45,45,45,45,45,44,44,44,44,43,43,43,42,42,42,
                                42,42,41,41,41,41,41,41,40,40,40,40,40,40,39,39,39,39,39,38,38,
                                38,37,36,40};
#endif


extern BYTE ConnectionEntry;
/*********************************************************************
* Function:         void RangeDemo(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Following routine 
*                    
*
* Note:			    
**********************************************************************/                                
void RangeDemo(void)
{
    BOOL Run_Demo = TRUE;
    BOOL Tx_Packet = TRUE;
    BYTE rssi = 0;
	BYTE Pkt_Loss_Cnt = 0;
	MIWI_TICK tick1, tick2;
    BYTE switch_val;
    BYTE pktCmd = 0;
    

	/*******************************************************************/
    // Dispaly Range Demo Splach Screen
    /*******************************************************************/	
    LCDBacklightON();
    LCDDisplay((char *)"   Microchip       Range Demo  ", 0, TRUE);
    LCDBacklightOFF();
        
	/*******************************************************************/
    // Read Start tickcount
    /*******************************************************************/	
	tick1 = MiWi_TickGet();
    
    while(Run_Demo)
    {	
        /*******************************************************************/
        // Read current tickcount
        /*******************************************************************/
    	tick2 = MiWi_TickGet();
        
    	// Send a Message
        if((MiWi_TickGetDiff(tick2,tick1) > (ONE_SECOND * TX_PKT_INTERVAL)))
    	{
        	LCDErase();
        	
        	if(Tx_Packet)
        	{	
            	LCDDisplay((char *)"Checking Signal Strength...", 0, TRUE);
            		
        	    MiApp_FlushTx();
        	    
        	    MiApp_WriteData(RANGE_PKT);
        	    MiApp_WriteData(0x4D);
        	    MiApp_WriteData(0x69);
        	    MiApp_WriteData(0x57);
        	    MiApp_WriteData(0x69);
        	    MiApp_WriteData(0x20);
        	    MiApp_WriteData(0x52);
        	    MiApp_WriteData(0x6F);
        	    MiApp_WriteData(0x63);
        	    MiApp_WriteData(0x6B);
        	    MiApp_WriteData(0x73);
        	    MiApp_WriteData(0x21);
        	    	    
                if( MiApp_UnicastConnection(ConnectionEntry, FALSE) == FALSE )
                    Pkt_Loss_Cnt++;
        	    else
        	        Pkt_Loss_Cnt = 0;
        	        
        	    Tx_Packet = FALSE;   		
    		}
    		else
    		{	      	
            	if(Pkt_Loss_Cnt < 1)
            	{   
                    #if defined(MRF24J40)         		
            		if(rssi > 120)
                    #else
                    if(rssi > 100)
                    #endif    
            		{
                		sprintf((char *)&LCDText, (far rom char*)"Strength: High ");
                        LED0 = 1;
                        LED1 = 0;
                        LED2 = 0;
                    }
                    #if defined(MRF24J40)  		
                	else if(rssi < 121 && rssi > 60)
                    #else
                    else if(rssi < 101 && rssi > 60)
                    #endif
            		{
                		sprintf((char *)&LCDText, (far rom char*)"Strength: Medium");
                        LED0 = 1;
                        LED1 = 1;
                        LED2 = 0;
                    } 
                    #if defined(MRF24J40)
                  	else if(rssi < 61)
                    #else
                    else if(rssi < 61)
                    #endif
            		{
                		sprintf((char *)&LCDText, (far rom char*)"Strength: Low");
                        LED0 = 0;
                        LED1 = 1;
                        LED2 = 0;
                    }
                    
                    // Convert to dB
        		    //rssi = RSSIlookupTable[rssi];
                  	sprintf((char *)&(LCDText[16]), (far rom char*)"Rcv RSSI: %03d", rssi);                  	                  
                }       
                else
                {
                        LCDDisplay((char *)"No Device Found or Out of Range ", 0, TRUE);
                        LED0 = 0;
                        LED1 = 0;
                        LED2 = 1;
                }

                LCDUpdate();
                Tx_Packet = TRUE;
                        		
            } 	     
    	    /*******************************************************************/
            // Read New Start tickcount
            /*******************************************************************/
      		tick1 = MiWi_TickGet(); 
      		  
        }
        
    	// Check if Message Available
    	if(MiApp_MessageAvailable())
    	{
            pktCmd = rxMessage.Payload[0];
        	if(pktCmd == EXIT_PKT)
        	{
            	MiApp_DiscardMessage();
            	MiApp_FlushTx();
    	        MiApp_WriteData(ACK_PKT);
    	        MiApp_UnicastConnection(ConnectionEntry, FALSE);
            	Run_Demo = FALSE;
            	LCDBacklightON();
                LCDDisplay((char *)"   Exiting....     Range Demo  ", 0, TRUE);
                LCDBacklightOFF();

                
            }
            else if(rxMessage.Payload[0] == RANGE_PKT)
            {
            	// Get RSSI value from Recieved Packet
            	rssi = rxMessage.PacketRSSI;
        	    #if defined(MRF89XA)
                    rssi = rssi<<1;
                #endif
        	    // Disguard the Packet so can recieve next
        	    MiApp_DiscardMessage();
        	}
        	else
            	MiApp_DiscardMessage(); 
    	}
    	
    	// Check if  Switch Pressed
    	switch_val = ButtonPressed();
    	
        if((switch_val == SW1) || (switch_val == SW2))
        {
            /*******************************************************************/
            // Send Exit Demo Request Packet and exit Range Demo
            /*******************************************************************/
            MiApp_FlushTx();    
            MiApp_WriteData(EXIT_PKT);
            MiApp_UnicastConnection(ConnectionEntry, FALSE);
   
            LCDBacklightON();
            LCDDisplay((char *)"   Exiting....     Range Demo  ", 0, TRUE);
            LCDBacklightOFF();
            
            tick1 = MiWi_TickGet();
            // Wait for ACK Packet
            while(Run_Demo)
            {
                if(MiApp_MessageAvailable())
                {
                    if(rxMessage.Payload[0] == ACK_PKT)          
                        Run_Demo = FALSE;
                        
                    MiApp_DiscardMessage();
                }
                if ((MiWi_TickGetDiff(tick2,tick1) > (ONE_SECOND * EXIT_DEMO_TIMEOUT)))
                    Run_Demo = FALSE;
                    
                tick2 = MiWi_TickGet();
            }    
        }    
    }
}
    	    