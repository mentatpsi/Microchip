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
 * Processor:       PIC32
 * Compiler:        Microchip C32 v1.05 or higher
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Elliott Wood     	6/18/07	    Original
 * Brad Rex             04 Apr 2010 Updated for MRF24WB0M
 ********************************************************************/
#define __CUSTOMHTTPAPP_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_HTTP2_SERVER)

#include "TCPIP Stack/TCPIP.h"
#include "MainDemo.h"		// Needed for SaveAppConfig() prototype
#include "TCPIP Stack/WFConsoleMsgHandler.h"
#include "WirelessProtocols/MCHP_API.h"
#include "Storage.h"

/****************************************************************************
  Section:
	Function Prototypes and Memory Globalizers
  ***************************************************************************/
#if defined(HTTP_USE_POST)
	#if defined(USE_LCD)
		static HTTP_IO_RESULT HTTPPostLCD(void);
	#endif
	#if defined(STACK_USE_HTTP_MD5_DEMO)
		#if !defined(STACK_USE_MD5)
			#error The HTTP_MD5_DEMO requires STACK_USE_MD5
		#endif
		static HTTP_IO_RESULT HTTPPostMD5(void);
	#endif
	#if defined(STACK_USE_EZ_CONFIG)
	    static HTTP_IO_RESULT HTTPPostWifiConfig(void);
    #endif
	#if defined(STACK_USE_HTTP_APP_RECONFIG)
		extern APP_CONFIG AppConfig;
		static HTTP_IO_RESULT HTTPPostConfig(void);
		#if defined(STACK_USE_SNMP_SERVER)
		static HTTP_IO_RESULT HTTPPostSNMPCommunity(void);
		#endif
	#endif
	#if defined(STACK_USE_HTTP_EMAIL_DEMO) || defined(STACK_USE_SMTP_CLIENT)
		#if !defined(STACK_USE_SMTP_CLIENT)
			#error The HTTP_EMAIL_DEMO requires STACK_USE_SMTP_CLIENT
		#endif
		static HTTP_IO_RESULT HTTPPostEmail(void);
	#endif
	#if defined(STACK_USE_DYNAMICDNS_CLIENT)
		static HTTP_IO_RESULT HTTPPostDDNSConfig(void);
	#endif
#endif

// bss descriptor fetched from on-chip
static tWFScanResult bssDesc;
static BOOL bssDescIsValid = FALSE;

// MiWi Scan Information
static BYTE miwiCmdFlag = 0;
static BYTE num_nodes = 0;
static BYTE nodeIdx = 0;
static BYTE SeqNum = 0;
static BOOL flashON = FALSE;

// RAM allocated for DDNS parameters
#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	static BYTE DDNSData[100];
#endif

// Sticky status message variable.
// This is used to indicated whether or not the previous POST operation was 
// successful.  The application uses these to store status messages when a 
// POST operation redirects.  This lets the application provide status messages
// after a redirect, when connection instance data has already been lost.

// Stick status message variable.  See lastSuccess for details.
static BOOL lastFailure = FALSE;

#if (MY_DEFAULT_NETWORK_TYPE == WF_SOFT_AP)
extern tWFScanResult preScanResult[50];
#endif

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
	// If the filename begins with the folder "protect", then require auth
	if(memcmppgm2ram(cFile, (ROM void*)"protect", 7) == 0)
		return 0x00;		// Authentication will be needed later

	// If the filename begins with the folder "snmp", then require auth
	if(memcmppgm2ram(cFile, (ROM void*)"snmp", 4) == 0)
		return 0x00;		// Authentication will be needed later

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
	BYTE *ptr, *ptr1, *ptr2, *ptr3, *temp;
	BYTE filename[32];
	
	BYTE code;
    WORD scan_chnl, channel, panId;
    WORD tmp = 0xFFFF;
    ROOM_RECORD roomInfo;

    //BYTE bssIdx;
    //WORD_VAL bssIdxStr;

	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, 30);
	
	
    if(!memcmppgm2ram(filename, "wifi.xml", 8))
   
	{
		ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"scan");
                //ptr1 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"getBSS");

                //Old Code
            if (ptr != NULL)
		{
                  
                    if(IS_SCAN_IN_PROGRESS(SCANCXT.scanState))
                        return HTTP_IO_WAITING;

                    if(IS_SCAN_STATE_DISPLAY(SCANCXT.scanState))
                        return HTTP_IO_DONE;

                    if(AppConfig.networkType == WF_SOFT_AP)
                    {
                        SCAN_SET_DISPLAY(SCANCXT.scanState);
                        SCANCXT.displayIdx = 0;
                        return HTTP_IO_DONE;
                    }
                    else
                    {
                       // Start Scan Request
                        if (WFStartScan() == WF_SUCCESS)
                        {
                        SCAN_SET_DISPLAY(SCANCXT.scanState);
                        SCANCXT.displayIdx = 0;
                        return HTTP_IO_WAITING;
                        }
                    }
		}

/*		else if ((ptr == NULL) && (ptr1 != NULL))
		{
			// getBss request
			// use the value to get the nth bss stored on chip
			bssDescIsValid = FALSE;
			bssIdxStr.v[1] = *ptr1;
			bssIdxStr.v[0] = *(ptr1+1);	
	  		bssIdx = hexatob(bssIdxStr);
	  		WFRetrieveScanResult(bssIdx, &bssDesc);
	  		
	  		bssDescIsValid = TRUE;
		}
*/
		  	
	}
    else if(!memcmppgm2ram(filename, "miwi.xml", 8))
	{
		ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"miwicmd");
        if (ptr != NULL)
        {
            switch(*ptr)
            {
                case '1':   // Perform Channel Scan
                    // Get the Channel to Scan for Nodes on
                    ptr1 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"scanchnl");
                    scan_chnl = 1 << (atoi((const char *)ptr1));

                    MiApp_SetChannel(scan_chnl);

                    // Scan for Nodes (request 10 = 1 sec.)
                    num_nodes = MiApp_SearchConnection(10, scan_chnl);
                    break;

                default:
                    // Identify Node
                    break;

            }
        }

		else
		{
			// impossible to get here
		}
	}
    else if(!memcmppgm2ram(filename, "cmd.cgi", 8))
	{
		ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"miwicmd");
        if (ptr != NULL)
        {
            switch(*ptr)
            {
                case '2':   // Identify Node
                    ptr1 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"nodeIdx");
                    // Get Node Information
                    nodeIdx = atoi((const char *)ptr1);

                    ptr2 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"roomChan");
                    scan_chnl = atoi((const char *)ptr2);

                    MiApp_SetChannel(scan_chnl);
                    
                    myPANID.Val = ActiveScanResults[nodeIdx].PANID.Val;
                    MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);

                    MiApp_FlushTx();
                    MiApp_WriteData(0x01);
                    MiApp_WriteData(0x01);
                    MiApp_WriteData(SeqNum++);
                    MiApp_WriteData(0x00);
                    flashON = (!flashON);
                    if(flashON)
                        MiApp_WriteData(0x01);        // Number of times to Flash LED
                    else
                        MiApp_WriteData(0x00);
                    MiApp_UnicastAddress( ActiveScanResults[nodeIdx].Address, TRUE, FALSE);

                    break;

                case '3':   // Configure Node
                    ptr1 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"nodeIdx");
                    // Get Node Information
                    nodeIdx = atoi((const char *)ptr1);

                    ptr2 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"chnl");
                    channel = atoi((const char *)ptr2);
                            
                    ptr3 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"panid");
                    panId = strtoul((const char *)ptr3, (char **)&temp, 16);

                    myPANID.Val = ActiveScanResults[nodeIdx].PANID.Val;
                    MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);
                    flashON = (!flashON);
                    
                    MiApp_FlushTx();
                    MiApp_WriteData(0x01);
                    MiApp_WriteData(0x02);
                    MiApp_WriteData(SeqNum++);
                    MiApp_WriteData(0x00);

                    MiApp_WriteData(channel);           // New Channel
                    MiApp_WriteData(panId&0xff);        // New PanID
                    MiApp_WriteData((panId>>8)&0xff);
                    MiApp_UnicastAddress( ActiveScanResults[nodeIdx].Address, TRUE, FALSE);

                    break;

                case '4':   // Add Room
                    ptr1 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"name");
                    ptr2 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"panId");
                    ptr3 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"roomChan");

                    // Get Node Information
                    strncpy((char *)roomInfo.Name, (const char *)ptr1, 20);
                    roomInfo.PANID.Val = strtoul((const char *)ptr2, (char **)&temp, 16);
                    roomInfo.Channel = atoi((const char *)ptr3);
                    AddRoomInfo(&roomInfo);
                    break;

                case '5':   // Delete Room
                    EraseRoomInfo();
                    break;

                case '6':   // Control Lights
                    ptr1 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"roomChan");
                    scan_chnl = atoi((const char *)ptr1);

                    MiApp_SetChannel(scan_chnl);

                    ptr2 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"panId");
                    panId = strtoul((const char *)ptr2, (char **)&temp, 16);

                    ptr3 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"code");
                    code = atoi((const char *)ptr3);

                    myPANID.Val = panId;
                    MiMAC_SetAltAddress((BYTE *)&tmp, (BYTE *)&myPANID.Val);

                    MiApp_FlushTx();
                    MiApp_WriteData(0x02);
                    MiApp_WriteData(code); // 0x00=Off, 0x01=On, 0xFF=Recommission
                    MiApp_WriteData(SeqNum++);
                    MiApp_WriteData(0x00); // 0x00 = Broadcast

                    MiApp_BroadcastPacket(FALSE);

                    break;

                default:
                    break;
            }
        }
    }
	else
	{
	}		
	
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
	// Resolve which function to use and pass along
	BYTE filename[20];
	
	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, sizeof(filename));
	
#if defined(USE_LCD)
	if(!memcmppgm2ram(filename, "forms.htm", 9))
		return HTTPPostLCD();
#endif

#if defined(STACK_USE_EZ_CONFIG)
	if(!memcmppgm2ram(filename, "wifi_configure.htm", 18))
		return HTTPPostWifiConfig();
#endif

#if defined(STACK_USE_HTTP_MD5_DEMO)
	if(!memcmppgm2ram(filename, "upload.htm", 10))
		return HTTPPostMD5();
#endif

#if defined(STACK_USE_HTTP_APP_RECONFIG)
	if(!memcmppgm2ram(filename, "protect/config.htm", 18))
		return HTTPPostConfig();
	#if defined(STACK_USE_SNMP_SERVER)
	else if(!memcmppgm2ram(filename, "snmp/snmpconfig.htm", 19))
		return HTTPPostSNMPCommunity();
	#endif
#endif

#if defined(STACK_USE_SMTP_CLIENT)
	if(!strcmppgm2ram((char*)filename, "email/index.htm"))
		return HTTPPostEmail();
#endif
	
#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	if(!strcmppgm2ram((char*)filename, "dyndns/index.htm"))
		return HTTPPostDDNSConfig();
#endif

	return HTTP_IO_DONE;
}

/*****************************************************************************
  Function:
	static HTTP_IO_RESULT HTTPPostWifiConfig(void)

  Summary:
	Processes the wifi config data

  Description:
	Accepts wireless configuration data from the www site and saves them to a
	structure to be applied by the ZG configuration manager.

    The following configurations are possible:
         i) Mode: adhoc or infrastructure
        ii) Security:
               - None
               - WPA/WPA2 passphrase
               - WPA/WPA2 pre-calculated key
               - WEP 64-bit
               - WEP 128-bit
       iii) Key material
	
	If an error occurs, such as data is invalid they will be redirected to a page
    informing the user of such results.  

    NOTE: This code for modified originally from HTTPPostWifiConfig as distributed
          by Microchip.

  Precondition:
	None

  Parameters:
	None

  Return Values:
  	HTTP_IO_DONE - all parameters have been processed
  	HTTP_IO_NEED_DATA - data needed by this function has not yet arrived
  ***************************************************************************/
#if defined(STACK_USE_EZ_CONFIG)
static HTTP_IO_RESULT HTTPPostWifiConfig(void)
{
	// Check to see if the browser is attempting to submit more data than we 
	// can parse at once.  This function needs to receive all updated 
	// parameters and validate them all before committing them to memory so that
	// orphaned configuration parameters do not get written (for example, if a 
	// static IP address is given, but the subnet mask fails parsing, we 
	// should not use the static IP address).  Everything needs to be processed 
	// in a single transaction.  If this is impossible, fail and notify the user.
	// As a web devloper, if you add parameters to AppConfig and run into this 
	// problem, you could fix this by to splitting your update web page into two 
	// seperate web pages (causing two transactional writes).  Alternatively, 
	// you could fix it by storing a static shadow copy of AppConfig someplace 
	// in memory and using it instead of newAppConfig.  Lastly, you could 
	// increase the TCP RX FIFO size for the HTTP server.  This will allow more 
	// data to be POSTed by the web browser before hitting this limit.

    //UINT8 ConnectionProfileID;
    //UINT8 ConnectionState;
    UINT8 ssidLen;

    //WF_CMGetConnectionState(&ConnectionState, &ConnectionProfileID);

	if(curHTTP.byteCount > TCPIsGetReady(sktHTTP) + TCPGetRxFIFOFree(sktHTTP))
		goto ConfigFailure;
	
	// Ensure that all data is waiting to be parsed.  If not, keep waiting for 
	// all of it to arrive.
	if(TCPIsGetReady(sktHTTP) < curHTTP.byteCount)
		return HTTP_IO_NEED_DATA;
	
	// Read all browser POST data
	while(curHTTP.byteCount)
	{
		// Read a form field name
		if(HTTPReadPostName(curHTTP.data, 6) != HTTP_READ_OK)
			goto ConfigFailure;
			
		// Read a form field value
		if(HTTPReadPostValue(curHTTP.data + 6, sizeof(curHTTP.data)-6-2) != HTTP_READ_OK)
			goto ConfigFailure;
			
		// Parse the value that was read
		if(!strcmppgm2ram((char*)curHTTP.data, "sec"))
		{// Read security type
            char   security_type[7];

            if (strlen((char*)(curHTTP.data+6)) > 6) /* Sanity check */
                goto ConfigFailure;

            memcpy(security_type, (void*)(curHTTP.data+6), strlen((char*)(curHTTP.data+6)));
            security_type[strlen((char*)(curHTTP.data+6))] = 0; /* Terminate string */

		    if (!strcmppgm2ram((char*)security_type, "no"))
            {
                CFGCXT.security = WF_SECURITY_OPEN;
                //putrsUART((ROM char*)"\r\nSelect open on www! ");
            }
		    else if(!strcmppgm2ram((char*)security_type, "wpa")) 
            {
                CFGCXT.security = WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE;
                //putrsUART((ROM char*)"\r\nWPA passphrase! ");
            }
		    else if(!strcmppgm2ram((char*)security_type, "calc")) 
            {   /* Pre-calculated key */
                CFGCXT.security = WF_SECURITY_WPA_AUTO_WITH_KEY;
            }
		    else if(!strcmppgm2ram((char*)security_type, "wep40"))
            {
                CFGCXT.security = WF_SECURITY_WEP_40;
                //putrsUART((ROM char*)"\r\nSelect wep64 on www! ");
            }
		    else if(!strcmppgm2ram((char*)security_type, "wep104"))
            {
                CFGCXT.security = WF_SECURITY_WEP_104;
                //putrsUART((ROM char*)"\r\nSelect wep128 on www! ");
            }
                else if(!strcmppgm2ram((char*)security_type, "wps_button"))
            {
                CFGCXT.security = WF_SECURITY_WPS_PUSH_BUTTON;
                
            }
                else if(!strcmppgm2ram((char*)security_type, "wps_pin"))
            {
                CFGCXT.security = WF_SECURITY_WPS_PIN;
                
            }

		    else 
            {   //Security type no good  :-(
				//memset(LCDText, ' ', 32);
                //strcpy((char*)LCDText, (char*)security_type);
                //LCDUpdate();
                //putrsUART((ROM char*)"\r\nUnknown key type on www! ");
                goto ConfigFailure;
            }				
		}
		else if(!strcmppgm2ram((char*)curHTTP.data, "key"))
		{// Read new key material
            BYTE key_size = 0, ascii_key = 0;

            switch ((BYTE)CFGCXT.security) {
                case WF_SECURITY_OPEN:   //keep compiler happy, nothing to do here!
                    break;
                case WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:  //wpa passphrase
                    //putrsUART((ROM char*)"\r\nPassphrase type of key! ");
                    ascii_key = 1;
                    key_size = strlen((char *)(curHTTP.data+6));
                    //between 8-63 characters, passphrase
                    if ((key_size < 8 ) || (key_size > 63))
                          goto ConfigFailure;
                    break;
                case WF_SECURITY_WPA_AUTO_WITH_KEY: //wpa pre-calculated key!!!
                    key_size = 64;
				//memset(LCDText, ' ', 32);
                //strcpy((char*)LCDText, (char*)(curHTTP.data+6));
                //LCDUpdate();
                    break;
                case WF_SECURITY_WEP_40:
                    key_size = 10; /* Assume hex size */
                    if (strlen((char *)(curHTTP.data+6)) == 5) {
                        key_size = 5;  /* ASCII key support */
                        ascii_key = 1;
                     } 
                    CFGCXT.defaultWepKey = 0; /* Example uses only key idx 0 (sometimes called 1) */
                    break;
                case WF_SECURITY_WEP_104:
                    key_size = 26; /* Assume hex size */
                    if (strlen((char *)(curHTTP.data+6)) == 13) {
                        key_size = 13;  /* ASCII key support */
                        ascii_key = 1;
                    } 
                    CFGCXT.defaultWepKey = 0; /* Example uses only key idx 0 (sometimes called 1) */
                    break;

                default:
                    break;
            }
			if (strlen((char *)(curHTTP.data+6)) != key_size) {
                //putrsUART((ROM char*)"\r\nIncomplete key received! ");
                goto ConfigFailure;
            }
            memcpy(CFGCXT.key, (void*)(curHTTP.data+6), key_size);
            CFGCXT.key[key_size] = 0; /* terminate string */
            if (!ascii_key) {
                //if ((cfg.security == sec_wep64) || (cfg.security == sec_wep128))
                    key_size /= 2;
                if (!convertAsciiToHexInPlace((INT8 *)&CFGCXT.key[0], key_size)) {
                    //putrsUART((ROM char*)"\r\nFailed to convert ASCII to hex! ");
                    goto ConfigFailure;
                }
            }
		}
		else if(!strcmppgm2ram((char*)curHTTP.data, "ssid"))
		{// Get new ssid and make sure it is valid
			if(strlen((char*)(curHTTP.data+6)) < 33u)
			{
                memcpy(CFGCXT.ssid, (void*)(curHTTP.data+6), strlen((char*)(curHTTP.data+6)));
                CFGCXT.ssid[strlen((char*)(curHTTP.data+6))] = 0; /* Terminate string */

                /* save current profile SSID for displaying later */
                WF_CPGetSsid(1, (UINT8*)&CFGCXT.prevSSID, &ssidLen);
                //WF_CPGetSsid(ConnectionProfileID, (UINT8*)&CFGCXT.prevSSID, &ssidLen);
                CFGCXT.prevSSID[ssidLen] = 0;
			}
			else
			{   //Invalid SSID... fail :-(
				goto ConfigFailure;
			}
		}
		else if(!strcmppgm2ram((char*)curHTTP.data, (ROM char*)"wlan"))
		{// Get the wlan mode: adhoc or infrastructure
            char mode[6];

            if (strlen((char*)(curHTTP.data+6)) > 5) /* Sanity check */
                goto ConfigFailure;

            memcpy(mode, (void*)(curHTTP.data+6), strlen((char*)(curHTTP.data+6)));
            mode[strlen((char*)(curHTTP.data+6))] = 0; /* Terminate string */
		    if(!strcmppgm2ram((char*)mode, (ROM char*)"infra"))
            {
                CFGCXT.type = WF_INFRASTRUCTURE;
            }
		    else if(!strcmppgm2ram((char*)mode, "adhoc")) 
            {
                //putrsUART((ROM char*)"\r\nSetting mode to adhoc! ");
                CFGCXT.type = WF_ADHOC;

                // always setup adhoc to attempt to connect first, then start
                WF_CPSetAdHocBehavior(1, WF_ADHOC_CONNECT_THEN_START);
                //WF_CPSetAdHocBehavior(ConnectionProfileID, WF_ADHOC_CONNECT_THEN_START);
            }
             else if(!strcmppgm2ram((char*)mode, "wfp2p"))
            {
                printf("\r\nSetting mode to wifi-direct! ");
                CFGCXT.type = WF_P2P;
            }
            else if(!strcmppgm2ram((char*)mode, "sofap"))
            {
                printf("\r\nSetting mode to softap! ");
                CFGCXT.type = WF_SOFT_AP;
            }
		    else 
            {   //Mode type no good  :-(
				//memset(LCDText, ' ', 32);
                //putrsUART((ROM char*)"\r\nUnknown mode type on www! ");
                //strcpy((char*)LCDText, (char*)mode);
                //LCDUpdate();
                goto ConfigFailure;
            }

            // save old WLAN mode
            WF_CPGetNetworkType(1, &CFGCXT.prevWLAN);
            //WF_CPGetNetworkType(ConnectionProfileID, &CFGCXT.prevWLAN);
		}
	}

    /* Check if WPA hasn't been selected with adhoc, if it has we choke! */
    if (((CFGCXT.type == WF_ADHOC) &&
      ((CFGCXT.security == WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE) || (CFGCXT.security == WF_SECURITY_WPA_AUTO_WITH_KEY)))
       || ((CFGCXT.type == WF_SOFT_AP) &&
      ((CFGCXT.security == WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE) || (CFGCXT.security == WF_SECURITY_WPA_AUTO_WITH_KEY))))
        goto ConfigFailure;

	/* 
     * All parsing complete!  If we have got to here all data has been validated and
     * We can handle what is necessary to start the reconfigure process of the WiFi device
     */

//TO BE FIXEd
//#if defined ( EZ_CONFIG_STORE )
    /* Copy wifi cfg data to be committed to NVM */
    
    strcpy((char *)AppConfig.MySSID, (char *)CFGCXT.ssid);
    AppConfig.SsidLength = strlen((char*)(CFGCXT.ssid));
    
    /* Going to set security type */
    AppConfig.SecurityMode = CFGCXT.security; 
    /* Going to save the key, if required */
    if (CFGCXT.security != WF_SECURITY_OPEN) {
        BYTE  key_size =0;

        switch ((BYTE)CFGCXT.security) {
            case WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:  //wpa passphrase
                key_size = strlen((char*)(CFGCXT.key)); //ascii so use strlen
                break;
            case WF_SECURITY_WPA_AUTO_WITH_KEY: //wpa pre-calculated key!!!
                key_size = 32;
                break;
            case WF_SECURITY_WEP_40:
                key_size = 5;
                break;
            case WF_SECURITY_WEP_104:
                key_size = 13;
                break;
            default: break;

        }
        memcpy(AppConfig.SecurityKey, CFGCXT.key, key_size);
        AppConfig.SecurityKey[strlen((char*)(CFGCXT.key))] = 0;
    }
    /* Going to save the network type */
    AppConfig.networkType = CFGCXT.type;

    AppConfig.dataValid = 1; /* Validate wifi configuration */
//TO BE FIXED
//#endif // EZ_CONFIG_STORE
	
	// Set the board to reboot and display reconnecting information
	strcpypgm2ram((char*)curHTTP.data, "/reconnect.htm");
	curHTTP.httpStatus = HTTP_REDIRECT;	

    /*
     * Set state here to inform that the Wifi device has config data and it is ready
     * to be acted upon.
     */
    //putrsUART((ROM char*)"\r\nFLaggin to start config change! ");
	WF_START_EASY_CONFIG();

	return HTTP_IO_DONE;


ConfigFailure:
	lastFailure = TRUE;
	strcpypgm2ram((char*)curHTTP.data, "/error.htm");
	curHTTP.httpStatus = HTTP_REDIRECT;		

	return HTTP_IO_DONE;
}
#endif	

/*****************************************************************************
  Function:
	static HTTP_IO_RESULT HTTPPostLCD(void)

  Summary:
	Processes the LCD form on forms.htm

  Description:
	Locates the 'lcd' parameter and uses it to update the text displayed
	on the board's LCD display.
	
	This function has four states.  The first reads a name from the data
	string returned as part of the POST request.  If a name cannot
	be found, it returns, asking for more data.  Otherwise, if the name 
	is expected, it reads the associated value and writes it to the LCD.  
	If the name is not expected, the value is discarded and the next name 
	parameter is read.
	
	In the case where the expected string is never found, this function 
	will eventually return HTTP_IO_NEED_DATA when no data is left.  In that
	case, the HTTP2 server will automatically trap the error and issue an
	Internal Server Error to the browser.

  Precondition:
	None

  Parameters:
	None

  Return Values:
  	HTTP_IO_DONE - the parameter has been found and saved
  	HTTP_IO_WAITING - the function is pausing to continue later
  	HTTP_IO_NEED_DATA - data needed by this function has not yet arrived
  ***************************************************************************/
#if defined(USE_LCD)
static HTTP_IO_RESULT HTTPPostLCD(void)
{
	BYTE* cDest;
	
	#define SM_POST_LCD_READ_NAME		(0u)
	#define SM_POST_LCD_READ_VALUE		(1u)
	
	switch(curHTTP.smPost)
	{
		// Find the name
		case SM_POST_LCD_READ_NAME:
		
			// Read a name
			if(HTTPReadPostName(curHTTP.data, HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
				return HTTP_IO_NEED_DATA;

			curHTTP.smPost = SM_POST_LCD_READ_VALUE;
			// No break...continue reading value
		
		// Found the value, so store the LCD and return
		case SM_POST_LCD_READ_VALUE:
					
			// If value is expected, read it to data buffer,
			// otherwise ignore it (by reading to NULL)	
			if(!strcmppgm2ram((char*)curHTTP.data, (ROM char*)"lcd"))
				cDest = curHTTP.data;
			else
				cDest = NULL;
			
			// Read a value string
			if(HTTPReadPostValue(cDest, HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
				return HTTP_IO_NEED_DATA;
			
			// If this was an unexpected value, look for a new name
			if(!cDest)
			{
				curHTTP.smPost = SM_POST_LCD_READ_NAME;
				break;
			}
			
			// Copy up to 32 characters to the LCD
			if(strlen((char*)cDest) < 32u)
			{
				memset(LCDText, ' ', 32);
				strcpy((char*)LCDText, (char*)cDest);
			}
			else
			{
				memcpy(LCDText, (void *)cDest, 32);
			}
			LCDUpdate();
			
			// This is the only expected value, so callback is done
			strcpypgm2ram((char*)curHTTP.data, "/forms.htm");
			curHTTP.httpStatus = HTTP_REDIRECT;
			return HTTP_IO_DONE;
	}
	
	// Default assumes that we're returning for state machine convenience.
	// Function will be called again later.
	return HTTP_IO_WAITING;
}
#endif

/****************************************************************************
  Function:
    HTTP_IO_RESULT HTTPPostDDNSConfig(void)
    
  Summary:
    Parsing and collecting http data received from http form.

  Description:
    This routine will be excuted every time the Dynamic DNS Client
    configuration form is submitted.  The http data is received 
    as a string of the variables seperated by '&' characters in the TCP RX
    buffer.  This data is parsed to read the required configuration values, 
    and those values are populated to the global array (DDNSData) reserved 
    for this purpose.  As the data is read, DDNSPointers is also populated
    so that the dynamic DNS client can execute with the new parameters.
    
  Precondition:
     curHTTP is loaded.

  Parameters:
    None.

  Return Values:
    HTTP_IO_DONE 		-  Finished with procedure
    HTTP_IO_NEED_DATA	-  More data needed to continue, call again later
    HTTP_IO_WAITING 	-  Waiting for asynchronous process to complete, 
    						call again later
  ***************************************************************************/
#if defined(STACK_USE_DYNAMICDNS_CLIENT)
static HTTP_IO_RESULT HTTPPostDDNSConfig(void)
{
	static BYTE *ptrDDNS;

	#define SM_DDNS_START			(0u)
	#define SM_DDNS_READ_NAME		(1u)
	#define SM_DDNS_READ_VALUE		(2u)
	#define SM_DDNS_READ_SERVICE	(3u)
	#define SM_DDNS_DONE			(4u)

	#define DDNS_SPACE_REMAINING				(sizeof(DDNSData) - (ptrDDNS - DDNSData))

	switch(curHTTP.smPost)
	{
		// Sets defaults for the system
		case SM_DDNS_START:
			ptrDDNS = DDNSData;
			DDNSSetService(0);
			DDNSClient.Host.szROM = NULL;
			DDNSClient.Username.szROM = NULL;
			DDNSClient.Password.szROM = NULL;
			DDNSClient.ROMPointers.Host = 0;
			DDNSClient.ROMPointers.Username = 0;
			DDNSClient.ROMPointers.Password = 0;
			curHTTP.smPost++;
			
		// Searches out names and handles them as they arrive
		case SM_DDNS_READ_NAME:
			// If all parameters have been read, end
			if(curHTTP.byteCount == 0u)
			{
				curHTTP.smPost = SM_DDNS_DONE;
				break;
			}
		
			// Read a name
			if(HTTPReadPostName(curHTTP.data, HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
				return HTTP_IO_NEED_DATA;
			
			if(!strcmppgm2ram((char *)curHTTP.data, (ROM char*)"service"))
			{
				// Reading the service (numeric)
				curHTTP.smPost = SM_DDNS_READ_SERVICE;
				break;
			}
			else if(!strcmppgm2ram((char *)curHTTP.data, (ROM char*)"user"))
				DDNSClient.Username.szRAM = ptrDDNS;
			else if(!strcmppgm2ram((char *)curHTTP.data, (ROM char*)"pass"))
				DDNSClient.Password.szRAM = ptrDDNS;
			else if(!strcmppgm2ram((char *)curHTTP.data, (ROM char*)"host"))
				DDNSClient.Host.szRAM = ptrDDNS;
			
			// Move to reading the value for user/pass/host
			curHTTP.smPost++;
			
		// Reads in values and assigns them to the DDNS RAM
		case SM_DDNS_READ_VALUE:
			// Read a name
			if(HTTPReadPostValue(ptrDDNS, DDNS_SPACE_REMAINING) == HTTP_READ_INCOMPLETE)
				return HTTP_IO_NEED_DATA;
				
			// Move past the data that was just read
			ptrDDNS += strlen((char*)ptrDDNS);
			if(ptrDDNS < DDNSData + sizeof(DDNSData) - 1)
				ptrDDNS += 1;			
			
			// Return to reading names
			curHTTP.smPost = SM_DDNS_READ_NAME;
			break;
		
		// Reads in a service ID
		case SM_DDNS_READ_SERVICE:
			// Read the integer id
			if(HTTPReadPostValue(curHTTP.data, HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
				return HTTP_IO_NEED_DATA;
			
			// Convert to a service ID
			DDNSSetService((BYTE)atol((char*)curHTTP.data));

			// Return to reading names
			curHTTP.smPost = SM_DDNS_READ_NAME;
			break;
			
		// Sets up the DDNS client for an update
		case SM_DDNS_DONE:
			// Since user name and password changed, force an update immediately
			DDNSForceUpdate();
			
			// Redirect to prevent POST errors
			lastSuccess = TRUE;
			strcpypgm2ram((char*)curHTTP.data, "/dyndns/index.htm");
			curHTTP.httpStatus = HTTP_REDIRECT;
			return HTTP_IO_DONE;				
	}
	
	return HTTP_IO_WAITING;		// Assume we're waiting to process more data
}
#endif	// #if defined(STACK_USE_DYNAMICDNS_CLIENT)

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
void HTTPPrint_builddate(void)
{
    curHTTP.callbackPos = 0x01;
    if(TCPIsPutReady(sktHTTP) < strlenpgm((ROM char*)__DATE__" "__TIME__))
        return;

    curHTTP.callbackPos = 0x00;
    TCPPutROMString(sktHTTP, (ROM void*)__DATE__" "__TIME__);
}

void HTTPPrint_version(void)
{
    TCPPutROMString(sktHTTP, (ROM void*)TCPIP_STACK_VERSION);
}

void HTTPPrint_scan(void)
{
    BYTE scanInProgressString[4];

    uitoa(IS_SCAN_IN_PROGRESS(SCANCXT.scanState), scanInProgressString);
    TCPPutString(sktHTTP, scanInProgressString);
}

void HTTPPrint_fwver(void)
{
    tWFDeviceInfo deviceInfo;
    BYTE fwVerString[8];

    WF_GetDeviceInfo(&deviceInfo);

    uitoa((deviceInfo.romVersion << 8) | deviceInfo.patchVersion, fwVerString);
    TCPPutString(sktHTTP, fwVerString);
}

void HTTPPrint_ssid(void)
{
    BYTE ssidString[33];
    BYTE ssidLength;

    BYTE connState;
    BYTE currCpId;

    WF_CMGetConnectionState(&connState, &currCpId);

    // we don't need to check the connection state as the only way this function
    // is called is from the webserver.  if the webserver is requesting this,
    // then you can infer that we should be connected to the network
    WF_CPGetSsid(currCpId, ssidString, &ssidLength);

    TCPPutArray(sktHTTP, ssidString, ssidLength);
}	

void HTTPPrint_bssCount(void)
{
    BYTE bssCountString[4];

    uitoa(SCANCXT.numScanResults, bssCountString);
    TCPPutString(sktHTTP, bssCountString);
}

void HTTPPrint_valid(void)
{
    BYTE bssDescIsValidString[4];

    uitoa((BYTE)bssDescIsValid, bssDescIsValidString);
    TCPPutString(sktHTTP, bssDescIsValidString);
}	

void HTTPPrint_name(void)
{
    if (bssDescIsValid)
    {
        TCPPutString(sktHTTP, bssDesc.ssid);
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE *)"0");
    }
}	

void HTTPPrint_privacy(void)
{
    BYTE security = (bssDesc.apConfig & 0xd0) >> 4;
    BYTE secString[4];

    if (bssDescIsValid)
    {
        uitoa(security, secString);
        TCPPutString(sktHTTP, secString);
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"0");
    }
}	

void HTTPPrint_curPrivacy(void)
{
    BYTE security = AppConfig.SecurityMode;

    if (bssDescIsValid)
    {
        switch (security)
        {
            case 0:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"None");
                break;
            case 1:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WEP");
                break;
            case 2:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WEP");
                break;
            case 3:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WPA");
                break;
            case 4:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WPA");
                break;
            case 5:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WPA2");
                break;
            case 6:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WPA2");
                break;
            case 7:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WPA-AUTO");
                break;
            case 8:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WPA-AUTO");
                break;
            case 13:
                TCPPutROMString(sktHTTP, (ROM BYTE*)"WPA");
                break;
            default:
                // impossible to get here!
            break;
        }
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"None");
    }
}

void HTTPPrint_wlan(void)
{
    BYTE bssTypeString[4];
    if (bssDescIsValid)
    {
        uitoa(bssDesc.bssType, bssTypeString);
        TCPPutString(sktHTTP, bssTypeString);
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"0");
    }
}	

void HTTPPrint_curWLAN(void)
{
    if(AppConfig.networkType == WF_ADHOC)
        TCPPutROMString(sktHTTP, (ROM BYTE*)"AdHoc  (IBSS)");
    else if(AppConfig.networkType == WF_INFRASTRUCTURE)
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Infrastructure (BSS)");
    else if(AppConfig.networkType == WF_SOFT_AP)
        TCPPutROMString(sktHTTP, (ROM BYTE*)"SoftAP (BSS)");
    else if(AppConfig.networkType == WF_P2P)
        TCPPutROMString(sktHTTP, (ROM BYTE*)"WiFi Direct P2P (BSS)");
    else
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Unknown BSS");
}	


void HTTPPrint_strength(void)
{
    BYTE strVal;
    BYTE strString[4];

    if (bssDescIsValid)
    {
        if (bssDesc.rssi < 121)
        {
            strVal = 1;
        }
        else if (bssDesc.rssi < 141)
        {
            strVal = 2;
        }
        else if (bssDesc.rssi < 161)
        {
            strVal = 3;
        }
        else
        {
            strVal = 4;
        }

        uitoa(strVal, strString);
        TCPPutString(sktHTTP, strString);
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"0");
    }
}

void HTTPPrint_nextSSID(void)
{
    TCPPutString(sktHTTP, (BYTE*)CFGCXT.ssid);
}

void HTTPPrint_prevSSID(void)
{
    TCPPutString(sktHTTP, (BYTE*)CFGCXT.prevSSID);
}

void HTTPPrint_prevWLAN(void)
{
    if (CFGCXT.prevWLAN == WF_INFRASTRUCTURE)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Infrastructure (BSS)");
    }
    else if(CFGCXT.prevWLAN == WF_SOFT_AP)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"SoftAP (BSS)");
    }
    else if(CFGCXT.prevWLAN == WF_ADHOC)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Adhoc (IBSS)");
    }
    else if(CFGCXT.prevWLAN == WF_P2P)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"WiFi Direct P2P (BSS)");
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Unknown BSS");
    }


}

void HTTPPrint_nextWLAN(void)
{
    if (CFGCXT.type == WF_INFRASTRUCTURE)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Infrastructure (BSS)");
    }
    else if (CFGCXT.type == WF_ADHOC)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"AdHoc (IBSS)");
    }
    else if (CFGCXT.type == WF_SOFT_AP)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"SoftAP (BSS)");
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Unknown");
    }
}

void HTTPPrint_Demoversion(void)
{
    TCPPutROMString(sktHTTP, (ROM void*)DEMO_VERSION);
}

void HTTPPrint_miwicmd(void)
{
    BYTE scanInProgressString[2];

    uitoa(miwiCmdFlag, scanInProgressString);
    TCPPutString(sktHTTP, scanInProgressString);
}


// =========================
// Sends the number of nodes found from current Active Scan
// =========================
void HTTPPrint_miwiscancnt(void)
{
    BYTE strString[2];

    uitoa(num_nodes, strString);
    TCPPutString(sktHTTP, strString);
}

// =========================
// Sends MiWi Nodes Information from the Active Scan List
// =========================
void HTTPPrint_nodes(void)
{
    int x,j;
    BYTE panidString[16];
    BYTE macString[4];
    BYTE chnlString[3];
    BYTE stringBuff[16];
    
    for(x=0; x < num_nodes; x++)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<Node>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<index>");
        uitoa(x, chnlString);
        TCPPutString(sktHTTP, chnlString);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</index>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<channel>");
        uitoa(ActiveScanResults[nodeIdx].Channel, chnlString);
        TCPPutString(sktHTTP, chnlString);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</channel>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<macaddr>");
        for (j=0; j<8; j++)
        {
            macString[0] = btohexa_high(ActiveScanResults[x].Address[7-j]);
            macString[1] = btohexa_low(ActiveScanResults[x].Address[7-j]);
            macString[2] = 0;
            TCPPutString(sktHTTP, macString);
        }
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</macaddr>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<panid>");
        panidString[0] = btohexa_high(ActiveScanResults[x].PANID.v[1]);
        panidString[1] = btohexa_low(ActiveScanResults[x].PANID.v[1]);
        panidString[2] = btohexa_high(ActiveScanResults[x].PANID.v[0]);
        panidString[3] = btohexa_low(ActiveScanResults[x].PANID.v[0]);
        panidString[4] = 0;
        TCPPutString(sktHTTP, panidString);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</panid>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<rssi>");
        uitoa(ActiveScanResults[nodeIdx].RSSIValue, stringBuff);
        TCPPutString(sktHTTP, stringBuff);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</rssi>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<lqi>");
        uitoa(ActiveScanResults[nodeIdx].LQIValue, stringBuff);
        TCPPutString(sktHTTP, stringBuff);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</lqi>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<capability>");
        uitoa(ActiveScanResults[nodeIdx].Capability.Val, stringBuff);
        TCPPutString(sktHTTP, stringBuff);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</capability>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</Node>\n");
    }
}


void HTTPPrint_rooms(void)
{
    int x = 0;
    BYTE stringBuff[16];
    ROOM_RECORD roomInfo;

    
    // Fill the xml file with room info
    for(x = 0; x < MAX_NUM_ROOMS; x++)
    {
        GetRoomInfo(&roomInfo, x);
        if(roomInfo.recType == VALID_REC)
        {
            TCPPutROMString(sktHTTP, (ROM BYTE*)"<Room>\n");
            TCPPutROMString(sktHTTP, (ROM BYTE*)"<index>");
            uitoa(x, stringBuff);
            TCPPutString(sktHTTP, stringBuff);
            TCPPutROMString(sktHTTP, (ROM BYTE*)"</index>\n");
            TCPPutROMString(sktHTTP, (ROM BYTE*)"<channel>");
            uitoa(roomInfo.Channel, stringBuff);
            TCPPutString(sktHTTP, stringBuff);
            TCPPutROMString(sktHTTP, (ROM BYTE*)"</channel>\n");
            TCPPutROMString(sktHTTP, (ROM BYTE*)"<name>");
            TCPPutString(sktHTTP, roomInfo.Name);
            TCPPutROMString(sktHTTP, (ROM BYTE*)"</name>\n");
            TCPPutROMString(sktHTTP, (ROM BYTE*)"<panid>");
            stringBuff[0] = btohexa_high(roomInfo.PANID.v[1]);
            stringBuff[1] = btohexa_low(roomInfo.PANID.v[1]);
            stringBuff[2] = btohexa_high(roomInfo.PANID.v[0]);
            stringBuff[3] = btohexa_low(roomInfo.PANID.v[0]);
            stringBuff[4] = 0;
            TCPPutString(sktHTTP, stringBuff);
            TCPPutROMString(sktHTTP, (ROM BYTE*)"</panid>\n");
            TCPPutROMString(sktHTTP, (ROM BYTE*)"</Room>\n");
        }
    }
}


void HTTPPrint_aplist(void)
{
    int x;
    BYTE security;
    BYTE secString[4];
    BYTE bssTypeString[4];
    BYTE strVal;
    BYTE strString[4];


    for(x=0; x < SCANCXT.numScanResults; x++)
    {
        if(AppConfig.networkType == WF_SOFT_AP)
        {
            bssDesc = preScanResult[x];
        }
        else
        {
            WFRetrieveScanResult(x, &bssDesc);
        }
        

        TCPPutROMString(sktHTTP, (ROM BYTE*)"<bss>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<name>");
        TCPPutString(sktHTTP, bssDesc.ssid);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</name>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<privacy>");
        security = (bssDesc.apConfig & 0xd0) >> 4;
        uitoa(security, secString);
        TCPPutString(sktHTTP, secString);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</privacy>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<wlan>");
        uitoa(bssDesc.bssType, bssTypeString);
        TCPPutString(sktHTTP, bssTypeString);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</wlan>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"<strength>");
        if (bssDesc.rssi < 121)
        {
            strVal = 1;
        }
        else if (bssDesc.rssi < 141)
        {
            strVal = 2;
        }
        else if (bssDesc.rssi < 161)
        {
            strVal = 3;
        }
        else
        {
            strVal = 4;
        }

        uitoa(strVal, strString);
        TCPPutString(sktHTTP, strString);
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</strength>\n");
        TCPPutROMString(sktHTTP, (ROM BYTE*)"</bss>\n");
    }

    SCAN_CLEAR_DISPLAY(SCANCXT.scanState);
}


#endif
