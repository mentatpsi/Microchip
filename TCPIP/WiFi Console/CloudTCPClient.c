/*********************************************************************
 *
 *  Cloud TCP Client Example Application
 *  Module for Microchip TCP/IP Stack
 *   -Implements an example HTTP client and should be used as a basis 
 *	  for creating new TCP client applications
 *	 -Reference: None.  Hopefully AN833 in the future.
 *
 *********************************************************************
 * FileName:        CloudTCPClient.c
 * Dependencies:    TCP, DNS, ARP, Tick
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2009 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Nohee Ko     11/28/12	Original
 ********************************************************************/
#include "TCPIPConfig.h"

#if defined(STACK_USE_CLOUD_TCPCLIENT)

#include "TCPIP Stack/TCPIP.h"
static BYTE ServerName[] =	""; /* Please refer to the release note */

/*
* Open WiFi Cloud™ aims to demo how to access and control the embedded devices
* powered by Microchip Wi-Fi modules via Amazon Cloud (AWS EC2) server.
* This demo particularly targets MRF24WG0MA/MRF24GW0MB IEEE802.11b/g modules.
* The functionality is basically access I/Os of Explorer16 evalution board.
* Those I/Os are specifically :
* A) read temperature 
* B) read potentiometer
* C) control LEDs on/off
*/

/* 
* A deficit : Currently LED7 is not properly controlled because that is multiplexed with Button 1 on Explorerer16.
* And this problem will be addressed in next release.
*/

// Defines the port to be accessed for this application
#if defined(STACK_USE_SSL_CLIENT)
    static WORD ServerPort = HTTPS_PORT;
	// Note that if HTTPS is used, the ServerName and URL 
	// must change to an SSL enabled server.
#else
    static WORD ServerPort = HTTP_PORT;
#endif

// Defines the URL to be requested by this HTTP client
static ROM BYTE RemoteURL[] = "/cgi-bin/dev_update";
static ROM BYTE MyDeviceID[] = "101350029";	/* Use your device ID. The ID is used to distinguish your board from others.
											* For the ID, you may use the serial# of the Explorer16 shown on the bottom. 
											* Give a numeric part of the serial#. For instance, input only 101350029 
											* if the serial# is BUR101350029. */


#define AN_VOLT_PIN  	AD1PCFGbits.PCFG5			//voltage input on AN5
#define ADC_VOLT_CHAN	0x00050005                  // for MCU32 devices higher word (0x50000) is active and for mcu16 lower word(0x5)
#define AN_TEMP_PIN	 	AD1PCFGbits.PCFG4			//temp input on AN4
#define ADC_TEMP_CHAN	0x00040004                  // for MCU32 devices higher word (0x40000) is active and for mcu16 lower word(0x4)
#define ADC_TEMPERATURE  ADC_TEMP_CHAN
#define ADC_VOLTAGE      ADC_VOLT_CHAN

static WORD temperature;

static int
FindTag(char *tag, char *xml, WORD xmlLen)
{
	int tagLen = strlen(tag);
	int i;

	for (i = 0; i < xmlLen - tagLen + 1; i++) {
		if (!memcmp(tag, xml + i, tagLen))
			return i + tagLen;
	}
	
	return -1;
}

static char *
GetElement(char *xml, WORD xmlLen)
{
	static char result[64];
	int last;

	for (last = 0; last < xmlLen; last++) {
		if (xml[last] == '<')
			break;
	}

	if (last > 0 && last <= sizeof(result) - 1) {
		memcpy(result, xml, last);
		xml[last] = '\0';
		return result;
	} 

	return NULL;
}

static char *
SimpleXmlParser(char *tags[], char *xml, WORD xmlLen) {
	int tagOffset;

	tagOffset = FindTag(tags[0], xml, xmlLen);
	if (tagOffset < 0)
		return NULL;
	
	if (tags[1] == NULL)
		return GetElement(xml + tagOffset, xmlLen - tagOffset);

	return SimpleXmlParser(tags + 1, xml + tagOffset, xmlLen - tagOffset);
}

static BYTE ParseLEDControl(BYTE *buffer, WORD length, BYTE index)
{
	BYTE on = 2;
	char *tags[3];
	char *element;
	char ledCtrlTag[16] = "<led0_c>";

	tags[0] = "<ledcontrol>";
	ledCtrlTag[4] += index;
	tags[1] = ledCtrlTag;
	tags[2] = NULL;
	
	element = SimpleXmlParser(tags, (char *)buffer, length);

	if (element) {
		if(strncmp((char*)element, " on", 3) == 0)
			on = 1;
		else if (strncmp((char*)element, " off", 4) == 0)
			on = 0;
	}
	
	return on;
}

void GetTemperature(void)
{
	unsigned int result;
	static unsigned int ADCTemperature = 0;
	static int count = 0;

	if (count++ % 64)
		return;
	
	AD1CON1bits.DONE=0;

	/* intentional delay for analog channel switching */
	DelayMs(1);

	AD1CON2 = 0;
	AD1CON3 = 0x8F05;	
	AD1PCFG = 0; //analog pin
	AD1CSSL = 0; //no scanning
	TRISBbits.TRISB4 = 1;   //input pin
	TRISBbits.TRISB5 = 1;   //input pin
	AD1CON1 = 0x00E4;
    AD1CHS = ADC_TEMPERATURE;
	AD1CON1bits.ADON = 1;

	/* intentional delay for analog channel switching */
	
	DelayMs(1);
    
    while(!AD1CON1bits.DONE);

	result = (unsigned int)ADC1BUF0;
	//filter temperature Value
    //ADCTemperature = 15/16*ADCTemperature + 1/16*New Sample   
    ADCTemperature -= ADCTemperature>>4;
    ADCTemperature += result;
    result = ADCTemperature>>4;

	temperature = result / 10;
}

 unsigned int GetPotentiometer(void)
{
	AD1CON1bits.DONE=0;

	/* intentional delay for analog channel switching */
	DelayMs(1);

	AD1CON2 = 0x404;
	AD1CON3 = 0x1003;	
	AD1PCFG = 0; //analog pin
	AD1CSSL = 4;
	TRISBbits.TRISB4 = 1;   //input pin
	TRISBbits.TRISB5 = 1;   //input pin
	AD1CON1 = 0x84e5;
    AD1CHS = ADC_VOLTAGE;
	AD1CON1bits.ADON = 1;

	/* intentional delay for analog channel switching */
	DelayMs(1);
    
    while(!AD1CON1bits.DONE);
     
	return ADC1BUF0;
}

static void PackStatus(BYTE *buf)
{
	WORD potentiometer = GetPotentiometer();
	
	sprintf((char *)buf, "device+id=%s&temp_sensor=%d&analog_pot=%d&update+led0+status=%s&update+led1+status=%s&update+led2+status=%s&update+led3+status=%s&update+led4+status=%s&update+led5+status=%s&update+led6+status=%s&update+led7+status=%s",
		MyDeviceID,
		temperature,
		potentiometer,
		LED0_IO ? "on" : "off", 
		LED1_IO ? "on" : "off",
		LED2_IO ? "on" : "off",
		LED3_IO ? "on" : "off",
		LED4_IO ? "on" : "off",
		LED5_IO ? "on" : "off",
		LED6_IO ? "on" : "off",
		LED7_IO ? "on" : "off");

	#if defined(STACK_USE_UART)
	//putsUART(buf);
	//putsUART("\n\r");
	#endif
}

/*****************************************************************************
  Function:
	void CloudTCPClient(void)

  Summary:
	Implements a simple HTTP client (over TCP).

  Description:
	This function implements a simple Cloud client, which operates over TCP.  
	The function is called periodically by the stack, and waits for BUTTON1 
	to be pressed.  When the button is pressed, the application opens a TCP
	connection to a Cloud Server, performs LED/Temperature/Potentiomenter
	status report and LED control.

  Precondition:
	TCP is initialized.

  Parameters:
	None

  Returns:
  	None
  ***************************************************************************/
void CloudTCPClient(void)
{
	BYTE 				i;
	WORD				w;
	BYTE				vBuffer[9];
	static DWORD		Timer;
	static TCP_SOCKET	MySocket = INVALID_SOCKET;
	static BYTE ContentLength[64];
	static BOOL DnsResolved = FALSE;
	static DWORD dwRemoteHost = 0;
	static BYTE RxBuffer[1024];
	static BYTE TxBuffer[1024];
	static WORD RxIndex = 0;
	static DWORD IterTimer = 0;
	static enum _GenericTCPExampleState
	{
		SM_HOME = 0,
		SM_DNS_RESOLVE_START,
		SM_DNS_GET_RESULT,
		SM_CONNECT,
		SM_SOCKET_OBTAINED,
		#if defined(STACK_USE_SSL_CLIENT)
    	SM_START_SSL,
    	#endif
		SM_PROCESS_RESPONSE,
		SM_DISCONNECT,
		SM_DONE
	} CloudTCPClientState = SM_DONE;

	GetTemperature();
 	
	switch(CloudTCPClientState)
	{
		case SM_HOME:
			CloudTCPClientState = SM_DNS_RESOLVE_START;
			DnsResolved = FALSE;
			break;
		 case SM_DNS_RESOLVE_START:
	    	if(DNSBeginUsage())
	    	{
		    	DNSResolveROM(ServerName, DNS_TYPE_A);
		    	CloudTCPClientState = SM_DNS_GET_RESULT;
		    }
	    	break;
	    case SM_DNS_GET_RESULT:
	    	if(!DNSIsResolved((IP_ADDR*)&dwRemoteHost))
	    		break;

	    	if(!DNSEndUsage())
	    	{
				#if defined(STACK_USE_UART)
				putrsUART((ROM char*)"Could not resolve ServerName[] to IP address.\r\n");
				#endif
		    	CloudTCPClientState = SM_DONE;
		    	break;
		    }

			DnsResolved = TRUE;
	    	CloudTCPClientState = SM_CONNECT;		
			break;
		case SM_CONNECT:
			// Connect a socket to the remote TCP server
			MySocket = TCPOpen((DWORD)(PTR_BASE)dwRemoteHost, TCP_OPEN_IP_ADDRESS, ServerPort, TCP_PURPOSE_CLOUD_TCP_CLIENT);
			
			// Abort operation if no TCP socket of type TCP_PURPOSE_CLOUD_TCP_CLIENT is available
			// If this ever happens, you need to go add one to TCPIPConfig.h
			if(MySocket == INVALID_SOCKET)
				break;

			#if defined(STACK_USE_UART)
			putrsUART((ROM char*)"\r\n\r\nConnecting using Microchip TCP API...\r\n");
			#endif

			CloudTCPClientState++;
			IterTimer = Timer = TickGet();
			break;

		case SM_SOCKET_OBTAINED:
			// Wait for the remote server to accept our connection request
			if(!TCPIsConnected(MySocket))
			{
				// Time out if too much time is spent in this state
				if(TickGet()-Timer > 5*TICK_SECOND)
				{
					// Close the socket so it can be used by other modules
					TCPDisconnect(MySocket);
					MySocket = INVALID_SOCKET;
					CloudTCPClientState--;
				}
				break;
			}

			Timer = TickGet();
			
    #if defined (STACK_USE_SSL_CLIENT)
            if(!TCPStartSSLClient(MySocket,(void *)"thishost"))
                break;
			CloudTCPClientState++;
			break;

        case SM_START_SSL:
            if (TCPSSLIsHandshaking(MySocket)) 
            {
				if(TickGet()-Timer > 10*TICK_SECOND)
				{
					// Close the socket so it can be used by other modules
					TCPDisconnect(MySocket);
					MySocket = INVALID_SOCKET;
					CloudTCPClientState=SM_HOME;
				}
                break;
            }
    #endif

			// Make certain the socket can be written to
			if(TCPIsPutReady(MySocket) < 125u)
				break;

			PackStatus(TxBuffer);
			
			// Place the application protocol data into the transmit buffer.  For this example, we are connected to an HTTP server, so we'll send an HTTP GET request.
			TCPPutROMString(MySocket, (ROM BYTE*)"POST ");
			TCPPutROMString(MySocket, RemoteURL);
			TCPPutROMString(MySocket, (ROM BYTE*)" HTTP/1.1\r\nHost: ");
			TCPPutString(MySocket, ServerName);
			TCPPutROMString(MySocket, (ROM BYTE*)"\r\nContent-Type: application/x-www-form-urlencoded");
			sprintf((char *)ContentLength, "\r\nContent-Length: %d", (int)strlen((char *)TxBuffer));
			TCPPutROMString(MySocket, (ROM BYTE*)ContentLength);
			TCPPutROMString(MySocket, (ROM BYTE*)"\r\nConnection: close\r\n\r\n");
			TCPPutROMString(MySocket, (ROM BYTE*)TxBuffer);
	
			// Send the packet
			TCPFlush(MySocket);
			CloudTCPClientState++;
			Timer = TickGet();
			RxIndex = 0;
			break;

		case SM_PROCESS_RESPONSE:
			// Check to see if the remote node has disconnected from us or sent us any application data
			// If application data is available, write it to the UART
			if(!TCPIsConnected(MySocket)) {
				CloudTCPClientState = SM_DISCONNECT;
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			} else {
				if(TickGet()-Timer > 20 * TICK_SECOND) {
					CloudTCPClientState = SM_DISCONNECT;
					break;
				}
			}
	
			// Get count of RX bytes waiting
			w = TCPIsGetReady(MySocket);	
	
			// Obtian and print the server reply
			i = sizeof(vBuffer)-1;
			vBuffer[i] = '\0';
	
			while(w)
			{
				if(w < i)
				{
					i = w;
					vBuffer[i] = '\0';
				}
				w -= TCPGetArray(MySocket, vBuffer, i);

				memcpy(&RxBuffer[RxIndex], vBuffer, i);
				RxIndex += i;

				#if defined(STACK_USE_UART)
				//putsUART((char*)vBuffer);
				//putsUART("\n\r");
				#endif
				
				// putsUART is a blocking call which will slow down the rest of the stack 
				// if we shovel the whole TCP RX FIFO into the serial port all at once.  
				// Therefore, let's break out after only one chunk most of the time.  The 
				// only exception is when the remote node disconncets from us and we need to 
				// use up all the data before changing states.
				if(CloudTCPClientState == SM_PROCESS_RESPONSE)
					break;
			}	
			break;
		case SM_DISCONNECT:
			// Close the socket so it can be used by other modules
			// For this application, we wish to stay connected, but this state will still get entered if the remote server decides to disconnect
			TCPDisconnect(MySocket);
			MySocket = INVALID_SOCKET;
			CloudTCPClientState = SM_DONE;
			break;
		case SM_DONE:
			if (RxIndex) {
				BYTE display[32];
				WORD on = 0;
				WORD i;

				RxBuffer[RxIndex] = '\0';	
				#if defined(STACK_USE_UART)
				//putsUART(RxBuffer);
				//putsUART("\n\r");
				#endif
				for (i = 1; i < 8; i++) {
					on = ParseLEDControl((BYTE *)RxBuffer, RxIndex, i);
					if (on == 1 || on == 0) {
						if (on)
							sprintf((char *)display, "LED %d ON\n\r", i);
						else 
							sprintf((char *)display, "LED %d OFF\n\r", i);
						
						switch (i) {
						case 1: LED1_IO = on; break;
						case 2: LED2_IO = on; break;
						case 3: LED3_IO = on; break;
						case 4: LED4_IO = on; break;
						case 5: LED5_IO = on; break;
						case 6: LED6_IO = on; break;
						case 7: LED7_IO = on; break;
						default: break;
						}
					} else if (on == 2){ 
						sprintf((char *)display, "LED %d No Change\n\r", i);
					}
					#if defined(STACK_USE_UART)
					putsUART((char *)display);
					#endif
				}
			}
			RxIndex = 0;
			if(DnsResolved && TickGet() - IterTimer > 1 * TICK_SECOND) {
				CloudTCPClientState = SM_CONNECT;
			}
			// Do nothing unless the user pushes BUTTON1 and wants to restart the whole connection/download process
			if(BUTTON1_IO == 0u) {
				CloudTCPClientState = SM_HOME;
			}
			break;
	}
}

#endif	//#if defined(STACK_USE_CLOUD_TCPCLIENT)
