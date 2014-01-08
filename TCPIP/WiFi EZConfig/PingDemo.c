/*********************************************************************
 *
 *  ICMP Client Demo (Ping)
 *
 *********************************************************************
 * FileName:        PingDemo.c
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
 * Author               Date      Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * E. Wood     			4/26/08	  Moved from MainDemo.c
 ********************************************************************/
#define __PINGDEMO_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_ICMP_CLIENT)

#include "TCPIP Stack/TCPIP.h"
#include "MainDemo.h"


#define HOST_TO_PING	"ww1.microchip.com"	// Address that ICMP client will ping.  If the DNS client module is not available in the stack, then this hostname is ignored and the local gateway IP address will be pinged instead.

/*****************************************************************************
  Function:
	void PingDemo(void)

  Summary:
	Demonstrates use of the ICMP (Ping) client.
	
  Description:
	This function implements a simple ICMP client.  The function is called
	periodically by the stack, and it checks if BUTTON0 has been pressed.  
	If the button is pressed, the function sends an ICMP Echo Request (Ping)
	to a Microchip web server.  The round trip time is displayed on the UART
	when the response is received.
	
	This function can be used as a model for applications requiring Ping 
	capabilities to check if a host is reachable.

  Precondition:
	TCP is initialized.

  Parameters:
	None

  Returns:
  	None
  ***************************************************************************/
void PingDemo(void)
{
	static enum
	{
		SM_HOME = 0,
		SM_GET_ICMP_RESPONSE
	} PingState = SM_HOME;
	static DWORD Timer;
	LONG ret;

	switch(PingState)
	{
		case SM_HOME:
			// Send a ping request out if the user pushes BUTTON0 (right-most one)
			if(BUTTON0_IO == 0u)
			{
				// Don't ping flood: wait at least 1 second between ping requests
				if(TickGet() - Timer > 1ul*TICK_SECOND)
				{
					// Obtain ownership of the ICMP module
					if(!ICMPBeginUsage())
						break;
					
					// Update anti-ping flood timer
					Timer = TickGet();

					// Send ICMP echo request
					#if defined(STACK_USE_DNS)
						ICMPSendPingToHostROM((ROM BYTE*)HOST_TO_PING);
					#else
						ICMPSendPing(AppConfig.MyGateway.Val);
					#endif
					PingState = SM_GET_ICMP_RESPONSE;
				}
			}
			break;
			
		case SM_GET_ICMP_RESPONSE:
			// Get the status of the ICMP module
			ret = ICMPGetReply();					
			if(ret == -2)
			{
				// Do nothing: still waiting for echo
				break;
			}
			else if(ret == -1)
			{
				// Request timed out
				#if defined(USE_LCD)
				memcpypgm2ram((void*)&LCDText[16], (ROM void *)"Ping timed out", 15);
				LCDUpdate();
				#endif
				PingState = SM_HOME;
			}
			else if(ret == -3)
			{
				// DNS address not resolvable
				#if defined(USE_LCD)
				memcpypgm2ram((void*)&LCDText[16], (ROM void *)"Can't resolve IP", 16);
				LCDUpdate();
				#endif
				PingState = SM_HOME;
			}
			else
			{
				// Echo received.  Time elapsed is stored in ret (Tick units).
				#if defined(USE_LCD)
				memcpypgm2ram((void*)&LCDText[16], (ROM void *)"Reply: ", 7);
				uitoa((WORD)TickConvertToMilliseconds((DWORD)ret), &LCDText[16+7]);
				strcatpgm2ram((char*)&LCDText[16+7], "ms");
				LCDUpdate();
				#endif
				PingState = SM_HOME;
			}
			
			// Finished with the ICMP module, release it so other apps can begin using it
			ICMPEndUsage();
			break;
	}
}

BYTE PING_Console_Host[32]="192.168.1.1";
INT32 Count_PingConsole = 0;
INT32 PINGINT_Console_Host=0x0100A8C0;  //192.168.0.1
BOOL pingUseDNS = TRUE;

#if defined(STACK_USE_CERTIFATE_DEBUG)
BOOL b_PingFroever = FALSE;
#endif
void PingConsole(void)
{
	static enum
	{
		SM_HOME = 0,
		SM_GET_ICMP_RESPONSE
	} PingState = SM_HOME;
	static DWORD Timer;
	LONG ret;
	static INT32 statistics_send=0, statistics_Recv=0, statistics_lost=0;
	static INT32 statistics_TimeMax=0, statistics_TimeMin=0, statistics_TimeTotal=0;
#if defined(STACK_USE_CERTIFATE_DEBUG)	
	if(b_PingFroever == TRUE) Count_PingConsole = 4;
#endif
	switch(PingState)
	{
		case SM_HOME:
			if(Count_PingConsole > 0)
			{
				if(TickGet() - Timer > 1ul*TICK_SECOND)
				{
					if(!ICMPBeginUsage())   break;
					Timer = TickGet();
					// Send ICMP echo request
					if(TRUE==pingUseDNS)
					{
					#if defined(STACK_USE_DNS)
						ICMPSendPingToHostROM((ROM BYTE*)PING_Console_Host);
					#else
						putsUART("DNS assert ...");while(1);
					#endif
					}
					else
					{
						ICMPSendPing(PINGINT_Console_Host);
					}
					statistics_send ++;
					Count_PingConsole --;
					PingState = SM_GET_ICMP_RESPONSE;
				}
			}break;	
		case SM_GET_ICMP_RESPONSE:
			// Get the status of the ICMP module
			ret = ICMPGetReply();					
			if(ret == -2)
			{
				// Do nothing: still waiting for echo
				break;
			}
			else if(ret == -1)
			{
				// Request timed out
				statistics_lost ++; 
				putsUART("Ping timed out ");
				{ char buf_t[20]; sprintf(buf_t,":Lost %d times\r\n",(int)statistics_lost);putsUART(buf_t);}
				PingState = SM_HOME;
				if(Count_PingConsole ==0) goto _DonePingConsole;
			}
			else if(ret == -3)
			{
				// DNS address not resolvable
				putsUART("Can't resolve IP\r\n");
				PingState = SM_HOME;
				Count_PingConsole =0;
			}
			else
			{
				// Echo received.  Time elapsed is stored in ret (Tick units).
				statistics_Recv ++;
				DWORD delay = TickConvertToMilliseconds((DWORD)ret);
				if(delay > statistics_TimeMax) statistics_TimeMax = delay;
				if(delay < statistics_TimeMin) statistics_TimeMin = delay;
				if(statistics_TimeMin ==0) statistics_TimeMin = delay;
				statistics_TimeTotal += delay;
				putsUART("Reply From ");
				char buf_t[32]={0};
				sprintf(buf_t, "%s: time=%dms\r\n",PING_Console_Host, (int)delay);
				putsUART(buf_t);
				PingState = SM_HOME;
				if(Count_PingConsole ==0) goto _DonePingConsole;
			}
			
			// Finished with the ICMP module, release it so other apps can begin using it
			ICMPEndUsage();
			break;
		default:
			break;

	}
	return;
_DonePingConsole:
			ICMPEndUsage();
			putsUART("Ping statistics for ");
			putsUART((char*)PING_Console_Host);
			putsUART(":\r\n");
			
			putsUART("  Packets: ");
			char buf_t[20];
			sprintf(buf_t, "Sent = %d, ",(int)statistics_send);		putsUART(buf_t);
			sprintf(buf_t, "Received = %d, ",(int)statistics_Recv);	putsUART(buf_t);
			sprintf(buf_t, "Lost = %d ", (int)statistics_lost);		putsUART(buf_t);
			sprintf(buf_t, "(%d%c loss)", (int)((100*statistics_lost)/statistics_send), '%'); putsUART(buf_t);
	
			putsUART("\r\nApproximate round trip times in milli-seconds:\r\n");
			sprintf(buf_t, "  Minimum = %dms, ", (int)statistics_TimeMin);putsUART(buf_t);
			sprintf(buf_t, "Maximum = %dms, ", (int)statistics_TimeMax);putsUART(buf_t);
			if(statistics_Recv != 0)
				sprintf(buf_t, "Average = %dms\r\n>", (int)(statistics_TimeTotal/statistics_Recv) );putsUART(buf_t);
			putsUART("\r\n>");
			statistics_send=0; statistics_Recv=0; statistics_lost=0;
			statistics_TimeMax=0; statistics_TimeMin=0; statistics_TimeTotal=0;
			return;

}


#endif	//#if defined(STACK_USE_ICMP_CLIENT)
