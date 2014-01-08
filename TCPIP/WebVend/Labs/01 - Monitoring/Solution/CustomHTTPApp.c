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
	// No GET functionality is defined

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
	// No POST functionality is defined
	
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

#endif
