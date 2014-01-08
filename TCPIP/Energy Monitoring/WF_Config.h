/******************************************************************************

 MRF24WB0M Driver Customization
 Module for Microchip TCP/IP Stack
  -Provides access to MRF24WB0M WiFi controller
  -Reference: MRF24WB0M Data sheet, IEEE 802.11 Standard

*******************************************************************************
 FileName:		WF_Config.h
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
 KH                 27 Jan 2010 Created for MRF24WB0M
******************************************************************************/


#ifndef __WF_CONFIG_H_
#define __WF_CONFIG_H_

/*
*********************************************************************************************************
*                                           DEFINES                               
*********************************************************************************************************
*/

/*----------------------------------------------------------------------------*/
/* This block of defines allows for code and data reduction by removing       */
/* WiFi driver code and or data that is not needed by the application.        */
/* Comment out those function blocks that are not needed.                     */
/*----------------------------------------------------------------------------*/
#define WF_USE_SCAN_FUNCTIONS
#define WF_USE_TX_POWER_CONTROL_FUNCTIONS
#define WF_USE_POWER_SAVE_FUNCTIONS
#define WF_USE_MULTICAST_FUNCTIONS
#define WF_USE_INDIVIDUAL_SET_GETS
#define WF_USE_GROUP_SET_GETS
//#define WF_USE_DATA_TX_RX_FUNCTIONS
//#define USE_GRATUITOUS_ARP



/*= WF_ASSERT MACRO ==================================================================*/
/* Customize how the WiFi driver assert macro (WF_ASSERT) should operate.             */
/*  To DISABLE the WF_ASSERT macro: Comment out '#define WF_DEBUG'                    */
/*  To ENABLE the WF_ASSERT macro:  Unomment out '#define WF_DEBUG'                   */
/*====================================================================================*/
#define WF_DEBUG 

/*= WF_CONSOLE =======================================================================*/
/* Customize whether the WiFi Driver supports a serial console application            */
/*  To DISABLE the serial console application: Comment out '#define WF_CONSOLE'       */
/*  To ENABLE the serial console application: Uncomment out '#define WF_CONSOLE'      */
/*====================================================================================*/
//#define WF_CONSOLE              /* needed for console demo */
//#define WF_CONSOLE_IFCFGUTIL    /* needed for console demo */


/*--------------------------------------------*/
/* Default settings for Connection Management */
/*--------------------------------------------*/
#define MY_DEFAULT_SSID_NAME                "MicrochipDemoAP"

#define MY_DEFAULT_NETWORK_TYPE             WF_INFRASTRUCTURE   /* WF_INFRASTRUCTURE or WF_ADHOC     */

#define MY_DEFAULT_SCAN_TYPE                WF_ACTIVE_SCAN      /* WF_ACTIVE_SCAN or WF_PASSIVE_SCAN */

#define MY_DEFAULT_CHANNEL_LIST             {1,6,11}            /* use {} to scan all channels       */

#define MY_DEFAULT_LIST_RETRY_COUNT_ADHOC           (3)                 /* Number of times to try to connect to the SSID when using Ad/Hoc network type */
#define MY_DEFAULT_LIST_RETRY_COUNT_INFRASTRUCTURE  (WF_RETRY_FOREVER)  /* Number of times to try to connect to the SSID when using Infrastructure network type */

#define MY_DEFAULT_EVENT_NOTIFICATION_LIST  (WF_NOTIFY_CONNECTION_ATTEMPT_SUCCESSFUL  |         \
                                             WF_NOTIFY_CONNECTION_ATTEMPT_FAILED      |         \
                                             WF_NOTIFY_CONNECTION_TEMPORARILY_LOST    |         \
                                             WF_NOTIFY_CONNECTION_PERMANENTLY_LOST    |         \
                                             WF_NOTIFY_CONNECTION_REESTABLISHED)

#define MY_DEFAULT_PS_POLL                   WF_DISABLED         /* WF_DISABLED or WF_ENABLED */

#define MY_DEFAULT_WIFI_SECURITY_MODE        WF_SECURITY_OPEN

//#define USE_MRF24W_HOST_BUFFER

//#define STACK_USE_EZ_CONFIG
//#define EZ_CONFIG_SCAN
//#define EZ_CONFIG_STALL
//#define EZ_CONFIG_STORE




/*****************************************************************************/
/*****************************************************************************/
/*                             WIFI SECURITY COMPILE-TIME DEFAULTS           */
/*****************************************************************************/
/*****************************************************************************/
// Security modes available on WiFi network:
//   WF_SECURITY_OPEN                      : No security
//   WF_SECURITY_WEP_40                    : WEP Encryption using 40 bit keys
//   WF_SECURITY_WEP_104                   : WEP Encryption using 104 bit keys
//   WF_SECURITY_WPA_WITH_KEY              : WPA-PSK Personal where binary key is given to MRF24WB0M 
//   WF_SECURITY_WPA_WITH_PASS_PHRASE      : WPA-PSK Personal where passphrase is given to MRF24WB0M and it calculates the binary key
//   WF_SECURITY_WPA2_WITH_KEY             : WPA2-PSK Personal where binary key is given to MRF24WB0M 
//   WF_SECURITY_WPA2_WITH_PASS_PHRASE     : WPA2-PSK Personal where passphrase is given to MRF24WB0M and it calculates the binary key
//   WF_SECURITY_WPA_AUTO_WITH_KEY         : WPA-PSK Personal or WPA2-PSK Personal where binary key is given and MRF24WB0M will 
//                                             connect at highest level AP supports (WPA or WPA2)                                                
//   WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE : WPA-PSK Personal or WPA2-PSK Personal where passphrase is given to MRF24WB0M and it 
//                                             calculates the binary key and connects at highest level AP supports (WPA or WPA2)

// Default pass phrase used for WF_SECURITY_WPA_WITH_PASS_PHRASE and 
// WF_SECURITY_WPA2_WITH_PASS_PHRASE security modes
#define MY_DEFAULT_PSK_PHRASE               "Microchip 802.11 Secret PSK Password"


// If using security mode of WF_SECURITY_WPA_WITH_KEY or WF_SECURITY_WPA2_WITH_KEY, then this section 
// must be set to  match the key for MY_DEFAULT_SSID_NAME and MY_DEFAULT_PSK_PHRASE
// combination.  The values below are derived from the SSID "MicrochipDemoAP" and the pass phrase
// "Microchip 802.11 Secret PSK Password".
// The tool at http://www.wireshark.org/tools/wpa-psk.html can be used to generate this field. 
#define MY_DEFAULT_PSK "\
\x86\xC5\x1D\x71\xD9\x1A\xAA\x49\
\x40\xC8\x88\xC6\xE9\x7A\x4A\xD5\
\xE5\x6D\xDA\x44\x8E\xFB\x9C\x0A\
\xE1\x47\x81\x52\x31\x1C\x13\x7C"


//-----------------------------------------------------------------------------------
// Default WEP keys used in WF_SECURITY_WEP_40  and WF_SECURITY_WEP_104 security mode
//-----------------------------------------------------------------------------------
#define MY_DEFAULT_WEP_PHRASE           "WEP Phrase"

// string 4 40-bit WEP keys -- corresponding to passphraseof "WEP Phrase"
#define MY_DEFAULT_WEP_KEYS_40 "\
\x5a\xfb\x6c\x8e\x77\
\xc1\x04\x49\xfd\x4e\
\x43\x18\x2b\x33\x88\
\xb0\x73\x69\xf4\x78"

// string containing 4 104-bit WEP keys -- corresponding to passphraseof "WEP Phrase"
#define MY_DEFAULT_WEP_KEYS_104 "\
\x90\xe9\x67\x80\xc7\x39\x40\x9d\xa5\x00\x34\xfc\xaa\
\x77\x4a\x69\x45\xa4\x3d\x66\x63\xfe\x5b\x1d\xb9\xfd\
\x82\x29\x87\x4c\x9b\xdc\x6d\xdf\x87\xd1\xcf\x17\x41\
\xcc\xd7\x62\xde\x92\xad\xba\x3b\x62\x2f\x7f\xbe\xfb"


/* Valid Key Index: 0, 1, 2, 3  */
#define MY_DEFAULT_WEP_KEY_INDEX        (0)


#endif /* __WF_CONFIG_H_ */


