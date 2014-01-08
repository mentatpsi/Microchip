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
 * Processor:       PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.10b or higher
 *					Microchip C30 v3.21 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Microchip Demo Code for Google PowerMeter. Copyright 2010 Microchip
 * Technology Inc. and its licensors.  All rights reserved.
 *
 * Microchip licenses to you the right use, modify, copy, and distribute
 * the accompanying Microchip demo code only when used with or embedded on
 * a Microchip microcontroller or Microchip digital signal controller that
 * is integrated into your product or a third party product.  Any
 * redistributions of Microchip's demo code in compliance with the
 * foregoing must include a copy of this entire notice.
 *
 * THE MICROCHIP DEMO CODE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS
 * LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT
 * LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
 * THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT
 * LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, OR CONSEQUENTIAL DAMAGES,
 * LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS,
 * TECHNOLOGY, SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT
 * LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 * THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any
 * third party software accompanying this software - including but not
 * limited to Google's reference code - is subject to the terms and
 * conditions of license agreement of such third party, such as the Apache
 * License Version 2.0 (www.apache.org/licenses/).  To the extent required
 * by such third party licenses, the terms of the third party license will
 * apply in lieu of the terms provided herein.  To the extent the terms of
 * such third party licenses prohibit any of the restrictions described
 * herein, such restrictions will not apply to such third party software.
 * THIRD PARTY SOFTWARE  IS SUBJECT TO THE FOREGOING WARRANTY DISCLAIMER AND
 * LIMIT ON LIABILITY PROVIDED IN THE PARAGRAPH ABOVE.
 *
 ********************************************************************/
#define __CUSTOMHTTPAPP_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_HTTP2_SERVER)

#include "TCPIP Stack/TCPIP.h"
#include "MainDemo.h"		// Needed for SaveAppConfig() prototype

/****************************************************************************
  Section:
	Function Prototypes and Memory Globalizers
  ***************************************************************************/

void ulltoaRadix3 (long long value, BYTE bufferSize, BYTE * buffer);

// Sticky status message variable.
// This is used to indicated whether or not the previous POST operation was
// successful.  The application uses these to store status messages when a
// POST operation redirects.  This lets the application provide status messages
// after a redirect, when connection instance data has already been lost.
static BOOL lastSuccess = FALSE;

// Stick status message variable.  See lastSuccess for details.
static BOOL lastFailure = FALSE;

/****************************************************************************
  Section:
	Authorization Handlers
  ***************************************************************************/

/*****************************************************************************
  Function:
	BYTE HTTPNeedsAuth(BYTE* cFile)

  Internal:
  	See documentation in the TCP/IP Stack API or HTTP2.h for details.
  ***************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPNeedsAuth(BYTE* cFile)
{
	#if defined(HTTP_MPFS_UPLOAD_REQUIRES_AUTH)
	if(memcmppgm2ram(cFile, (ROM void*)"mpfsupload", 10) == 0)
		return 0x00;
	#endif

	// You can match additional strings here to password protect other files.
	// You could switch this and exclude files from authentication.
	// You could also always return 0x00 to require auth for all files.
	// You can return different values (0x00 to 0x79) to track "realms" for below.

	return 0x80;			// No authentication required
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
	if(strcmppgm2ram((char *)cUser,(ROM char *)"admin") == 0
		&& strcmppgm2ram((char *)cPass, (ROM char *)"microchip") == 0)
		return 0x80;		// We accept this combination
	
	// You can add additional user/pass combos here.
	// If you return specific "realm" values above, you can base this 
	//   decision on what specific file or folder is being accessed.
	// You could return different values (0x80 to 0xff) to indicate 
	//   various users or groups, and base future processing decisions
	//   in HTTPExecuteGet/Post or HTTPPrint callbacks on this value.
	
	return 0x00;			// Provided user/pass is invalid
}
#endif

/****************************************************************************
  Section:
	GET Form Handlers
  ***************************************************************************/
  
/*****************************************************************************
  Function:
	HTTP_IO_RESULT HTTPExecuteGet(void)
	
  Internal:
  	See documentation in the TCP/IP Stack API or HTTP2.h for details.
  ***************************************************************************/
HTTP_IO_RESULT HTTPExecuteGet(void)
{
	return HTTP_IO_DONE;
}


/****************************************************************************
  Section:
	POST Form Handlers
  ***************************************************************************/
#if defined(HTTP_USE_POST)

/*****************************************************************************
  Function:
	HTTP_IO_RESULT HTTPExecutePost(void)
	
  Internal:
  	See documentation in the TCP/IP Stack API or HTTP2.h for details.
  ***************************************************************************/
HTTP_IO_RESULT HTTPExecutePost(void)
{
	return HTTP_IO_DONE;
}



#endif //(use_post)


/****************************************************************************
  Section:
	Dynamic Variable Callback Functions
  ***************************************************************************/

/*****************************************************************************
  Function:
	void HTTPPrint_varname(void)
	
  Internal:
  	See documentation in the TCP/IP Stack API or HTTP2.h for details.
  ***************************************************************************/

extern APP_CONFIG AppConfig;

void HTTPPrintIP(IP_ADDR ip)
{
	BYTE digits[4];
	BYTE i;
	
	for(i = 0; i < 4u; i++)
	{
		if(i)
			TCPPut(sktHTTP, '.');
		uitoa(ip.v[i], digits);
		TCPPutString(sktHTTP, digits);
	}
}

void HTTPPrint_config_hostname(void)
{
	TCPPutString(sktHTTP, AppConfig.NetBIOSName);
	return;
}

void HTTPPrint_ddns_user(void)
{
	#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	if(DDNSClient.ROMPointers.Username || !DDNSClient.Username.szRAM)
		return;
	if(curHTTP.callbackPos == 0x00u)
		curHTTP.callbackPos = (PTR_BASE)DDNSClient.Username.szRAM;
	curHTTP.callbackPos = (PTR_BASE)TCPPutString(sktHTTP, (BYTE*)(PTR_BASE)curHTTP.callbackPos);
	if(*(BYTE*)(PTR_BASE)curHTTP.callbackPos == '\0')
		curHTTP.callbackPos = 0x00;
	#endif
}

void HTTPPrint_ddns_pass(void)
{
	#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	if(DDNSClient.ROMPointers.Password || !DDNSClient.Password.szRAM)
		return;
	if(curHTTP.callbackPos == 0x00u)
		curHTTP.callbackPos = (PTR_BASE)DDNSClient.Password.szRAM;
	curHTTP.callbackPos = (PTR_BASE)TCPPutString(sktHTTP, (BYTE*)(PTR_BASE)curHTTP.callbackPos);
	if(*(BYTE*)(PTR_BASE)curHTTP.callbackPos == '\0')
		curHTTP.callbackPos = 0x00;
	#endif
}

void HTTPPrint_ddns_host(void)
{
	#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	if(DDNSClient.ROMPointers.Host || !DDNSClient.Host.szRAM)
		return;
	if(curHTTP.callbackPos == 0x00u)
		curHTTP.callbackPos = (PTR_BASE)DDNSClient.Host.szRAM;
	curHTTP.callbackPos = (PTR_BASE)TCPPutString(sktHTTP, (BYTE*)(PTR_BASE)curHTTP.callbackPos);
	if(*(BYTE*)(PTR_BASE)curHTTP.callbackPos == '\0')
		curHTTP.callbackPos = 0x00;
	#endif
}

extern ROM char * ROM ddnsServiceHosts[];
void HTTPPrint_ddns_service(WORD i)
{
	#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	if(!DDNSClient.ROMPointers.UpdateServer || !DDNSClient.UpdateServer.szROM)
		return;
	if((ROM char*)DDNSClient.UpdateServer.szROM == ddnsServiceHosts[i])
		TCPPutROMString(sktHTTP, (ROM BYTE*)"selected");
	#endif
}


void HTTPPrint_ddns_status(void)
{
	#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	DDNS_STATUS s;
	s = DDNSGetLastStatus();
	if(s == DDNS_STATUS_GOOD || s == DDNS_STATUS_UNCHANGED || s == DDNS_STATUS_NOCHG)
		TCPPutROMString(sktHTTP, (ROM BYTE*)"ok");
	else if(s == DDNS_STATUS_UNKNOWN)
		TCPPutROMString(sktHTTP, (ROM BYTE*)"unk");
	else
		TCPPutROMString(sktHTTP, (ROM BYTE*)"fail");
	#else
	TCPPutROMString(sktHTTP, (ROM BYTE*)"fail");
	#endif
}

void HTTPPrint_ddns_status_msg(void)
{
	if(TCPIsPutReady(sktHTTP) < 75u)
	{
		curHTTP.callbackPos = 0x01;
		return;
	}

	#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	switch(DDNSGetLastStatus())
	{
		case DDNS_STATUS_GOOD:
		case DDNS_STATUS_NOCHG:
			TCPPutROMString(sktHTTP, (ROM BYTE*)"The last update was successful.");
			break;
		case DDNS_STATUS_UNCHANGED:
			TCPPutROMString(sktHTTP, (ROM BYTE*)"The IP has not changed since the last update.");
			break;
		case DDNS_STATUS_UPDATE_ERROR:
		case DDNS_STATUS_CHECKIP_ERROR:
			TCPPutROMString(sktHTTP, (ROM BYTE*)"Could not communicate with DDNS server.");
			break;
		case DDNS_STATUS_INVALID:
			TCPPutROMString(sktHTTP, (ROM BYTE*)"The current configuration is not valid.");
			break;
		case DDNS_STATUS_UNKNOWN:
			TCPPutROMString(sktHTTP, (ROM BYTE*)"The Dynamic DNS client is pending an update.");
			break;
		default:
			TCPPutROMString(sktHTTP, (ROM BYTE*)"An error occurred during the update.<br />The DDNS Client is suspended.");
			break;
	}
	#else
	TCPPutROMString(sktHTTP, (ROM BYTE*)"The Dynamic DNS Client is not enabled.");
	#endif

	curHTTP.callbackPos = 0x00;
}

void HTTPPrint_status_ok(void)
{
	if(lastSuccess)
		TCPPutROMString(sktHTTP, (ROM BYTE*)"block");
	else
		TCPPutROMString(sktHTTP, (ROM BYTE*)"none");
	lastSuccess = FALSE;
}

void HTTPPrint_status_fail(void)
{
	if(lastFailure)
		TCPPutROMString(sktHTTP, (ROM BYTE*)"block");
	else
		TCPPutROMString(sktHTTP, (ROM BYTE*)"none");
	lastFailure = FALSE;
}

void HTTPPrint_cumulativePower(void)
{
    WORD len;
    WORD len2;
    BYTE buffer[30];
    INT64 value;

    value = (gCumulativePowerInSamplingPeriod / 10000) + gCumulativePower;

    ulltoaRadix3 (value, 30, buffer);

    len = TCPIsPutReady (sktHTTP);
    len2 = strlen ((char *)buffer);

    if (curHTTP.callbackPos == 0)
        curHTTP.callbackPos = len2;

    while (len && curHTTP.callbackPos)
    {
        len -= TCPPut(sktHTTP, *(buffer + (len2 - curHTTP.callbackPos)));
        curHTTP.callbackPos--;
    }
	return;
}


void ulltoaRadix3 (long long value, BYTE bufferSize, BYTE * buffer)
{
    INT64 integer;
    INT32 fraction;
    char sign = '+';
    
    if (bufferSize == 0)
        return;

    if (value < 0)
    {
        sign = '-';
        value *= -1;
    }

    integer = value / 1000;
    fraction = value % 1000;

    if (bufferSize <= 28)
    {
        *buffer = 0;
    }
    else
    {
        sprintf((char *) buffer, "%c%lld.%03ld", sign, integer, fraction);
    }
}

void HTTPPrint_instPower(void)
{
    WORD len;
    WORD len2;
    BYTE buffer[20];
    DWORD decimal;
    DWORD value;

    value = gInstantaneousPower / 10000;
    decimal = gInstantaneousPower - (value * 10000);

    ultoa (value, buffer);
    len2 = strlen ((char *)buffer);
    buffer[len2++] = '.';
    ultoa (decimal, buffer + len2);

    len = TCPIsPutReady (sktHTTP);
    len2 = strlen ((char *)buffer);

    if (curHTTP.callbackPos == 0)
        curHTTP.callbackPos = len2;

    while (len && curHTTP.callbackPos)
    {
        len -= TCPPut(sktHTTP, *(buffer + (len2 - curHTTP.callbackPos)));
        curHTTP.callbackPos--;
    }
	return;
}

void HTTPPrint_apparentPower(void)
{
    WORD len;
    WORD len2;
    BYTE buffer[20];
    DWORD decimal;
    DWORD value;

    value = gApparentPower / 10000;
    decimal = gApparentPower - (value * 10000);

    ultoa (value, buffer);
    len2 = strlen ((char *)buffer);
    buffer[len2++] = '.';
    ultoa (decimal, buffer + len2);

    len = TCPIsPutReady (sktHTTP);
    len2 = strlen ((char *)buffer);

    if (curHTTP.callbackPos == 0)
        curHTTP.callbackPos = len2;

    while (len && curHTTP.callbackPos)
    {
        len -= TCPPut(sktHTTP, *(buffer + (len2 - curHTTP.callbackPos)));
        curHTTP.callbackPos--;
    }
	return;
}

void HTTPPrint_reactivePower(void)
{
    WORD len;
    WORD len2;
    BYTE buffer[20];
    DWORD decimal;
    DWORD value;

    value = gReactivePower / 10000;
    decimal = gReactivePower - (value * 10000);

    ultoa (value, buffer);
    len2 = strlen ((char *)buffer);
    buffer[len2++] = '.';
    ultoa (decimal, buffer + len2);

    len = TCPIsPutReady (sktHTTP);
    len2 = strlen ((char *)buffer);

    if (curHTTP.callbackPos == 0)
        curHTTP.callbackPos = len2;

    while (len && curHTTP.callbackPos)
    {
        len -= TCPPut(sktHTTP, *(buffer + (len2 - curHTTP.callbackPos)));
        curHTTP.callbackPos--;
    }
	return;
}

void HTTPPrint_volts(void)
{
    WORD len;
    WORD len2;
    BYTE buffer[20];
    WORD decimal;
    WORD value;

    value = gVoltsRMS / 10;
    decimal = gVoltsRMS - (value * 10);

    ultoa (value, buffer);
    len2 = strlen ((char *)buffer);
    buffer[len2++] = '.';
    uitoa (decimal, buffer + len2);

    len = TCPIsPutReady (sktHTTP);
    len2 = strlen ((char *)buffer);

    if (curHTTP.callbackPos == 0)
        curHTTP.callbackPos = len2;

    while (len && curHTTP.callbackPos)
    {
        len -= TCPPut(sktHTTP, *(buffer + (len2 - curHTTP.callbackPos)));
        curHTTP.callbackPos--;
    }
	return;
}

void HTTPPrint_amps(void)
{
    WORD len;
    WORD len2;
    BYTE buffer[20];
    WORD decimal;
    WORD value;

    value = gAmpsRMS / 100;
    decimal = gAmpsRMS - (value * 100);

    ultoa (value, buffer);
    len2 = strlen ((char *)buffer);
    buffer[len2++] = '.';
    uitoa (decimal, buffer + len2);

    len = TCPIsPutReady (sktHTTP);
    len2 = strlen ((char *)buffer);

    if (curHTTP.callbackPos == 0)
        curHTTP.callbackPos = len2;

    while (len && curHTTP.callbackPos)
    {
        len -= TCPPut(sktHTTP, *(buffer + (len2 - curHTTP.callbackPos)));
        curHTTP.callbackPos--;
    }
	return;
}

#endif
