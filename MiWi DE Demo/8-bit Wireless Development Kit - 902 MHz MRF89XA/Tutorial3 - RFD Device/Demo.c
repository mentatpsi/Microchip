/****************************************************************************
* FileName:		Demo.c
* Dependencies: none   
* Processor:	PIC18F46J50	
* Complier:     Microchip C18 v3.04 or higher
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice for P2P Software:
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

BYTE    previouscasevalue = 0;


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
    BoardInit();    //Has LCDInit() also covered in this, need to make this separate to ensure that if there was a problem with I2C the board hangs up

  
    /*****************************************************************/

    //Read the Button Status to determine if RB0 is pressed.

    /****************************************************************/

    /*******************************************************************/

    // Initialize the system

    /*******************************************************************/
    if(PUSH_BUTTON_RB0 == 0)
    {
        while(PUSH_BUTTON_RB0 == 0);

        //Enable Network Freezer Mode
        LCDDisplay((char *)"Network Freezer?Yes[RB0]/No[RB2]", 0, TRUE);
        #ifdef ENABLE_CONSOLE
        Printf("\r\nEnable Network Freezer?\n\rPress RB0 - Yes, RB2-No\r\n");
        #endif
        while(1)
        {
            if(PUSH_BUTTON_RB0 == 0)
            {
                MiApp_ProtocolInit(TRUE);
                NVMEnable = TRUE;

                LCDDisplay((char *)"Network Freezer Mode Enabled", 0, TRUE);
                DelayMs(2000);
                Printf("\n\rNetwork Freezer mode is enabled. There will be no handshake process\r\n");
                break;
            }

            if(PUSH_BUTTON_RB2 == 0)
            {
                NVMEnable = FALSE;
                LCDDisplay((char *)"Network Freezer Mode Disabled", 0, TRUE);
                DelayMs(2000);
                break;
            }
        }
    }

    /*******************************************************************/

    // Following block display demo information on LCD
    //LCDBacklightON() - Switches on the LCD Backlight
    //LCDBacklightOFF() - Switches of LCD Backlight
    //Can use Automatic Backlight mode for the LCD instead, which switches
    //on the backlight for every LCD display update

    /*******************************************************************/
    if(!NVMEnable)
    {
        LCDBacklightON();
        LCDDisplay((char *)"8-Bit Wireless  Development Kit ", 0, TRUE);
        DelayMs(5000);
        LCDBacklightOFF();

              
        LED_1 = 0;
        LED_2 = 0;

        /*******************************************************************/

        // Initialize Microchip proprietary protocol. Which protocol to use
        // depends on the configuration in ConfigApp.h

        /*******************************************************************/

        /*******************************************************************/

        // Function MiApp_ProtocolInit initialize the protocol stack. The
        // only input parameter indicates if previous network configuration
        // should be restored. In this simple example, we assume that the
        // network starts from scratch.

        /*******************************************************************/
        ReadMacAddress();
CreateorJoin:
       
        MiApp_ProtocolInit(FALSE);

        //	myPANID.Val = (WORD) myLongAddress[7];
        // Set default channel
        if(MiApp_SetChannel(myChannel) == FALSE)
        {
            return;
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

        // Display current opertion on LCD of demo board, if applicable

        /*******************************************************************/
        LCDDisplay((char *)"RFD Device      Scan in progress", 0, TRUE);
        {
            BOOL    result = TRUE;
            while(result == TRUE)
            {
                volatile BYTE   scanresult;
                Nop();
                Nop();
                MiApp_ProtocolInit(FALSE);
                if(MiMAC_ReceivedPacket())
                {
                    MiApp_DiscardMessage();
                }

                scanresult = MiApp_SearchConnection(10, 0x00000800);  //for channel 11
                
                
                if(scanresult > 0)
                {

                    // now print out the scan result.
                    #ifdef ENABLE_CONSOLE
                    Printf("\r\nActive Scan Results: \r\n");
                    #endif
                    for(j = 0; j < scanresult; j++)
                    {
                        #ifdef ENABLE_CONSOLE
                        Printf("Channel: ");
                        PrintDec(ActiveScanResults[j].Channel);
                        Printf("   RSSI: ");
                        PrintChar(ActiveScanResults[j].RSSIValue);
                       
                        #endif
                        myChannel = ActiveScanResults[j].Channel;

                        connectionAddress[0] = ActiveScanResults[j].PeerInfo[0];
                        connectionAddress[1] = ActiveScanResults[j].PeerInfo[1];
                        connectionAddress[2] = ActiveScanResults[j].PeerInfo[2];

                        //Display the index on LCD
                        LCDErase();
                        
                        sprintf
                            (
                                (char *)LCDText,
                                    (far rom char *)
                                        "RB0:(C)%02x%02x%02x", ActiveScanResults[j].PeerInfo[0],
                                            ActiveScanResults[j].PeerInfo[1], ActiveScanResults[j].PeerInfo[2]
                            );
                   
                        sprintf((char *) &(LCDText[16]), (far rom char *) "RB2:Additnl NW-%d", (scanresult - 1));
                        LCDUpdate();
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

                                Status = MiApp_EstablishConnection(j, CONN_MODE_DIRECT);
                                if(Status == 0xFF)
                                {
                                    #ifdef ENABLE_CONSOLE
                                    Printf("\r\nConnection Failed\r\n");
                                    #endif
                                    LCDDisplay((char *)"Join Failed!!!", 0, TRUE);
                                    DelayMs(5000);
                                    goto CreateorJoin;
                                }
                                else
                                {
                                    #ifdef ENABLE_CONSOLE
                                    Printf("\r\nConnection Established with Peer\r\n");
                                    #endif
                                    LCDDisplay((char *)"Joined  Network Successfully..", 0, TRUE);
                                    DelayMs(3000);

                                    NodeTemp[0].Address[0] = myLongAddress[2];
                                    NodeTemp[0].Address[1] = myLongAddress[1];
                                    NodeTemp[0].Address[2] = myLongAddress[0];

                                    NodeTemp[1].Address[0] = connectionAddress[0];
                                    NodeTemp[1].Address[1] = connectionAddress[1];
                                    NodeTemp[1].Address[2] = connectionAddress[2];
                                    NodeTemp[1].TempValue = 255;
                                    NodeIndex = 2;
                                    result = FALSE;
                                }

                                break;
                            }

                            if(PUSH_BUTTON_RB2 == 0)
                            {
                                while(PUSH_BUTTON_RB2 == 0);
                                if((scanresult - j - 1) == 0)
                                {
                                    j = -1;
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
                    #ifdef ENABLE_CONSOLE
                    Printf("\r\nNo networks were found\r\n");
                    #endif
                    LCDDisplay((char *)"0 Networks FoundRB2: Re-Scan", 0, TRUE);
                }
            };
        }
    }

    /*******************************************************************/

    // Function DumpConnection is used to print out the content of the
    //  Connection Entry on the hyperterminal. It may be useful in
    //  the debugging phase.
    // The only parameter of this function is the index of the
    //  Connection Entry. The value of 0xFF means to print out all
    //  valid Connection Entry; otherwise, the Connection Entry
    //  of the input index will be printed out.

    /*******************************************************************/
    #ifdef ENABLE_DUMP
    DumpConnection(0xFF);
    #endif
    #ifndef ENABLE_POWERSAVE

    // Turn on LED 1 to indicate P2P connection established
    LED_1 = 1;
    #endif
    Read_VBGVoltage();          //Measure the VDD to use in conversion

    //Band gap reference voltage is 1.2 V value, so read this to convert, the measure the VDD

    /*******************************************************************/

    // Following block display demo instructions on LCD based on the
    // demo board used.

    /*******************************************************************/
    readSensor = TRUE;
    tick1 = MiWi_TickGet();
    TRISAbits.TRISA0 = 0;       //Configure the Temp sensor port

    //add this to board init steps
    while(1)
    {
        tick2 = MiWi_TickGet();
        if((MiWi_TickGetDiff(tick2, tick1) > (ONE_SECOND * 10)) | readSensor | (PUSH_BUTTON_RB0 == 0))
        {
            
            BYTE    tempArray[samplingRate];
            BYTE    i = 0;
            
            while(PUSH_BUTTON_RB0 == 0);
            //Power-up Temperature Sensor								
            LATAbits.LATA0 = 1;
            DelayMs(2);         //Delay 2 ms after powering up the temperature sensor
            ConfigADC();
            do
            {
                ADCON0bits.ADON = 1;
                Delay10us(1);   //Acquisition time
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
                Delay10us(1);   //A minimum 2TAD is required before the next acquisition starts
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

            //This Print is for GUI version - disable the remaining prints for GUI version
            #ifdef ENABLE_GUI
            ConsolePut(tempLocal);
            #endif
            MiApp_FlushTx();
            if(1)
            {

                //Write temperature reading
                MiApp_WriteData(tempLocal);
                MiApp_WriteData(myLongAddress[2]);
                MiApp_WriteData(myLongAddress[1]);
                MiApp_WriteData(myLongAddress[0]);
            }


            //Update NodeTemp Structure
            {
                NodeTemp[0].TempValue = tempLocal;
            }

            //Update StoreTemp Array
            {
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
            }

            /*******************************************************************/

            // Function MiApp_BroadcastPacket is used to broadcast a message
            //    The only parameter is the boolean to indicate if we need to
            //       secure the frame

            /*******************************************************************/
            MiApp_BroadcastPacket(FALSE);

            
            /*******************************************************************/

            // Following block update the total received and transmitted
            // messages on the LCD of the demo board.

            /*******************************************************************/
            if(readSensor)
            {
                readSensor = FALSE;
               
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

        /*******************************************************************/

        // Function MiApp_MessageAvailable returns a boolean to indicate if
        // a packet has been received by the transceiver. If a packet has
        // been received, all information will be stored in the rxFrame,
        // structure of RECEIVED_MESSAGE.

        /*******************************************************************/
        else if(MiApp_MessageAvailable())
        {

            /*******************************************************************/

            // If a packet has been received, following code prints out some of
            // the information available in rxMessage.

            /*******************************************************************/
            if(NVMEnable)
            {
                if(connectionEntry == FALSE)
                {
                    if((rxMessage.flags.bits.srcPrsnt))
                    {
                        if((rxMessage.SourceAddress[1] == 0x00) & (rxMessage.SourceAddress[0] == 0x00))
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

            if
            (
                (
                    (connectionAddress[0] == rxMessage.Payload[1]) &&
                    (connectionAddress[1] == rxMessage.Payload[2]) &&
                    (connectionAddress[2] == rxMessage.Payload[3])
                )
            )
            {
                tempRemote = rxMessage.Payload[0];
                for(i = 0; i < rxMessage.PayloadSize; i++)
                { }

               
                NodeTemp[1].TempValue = tempRemote;

             

                /*******************************************************************/

                // Function MiApp_DiscardMessage is used to release the current
                //  received packet.
                // After calling this function, the stack can start to process the
                //  next received frame

                /*******************************************************************/
                MiApp_DiscardMessage();
            }
            else
            {
                for(i = 0; i < rxMessage.PayloadSize; i++)
                { }

                MiApp_DiscardMessage();
            }
        }
        else if (PUSH_BUTTON_RB2 == 0)
        {

            /*******************************************************************/

            // If no packet received, now we can check if we want to send out
            // any information.
            // Function ButtonPressed will return if any of the two buttons
            // has been pushed.

            /*******************************************************************/
                    while(PUSH_BUTTON_RB2 == 0);
                
                    CurrentNodeIndex++;
                    if(CurrentNodeIndex >= NodeIndex)
                    {
                        CurrentNodeIndex = 0;
                    }

                    PrintTempLCD();
                 
            }
        }
} 

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
            myLongAddress[2] = SPIGet();
            myLongAddress[1] = SPIGet();
            myLongAddress[0] = SPIGet();
            myLongAddress[3] = 0xFE;
            myLongAddress[4] = 0xFF;
            myLongAddress[5] = 0xA3;
            myLongAddress[6] = 0x04;
            myLongAddress[7] = 0x00;

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

/* */
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

/* */
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
