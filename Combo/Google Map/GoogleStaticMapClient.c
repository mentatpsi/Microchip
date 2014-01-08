/*********************************************************************
 *
 *  Google Static Map v2 API client
 *  Module for Microchip TCP/IP Stack
 *   -Downloads compressed map images from the http://maps.google.com site
 *	 -Reference: Google Static Map v2 API 
 *	    http://code.google.com/apis/maps/documentation/staticmaps/index.html
 *
 *********************************************************************
 * FileName:        GoogleStaticMapClient.c
 * Dependencies:    TCP, DNS, ARP, Tick
 * Processor:       PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
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
 * Author               Date    	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder     03/10/2010	Original (started from GenericTCPClient.c)
 ********************************************************************/
#define GOOGLESTATICMAPCLIENT_C

#include "TCPIPConfig.h"
#include "TCPIP Stack/TCPIP.h"
#include "GoogleStaticMapClient.h"
#include "EDSMemCopy.h"
#include "JPEGImage.h"


// Defines the server to be accessed for this application
static ROM BYTE ServerName[] =	"maps.google.com";

// Defines the port to be accessed for this application
static WORD ServerPort = 80;

// Defines the URL to be requested by this HTTP client
static ROM BYTE RemoteURL[] = "/maps/api/staticmap?center=";//"33.304524,-111.882024&zoom=16&size=480x272&maptype=hybrid&format=jpg-baseline&sensor=false";

GOOGLE_STATIC_MAP_STATE MapDownloaderState;



// C30 compiler allows only up to 32767 bytes in a single array without using a 
// special -mlarge-arrays command line option.  To work around this, we must 
// allocate three smaller arrays that are linked at contiguous addresses.

#define DOWNLOAD_IMAGE_RAM_ALLOCATION	(67584ul)	// Sum of bytes allocated by vDownloadedImage[], vDownloadedImage1[], and vDownloadedImage2[]

#ifndef __PIC32MX__
static unsigned char __eds__ vDownloadedImage[22528]  __attribute__((section("DownloadedImageSection0"),space(eds),address(DOWNLOAD_IMAGE_BUFFER_ADDRESS),unused));
static unsigned char __eds__ vDownloadedImage1[22528] __attribute__((section("DownloadedImageSection1"),space(eds),address(DOWNLOAD_IMAGE_BUFFER_ADDRESS + 22528),unused));
static unsigned char __eds__ vDownloadedImage2[22528] __attribute__((section("DownloadedImageSection2"),space(eds),address(DOWNLOAD_IMAGE_BUFFER_ADDRESS + 22528 + 22528),unused));
#else
unsigned char vDownloadedImage[DOWNLOAD_IMAGE_RAM_ALLOCATION];
#endif


/*****************************************************************************
  Function:
	void GoogleStaticMapReset(GOOGLE_STATIC_MAP_STATE *state)

  Summary:
	Resets the Google Static Map downloader client and aborts any in progress 
	downloads.  The client enters an idle state.

  Description:
	Resets the Google Static Map downloader client and aborts any in progress 
	downloads.  The client enters an idle state.

  Precondition:
	If the state parameter has not been used before, set state->hSocket equal 
	to INVALID_SOCKET.  This is necessary to avoid attemptting to disconnect a 
	random TCP socket handle.

  Parameters:
	state - Pointer to a GOOGLE_STATIC_MAP_STATE structure to be reset.

  Returns:
	None
  ***************************************************************************/
void GoogleStaticMapReset(GOOGLE_STATIC_MAP_STATE *state)
{
	// Close TCP socket to abort any in progress operation, if there is one
	if(state->hSocket != INVALID_SOCKET)
	{
		TCPDisconnect(state->hSocket);
		TCPDisconnect(state->hSocket);
		state->hSocket = INVALID_SOCKET;
	}

	// Set Map downloader to an idle state
	state->vReturnCode = 0x7F;
	state->smClient = GOOGLE_STATIC_MAP_DONE;
}


/*****************************************************************************
  Function:
	signed char GoogleStaticMapClient(GOOGLE_STATIC_MAP_STATE *state)

  Summary:
	Implements a simple HTTP client (over TCP).

  Description:
	This function implements a simple HTTP client, which operates over TCP.  
	
	This example can be used as a model for many TCP and HTTP client
	applications.

  Precondition:
	TCP is initialized.

  Parameters:
	state - Pointer to a GOOGLE_STATIC_MAP_STATE structure containing state 
			variables.  Upon first call, state->smClient must be set to 
			GOOGLE_STATIC_MAP_START.

  Returns:
  	* >0: Client working (attempting to download an image)
  	* =0: Download successfully completed.  Image data is in array.  
  		  Length of data is in state->dwImageLength.
  	* <0: Download failed (illegal data, service can't be reached, etc.
  ***************************************************************************/
signed char GoogleStaticMapClient(GOOGLE_STATIC_MAP_STATE *state)
{
	WORD w;
	BYTE vBuffer[64];

	switch(state->smClient)
	{
		case GOOGLE_STATIC_MAP_START:
			// Initialize return code variable
			state->vReturnCode = 1;
			
			// Connect a socket to the remote TCP server
			state->hSocket = TCPOpen((DWORD)&ServerName[0], TCP_OPEN_ROM_HOST, ServerPort, TCP_PURPOSE_GOOGLE_STATIC_MAP_CLIENT);
			
			// Abort operation if no TCP socket of type TCP_PURPOSE_GENERIC_TCP_CLIENT is available
			// If this ever happens, you need to go add one to TCPIPConfig.h
			if(state->hSocket == INVALID_SOCKET)
				return 1;

			state->smClient = GOOGLE_STATIC_MAP_SOCKET_OBTAINED;
			state->dwTimer = TickGet();
			return 2;

		case GOOGLE_STATIC_MAP_SOCKET_OBTAINED:
			// Wait for the remote server to accept our connection request
			if(!TCPIsConnected(state->hSocket))
			{
				// Time out if too much time is spent in this state
				if(TickGet() - state->dwTimer > 10*TICK_SECOND)
				{
					// Close the socket so it can be used by other modules
					TCPDisconnect(state->hSocket);
					state->hSocket = INVALID_SOCKET;
					state->smClient = GOOGLE_STATIC_MAP_DONE;
					state->vReturnCode = -2;
				}
				return 3;
			}

			state->dwTimer = TickGet();

			// Make certain the socket can be written to
			if(TCPIsPutReady(state->hSocket) < 250u)
				return 4;
			
			// Place the application protocol data into the transmit buffer.  For this example, we are connected to an HTTP server, so we'll send an HTTP GET request.
			TCPPutROMString(state->hSocket, (ROM BYTE*)"GET ");
			TCPPutROMString(state->hSocket, RemoteURL);
			TCPPutString(   state->hSocket, state->szCenter);
			TCPPutROMString(state->hSocket, (ROM BYTE*)"&zoom=");
			uitoa(state->vZoom, vBuffer);
			TCPPutString(   state->hSocket, vBuffer);
			TCPPutROMString(state->hSocket, (ROM BYTE*)"&size=");
			uitoa(state->vSizeX, vBuffer);
			TCPPutString(   state->hSocket, vBuffer);
			TCPPut(         state->hSocket, 'x');
			uitoa(state->vSizeY, vBuffer);
			TCPPutString(   state->hSocket, vBuffer);
			TCPPutROMString(state->hSocket, (ROM BYTE*)"&format=");
			switch(state->vFormat)
			{
				case GOOGLE_STATIC_MAP_PNG:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"png");
					break;
				case GOOGLE_STATIC_MAP_PNG32:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"png32");
					break;
				case GOOGLE_STATIC_MAP_GIF:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"gif");
					break;
				case GOOGLE_STATIC_MAP_JPG:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"jpg");
					break;
				default:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"jpg-baseline");
					break;
			}
			TCPPutROMString(state->hSocket, (ROM BYTE*)"&maptype=");
			switch(state->vMapType)
			{
				case GOOGLE_STATIC_MAP_SATELLITE:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"satellite");
					break;
				case GOOGLE_STATIC_MAP_HYBRID:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"hybrid");
					break;
				case GOOGLE_STATIC_MAP_TERRAIN:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"terrain");
					break;
				default:
					TCPPutROMString(state->hSocket, (ROM BYTE*)"roadmap");
					break;
			}
			TCPPutROMString(state->hSocket, (ROM BYTE*)"&mobile=");
			if(state->vMobile)
				TCPPutROMString(state->hSocket, (ROM BYTE*)"true");
			else
				TCPPutROMString(state->hSocket, (ROM BYTE*)"false");
			if(state->szLanguage)
			{
				TCPPutROMString(state->hSocket, (ROM BYTE*)"&language=");
				TCPPutString(state->hSocket, state->szLanguage);
			}
			TCPPutROMString(state->hSocket, (ROM BYTE*)"&sensor=");
			if(state->vSensor)
				TCPPutROMString(state->hSocket, (ROM BYTE*)"true");
			else
				TCPPutROMString(state->hSocket, (ROM BYTE*)"false");
			TCPPutROMString(state->hSocket, (ROM BYTE*)" HTTP/1.0\r\nHost: ");
			TCPPutROMString(state->hSocket, ServerName);
			TCPPutROMString(state->hSocket, (ROM BYTE*)"\r\nConnection: close\r\n\r\n");

			// Send the request immediately (Optional, but improves performance.  If we ignore this step, TCP will automatically flush after a timeout.)
			TCPFlush(state->hSocket);
			state->dwEDSWritePtr = DOWNLOAD_IMAGE_BUFFER_ADDRESS;
			state->dwImageLength = 0;
			state->smClient = GOOGLE_STATIC_MAP_PROCESS_RESPONSE_HTTP_CODE;
			return 5;

		case GOOGLE_STATIC_MAP_PROCESS_RESPONSE_HTTP_CODE:
			// Check to see if the remote node has disconnected from us
			if(!TCPIsConnected(state->hSocket))
			{
				state->smClient = GOOGLE_STATIC_MAP_DISCONNECT;
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}

			// Look for the new line character indicating that a full line of 
			// data is available for parsing.
			w = TCPFindEx(state->hSocket, '\n', 0, 0xFFFF, FALSE);
			if(w == 0xFFFFu)
			{
				// Time out if too much time is spent in this state
				if(TickGet() - state->dwTimer > 10*TICK_SECOND)
				{
					// Close the socket so it can be used by other modules
					TCPDisconnect(state->hSocket);
					state->hSocket = INVALID_SOCKET;
					state->smClient = GOOGLE_STATIC_MAP_DONE;
					state->vReturnCode = -2;
				}
				return 6;
			}
			
			// Response should be "HTTP/1.1 200...".  The 200 HTTP status code 
			// needs checking, which means at least 12 characters must be in the 
			// buffer before the new line character.
			if(w < 12u)
			{
				// Close the socket so it can be used by other modules
				TCPDisconnect(state->hSocket);
				state->hSocket = INVALID_SOCKET;
				state->smClient = GOOGLE_STATIC_MAP_DONE;
				state->vReturnCode = -2;
				return 7;
			}
			
			// See if this is a 2xx status code (okay)
			if(TCPPeek(state->hSocket, 9) != '2')
			{
				// Close the socket so it can be used by other modules
				TCPDisconnect(state->hSocket);
				state->hSocket = INVALID_SOCKET;
				state->smClient = GOOGLE_STATIC_MAP_DONE;
				state->vReturnCode = -2;
				return 8;
			}
			
			// First line with HTTP status code looks good, eat the line and 
			// move to the next with the HTTP headers.
			TCPGetArray(state->hSocket, NULL, w + 1);
			state->smClient = GOOGLE_STATIC_MAP_PROCESS_RESPONSE_HTTP_HEADERS;
			// No break needed

		case GOOGLE_STATIC_MAP_PROCESS_RESPONSE_HTTP_HEADERS:
			// Check to see if the remote node has disconnected from us
			if(!TCPIsConnected(state->hSocket))
			{
				state->smClient = GOOGLE_STATIC_MAP_DISCONNECT;
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}

			while(1)
			{
				// Look for the new line character indicating that a full line of 
				// data is available for parsing.
				w = TCPFindEx(state->hSocket, '\n', 0, 0xFFFF, FALSE);
				if(w == 0xFFFFu)
				{
					// Time out if too much time is spent in this state
					if(TickGet() - state->dwTimer > 10*TICK_SECOND)
					{
						// Close the socket so it can be used by other modules
						TCPDisconnect(state->hSocket);
						state->hSocket = INVALID_SOCKET;
						state->smClient = GOOGLE_STATIC_MAP_DONE;
						state->vReturnCode = -2;
					}
					return 9;
				}
	
				// If this is a line with only another carriage return + newline on 
				// it, we are done with the HTTP header.  Binary image data follows.
				if(w == 1u)
				{
					TCPGetArray(state->hSocket, NULL, 2);
					state->dwTimer = TickGet();
					state->smClient = GOOGLE_STATIC_MAP_PROCESS_RESPONSE_BODY;
					break;
				}
				
				// Finished processing this line, discard it
				TCPGetArray(state->hSocket, NULL, w + 1);
			}

			// No break needed

		case GOOGLE_STATIC_MAP_PROCESS_RESPONSE_BODY:
			// Check to see if the remote node has disconnected from us or sent us any application data
			// If application data is available, write it to the UART
			if(!TCPIsConnected(state->hSocket))
			{
				state->smClient = GOOGLE_STATIC_MAP_DISCONNECT;
				state->vReturnCode = 0;	// Successfully finished downloading image
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}
	
			// Get count of RX bytes waiting
			w = TCPIsGetReady(state->hSocket);
	
            /***
             * Since PIC32 does not have eds memory, do not double buffer the data
             ***/
            #ifndef __PIC32MX__
			{
				BYTE i;

				// Obtian and print the server reply
				i = sizeof(vBuffer);
				while(w)
				{
					// Don't attempt to read more than exists in the TCP FIFO
					if(w < i)
						i = w;
						
					// Update image length and abort if this data will exceed the 
					// amount of memory we have allocated to store the image
					state->dwImageLength += i;
					if(state->dwImageLength > DOWNLOAD_IMAGE_RAM_ALLOCATION)
					{
						// Close the socket so it can be used by other modules
						TCPDisconnect(state->hSocket);
						state->hSocket = INVALID_SOCKET;
						state->smClient = GOOGLE_STATIC_MAP_DONE;
						state->vReturnCode = -3;
						return -3;
					}
					
					// Obtain the data and store it in the vDownloadedImage array
					w -= TCPGetArray(state->hSocket, vBuffer, i);
					memcpyram2eds(state->dwEDSWritePtr, vBuffer, i);
					state->dwEDSWritePtr += i;
				}
			}
            #else
			{
				if(w)
				{
					state->dwImageLength += w;
					if(state->dwImageLength > DOWNLOAD_IMAGE_RAM_ALLOCATION)
					{
						// Close the socket so it can be used by other modules
						TCPDisconnect(state->hSocket);
						state->hSocket = INVALID_SOCKET;
						state->smClient = GOOGLE_STATIC_MAP_DONE;
						state->vReturnCode = -3;
						return -3;
					}
					
					// Obtain the data and store it in the vDownloadedImage array
					TCPGetArray(state->hSocket, (BYTE *)state->dwEDSWritePtr, w);
					state->dwEDSWritePtr += w;
				}
			}
            #endif
			return 8;
	
		case GOOGLE_STATIC_MAP_DISCONNECT:
			// Close the socket so it can be used by other modules
			// For this application, we wish to stay connected, but this state will still get entered if the remote server decides to disconnect
			TCPDisconnect(state->hSocket);
			state->hSocket = INVALID_SOCKET;
			state->smClient = GOOGLE_STATIC_MAP_DONE;
			return 9;
	
		case GOOGLE_STATIC_MAP_DONE:
			// All finished.  Return status code only.
			return state->vReturnCode;
	}
	
	return -1;	// This should never happen.  If it does, it indicates your state parameter isn't initialized correctly.
}



/*****************************************************************************
  Function:
	void GoogleStaticMapUserAction(USER_INPUT_REQUEST *UserState)

  Summary:
	Callback that gets fired anytime we need to display a new map

  Description:
	Callback that gets fired anytime we need to display a new map

  Precondition:
	None

  Parameters:
	UserState - Pointer to a USER_INPUT_REQUEST structure containing variables 
				that the user has selected for the map to be displayed.

  Returns:
	None
  ***************************************************************************/
void GoogleStaticMapUserAction(USER_INPUT_REQUEST *UserState)
{
	static unsigned char vGoogleMapCenterString[257];
	#if !defined(__PIC32MX__)
	#ifdef __DEBUG
    printf("New Item Flags: 0x%02hhX; Zoom: %hhd; Center: %s\r\n", UserState->newItemsFlag, UserState->vZoom, UserState->szCenter);
    #endif
    #endif
	// Abort immediately any in progress operation, if there is one
	if(MapDownloaderState.hSocket != INVALID_SOCKET)
	{
		TCPDisconnect(MapDownloaderState.hSocket);
		TCPDisconnect(MapDownloaderState.hSocket);
		MapDownloaderState.hSocket = INVALID_SOCKET;
	}

//	if(strlen((char*)UserState->szCenter) >= sizeof(vGoogleMapCenterString))
//		return;
//	strcpy((char*)vGoogleMapCenterString, (char*)UserState->szCenter);
	strncpy((char*)vGoogleMapCenterString, (char*)UserState->szCenter, sizeof(vGoogleMapCenterString) - 1);
	vGoogleMapCenterString[sizeof(vGoogleMapCenterString) - 1] = 0x00;	// Force null terminatation
	Replace(vGoogleMapCenterString, (BYTE*)"+", (BYTE*)"%2B", sizeof(vGoogleMapCenterString), FALSE);	// Convert pluses to %2B to URLize string
	Replace(vGoogleMapCenterString, (BYTE*)" ", (BYTE*)"+", sizeof(vGoogleMapCenterString), FALSE);		// Convert spaces to pluses to URLize string

	// Set up for a test map download	
	MapDownloaderState.szCenter = vGoogleMapCenterString;
	MapDownloaderState.vZoom = UserState->vZoom;
	MapDownloaderState.vSizeX = MAPAREA_X2 - MAPAREA_X1;
	MapDownloaderState.vSizeY = MAPAREA_Y2 - MAPAREA_Y1;
	MapDownloaderState.vFormat = GOOGLE_STATIC_MAP_JPG_BASELINE;
	MapDownloaderState.vMapType = UserState->vMapType;
	MapDownloaderState.vMobile = 0;
	MapDownloaderState.szLanguage = NULL;
	MapDownloaderState.vSensor = 0;
	MapDownloaderState.smClient = GOOGLE_STATIC_MAP_START;
}




/*****************************************************************************
  Function:
	SHORT Replace(BYTE *vExpression, ROM BYTE *vFind, ROM BYTE *vReplacement, 
				  WORD wMaxLen, BOOL bSearchCaseInsensitive)

  Summary:
	Replaces all instances of a particular substring with a new string

  Description:
	Searches a string (vExpression) and replaces all instances of a particular 
	substring (vFind) with a new string (vReplacement).  The start offset to 
	being searching and a maximum number of replacements can be specified.  The 
	search can be performed in a case sensitive or case insensitive manner.

  Precondition:
	This function is commented out by default to save code space because 
	it is not used by any current stack features.  However, if you want to use 
	it, go ahead and uncomment it.  It has been tested, so it (should) work 
	correctly.

  Parameters:
	vExpression - Null terminated string to search and make replacements within.
	vFind - Null terminated string to search for.
	vReplacement - Null terminated string to replace all instances of vFind with.
	wMaxLen - Maximum length of the output vExpression string if string 
		expansion is going to occur (replacement length is longer than find 
		length).  If the replacements will cause this maximum string length to 
		be exceeded, then no replacements will be made and a negative result 
		will be returned, indicating failure.  If the replacement length is 
		shorter or equal to the search length, then this parameter is ignored.
	bSearchCaseInsensitive - Boolean indicating if the search should be 
		performed in a case insensitive manner.  Specify TRUE for case 
		insensitive searches (slower) or FALSE for case sensitive 
		searching (faster).

  Remarks:
	If the replacement string length is shorter than or equal to the search 
	string length and the search string occurs in multiple overlapping 
	locations (ex\: expression is "aaa", find is "aa", and replacement is "bb") 
	then the first find match occuring when searching from left to right will 
	be replaced.  (ex\: output expression will be "bba").
	
	However, if the replacement string length is longer than the search string 
	length, the search will occur starting from the end of the string and 
	proceed to the beginning (right to left searching).  In this case if the 
	expression was "aaa", find was "aa", and replacement was "bbb", then the 
	final output expression will be "abbb".  

  Returns:
	If zero or greater, indicates the count of how many replacements were made.  
	If less than zero (negative result), indicates that wMaxLen was too small 
	to make the necessary replacements.  In this case, no replacements were 
	made.
  ***************************************************************************/
SHORT Replace(BYTE *vExpression, ROM BYTE *vFind, ROM BYTE *vReplacement, WORD wMaxLen, BOOL bSearchCaseInsensitive)
{
	WORD wExpressionLen, wFindLen, wFindLenMinusOne, wReplacementLen;
	WORD wFindCount, wReplacementsLeft;
	BYTE i, j;
	BYTE vFirstFindChar;
	WORD wBytesLeft;
	BYTE *vDest;
	BYTE *vExpressionCompare;
	ROM BYTE *vFindCompare;
	WORD w;

	wFindLen = strlenpgm((ROM char*)vFind);
	if(wFindLen == 0u)
		return 0;
	
	wExpressionLen = strlen((char*)vExpression);
	wReplacementLen = strlenpgm((ROM char*)vReplacement);

	wFindCount = 0;
	wFindLenMinusOne = wFindLen - 1;
	vFirstFindChar = *vFind++;
	if(bSearchCaseInsensitive)	// Convert to all lowercase if needed
		if((vFirstFindChar >= (BYTE)'A') && (vFirstFindChar <= (BYTE)'Z'))
			vFirstFindChar += 'a' - 'A';

	// If the replacement string is the same length as the search string, then 
	// we can immediately do the needed replacements inline and return.
	if(wFindLen == wReplacementLen)
	{
		for(wBytesLeft = wExpressionLen; wBytesLeft; wBytesLeft--)
		{
			i = *vExpression++;
			if(bSearchCaseInsensitive)
			{
				if((i >= (BYTE)'A') && (i <= (BYTE)'Z'))
					i += 'a' - 'A';
				if(i != vFirstFindChar)
					continue;
				vExpressionCompare = vExpression;
				vFindCompare = vFind;
				w = wFindLenMinusOne;
				while(w)
				{
					i = *vExpressionCompare++;
					j = *vFindCompare++;
					if((i >= (BYTE)'A') && (i <= (BYTE)'Z'))
						i += 'a' - 'A';
					if((j >= (BYTE)'A') && (j <= (BYTE)'Z'))
						j += 'a' - 'A';
					if(i != j)
						break;
					w--;
				}
				if(w)
					continue;
			}
			else
			{
				if(i != vFirstFindChar)
					continue;
				if(memcmppgm2ram((void*)vExpression, (ROM void*)vFind, wFindLenMinusOne))
					continue;
			}
	
			memcpypgm2ram((void*)vExpression-1, (ROM void*)vReplacement, wReplacementLen);
			wFindCount++;
			vExpression += wFindLenMinusOne;
			wBytesLeft -= wFindLenMinusOne;
		}
		return wFindCount;
	}
	
	
	// If the replacement string is shorter than the search string, then we can 
	// search from left to right and move the string over as we find occurrences.
	if(wFindLen > wReplacementLen)
	{
		vDest = vExpression;
		for(wBytesLeft = wExpressionLen; wBytesLeft; wBytesLeft--)
		{
			i = *vExpression++;
			*vDest++ = i;
			if(bSearchCaseInsensitive)
			{
				if((i >= (BYTE)'A') && (i <= (BYTE)'Z'))
					i += 'a' - 'A';
				if(i != vFirstFindChar)
					continue;
				vExpressionCompare = vExpression;
				vFindCompare = vFind;
				w = wFindLenMinusOne;
				while(w)
				{
					i = *vExpressionCompare++;
					j = *vFindCompare++;
					if((i >= (BYTE)'A') && (i <= (BYTE)'Z'))
						i += 'a' - 'A';
					if((j >= (BYTE)'A') && (j <= (BYTE)'Z'))
						j += 'a' - 'A';
					if(i != j)
						break;
					w--;
				}
				if(w)
					continue;
			}
			else
			{
				if(i != vFirstFindChar)
					continue;
				if(memcmppgm2ram((void*)vExpression, (ROM void*)vFind, wFindLenMinusOne))
					continue;
			}
	
			memcpypgm2ram((void*)vDest-1, (ROM void*)vReplacement, wReplacementLen);
			vDest += wReplacementLen-1;
			wFindCount++;
			vExpression += wFindLenMinusOne;
			wBytesLeft -= wFindLenMinusOne;
		}
		*vDest = 0x00;	// Write new null terminator since the string may have shrunk
		return wFindCount;
	}
	
	// If the replacement string is longer than the search string, then we will 
	// take a two pass approach.  On the first pass, we will merely count how 
	// many replacements to make.  With this we can calculate how long the 
	// final string is going to be.  On the second pass, we will search from 
	// right to left and expand the string as needed.

	// Pass 1: count how many occurrences of vFind are in vExpression
	for(wBytesLeft = wExpressionLen; wBytesLeft; wBytesLeft--)
	{
		i = *vExpression++;
		if(bSearchCaseInsensitive)
		{
			if((i >= (BYTE)'A') && (i <= (BYTE)'Z'))
				i += 'a' - 'A';
			if(i != vFirstFindChar)
				continue;
			vExpressionCompare = vExpression;
			vFindCompare = vFind;
			w = wFindLenMinusOne;
			while(w)
			{
				i = *vExpressionCompare++;
				j = *vFindCompare++;
				if((i >= (BYTE)'A') && (i <= (BYTE)'Z'))
					i += 'a' - 'A';
				if((j >= (BYTE)'A') && (j <= (BYTE)'Z'))
					j += 'a' - 'A';
				if(i != j)
					break;
				w--;
			}
			if(w)
				continue;
		}
		else
		{
			if(i != vFirstFindChar)
				continue;
			if(memcmppgm2ram((void*)vExpression, (ROM void*)vFind, wFindLenMinusOne))
				continue;
		}

		wFindCount++;
		vExpression += wFindLenMinusOne;
		wBytesLeft -= wFindLenMinusOne;
	}
	
	// Return immediately if no replacements are needed
	if(wFindCount == 0u)
		return 0;

	// Pass 2: make replacements and move string over
	vDest = vExpression + wFindCount * (wReplacementLen - wFindLen);
	if(vDest > vExpression - wExpressionLen + wMaxLen)
		return -1;
	*vDest-- = 0x00;	// Write new null terminator
	vExpression -= 1;
	vFind -= 1;
	vFirstFindChar = vFind[wFindLenMinusOne];
	if(bSearchCaseInsensitive)	// Convert to all lowercase if needed
		if((vFirstFindChar >= (BYTE)'A') && (vFirstFindChar <= (BYTE)'Z'))
			vFirstFindChar += 'a' - 'A';
	wReplacementsLeft = wFindCount;
	while(wReplacementsLeft)
	{
		i = *vExpression--;
		*vDest-- = i;
		if(bSearchCaseInsensitive)
		{
			if((i >= (BYTE)'A') && (i <= (BYTE)'Z'))
				i += 'a' - 'A';
			if(i != vFirstFindChar)
				continue;
			vExpressionCompare = vExpression;
			vFindCompare = &vFind[wFindLenMinusOne-1];
			w = wFindLenMinusOne;
			while(w)
			{
				i = *vExpressionCompare--;
				j = *vFindCompare--;
				if((i >= (BYTE)'A') && (i <= (BYTE)'Z'))
					i += 'a' - 'A';
				if((j >= (BYTE)'A') && (j <= (BYTE)'Z'))
					j += 'a' - 'A';
				if(i != j)
					break;
				w--;
			}
			if(w)
				continue;
		}
		else
		{
			if(i != vFirstFindChar)
				continue;
			if(memcmppgm2ram((void*)vExpression-wFindLenMinusOne, (ROM void*)vFind, wFindLenMinusOne))
				continue;
		}
		memcpypgm2ram((void*)vDest-wReplacementLen+2, (ROM void*)vReplacement, wReplacementLen);
		vDest -= wReplacementLen-1;

		vExpression -= wFindLenMinusOne;
		wBytesLeft -= wFindLenMinusOne;
		wReplacementsLeft--;
	}
	return wFindCount;
}

