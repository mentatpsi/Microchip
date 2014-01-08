/*********************************************************************
 *
 *	Mobile TCP Server Example Application
 *  Module for Microchip TCP/IP Stack
 *   -Implements an example "ToUpper" TCP server on port 9760 and 
 *	  should be used as a basis for creating new TCP server 
 *    applications
 *
 *********************************************************************
 * FileName:        GenericTCPServer.c
 * Dependencies:    TCP
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
 * Author               Date    	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Nohee Ko     12/17/12	Original
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ********************************************************************/
#define __GENERICTCPSERVER_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_TCP_MOBILE_APP_SERVER)

#include "TCPIP Stack/TCPIP.h"

/*
* The mobile application is available for Android devices (version 4.0 or the later) in Google Play Store, 
* and iOS devices (iPhone/iPod-touch for version 6.0 or the later) in Apple App store. 
* For the app names, please refer to the release note. The functionality is basically access I/Os of
* Explorer16 evalution board. Those I/Os are specifically :
* A) read temperature 
* B) read potentiometer
* C) control LEDs on/off
*/

/* 
* A deficit : Currently LED7 is not properly controlled because that is multiplexed with Button 1 on Explorerer16.
* And this problem will be addressed in next release.
*/

static ROM BYTE MyDeviceID[] = "123456789";	// dummy just to be compatible with cloud client demo

// Defines which port the server will listen on
#define SERVER_PORT	27561

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

	tags[0] = "<response>";
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
	void MobileTCPServer(void)

  Summary:
	Implements a Mobile TCP server

  Description:
	This function implements a Mobile TCP server.  The function is invoked
	periodically by the stack to listen for incoming connections.  When a 
	connection is made, the server reads all incoming data, and controls the LEDs
	as the mobile device requests. And also the server responds to it with current
	status.

  Precondition:
	TCP is initialized.

  Parameters:
	None

  Returns:
  	None
  ***************************************************************************/
void MobileTCPServer(void)
{
	WORD w;
	BYTE AppBuffer[32];
	WORD wMaxGet, wMaxPut, wCurrentChunk;
	static TCP_SOCKET	MySocket;
	static BYTE RxBuffer[1024];
	static BYTE TxBuffer[1024];
	static WORD RxIndex = 0;
	static DWORD StartTime = 0;
	static enum _TCPServerState
	{
		SM_HOME = 0,
		SM_LISTENING,
        SM_CLOSING,
	} TCPServerState = SM_HOME;

	GetTemperature();

	switch(TCPServerState)
	{
		case SM_HOME:
			// Allocate a socket for this server to listen and accept connections on
			MySocket = TCPOpen(0, TCP_OPEN_SERVER, SERVER_PORT, TCP_PURPOSE_MOBILE_APP_SERVER);
			if(MySocket == INVALID_SOCKET)
				return;

			RxIndex = 0;
			TCPServerState = SM_LISTENING;
			break;

		case SM_LISTENING:
			// See if anyone is connected to us
			if(!TCPIsConnected(MySocket))
				return;

			if (StartTime == 0)
				StartTime = TickGet();

			// Figure out how many bytes have been received and how many we can transmit.
			wMaxGet = TCPIsGetReady(MySocket);	// Get TCP RX FIFO byte count
			wMaxPut = TCPIsPutReady(MySocket);	// Get TCP TX FIFO free space

			// Process all bytes that we can
			// This is implemented as a loop, processing up to sizeof(AppBuffer) bytes at a time.  
			// This limits memory usage while maximizing performance.  Single byte Gets and Puts are a lot slower than multibyte GetArrays and PutArrays.
			wCurrentChunk = sizeof(AppBuffer);
			for(w = 0; w < wMaxGet; w += sizeof(AppBuffer))
			{
				// Make sure the last chunk, which will likely be smaller than sizeof(AppBuffer), is treated correctly.
				if(w + sizeof(AppBuffer) > wMaxGet)
					wCurrentChunk = wMaxGet - w;

				// Transfer the data out of the TCP RX FIFO and into our local processing buffer.
				TCPGetArray(MySocket, AppBuffer, wCurrentChunk);
				memcpy(&RxBuffer[RxIndex], AppBuffer, wCurrentChunk);
				RxIndex += wCurrentChunk;
			}

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
				PackStatus(TxBuffer);
				TCPPutArray(MySocket, TxBuffer, strlen((char *)TxBuffer));
				TCPServerState = SM_CLOSING;
			}

			if(TickGet() - StartTime > 1*TICK_SECOND)
				TCPServerState = SM_CLOSING;
			// No need to perform any flush.  TCP data in TX FIFO will automatically transmit itself after it accumulates for a while.  If you want to decrease latency (at the expense of wasting network bandwidth on TCP overhead), perform and explicit flush via the TCPFlush() API.

			break;

		case SM_CLOSING:
			// Close the socket connection.
            TCPClose(MySocket);

			TCPServerState = SM_HOME;
			StartTime = 0;
			break;
	}
}

#endif //#if defined(STACK_USE_TCP_MOBILE_APP_SERVER)
