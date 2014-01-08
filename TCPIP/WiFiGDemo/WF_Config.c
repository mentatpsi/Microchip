/******************************************************************************

 MRF24W Driver Customization
 Module for Microchip TCP/IP Stack
  -Provides access to MRF24W WiFi controller
  -Reference: MRF24WG0MA/B Data sheet, IEEE 802.11 Standard

*******************************************************************************
 FileName:		WF_Config.c
 Dependencies:	TCP/IP Stack header files
 Processor:            PIC32MX695F512H
 Compiler:             Microchip  XC32 Compiler
 Company:		Microchip Technology, Inc.

 Software License Agreement

 Copyright (C) 2002-2013 Microchip Technology Inc.  All rights reserved.

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 AO                 10/22/2012 Modified for WiFi G Demo Board and integrate into MLA
******************************************************************************/


/*==========================================================================*/
/*                                  INCLUDES                                */
/*==========================================================================*/
#include "HardwareProfile.h"
#include "TCPIP Stack/TCPIP.h"

#if defined ( EZ_CONFIG_SCAN )
#include "TCPIP Stack/WFEasyConfig.h"
#endif /* EZ_CONFIG_SCAN */

// Differences to wifi comm demo board (MRF24WB0MA) :
//        Wifi comm demo board is centered on variable CPElements.
//        Wifi G demo board is centered on variable AppConfig, since this is the generic approach adopted by 
//        TCPIP demo/ezconsole/ezconfig apps.


/*==========================================================================*/
/*                                  DEFINES                                 */
/*==========================================================================*/

extern UINT8 g_scan_done;

/*****************************************************************************
 * FUNCTION: void WF_Connect(void)
 *
 * RETURNS:  None
 *
 * PARAMS:   none
 *
 * NOTES:    This routine creates a Connection Profile Entry, Initializes it
 *                then creates the connection algorithm and establishes the WiFi
 *                connection according to the Profile settings.
 *                Customize this function as needed for your application.
 *
 *                Wifi Direct, ensure following parameters are set up properly
 *                  - AppConfig.MySSID =  "DIRECT-"
 *                  - sizeof(channelList) == 3)
 *                  - channelList[0] == 1
 *                  - channelList[1] == 6
 *                  - channelList[2] == 11
 *
 *****************************************************************************/
void WF_Connect(void)
{
    UINT8 ConnectionProfileID;
    UINT8 channelList[] = MY_DEFAULT_CHANNEL_LIST;
    UINT8 channelList_postscan[] = MY_DEFAULT_CHANNEL_LIST_POSTSCAN;
	 
    /* create a Connection Profile */
    WF_CPCreate(&ConnectionProfileID);
	
    // Enables or disables the MRF24W Regional Domain with RF module FW version 0x3106 or earlier
    // With RF module FW version 0x3107 and future releases, this function is NOT supported due to changes
    // in FCC requirements, which does not allow programming of the regional domain. 
    WF_SetRegionalDomain(MY_DEFAULT_DOMAIN);  

    if ((AppConfig.networkType == WF_SOFT_AP) || (AppConfig.networkType == WF_ADHOC))
    {
          /************************************************************/
          /* Append Last 4 digits to MAC address to SSID - Creating unique SSID */
          /* Wifi comm demo SSID : MCHP_xxxx                                                */
          /* Wifi G demo        SSID : MCHP_G_xxxx                                            */
          /************************************************************/
          sprintf((char *)AppConfig.MySSID,"MCHP_G_%02x%02x", AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
          sprintf((char *)AppConfig.NetBIOSName,"%s%02x%02x", MY_DEFAULT_HOST_NAME, AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
    }
    AppConfig.SsidLength =  strlen((char*)(AppConfig.MySSID));

    WF_CPSetSsid(ConnectionProfileID, AppConfig.MySSID, AppConfig.SsidLength);
     
    WF_CPSetNetworkType(ConnectionProfileID, AppConfig.networkType);
    if (AppConfig.networkType == WF_ADHOC)
    {
        WF_CPSetAdHocBehavior(ConnectionProfileID, WF_ADHOC_CONNECT_THEN_START);
    }

    // Delay10us(10);  // Determine if needed later as security data is a large packet. MRF24WB0M is found not working on selected PIC24.
	
	switch(AppConfig.SecurityMode) {
		case WF_SECURITY_OPEN:
			WF_CPSetSecurity(ConnectionProfileID, WF_SECURITY_OPEN, 0, NULL, 0);
			break;
		case WF_SECURITY_WEP_40:
			// assume key 0
			WF_CPSetSecurity(ConnectionProfileID, AppConfig.SecurityMode, 0, AppConfig.SecurityKey, 5);
			break;
		case WF_SECURITY_WEP_104:
			// assume key 0
			WF_CPSetSecurity(ConnectionProfileID, AppConfig.SecurityMode, 0, AppConfig.SecurityKey, 13);
			break;
		case WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:
			WF_CPSetSecurity(ConnectionProfileID, WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE, 
							 0, AppConfig.SecurityKey, strlen((char*)AppConfig.SecurityKey));
			break;
		case WF_SECURITY_WPA_AUTO_WITH_KEY:
			WF_CPSetSecurity(ConnectionProfileID, WF_SECURITY_WPA_AUTO_WITH_KEY,
							 0, AppConfig.SecurityKey, 32);
			break;
		default:
			putrsUART("\r\n\r\nCaptain this should NOT happen.\r\n\r\n");

	}

    WF_CASetScanType(MY_DEFAULT_SCAN_TYPE);
    
    if (((CFGCXT.type!=WF_SOFT_AP)&&(CFGCXT.prevWLAN==WF_SOFT_AP)) || (AppConfig.networkType!=WF_SOFT_AP))
    {
        WF_CASetChannelList(channelList_postscan, sizeof(channelList_postscan));
    }
    else
    {
        WF_CASetChannelList(channelList, sizeof(channelList));
    }
    
    // The Retry Count parameter tells the WiFi Connection manager how many attempts to make when trying
    // to connect to an existing network.  In the Infrastructure case, the default is to retry forever so that
    // if the AP is turned off or out of range, the radio will continue to attempt a connection until the
    // AP is eventually back on or in range.  In the Adhoc case, the default is to retry 3 times since the 
    // purpose of attempting to establish a network in the Adhoc case is only to verify that one does not
    // initially exist.  If the retry count was set to WF_RETRY_FOREVER in the AdHoc mode, an AdHoc network
    // would never be established.  The constants MY_DEFAULT_LIST_RETRY_COUNT_ADHOC and 
    // MY_DEFAULT_LIST_RETRY_COUNT_INFRASTRUCTURE have been created specifically for the June 2011 MAL release.
    WF_CASetListRetryCount(RETRY_COUNT);
	
    WF_CASetEventNotificationAction(MY_DEFAULT_EVENT_NOTIFICATION_LIST);
    
#if defined(WF_USE_POWER_SAVE_FUNCTIONS)
	#if (MY_DEFAULT_PS_POLL == WF_ENABLED)
            WF_PsPollEnable(TRUE);
	#else
            WF_PsPollDisable();    
	#endif
#endif

    WF_CASetBeaconTimeout(MY_DEFAULT_BEACON_TIMEOUT);
	
#if defined(STACK_USE_UART)   
    WF_OutputConnectionInfo(&AppConfig);
#endif

#if (SOFTAP_CHECK_LINK_STATUS == WF_ENABLED)
    WF_SetLinkDownThreshold(SOFTAP_LINK_FAILURE_THRESHOLD);
#endif

    // Initiates connection to BSS 
    WF_CMConnect(ConnectionProfileID);
}


/*****************************************************************************
 * FUNCTION: WF_ProcessEvent
 *
 * RETURNS:  None
 *
 * PARAMS:   event      -- event that occurred
 *           eventInfo  -- additional information about the event.  Not all events
 *           have associated info, in which case this value will be
 *           set to WF_NO_ADDITIONAL_INFO (0xff)
 *           extraInfo - more additional information about the event
 *
 *  NOTES:   The Host application must NOT directly call this function.  This
 *           function is called by the WiFi Driver code when an event occurs
 *           that may need processing by the Host CPU.
 *
 *           No other WiFi Driver function should be called from this function, with the
 *           exception of WF_ASSERT.  It is recommended that if the application wishes to be
 *           notified of an event that it simply set a flag and let application code in the
 *           main loop handle the event.
 *
 *           WFSetFuncState must be called when entering and leaving this function.
 *           When WF_DEBUG is enabled this allows a runtime check if any illegal WF functions
 *           are called from within this function.
 *
 *           For events that the application is not interested in simply leave the
 *           case statement empty.
  *
 *           Customize this function as needed for your application.
 *****************************************************************************/
void WF_ProcessEvent(UINT8 event, UINT16 eventInfo, UINT8 *extraInfo)
{
    #if defined(STACK_USE_UART)
		char buf[8];
    #endif
		tMgmtIndicateSoftAPEvent *softAPEvent;
	
		/* this function tells the WF driver that we are in this function */
		WFSetFuncState(WF_PROCESS_EVENT_FUNC, WF_ENTERING_FUNCTION);
		  
		switch (event)
		{
			/*--------------------------------------*/
			case WF_EVENT_CONNECTION_SUCCESSFUL:
			/*--------------------------------------*/	 	
            #if defined(EZ_CONFIG_STORE)
				AppConfig.saveSecurityInfo = TRUE;
            #endif	
				break;
			
			/*--------------------------------------*/			  
			case WF_EVENT_CONNECTION_FAILED:
			case WF_EVENT_CONNECTION_TEMPORARILY_LOST:
			case WF_EVENT_CONNECTION_PERMANENTLY_LOST:			  
			/*--------------------------------------*/
				break;
	
			/*--------------------------------------*/	  
			case WF_EVENT_CONNECTION_REESTABLISHED:
			/*--------------------------------------*/
#if 0  //This code will cause assert, so I mask them  -- Jian Wan
            #if defined(WF_USE_POWER_SAVE_FUNCTIONS)
				{
					BOOL PsPollEnabled;
					
					PsPollEnabled = (MY_DEFAULT_PS_POLL == WF_ENABLED);
					if (!PsPollEnabled)
					{	 
						/* disable low power (PS-Poll) mode */
                    #if defined(STACK_USE_UART)
						putrsUART("Disable PS-Poll\r\n");		 
                    #endif
						WF_PsPollDisable();
					}	 
					else
					{
						/* Enable low power (PS-Poll) mode */
                    #if defined(STACK_USE_UART)
						putrsUART("Enable PS-Poll\r\n");		
                    #endif
						WF_PsPollEnable(TRUE);
					}	
				}	  
            #endif
#endif
				break;
				
			/*--------------------------------------*/
			case WF_EVENT_SCAN_RESULTS_READY:
			/*--------------------------------------*/		
            #if defined ( EZ_CONFIG_SCAN ) 
				WFScanEventHandler(eventInfo);
            #endif /* EZ_CONFIG_SCAN */
				g_scan_done = 1;	// WF_PRESCAN		   
				break;

            		case WF_EVENT_SOFT_AP_EVENT:
                		softAPEvent = (tMgmtIndicateSoftAPEvent *)extraInfo;
                		#if defined(STACK_USE_UART) 
                		{
                    			char str[96];
                    			char *result = "None";
                    			char *reason = "None";
                    			UINT8 *addr = softAPEvent->address;
                    			putrsUART("Event: SoftAP, ");
                    			if (softAPEvent->event == SOFTAP_EVENT_CONNECTED) {
                        			result = "Connected";
                    			} else if (softAPEvent->event == SOFTAP_EVENT_DISCONNECTED) {
                        			result = "Disconnected";
                        			if (softAPEvent->reason == SOFTAP_EVENT_LINK_LOST)
                            				reason = "LinkLost";
                        			else if (softAPEvent->reason == SOFTAP_EVENT_RECEIVED_DEAUTH)
                            				reason = "ReceivedDeauth";
                    			}
                    			sprintf(str, "%s, %s, %x:%x:%x:%x:%x:%x", result, reason, addr[0], addr[1], addr[2],
                        			addr[3], addr[4], addr[5]);
                    			putsUART(str);
                    			putrsUART("\r\n");
                		}
                		#endif /* STACK_USE_UART */
                		break;
				
			default:
				WF_ASSERT(FALSE);  /* unknown event */
				break;
		}		 
		
		/* Informs the WF driver that we are leaving this function */
		WFSetFuncState(WF_PROCESS_EVENT_FUNC, WF_LEAVING_FUNCTION);
}
