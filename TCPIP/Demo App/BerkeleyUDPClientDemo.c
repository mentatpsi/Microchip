/*********************************************************************
 *
 *	Simple Network Time Protocol (SNTP) Client Version 3
 *  Module for Microchip TCP/IP Stack - implemented with Berkeley API
 *	 -Locates an NTP Server from public site using DNS
 *	 -Requests UTC time using SNTP and updates SNTPTime structure
 *	  periodically, according to NTP_QUERY_INTERVAL value
 *	- Reference: RFC 1305
 *
 *********************************************************************
 * FileName:        BerkeleyUDPClientDemo.c
 * Dependencies:    UDP, ARP, DNS, Tick
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
 * Darren Wenn			03/08/07	Original
 * Howard Schlunder		06/20/07	Modified for release
 ********************************************************************/
#define __SNTP_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_BERKELEY_API)

#include "TCPIP Stack/TCPIP.h"


// Defines how frequently to resynchronize the date/time (default: 10 minutes)
#define NTP_QUERY_INTERVAL		(10ull*60ull * TICK_SECOND)

// Defines how long to wait to retry an update after a failure.
// Updates may take up to 6 seconds to fail, so this 14 second delay is actually only an 8-second retry.
#define NTP_FAST_QUERY_INTERVAL	(14ull * TICK_SECOND)

// Port for contacting NTP servers
#define NTP_SERVER_PORT			(123ul)

// Reference Epoch to use.  (default: 01-Jan-1970 00:00:00)
#define NTP_EPOCH 				(86400ul * (365ul * 70ul + 17ul))

// Defines how long to wait before assuming the query has failed
#define NTP_REPLY_TIMEOUT		(6ul*TICK_SECOND)

// These are normally available network time servers.
// The actual IP returned from the pool will vary every
// minute so as to spread the load around stratum 1 timeservers.
// For best accuracy and network overhead you should locate the 
// pool server closest to your geography, but it will still work
// if you use the global pool.ntp.org address or choose the wrong 
// one or ship your embedded device to another geography.
#ifdef WIFI_NET_TEST
#define NTP_SERVER	"ntp" WIFI_NET_TEST_DOMAIN
#else
#define NTP_SERVER	"pool.ntp.org"
//#define NTP_SERVER	"europe.pool.ntp.org"
//#define NTP_SERVER	"asia.pool.ntp.org"
//#define NTP_SERVER	"oceania.pool.ntp.org"
//#define NTP_SERVER	"north-america.pool.ntp.org"
//#define NTP_SERVER	"south-america.pool.ntp.org"
//#define NTP_SERVER	"africa.pool.ntp.org"
#endif

// Defines the structure of an NTP packet
typedef struct
{
	struct
	{
		BYTE mode			: 3;	// NTP mode
		BYTE versionNumber 	: 3;	// SNTP version number
		BYTE leapIndicator	: 2;	// Leap second indicator
	} flags;						// Flags for the packet

	BYTE stratum;					// Stratum level of local clock
	CHAR poll;						// Poll interval
	CHAR precision;					// Precision (seconds to nearest power of 2)
	DWORD root_delay;				// Root delay between local machine and server
	DWORD root_dispersion;			// Root dispersion (maximum error)
	DWORD ref_identifier;			// Reference clock identifier
	DWORD ref_ts_secs;				// Reference timestamp (in seconds)
	DWORD ref_ts_fraq;				// Reference timestamp (fractions)
	DWORD orig_ts_secs;				// Origination timestamp (in seconds)
	DWORD orig_ts_fraq;				// Origination timestamp (fractions)
	DWORD recv_ts_secs;				// Time at which request arrived at sender (seconds)
	DWORD recv_ts_fraq;				// Time at which request arrived at sender (fractions)
	DWORD tx_ts_secs;				// Time at which request left sender (seconds)
	DWORD tx_ts_fraq;				// Time at which request left sender (fractions)
} NTP_PACKET;

// Seconds value obtained by last update
static DWORD dwSNTPSeconds = 0;

// Tick count of last update
static DWORD dwLastUpdateTick = 0;


/*****************************************************************************
  Function:
	void BerkeleyUDPClientDemo(void)

  Summary:
	Periodically checks the current time from a pool of servers.

  Description:
	This function periodically checks a pool of time servers to obtain the
	current date/time.

  Precondition:
	UDP is initialized.

  Parameters:
	None

  Returns:
  	None
  	
  Remarks:
	This function requires once available UDP socket while processing, but
	frees that socket when the SNTP module is idle.
  ***************************************************************************/
void BerkeleyUDPClientDemo(void)
{
	#if defined(STACK_USE_DNS)
	NTP_PACKET			pkt;
	int			 		i;
	static DWORD		dwServerIP;
	static DWORD		dwTimer;
    static SOCKET		bsdUdpClient;
    int 				addrlen = sizeof(struct sockaddr_in);
    static struct sockaddr_in	udpaddr;
	static enum
	{
		SM_HOME = 0,
		SM_NAME_RESOLVE,
		SM_CREATE_SOCKET,
		//SM_BIND,	// Not required since we are sending the first packet
		SM_UDP_SEND,
		SM_UDP_RECV,
		SM_SHORT_WAIT,
		SM_WAIT
	} SNTPState = SM_HOME;

	switch(SNTPState)
	{
		case SM_HOME:
			// Obtain ownership of the DNS resolution module
			if(!DNSBeginUsage())
				break;

			// Obtain the IP address associated with the server name
			DNSResolveROM((ROM BYTE*)NTP_SERVER, DNS_TYPE_A);
			dwTimer = TickGet();
			SNTPState = SM_NAME_RESOLVE;
			break;

		case SM_NAME_RESOLVE:
			// Wait for DNS resolution to complete
			if(!DNSIsResolved((IP_ADDR*)&dwServerIP)) 
			{
				if((TickGet() - dwTimer) > (5 * TICK_SECOND)) 
				{
					DNSEndUsage();
					dwTimer = TickGetDiv64K();
					SNTPState = SM_SHORT_WAIT;
				}
				break;
			}
			
			// Obtain DNS resolution result
			if(!DNSEndUsage())
			{
				// No valid IP address was returned from the DNS 
				// server.  Quit and fail for a while if host is not valid.
				dwTimer = TickGetDiv64K();
				SNTPState = SM_SHORT_WAIT;
				break;
			}
			
			SNTPState = SM_CREATE_SOCKET;
			// No break needed

		case SM_CREATE_SOCKET:
			bsdUdpClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if(bsdUdpClient == INVALID_SOCKET)
				return;

// No explicit binding is necessary since we are going to be the first one 
// sending a UDP packet.  sendto() will do implicit binding.  Explicit binding 
// is necessary only when creating UDP servers that will be receiving the first 
// packet.
//			SNTPState = SM_BIND;
//			// No break needed
//			
//		case SM_BIND:
//            udpaddr.sin_port = 0;
//            udpaddr.sin_addr.S_un.S_addr = IP_ADDR_ANY;
//            if( bind(bsdUdpClient, (struct sockaddr*)&udpaddr, addrlen) == SOCKET_ERROR )
//                break;
//
            SNTPState = SM_UDP_SEND;
            // No break needed
            
		case SM_UDP_SEND:
            // Transmit a time request packet
			memset(&pkt, 0, sizeof(pkt));
			pkt.flags.versionNumber = 3;	// NTP Version 3
			pkt.flags.mode = 3;				// NTP Client
			pkt.orig_ts_secs = swapl(NTP_EPOCH);
            udpaddr.sin_port = NTP_SERVER_PORT;
            udpaddr.sin_addr.S_un.S_addr = dwServerIP;
			if(sendto(bsdUdpClient, (const char*)&pkt, sizeof(pkt), 0, (struct sockaddr*)&udpaddr, addrlen)>0)
            {
				dwTimer = TickGet();
                SNTPState = SM_UDP_RECV;
            }
			break;

		case SM_UDP_RECV:
			// Look for a response time packet
			i = recvfrom(bsdUdpClient, (char*)&pkt, sizeof(pkt), 0, (struct sockaddr*)&udpaddr, &addrlen);
            if(i < (int)sizeof(pkt))
			{
				if((TickGet()) - dwTimer > NTP_REPLY_TIMEOUT)
				{
					// Abort the request and wait until the next timeout period
					closesocket(bsdUdpClient);
					dwTimer = TickGetDiv64K();
					SNTPState = SM_SHORT_WAIT;
					break;
				}
				break;
			}
			closesocket(bsdUdpClient);
			dwTimer = TickGetDiv64K();
			SNTPState = SM_WAIT;

			// Set out local time to match the returned time
			dwLastUpdateTick = TickGet();
			dwSNTPSeconds = swapl(pkt.tx_ts_secs) - NTP_EPOCH;
			// Do rounding.  If the partial seconds is > 0.5 then add 1 to the seconds count.
			if(((BYTE*)&pkt.tx_ts_fraq)[0] & 0x80)
				dwSNTPSeconds++;

			#ifdef WIFI_NET_TEST
				wifi_net_test_print("SNTP: current time", dwSNTPSeconds);
			#endif
			break;

		case SM_SHORT_WAIT:
			// Attempt to requery the NTP server after a specified NTP_FAST_QUERY_INTERVAL time (ex: 8 seconds) has elapsed.
			if(TickGetDiv64K() - dwTimer > (NTP_FAST_QUERY_INTERVAL/65536ull))
				SNTPState = SM_HOME;	
			break;

		case SM_WAIT:
			// Requery the NTP server after a specified NTP_QUERY_INTERVAL time (ex: 10 minutes) has elapsed.
			if(TickGetDiv64K() - dwTimer > (NTP_QUERY_INTERVAL/65536ull))
				SNTPState = SM_HOME;	

			break;
	}

	//#if defined(STACK_USE_DNS)
	#else
		#warning You must define STACK_USE_DNS for BerkeleyUDPClientDemo to work
	#endif
}


/*****************************************************************************
  Function:
	DWORD BerkeleySNTPGetUTCSeconds(void)

  Summary:
	Obtains the current time from the SNTP module.

  Description:
	This function obtains the current time as reported by the SNTP module.  
	Use this value for absolute time stamping.  The value returned is (by
	default) the number of seconds since 01-Jan-1970 00:00:00.

  Precondition:
	None

  Parameters:
	None

  Returns:
  	The number of seconds since the Epoch.  (Default 01-Jan-1970 00:00:00)
  	
  Remarks:
	Do not use this function for time difference measurements.  The Tick
	module is more appropriate for those requirements.
  ***************************************************************************/
DWORD BerkeleySNTPGetUTCSeconds(void)
{
	DWORD dwTickDelta;
	DWORD dwTick;

	// Update the dwSNTPSeconds variable with the number of seconds 
	// that has elapsed
	dwTick = TickGet();
	dwTickDelta = dwTick - dwLastUpdateTick;
	while(dwTickDelta > TICK_SECOND)
	{
		dwSNTPSeconds++;
		dwTickDelta -= TICK_SECOND;
	}
	
	// Save the tick and residual fractional seconds for the next call
	dwLastUpdateTick = dwTick - dwTickDelta;

	return dwSNTPSeconds;
}

#endif  //if defined(STACK_USE_BERKELEY_API)

