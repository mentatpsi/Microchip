/****************************************************************************
* FileName:         Demo.c
* Dependencies:     none   
* Processor:        PIC18F46J50	
* Complier:         Microchip C18 v3.04 or higher
* Company:          Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice for MiWi DE Software:
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute 
* Software only when embedded on a Microchip microcontroller or digital 
* signal controller and used with a Microchip radio frequency transceiver, 
* which are integrated into your product or third party product (pursuant 
* to the terms in the accompanying license agreement).   
*
* You should refer to the license agreement accompanying this Software for 
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
****************************************************************************
* File Description:

*   This is the simple demo for the 8-bit Wireless Development Kit 
*   that comes pre-programmed with the Kit.This application 
*   demonstrates how to create a two node MiWi network. After
*   creating the network the nodes in the network exchange the
*   temperature sensor readings.
*   In this demo, following features are demonstrated
*   - Creating a MiWi PANCoordinator
*       This example demonstrates how to create a PANCoordinator
*       (with and without recovering the status from NVM)
*   - Establishing connection with PANCoordinator
*       This example demonstrates the scan process and allows
*       the user to choose a network to join
*   - Using MAC Address EEPROM for unique addresses
*       This demo uses MAC Address EEPROM available on the 
*       RF module for unique long addresses
*   - Data Exchange 
*       Temperature sensor readings are exchanged using the 
*       broadcast messages.
*   - Expandable over multiple kits
*       To create a large network, the users can add more 8-bit
*       Wireless Development Kits and expand the 2-node network to
*       any size.
* Change History:
*  Rev   Date         Author    Description
*  0.1	09/29/2010    mynenis	Initial Version
*  1.0  03/23/2010    mynenis   Updated to work with latest version 
*                               of MiWi Stack (4.1 Beta)
*  1.1  23/11/2011    mynenis   Bug Fix in LcdDisplay.c
**************************************************************************/

/************************ HEADERS ****************************************/
#include "WirelessProtocols/Console.h"
#include "ConfigApp.h"
#include "HardwareProfile.h"
#include "TimeDelay.h"
#include "LcdDisplay.h"
#include "WirelessProtocols/MSPI.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/SymbolTime.h"
#include "WirelessProtocols/NVM.h"
#include "Transceivers/Transceivers.h"
#include "SelfTestMode.h"

/************************** VARIABLES ************************************/
#define BANDGAP_VOLTAGE (1200)
#define samplingRate    5
/*************************************************************************/

// The variable myChannel defines the channel that the device
// operates on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).

/*************************************************************************/
BYTE    myChannel = 11;


/***********************************************************************/
//  Variables used for recording temperature sensor values
/***********************************************************************/
BYTE    tempLocal = 0xFF;               //Temperature Reading at this node in Celsius - Default value is set to 0xFF (Not Available)
BYTE    tempRemote = 0xFF;              //Temperature Reading at remote node in Celsius
BYTE    tempLocalF = 0xFF;              //Local temperature Reading in Farenheit
BYTE    tempRemoteF = 0xFF;             //Remote temperature reading in Farenheit

BYTE    tempFormat = 0x00;              //TempFormat "0" - Celsius
                                        //           Otherwise - Farenheit
WORD    VBGResult;                      //Bandgap Voltage    
            
//Flags
BOOL    NVMEnable = FALSE;              //NVM Enabled or not
BOOL    readSensor = FALSE;             
BOOL    connectionEntry = FALSE;
BOOL    interactivemode = FALSE;

BYTE    connectionAddress[3];           // Directly connected to ?? (Remote Node address )

struct TempValues
{
    BYTE    Address[3];
    BYTE    TempValue;
};

struct TempValues   NodeTemp[10];
BYTE                NodeIndex = 1;
BYTE                CurrentNodeIndex = 0;

// Variables and #defines for Storing Node's Temperature sensor values
BYTE                storeTemp[100];
BYTE                storeTemp_currentindex = 0;
BYTE                storeTemp_index = 0;
#define MAXLIMIT_STORETEMP  100

//Variables for Extended OOB - Display Temp sensor readings for every minute
BOOL            En_1mindisplay = FALSE;

extern BYTE     myLongAddress[];
MIWI_TICK       tick1, tick2;
extern BYTE     role;
extern WORD_VAL myPANID;


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
BYTE    AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x00, 0x00, 0x00};
#endif

/*********************************************************************
* Function:         void main(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    This is the main function that runs the 8-bit 
*                   WDK demo. The application will first try to establish
*                   connection with another device and then process the 
*                   received information as well as transmit its own 
*                   temperature information.
*
* Note:			    
**********************************************************************/
void main (void)
{
    BYTE    i, j;
   
    NVMEnable = FALSE;
    readSensor = TRUE;
    
    BoardInit();                                //Initializes PIC18 Wireless Development board and LCD Serial Accessory board.
    
    /****************************************************************/
    //SelfTest() mode can be used to verify the 8-bit Wireless 
    //Development Kit hardware. This routine can be used to pinpoint
    //the problem
    
    //It verifies the following components -
    //  -Push Buttons
    //  -LEDs
    //  -Temperature Sensor
    //  -PICtail SPI connections
    //  -EEPROM 
    //  -LCD Serial Accessory board and RS232 Serial Accessory board
    /****************************************************************/
    SelfTest();
    
    /*****************************************************************/
    //NetworkSetup() allows the user to setup the network based on 
    //sequence of push button press.
    
    //This allows
    //  Create a network (Start the node as PANCoordinator)
    //  Join an existing network (Establish connection with a coordinator
    //                            or PANCoordinator)
    //  Recover network status from NVM
    /*****************************************************************/
    {
    BOOL NvmStatus;  
         
    NvmStatus = NetworkSetup();
    if(NvmStatus == FALSE)
        return;                 //Restart the application, User has chosen not to go with
                                //Network Freezer settings
    }
    
    DumpConnection(0xFF);           //Prints out content of connection entry on the hyperterminal
    
    
    
    //Measure the VDD to use in conversion
    Read_VBGVoltage();              


    tick1 = MiWi_TickGet();
    TRISAbits.TRISA0 = 0;           //Configure the Temp sensor port

   
    while(1)
    {
        tick2 = MiWi_TickGet();
        /*******************************************************************/
        //For every 30 seconds read the temperature sensor value and 
        //broadcast the information over the network.
        /*******************************************************************/
        if(
            (MiWi_TickGetDiff(tick2, tick1) > (ONE_SECOND * 30)) | 
            (readSensor) |
            (PUSH_BUTTON_RB0 == 0)
          )
          
          {
              ReadTempSensor();
              TransmitMessage();
          }
              
        

        /*******************************************************************/

        // Function MiApp_MessageAvailable returns a boolean to indicate if
        // a packet has been received by the transceiver. If a packet has
        // been received, all information will be stored in the rxFrame,
        // structure of RECEIVED_MESSAGE.

        /*******************************************************************/
        else if(MiApp_MessageAvailable())
        {

             ProcessRxMessage();
            
            /*******************************************************************/

            // Function MiApp_DiscardMessage is used to release the current
            // received packet.
            // After calling this function, the stack can start to process the
            // next received frame

            /*******************************************************************/
             MiApp_DiscardMessage();
            
        }
        else if(PUSH_BUTTON_RB2 == 0)
        {

            /*******************************************************************/
            // Button 2 pressed. 
            // Print the next node information on the LCD
            // MACAddress and latest temperature sensor reading for that node
            /*******************************************************************/
            CurrentNodeIndex++;
            if(CurrentNodeIndex >= NodeIndex)
            {
                CurrentNodeIndex = 0;
            }

            PrintTempLCD();
                 
        }
        
        /********************************************************/
        //Check continuoulsy if the user wants to go the interactive
        //mode for more information about the network and the node
        
        //In the interactive mode, the user can read more 
        //comprehensive list of readings for the temperature sensor
        /********************************************************/
          else  if(ConsoleIsGetReady())
          {
              SwitchToInteractiveMode();
            
            
          }          //end of (if(ConsoleIsGetReady())
            
        }           //end of While(1) loop
        
    }               //end of main
    

/*********************************************************************
* Function:         void ReadMacAddress()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Reads MAC Address from MAC Address EEPROM
*
* Side Effects:	    none
*
* Overview:		    Uses the MAC Address from the EEPROM for addressing
*
* Note:			    
**********************************************************************/
void ReadMacAddress(void)
{
    RF_EEnCS_TRIS = 0;
    {
        BYTE    Address0, Address1, Address2;
        RF_EEnCS = 0;
        Nop();
        Nop();
        SPIPut(0x03);   //Read Sequence to EEPROM
        SPIPut(0xFA);   //MAC address Start byte
        Address0 = SPIGet();
        Address1 = SPIGet();
        Address2 = SPIGet();
        RF_EEnCS = 1;
        
        if((Address0 == 0x00) && (Address1 == 0x04) && (Address2 == 0xA3))  //Compare the value against Microchip's OUI
        {
            RF_EEnCS = 0;
            SPIPut(0x03);
            SPIPut(0xFD);
            switch(MY_ADDRESS_LENGTH)
                {
                    case 8: myLongAddress[7] = 0x00;
                    case 7: myLongAddress[6] = 0x04;
                    case 6: myLongAddress[5] = 0xA3;
                    case 5: myLongAddress[4] = 0xFF;
                    case 4: myLongAddress[3] = 0xFE;
                    case 3: myLongAddress[2] = SPIGet();
                    case 2: myLongAddress[1] = SPIGet();
                    case 1: myLongAddress[0] = SPIGet();
                            break;
                    default: break;
                }
                     
            RF_EEnCS = 1;
            
        }
    }
        
    #if ADDITIONAL_NODE_ID_SIZE > 0
        AdditionalNodeID[0] = myLongAddress[2];
        AdditionalNodeID[1] = myLongAddress[1];
        AdditionalNodeID[2] = myLongAddress[0];
    #endif
}
 

/*********************************************************************
* Function:         void Read_VBGVoltage(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Reads the band gap voltage and compares with reference voltage
*					to arrive at the current voltage level
*
* Side Effects:	    none
*
* Overview:		    Uses the MAC Address from the EEPROM for addressing
*
* Note:			    
**********************************************************************/
void Read_VBGVoltage(void)
{
    ADCON0 = 0x3D;      //Configures the channel as VBG
    ADCON1 = 0xB1;      //Program the acquisition time
    ANCON0 = 0xFF;      //Reset all the AN ports as digital
    ANCON1 = 0x9F;      //Enable Band gap reference voltage
    Delay10us(1);       //Wait for the acquisition time
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 0;  //Disable ADC interrupts

    //This routine uses the polling based mechanism
    ADCON0bits.GO = 1;

    //Start A/D conversion
    while(ADCON0bits.DONE);
    {
        float   voltage;

        VBGResult = ADRES;
    }

    ADCON0 = 0x3C;      //ADC OFF
    ANCON1 = 0x1F;      //Disable Bandgap
    return;
}

/*********************************************************************
* Function:         void PrintTempLCD(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Updates the LCD Display with current node index values
*
* Side Effects:	    none
*
* Overview:
*
* Note:			    
**********************************************************************/
void PrintTempLCD(void)
{
    LCDErase();
    if(CurrentNodeIndex == 0)
    {
        sprintf((char *)LCDText, (far rom char *) "Local Temp:");
        if(!interactivemode)
        {
            Printf("\rLocal Temp (");
        }
    }
    else
    {
        sprintf((char *)LCDText, (far rom char *) "Remote Temp:");
        if(!interactivemode)
        {
            Printf("\rRemote Temp (");
        }
    }

    sprintf
    (
        (char *) &LCDText[16],
        (far rom char *) "%02x%02x%02x",
        NodeTemp[CurrentNodeIndex].Address[0],
        NodeTemp[CurrentNodeIndex].Address[1],
        NodeTemp[CurrentNodeIndex].Address[2]
    );
    if(!interactivemode)
    {
        PrintChar(NodeTemp[CurrentNodeIndex].Address[0]);
        PrintChar(NodeTemp[CurrentNodeIndex].Address[1]);
        PrintChar(NodeTemp[CurrentNodeIndex].Address[2]);
        Printf("):");
    }

    sprintf((char *) &LCDText[22], (far rom char *) ":");
    if(NodeTemp[CurrentNodeIndex].TempValue == 255)
    {
        sprintf((char *) &LCDText[23], (far rom char *) " - ");
        if(!interactivemode)
        {
            Printf(" - ");
        }
    }
    else
    {
        sprintf((char *) &LCDText[23], (far rom char *) " %d", NodeTemp[CurrentNodeIndex].TempValue);
        sprintf((char *) &LCDText[26], (far rom char *) "C");
        {
            float   temp;
            BYTE    tempF;
            temp = NodeTemp[CurrentNodeIndex].TempValue;
            temp = ((temp * 1.8) + 32);
            tempF = (BYTE) temp;
            sprintf((char *) &LCDText[27], (far rom char *) "/ %d", tempF);
            sprintf((char *) &LCDText[31], (far rom char *) "F");
            if(!interactivemode)
            {
                PrintDec(NodeTemp[CurrentNodeIndex].TempValue);
                Printf("°C/");
                PrintDec(tempF);
                Printf("°F");
            }
        }
    }

    LCDUpdate();
}

/*********************************************************************
* Function:         void ConfigADC(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    This functions programs the ADC registers for capture
*
* Note:			    
**********************************************************************/
void ConfigADC(void)
{

    //Enable AN1 as input
    TRISAbits.TRISA1 = 1;

    //Configure the ADC register settings
    ANCON0 = 0xFC;
    ANCON1 = 0x1F;
    ADCON0 = 0x05;
    ADCON1 = 0xB9;
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 0;
}

/*********************************************************************
* Function:         BOOL NetworkSetup(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    In this fucntion based push button input from the 
*                   user, either a PAN Coordinator or a coordinator is
*                   created
* Note:			    
**********************************************************************/
BOOL NetworkSetup()
{
    /*******************************************************************/
    //If RB2 is not pressed during the startup of the program; check if RB0
    //push button is pressed
    //If RB0 push button is pressed, recover the node status from NVM
    //after prompting the user 
    /*******************************************************************/
    if(PUSH_BUTTON_RB0 == 0)
    {
        while(PUSH_BUTTON_RB0 == 0);

        //Enable Network Freezer Mode
        LCDDisplay((char *)"Network Freezer?Yes[RB0]/No[RB2]", 0, TRUE);
        Printf("\r\nEnable Network Freezer?\n\rPress RB0 - Yes, RB2-No\r\n");
        while(1)
        {
            /*************************************************************
            //If the user prompts for bootup from NVM then setup the node
            //based on the status from NVM
            /*************************************************************/
            if(PUSH_BUTTON_RB0 == 0)
            {
                MiApp_ProtocolInit(TRUE);
                NVMEnable = TRUE;

                LCDDisplay((char *)"Network Freezer Mode Enabled", 0, TRUE);
                DelayMs(2000);
                Printf("\n\rNetwork Freezer mode is enabled. There will be no handshake process\r\n");
                break;
            }
            /**************************************************************
            //If the user does not want to bootup from NVM; then continue 
            //with creating/joining a network (normal operation)
            //All the previously stored NVM status will be lost and will be
            //updated based on the new network setup parameters.
            /**************************************************************/
            if(PUSH_BUTTON_RB2 == 0)
            {
                NVMEnable = FALSE;
                LCDDisplay((char *)"Network Freezer Mode Disabled", 0, TRUE);
                DelayMs(2000);
                return FALSE;
            }
        }
    }
    else
    {
        /*******************************************************************/
        //LCDBacklightON() - Switches on the LCD Backlight
        //LCDBacklightOFF() - Switches off LCD Backlight
        //Can use Automatic Backlight mode for the LCD instead, which switches
        //on the backlight for every LCD display update
        //To conserve battery power, using LCDBacklight selectively 
        //is recommended
        /*******************************************************************/
        LCDBacklightON();
        LCDDisplay((char *)"8-Bit Wireless  Development Kit ", 0, TRUE);
        DelayMs(5000);
        LCDBacklightOFF();

        /*******************************************************************/

        //If Hyperterminal/Console program is enabled
        //Display the demo instructions on the Console

        /*******************************************************************/
        
      
        Printf("\r\n--------------------------------------------------------------------------\r\n");
        Printf("                     8-bit Wireless Development Kit - Demo               \r\n");
        Printf("--------------------------------------------------------------------------\r\n");
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RB0");
        Printf("\r\n           Button 2: RB2");
        #if defined(MRF24J40)
            Printf("\r\n     RF Transceiver: MRF24J40");
        #elif defined(MRF49XA)
            Printf("\r\n     RF Transceiver: MRF49XA");
        #elif defined(MRF89XA)
            Printf("\r\n     RF Transceiver: MRF89XA");
        #endif
        Printf("\r\n   Demo Instruction:");
        Printf("\r\n                     Push Button 1 to broadcast temperature.  ");
        Printf("\r\n                     Push Button 2 to switch between Local and Remote Temps");
        Printf("\r\n                     Press 'ESC' Key to switch to Interactive Mode");
        Printf("\r\n    Other Features:");
        Printf("\r\n                     Use MCLR + Button 2 to switch to Self Test Mode");
        Printf("\r\n                     Use MCLR + Button 1 to switch to test Network Freezer Mode");
        Printf("\r\n\r\n");
        

        LED_1 = 0;
        LED_2 = 0;

        /*******************************************************************/

        // Initialize Microchip proprietary protocol. Which protocol to use
        // depends on the configuration in ConfigApp.h

        /*******************************************************************/

        /*********************************************************************/
        //Read the mac address from the MAC Address EEPROM and use it for long
        //Address
        /********************************************************************/   
        ReadMacAddress();
CreateorJoin:
        /*******************************************************************/
        // Function MiApp_ProtocolInit initialize the protocol stack. The
        // only input parameter "FALSE" indicates to create network 
        // configuration from scratch
        /*******************************************************************/
        MiApp_ProtocolInit(FALSE);

        //Set the channel to myChannel defined in the application
        if(MiApp_SetChannel(myChannel) == FALSE)
        {
            Printf("\r\nERROR: Unable to program the channel\r\n");
            Printf("\r\nPress MCLR to start again\r\n");
            LCDDisplay((char *)"Error: Unable to Program Channel ", 0, TRUE);
            while(1);
            //Display error message on LCD and Console
        }

        /*******************************************************************/

        // Function MiApp_ConnectionMode defines the connection mode. The
        // possible connection modes are:
        //  ENABLE_ALL_CONN:    Enable all kinds of connection
        //  ENABLE_PREV_CONN:   Only allow connection already exists in
        //                      connection table
        //  ENABL_ACTIVE_SCAN_RSP:  Allow response to Active scan
        //  DISABLE_ALL_CONN:   Disable all connections.

        /*******************************************************************/
        MiApp_ConnectionMode(ENABLE_ALL_CONN);

        /*******************************************************************/

        //Prompt the user to press RB0 or RB2
        
        //If RB0 is pressed then setup the node as PANCoordinator
        //If PANID is set as 0xFFFF in ConfigApp.h, then a random value for
        //PANID will be used to create the network
        
        //If RB2 is pressed scan the channel for networks and display the 
        //available networks on the LCD display
        /*******************************************************************/
        LCDDisplay((char *)"RB0: Create NWK RB2: Join NWK    ", 0, FALSE);
        {
            BOOL    result = TRUE;
            while(result == TRUE)
            {
                if(PUSH_BUTTON_RB0 == 0)
                {

                    //RB0 is pressed
                    //Set the node as PANCoordinator	
                    while(PUSH_BUTTON_RB0 == 0);
                    Nop();
                    MiApp_ProtocolInit(FALSE);
                    //Starts a new connection on the channel given by the bitmap
                    MiApp_StartConnection(START_CONN_DIRECT, 0, 0);    
                    /********************************************************/
                    //Create a NodeTemp[] for storing address and corresponding
                    //Temperature sensor readings
                    //Use index 0 for "Own" temperature sensor readings
                    /********************************************************/
                    NodeTemp[0].Address[0] = myLongAddress[2];
                    NodeTemp[0].Address[1] = myLongAddress[1];
                    NodeTemp[0].Address[2] = myLongAddress[0];
                    
                    //Print Status on LCD Display and the Console
                    
                    Printf("\r\nCreated Network Successfully\r\n");
                    Printf("Network Details:\r\n");
                    LCDDisplay((char *)"Created Network Successfully", 0, FALSE);
                    DelayMs(3000);
                    result = FALSE;
                }

                if(PUSH_BUTTON_RB2 == 0)
                {

                    //RB2 is pressed
                    //Scan for the Networks and Join a network
                    BYTE   scanresult;
                    while(PUSH_BUTTON_RB2 == 0);
                    MiApp_ProtocolInit(FALSE);
                    /****************************************************************/
                    //Discard application layer messages that are in the queue.
                    //If you don't discard the previous messages, stack will not process 
                    //the new packets received; scanresult will be empty
                    /****************************************************************/
                    if(MiMAC_ReceivedPacket())
                    {
                        MiApp_DiscardMessage();
                    }

                    scanresult = MiApp_SearchConnection(10, 0x00000800);    
                    
                    //Print status on Console
                    if(scanresult > 1)
                    {
                        Printf("Multiple Networks present \r\n");
                    }

                    if(scanresult > 0)
                    {
                        BYTE scanIndex = 0;

                        // Print Active Scan results on Console
                        Printf("\r\nActive Scan Results: \r\n");
                        for(scanIndex = 0; scanIndex < scanresult; scanIndex++)
                        {
                            BYTE nodeIdSize;
                            Printf("Channel: ");
                            PrintDec(ActiveScanResults[scanIndex].Channel);
                            Printf("   RSSI: ");
                            PrintChar(ActiveScanResults[scanIndex].RSSIValue);
                            Printf("   Peer Info: ");
                            myChannel = ActiveScanResults[scanIndex].Channel;
                            #if ADDITIONAL_NODE_ID_SIZE > 0           
                                for(nodeIdSize = 0; nodeIdSize < ADDITIONAL_NODE_ID_SIZE; nodeIdSize++)
                                {
                                    PrintChar(ActiveScanResults[scanIndex].PeerInfo[nodeIdSize]);
                                    Printf(" ");
                                }           
                                    Printf("\r\n");
                                
                                
                                connectionAddress[0] = ActiveScanResults[scanIndex].PeerInfo[0];
                                connectionAddress[1] = ActiveScanResults[scanIndex].PeerInfo[1];
                                connectionAddress[2] = ActiveScanResults[scanIndex].PeerInfo[2];                                    
                                
                            #endif
                            /*********************************************************/
                            //Display the network information on the LCD
                            //Line 1: RB0: (PC) or (C) with MACAddress (LSB 3 bytes)
                            //Line 2: RB2: Additnl NW - (j)
                            /*********************************************************/
                            LCDErase();
                            
                            #if ADDITIONAL_NODE_ID_SIZE > 0
                            
                                sprintf
                                    (
                                        (char *)LCDText,
                                            (far rom char *)
                                                "RB0:(C)%02x%02x%02x", ActiveScanResults[scanIndex].PeerInfo[0],
                                                    ActiveScanResults[scanIndex].PeerInfo[1], ActiveScanResults[scanIndex].PeerInfo[2]
                                    );
                            #endif
                            

                            sprintf((char *) &(LCDText[16]), (far rom char *) "RB2:Additnl NW-%d", (scanresult - 1));
                            LCDUpdate();
                            /*************************************************************************/
                            //If RB0 is pressed connect to the network in ActiveScanResults[j]
                            //If RB2 is pressed print the next network details on the LCD (if available)
                            /*************************************************************************/
                            while(1)
                            {
                                if(PUSH_BUTTON_RB0 == 0)
                                {

                                    //Connect to this index
                                    BYTE    Status;
                                    while(PUSH_BUTTON_RB0 == 0);
                                    if(MiMAC_ReceivedPacket())
                                    {
                                        MiApp_DiscardMessage();
                                    }

                                    Status = MiApp_EstablishConnection(scanIndex, CONN_MODE_DIRECT);
                                    //Print Connection status on the console and LCD
                                    if(Status == 0xFF)
                                    {
                                        Printf("\r\nConnection Failed\r\n");
                                        LCDDisplay((char *)"Join Failed!!!", 0, TRUE);
                                        DelayMs(5000);
                                        goto CreateorJoin;
                                    }
                                    else
                                    {
                                        Printf("\r\nConnection Established with Peer\r\n");

                                        LCDDisplay((char *)"Joined  Network Successfully..", 0, TRUE);
                                        DelayMs(3000);

                                        NodeTemp[0].Address[0] = myLongAddress[2];
                                        NodeTemp[0].Address[1] = myLongAddress[1];
                                        NodeTemp[0].Address[2] = myLongAddress[0];
                                        
                                        /*********************************************************/
                                        //Use index 1 to store connectionAddress and temperature
                                        //sensor readings
                                        /*********************************************************/
                                        
                                        NodeTemp[1].Address[0] = connectionAddress[0];
                                        NodeTemp[1].Address[1] = connectionAddress[1];
                                        NodeTemp[1].Address[2] = connectionAddress[2];
                                        
                                        //Use value 255 to indicate the temperature sensor reading is not yet available
                                        
                                        NodeTemp[1].TempValue = 255;
                                        NodeIndex = 2;
                                        result = FALSE;
                                    }

                                    break;
                                }

                                if(PUSH_BUTTON_RB2 == 0)
                                {
                                    while(PUSH_BUTTON_RB2 == 0);
                                    //Rotate the index; for example 0-1-2-0-1-2-0 (if 3 networks are present)
                                    if((scanresult - scanIndex - 1) == 0)
                                    {
                                        scanIndex = -1;
                                    }

                                    break;
                                }
                            }

                            if(result == FALSE)
                            {
                                break;
                            }
                        }

                        result = FALSE;
                    }
                    else
                    {
                        /******************************************************************/                        
                        //If no networks are found print the message on the LCD and Console
                        //and ask the user to do a re-scan
                        /******************************************************************/
                        Printf("\r\nNo networks were found\r\n");
                        LCDDisplay((char *)"0 Networks FoundRB2: Re-Scan", 0, TRUE);
                    }
                    
                }
                
            }
            
        }
        
    }
    return TRUE;
} 
/*********************************************************************
* Function:         void ReadTempSensor(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Reads the ADC value and converts it to temperature in
*                   celsius
*
* Note:			    
**********************************************************************/
void ReadTempSensor()
{
    
    
    BYTE    tempArray[samplingRate];
    BYTE    i = 0;

    //Power-up Temperature Sensor								
    LATAbits.LATA0 = 1;
    DelayMs(2);             //Delay 2 ms after powering up the temperature sensor
    ConfigADC();
    do
    {
        ADCON0bits.ADON = 1;
        Delay10us(1);       //Acquisition time
        ADCON0bits.GO = 1;
        while(ADCON0bits.DONE);
        {
            WORD    tempValue;
            BYTE    inttemp;
            double  temp;
            temp = (1200.0 / VBGResult);
            tempValue = ADRES;

            temp = (temp * tempValue);
            temp = (temp - 500.0) / 10.0;
            tempArray[i] = (BYTE) temp;
        }

        ADCON0bits.ADON = 0;
        Delay10us(1);       //A minimum 2TAD is required before the next acquisition starts
        i++;
    } while(i < samplingRate);
    
    LATAbits.LATA0 = 0;
    {
        float   tempAverage = 0;
        for(i = 0; i < samplingRate; i++)
        {
            tempAverage = (tempAverage + tempArray[i]);
        }

        tempAverage = (tempAverage / samplingRate);
        tempLocal = (BYTE) tempAverage;
        tempAverage = (tempAverage - tempLocal) * 10;
        if(tempAverage >= 5)
        {
            tempLocal = tempLocal + 1;
        }
    }

    
    ConsolePut(tempLocal);
    
    


    //Update NodeTemp Structure
    NodeTemp[0].TempValue = tempLocal;
    

    //Update StoreTemp Array
    
    storeTemp[storeTemp_currentindex] = tempLocal;
    storeTemp_currentindex++;
    if(storeTemp_currentindex == MAXLIMIT_STORETEMP)
    {
        storeTemp_currentindex = 0;
    }

    if(storeTemp_index <= MAXLIMIT_STORETEMP)
    {
        storeTemp_index++;
    }
    


    if(En_1mindisplay)          
    {

        //display all the nodes temperature sensor readings
        BYTE    j = NodeIndex;
        BYTE    i = 0;
        while(j >= 1)
        {
            if(i == 0)
            {
                Printf("\n\r\n\rLocal Temp (");
            }
            else
            {
                Printf("\n\rRemote Temp at ");
            }

            PrintChar(NodeTemp[i].Address[0]);
            PrintChar(NodeTemp[i].Address[1]);
            PrintChar(NodeTemp[i].Address[2]);

            if(i == 0)
            {
                Printf("): ");
            }
            else
            {
                Printf(": ");
            }

            PrintDec(NodeTemp[i].TempValue);
            Printf("°C/");
            {
                float   temp;
                BYTE    tempF;
                temp = NodeTemp[i].TempValue;
                temp = ((temp * 1.8) + 32);
                tempF = (BYTE) temp;
                PrintDec(tempF);
            }

            Printf("°F");
            i++;
            j--;
        }
    }

    
    if(readSensor)
    {
        readSensor = FALSE;
        Printf("\r\nEnter 'ESC' Key to switch to Interactive Mode\r\n");
        LCDBacklightON();
        PrintTempLCD();
        DelayMs(2000);
        LCDBacklightOFF();
    }
    else
    {
        PrintTempLCD();
    }

    tick1 = MiWi_TickGet();

    
}

/*********************************************************************
* Function:         void TransmitMessage(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Transmit the Message with the new temperature 
*                   information
*
* Note:			    
**********************************************************************/
void TransmitMessage()
{
    /************************************************/
            //Update Tx Buffer with the new reading
    /************************************************/
    MiApp_FlushTx();
 
    //Write temperature reading
    MiApp_WriteData(tempLocal);
    MiApp_WriteData(myLongAddress[2]);
    MiApp_WriteData(myLongAddress[1]);
    MiApp_WriteData(myLongAddress[0]);

    /*******************************************************************/
    // Broadcast the temperature sensor reading along with the last 3 
    // bytes of MACAddress
    // Function MiApp_BroadcastPacket is used to broadcast a message
    // The only parameter is the boolean to indicate if we need to
    // secure the frame

    /*******************************************************************/
    MiApp_BroadcastPacket(FALSE);
    
}
    
/*********************************************************************
* Function:         void ProcessRxMessage(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    Create a data structure for the node if it does 
*                   not already exist otherwise update the remote
*                   temperature information based on the latest value
*                   in the RxMessage
*
* Note:			    
**********************************************************************/   
void ProcessRxMessage(void)
{
    /*******************************************************************/

    // If a packet has been received, read the temperature sensor reading
    // of the other and store the values
    
    // If booting up from NVM is used for then create the NodeTemp[] 
    // as there is no handshake process (for creating NodeTemp[] during
    // create or join NW)
    /*******************************************************************/
    if(NVMEnable)
    {
        if(connectionEntry == FALSE)
        {
            if((rxMessage.flags.bits.srcPrsnt))
            {
                if((rxMessage.SourceAddress[1] == 0x00) && (rxMessage.SourceAddress[0] == 0x00))
                {
                    if(rxMessage.SourcePANID.Val == myPANID.Val)
                    {
                        connectionAddress[0] = rxMessage.Payload[1];
                        connectionAddress[1] = rxMessage.Payload[2];
                        connectionAddress[2] = rxMessage.Payload[3];

                        //Printf("\r\nCreated connection Address, connection entry for PAN Coordinator");
                        NodeTemp[1].Address[0] = connectionAddress[0];
                        NodeTemp[1].Address[1] = connectionAddress[1];
                        NodeTemp[1].Address[2] = connectionAddress[2];
                        NodeTemp[1].TempValue = 255;
                        NodeIndex = 2;
                        connectionEntry = TRUE;
                    }
                }
            }
        }
    }

    //If role is PAN Coordinator accept all the messages as store the information in the NodeTemp[] structure
    //If role is not a PAN Coordinator the 
    if(((connectionAddress[0] == rxMessage.Payload[1]) &&(connectionAddress[1] == rxMessage.Payload[2]) &&(connectionAddress[2] == rxMessage.Payload[3])) ||(role == ROLE_PAN_COORDINATOR))
    {
        tempRemote = rxMessage.Payload[0];
        

        
        if(role == ROLE_PAN_COORDINATOR)
        {
            if(NodeIndex > 1)
            {
                BOOL AddressFound = FALSE;
                BYTE index = 0;

                //Check if the node address is already registered in NodeTemp data structure
                for(index = 1; index < NodeIndex; index++)
                {
                    if
                    (
                        (NodeTemp[index].Address[0] == rxMessage.Payload[1]) &&
                        (NodeTemp[index].Address[1] == rxMessage.Payload[2]) &&
                        (NodeTemp[index].Address[2] == rxMessage.Payload[3])
                    )
                    {

                        //Address already registered
                        //Update the temp value
                        NodeTemp[index].TempValue = tempRemote;
                        AddressFound = TRUE;
                        break;
                    }
                }

                if(!AddressFound)

                //Create an index
                {
                    NodeTemp[NodeIndex].Address[0] = rxMessage.Payload[1];
                    NodeTemp[NodeIndex].Address[1] = rxMessage.Payload[2];
                    NodeTemp[NodeIndex].Address[2] = rxMessage.Payload[3];
                    NodeTemp[NodeIndex].TempValue = tempRemote;
                    NodeIndex++;
                }
            }
            else
            {
                NodeTemp[NodeIndex].Address[0] = rxMessage.Payload[1];
                NodeTemp[NodeIndex].Address[1] = rxMessage.Payload[2];
                NodeTemp[NodeIndex].Address[2] = rxMessage.Payload[3];
                NodeTemp[NodeIndex].TempValue = tempRemote;
                NodeIndex++;
            }
        }
        else
        {
            NodeTemp[1].TempValue = tempRemote;
        }

        PrintTempLCD();     
    }
    
}

/*********************************************************************
* Function:         void SwitchToInteractiveMode(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    This function supports the interactive mode, where 
*                   the user can get more details about the nodes
*
* Note:			    
**********************************************************************/
void SwitchToInteractiveMode()
{
    BYTE input = ConsoleGet();
    if((input == 27) || (interactivemode == TRUE))
    {
        ConsolePut(input);
        switch(input)
        {
            case 27:   
                /****************************************************************/
                //Press 'ESC' Key to switch to interactive mode
                //ESC character
                //If already in the interactive mode, this re-prints the menu
                /****************************************************************/                            
                interactivemode = TRUE;
                En_1mindisplay = FALSE;

                //Print Interactive Mode main menu ----
                Printf("\n\r------------------------------------------------------------\r\n");
                Printf("                     Interactive Mode                       \r\n");
                Printf("------------------------------------------------------------\r\n");
                Printf("Menu:\r\n");
                Printf("1. Display connection status\r\n");
                Printf("2. Display battery status\r\n");
                Printf("3. Display node's recorded temperature sensor readings\r\n");
                Printf("4. Print temp sensor readings (all nodes) for every 15 secs\r\n");
                Printf(">>");
                break;

            case '2':

                /****************************************************************/    
                //Case '2'/50
                //Display batttery Status
                /****************************************************************/
                
                En_1mindisplay = FALSE;
                Printf("\n\rBattery Status:\r\n");
                Printf("Battery level is indicated using '|||'\r\n\n");
                Printf("<");
                {
                    float   voltage = (1226.7 / VBGResult);

                    float   index = 2.1;
                    while(index < 3.6)
                    {
                        if(voltage > index)
                        {
                            Printf("--- ");
                        }
                        else
                        {
                            Printf("||| ");
                            index = index + 0.1;
                            while(index < 3.6)
                            {
                                Printf("--- ");
                                index = index + 0.1;
                            }
                        }

                        index = index + 0.1;
                    }
                }

                Printf(">\r\n 2.1         2.4         2.7         3.0         3.3         3.6 (V)\r\n>>");
                break;

            case '1':
                En_1mindisplay = FALSE;
                /****************************************************************/
                //Case '1'
                //Display connection Status
                /****************************************************************/
                
                Printf("\n\rConnection Status:\r\n");
                if(role == ROLE_PAN_COORDINATOR)
                {
                    BYTE index = 0;

                    //Print Coordinator address
                    Printf("\r\nI AM PAN Coordinator: ");
                    PrintChar(NodeTemp[0].Address[0]);
                    PrintChar(NodeTemp[0].Address[1]);
                    PrintChar(NodeTemp[0].Address[2]);

                    //Print Joining nodes address
                    Printf("\r\nDirectly Connected Nodes: ");
                    for(index = 1; index < NodeIndex; index++)
                    {
                        Printf("\r\n Node: ");
                        PrintChar(NodeTemp[index].Address[0]);
                        PrintChar(NodeTemp[index].Address[1]);
                        PrintChar(NodeTemp[index].Address[2]);
                    }
                }
                else
                {
                    Printf("\r\nConnection Established with: ");
                    PrintChar(NodeTemp[1].Address[0]);
                    PrintChar(NodeTemp[1].Address[1]);
                    PrintChar(NodeTemp[1].Address[2]);
                }

                break;

            case '3':
                /****************************************************************/
                //Case '3'
                //Display's Temperature sensor past accumulated readings
                /****************************************************************/
                
                En_1mindisplay = FALSE;
                Printf("\n\rRecorded Temperature Sensor readings\r\nIn the order of Latest values on Top");
                if(storeTemp_index == 0)
                {
                    Printf("No recorded Readings\r\n");
                }
                else
                {
                    BYTE arrayIndex = storeTemp_currentindex;
                    BYTE index = 0;
                    if(arrayIndex == 0)
                    {
                        arrayIndex = 99;
                    }
                    else
                    {
                        arrayIndex--;
                    }

                    Printf("\r\n");
                    for(index = 0; index < storeTemp_index; index++)
                    {
                        PrintDec(index + 1);
                        Printf("\t");
                        PrintDec(storeTemp[arrayIndex]);
                        Printf("°C\r\n");
                        if(arrayIndex == 0)
                        {
                            arrayIndex = 99;
                        }
                        else
                        {
                            arrayIndex--;
                        }
                    }
                }

                Printf("Finished Dump!!\n\r>>");
                break;

            case '4':
                /****************************************************************/
                //Case '4'
                //Display Temperature sensor reading for every 1 sec
                /****************************************************************/
                En_1mindisplay = TRUE;
                Printf("\n\rTemperature Sensor readings:\r\n");
                break;

            default:
                En_1mindisplay = FALSE;
                Printf("\r\n>>");
                break;
        }               //end of switch statement
        
    }
}
    
    

    
