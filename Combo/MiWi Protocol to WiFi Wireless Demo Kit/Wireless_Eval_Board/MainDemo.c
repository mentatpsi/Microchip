/*********************************************************************
 *
 *  Main Application Entry Point and TCP/IP Stack Demo
 *  Module for Microchip TCP/IP Stack
 *   -Demonstrates how to call and use the Microchip TCP/IP stack
 *	 -Reference: Microchip TCP/IP Stack Help (TCPIP Stack Help.chm)
 *
 *********************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    TCPIP.h
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.11b or higher
 *					Microchip C30 v3.24 or higher
 *					Microchip C18 v3.36 or higher
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
 * File Description:
 * Change History:
 * Rev   Description
 * ----  -----------------------------------------
 * 1.0   Initial release
 * V5.36 ---- STACK_USE_MPFS support has been removed 
 ********************************************************************/
/*
 * This macro uniquely defines this file as the main entry point.
 * There should only be one such definition in the entire project,
 * and this file must define the AppConfig variable as described below.
 */
#define THIS_IS_STACK_APPLICATION

#pragma config FNOSC = FRCPLL, FPLLIDIV = DIV_2, FPLLMUL = MUL_20, FPLLODIV = DIV_2, FPBDIV = DIV_1, FWDTEN = OFF, POSCMOD = OFF, FSOSCEN = OFF, CP = OFF

// Include all headers for any enabled TCPIP Stack functions
#include "TCPIP Stack/TCPIP.h"
#include "WF_Config.h"

#if defined(STACK_USE_ZEROCONF_LINK_LOCAL)
#include "TCPIP Stack/ZeroconfLinkLocal.h"
#endif
#if defined(STACK_USE_ZEROCONF_MDNS_SD)
#include "TCPIP Stack/ZeroconfMulticastDNS.h"
#endif

// Include functions specific to this stack application
#include "MainDemo.h"

//Include MiWi Related Includes
#include "ConfigApp.h"
#include "SystemProfile.h"
#include "WirelessProtocols/MCHP_API.h"
#include "Transceivers/Transceivers.h"
#include "LCD_ST7032.h"
#include "Storage.h"
#include "SelfTestMode.h"


// Used for Wi-Fi assertions
#define WF_MODULE_NUMBER   WF_MODULE_MAIN_DEMO



#if defined( WF_CONSOLE )
#include "TCPIP Stack/WFConsole.h"
#endif 

/*************************************************************************/
// The variable myChannel defines the channel that the device
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).
/*************************************************************************/

// MiWi Possible channel numbers are from 11 to 26 for MRF24J40
BYTE myChannel = 11;

#define MiWi_CHANNEL        MiWi_CHANNEL_11

#define MiWi_CHANNEL_26     0x04000000
#define MiWi_CHANNEL_23     0x00800000
#define MiWi_CHANNEL_21     0x00200000
#define MiWi_CHANNEL_19     0x00080000
#define MiWi_CHANNEL_15     0x00008000
#define MiWi_CHANNEL_13     0x00002000
#define MiWi_CHANNEL_11     0x00000800

#define PIC32MX_SPI1_SDO_SCK_MASK_VALUE      (0x00000140)
#define PIC32MX_SPI1_SDI_MASK_VALUE          (0x00000080)
//#define PIC32MX_INT2_MASK_VALUE              (0x00000010)
//#define PIC32MX_INT1_MASK_VALUE              (0x00000008)
#define PIC32MX_INT1_MASK_VALUE              (0x00000100)
/* MAX SPI CLOCK FREQ SUPPORTED FOR MIWI TRANSCIEVER */
#define MAX_SPI_CLK_FREQ_FOR_P2P             (10000000) //For MRF24J40 10MHz, for MRF89XA 1MHz



BOOL NetFreezerEnable = FALSE;
BYTE connectionAddress[3];

extern BYTE myLongAddress[];


#define SPI_READ_ID 0x90

/*************************************************************************/
// AdditionalNodeID variable array defines the additional
// information to identify a device on a PAN. This array
// will be transmitted when initiate the connection between
// the two devices. This  variable array will be stored in
// the Connection Entry structure of the partner device. The
// size of this array is ADDITIONAL_NODE_ID_SIZE, defined in
// ConfigApp.h.
// In this demo, this variable array is set to be empty.
/*************************************************************************/
#if ADDITIONAL_NODE_ID_SIZE > 0
BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x00};
#endif



//--------------------------------------------------------------------------------
// These defines use the C preprocessor to create the AppConfig structure elements
//--------------------------------------------------------------------------------
// MDEF(R,N) macro concatenates tokens to form "MY_DEFAULT_<R>BYTE<N>"
// For example MDEF(IP_ADDR_,4) expands to "MY_DEFAULT_IP_ADDR_BYTE4"

#define MDEF(R,N) MY_DEFAULT_ ## R ## BYTE ## N

// BUILD_MYDEFAULT32 invokes the MDEF(R,N) macro 4 times to create a 32 bit
// value from 4 byte values.
//
// For example BUILD_MYDEFAULT32(IP_ADDR_) expands to
// (MY_DEFAULT_IP_ADDR_BYTE4<<24|MY_DEFAULT_IP_ADDR_BYTE3<<16
//  |MY_DEFAULT_IP_ADDR_BYTE2<<8|MY_DEFAULT_IP_ADDR_BYTE1)

#define BUILD_MYDEFAULT32(R) (MDEF(R,4)<<24|MDEF(R,3)<<16|MDEF(R,2)<<8|MDEF(R,1))

// MACD(N) expands to MY_DEFAULT_MAC_BYTE<N>
// For example MACD(1) expands to MY_DEFAULT_MAC_BYTE1

#define MACD(N) MY_DEFAULT_MAC_BYTE ## N


// Application-dependent structure used to contain IP address information
APP_CONFIG AppConfig ={
    {BUILD_MYDEFAULT32(IP_ADDR_)},
    {BUILD_MYDEFAULT32(MASK_)},
    {BUILD_MYDEFAULT32(GATE_)},
    {BUILD_MYDEFAULT32(PRIMARY_DNS_)},
    {BUILD_MYDEFAULT32(SECONDARY_DNS_)},
    {BUILD_MYDEFAULT32(IP_ADDR_)},
    {BUILD_MYDEFAULT32(MASK_)},
    MY_DEFAULT_HOST_NAME,
    {1, 1}, // Flags
    {
        {MACD(1), MACD(2), MACD(3), MACD(4), MACD(5), MACD(6)}}
};


// Declare AppConfig structure and some other supporting stack variables
//APP_CONFIG AppConfig;
static unsigned short wOriginalAppConfigChecksum; // Checksum of the ROM defaults for AppConfig
BYTE AN0String[8];


#if (MY_DEFAULT_NETWORK_TYPE == WF_SOFT_AP)
UINT8 g_scan_done = 0; // WF_PRESCAN   This will be set wheneven event scan results are ready.
UINT8 g_prescan_waiting = 1; // WF_PRESCAN   This is used only to allow POR prescan once.
#endif

// Private helper functions.
// These may or may not be present in all applications.
static void InitAppConfig(void);
static void InitializeBoard(void);


#if defined(WF_CS_TRIS)
void WF_Connect(void);
#endif

void SpiInitDevice(SpiChannel chn, int isMaster, int frmEn, int frmMaster);


#if defined(__C32__)

void _general_exception_handler(unsigned cause, unsigned status) {
    Nop();
    Nop();
}
#endif


#if defined(WF_CS_TRIS)
// Global variables
UINT8 ConnectionProfileID;
#endif

/*********************************************************************
* Function:         void main(void)
*
* PreCondition:     none
*
* Input:            none
*
* Output:	    none
*
* Side Effects:	    none
*
* Overview:         This is the main function that runs the Wireless Gateway
*                   example demo. The purpose of this example is to
*                   demonstrate bridge functionality between MiWi and WiFi
*
*
*
* Note:
**********************************************************************/
#if defined(__18CXX)
void main(void)
#else

int main(void)
#endif
{
    BYTE i;
    static DWORD t = 0;
    static DWORD dwLastIP = 0;
    static UINT8 updateDisplay = 0;

#if defined (EZ_CONFIG_STORE)
    static DWORD ButtonPushStart = 0;
#endif

#if (MY_DEFAULT_NETWORK_TYPE == WF_SOFT_AP)
    UINT8 channelList[] = MY_DEFAULT_CHANNEL_LIST_PRESCAN; // WF_PRESCAN
    tWFScanResult bssDesc;
#endif

    // Initialize application specific hardware
    InitializeBoard();

    // Initiates board setup process if button is depressed
    // on startup
    if (BUTTON1_IO == 0u) {
        while (BUTTON1_IO == 0);
        SelfTestMode();
    }

    //#if defined(USE_LCD)



    /*******************************************************************/
    // Initialize the LCD
    /*******************************************************************/

    ConfigureLCD_SPI();

    LCDInit();

    /*******************************************************************/
    // Display Start-up Splash Screen
    /*******************************************************************/
    LCDBacklightON();
    LEDS_ON();
    LCDErase();
    sprintf((char *) LCDText, (char*) "  MiWi - WiFi  ");
    sprintf((char *) &(LCDText[16]), (char*) " Gateway  Demo");
    LCDUpdate();





    /*******************************************************************/
    // Initialize the MiWi Protocol Stack. The only input parameter indicates
    // if previous network configuration should be restored.
    /*******************************************************************/
    MiApp_ProtocolInit(FALSE);

    /*******************************************************************/
    // Set Device Communication Channel
    /*******************************************************************/
    if (MiApp_SetChannel(myChannel) == FALSE) {
        LCDDisplay((char *) "ERROR: Unable to Set Channel..", 0, TRUE);
        while (1);
    }

    /*******************************************************************/
    //  Set the connection mode. The possible connection modes are:
    //      ENABLE_ALL_CONN:    Enable all kinds of connection
    //      ENABLE_PREV_CONN:   Only allow connection already exists in
    //                          connection table
    //      ENABL_ACTIVE_SCAN_RSP:  Allow response to Active scan
    //      DISABLE_ALL_CONN:   Disable all connections.
    /*******************************************************************/
    MiApp_ConnectionMode(ENABLE_ALL_CONN);

    /*******************************************************************/
    // Function MiApp_EstablishConnection try to establish a new
    // connection with peer device.
    // The first parameter is the index to the active scan result,
    //      which is acquired by discovery process (active scan). If
    //      the value of the index is 0xFF, try to establish a
    //      connection with any peer.
    // The second parameter is the mode to establish connection,
    //      either direct or indirect. Direct mode means connection
    //      within the radio range; indirect mode means connection
    //      may or may not in the radio range.
    /*******************************************************************/
    i = MiApp_EstablishConnection(0xFF, CONN_MODE_DIRECT);

    /*******************************************************************/
    // Display current opertion on LCD of demo board, if applicable
    /*******************************************************************/
    if (i != 0xFF) {
        ; // Connected Peer on Channel
    } else {
        /*******************************************************************/
        // If no network can be found and join, we need to start a new
        // network by calling function MiApp_StartConnection
        //
        // The first parameter is the mode of start connection. There are
        // two valid connection modes:
        //   - START_CONN_DIRECT        start the connection on current
        //                              channel
        //   - START_CONN_ENERGY_SCN    perform an energy scan first,
        //                              before starting the connection on
        //                              the channel with least noise
        //   - START_CONN_CS_SCN        perform a carrier sense scan
        //                              first, before starting the
        //                              connection on the channel with
        //                              least carrier sense noise. Not
        //                              supported for current radios
        //
        // The second parameter is the scan duration, which has the same
        //     definition in Energy Scan. 10 is roughly 1 second. 9 is a
        //     half second and 11 is 2 seconds. Maximum scan duration is
        //     14, or roughly 16 seconds.
        //
        // The third parameter is the channel map. Bit 0 of the
        //     double word parameter represents channel 0. For the 2.4GHz
        //     frequency band, all possible channels are channel 11 to
        //     channel 26. As the result, the bit map is 0x07FFF800. Stack
        //     will filter out all invalid channels, so the application
        //     only needs to pay attention to the channels that are not
        //     preferred.
        /*******************************************************************/
        MiApp_StartConnection(START_CONN_DIRECT, 10, 0);
    }

    // Turn OFF LCD after setting up MiWi Connection
    LCDBacklightOFF();

    // Initialize stack-related hardware components that may be
    // required by the UART configuration routines
    TickInit();
#if defined(STACK_USE_MPFS2)
    MPFSInit();
#endif

    // Initialize Stack and application related NV variables into AppConfig.
    InitAppConfig();
    dwLastIP = AppConfig.MyIPAddr.Val;
    // Initialize core stack layers (MAC, ARP, TCP, UDP) and
    // application modules (HTTP, SNMP, etc.)
    StackInit();

#if defined ( EZ_CONFIG_SCAN )
    WFInitScan();
#endif

#if (MY_DEFAULT_NETWORK_TYPE == WF_SOFT_AP)
    // WF_PRESCAN: Pre-scan before starting up as SoftAP mode  
    WF_CASetScanType(MY_DEFAULT_SCAN_TYPE);
    WF_CASetChannelList(channelList, sizeof (channelList));

    if (WFStartScan() == WF_SUCCESS) {
        SCAN_SET_DISPLAY(SCANCXT.scanState);
        SCANCXT.displayIdx = 0;
        //putsUART("main: Prescan WFStartScan() success ................. \r\n");
    }

    // Needed to trigger g_scan_done		
    WFRetrieveScanResult(0, &bssDesc);
#else

#if defined(WF_CS_TRIS)
    WF_Connect();
#endif // defined(WF_CS_TRIS)

#endif // (MY_DEFAULT_NETWORK_TYPE == WF_SOFT_AP)

    // Initialize any application-specific modules or functions/
    // For this demo application, this only includes the
    // UART 2 TCP Bridge
#if defined(STACK_USE_UART2TCP_BRIDGE)
    UART2TCPBridgeInit();
#endif

#if defined(STACK_USE_ZEROCONF_LINK_LOCAL)
    ZeroconfLLInitialize();
#endif

#if defined(STACK_USE_ZEROCONF_MDNS_SD)
    mDNSInitialize(MY_DEFAULT_HOST_NAME);
    mDNSServiceRegister(
            (const char *) AppConfig.NetBIOSName, // base name of the service
            "_http._tcp.local", // type of the service
            80, // TCP or UDP port, at which this service is available
            ((const BYTE *) "path=/index.htm"), // TXT info
            1, // auto rename the service when if needed
            NULL, // no callback function
            NULL // no application context
            );

    mDNSMulticastFilterRegister();
#endif

#if defined(WF_CONSOLE)
    WFConsoleInit();
#endif

    // Now that all items are initialized, begin the co-operative
    // multitasking loop.  This infinite loop will continuously
    // execute all stack-related tasks, as well as your own
    // application's functions.  Custom functions should be added
    // at the end of this loop.
    // Note that this is a "co-operative mult-tasking" mechanism
    // where every task performs its tasks (whether all in one shot
    // or part of it) and returns so that other tasks can do their
    // job.
    // If a task needs very long time to do its job, it must be broken
    // down into smaller pieces so that other tasks can have CPU time.

    LEDS_OFF();

    while (1) {

        /*******************************************************************/
        // Check Button Events
        /*******************************************************************/
        if (BUTTON1_IO == 0u) {
            while (BUTTON1_IO == 0);
            LCDErase();
            sprintf((char *) LCDText, (char*) "Erase Room Info?");
            sprintf((char *) &(LCDText[16]), (char*) "SW0:Yes  SW2:No");
            LCDUpdate();

            while (1) {
                if (BUTTON1_IO == 0u) {
                    while (BUTTON1_IO == 0);
                    LCDDisplay((char *) "STATUS: Erasing...", 0, TRUE);
                    EraseRoomInfo();
                    DisplaySSID();
                    break;
                } else if (BUTTON2_IO == 0u) {
                    while (BUTTON2_IO == 0);
                    DisplaySSID();
                    break;
                }


            }
        }

        // Blink LED0 twice per sec when unconfigured, once per sec after config
        if ((TickGet() - t >= TICK_SECOND / (4ul - (CFGCXT.isWifiDoneConfigure * 2ul)))) {
            t = TickGet();
            LED0_INV();
        }

        if(CFGCXT.isWifiNeedToConfigure) updateDisplay = 1;
        

        #if (MY_DEFAULT_NETWORK_TYPE == WF_SOFT_AP)
                if (g_scan_done) {
                    if (g_prescan_waiting) {
                        SCANCXT.displayIdx = 0;
                        while (IS_SCAN_STATE_DISPLAY(SCANCXT.scanState)) {
                            WFDisplayScanMgr();
                        }


        #if defined(WF_CS_TRIS)
                        WF_Connect();
        #endif
                        DisplaySSID();
                        g_scan_done = 0;
                        g_prescan_waiting = 0;
                    }
                }
        #endif // (MY_DEFAULT_NETWORK_TYPE == WF_SOFT_AP)


        // This task performs normal stack task including checking
        // for incoming packet, type of packet and calling
        // appropriate stack entity to process it.
        StackTask();

        WiFiTask();
        // This tasks invokes each of the core stack application tasks
        StackApplications();

        #if defined(STACK_USE_ZEROCONF_LINK_LOCAL)
                ZeroconfLLProcess();
        #endif

        #if defined(STACK_USE_ZEROCONF_MDNS_SD)
                mDNSProcess();
                // Use this function to exercise service update function
                // HTTPUpdateRecord();
        #endif

        // Process application specific tasks here.
        // For this demo app, this will include the Generic TCP
        // client and servers, and the SNMP, Ping, and SNMP Trap
        // demos.  Following that, we will process any IO from
        // the inputs on the board itself.
        // Any custom modules or processing you need to do should
        // go here.

        #if defined(WF_CONSOLE)
                //WFConsoleProcess();
                //    #if !defined(STACK_USE_EZ_CONFIG)
                //	IperfAppCall();
                //      #endif
                //WFConsoleProcessEpilogue();
        wait_console_input:
        #endif

        #if defined(STACK_USE_GENERIC_TCP_CLIENT_EXAMPLE)
                GenericTCPClient();
        #endif

        #if defined(STACK_USE_GENERIC_TCP_SERVER_EXAMPLE)
                GenericTCPServer();
        #endif

        #if defined(STACK_USE_SMTP_CLIENT)
                SMTPDemo();
        #endif

        #if defined(STACK_USE_ICMP_CLIENT)
                PingDemo();
                //PingConsole();
        #endif

        #if defined(STACK_USE_SNMP_SERVER) && !defined(SNMP_TRAP_DISABLED)
                //User should use one of the following SNMP demo
                // This routine demonstrates V1 or V2 trap formats with one variable binding.
                SNMPTrapDemo();

        #if defined(SNMP_STACK_USE_V2_TRAP) || defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)
                //This routine provides V2 format notifications with multiple (3) variable bindings
                //User should modify this routine to send v2 trap format notifications with the required varbinds.
                //SNMPV2TrapDemo();
        #endif
                if (gSendTrapFlag)
                    SNMPSendTrap();
        #endif

        #if defined ( WF_CONSOLE ) && defined ( EZ_CONFIG_SCAN )
                WFDisplayScanMgr();
        #endif

        #if defined(STACK_USE_BERKELEY_API)
                BerkeleyTCPClientDemo();
                BerkeleyTCPServerDemo();
                BerkeleyUDPClientDemo();
        #endif

        if((updateDisplay && CFGCXT.isWifiDoneConfigure) || (dwLastIP != AppConfig.MyIPAddr.Val))
        {
            if(dwLastIP != AppConfig.MyIPAddr.Val)
                dwLastIP = AppConfig.MyIPAddr.Val;
            if(updateDisplay && CFGCXT.isWifiDoneConfigure)
                updateDisplay = 0;
            
            #if defined(STACK_USE_ANNOUNCE)
                AnnounceIP();
            #endif
            #if defined(STACK_USE_ZEROCONF_MDNS_SD)
                mDNSFillHostRecord();
            #endif
            DisplaySSID();


        }

    }
}

/*********************************************************************
* Function:         void SpiInitDevice(SpiChannel, int, int, int)
*
* PreCondition:     none
*
* Input:            none
*
* Output:	    none
*
* Side Effects:	    none
*
* Overview:         Initializes SPI Channels
*
*
*
* Note:
**********************************************************************/
void SpiInitDevice(SpiChannel chn, int isMaster, int frmEn, int frmMaster) {
    SpiOpenFlags oFlags = SPI_OPEN_MODE16 | SPI_OPEN_SMP_END; // SPI open mode
    if (isMaster) {
        oFlags |= SPI_OPEN_MSTEN;
    }
    if (frmEn) {
        oFlags |= SPI_OPEN_FRMEN;
        if (!frmMaster) {
            oFlags |= SPI_OPEN_FSP_IN;
        }
    }


    SpiChnOpen(chn, oFlags, 2); // divide fpb by 4, configure the I/O ports. Not using SS in this example

}




/*********************************************************************
* Function:         void DisplaySSID(void)
*
* PreCondition:     none
*
* Input:            none
*
* Output:	    none
*
* Side Effects:	    none
*
* Overview:         Updates the current SSID on line1 of the LCD and IPAddress
*                   on line2
*
*
*
* Note:
**********************************************************************/

void DisplaySSID(void)
{

    IP_ADDR IPVal;
    BYTE IPDigit[4];
    BYTE i;
  
    
    LCDErase();
    LCDUpdate();
    //Display SSID
    //REMOVE ME
    /*
    if (AppConfig.networkType == WF_ADHOC)
        sprintf((char *)MySSID, "MCHP_ADHOC_%02x%02x", AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
    else if (AppConfig.networkType == WF_SOFT_AP)
        sprintf((char *) MySSID, "MCHP_SFTAP_%02x%02x", AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
    else
        sprintf((char *) MySSID, "MCHP_%02x%02x", AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
     */
    sprintf((char *) LCDText, (char*) AppConfig.MySSID);

    //Display IP Address
    IPVal = AppConfig.MyIPAddr;

    BYTE j;
    BYTE LCDPos = 16;
    for (i = 0; i < sizeof (IP_ADDR); i++) {
        uitoa((WORD) IPVal.v[i], IPDigit);
     for (j = 0; j < strlen((char*) IPDigit); j++) {
        LCDText[LCDPos++] = IPDigit[j];
    }
    if (i == sizeof (IP_ADDR) - 1)
        break;
    LCDText[LCDPos++] = '.';
    }
    if (LCDPos < 32u)
        LCDText[LCDPos] = 0;
    LCDUpdate();
}
/*********************************************************************
* Function:         void DisplayIPValue(IP_ADDR)
*
* PreCondition:     none
*
* Input:            none
*
* Output:	    none
*
* Side Effects:	    none
*
* Overview:         Updates only the IP Address value on Line 2 of the LCD
*
*
*
* Note:
**********************************************************************/
void DisplayIPValue(IP_ADDR IPVal) {
    //	printf("%u.%u.%u.%u", IPVal.v[0], IPVal.v[1], IPVal.v[2], IPVal.v[3]);
    BYTE IPDigit[4];
    BYTE i;
#ifdef USE_LCD
    BYTE j;
    BYTE LCDPos = 16;
#endif

    for (i = 0; i < sizeof (IP_ADDR); i++) {
        uitoa((WORD) IPVal.v[i], IPDigit);

#if defined(STACK_USE_UART)
        putsUART((char *) IPDigit);
#endif

#ifdef USE_LCD
        for (j = 0; j < strlen((char*) IPDigit); j++) {
            LCDText[LCDPos++] = IPDigit[j];
        }
        if (i == sizeof (IP_ADDR) - 1)
            break;
        LCDText[LCDPos++] = '.';
#else
        if (i == sizeof (IP_ADDR) - 1)
            break;
#endif

#if defined(STACK_USE_UART)
        while (BusyUART());
        WriteUART('.');
#endif
    }

#ifdef USE_LCD
    if (LCDPos < 32u)
        LCDText[LCDPos] = 0;
    LCDUpdate();
#endif
}

/****************************************************************************
  Function:
    static void InitializeBoard(void)

  Description:
    This routine initializes the hardware.  It is a generic initialization
    routine for many of the Microchip development boards, using definitions
    in HardwareProfile.h to determine specific initialization.

  Precondition:
    None

  Parameters:
    None - None

  Returns:
    None

  Remarks:
    None
 ***************************************************************************/
static void InitializeBoard(void) {
    // WiFi Module hardware Initialization handled by Library

    // Enable multi-vectored interrupts
    INTEnableSystemMultiVectoredInt();

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1); // Use 1:1 CPU Core:Peripheral clocks

    // Disable JTAG port so we get our I/O pins back, but first
    // wait 50ms so if you want to reprogram the part with
    // JTAG, you'll still have a tiny window before JTAG goes away.
    // The PIC32 Starter Kit debuggers use JTAG and therefore must not
    // disable JTAG.
    DelayMs(50);
    DDPCONbits.JTAGEN = 0;

    // LEDs
    LEDS_OFF();
    mPORTESetPinsDigitalOut(BIT_5 | BIT_6 | BIT_7);

    // Switches
    mPORTDSetPinsDigitalIn(BIT_4 | BIT_5 | BIT_6);
    ConfigCNPullups(CN13_PULLUP_ENABLE | CN14_PULLUP_ENABLE | CN15_PULLUP_ENABLE);

    // LCD
    mPORTESetPinsDigitalOut(BIT_0 | BIT_1 | BIT_2 | BIT_3);
    //Configure LCD SPI pins
    mPORTFSetPinsDigitalOut(BIT_8);
    mPORTDSetPinsDigitalOut(BIT_15);

    //SPI Flash
    mPORTDSetPinsDigitalOut(BIT_14);


    //UART
    mPORTFSetPinsDigitalOut(BIT_5);
    mPORTFSetPinsDigitalIn(BIT_4);

    //MiWi
#if defined(MRF24J40) || defined(MRF49XA)
    PHY_CS = 1;
    mPORTDSetPinsDigitalOut(BIT_9);

    PHY_RESETn = 1;
    mPORTDSetPinsDigitalOut(BIT_11);
#endif

#if defined(MRF49XA)
    nFSEL_TRIS = 0;
    FINT_TRIS = 1;

    nFSEL = 1;
#elif defined(MRF24J40)
    PHY_WAKE = 1;
    mPORTBSetPinsDigitalOut(BIT_9);
#else
    Data_nCS_TRIS = 0;
    Config_nCS_TRIS = 0;
    Data_nCS = 1;
    Config_nCS = 1;
    IRQ1_INT_TRIS = 1;
    IRQ0_INT_TRIS = 1;

#endif

    /* Set the Port Directions of SDO, SDI, Clock & Slave Select Signal */
    /* Set SCK port pin to output */
    mPORTDSetPinsDigitalOut(BIT_10);
    /* Set SDO port pin to output */
    mPORTDSetPinsDigitalOut(BIT_0);
    /* Set SDI port pin to input */
    mPORTCSetPinsDigitalIn(BIT_4);
    /* Set INT1, INT2 port pins to input */
    mPORTESetPinsDigitalIn(BIT_8 | BIT_9);

    /* Clear SPI1CON register */
    SPI1CONCLR = 0xFFFFFFFF;

#ifdef HARDWARE_SPI
    unsigned int pbFreq;

    /* Enable SPI1, Set to Master Mode & Set CKE bit : Serial output data changes on transition
      from active clock state to Idle clock state */
    SPI1CON = 0x00008120;
    /* Peripheral Bus Frequency = System Clock / PB Divider */
    pbFreq = (DWORD) CLOCK_FREQ / (1 << mOSCGetPBDIV());

    /* PB Frequency can be maximum 40 MHz */
    if (pbFreq > (2 * MAX_SPI_CLK_FREQ_FOR_P2P)) {
        {
            unsigned int SPI_Clk_Freq;

            unsigned char SPI_Brg1 = 1;

            //For the SPI1
            /* Continue the loop till you find SPI Baud Rate Register Value */
            while (1) {
                /* SPI Clock Calculation as per PIC32 Manual */
                SPI_Clk_Freq = pbFreq / (2 * (SPI_Brg1 + 1));

                if (SPI_Clk_Freq <= MAX_SPI_CLK_FREQ_FOR_P2P) {
                    break;
                }

                SPI_Brg1++;
            }



            mSpiChnSetBrg(1, SPI_Brg1);

        }
    } else {
        /* Set SPI1 Baud Rate */
        mSpiChnSetBrg(1, 0);

    }

#endif

    /* Set the Interrupt Priority */
    mINT2SetIntPriority(4);

#if defined(MRF89XA)
    mINT1SetIntPriority(4);
#endif

    /* Set Interrupt Subpriority Bits for INT2 */
    mINT2SetIntSubPriority(2);

#if defined(MRF89XA)
    mINT2SetIntSubPriority(1);
#endif

    /* Set INT2 to falling edge */
    mINT2SetEdgeMode(0);

#if defined(MRF89XA)
    mINT1SetEdgeMode(1);
    mINT2SetEdgeMode(1);
#endif

    /* Enable INT2 */
    mINT2IntEnable(1);

#if defined(MRF89XA)
    mINT2IntEnable(1);
#endif

    /* Enable Multi Vectored Interrupts */
    //    INTEnableSystemMultiVectoredInt();

#if defined(MRF89XA)
    PHY_IRQ1 = 0;
    PHY_IRQ0 = 0;
    PHY_RESETn_TRIS = 1;
#else
    RFIF = 0;
    if (RF_INT_PIN == 0) {
        RFIF = 1;
    }
#endif

    // Initialize the EEPROM
    XEEInit();

    // UART Initialization
#if defined(STACK_USE_UART)
    UARTTX_TRIS = 0;
    UARTRX_TRIS = 1;
    UMODE = 0x8000; // Set UARTEN.  Note: this must be done before setting UTXEN
    USTA = 0x00001400; // RXEN set, TXEN set
#define CLOSEST_UBRG_VALUE ((GetPeripheralClock()+8ul*BAUD_RATE)/16/BAUD_RATE-1)
#define BAUD_ACTUAL (GetPeripheralClock()/16/(CLOSEST_UBRG_VALUE+1))

#define BAUD_ERROR ((BAUD_ACTUAL > BAUD_RATE) ? BAUD_ACTUAL-BAUD_RATE : BAUD_RATE-BAUD_ACTUAL)
#define BAUD_ERROR_PRECENT	((BAUD_ERROR*100+BAUD_RATE/2)/BAUD_RATE)
#if (BAUD_ERROR_PRECENT > 3)
#warning UART frequency error is worse than 3%
#elif (BAUD_ERROR_PRECENT > 2)
#warning UART frequency error is worse than 2%
#endif

    UBRG = CLOSEST_UBRG_VALUE;
#endif


}



#if 1 // debug soft AP
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1, MY_DEFAULT_MAC_BYTE2, MY_DEFAULT_MAC_BYTE3, MY_DEFAULT_MAC_BYTE4, MY_DEFAULT_MAC_BYTE5, MY_DEFAULT_MAC_BYTE6};

/*********************************************************************
 * Function:        void InitAppConfig(void)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           None
 *
 * Output:          Write/Read non-volatile config variables.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
static void InitAppConfig(void) {
#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
    //unsigned char vNeedToSaveDefaults = 0;
#endif

    while (1) {
        // Start out zeroing all AppConfig bytes to ensure all fields are
        // deterministic for checksum generation
        memset((void*) &AppConfig, 0x00, sizeof (AppConfig));

        AppConfig.Flags.bIsDHCPEnabled = TRUE;
        AppConfig.Flags.bInConfigMode = TRUE;
        memcpypgm2ram((void*) &AppConfig.MyMACAddr, (ROM void*) SerializedMACAddress, sizeof (AppConfig.MyMACAddr));
        //                            {
        //                                            _prog_addressT MACAddressAddress;
        //                                            MACAddressAddress.next = 0x157F8;
        //                                            _memcpy_p2d24((char*)&AppConfig.MyMACAddr, MACAddressAddress, sizeof(AppConfig.MyMACAddr));
        //                            }


        // SoftAP on certain setups with IP 192.168.1.1 has problem with DHCP client assigning new IP address on redirection.
        // 192.168.1.1 is a common IP address with most APs. This is still under investigation.
        // For now, assign this as 192.168.1.3
        //#if (MY_DEFAULT_NETWORK_TYPE == WF_SOFT_AP)
            AppConfig.MyIPAddr.Val = 192ul | 168ul<<8ul | 1ul<<16ul | 3ul<<24ul;
            AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
            AppConfig.MyMask.Val = 255ul | 255ul<<8ul | 0ul<<16ul | 0ul<<24ul;
            AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
            AppConfig.MyGateway.Val = AppConfig.MyIPAddr.Val;
            AppConfig.PrimaryDNSServer.Val = AppConfig.MyIPAddr.Val;
            AppConfig.SecondaryDNSServer.Val = AppConfig.MyIPAddr.Val;

        //#else
         /*
        AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2 << 8ul | MY_DEFAULT_IP_ADDR_BYTE3 << 16ul | MY_DEFAULT_IP_ADDR_BYTE4 << 24ul;
        AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
        AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2 << 8ul | MY_DEFAULT_MASK_BYTE3 << 16ul | MY_DEFAULT_MASK_BYTE4 << 24ul;
        AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
        AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2 << 8ul | MY_DEFAULT_GATE_BYTE3 << 16ul | MY_DEFAULT_GATE_BYTE4 << 24ul;
        AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 | MY_DEFAULT_PRIMARY_DNS_BYTE2 << 8ul | MY_DEFAULT_PRIMARY_DNS_BYTE3 << 16ul | MY_DEFAULT_PRIMARY_DNS_BYTE4 << 24ul;
        AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 | MY_DEFAULT_SECONDARY_DNS_BYTE2 << 8ul | MY_DEFAULT_SECONDARY_DNS_BYTE3 << 16ul | MY_DEFAULT_SECONDARY_DNS_BYTE4 << 24ul;
         * */
        //#endif
        // Load the default NetBIOS Host Name
        memcpypgm2ram(AppConfig.NetBIOSName, (ROM void*) MY_DEFAULT_HOST_NAME, 16);
        FormatNetBIOSName(AppConfig.NetBIOSName);

#if defined(WF_CS_TRIS)
        // Load the default SSID Name
        WF_ASSERT(sizeof (MY_DEFAULT_SSID_NAME) <= sizeof (AppConfig.MySSID));
        memcpypgm2ram(AppConfig.MySSID, (ROM void*) MY_DEFAULT_SSID_NAME, sizeof (MY_DEFAULT_SSID_NAME));
        AppConfig.SsidLength = sizeof (MY_DEFAULT_SSID_NAME) - 1;
        AppConfig.SecurityMode = MY_DEFAULT_WIFI_SECURITY_MODE;
        if (AppConfig.SecurityMode == WF_SECURITY_WEP_40) {
            AppConfig.WepKeyIndex = MY_DEFAULT_WEP_KEY_INDEX;
            memcpypgm2ram(AppConfig.SecurityKey, (ROM void*) MY_DEFAULT_WEP_KEYS_40, sizeof (MY_DEFAULT_WEP_KEYS_40) - 1);
            AppConfig.SecurityKeyLength = sizeof (MY_DEFAULT_WEP_KEYS_40) - 1;
        } else if (AppConfig.SecurityMode == WF_SECURITY_WEP_104) {
            AppConfig.WepKeyIndex = MY_DEFAULT_WEP_KEY_INDEX;
            memcpypgm2ram(AppConfig.SecurityKey, (ROM void*) MY_DEFAULT_WEP_KEYS_104, sizeof (MY_DEFAULT_WEP_KEYS_104) - 1);
            AppConfig.SecurityKeyLength = sizeof (MY_DEFAULT_WEP_KEYS_104) - 1;
        }
        AppConfig.networkType = MY_DEFAULT_NETWORK_TYPE;
        AppConfig.dataValid = 0;
#endif

        // Compute the checksum of the AppConfig defaults as loaded from ROM
        wOriginalAppConfigChecksum = CalcIPChecksum((BYTE*) & AppConfig, sizeof (AppConfig));
        /*
        #if defined(EEPROM_CS_TRIS)
                    NVM_VALIDATION_STRUCT NVMValidationStruct;

                    // Check to see if we have a flag set indicating that we need to
                    // save the ROM default AppConfig values.
                    if(vNeedToSaveDefaults)
                        SaveAppConfig(&AppConfig);

                    // Read the NVMValidation record and AppConfig struct out of EEPROM/Flash
                        XEEReadArray(0x0000, (BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
                        XEEReadArray(sizeof(NVMValidationStruct), (BYTE*)&AppConfig, sizeof(AppConfig));

                    // Check EEPROM/Flash validitity.  If it isn't valid, set a flag so
                    // that we will save the ROM default values on the next loop
                    // iteration.
                    if((NVMValidationStruct.wConfigurationLength != sizeof(AppConfig)) ||
                       (NVMValidationStruct.wOriginalChecksum != wOriginalAppConfigChecksum) ||
                       (NVMValidationStruct.wCurrentChecksum != CalcIPChecksum((BYTE*)&AppConfig, sizeof(AppConfig))))
                    {
                        // Check to ensure that the vNeedToSaveDefaults flag is zero,
                        // indicating that this is the first iteration through the do
                        // loop.  If we have already saved the defaults once and the
                        // EEPROM/Flash still doesn't pass the validity check, then it
                        // means we aren't successfully reading or writing to the
                        // EEPROM/Flash.  This means you have a hardware error and/or
                        // SPI configuration error.
                        if(vNeedToSaveDefaults)
                        {
                            while(1);
                        }

                        // Set flag and restart loop to load ROM defaults and save them
                        vNeedToSaveDefaults = 1;
                        continue;
                    }

                    // If we get down here, it means the EEPROM/Flash has valid contents
                    // and either matches the ROM defaults or previously matched and
                    // was run-time reconfigured by the user.  In this case, we shall
                    // use the contents loaded from EEPROM/Flash.
                    break;
        #endif
         */
        break;

    }


#if defined (EZ_CONFIG_STORE)
    // Set configuration for ZG from NVM
    /* Set security type and key if necessary, convert from app storage to ZG driver */

    if (AppConfig.dataValid)
        CFGCXT.isWifiDoneConfigure = 1;

    AppConfig.saveSecurityInfo = FALSE;
#endif // EZ_CONFIG_STORE

}
#endif





void ConfigureLCD_SPI() {
    LCD_SPI_BRG = (GetPeripheralClock() - 1ul) / 2ul / LCD_MAX_SPI_FREQ;
    LCD_SPICON1 = 0x00000260; // sample at end, data change idle to active, clock idle high, master
    LCD_SPICON1bits.ON = 1;
}

void ConfigureFlash_SPI() {
    SPIFLASH_SPIBRG = (GetPeripheralClock() - 1ul) / 2ul / FLASH_MAX_SPI_FREQ;
    SPIFLASH_SPICON1 = 0x00008120; // sample in the middle, data change idle to active,  master
    SPIFLASH_SPICON1bits.ON = 1;
}



#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)

void SaveAppConfig(const APP_CONFIG *ptrAppConfig) {
    NVM_VALIDATION_STRUCT NVMValidationStruct;

    // Ensure adequate space has been reserved in non-volatile storage to
    // store the entire AppConfig structure.  If you get stuck in this while(1)
    // trap, it means you have a design time misconfiguration in TCPIPConfig.h.
    // You must increase MPFS_RESERVE_BLOCK to allocate more space.
#if defined(STACK_USE_MPFS2)
    if (sizeof (NVMValidationStruct) + sizeof (AppConfig) > MPFS_RESERVE_BLOCK)
        while (1);
#endif

    // Get proper values for the validation structure indicating that we can use
    // these EEPROM/Flash contents on future boot ups
    NVMValidationStruct.wOriginalChecksum = wOriginalAppConfigChecksum;
    NVMValidationStruct.wCurrentChecksum = CalcIPChecksum((BYTE*) ptrAppConfig, sizeof (APP_CONFIG));
    NVMValidationStruct.wConfigurationLength = sizeof (APP_CONFIG);

    // Write the validation struct and current AppConfig contents to EEPROM/Flash
#if defined(EEPROM_CS_TRIS)
    XEEBeginWrite(0x0000);
    XEEWriteArray((BYTE*) & NVMValidationStruct, sizeof (NVMValidationStruct));
    XEEWriteArray((BYTE*) ptrAppConfig, sizeof (APP_CONFIG));
#else
    SPIFlashBeginWrite(0x0000);
    SPIFlashWriteArray((BYTE*) & NVMValidationStruct, sizeof (NVMValidationStruct));
    SPIFlashWriteArray((BYTE*) ptrAppConfig, sizeof (APP_CONFIG));
#endif
}
#endif


#if defined (EZ_CONFIG_STORE)

void RestoreWifiConfig(void) {
    putrsUART((ROM char*) "\r\nButton push, restore wifi configuration!!!\r\n");

#if defined(EEPROM_CS_TRIS)
    XEEBeginWrite(0x0000);
    XEEWrite(0xFF);
    XEEWrite(0xFF);
    XEEEndWrite();
#elif defined(SPIFLASH_CS_TRIS)
    SPIFlashBeginWrite(0x0000);
    SPIFlashWrite(0xFF);
    SPIFlashWrite(0xFF);
#endif

    // reboot here...
    LED_PUT(0x00);
    while (BUTTON3_IO == 0u);
    Reset();
}


#endif // EZ_CONFIG_STORE