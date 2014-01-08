/******************************************************************************

 MRF24W Driver iperf
 Module for Microchip TCP/IP Stack
  -Provides access to MRF24W WiFi controller
  -Reference: MRF24W Data sheet, IEEE 802.11 Standard

*******************************************************************************
 FileName:		IperfApp.c
 Dependencies:	TCP/IP Stack header files
 Processor:		PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 Compiler:		Microchip C32 v1.10b or higher
				Microchip C30 v3.22 or higher
				Microchip C18 v3.34 or higher
 Company:		Microchip Technology, Inc.

 Software License Agreement

 Copyright (C) 2002-2010 Microchip Technology Inc.  All rights reserved.

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


 Author				Date		Comment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 KH                 27 Jan 2010 Updated for MRF24W
******************************************************************************/

#include <string.h> /* for memcpy */

#include "TCPIP Stack/TCPIP.h"

#if defined (WF_CONSOLE)

#include "IperfApp.h"
#include "TCPIP Stack/WFConsole.h"

#if defined(WF_DEBUG)
    #define WF_MODULE_NUMBER   WF_MODULE_IPERF_APP
#endif
/*****************************************************************************/
/* Constants used internally by code in this file.                           */
/*****************************************************************************/


#define LOCAL_PORT_START_NUMBER     (1024u)


//#define kIperfRxTimeOut 				1000    // 1 second.

#define kIperfUDPFinRetransmitCount		10u		// iperf retransmits 10 times the last UDP packet,
#define kIperfUDPFinRetransmitPeriod    10      // at 10ms apart.
#define kIperfTimingErrorMargin 		2		// Account for msec tick uncertainty.

enum {
    kStateStandby=1,

    kStateRxStart,
    kStateUdpRx,
    kStateUdpRxDrain,
    kStateUdpRxDone,
    kStateTcpRxListen,
    kStateTcpRx,
    kStateTcpRxDone,
    kStateRxDone,

    kStateTxStart,
    kStateTxArpResolve,

    kStateTcpTxOpen,
    kStateTcpTxConnect,
    kStateTcpTxSegment,
    kStateTcpTxDone,

    kStateUdpTxOpen,
    kStateUdpTxDatagram,
    kStateUdpTxDone

};

/*****************************************************************************/
/* Data types used internally by code in this file.                          */
/*****************************************************************************/

typedef enum {
    kIperfProtoUDP = 1,
    kIperfProtoTCP
} tIperfProto;

typedef struct
{
    UINT32		mInterval;		// -i
    UINT32		mAmount;		// -n
    UINT32		mDuration;		// -t. Default = 10*TICK_SECOND msec
    UINT32		mDatagramSize;	// -l
    UINT16		mMSS;			// -M
    BOOL		mServerMode;    // -s or -c
    BOOL   		mBufLenSet;     // -l
    tIperfProto	mProtocol;		// -b or -u
    UINT16		mServerPort;	// -p

    UINT32		mTxRate;		// -b or
                                // -x: NONE-STANDARD IPERF OPTION. Max Tx bps rate for TCP.

    double			totalLen; // mTotalLen
    long 			pktId; 		// datagramID
    long			lastPktId; // lastDatagramID
    UINT32		errorCount;
    UINT32		outofOrder;

    TCP_SOCKET tcpServerSock;
    TCP_SOCKET tcpClientSock;

    UDP_SOCKET udpSock;

    SOCKET_INFO  remoteSide;
    UINT16		 localPort;

    //struct sockaddr_in remoteAddr;
   // int 		remoteAddrlen;
    //	tWFSocketAddr remoteAddr;

    // Calculated packet period, in msec, to reflect the target bit rate.
    UINT32		mPktPeriod;

    UINT32		startTime;
    UINT32		stopTime;
    UINT32		nextTxTime;
    //UINT32		remoteStartTime;
    //UINT32		remoteStopTime;

    UINT8			nAttempts;
    UINT32		pktCount;
    BOOL			stopRequested;


    UINT32		lastCheckPktCount;  // Check if pktCount changes within mInterval; or kIperfRxTimeOut.
    long			lastCheckPktId;
    UINT32		lastCheckErrorCount;
    UINT32		lastCheckTotalLen;
    UINT32		lastCheckTime;
    UINT8 		statusReported;

//	long		mPendingACK;		// number of outstanding TCP ACKs
//	UINT8		mRetransmit;

    UINT8 		state;

    UINT32      timer;
    UINT16      remainingTxData;
    UINT16      availUDPTxSpace;

    BOOL     	isLastTransmit;


} tAppState;

typedef enum
{
    kIntervalReport,
    kSubtotalReport,
    kSessionReport
} tIperfReport;

//
// Data structure used by iperf protocol
//

#define HEADER_VERSION1 0x80000000

typedef struct
{
    long id;
    UINT32 tv_sec;
    UINT32 tv_usec;
} tIperfPktInfo; 	// In the original Iperf, this is the "UDP_datagram" structure.

// tUDP_datagram
typedef struct
{
    UINT32 flags;
    UINT32 total_len1;
    UINT32 total_len2;
    UINT32 stop_sec;
    UINT32 stop_usec;
    UINT32 error_cnt;
    UINT32 outorder_cnt;
    UINT32 datagrams;
    UINT32 jitter1;
    UINT32 jitter2;
} tServerHdr;

typedef struct
{
    UINT32 flags;
    UINT32 numThreads;
    UINT32 mPort;
    UINT32 bufferlen;
    UINT32 mWinBand;
    UINT32 mAmount;
} tClientHdr;

/*****************************************************************************/
/* Global variables used internally by code in this file.                    */
/*****************************************************************************/

#define MAX_BUFFER   (sizeof(tIperfPktInfo) + sizeof(tServerHdr))
UINT8  g_bfr[ MAX_BUFFER ];


static tAppState gAppState;

#define APPCTX (gAppState)

//
// Helper utilities
//

/* On the Microchip v4.51 stack the MSS is a compile time setting and not within the control */
/* of the application to change on a per socket basis, nor is it even a runtime configurable */
/* setting.  Search tcp.c for TCP_MAX_SEG_SIZE.                                              */
/* However, TCP_MAX_SEG_SIZE is not a public macro.                                          */
/* RFC 879 specifies the default MSS to be 536. Hence we assume this number here.            */
/* (WF_IPERF_TCP_MSS == TCP_MAX_SEG_SIZE) is desirable but not required.                     */

#define WF_IPERF_TCP_MSS  536

static void
ResetIperfCounters(void)
{
    // APPCTX.mAmount = 0;
    // APPCTX.mDuration = 10*1000; // -t: default 10 sec
    // APPCTX.mInterval = 1000; 	// -i: default 1 sec
    APPCTX.mMSS = WF_IPERF_TCP_MSS;
    APPCTX.mDatagramSize = 1470; // -l: default 1470 bytes. UDP datagram size.
    APPCTX.totalLen = 0;
    APPCTX.pktId = 0;
    APPCTX.lastPktId = 0;
    APPCTX.errorCount = 0;
    APPCTX.outofOrder = 0;
    APPCTX.pktCount = 0;
    APPCTX.statusReported = 0;
    APPCTX.startTime = 0;
    APPCTX.stopTime = 0;

    APPCTX.lastCheckPktCount = 0;
    APPCTX.lastCheckPktId = 0;
    APPCTX.lastCheckErrorCount = 0;
    APPCTX.lastCheckTotalLen = 0;
    APPCTX.lastCheckTime = 0;

    APPCTX.isLastTransmit = FALSE;

//	APPCTX.mPendingACK = 0;
//	APPCTX.mRetransmit = 0;

}

static void ascii_to_u32s(INT8 *ptr, UINT32 *values, UINT8 count)
{
    UINT8 i;
    UINT32 tmp;

    // Convert "123.456_78_90k", with count set to 4,  to
    // unsigned 32-bit numbers 123, 456, 78 and 90000, and
    // store them in the values array.

    for (i = 0; i < count; i++)
    {
        tmp = 0;

        while ( (*ptr > (INT8)'9') || (*ptr < (INT8)'0') )
        {
            if ( (*ptr == (INT8)' ') || (*ptr == (INT8)0) ) return; // terminates at blank or NULL.

            ptr++;
        }

        while ( (*ptr <= (INT8)'9') && (*ptr >= (INT8)'0') )
        {
            tmp = tmp*10 + *ptr - '0';
            ptr++;
        }
        if ( (*ptr == (INT8)'k') || (*ptr == (INT8)'K') )
        {
            tmp = tmp * 1000;
            ptr++;
        }
        else if ( (*ptr == (INT8)'m') || (*ptr == (INT8)'M') )
        {
            tmp = tmp * 1000 * 1000;
            ptr++;
        }

        values[i] = tmp;
    }
}

//
// Report bandwith, jitter, and packet loss stastistics.
// Used by in both server and client modes.
//
// Todo: implement the jitter report.
//

static void ReportBW_Jitter_Loss(tIperfReport reportType)
{
    UINT32 nAttempted;
    UINT32 nDropped;
    double kbps;
    UINT32 currentTime;
    UINT32 sec;
	UINT32 msec;

    currentTime = IPERF_GET_MSEC_TICK_COUNT();

    switch ( reportType )
    {
        case kIntervalReport:

            nDropped = APPCTX.errorCount - APPCTX.lastCheckErrorCount;

            // bits-per-msec == Kbps

            sec = (currentTime- APPCTX.lastCheckTime)/TICK_SECOND;
			msec = ((double) (currentTime - APPCTX.lastCheckTime)) / (((double)(TICK_SECOND))/1000);

            if ( APPCTX.state == (UINT8)kStateUdpTxDone )
            {
               nAttempted = (APPCTX.lastPktId - APPCTX.lastCheckPktId) + nDropped;
            }
            else
            {
                nAttempted = APPCTX.pktId - APPCTX.lastCheckPktId;
            }

			if ( msec == 0u )
            {
                kbps = 0;
            }
            else
            {
				kbps = ((APPCTX.totalLen - APPCTX.lastCheckTotalLen)*((double) 8)) / msec;
            }

            sec = (APPCTX.lastCheckTime - APPCTX.startTime)/TICK_SECOND;

            sprintf( (char *) g_ConsoleContext.txBuf, "    - [0.0- %lu.%lu sec] %3lu/ %3lu (%2lu%%)    %4lu Kbps",
                             (unsigned long)(msec/1000),
                             (unsigned long)((msec%1000)/100),
                      (unsigned long)nDropped,
                      (unsigned long)nAttempted,
                             (nAttempted == 0u) ? 0 : ((unsigned long)nDropped*100/(unsigned long)nAttempted),
                      (unsigned long) (kbps + ((double) 0.5)));
            WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

            break;


        case kSubtotalReport:
            // intentional fall-through
        case kSessionReport:

           nDropped = APPCTX.errorCount;

           if (APPCTX.state == (UINT8)kStateUdpTxDone)
            {
               nAttempted = APPCTX.lastPktId + nDropped;
            }
            else
            {
                nAttempted = APPCTX.lastPktId;
            }

			msec = ((double) (APPCTX.stopTime - APPCTX.startTime)) / (((double)(TICK_SECOND))/1000);

			if ( msec == 0u )
            {
                kbps = 0;
            }
            else
            {
   				kbps = (APPCTX.totalLen * ((double) 8)) / msec;
            }

            sprintf( (char *) g_ConsoleContext.txBuf, "    - [0.0- %lu.%lu sec] %3lu/ %3lu (%2lu%%)    %4lu Kbps",
                             (unsigned long)(msec/1000),
                             (unsigned long)((msec%1000)/100),
                             (unsigned long)nDropped,
                             (unsigned long)nAttempted,
                             (nAttempted == 0u) ? 0 : ((unsigned long)nDropped*100/(unsigned long)nAttempted),
                             (unsigned long) (kbps + ((double) 0.5)));

            WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

            break;
    }

    if ( reportType == 	kSessionReport )
    {
      WFConsolePrintRomStr("    Session completed ...", TRUE);
    }

    APPCTX.lastCheckPktId = APPCTX.pktId;
    APPCTX.lastCheckErrorCount = APPCTX.errorCount;
    APPCTX.lastCheckPktCount = APPCTX.pktCount;
    APPCTX.lastCheckTime = currentTime;
    APPCTX.lastCheckTotalLen = APPCTX.totalLen;
}

void IperfAppInit(void)
{

    memset( &APPCTX, 0, sizeof(tAppState) );

    APPCTX.state = kStateStandby;
    APPCTX.stopRequested = FALSE;

    APPCTX.tcpClientSock = INVALID_SOCKET;
    APPCTX.tcpServerSock = INVALID_SOCKET;
    APPCTX.udpSock = INVALID_SOCKET;

}

static BOOL IperfAppKillRequested(void)
{
    UINT8 argc;
    INT8 **argv;

    // Did user ask to "kill iperf"?


    if ( WFConsoleIsConsoleMsgReceived() == FALSE ) return FALSE;

    argv = WFConsoleGetCmdLineArgv();
    argc = WFConsoleGetCmdLineArgc();

    if (argc == 2u)
    {
        if ((memcmppgm2ram(argv[0], "kill",  4) == 0) &&
           (memcmppgm2ram(argv[1],  "iperf", 5) == 0))
        {
           //WFCliServicePerformed();
           WFConsoleReleaseConsoleMsg();

           APPCTX.stopRequested = TRUE;
           return TRUE;
        }
    }
    WFConsolePrintRomStr("Invalid commmand sequence", TRUE);
    WFConsoleReleaseConsoleMsg();
    return FALSE;

}


static void StateMachineStandby(void)
{
    //tWFCliArguments *args;
    UINT8 argc;
    INT8 **argv;
    UINT8 i;
    INT8 *ptr;
    UINT32 values[4];
    UINT16 payloadSize;
    float pktRate;
    
    //if ( WFCliServiceRequested() == FALSE ) return;
    //if ( !(args = WFCliGetArguments()) ) return;


    if ( WFConsoleIsConsoleMsgReceived() == FALSE ) return;
    
    argv = WFConsoleGetCmdLineArgv();
    argc = WFConsoleGetCmdLineArgc();   // needed because WFConsoleGetCmdLineTokens() returns 0 for argc when in -Os 


    if ( (argc == 2u)                             &&
         (memcmppgm2ram(argv[0], "kill", 4) == 0) &&
         (memcmppgm2ram(argv[1], "iperf", 5) == 0) )
    {
       WFConsolePrintRomStr("Iperf not started for the kill...", TRUE);
       WFConsoleReleaseConsoleMsg();
       return;
    }

    if ((memcmppgm2ram(argv[0], "kill", 4) == 0) && (argc == 1u))
    {
        WFConsolePrintRomStr("'kill iperf' is the only valid kill command", TRUE);
        WFConsoleReleaseConsoleMsg();
    }



    if ( (argc == 2u)                             &&
         (memcmppgm2ram(argv[0], "kill", 4) == 0) &&
         (memcmppgm2ram(argv[1], "iperf", 5) != 0) )
    {
       WFConsolePrintRomStr("Invalid command sequence...", TRUE);
       WFConsoleReleaseConsoleMsg();
       return;
    }


    if ( memcmppgm2ram(argv[0], "iperf", 5) ) return;

    // OK, we will handle the "iperf" command.

    APPCTX.mServerMode = FALSE;
    APPCTX.mProtocol = kIperfProtoTCP;   			// default is TCP mode.
    APPCTX.stopRequested = FALSE;

    APPCTX.mServerPort = IPERF_APP_SERVER_PORT;		// -p. default: server port 5001



    APPCTX.mTxRate = ((UINT32) 500)*((UINT32) 1000);		// -b or -x. Target tx rate.
    // KS: default tx rate for iperf is actually 1Mbps. Here we set it to 500Kbps instead.

    APPCTX.mAmount = 0;			// -n: default 0.
    APPCTX.mDuration = ((UINT32) 10)*((UINT32) TICK_SECOND); // -t: default 10 sec.
    APPCTX.mInterval =  TICK_SECOND; 	// -i: default 1 sec.

    // Initialize statistics

    ResetIperfCounters();

    WF_ASSERT(argc != 0);
    for (i = 1; i < argc; i++)
    {
        if ((memcmppgm2ram(argv[i], "-s", 2) == 0) ||
            (memcmppgm2ram(argv[i], "--server", 5) == 0) )
        {
            // Function as an iperf server.

            APPCTX.mServerMode = TRUE;
        }
        else if ((memcmppgm2ram(argv[i], "-u", 2) == 0) ||
                 (memcmppgm2ram(argv[i], "--udp", 5) == 0) )
        {
            // iperf UDP mode.
            APPCTX.mProtocol = kIperfProtoUDP;
        }
        else if ((memcmppgm2ram(argv[i], "-b", 2) == 0) ||
                 (memcmppgm2ram(argv[i], "--bandwidth", 5) == 0) )
        {
            // iperf UDP mode.

            APPCTX.mProtocol = kIperfProtoUDP;

            // Next argument should be the target rate, in bps.

            i++;
            ptr = argv[i];

            ascii_to_u32s(ptr, values, 1);

            APPCTX.mTxRate = values[0];
        }
        else if ((memcmppgm2ram(argv[i], "-x", 2) == 0) ||
                 (memcmppgm2ram(argv[i], "--xmitrate", 5) == 0) )
        {
            // NON-STANDARD IPERF OPTION. Set the max TCP tx rate.
            // Next argument should be the target rate, in bps.

            i++;
            ptr = argv[i];

            ascii_to_u32s(ptr, values, 1);

            APPCTX.mTxRate = values[0];
        }
        else if ((memcmppgm2ram(argv[i], "-c", 2) == 0) ||
                 (memcmppgm2ram(argv[i], "--client", 5) == 0) )
        {
            // Function as an iperf client.

            APPCTX.mServerMode = FALSE;

            // Next argument should be the server IP, such as "192.168.1.100".

            i++;
            ptr = argv[i];

            ascii_to_u32s(ptr, values, 4);

            APPCTX.remoteSide.remote.IPAddr.v[0] = values[0];
            APPCTX.remoteSide.remote.IPAddr.v[1] = values[1];
            APPCTX.remoteSide.remote.IPAddr.v[2] = values[2];
            APPCTX.remoteSide.remote.IPAddr.v[3] = values[3];

        }
        else if ((memcmppgm2ram(argv[i], "-t", 2) == 0) ||
                 (memcmppgm2ram(argv[i], "--time", 5) == 0) )
        {
            // Next argument should be the (client tx) duration, in seconds.

            i++;
            ptr = argv[i];

            ascii_to_u32s(ptr, values, 1);

            APPCTX.mDuration = values[0]*TICK_SECOND;
            APPCTX.mAmount = 0;
        }
        else if ((memcmppgm2ram(argv[i], "-n", 2) == 0) ||
                 (memcmppgm2ram(argv[i], "--num", 5) == 0) )
        {
            // Next argument should be the (client tx) size, in bytes.

            i++;
            ptr = argv[i];

            ascii_to_u32s(ptr, values, 1);

            APPCTX.mAmount = values[0];
            APPCTX.mDuration = 0;
        }

    /* On the Microchip v4.51 stack the MSS is a compile time setting and not within the control */
    /* of the application to change on a per socket basis, nor is it even a runtime configurable */
    /* setting.  Search tcp.c for TCP_MAX_SEG_SIZE  */

    //	else if ((memcmppgm2ram(argv[i], "-M", 2) == 0) ||
    //			 (memcmppgm2ram(argv[i], "--mss", 5) == 0) )
    //	{
    //		// Next argument should be the (client tcp tx) MSS size, in bytes.
    //
    //		i++;
    //		ptr = argv[i];
    //
    //		ascii_to_u32s(ptr, values, 1);
    //
    //		APPCTX.mMSS = values[0];
    //	}

        else if ((memcmppgm2ram(argv[i], "-i", 2) == 0) ||
                 (memcmppgm2ram(argv[i], "--interval", 5) == 0) )
        {
            // Next argument should be the report interval, in seconds.

            i++;
            ptr = argv[i];

            ascii_to_u32s(ptr, values, 1);

            APPCTX.mInterval = values[0]*TICK_SECOND; // Convert to msec
        }
        else if ((memcmppgm2ram(argv[i], "-l", 2) == 0) ||
                 (memcmppgm2ram(argv[i], "--len", 5) == 0) )
        {
            // Next argument should be the buffer length, in bytes.
            // This is used as the UDP datagram size.

            i++;
            ptr = argv[i];

            ascii_to_u32s(ptr, values, 1);

            if ( values[0] <  MAX_BUFFER  )
            {
               sprintf( (char *) g_ConsoleContext.txBuf,"The minimum datagram size is %d", (int)MAX_BUFFER);
               WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf, TRUE);

               WFConsoleReleaseConsoleMsg();
               return;
            }

            APPCTX.mDatagramSize = values[0];
        }
    }

    //WFCliServicePerformed();
    WFConsoleReleaseConsoleMsg();

    switch (APPCTX.mServerMode)
    {
        case 0:
            // iperf client

            payloadSize = 	(APPCTX.mProtocol == kIperfProtoUDP) ?
                            APPCTX.mDatagramSize : APPCTX.mMSS;

            pktRate =  (float) (APPCTX.mTxRate / 8) / (float) payloadSize;
            APPCTX.mPktPeriod =  (UINT32) ( (float) TICK_SECOND / pktRate );

            APPCTX.state = kStateTxStart;
            break;

        case 1:
            // iperf server

            WFConsolePrintRomStr("---------------------------------------------------------", TRUE);

            WFConsolePrintRomStr("Server listening on ", FALSE);
            if (APPCTX.mProtocol == kIperfProtoUDP)
            {
                WFConsolePrintRomStr((ROM char *)"UDP", FALSE);
            }    
            else
            {
                WFConsolePrintRomStr((ROM char *)"TCP", FALSE);
            }    
            WFConsolePrintRomStr(" port ", FALSE);
            WFConsolePrintInteger(APPCTX.mServerPort, 'd');
            WFConsolePrintRomStr("", TRUE);
            APPCTX.state = kStateRxStart;
            break;
    }
}

static void
StateMachineRxStart(void)
{
  if ( !APPCTX.mServerMode )
  {
     putrsUART("Unsupported Configuration\n\r");
     APPCTX.state = kStateStandby;
     return;
  }


 switch ( APPCTX.mProtocol )
 {
      case kIperfProtoTCP:	// TCP


            /* TCP Server sockets are allocated for entire runtime duration, a call to disconnect does not free them */
            /* therefore a subsequent N+1 open will fail */
            if ( (APPCTX.tcpServerSock == INVALID_SOCKET) &&
                 (APPCTX.tcpServerSock = TCPOpen(0, TCP_OPEN_SERVER,  APPCTX.mServerPort, TCP_PURPOSE_GENERIC_TCP_SERVER/*TCP_PURPOSE_IPERF_TCP_SERVER*/)) == INVALID_SOCKET )
            {
               /* error case */
               WFConsolePrintRomStr("Create TCP socket failed", TRUE);
               APPCTX.state = kStateStandby;
               return;
            }

            TCPAdjustFIFOSize(APPCTX.tcpServerSock, 1, 0,TCP_ADJUST_PRESERVE_RX | TCP_ADJUST_GIVE_REST_TO_RX);

            APPCTX.state = kStateTcpRxListen;
            break;

      case kIperfProtoUDP:	// UDP

           if ( (APPCTX.udpSock = UDPOpenEx(0, UDP_OPEN_SERVER, APPCTX.mServerPort, 0)) == INVALID_UDP_SOCKET )
           {
               /* error case */
               WFConsolePrintRomStr("Create UDP socket failed", TRUE);
               APPCTX.state = kStateStandby;
               return;
           }

           APPCTX.state = kStateUdpRx;
        break;

      default:
           WFConsolePrintRomStr("Protocol error", TRUE);
           APPCTX.state = kStateStandby;
           return;
  }

}


static void
StateMachineTcpListen(void)
{

   if ( IperfAppKillRequested() == TRUE )
   {
        APPCTX.state = kStateRxDone;
        return;
   }

   if( TCPIsConnected(APPCTX.tcpServerSock) )
   {
      SOCKET_INFO* ptrSocketInfo = TCPGetRemoteInfo( APPCTX.tcpServerSock );
      memcpy ( (void *) &APPCTX.remoteSide, (void *) ptrSocketInfo, sizeof ( SOCKET_INFO) );
      APPCTX.state = kStateTcpRx;

      /* clear the stack's reset flag */
      TCPWasReset(APPCTX.tcpServerSock);
   }
}


static void
StateMachineTcpRx(void)
{
    UINT16 length;

    if( (length = TCPIsGetReady(APPCTX.tcpServerSock)) == 0 )
    {

      if ( TCPWasReset(APPCTX.tcpServerSock)  )
      {
          APPCTX.stopTime = IPERF_GET_MSEC_TICK_COUNT();
          APPCTX.state = kStateTcpRxDone;
          return;
      }

    }
    else
    {
       if ( APPCTX.pktId == 0)
       {
          // This is the first rx pkt.
          putrsUART("\n\r    Session started ...\n\r");

          APPCTX.startTime = IPERF_GET_MSEC_TICK_COUNT();
          APPCTX.lastCheckTime = 	APPCTX.startTime;

          APPCTX.lastCheckPktId = APPCTX.pktId;

          sprintf((char *) g_ConsoleContext.txBuf, "    - Local  %u.%u.%u.%u port %u connected with",
                   AppConfig.MyIPAddr.v[0],
                   AppConfig.MyIPAddr.v[1],
                   AppConfig.MyIPAddr.v[2],
                   AppConfig.MyIPAddr.v[3],
                   APPCTX.mServerPort);

          WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

          sprintf( (char *) g_ConsoleContext.txBuf,"    - Remote %u.%u.%u.%u port %u",
                   APPCTX.remoteSide.remote.IPAddr.v[0],
                   APPCTX.remoteSide.remote.IPAddr.v[1],
                   APPCTX.remoteSide.remote.IPAddr.v[2],
                   APPCTX.remoteSide.remote.IPAddr.v[3],
                   APPCTX.remoteSide.remotePort.Val );

          WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

       }

       APPCTX.pktId++;
       APPCTX.pktCount++;
       APPCTX.lastPktId = APPCTX.pktId;
       APPCTX.totalLen += length;

       /* read the remaining datagram payload */
       /* a UdpDiscard would be disingenuous, because it would not reflect the bandwidth at L7 */
       while ( length > 0 )
       {
          UINT16 chunk;

          if ( length <  (UINT16)MAX_BUFFER )
            chunk = length;
          else
            chunk = MAX_BUFFER;

          TCPGetArray( APPCTX.tcpServerSock, (BYTE*)g_bfr, chunk);
          length -= chunk;
       }

    }

    if ((APPCTX.pktId != (long)0) &&
       (IPERF_GET_MSEC_TICK_COUNT() > (APPCTX.lastCheckTime + APPCTX.mInterval)) )
    {
         // Time to report statistics
         ReportBW_Jitter_Loss(kIntervalReport);
    }

    if ( IperfAppKillRequested() == TRUE )
    {
       APPCTX.state = kStateTcpRxDone;
       APPCTX.stopTime = IPERF_GET_MSEC_TICK_COUNT();

       return;
    }
}


static void
StateMachineUdpRx(void)
{
    UINT16 length =0;
    tIperfPktInfo *pPktInfo;

    // Do nothing if no data is waiting
    if( (length = UDPIsGetReady(APPCTX.udpSock)) >= (UINT16)(sizeof(tIperfPktInfo)) )
    {

       /* The GetArray should not fail... */
       if ( UDPGetArray((BYTE*)g_bfr, sizeof(tIperfPktInfo)) != sizeof(tIperfPktInfo) )
       {
          putrsUART("      UDP Get Array Failed\n\r");
          APPCTX.state = kStateUdpRxDrain;
          return;
       }

       pPktInfo = (tIperfPktInfo *) g_bfr;
       APPCTX.pktId = WFTOHL(pPktInfo->id);

       if ( (APPCTX.pktCount == (UINT32)0) && (APPCTX.pktId < (long)0) )
       {
          // Ignore retransmits from previous session.
          UDPDiscard();
          return;
       }

       APPCTX.pktCount++;
       if (APPCTX.pktCount == (UINT32)1 )
       {
          // The first pkt is used to set up the server,
          // does not count as a data pkt.

          WFConsolePrintRomStr("", TRUE);
          WFConsolePrintRomStr("    Session started ...", TRUE);

          if ( APPCTX.pktId != 0 )
          {
             // We have lost a few packets before the first pkt arrived.
             sprintf((char *) g_ConsoleContext.txBuf, "    - First pkt id = %ld (should be 0)",
                              APPCTX.pktId);

             WFConsolePrintRamStr((char *) g_ConsoleContext.txBuf, TRUE );


             // The first data pkt starts with id = 1.
             APPCTX.errorCount	+= 	APPCTX.pktId - 1;
          }

          APPCTX.lastPktId = APPCTX.pktId;

          sprintf((char *) g_ConsoleContext.txBuf, "    - Local  %u.%u.%u.%u port %u connected with ",
                           AppConfig.MyIPAddr.v[0],
                           AppConfig.MyIPAddr.v[1],
                           AppConfig.MyIPAddr.v[2],
                           AppConfig.MyIPAddr.v[3],
                           APPCTX.mServerPort);

          WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

          sprintf((char *) g_ConsoleContext.txBuf,"    - Remote %u.%u.%u.%u port %u",
                           UDPSocketInfo[APPCTX.udpSock].remote.remoteNode.IPAddr.v[0],
                           UDPSocketInfo[APPCTX.udpSock].remote.remoteNode.IPAddr.v[1],
                           UDPSocketInfo[APPCTX.udpSock].remote.remoteNode.IPAddr.v[2],
                           UDPSocketInfo[APPCTX.udpSock].remote.remoteNode.IPAddr.v[3],
                           UDPSocketInfo[APPCTX.udpSock].remotePort );

          WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

          // Store the remote info so we can send the iperf "UDP-FIN-ACK" msg
          APPCTX.remoteSide.remote.IPAddr.v[0] = UDPSocketInfo[APPCTX.udpSock].remote.remoteNode.IPAddr.v[0];
          APPCTX.remoteSide.remote.IPAddr.v[1] = UDPSocketInfo[APPCTX.udpSock].remote.remoteNode.IPAddr.v[1];
          APPCTX.remoteSide.remote.IPAddr.v[2] = UDPSocketInfo[APPCTX.udpSock].remote.remoteNode.IPAddr.v[2];
          APPCTX.remoteSide.remote.IPAddr.v[3] = UDPSocketInfo[APPCTX.udpSock].remote.remoteNode.IPAddr.v[3];

          APPCTX.remoteSide.remotePort.Val =  UDPSocketInfo[APPCTX.udpSock].remotePort;

          APPCTX.startTime = IPERF_GET_MSEC_TICK_COUNT();
          //APPCTX.remoteStartTime = WFTOHL(pPktInfo->tv_sec);

          APPCTX.lastCheckTime = 	APPCTX.startTime;

          APPCTX.lastCheckPktId = APPCTX.pktId;
          APPCTX.lastCheckPktCount = APPCTX.pktCount;
          APPCTX.lastCheckErrorCount = APPCTX.errorCount;

          UDPDiscard();

          return;

      }

      APPCTX.totalLen += length;

      if ( APPCTX.pktId < 0 )
      {
         // this is the last datagram
         APPCTX.pktId = - APPCTX.pktId;

         APPCTX.stopTime = IPERF_GET_MSEC_TICK_COUNT();
        //APPCTX.remoteStopTime = WFTOHL(pPktInfo->tv_sec);

        APPCTX.nAttempts = 0;
        APPCTX.state = kStateUdpRxDrain;
      }

      if ( APPCTX.pktId != APPCTX.lastPktId+1 )
      {
         if ( APPCTX.pktId < APPCTX.lastPktId+1 )
         {
            APPCTX.outofOrder++;
         }
         else
         {
            APPCTX.errorCount += APPCTX.pktId - (APPCTX.lastPktId+1);
         }
      }

      // never decrease pktId (e.g. if we get an out-of-order packet)
      if ( APPCTX.pktId == APPCTX.lastPktId )
      {
         WFConsolePrintRomStr("      Recv duplicated pkt", TRUE);
      }

      if ( APPCTX.pktId > APPCTX.lastPktId )
      {
         APPCTX.lastPktId = APPCTX.pktId;
      }

      /* read the remaining datagram payload - the full payload */
      /* a UdpDiscard would be disingenuous, because it would not reflect the bandwidth at L7 */
      length -=  sizeof(tIperfPktInfo);
      while ( length > 0 )
      {
         UINT16 chunk;

         if ( length <  (UINT16)MAX_BUFFER )
            chunk = length;
         else
            chunk = MAX_BUFFER;

         UDPGetArray((BYTE*)g_bfr, chunk);
         length -= chunk;
      }


    }  /* end got a datagram */

    if ( (APPCTX.pktCount != (UINT32)0) &&
         (IPERF_GET_MSEC_TICK_COUNT() > (APPCTX.lastCheckTime + APPCTX.mInterval)) )
    {
        if ( APPCTX.pktCount == APPCTX.lastCheckPktCount )
        {
          // No events in APPCTX.mInterval msec, we timed out
          WFConsolePrintRomStr("    Rx timed out", TRUE);

          APPCTX.stopTime = IPERF_GET_MSEC_TICK_COUNT();

          APPCTX.nAttempts = 0;
          APPCTX.state = kStateUdpRxDrain;
        }
        else
        {
          ReportBW_Jitter_Loss(kIntervalReport);
        }
    }

    if ( IperfAppKillRequested() == TRUE )
    {
        APPCTX.state = kStateUdpRxDrain;
        return;
    }

}

static void
StateMachineUdpRxDrain(void)
{
    if( UDPIsGetReady(APPCTX.udpSock) > (WORD)0 )
    {
         UDPDiscard();
         return;
    }

   /* If iperf kill was done, just jump to closing the socket */
   if ( APPCTX.stopRequested )
     APPCTX.state = kStateRxDone;
   else /* go ahead an generate reports, etc */
     APPCTX.state = kStateUdpRxDone;

}


static void
StateMachineUdpRxDone(void)
{
    tIperfPktInfo *pPktInfo;
    tServerHdr *pServer_hdr;
    UINT16  tmp;
    float tmp2;


    if ( APPCTX.statusReported == 0u )
    {
        ReportBW_Jitter_Loss(kSessionReport);
        APPCTX.statusReported = 1;
    }

    /* Drain any waiting pkts */
    if (  UDPIsGetReady(APPCTX.udpSock)  )
    {
        UDPDiscard();
        return;
    }

    // Send the iperf UDP "FIN-ACK" 10 times.
    if ( APPCTX.nAttempts++ > 10u )
    {
        APPCTX.state = kStateRxDone;
        return;
    }

     /* Make sure space is available to TX the ACK packet of 128 bytes */
    if ( UDPIsPutReady(APPCTX.udpSock ) > 0u )
    {

      pPktInfo = (tIperfPktInfo *) g_bfr;
      pServer_hdr = (tServerHdr *) (pPktInfo +1);

      pPktInfo->id = HTOWFL( -APPCTX.lastPktId );
      pPktInfo->tv_sec = 0;
      pPktInfo->tv_usec = 0;

      pServer_hdr->flags = HTOWFL(HEADER_VERSION1);
      pServer_hdr->total_len1 = 0;
      pServer_hdr->total_len2 = HTOWFL( (UINT32) APPCTX.totalLen);

      pServer_hdr->stop_sec =  HTOWFL( (UINT32) (APPCTX.stopTime - APPCTX.startTime)/TICK_SECOND);

      /* get the remainder of the ticks using modulus */
      tmp2 = ((APPCTX.stopTime - APPCTX.startTime)%TICK_SECOND);

      /* normalize  to uSecs */
      tmp2 =  TickConvertToMilliseconds( tmp2 );
      tmp2 *= 1000;   /* 1000 uSecs per mSec */


      pServer_hdr->stop_usec = HTOWFL( (UINT32) tmp2 );
      pServer_hdr->error_cnt = HTOWFL( (UINT32)  APPCTX.errorCount);;
      pServer_hdr->outorder_cnt = HTOWFL( (UINT32) APPCTX.outofOrder);
      pServer_hdr->datagrams = HTOWFL( (UINT32) APPCTX.lastPktId);
      pServer_hdr->jitter1 = 0;
      pServer_hdr->jitter2 = 0;

      UDPPutArray((BYTE*)g_bfr, MAX_BUFFER);

      for ( tmp=0; tmp < (128 - MAX_BUFFER); tmp++)
        UDPPut(0x00);

      UDPFlush( );

    }

}

static void
StateMachineTcpRxDone(void)
{
    if ( APPCTX.statusReported == 0u )
    {
        ReportBW_Jitter_Loss(kSessionReport);
        APPCTX.statusReported = 1;
    }

    APPCTX.state = kStateRxDone;
}

static void
StateMachineRxDone(void)
{

   if ( APPCTX.mProtocol == kIperfProtoUDP )
   {
     UDPClose(  APPCTX.udpSock );
   }
   else
   {

     /* 2 disconnects on purpose not mistake */
     TCPDisconnect( APPCTX.tcpServerSock );
     TCPDisconnect( APPCTX.tcpServerSock );
   }

    WFConsolePrintRomStr("", TRUE);
    WFConsolePrintRomStr("    Rx done. Socket closed.", TRUE);

    // Clear statistics
    ResetIperfCounters();

    // In server mode, continue to accept new session requests ...

    if ((APPCTX.mServerMode == TRUE) 	&&
        (APPCTX.stopRequested == FALSE) )
    {
        WFConsolePrintRomStr("    Ready for the next session.", TRUE);

        APPCTX.state = kStateRxStart;
    }
    else
    {
        WFConsolePrintRomStr("    Iperf completed.", TRUE);
        APPCTX.state = kStateStandby;
    }

}




/******************************/
/* TX CLIENT CODE BEGINS HERE */
/******************************/


static void
StateMachineTxStart(void)
{

   ARPResolve(&APPCTX.remoteSide.remote.IPAddr);
   APPCTX.state = kStateTxArpResolve;
   APPCTX.timer = TickGet();

}

static void
StateMachineTxArpResolve(void)
{

  if ( IperfAppKillRequested() == TRUE )
  {
     WFConsolePrintRomStr("Iperf client session closed.", TRUE);
     APPCTX.state = kStateStandby;
     return;
  }

  if(!ARPIsResolved(&APPCTX.remoteSide.remote.IPAddr, &APPCTX.remoteSide.remote.MACAddr))
  {
      /* every 3 seconds print a warning */
      if( TickGet()- APPCTX.timer > 5*TICK_SECOND )
      {
         WFConsolePrintRomStr("ARP unable to resolve the MAC address of remote side.", TRUE);
         APPCTX.timer = TickGet();
      }
      return;
  }

  if ( APPCTX.mProtocol == kIperfProtoUDP )
     APPCTX.state = kStateUdpTxOpen;
  else
     APPCTX.state = kStateTcpTxOpen;

}

static void
StateMachineUDPTxOpen(void)
{
    if ( (APPCTX.udpSock = UDPOpenEx((DWORD)((unsigned int)APPCTX.remoteSide.remote.IPAddr.v),UDP_OPEN_NODE_INFO, 0, APPCTX.mServerPort)) == INVALID_UDP_SOCKET )
    {
        /* error case */
        WFConsolePrintRomStr("Create UDP socket failed", TRUE);
        APPCTX.state = kStateStandby;
        return;
    }


    APPCTX.localPort = UDPSocketInfo[APPCTX.udpSock].localPort;

    WFConsolePrintRomStr("---------------------------------------------------------", TRUE);
    sprintf( (char *) g_ConsoleContext.txBuf,
             "Client connecting to %u.%u.%u.%u, UDP port %u",
              APPCTX.remoteSide.remote.IPAddr.v[0],
              APPCTX.remoteSide.remote.IPAddr.v[1],
              APPCTX.remoteSide.remote.IPAddr.v[2],
              APPCTX.remoteSide.remote.IPAddr.v[3],
              APPCTX.mServerPort );

    WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

    APPCTX.state = kStateUdpTxDatagram;

    APPCTX.startTime = IPERF_GET_MSEC_TICK_COUNT();

     // Wait for a few seconds before first TCP tx, so we can resolve ARP.
    APPCTX.nextTxTime = APPCTX.startTime + APPCTX.mPktPeriod;

}

static void
StateMachineTCPTxOpen(void)
{

   if  ( (APPCTX.tcpClientSock = TCPOpen( (DWORD) ((unsigned int)&APPCTX.remoteSide.remote), TCP_OPEN_NODE_INFO,  APPCTX.mServerPort, TCP_PURPOSE_GENERIC_TCP_CLIENT/*TCP_PURPOSE_IPERF_TCP_CLIENT*/)) == INVALID_SOCKET )
   {
       /* error case */
        WFConsolePrintRomStr("Create TCP socket failed", TRUE);
        APPCTX.state = kStateStandby;
        return;
    }

    APPCTX.localPort = LOCAL_PORT_START_NUMBER;

    WFConsolePrintRomStr("---------------------------------------------------------", TRUE);
    sprintf( (char *) g_ConsoleContext.txBuf,
             "Client connecting to %u.%u.%u.%u, TCP port %u",
              APPCTX.remoteSide.remote.IPAddr.v[0],
              APPCTX.remoteSide.remote.IPAddr.v[1],
              APPCTX.remoteSide.remote.IPAddr.v[2],
              APPCTX.remoteSide.remote.IPAddr.v[3],
              APPCTX.mServerPort );

    WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

    APPCTX.state =  kStateTcpTxConnect;

    TCPAdjustFIFOSize(APPCTX.tcpClientSock, 10, 1,TCP_ADJUST_GIVE_REST_TO_TX);

    APPCTX.timer = TickGet();
}


static void
StateMachineTCPTxConnect(void)
{

 if ( IperfAppKillRequested() == TRUE )
 {
     APPCTX.state = kStateTcpTxDone;
     return;
 }

  if( !TCPIsConnected(APPCTX.tcpClientSock) )
  {

    // Time out if too much time is spent in this state
    if(TickGet()- APPCTX.timer > 5*TICK_SECOND)
    {
       TCPDisconnect(APPCTX.tcpClientSock);
       APPCTX.tcpClientSock = INVALID_SOCKET;
       WFConsolePrintRomStr("TCP Client connection timeout", TRUE);
       APPCTX.state = kStateTcpTxDone;
    }

    return;
  }

  /* reset the reset flag so we don't get a FALSE positive */
  TCPWasReset(APPCTX.tcpClientSock);

  APPCTX.startTime = IPERF_GET_MSEC_TICK_COUNT();
  APPCTX.nextTxTime = APPCTX.startTime + APPCTX.mPktPeriod;
  APPCTX.state = kStateTcpTxSegment;
}



static void
GenericTxHeaderPreparation(UINT8 *pData, BOOL isTheLastTransmit)
{
    tIperfPktInfo *pPktInfo = NULL;
    UINT32 currentTime;
    tClientHdr *pClientHdr = NULL;
    long tmp2;

    if ( APPCTX.pktId == 0 ) {
        // The first tx packet
    }

    switch ( APPCTX.mProtocol )
    {
        case kIperfProtoTCP: // TCP

            // Required by iperf.
            pClientHdr = (tClientHdr *) pData;

            // We borrow the same tIperfPktInfo structure to embed
            // some useful (non-standard iperf) meta info.
            // Unfortunately, the order has to be reversed.

            pPktInfo = (tIperfPktInfo *) (pClientHdr + 1);

            break;

        case kIperfProtoUDP: // UDP

            // Both are required by iperf.

            pPktInfo = (tIperfPktInfo *) pData;
            pClientHdr = (tClientHdr *) (pPktInfo + 1);

            break;
    }

    // Client header:
    // Needed for all UDP packets.
    // For TCP, only the first two segments need this info. However,
    // there seems to be no harm to put it to all segments though.

    pClientHdr->flags = HTOWFL( (UINT32) 0);
    pClientHdr->numThreads = HTOWFL((UINT32) 1);
    pClientHdr->mPort = HTOWFL((UINT32) APPCTX.mServerPort);
    pClientHdr->bufferlen = HTOWFL( (UINT32) 0);
    pClientHdr->mWinBand = HTOWFL(APPCTX.mTxRate);

    if ( APPCTX.mAmount != 0u )
    {
        pClientHdr->mAmount = HTOWFL(APPCTX.mAmount);
    }
    else
    {
        pClientHdr->mAmount = HTOWFL( - (long) (APPCTX.mDuration/10) );
    }

    // Additional info: needed for UDP only.
    // No harm to put it to all TCP segments though.

    if ( isTheLastTransmit == TRUE )
    {
        // The last UDP tx packet. Some caveats:
        // 1. Iperf uses a negative Id for the last tx packet.
        // 2. Its id should not change during retransmit.

        pPktInfo->id = - ( (long) (APPCTX.pktId - APPCTX.nAttempts) );
    }
    else
    {
        pPktInfo->id = APPCTX.pktId;
    }

    pPktInfo->id = HTOWFL(pPktInfo->id);

    currentTime = IPERF_GET_MSEC_TICK_COUNT();

    pPktInfo->tv_sec = HTOWFL(currentTime / TICK_SECOND);

    /* get the remainder of the ticks using modulus */
    tmp2 = ((APPCTX.stopTime - APPCTX.startTime)%TICK_SECOND);

    /* normalize  to uSecs */
    tmp2 =  TickConvertToMilliseconds( tmp2 );
    tmp2 *= 1000;   /* 1000 uSecs per mSec */


    pPktInfo->tv_usec = HTOWFL( tmp2 );

    return;
}



static BOOL
GenericTxStart(void)
{
    UINT32 currentTime;
    BOOL iperfKilled;

    currentTime = IPERF_GET_MSEC_TICK_COUNT();

    if ( currentTime < (APPCTX.nextTxTime - kIperfTimingErrorMargin))
    {
        // Wait until we are scheduled to Tx.
        return FALSE;
    }

    iperfKilled = IperfAppKillRequested();

    if ((iperfKilled == TRUE) ||
        ((APPCTX.mDuration != 0u) &&
         (currentTime > (APPCTX.startTime + APPCTX.mDuration))) ||
        ((APPCTX.mAmount != 0u) &&
         (APPCTX.totalLen > APPCTX.mAmount)))
    {
        // Prepare to transmit the last packet.
        // Although the last packet needs to be retransmitted kIperfUDPFinRetransmitCount times,
        // if we are in UDP mode.

         APPCTX.isLastTransmit = TRUE;
    }

    if ( APPCTX.pktId == 0 )
    {
        // The first pkt is going out ...

        // Reset startTime, to get a more accurate report.

        APPCTX.startTime = currentTime;
        APPCTX.nextTxTime = APPCTX.startTime;

        APPCTX.lastCheckTime = 	APPCTX.startTime;

        APPCTX.lastCheckPktId = APPCTX.pktId;
        APPCTX.lastCheckPktCount = APPCTX.pktCount;
        APPCTX.lastCheckErrorCount = APPCTX.errorCount;
        APPCTX.nAttempts = 0;
    }

    if ( APPCTX.mProtocol == kIperfProtoTCP )
    {
       /* Manage socket */
       if( TCPIsGetReady(APPCTX.tcpClientSock) > 0u )
       {
          TCPDiscard(APPCTX.tcpClientSock);
          return FALSE;
       }

       if ( TCPWasReset(APPCTX.tcpClientSock) )
       {
         // We don't close the socket. We wait for user to "kill iperf" explicitly.
         WFConsolePrintRomStr("", TRUE);
         WFConsolePrintRomStr("    Warning, TCP server disconnect detected", TRUE);
       }

       if  (( TCPIsPutReady(APPCTX.tcpClientSock) <= APPCTX.mMSS ) && (!iperfKilled))
          return FALSE;

    }
    else
    {
       /* Manage socket */
       if( UDPIsGetReady(APPCTX.udpSock) > 0u )
       {
          UDPDiscard();
          return FALSE;
       }

       if ( UDPIsPutReady(APPCTX.udpSock) <= APPCTX.mDatagramSize )
       {
          return FALSE;
       }

    }


    // One Tx per mPktPeriod msec.
    APPCTX.nextTxTime = currentTime + APPCTX.mPktPeriod;

    GenericTxHeaderPreparation(g_bfr, APPCTX.isLastTransmit);

    if ( APPCTX.mProtocol == kIperfProtoUDP )
    {

        APPCTX.remainingTxData = (APPCTX.mDatagramSize - MAX_BUFFER);

        if ( UDPPutArray(g_bfr, MAX_BUFFER) != MAX_BUFFER )
        {
            WFConsolePrintRomStr("Socket send failed", TRUE);
            APPCTX.errorCount++;
            return FALSE;
        }

    }
    else
    {

        APPCTX.remainingTxData = (APPCTX.mMSS - MAX_BUFFER);

        if (( TCPPutArray(APPCTX.tcpClientSock, (BYTE*) g_bfr, MAX_BUFFER) != MAX_BUFFER ) && (!iperfKilled))
        {
            WFConsolePrintRomStr("Socket send failed", TRUE);
            APPCTX.errorCount++;
            return FALSE;
        }

    }

     return TRUE;

}


/* This routine does a piecewise send, because the entire RAM buffer may not be available for putArray */
static void
TcpTxFillSegment(void)
{
  UINT16 chunk;

  /* Fill the buffer with ASCII char T */
  memset( g_bfr, 0x54, MAX_BUFFER);

  while( APPCTX.remainingTxData > 0u )
  {
    chunk = MAX_BUFFER;

    /* finish case where we get more than is needed */
    if ( APPCTX.remainingTxData < MAX_BUFFER )
      chunk = APPCTX.remainingTxData;

    APPCTX.remainingTxData -= chunk;

    if ( TCPPutArray( APPCTX.tcpClientSock, (BYTE *) g_bfr, chunk) != chunk )
      return;

  }

}

/* This routine does a piece wis send, because the entire RAM pkt buffer may not be available for putArray */
static void
UdpTxFillDatagram(void)
{

  UINT16 chunk;

  /* Fill the buffer with ASCII char U */
  memset( g_bfr, 0x55, MAX_BUFFER);

  while( APPCTX.remainingTxData > 0u )
  {
    chunk = MAX_BUFFER;

    /* finish case where we get more than is needed */
    if ( APPCTX.remainingTxData < MAX_BUFFER )
      chunk = APPCTX.remainingTxData;

    APPCTX.remainingTxData -= chunk;

    if (  UDPPutArray( (BYTE *) g_bfr, chunk) != chunk )
     return;

  }

}



static void
GenericTxEnd(void)
{

  if(  APPCTX.remainingTxData  > 0u )
  {
     /* unhandled error */
     WFConsolePrintRomStr("Socket send failed", TRUE);
     APPCTX.errorCount++;
  }
  else
  {
     // send successful.

     if ( APPCTX.pktCount == 0u )
     {
        // first tx pkt

        putrsUART("\n\r    Session started ...\n\r");


        sprintf( (char *) g_ConsoleContext.txBuf,
                 "    - Local  %u.%u.%u.%u port %u connected with",
                 AppConfig.MyIPAddr.v[0],
                 AppConfig.MyIPAddr.v[1],
                 AppConfig.MyIPAddr.v[2],
                 AppConfig.MyIPAddr.v[3],
                 APPCTX.localPort);

        WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE );

        sprintf( (char *) g_ConsoleContext.txBuf,
                 "    - Remote %u.%u.%u.%u port %u",
                 APPCTX.remoteSide.remote.IPAddr.v[0],
                 APPCTX.remoteSide.remote.IPAddr.v[1],
                 APPCTX.remoteSide.remote.IPAddr.v[2],
                 APPCTX.remoteSide.remote.IPAddr.v[3],
                 APPCTX.mServerPort );

        WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE );

        sprintf( (char *) g_ConsoleContext.txBuf, "    - Target rate = %ld bps, period = %ld ms",
                                        (unsigned long)APPCTX.mTxRate, 
                                        (unsigned long)TickConvertToMilliseconds(APPCTX.mPktPeriod) );

        WFConsolePrintRamStr( (char *) g_ConsoleContext.txBuf , TRUE);

     }

     APPCTX.pktId++;
     APPCTX.pktCount++;

     if ( APPCTX.mProtocol == kIperfProtoUDP )
     {
        APPCTX.totalLen += APPCTX.mDatagramSize;
     }
     else
     {

        APPCTX.totalLen += APPCTX.mMSS;
     }


  }

  APPCTX.lastPktId = APPCTX.pktId - 1;



  if ( IPERF_GET_MSEC_TICK_COUNT() > (APPCTX.lastCheckTime + APPCTX.mInterval - kIperfTimingErrorMargin) )
  {
        // Time to report statistics
        ReportBW_Jitter_Loss(kIntervalReport);

        //APPCTX.lastCheckPktCount = APPCTX.pktCount;
  }


  if ( APPCTX.isLastTransmit == TRUE )
  {
      if ((APPCTX.mProtocol == (tIperfProto)kIperfProtoUDP) &&
          (++APPCTX.nAttempts < kIperfUDPFinRetransmitCount) ) {

          if ( APPCTX.nAttempts == 1u )
          {
              // So the normal pkt statistics is not mixed with the retransmited last pkt.
              APPCTX.stopTime = IPERF_GET_MSEC_TICK_COUNT();

              ReportBW_Jitter_Loss(kSubtotalReport);
              WFConsolePrintRomStr("    -----------------------------------------", TRUE);
          }


          // Don't follow the same transmision rate during retransmit.
          APPCTX.mPktPeriod = kIperfUDPFinRetransmitPeriod;
      }
      else
      {
          APPCTX.state = (APPCTX.mProtocol == kIperfProtoUDP) ? kStateUdpTxDone : kStateTcpTxDone;
          APPCTX.stopTime = IPERF_GET_MSEC_TICK_COUNT();
      }
  }

}

static void
StateMachineTcpTxSegment(void)
{

    if ( GenericTxStart() == TRUE )
    {
       TcpTxFillSegment();
       TCPFlush(APPCTX.tcpClientSock);
       GenericTxEnd();
    }
}

static void
StateMachineUdpTxDatagram(void)
{

    if ( GenericTxStart() == TRUE )
    {
       UdpTxFillDatagram();
       UDPFlush();
       GenericTxEnd();
    }
}


static void
GenericTxDone(void)
{
    if ( APPCTX.statusReported == 0u )
    {
        ReportBW_Jitter_Loss(kSessionReport);
        APPCTX.statusReported = 1;
    }

    APPCTX.state = kStateStandby;

    WFConsolePrintRomStr("    Tx done. Socket closed.", TRUE);

    // Clear statistics
    ResetIperfCounters();

    //WFConsolePrintRomStr("    Back to standby mode.", TRUE);
    WFConsolePrintRomStr("    Iperf completed.", TRUE);

}


static void
StateMachineTcpTxDone(void)
{
    GenericTxDone();

    //No calling this API twice is NOT redundant...  The 2nd time forces a RST per Microchip's own doc for v4.51
    TCPDisconnect(APPCTX.tcpClientSock);
    TCPDisconnect(APPCTX.tcpClientSock);
    APPCTX.tcpClientSock = INVALID_SOCKET;
}

static void
StateMachineUdpTxDone(void)
{

    GenericTxDone();

    UDPClose(APPCTX.udpSock );
}


void IperfAppCall(void)
{
    UINT8 argc;
    INT8 **argv;

	if ( WFConsoleIsConsoleMsgReceived() == TRUE )
	{
       	argv = WFConsoleGetCmdLineArgv();
       	argc = WFConsoleGetCmdLineArgc();

		if ( memcmppgm2ram(argv[0], "help", 4) == 0 )
		{
            WFConsolePrintRomStr("iperf\t\tsee documentation", TRUE);
            WFConsolePrintRomStr("kill iperf\tstop the running iperf session", TRUE);
		}
	    /* This check is for duplicate iperf app calls */
    	/* the only legitimate state that is ready for */
   		/* iperf server or client is standby           */
		else if ( APPCTX.state != (UINT8)kStateStandby )
		{
        	if ( memcmppgm2ram(argv[0], "iperf", 5) == 0 )
        	{
            	WFConsolePrintRomStr("", TRUE);
            	WFConsolePrintRomStr("Error, Iperf session already started", TRUE);
            	WFConsoleReleaseConsoleMsg();
        	}
		}
	}

    switch ( APPCTX.state )
    {
        case kStateStandby:

            StateMachineStandby();

            break;

        /********************/
        /* RX Client States */
        /********************/

        case kStateRxStart:

            StateMachineRxStart();

            break;

        case kStateUdpRx:

            StateMachineUdpRx();

            break;

        case kStateUdpRxDrain:

            StateMachineUdpRxDrain();

            break;


        case kStateUdpRxDone:

            StateMachineUdpRxDone();

            break;


        case kStateTcpRxListen:

            StateMachineTcpListen();

            break;

        case kStateTcpRx:

            StateMachineTcpRx();

            break;

        case kStateTcpRxDone:

            StateMachineTcpRxDone();

            break;

        case kStateRxDone:

            StateMachineRxDone();

            break;


       /********************/
       /* TX Client states */
       /********************/


        case kStateTxStart:

            StateMachineTxStart();

            break;


        case kStateTxArpResolve:

           StateMachineTxArpResolve();

           break;

        case kStateUdpTxOpen:

            StateMachineUDPTxOpen();

            break;

        case kStateTcpTxOpen:

            StateMachineTCPTxOpen();

            break;

        case kStateTcpTxConnect:

            StateMachineTCPTxConnect();

            break;

        case kStateTcpTxSegment:

            StateMachineTcpTxSegment();

            break;

        case kStateUdpTxDatagram:

            StateMachineUdpTxDatagram();

            break;

        case kStateTcpTxDone:

            StateMachineTcpTxDone();

            break;


        case kStateUdpTxDone:

            StateMachineUdpTxDone();

            break;

        }
}

#endif /* WF_CONSOLE */
