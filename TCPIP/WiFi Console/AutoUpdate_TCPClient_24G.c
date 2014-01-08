/******************************************************************************

 FileName:        AutoUpdate_TCPClient.c
 Company:       Microchip Technology, Inc.

 Software License Agreement

 Copyright (C) 2002-2012 Microchip Technology Inc.  All rights reserved.

 Microchip licenses to you the right to use, modify, copy, and distribute:
 (i)  the Software when embedded on a Microchip microcontroller or digital 
      signal controller product ("Device") which is integrated into 
      Licensee's product; or
 (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
      ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device used in 
      conjunction with a Microchip ethernet controller for the sole purpose 
      of interfacing with the ethernet controller.

 You should refer to the license agreement accompanying this Software for 
 additional information regarding your rights and obligations.

 THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY INCIDENTAL,
 SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST
 OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS BY
 THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS
 FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS, WHETHER ASSERTED ON
 THE BASIS OF CONTRACT, TORT (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR
 OTHERWISE.

******************************************************************************/

#include "TCPIPConfig.h"
#include "TCPIP Stack/TCPIP.h"

//
// This file is used in OTA (over-the-air) web client MRF24WG RF transceiver firmware update.
// Applicable to MRF24WG only.
// MRF24WG0M RTP RF Transceiver FW is 0x3107  ( a2patch_3107_1029.bin)
//

#if defined(STACK_USE_AUTOUPDATE_TCPCLIENT) && defined(MRF24WG)
// Defines the server to be accessed for this application
// Username is mrfupdates , password is mchp1234
static BYTE ServerName[] = "www.microchip.com"; 

// Enter RF transceiver FW file name here. Case-sensitive.
static BYTE PatchName[]  = "/mrfupdates/a2patch_3107_1029.bin";
static BYTE Key_authorization[]="bXJmdXBkYXRlczptY2hwMTIzNA==" ; 

// Defines the port to be accessed for this application
static WORD ServerPort = 80;

extern UINT32 ImageUpdate_Addr;
extern UINT32 ImageUpdate_Checksum ;
extern UINT32 ImageUpdate_Size ;
extern void AutoUpdate_Initialize(void);
extern void AutoUpdate_Completed(void);
extern void AutoUpdate_Restore(void);

//#define SendSetParamMsg_debug   SendSetParamMsg


static void AU_print_string(UINT8 *buf,UINT8 length)
{
	int i;
	for(i=0;i<length;i++) putcUART(buf[i]);

}

static void AutoUpdate_SendToModule(UINT8 c, BOOL all)
{
	static UINT8 buf_module[36]={0};
	static UINT8 index_b = 0;
	if(all == TRUE)
	{
		if(index_b >0)
		{
			buf_module[0]=(ImageUpdate_Addr&0x00FF0000)>>16;
			buf_module[1]=(ImageUpdate_Addr&0x0000FF00)>>8;
			buf_module[2]=(ImageUpdate_Addr&0xFF);
			buf_module[3]=index_b;
			putsUART("!");
			SendSetParamMsg(PARAM_FLASH_WRITE, buf_module, index_b+4);
			ImageUpdate_Addr += index_b;
			index_b = 0;
		}
	}
	else
	{
		buf_module[index_b+4] = c;
		index_b ++;
		if(index_b ==32)
		{
			buf_module[0]=(ImageUpdate_Addr&0x00FF0000)>>16;
			buf_module[1]=(ImageUpdate_Addr&0x0000FF00)>>8;
			buf_module[2]=(ImageUpdate_Addr&0xFF);
			buf_module[3]=32;
			putsUART(".");
			SendSetParamMsg(PARAM_FLASH_WRITE, buf_module, index_b+4);
			ImageUpdate_Addr += 32;
			index_b = 0;
		}
	}
}
/*****************************************************************************
  Function:
	void AutoUpdate_TCPClient(void)

  Summary:
	 
  Description:

  Precondition:
	TCP is initialized.

  Parameters:
	None

  Returns:
  	None
  ***************************************************************************/

void AutoUpdate_TCPClient(void)
{
	BYTE 				j;
	WORD				w;
	BYTE				vBuffer[32];
	static DWORD		Timer;
	static TCP_SOCKET	MySocket = INVALID_SOCKET;
	static int SizeOfPackage=0;
	WORD lenA,lenB;
	static enum _GenericTCPExampleState
	{
		SM_UPDATE_HOME = 0,
		SM_UPDATE_SOCKET_OBTAINED,
		SM_UPDATE_GET_SIZE_1,
		SM_UPDATE_GET_SIZE_2,
		SM_UPDATE_SKIP_TO_DATA,
		SM_UPDATE_READ_DATA,
		SM_UPDATE_DISCONNECT,
		SM_UPDATE_DONE
	} AutoUpdateTCPExampleState = SM_UPDATE_DONE;
	switch(AutoUpdateTCPExampleState)
	{
		case SM_UPDATE_HOME:
			// Connect a socket to the remote TCP server
			MySocket = TCPOpen((DWORD)((unsigned int)&ServerName[0]), TCP_OPEN_RAM_HOST, ServerPort, TCP_PURPOSE_AUTOUPDATE_TCP_CLIENT);
			
			// Abort operation if no TCP socket of type TCP_PURPOSE_AUTOUPDATE_TCP_CLIENT is available
			// If this ever happens, you need to go add one to TCPIPConfig.h
			if(MySocket == INVALID_SOCKET)
				break;

			#if defined(STACK_USE_UART)
			putrsUART((ROM char*)"\r\n\r\nUpdate using Microchip TCP API...\r\n");
			#endif

			AutoUpdateTCPExampleState++;
			Timer = TickGet();
			break;

		case SM_UPDATE_SOCKET_OBTAINED:
			// Wait for the remote server to accept our connection request
			if(!TCPIsConnected(MySocket))
			{
				// Time out if too much time is spent in this state
				if(TickGet()-Timer > 5*TICK_SECOND)
				{
					// Close the socket so it can be used by other modules
					TCPDisconnect(MySocket);
					MySocket = INVALID_SOCKET;
					AutoUpdateTCPExampleState--;
				}
				break;
			}
            AutoUpdate_Initialize();
			Timer = TickGet();

			// Make certain the socket can be written to
			if(TCPIsPutReady(MySocket) < 125u)
				break;
			TCPPutROMString(MySocket, (ROM BYTE*)"GET ");
			TCPPutROMString(MySocket, (ROM BYTE*)PatchName);
			TCPPutROMString(MySocket, (ROM BYTE*)" HTTP/1.1\r\nHost:");			
			TCPPutROMString(MySocket, (ROM BYTE*)ServerName);
			TCPPutROMString(MySocket, (ROM BYTE*)"\r\nConnection: close\r\n");
			TCPPutROMString(MySocket, (ROM BYTE*)"Authorization: Basic ");
			TCPPutROMString(MySocket, (ROM BYTE*)Key_authorization);
			TCPPutROMString(MySocket, (ROM BYTE*)"\r\n\r\n");

			// Send the packet
			TCPFlush(MySocket);
		
			AutoUpdateTCPExampleState++;
			break;
		case SM_UPDATE_GET_SIZE_1:  //Looking for "Length:"
			if(!TCPIsConnected(MySocket))
			{
				AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
				break;
			}
			// 2. Check ,if there is data?
			w = TCPIsGetReady(MySocket);		
			if(w ==0) break;
			
			lenA = TCPFindROMArray(MySocket, (ROM BYTE*)"Length:", 7, 0, FALSE);
			if(lenA != 0xffff)
			{// Found it, so remove all data up to and including
				while(lenA>0)
				{
					lenB = TCPGetArray(MySocket, vBuffer, ((lenA <= sizeof(vBuffer)) ? lenA : sizeof(vBuffer)));
					AU_print_string(vBuffer,lenB);
					lenA -= lenB;
				}
				AutoUpdateTCPExampleState = SM_UPDATE_GET_SIZE_2;
				
				// No break if we found the "Length:"	
			}
			else
			{// Otherwise, remove as much as possible
				w -= 7;
				while(w>0)
				{
					lenB = TCPGetArray(MySocket, vBuffer, ((w <= sizeof(vBuffer)) ? w : sizeof(vBuffer)));
					AU_print_string(vBuffer,lenB);
					w -= lenB;
				}
				break;
			}
		case SM_UPDATE_GET_SIZE_2:  //Looking for "\r\n",if yes, we get the size of package
			if(!TCPIsConnected(MySocket))
			{
				AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
				break;
			}
			// 2. Check ,if there is data?
			w = TCPIsGetReady(MySocket);		
			if(w ==0) break;
			lenA = TCPFindROMArray(MySocket, (ROM BYTE*)"\r\n", 2, 0, FALSE);
			if(lenA != 0xffff)
			{// Found it, get the size
				lenA = TCPGetArray(MySocket, vBuffer, lenA+2);	
				AU_print_string(vBuffer,lenA);
				sscanf((const char*)vBuffer,"Length: %d\r\n",&SizeOfPackage);
				//{char buf_t[16];sprintf(buf_t,"\r\n size=%d\r\n",SizeOfPackage);putsUART(buf_t);}
				AutoUpdateTCPExampleState = SM_UPDATE_SKIP_TO_DATA;
				// No break if we found the size
			}
			else
			{// Otherwise, wait to next read
				break;
			}

		case SM_UPDATE_SKIP_TO_DATA: // look for "\r\n\r\n"
			// 1. Check to see if the remote node has disconnected from us or sent us any application data
			if(!TCPIsConnected(MySocket))
			{
				AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}

			// 2. Check ,if there is data?
			w = TCPIsGetReady(MySocket);		
			if(w ==0) break;
			
			lenA = TCPFindROMArray(MySocket, (ROM BYTE*)"\r\n\r\n", 4, 0, FALSE);
	
			if(lenA != 0xffff)
			{// Found it, so remove all data up to and including
				while(lenA>0)
				{
					lenB = TCPGetArray(MySocket, vBuffer, ((lenA <= sizeof(vBuffer)) ? lenA : sizeof(vBuffer)));
					AU_print_string(vBuffer,lenB);
					lenA -= lenB;
				}
				lenB = TCPGetArray(MySocket, vBuffer, 4);
				AU_print_string(vBuffer,lenB);
				AutoUpdateTCPExampleState = SM_UPDATE_READ_DATA;
				
				// No break if we found the header terminator	
			}
			else
			{// Otherwise, remove as much as possible
				
				w -= 4;
				while(w>0)
				{
					lenB = TCPGetArray(MySocket, vBuffer, ((w <= sizeof(vBuffer)) ? w : sizeof(vBuffer)));
					AU_print_string(vBuffer,lenB);
					w -= lenB;
				}
				//
				break;
			}
						
		case SM_UPDATE_READ_DATA:
			if(!TCPIsConnected(MySocket))
			{
				AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}
			// Get count of RX bytes waiting
			w = TCPIsGetReady(MySocket);
			j = sizeof(vBuffer)-1;
			while(w)
			{
				int i;
				if(w < j){j = w; }
				vBuffer[j] = '\0';
				lenB= TCPGetArray(MySocket, vBuffer, j);
				
				w -= lenB;
				
				for(i=0;i<lenB;i++)
				{
					//Size and CheckSum
					if((ImageUpdate_Size % 4u) == 0u) ImageUpdate_Checksum += (UINT32)vBuffer[i]<<24;
					if((ImageUpdate_Size % 4u) == 1u) ImageUpdate_Checksum += (UINT32)vBuffer[i]<<16;
					if((ImageUpdate_Size % 4u) == 2u) ImageUpdate_Checksum += (UINT32)vBuffer[i]<<8;
					if((ImageUpdate_Size % 4u) == 3u) ImageUpdate_Checksum += (UINT32)vBuffer[i];
					ImageUpdate_Size ++;
					
					//{ char buf_t[10];sprintf(buf_t,"%02x",vBuffer[i]);putsUART(buf_t);}
					AutoUpdate_SendToModule(vBuffer[i],FALSE);
				}
				
			}
			break;
		case SM_UPDATE_DISCONNECT:
			putsUART("\r\nClosed---\r\n");
			TCPDisconnect(MySocket);
			MySocket = INVALID_SOCKET;
			AutoUpdateTCPExampleState = SM_UPDATE_DONE;
	   					
			//did we receive enough data? Is the size correct?
			if( SizeOfPackage == ImageUpdate_Size)
			{	
				AutoUpdate_SendToModule(0,TRUE);  //Send all data left in buffer
				AutoUpdate_Completed();
			}
			else   //We lost data, so cancel , and roll back
			{
				putsUART("Revert begin...\r\n");
				AutoUpdate_Restore();
				putsUART("Revert Done\r\n");
			}
			break;
	
		case SM_UPDATE_DONE:
			// Do nothing unless the user pushes BUTTON1 and wants to restart the whole connection/download process
			if(BUTTON2_IO == 0u)
			{
				AutoUpdateTCPExampleState = SM_UPDATE_HOME;
				//AutoUpdate_Initialize();				
			}
			break;
	}
}

#endif
