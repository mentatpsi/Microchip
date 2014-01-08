/*********************************************************************
 *
 *     Microchip Announcement Detector
 *
 *********************************************************************
 * FileName:        MCHPDetect.cpp
 * Dependencies:    Must be linked with Ws2_32.lib
 * Processor:       x86
 * Complier:        Microsoft Visual C++ 6.0
 *		    Borland C++ Builder 6
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * This software is owned by Microchip Technology Inc. ("Microchip")
 * and is supplied to you for use exclusively as described in the
 * associated software agreement.  This software is protected by 
 * software and other intellectual property laws.  Any use in 
 * violation of the software license may subject the user to criminal 
 * sanctions as well as civil liability.  Copyright 2006 Microchip
 * Technology Inc.  All rights reserved.
 *
 * This software is provided "AS IS."  MICROCHIP DISCLAIMS ALL 
 * WARRANTIES, EXPRESS, IMPLIED, STATUTORY OR OTHERWISE, NOT LIMITED 
 * TO MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
 * INFRINGEMENT.  Microchip shall in no event be liable for special, 
 * incidental, or consequential damages.
 *
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder	10/8/04	Original
********************************************************************/

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers

#define UDPPort	       30303

#include <Winsock2.h>
#include <iostream>
#include <stdio.h>


				
int main(int argc, char* argv[])
{
	WSADATA			WSAData;
	SOCKET			MySocket;
	char			PacketPayload[1500];
	int			ret;
	sockaddr_in		MySocketInfo;
	sockaddr		RemoteAddr;
	int			RemoteAddrSize;
	unsigned int	        MessageCount = 1;


	if(WSAStartup((short)0x0202, &WSAData) != 0)
	{
        printf("WSAStartup failed!\n");
        return 1;
	}


	MySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(MySocket == INVALID_SOCKET)
	{
		std::cout << "Unable to create a socket.\n";
		WSACleanup();
		return 1;
	}


	MySocketInfo.sin_family = AF_INET;
	MySocketInfo.sin_port = htons(UDPPort);
	MySocketInfo.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
	ZeroMemory(&MySocketInfo.sin_zero, sizeof(MySocketInfo.sin_zero));

	ret = bind(MySocket, (const struct sockaddr *)&MySocketInfo, sizeof(MySocketInfo));
	if(ret == INVALID_SOCKET)
	{
		std::cout << "Bind procedure failed.\n";
		WSACleanup();
		return 1;
	}

	RemoteAddrSize = sizeof(RemoteAddr);

	std::cout << "Microchip Announcement Detector\n"
		     "Copyright (c) 2004 Microchip Technology, Inc. Ver. 1.0 (Oct 06 2004)\n"
		     "Written by Howard Henry Schlunder\n\n"
		     "Waiting for data on UDP port " << UDPPort << "...\n\n";


	while(1)
	{
		ret = recvfrom(MySocket, PacketPayload, sizeof(PacketPayload), 0, &RemoteAddr, &RemoteAddrSize);
		if(ret == SOCKET_ERROR)
		{
			std::cout << "Socket error while trying to receive.\n";
			WSACleanup();
			return 1;
		}

		PacketPayload[ret] = 0;	// Null terminate

		std::cout << "Total packets received: " << MessageCount++ << "\n"
				  << "Received " << ret << " bytes of data from " 
				  << (unsigned char)RemoteAddr.sa_data[2] + '\0' << '.' 
				  << (unsigned char)RemoteAddr.sa_data[3] + '\0' << '.' 
				  << (unsigned char)RemoteAddr.sa_data[4] + '\0' << '.' 
				  << (unsigned char)RemoteAddr.sa_data[5] + '\0' << '\n'
				  << PacketPayload << "\n\n";
	}
	

	WSACleanup();
	return 0;
}

