/********************************************************************
/********************************************************************
* FileName:		TempDemo.c
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
*  This file runs the Temperature Demo
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/02/2011    ccs       Initial revision
********************************************************************/

#include "TempDemo.h"

#define TEMP_SECOND_INTERVAL        5
#define DISPLAY_CYCLE_INTERVAL      4
#define EXIT_DEMO_TIMEOUT           6
#define NUM_TEMP_SAMPLES            4

#define EXIT_PKT                    1
#define TEMP_PKT                    3
#define ACK_PKT                     4

unsigned short tempAverage = 0;
BYTE tempRemote = 255;
BYTE NodeIndex = 1;
BYTE CurrentNodeIndex = 0;


extern BOOL NetFreezerEnable;
extern BYTE role;

struct TempPacket
{
	BYTE NodeAddress[2];
	BYTE TempValue;
}; 
struct TempPacket NodeTemp[10];



/*********************************************************************
* Function:         void TempDemo(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Following routine reads the on board Tempature Sensor and Broadcasts
*                   it to the Network and Dispays other Nodes connected to it's Temp Value. 
*
* Note:			    
**********************************************************************/
void TempDemo(void)
{
    BOOL Run_Demo = TRUE;
    WORD VBGResult;
    double temp;
    MIWI_TICK tick1, tick2, tick3;
    BYTE switch_val;
    
    
	/*******************************************************************/
    // Dispaly Temp Demo Splach Screen
    /*******************************************************************/	
    LCDBacklightON();
    LCDDisplay((char *)"   Microchip       Temp Demo  ", 0, TRUE);
    LCDBacklightOFF();
        
    /*******************************************************************/
    // Read Band Gap Voltage(1.2V), Since this is Battery Powered application 
    // for the ADC Temp readings for proper calculation of Temp.
    /*******************************************************************/
	VBGResult = Read_VBGVoltage();		

    /*******************************************************************/
    // Initialize Temp Data Packet
    // NodeTemp[0] = Self
    /*******************************************************************/
	NodeTemp[0].NodeAddress[0] = myShortAddress.v[0];
	NodeTemp[0].NodeAddress[1] = myShortAddress.v[1];
    
	/*******************************************************************/
    // Read Start tickcount
    /*******************************************************************/	
	tick1 = MiWi_TickGet();
    tick3 = tick1;
    
    while(Run_Demo)
    {
        /*******************************************************************/
        // Read current tickcount
        /*******************************************************************/
		tick2 = MiWi_TickGet();
		
        /*******************************************************************/
        // Check if User wants to Exit Demo
        /*******************************************************************/
        switch_val = ButtonPressed();		
	    if((switch_val == SW1) || (switch_val == SW2))
        {
            /*******************************************************************/
        	// Send Exit Demo Request Packet and exit Temp Demo
        	/*******************************************************************/ 
            MiApp_FlushTx();    
            MiApp_WriteData(EXIT_PKT);
            MiApp_BroadcastPacket(FALSE);   
            LCDBacklightON();
            LCDDisplay((char *)"   Exiting....     Temp Demo  ", 0, TRUE);
            LCDBacklightOFF();
            
            /*******************************************************************/
            // Wait for ACK Packet or Timeout
            /*******************************************************************/
            tick1 = MiWi_TickGet();
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
            
		/*******************************************************************/
		// Rotate through Displaying All Node Temp's
		/*******************************************************************/
        if ((MiWi_TickGetDiff(tick2,tick3) > (ONE_SECOND * DISPLAY_CYCLE_INTERVAL)))
		{
    		if((ConnectionTable[CurrentNodeIndex].status.bits.isValid))
    		{
    		    CurrentNodeIndex++;
    		}    
    		else
    		{
    		    CurrentNodeIndex = 0;
    		}    
    		
    		PrintTempLCD();
    		
    		tick3 = MiWi_TickGet();
        }      
		
		/*******************************************************************/
		// Read the Temp every TEMP_SECOND_INTERVAL
		/*******************************************************************/
		if ((MiWi_TickGetDiff(tick2,tick1) > (ONE_SECOND * TEMP_SECOND_INTERVAL)))
		{
			
			/*******************************************************************/
			// Power-up Temperature Sensor
			/*******************************************************************/							
			//LATAbits.LATA0 = 1;
			//DelayMs(2);         // Delay 2 ms after powering up the temperature sensor
            
            /*******************************************************************/
            // Toggle LED1 Every Temp Read Cycle
            /*******************************************************************/
            //LED0 ^= 1;
            
            /*******************************************************************/
            // Take specified number of Temp Readings
            /*******************************************************************/
            temp = ReadTempSensor(VBGResult);

            /*******************************************************************/
            // Turn Temp Sensor Off
            /*******************************************************************/
			//LATAbits.LATA0 = 0;

            /*******************************************************************/
            // Calculate Average Temp
            /*******************************************************************/			
			tempAverage = 0;
			tempAverage = temp;
			
            /*******************************************************************/
            // Reset TX Buffer Pointer
            /*******************************************************************/			
            MiApp_FlushTx();
            
            /*******************************************************************/
            // Write this Nodes temperature value and Address to the TX Buffer
            /*******************************************************************/
            MiApp_WriteData(TEMP_PKT);
           	MiApp_WriteData((BYTE) tempAverage);
		   	MiApp_WriteData(myShortAddress.v[0]);
			MiApp_WriteData(myShortAddress.v[1]);	

            
         	// Update NodeTemp Structure
			NodeTemp[0].TempValue = (BYTE)tempAverage;
					
           /*******************************************************************/
           // Broadcast Node Tempature across Network.
           /*******************************************************************/
           // Function MiApp_BroadcastPacket is used to broadcast a message
           // The only parameter is the boolean to indicate if we need to
           // secure the frame
           /*******************************************************************/
           MiApp_BroadcastPacket(FALSE);

			/*******************************************************************/
            // Read New Start tickcount
            /*******************************************************************/
      		tick1 = MiWi_TickGet();	
		}

        /*******************************************************************/
        // Check for Incomming Recieve Packet.
        /*******************************************************************/
        // Function MiApp_MessageAvailable returns a boolean to indicate if 
        // a packet has been received by the transceiver. If a packet has 
        // been received, all information will be stored in the rxFrame, 
        // structure of RECEIVED_MESSAGE.
        /*******************************************************************/
        if(MiApp_MessageAvailable())
        {
            BYTE i;
            
			/*******************************************************************/	
           	// Check if Exit Demo Packet
           	/*******************************************************************/   
        	if(rxMessage.Payload[0] == EXIT_PKT)
        	{
            	MiApp_DiscardMessage();
            	MiApp_FlushTx();
    	        MiApp_WriteData(ACK_PKT);
    	        MiApp_UnicastConnection(0, FALSE);
            	Run_Demo = FALSE;
            	LCDBacklightON();
                LCDDisplay((char *)"   Exiting....     Temp Demo  ", 0, TRUE);
                LCDBacklightOFF();
            }
            
 			/*******************************************************************/	
           	// Check if Message from know Connection
           	/*******************************************************************/            
			for(i = 0; i < CONNECTION_SIZE; i++)
			{
				if((ConnectionTable[i].status.bits.isValid) && (ConnectionTable[i].AltAddress.v[0] == rxMessage.Payload[2]) && (ConnectionTable[i].AltAddress.v[1] == rxMessage.Payload[3]))
				{
					if(rxMessage.Payload[0] == TEMP_PKT)
					{
						// Update the Remote Nodes Temp value
						NodeTemp[i+1].TempValue = rxMessage.Payload[1];
    					NodeTemp[i+1].NodeAddress[0] = rxMessage.Payload[2];
    					NodeTemp[i+1].NodeAddress[1] = rxMessage.Payload[3];
					}	
				}
			}
			
			MiApp_DiscardMessage();
        }
    }   
}


/*********************************************************************
* Function:         BYTE ReadTempSensor(WORD VBGResult)
*
* PreCondition:     Proper reference voltage value has been determined.
*
* Input:		    WORD VBGResult - Reference voltage for temp calculation.
*
* Output:		    BYTE temp
*
* Side Effects:	    none
*
* Overview:		    Following routine reads the on board Tempature Sensor and
*                   calculates the temp value. 
*
* Note:			    
**********************************************************************/
BYTE ReadTempSensor(WORD VBGResult)
{
	WORD tempValue;
	double temp;
	BYTE tempHere;
	BYTE i = 0;
    float tempAverage = 0;
    BYTE tempArray[NUM_TEMP_SAMPLES];
    
     
    // Configure the ADC register settings
    ADCON0 = 0x04;
    ADCON1 = 0xBD;
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 0;
/*
	ADCON0bits.ADON = 1;
	Delay10us(10);					// Wait Acquisition time
	
	ADCON0bits.GO = 1;	
	while(ADCON0bits.DONE);
 
 	tempValue = ADRES;
	ADCON0bits.ADON = 0;
	 	   
	temp = (1200.0/VBGResult);
	temp = (temp * tempValue);				
	temp = (temp - 500.0)/10.0;

    return (BYTE) temp;
*/	
   
    do
    {
    	ADCON0bits.ADON = 1;
    	Delay10us(10);					// Wait Acquisition time
    	
    	ADCON0bits.GO = 1;	
    	while(ADCON0bits.DONE);
        
    	temp = (1200.0/VBGResult);
    	tempValue = ADRES;
    	temp = (temp * tempValue);				
    	temp = (temp - 500.0)/10.0;
    	
    	tempArray[i] = (BYTE) temp;
    
	    ADCON0bits.ADON = 0;
	    Delay10us(1);
	    i++;
	} while(i < NUM_TEMP_SAMPLES);
	

    for(i = 0; i<NUM_TEMP_SAMPLES; i++)
    {
        tempAverage = (tempAverage + tempArray[i]);
    }
    tempAverage = (tempAverage/NUM_TEMP_SAMPLES);
    tempHere = (BYTE) tempAverage;
    tempAverage = (tempAverage - tempHere) * 10;
    
    if(tempAverage >= 5)
        tempHere = tempHere + 1;
        
    return (BYTE)tempHere;    


}

    				
/*********************************************************************
* Function:         WORD Read_VBGVoltage(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    ADRES
*
* Side Effects:	    none
*
* Overview:		    Reads the band gap voltage and compares with reference voltage
*					to arrive at the current voltage level
*
* Note:			    
**********************************************************************/
WORD Read_VBGVoltage(void)
{
    ADCON0 = 0x3D;				// Configures the channel as VBG
    ADCON1 = 0xBD;				// Program the acquisition time
    ANCON1bits.VBGEN = 1;		// Enable Band gap reference voltage
    
    Delay10us(1000);			//Wait for the Band Gap Settling time
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 0;			//Disable ADC interrupts
    							//This routine uses the polling based mechanism
    ADCON0bits.GO = 1;		    //Start A/D conversion    
    while(ADCON0bits.DONE);
    
    ADCON0bits.ADON = 0;	    // Turn ADC OFF
    ANCON1bits.VBGEN = 0;	    // Disable Bandgap
    
    return ADRES;
}


/*********************************************************************
* Function:         void PrintTempLCD(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Converts the Node Temp reading to Feirenhiet and
*					display's it on the LCD.
*
* Note:			    
**********************************************************************/
void PrintTempLCD(void)
{
	int temp;
	BYTE tempF;
	
    LCDErase();
    
    if(CurrentNodeIndex == 0)
	{
		sprintf((char *)LCDText, (far rom char*)"   Local Temp   ");
	}
    else
	{
		sprintf((char *)LCDText, (far rom char*)"  Remote Temp  ");
	}
	
    sprintf((char *)&LCDText[16], (far rom char*)"%02x%02x:",NodeTemp[CurrentNodeIndex].NodeAddress[1],NodeTemp[CurrentNodeIndex].NodeAddress[0]);
    
    if(NodeTemp[CurrentNodeIndex].TempValue == 255)
	{
		sprintf((char*)&LCDText[23], (far rom char*) " - ");
	}
    else
	{
    	sprintf((char *)&LCDText[21], (far rom char*)" %d", NodeTemp[CurrentNodeIndex].TempValue);
    	sprintf((char *)&LCDText[24], (far rom char*)"C"); 	

		temp = NodeTemp[CurrentNodeIndex].TempValue;
		temp = temp * 9;
		temp = temp / 5;
		temp = temp + 32;
		tempF = (BYTE) temp;
    	sprintf((char *)&LCDText[25], (far rom char*)"/ %d", tempF);
    	sprintf((char *)&LCDText[29], (far rom char*)"F"); 				
 	}
    
    LCDUpdate();
}
