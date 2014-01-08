/*********************************************************************
 *
 *  Berekely TCP server demo application.
 *  This application uses the BSD socket APIs and starts a server 
 *  listening on TCP port 9764.  All data sent to a connection on 
 *  this port will be echoed back to the sender.  By default, this 
 *  demo supports 3 simultaneous connections.
 *
 *********************************************************************
 * FileName:        BerkeleyTCPServerDemo.c
 * Company:         Microchip Technology, Inc.
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
 * Author               Date    	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Aseem Swalah         4/21/08  	Original
 ********************************************************************/
 
#include "TCPIPConfig.h" 

#if defined(STACK_USE_BERKELEY_API)

#include "TCPIP Stack/TCPIP.h"


#define PORTNUM 9764
#define MAX_CLIENT (3) // Maximum number of simultanous connections accepted by the server.


/*********************************************************************
 * Function:        void BerkeleyTCPServerDemo(void)
 *
 * PreCondition:    Stack is initialized
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void BerkeleyTCPServerDemo(void)
{
    static SOCKET bsdServerSocket;   
    static SOCKET ClientSock[MAX_CLIENT];
    struct sockaddr_in addr;
    struct sockaddr_in addRemote;
    int addrlen = sizeof(struct sockaddr_in);
    char bfr[15];
    int length;
    int i;
    static enum
    {
	    BSD_INIT = 0,
        BSD_CREATE_SOCKET,
        BSD_BIND,
        BSD_LISTEN,
        BSD_OPERATION
    } BSDServerState = BSD_INIT;

    switch(BSDServerState)
    {
	    case BSD_INIT:
        	// Initialize all client socket handles so that we don't process 
        	// them in the BSD_OPERATION state
        	for(i = 0; i < MAX_CLIENT; i++)
        		ClientSock[i] = INVALID_SOCKET;
        		
        	BSDServerState = BSD_CREATE_SOCKET;
        	// No break needed
	    
        case BSD_CREATE_SOCKET:
            // Create a socket for this server to listen and accept connections on
            bsdServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(bsdServerSocket == INVALID_SOCKET)
                return;
            
            BSDServerState = BSD_BIND;
            // No break needed

        case BSD_BIND:
            // Bind socket to a local port
            addr.sin_port = PORTNUM;
            addr.sin_addr.S_un.S_addr = IP_ADDR_ANY;
            if( bind( bsdServerSocket, (struct sockaddr*)&addr, addrlen ) == SOCKET_ERROR )
                return;
            
            BSDServerState = BSD_LISTEN;
            // No break needed
         
      case BSD_LISTEN:
            if(listen(bsdServerSocket, MAX_CLIENT) == 0)
	            BSDServerState = BSD_OPERATION;

			// No break.  If listen() returns SOCKET_ERROR it could be because 
			// MAX_CLIENT is set to too large of a backlog than can be handled 
			// by the underlying TCP socket count (TCP_PURPOSE_BERKELEY_SERVER 
			// type sockets in TCPIPConfig.h).  However, in this case, it is 
			// possible that some of the backlog is still handleable, in which 
			// case we should try to accept() connections anyway and proceed 
			// with normal operation.
         
      case BSD_OPERATION:
            for(i=0; i<MAX_CLIENT; i++)
            {
	            // Accept any pending connection requests, assuming we have a place to store the socket descriptor
                if(ClientSock[i] == INVALID_SOCKET)
                    ClientSock[i] = accept(bsdServerSocket, (struct sockaddr*)&addRemote, &addrlen);
                
                // If this socket is not connected then no need to process anything
                if(ClientSock[i] == INVALID_SOCKET)
                	continue;

	            // For all connected sockets, receive and send back the data
                length = recv( ClientSock[i], bfr, sizeof(bfr), 0);
         
                if( length > 0 )
                {
                    bfr[length] = '\0';
                    send(ClientSock[i], bfr, strlen(bfr), 0);
                }
                else if( length < 0 )
                {
                    closesocket( ClientSock[i] );
                    ClientSock[i] = INVALID_SOCKET;
                }
            }
            break;
         
        default:
            return;
    }
    return;
}

#endif //#if defined(STACK_USE_BERKELEY_API)

