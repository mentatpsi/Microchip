/**********************************************************************
* FileName:		SelfTestMode.c
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
* SOFTWARE AND DOCUMENTATION ARE PROVIDED ï¿½AS ISï¿½ WITHOUT WARRANTY OF ANY 
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
****************************************************************************/
#include "SelfTestMode.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/SymbolTime.h"

#define EEPROM_MAC_ADDR 0xFA

BYTE switch_val;
BYTE EEPROM_Val[3];
BYTE SST_Val;
BYTE init_case = 0x00;


void SelfTest(BYTE myChannel)
{

    LCDDisplay((char *)"   MiWi  Demo    Self Test Mode ", 0, TRUE);
    LCDDisplay((char *)"SW1: Start Test SW2: Exit Test", 0, TRUE);
    init_case = 0;
    while(1)
    {
    switch_val = ButtonPressed();
    if(switch_val == SW1)
        {
            //SW1 and SW2 Push Buttons verified
            //LCD Display and Backlight verified (MiWi Card Demo Display)
            while(1)
            {
                LCDDisplay((char *)"Testing ...", 0, TRUE);
                LED0 = LED1 = LED2 = 0;
                switch(init_case)
                {
                    case 0: //Test  MAC EEPROM 
                          
                            EEPROMRead(&(EEPROM_Val[0]), EEPROM_MAC_ADDR, 3);
                            if((EEPROM_Val[0] == 0x00) && (EEPROM_Val[1] == 0x04) && (EEPROM_Val[2] == 0xA3))
                                init_case++;
                            else init_case = 255;
                            break;
                    case 1: //Test SST Flash
                         
                            SSTGetID(&SST_Val);
                            if(SST_Val == 0xBF)
                                init_case++;
                            else init_case = 255;
                            break;
                    case 2: //Test Radio
                         
                            if (MiApp_SetChannel(myChannel) == FALSE) 
                            {
                                init_case = 254;
                                break;
                            }
                            //Scan for networks (Testing TX and RX functionality
                            {
                               
                                BOOL Tx_Success = FALSE;
                                BYTE Tx_Trials = 0, scanresult = 0;
                                
                                while(!Tx_Success)
                                {
                                    if(myChannel < 8)
                                        scanresult = MiApp_SearchConnection(10, (0x00000001 << myChannel));
                                    else if(myChannel < 16)
                    				    scanresult = MiApp_SearchConnection(10, (0x00000100 << (myChannel-8)));
                                    else if(myChannel < 24)
                    				    scanresult = MiApp_SearchConnection(10, (0x00010000 << (myChannel-16)));
                                    else 
                    				    scanresult = MiApp_SearchConnection(10, (0x01000000 << (myChannel-24)));
                                    if(scanresult == 0)
                                    {
                                        Tx_Trials++;
                                        if(Tx_Trials > 2) break;
  
                                    }
                                    else Tx_Success = TRUE; 
                                   
                                }
                                if(Tx_Success) init_case = 254;
                                else init_case = 255;
                            }
                            break;
                   
                    case 254:   LCDDisplay("Self Test Mode  Test Pass", 0, TRUE);
                                LED0 = LED1= LED2 = 1;
                                init_case = 0xF0;
                                break;
                    case 255:   LCDDisplay("Self Test Mode  Test Fail", 0, TRUE);
                                LED0 = LED1 = LED2 = 1;
                                init_case = 0xF0;
                                break;
                    default:    break;
    
                }
                
            if(init_case == 0xF0) 
            {
                while(1)
                {
                    switch_val = ButtonPressed();
                    if((switch_val == SW1) || (switch_val == SW2))
                        break;
                }
                break;
            }                       
            }
            break;

        }
    else if(switch_val == SW2)
        {
            break;
        }
    }
}