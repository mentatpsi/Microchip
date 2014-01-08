/*********************************************************************
 *
 *  Application to Demo HTTP2 Server
 *  Support for HTTP2 module in Microchip TCP/IP Stack
 *	 -Implements the application 
 *	 -Reference: RFC 1002
 *
 *********************************************************************
 * FileName:        CustomHTTPApp.c
 * Dependencies:    TCP/IP stack
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2010 Microchip Technology Inc.  All rights
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
 * Elliott Wood     	6/18/07	Original
 ********************************************************************/
#define __CUSTOMHTTPAPP_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_HTTP2_SERVER)

#include "TCPIP Stack/TCPIP.h"

extern HTTP_CONN curHTTP;
extern HTTP_STUB httpStubs[MAX_HTTP_CONNECTIONS];
extern BYTE curHTTPID;

// Access to MainDemo.c functions and variables
#include "MainDemo.h"


// Vending Machine Application Global Variables
#include "VendingMachine.h"
extern VEND_ITEM Products[MAX_PRODUCTS];	// All Products in the machine
extern BYTE machineDesc[33];				// Machine descript string


/*****************************************************************************
  Function:
	BYTE HTTPNeedsAuth(BYTE* cFile)
	
  Internal:
  	See documentation in the TCP/IP Stack API or HTTP2.h for details.
  ***************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPNeedsAuth(BYTE* cFile)
{
	// No authentication is defined yet.
	
	return 0x80;
}
#endif

/*****************************************************************************
  Function:
	BYTE HTTPCheckAuth(BYTE* cUser, BYTE* cPass)
	
  Internal:
  	See documentation in the TCP/IP Stack API or HTTP2.h for details.
  ***************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPCheckAuth(BYTE* cUser, BYTE* cPass)
{
	// No authentication is defined yet.
	
	return 0x80;
}
#endif

/*********************************************************************
 * Function:        HTTP_IO_RESULT HTTPExecuteGet(void)
 *
 * PreCondition:    curHTTP is loaded
 *
 * Input:           None
 *
 * Output:          HTTP_IO_DONE on success
 *					HTTP_IO_WAITING if waiting for asynchronous process
 *
 * Side Effects:    None
 *
 * Overview:        This function is called if data was read from the
 *					HTTP request from either the GET arguments, or
 *					any cookies sent.  curHTTP.data contains
 *					sequential pairs of strings representing the
 *					data received.  Any required authentication has 
 *					already been validated.
 *
 * Note:            Custom implementation for WebVend Application
 ********************************************************************/
HTTP_IO_RESULT HTTPExecuteGet(void)
{
	BYTE *ptr, name[20];
	
	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, name, 20);
	
	// Make sure it's the machine.htm page
	if(strcmppgm2ram((char*)name, (ROM char*)"lights.htm") != 0)
		return HTTP_IO_DONE;
	
	// Find the new light state value
	ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"lights");
	if(ptr)	// Make sure ptr is not NULL
	{// Set the new lights state
		if(strcmppgm2ram((char*)ptr, (ROM char*)"on") == 0)
		{// Set lights on
			VendSetLights(TRUE);
		}
		else
		{// Set lights off
			VendSetLights(FALSE);
		}
	}	

	// Indicate that we're finished
	return HTTP_IO_DONE;
}

#if defined(HTTP_USE_POST)

/*********************************************************************
 * Function:        HTTP_IO_RESULT HTTPExecutePost(void)
 *
 * PreCondition:    curHTTP is loaded
 *
 * Input:           None
 *
 * Output:          HTTP_IO_DONE on success
 *					HTTP_IO_NEED_DATA if more data is requested
 *					HTTP_IO_WAITING if waiting for asynchronous process
 *
 * Side Effects:    None
 *
 * Overview:        This function is called if the request method was
 *					POST.  It is called after HTTPExecuteGet and 
 *					after any required authentication has been validated.
 *
 * Note:            Custom implementation for WebVend Application
 ********************************************************************/
HTTP_IO_RESULT HTTPExecutePost(void)
{
	BYTE name[20], item, *ptr;
	WORD len;
	
	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, name, 20);
	
	// Make sure it's the products.htm page
	if(strcmppgm2ram((char*)name, (ROM char*)"products.htm") != 0)
		return HTTP_IO_DONE;
		
	// Loop while data remains
	while(curHTTP.byteCount)
	{
		// Check for a complete variable
		len = TCPFind(sktHTTP, '&', 0, FALSE);
		if(len == 0xffff)
		{// Check if this is the last one
			if(TCPIsGetReady(sktHTTP) == curHTTP.byteCount)
				len = curHTTP.byteCount - 1;
			else // Wait for more data
				return HTTP_IO_NEED_DATA;
		}
		
		// Make sure we don't overflow
		if(len > HTTP_MAX_DATA_LEN-2)
		{
			curHTTP.byteCount -= TCPGetArray(sktHTTP, NULL, len+1);
			continue;
		}

		// Read the next variable and parse
		len = TCPGetArray(sktHTTP, curHTTP.data, len+1);
		curHTTP.data[len] = '\0';
		HTTPURLDecode(curHTTP.data);
		curHTTP.byteCount -= len;
		
		// Figure out which variable it is
		if(memcmppgm2ram(curHTTP.data, (ROM void*)"name", 4) == 0)
		{// A name was found
			item = curHTTP.data[5] - '0';
			if(item > MAX_PRODUCTS)
				continue;
			memcpy((void*)Products[item].name, (void*)&curHTTP.data[8], 10);
		}
		else if(memcmppgm2ram(curHTTP.data, (ROM void*)"price", 5) == 0)
		{// A price was found
			item = curHTTP.data[6] - '0';
			if(item > MAX_PRODUCTS)
				continue;
			ptr = curHTTP.data + 9;

			// Skip the $ if entered
			if(*ptr == '$')
				ptr++;
				
			// Read the dollars value
			Products[item].price = (*ptr++ - '0') * 4;
			
			// Read in the cents value
			if(strcmppgm2ram((char*)ptr, (ROM char*)".87") > 0)
				Products[item].price += 4;
			else if(strcmppgm2ram((char*)ptr, (ROM char*)".62") > 0)
				Products[item].price += 3;
			else if(strcmppgm2ram((char*)ptr, (ROM char*)".37") > 0)
				Products[item].price += 2;
			else if(strcmppgm2ram((char*)ptr, (ROM char*)".12") > 0)
				Products[item].price += 1;
				
			// Make sure price isn't over the max
			if(Products[item].price > 20)
				Products[item].price = 20;
		}
		
	}

	// Update the LCD and AppConfig
	WriteLCDMenu();
	SaveAppConfig(&AppConfig);

	return HTTP_IO_DONE;
}

#endif //(use_post)

/*********************************************************************
 * Function:        void HTTPPrint_varname(TCP_SOCKET sktHTTP, 
 *							DWORD callbackPos, BYTE *data)
 *
 * PreCondition:    None
 *
 * Input:           sktHTTP: the TCP socket to which to write
 *					callbackPos: 0 initially
 *						return value of last call for subsequent callbacks
 *					data: this connection's data buffer
 *
 * Output:          0 if output is complete
 *					application-defined otherwise
 *
 * Side Effects:    None
 *
 * Overview:        Outputs a variable to the HTTP client.
 *
 * Note:            Return zero to indicate that this callback function 
 *					has finished writing data to the TCP socket.  A 
 *					non-zero return value indicates that more data 
 *					remains to be written, and this callback should 
 *					be called again when more space is available in 
 *					the TCP TX FIFO.  This non-zero return value will 
 *					be the value of the parameter callbackPos for the 
 *					next call.
 ********************************************************************/

void HTTPPrint_version(void)
{
	TCPPutROMString(sktHTTP,(ROM void*)TCPIP_STACK_VERSION);
	return;
}

void HTTPPrint_builddate(void)
{
	TCPPutROMString(sktHTTP,(ROM void*)__DATE__" "__TIME__);
	return;
}

void HTTPPrint_hostname(void)
{
	TCPPutString(sktHTTP, AppConfig.NetBIOSName);
	return;
}

void HTTPPrint_machineDesc(void)
{
	// Make sure enough space exists
	if(strlen((char*)machineDesc) > TCPIsPutReady(sktHTTP))
	{
		curHTTP.callbackPos = 0x01;
		return;
	}
	
	// Write the string and make sure we continue
	TCPPutString(sktHTTP, machineDesc);
	curHTTP.callbackPos = 0x00;
	return;
}

void HTTPPrint_name(WORD item)
{
	TCPPutString(sktHTTP, Products[item].name);
}

void HTTPPrint_stock(WORD item)
{
	BYTE digits[4];
	uitoa(Products[item].stock, digits);
	TCPPutString(sktHTTP, digits);
}

void HTTPPrint_status(WORD item)
{
	if(Products[item].stock < 10)
		TCPPutROMString(sktHTTP, (ROM BYTE*)"low");
	else
		TCPPutROMString(sktHTTP, (ROM BYTE*)"ok");
}

void HTTPPrint_lights_chk(WORD on)
{
	if(LED1_IO == on)
		TCPPutROMString(sktHTTP, (ROM BYTE*)"checked");
}

void HTTPPrint_price(WORD item)
{
	TCPPut(sktHTTP, '$');
	TCPPut(sktHTTP, (Products[item].price >> 2) + '0');
	switch(Products[item].price & 0x03)
	{
		case 0:
			TCPPutROMString(sktHTTP, (ROM BYTE*)".00");
			break;
		case 1:
			TCPPutROMString(sktHTTP, (ROM BYTE*)".25");
			break;
		case 2:
			TCPPutROMString(sktHTTP, (ROM BYTE*)".50");
			break;
		case 3:
			TCPPutROMString(sktHTTP, (ROM BYTE*)".75");
			break;
	}
}


#endif
