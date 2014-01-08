/*********************************************************************
 *
 *  HTTP Server Application Routines
 *  Support for HTTP2 module in Microchip TCP/IP Stack
 *	 -Implements the application 
 *	 -Reference: RFC 1002
 *
 *********************************************************************
 * FileName:           CustomHTTPApp.c
 * Dependencies:    TCP/IP stack
 * Processor:          PIC32MX695F512H
 * Compiler:           Microchip  XC32 Compiler
 * Company:          Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2013 Microchip Technology Inc.  All rights
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
 * Amy Ong             10/24/12          Created for WiFi G Demo  (MRF24WG0MA)
 *
 ********************************************************************/
#define __CUSTOMHTTPAPP_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_HTTP2_SERVER)
#include "TCPIP Stack/TCPIP.h"
#include "TCPIP Stack/WFConsoleMsgHandler.h"
#include "MainDemo.h"        // Needed for SaveAppConfig() prototype


// Differences to wifi comm demo board (MRF24WB0MA) :
//        Wifi comm demo board is centered on variable CPElements.
//        Wifi G demo board is centered on variable AppConfig, since this is the generic approach adopted by 
//        TCPIP demo/ezconsole/ezconfig apps.

extern APP_CONFIG AppConfig;

/****************************************************************************
  Section:
	Function Prototypes and Memory Globalizers
  ***************************************************************************/
#if defined(HTTP_USE_POST)
	#if defined(STACK_USE_EZ_CONFIG)
        #include "TCPIP Stack/WFEasyConfig.h"
	    static HTTP_IO_RESULT HTTPPostWifiConfig(void);
    #endif
#endif

// bss descriptor fetched from on-chip
static tWFScanResult bssDesc;
static BOOL bssDescIsValid = FALSE;

// RAM allocated for DDNS parameters
#if defined(STACK_USE_DYNAMICDNS_CLIENT)
	static BYTE DDNSData[100];
#endif

// Sticky status message variable.
// This is used to indicated whether or not the previous POST operation was 
// successful.  The application uses these to store status messages when a 
// POST operation redirects.  This lets the application provide status messages
// after a redirect, when connection instance data has already been lost.
static BOOL lastFailure = FALSE;    

extern tWFScanResult preScanResult[50]; // WF_PRESCAN

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
	BYTE *ptr, *ptr1;
	BYTE filename[20];

    BYTE bssIdx;
    WORD_VAL bssIdxStr;
	
	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, 20);

    /******************************************/
	// If it's the leds.cgi LED update file
    /******************************************/
	if(!memcmppgm2ram(filename, "leds.cgi", 8))
	{
		// Determine which LED to toggle
		ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"led");
		
		// Toggle the specified LED
		switch(*ptr)
        {
			case '1':
				LED1_INV();
				break;
			case '2':
				LED2_INV();
				break;
            default:
                break;
		}
		
	}
    /******************************************/
    // If it's the  scan.cgi scan file
    /******************************************/
	if(!memcmppgm2ram(filename, "scan.cgi", 8))
	{
		ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"scan");
		ptr1 = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"getBss");

		if ((ptr != NULL) && (ptr1 == NULL))
		{
			bssDescIsValid = FALSE;
			// scan request
			if (AppConfig.networkType == WF_SOFT_AP)
			{   // SoftAP: display pre-scan results before starting as SoftAP. SoftAP does not scan.
				SCAN_SET_DISPLAY(SCANCXT.scanState);				
			}
			else
			{
				if (WFStartScan() == WF_SUCCESS)
				{
					SCAN_SET_DISPLAY(SCANCXT.scanState);
					SCANCXT.displayIdx = 0;						
				}
			}
        }
        else if ((ptr == NULL) && (ptr1 != NULL))
        {
            // getBss request
            // use the value to get the nth bss stored on chip
            bssDescIsValid = FALSE;
            bssIdxStr.v[1] = *ptr1;
            bssIdxStr.v[0] = *(ptr1+1);    
            bssIdx = hexatob(bssIdxStr);

            if (AppConfig.networkType == WF_SOFT_AP)
            {   // SoftAP: display pre-scan results before starting as SoftAP. SoftAP does not scan.
                bssDesc = preScanResult[bssIdx];
            }
            else
            {
            	WFRetrieveScanResult(bssIdx, &bssDesc);
            }

			// This is needed to ensure index.htm displays correct WLAN type.
			// Index.htm differs between EZConfig and wifi g demo in that index.htm
			// in wifi g demo displays BSS information of MRF24WG. 		
			if (++bssIdx == SCANCXT.numScanResults)  
				SCAN_CLEAR_DISPLAY(SCANCXT.scanState);
		
            bssDescIsValid = TRUE;
        }
        else
        {
            // impossible to get here
        }  

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

    /******************************************/
    // If it's the configure.htm page, use settings to reconfigure wifi settings
    /******************************************/
    #if defined(STACK_USE_EZ_CONFIG)
	if(!memcmppgm2ram(filename, "configure.htm", 13))
		return HTTPPostWifiConfig();
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
	structure to be applied by the wifi configuration manager.

    The following configurations are possible:
         i) Mode: Adhoc / infrastructure / softAP / wifi direct
        ii) Security:
               - None
               - WPA/WPA2 passphrase (passphrase between 8-63 characters)
               - WPA/WPA2 pre-calculated key
               - WEP 64-bit
               - WEP 128-bit
               - WPS Push Button
               - WPS PIN
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

    //  UINT8 ConnectionProfileID; 
    //  UINT8 ConnectionState; 
    UINT8 ssidLen;

   // Comments: Using WF_CMGetConnectionState causes redirection issues when using iPhone iOS6.1, even though
   //                  redirection on laptop is functional.
   //                  Customers are encouraged to use 1 profile ID for MRF24W based on v5 stack SW.
   //                  In v6 stack SW, we are keeping to 1 profile ID for MRF24W but stack will have
   //                  capability to handle multiple profiles.
   // WF_CMGetConnectionState(&ConnectionState, &ConnectionProfileID);

	if(curHTTP.byteCount > TCPIsGetReady(sktHTTP) + TCPGetRxFIFOFree(sktHTTP))
		goto ConfigFailure;

	// Ensure that all data is waiting to be parsed.  If not, keep waiting for 
	// all of it to arrive.
	if(TCPIsGetReady(sktHTTP) < curHTTP.byteCount)
		return HTTP_IO_NEED_DATA;

    //**************************************
	// Read all browser POST data
    //**************************************
	while(curHTTP.byteCount)
	{
		// Read a form field name
		if(HTTPReadPostName(curHTTP.data, 6) != HTTP_READ_OK)
			goto ConfigFailure;
			
		// Read a form field value
		if(HTTPReadPostValue(curHTTP.data + 6, sizeof(curHTTP.data)-6-2) != HTTP_READ_OK)
			goto ConfigFailure;
			
		// Parse the values that were read     
        //**************************************
        // Read security type
        //**************************************
		if(!strcmppgm2ram((char*)curHTTP.data, "sec"))
		{
            char   security_type[7];

            if (strlen((char*)(curHTTP.data+6)) > 6) /* Sanity check */
                goto ConfigFailure;

            memcpy(security_type, (void*)(curHTTP.data+6), strlen((char*)(curHTTP.data+6)));
            security_type[strlen((char*)(curHTTP.data+6))] = 0; /* Terminate string */
            
            printf("\r\nSecurity Mode: ");
		    if (!strcmppgm2ram((char*)security_type, "no"))
            {
                CFGCXT.security = WF_SECURITY_OPEN;
                printf("OPEN");
            }
		    else if(!strcmppgm2ram((char*)security_type, "wpa")) 
            {
                CFGCXT.security = WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE;
                printf("WPA w/PASSPHRASE");
            }
		    else if(!strcmppgm2ram((char*)security_type, "calc")) 
            {   /* Pre-calculated key */
                CFGCXT.security = WF_SECURITY_WPA_AUTO_WITH_KEY;
                printf("WPA w/AUTO Key");
            }
		    else if(!strcmppgm2ram((char*)security_type, "wep40"))
            {
                CFGCXT.security = WF_SECURITY_WEP_40;
                printf("WEP 64-bit");
            }
		    else if(!strcmppgm2ram((char*)security_type, "wep104"))
            {
                CFGCXT.security = WF_SECURITY_WEP_104;
                printf("WEP 128-bit");
            }
			else if(!strcmppgm2ram((char*)security_type, "wps_button"))
            {
                CFGCXT.security = WF_SECURITY_WPS_PUSH_BUTTON;
                printf("WPS Push-Button");
            }
            else if(!strcmppgm2ram((char*)security_type, "wps_pin"))
            {
                CFGCXT.security = WF_SECURITY_WPS_PIN;
                printf("WPS PIN");
            }
		    else 
            {   //Security type no good  :-(
                printf("\r\nUnknown key type!");
                goto ConfigFailure;
            }				
		}

        //**************************************
        // Read new Security Key
        //**************************************
		else if(!strcmppgm2ram((char*)curHTTP.data, "key"))
		{
            BYTE key_size = 0, ascii_key = 0;

            switch ((BYTE)CFGCXT.security)
            {
                case WF_SECURITY_OPEN:   //keep compiler happy, nothing to do here!
                    break;

                case WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:  //wpa passphrase
                    printf("\r\nPassphrase type of key! ");
                    ascii_key = 1;
                    key_size = strlen((char *)(curHTTP.data+6));
                    //between 8-63 characters, passphrase
                    if ((key_size < 8 ) || (key_size > 63))
                          goto ConfigFailure;
                    break;

                case WF_SECURITY_WPA_AUTO_WITH_KEY: //wpa pre-calculated key!!!
                    key_size = 64;
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

			if (strlen((char *)(curHTTP.data+6)) != key_size)
            {
                printf("\r\nIncomplete key received! ");
                goto ConfigFailure;
            }
            memcpy(CFGCXT.key, (void*)(curHTTP.data+6), key_size);
            CFGCXT.key[key_size] = 0; /* terminate string */
            if (!ascii_key)
            {
                //if ((cfg.security == sec_wep64) || (cfg.security == sec_wep128))
                    key_size /= 2;
                if (!convertAsciiToHexInPlace((INT8 *)&CFGCXT.key[0], key_size))
                {
                    printf("\r\nFailed to convert ASCII to hex! ");
                    goto ConfigFailure;
                }
            }
		}

        //**************************************
        // Get new SSID and make sure it is valid
        //**************************************
		else if(!strcmppgm2ram((char*)curHTTP.data, "ssid")) 
		{
			if(strlen((char*)(curHTTP.data+6)) < 33u)
			{
                memcpy(CFGCXT.ssid, (void*)(curHTTP.data+6), strlen((char*)(curHTTP.data+6)));
                CFGCXT.ssid[strlen((char*)(curHTTP.data+6))] = 0; /* Terminate string */

                /* save current profile SSID for displaying later */
                WF_CPGetSsid(1, (UINT8*)&CFGCXT.prevSSID, &ssidLen);
                // WF_CPGetSsid(ConnectionProfileID, (UINT8*)&CFGCXT.prevSSID, &ssidLen);
               
                CFGCXT.prevSSID[ssidLen] = 0;
                printf("\r\nSSID: %s",CFGCXT.ssid);
			}
			else
			{   //Invalid SSID... fail :-(
                printf("\r\nInvalid SSID...! ");
				goto ConfigFailure;
			}
		}

        //*************************************************************
        // Get the WLAN Mode: Adhoc / Infrastructure / SoftAP / Wifi Direct
        //*************************************************************
        else if(!strcmppgm2ram((char*)curHTTP.data, (ROM char*)"wlan"))
        {
            char mode[6];

            if (strlen((char*)(curHTTP.data+6)) > 5) /* Sanity check */
                goto ConfigFailure;

            memcpy(mode, (void*)(curHTTP.data+6), strlen((char*)(curHTTP.data+6)));
            mode[strlen((char*)(curHTTP.data+6))] = 0; /* Terminate string */
            if(!strcmppgm2ram((char*)mode, (ROM char*)"infra"))
            {
                printf("\r\nSetting mode to infrastructure! ");
                CFGCXT.type = WF_INFRASTRUCTURE;
            }
            else if(!strcmppgm2ram((char*)mode, "adhoc")) 
            {
                printf("\r\nSetting mode to adhoc! ");
                CFGCXT.type = WF_ADHOC;

                // Always setup adhoc to attempt to connect first, then start
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
                printf("\r\nConfig WLAN Mode Failure! ");
                goto ConfigFailure;
            }

            // save old WLAN mode
            // WF_CPGetNetworkType(ConnectionProfileID, &CFGCXT.prevWLAN);				
            WF_CPGetNetworkType(1, &CFGCXT.prevWLAN);                
            }
	}

    /* Check if WPA hasn't been selected with adhoc, if it has we choke! */
	/* Adhoc : Support only OPEN and WEP                                           */
	/* SoftAP: Support only OPEN and WEP (0x3107)                             */

    if (((CFGCXT.type == WF_ADHOC) &&
               ((CFGCXT.security == WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE) || (CFGCXT.security == WF_SECURITY_WPA_AUTO_WITH_KEY)))
     ||((CFGCXT.type == WF_SOFT_AP) &&
	           ((CFGCXT.security == WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE) || (CFGCXT.security == WF_SECURITY_WPA_AUTO_WITH_KEY))))
        goto ConfigFailure;

    /*  All parsing complete!  If we have got to here all data has been validated and
            we can handle what is necessary to start the reconfigure process of the WiFi device   */

    // Copy wifi cfg data to be committed
#if defined ( EZ_CONFIG_STORE )
    strcpy((char *)AppConfig.MySSID, (char *)CFGCXT.ssid);
    AppConfig.SsidLength = strlen((char*)(CFGCXT.ssid)); 

	/* Going to set security type */
    AppConfig.SecurityMode =  CFGCXT.security;  

    /* Going to save the key, if required */
    if (CFGCXT.security != WF_SECURITY_OPEN)
    {
        BYTE  key_size =0;

        switch ((BYTE)CFGCXT.security)
        {
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

        }
        memcpy(AppConfig.SecurityKey, CFGCXT.key, key_size);
        AppConfig.SecurityKey[strlen((char*)(CFGCXT.key))] = 0;   
		
    }

    /* Going to save the network type */
    AppConfig.networkType = CFGCXT.type; 
	AppConfig.dataValid = 1; /* Validate wifi configuration */

#endif // EZ_CONFIG_STORE

	// Set the board to reboot and display reconnecting information
	strcpypgm2ram((char*)curHTTP.data, "/reconnect.htm");   
	curHTTP.httpStatus = HTTP_REDIRECT;	

    /*
         * Set state here to inform that the Wifi device has config data and it is ready
         * to be acted upon.
         */
    printf("\r\nFlagging to start config change!\r\n");

	WF_START_EASY_CONFIG();

	return HTTP_IO_DONE;

ConfigFailure:
	lastFailure = TRUE;
	strcpypgm2ram((char*)curHTTP.data, "/error.htm");
	curHTTP.httpStatus = HTTP_REDIRECT;		

	return HTTP_IO_DONE;
}

#endif //(HTTP_USE_POST)


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

// ======================================
// = Sends the State of SW0             =
// ======================================
ROM BYTE HTML_UP_ARROW[] = "up";
ROM BYTE HTML_DOWN_ARROW[] = "dn";
void HTTPPrint_btn(WORD num)
{
    // Determine which button
    switch(num)
    {
        case 0:
            num = SW0_IO;
            break;
        default:
            num = 0;
    }

    // Print the output
    TCPPutROMString(sktHTTP, (num?HTML_UP_ARROW:HTML_DOWN_ARROW));
    return;
}

// ======================================
// = Sends the Requested LED's State    =
// ======================================
void HTTPPrint_led(WORD num)
{
    // Determine which LED requested
    switch(num)
    {
        case 0:
            num = LED0_IO;
            break;
        case 1:
            num = LED1_IO;
            break;
        case 2:
            num = LED2_IO;
            break;
        default:
            num = 0;
    }

    // Print the output
    TCPPut(sktHTTP, (num?'1':'0'));
    return;
}

// ======================================
// = Sends the IP Address    =
// ======================================
void HTTPPrint_ipaddr(void)
{
    char ipAddr[16];

    sprintf(ipAddr,"%u.%u.%u.%u",AppConfig.MyIPAddr.v[0], AppConfig.MyIPAddr.v[1],AppConfig.MyIPAddr.v[2],AppConfig.MyIPAddr.v[3]);
    TCPPutString(sktHTTP, (BYTE *)ipAddr);
}

// ================================
// = Kick-off a WiFi Scan Process =
// ================================
void HTTPPrint_scan(void)
{
    BYTE scanInProgressString[4];

    uitoa(IS_SCAN_IN_PROGRESS(SCANCXT.scanState), scanInProgressString);
    TCPPutString(sktHTTP, scanInProgressString);
}

// ======================================
// = Sends the FW version     =
// ======================================
void HTTPPrint_fwver(void)
{
    static BOOL firstTime = TRUE;
    static tWFDeviceInfo deviceInfo;
    BYTE fwVerString[8];
    int temp1;

    if (firstTime)
    {
        firstTime = FALSE;
        WF_GetDeviceInfo(&deviceInfo);  // only call this once, not continually
    }    

	temp1 = sprintf((char *)fwVerString,"%02x%02x", deviceInfo.romVersion , deviceInfo.patchVersion);
	TCPPutString(sktHTTP, fwVerString);	
}

// ======================================
// = Sends the new/current SSID setting =
// ======================================
void HTTPPrint_ssid(void)
{
    TCPPutArray(sktHTTP, AppConfig.MySSID, AppConfig.SsidLength);
}

// =====================================================
// = Sends the number of BSS entries found from a scan =
// =====================================================
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
        //TCPPutString(sktHTTP, bssDesc.ssid);
        if(strlen((const char*)bssDesc.ssid)<32)
            TCPPutString(sktHTTP, bssDesc.ssid);
        else
        {
            unsigned char buf_tmp[33];
            int i;
            for(i=0;i<32;i++) buf_tmp[i] = bssDesc.ssid[i];
            buf_tmp[32] = 0;
            TCPPutString(sktHTTP, buf_tmp);
        }
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE *)"0");
    }        
}    


// ======================================
// = Sends the Current Security Setting =
// ======================================

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

// ===========================================
// = Sends the new/current WLAN setting type =
// ===========================================
void HTTPPrint_wlan(void)
{
	BYTE bssTypeString[4];

	// Index.htm differs between EZConfig and wifi g demo in that index.htm
	// in wifi g demo displays BSS information of MRF24WG.		

	if (bssDescIsValid)
	{
		if(IS_SCAN_STATE_DISPLAY(SCANCXT.scanState)) 
		{
			uitoa(bssDesc.bssType, bssTypeString);
			TCPPutString(sktHTTP, bssTypeString);	
		}
		else
		{
			if(AppConfig.networkType == WF_ADHOC)
				TCPPutROMString(sktHTTP, (ROM BYTE*)"AdHoc (IBSS)");
			else if (AppConfig.networkType == WF_INFRASTRUCTURE)
				TCPPutROMString(sktHTTP, (ROM BYTE*)"Infrastructure (BSS)");
			else if (AppConfig.networkType == WF_SOFT_AP)
				TCPPutROMString(sktHTTP, (ROM BYTE*)"SoftAP (BSS)");
			else if (AppConfig.networkType == WF_P2P)
				TCPPutROMString(sktHTTP, (ROM BYTE*)"WiFi Direct P2P (BSS)");
			else
				TCPPutROMString(sktHTTP, (ROM BYTE*)"Unknown BSS");
		}		
	}	
	else
	{	 		
		if(AppConfig.networkType == WF_ADHOC)
			TCPPutROMString(sktHTTP, (ROM BYTE*)"AdHoc (IBSS)");
		else if (AppConfig.networkType == WF_INFRASTRUCTURE)
			TCPPutROMString(sktHTTP, (ROM BYTE*)"Infrastructure (BSS)");
		else if (AppConfig.networkType == WF_SOFT_AP)
			TCPPutROMString(sktHTTP, (ROM BYTE*)"SoftAP (BSS)");
		else if (AppConfig.networkType == WF_P2P)
			TCPPutROMString(sktHTTP, (ROM BYTE*)"WiFi Direct P2P (BSS)");
		else
			TCPPutROMString(sktHTTP, (ROM BYTE*)"Unknown BSS");
	}	 

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

// ==============================
// = Sends the old SSID setting =
// ==============================
void HTTPPrint_prevSSID(void)
{
    TCPPutString(sktHTTP, (BYTE*)CFGCXT.prevSSID);
}

// ===================================
// = Sends the old WLAN setting type =
// ===================================
void HTTPPrint_prevWLAN(void)
{
    if (CFGCXT.prevWLAN == WF_INFRASTRUCTURE)
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Infrastructure (BSS)");	
    else if (CFGCXT.prevWLAN == WF_SOFT_AP)
        TCPPutROMString(sktHTTP, (ROM BYTE*)"SoftAP (BSS)");
    else if (CFGCXT.prevWLAN == WF_ADHOC)
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Adhoc (IBSS)");
    else if (CFGCXT.prevWLAN == WF_P2P)
        TCPPutROMString(sktHTTP, (ROM BYTE*)"WiFi Direct P2P (BSS)");
    else
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Unknown BSS");
}

void HTTPPrint_nextWLAN(void)
{
    if (CFGCXT.type == WF_INFRASTRUCTURE)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Infrastructure (BSS)");
    }
    else if (CFGCXT.type == WF_ADHOC)
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"Adhoc (IBSS)");
    }
    else if (CFGCXT.type == WF_SOFT_AP)
    {     
        TCPPutROMString(sktHTTP, (ROM BYTE*)"SoftAP (BSS)");
    }
    else
    {
        TCPPutROMString(sktHTTP, (ROM BYTE*)"unknown");
    }
}

// ==========================
// = Sends the Demo Version =
// ==========================
void HTTPPrint_Demoversion(void)
{
    TCPPutROMString(sktHTTP, (ROM void*)DEMO_VERSION);
}

// ==============================================
// = Provides the XML list of WiFi Scan Results =
// ==============================================
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
        WFRetrieveScanResult(x, &bssDesc);

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
}

#endif
