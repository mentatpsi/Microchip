/*****************************************************************************
 *
 *              SelfTestMode.c -- Self Test Mode
 *
 *****************************************************************************
 * FileName:        SelfTest.c
 * Dependencies:
 * Processor:       PIC18
 * Compiler:        C18 02.20.00 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
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
 *****************************************************************************
 * File Description:
 *
 *   This file provides configuration and basic hardware functionality 
 *   based on chosen hardware demo boards.
 *
 * Change History:
 *  Rev   Date         Description
 *  0.1   5/3/2011    Initial revision
 *****************************************************************************/
 #include "SelfTestMode.h"
 #include "WirelessProtocols/SymbolTime.h"
 #include "HardwareProfile.h"
 #include "WirelessProtocols/Console.h"
 #include "LcdDisplay.h"
 #include "WirelessProtocols/MSPI.h"
 #include "TimeDelay.h"
 
 //Variable for Self Test Mode
 BYTE    previouscasevalue = 0;
 
 
void SelfTest()
{
    /*****************************************************************/
    //Read the Button Status to determine if RB0 or RB2 is pressed.
    //if RB2 is pressed along with MCLR, then go to Self test
    //mode.
    
    //PUSH_BUTTON_RB0  - RB0
    //PUSH_BUTTON_RB2  - RB2
    
    //Using the self test mode, users can verify if there is any 
    //problem with the hardware.
    //The self test mode for 8-bit Wireless Development Kit hardware
    //comprises of testing the  
    // - PIC18 Wireless Development Board
    //      Testing push buttons
    //      Verfying that LEDs work
    //      Verfying the SPI connection to the on-board EEPROM
    //      Verfying MCP9700A - Temperature sensor is working
    //      Testing the PICtail SPI connections (Module EEPROM)
    //- LCD Serial Accessory Board
    //- RS232 Serial Accessory Board
    //Note: To test PICtail SPI connections RF module needs to plugged
    //in.
    /****************************************************************/

    if(PUSH_BUTTON_RB2 == 0)
    {
        while(PUSH_BUTTON_RB2 == 0);
        {
            BOOL    result;
            BYTE    case_value = 0x00;
            
            
            while(case_value != 0xFF)
            {
                switch(case_value)
                {
                    /*****************************************************/
                    //Test LED's: Turns on both the LEDs (RA2 and RA3)
                    //If the user can see both the LEDs light up, then 
                    //press RB0 to continue with testing else press RB2 to 
                    //report error status
                    //LED_1 : RA2
                    //LED_2 : RA3
                    /*******************************************************/    
                    case 0:

                        //LED Test
                        result = LEDTest();
                        if(result == FALSE)
                        {
                            case_value = 0xFE;
                        }
                        else
                        {
                            case_value++;
                        }

                        break;
                        
                    /******************************************************/
                    //The program verifies the EEPROM by writing and reading 
                    //back from the EEPROM. If there is a problem with 
                    //EEPROM write/read operation, the Self test fail message is 
                    //indicated on the LEDs
                    //EEPROM fail Error Codes: LED_1 = OFF LED_2 = ON
                    //Note: EEPROM and Temperature sensor share same error code
                    /*******************************************************/

                    case 1:

                        //EEPROM Test
                        result = EEPROMTest();
                        if(result == FALSE)
                        {
                            
                            previouscasevalue = case_value;
                            case_value = 0xFE;
                            //Update EEPROM fail Status message on LEDs
                            LED_1 = 0;
                            LED_2 = 1;              

                            
                        }
                        else
                        {
                            case_value++;
                        }

                        break;

                    /*********************************************************/
                    //This program verifies the temperature sensor by reading
                    //back the values from temperature sensor.
                    //If the values are not in the normal range then Temperature
                    //sensor fail message is indicated on the LEDs
                    //Temperature Sensor fail Error Codes: LED_1: OFF LED_2 ON
                    //Note: EEPROM and Temperature sensor share same error code
                    /***********************************************************/
                    case 2:

                        //Temperature Sensor Test
                        result = MCP9700Test();
                        if(result == FALSE)
                        {
                            previouscasevalue = case_value;
                            case_value = 0xFE;
                            //Update Temperature Sensor Fail Status Message on LEDs
                            LED_1 = 0;
                            LED_2 = 1;

                            
                        }
                        else
                        {
                            case_value++;
                        }

                        break;
                    /*************************************************************/
                    //This program verifies the PICtail SPI connections by reading
                    //back the MACAddress EEPROM values.
                    //If the values read back contain the Microchip EUI then 
                    //it reporta back success status
                    //PICtail SPI fail Error Codes: LED_1: ON LED_2 OFF
                    /**************************************************************/

                    case 3:

                        //PICTail SPI Test
                        result = PICTailSPITest();
                        if(result == FALSE)
                        {
                            case_value = 0xFE;
                            //Update PICTail SPI fail Staus Message on LEDs
                            LED_1 = 1;
                            LED_2 = 0;

                            
                        }
                        else
                        {
                            case_value = 6;
                        }

                        break;

                    case 6:
                        /***********************************************************/
                        //Indicate all tests pass on PIC18 Wireless Development Board
                        
                        //If users want to continue testing with the LCD Serial
                        //press RB0 push button
                        /*************************************************************/
                        LEDStatus(TRUE, TRUE);  //All cases pass on the WDK board

                        //To continue with the testing the LCD board, the user needs to hit RB0 push button, else to exit
                        //user needs to hit MCLR or user needs to press RB2 to skip testing LCD and move on to RS232.
                        result = TRUE;
                        while(result == TRUE)
                        {
                            if(PUSH_BUTTON_RB0 == 0)
                            {

                                //RB0 is pressed
                                //Continue with testing LCD
                                while(PUSH_BUTTON_RB0 == 0);

                                result = FALSE;
                                case_value = 7;
                            }

                            if(PUSH_BUTTON_RB2 == 0)
                            {

                                //RB2 is pressed
                                while(PUSH_BUTTON_RB2 == 0);

                                result = FALSE;
                                case_value = 8;
                            }
                        };
                        break;
                    /*********************************************************/
                    //This program prints out the following message on the LCD
                    //line 1 : Self Test Mode
                    //line 2 : Wireless Dev Kit
                    //If the users can see this message on the LCD display then 
                    //press RB0 to indicate that LCD is working
                    //Otherwise press RB2 to indicate error                    
                    /*********************************************************/
                    case 7:
                        {

                            //Test LCD
                            //Display Welcome Message on LCD
                            result = TRUE;
                            LCDBacklightON();
                            LCDDisplay((char *)"Self Test Mode  Wireless Dev Kit", 0, TRUE);

                            //Proceed to next step UART
                            while(result == TRUE)
                            {
                                if(PUSH_BUTTON_RB0 == 0)
                                {

                                    //RB0 is pressed
                                    //LCD is working
                                    while(PUSH_BUTTON_RB0 == 0);

                                    LCDBacklightOFF();

                                    result = FALSE;
                                    case_value++;
                                }

                                if(PUSH_BUTTON_RB2 == 0)
                                {               //RB2 is pressed
                                    //LCD Not Working
                                    while(PUSH_BUTTON_RB2 == 0);

                                    LCDBacklightOFF();
                                    result = FALSE;

                                    case_value = 0xFF;
                                }
                            };
                        }

                        break;
                    /*****************************************************************
                    //This program verifies the RS232 Serial Accessory Board by 
                    //writing and reading back from the console
                    //Console Settings -
                    //  Baudrate - 19200
                    //  Data bits - 8
                    //  Parity - None
                    //  Stop Bits - 1
                    //  Flow Control - None
                    //If users can see the message -
                    //          Self Test Mode - Wireless Development Kit
                    //          Press any Key to continue with the Test
                    //on the hyperterminal window, then press any key to continue with the
                    //test
                    //If readback works then it reports success; otherwise fail
                    /**********************************************************************/
                    case 8:
                        {

                            //Send a Message over Console and readback data from the Console
                            //This function verifies both the TX and RX over Console
                            Printf("\r\nSelf Test Mode - Wireless Development Kit\r\n");
                            Printf("\r\nPress any key to continue with the Test\r\n");
                            result = TRUE;
                            {
                                MIWI_TICK   t1 = MiWi_TickGet();
                                while(result == TRUE)
                                {
                                    MIWI_TICK   t2;
                                    if(ConsoleIsGetReady())
                                    {

                                        //RB0 is pressed
                                        //Console is Working
                                        BYTE    read = ConsoleGet();
                                        ConsolePut(read);

                                        //LCDDisplay((char *)"Self Test Mode  UART Working", 0, TRUE);
                                        Printf("\r\nSelf Test Mode: UART Working");
                                        result = FALSE;
                                        case_value = 0xFF;
                                    }

                                    t2 = MiWi_TickGet();
                                    if(MiWi_TickGetDiff(t2, t1) > (ONE_SECOND * 60))
                                    {

                                        //Console Not Working
                                        result = FALSE;

                                        //LCDDisplay((char *)"Self Test Mode  UART Fail  ", 0, TRUE);
                                        Printf("\r\nSelf Test Mode: Fail");
                                        case_value = 0xFF;
                                    }
                                };
                            }
                        }

                        break;
                    /**********************************************************/
                    //If the program reports fail message with Error code
                    //LED_1 = OFF and LED_2 = ON
                    //To find out if the status message is for On-board EEPROM
                    //or for the temperature sensor - press RB0 push button
                    //Once RB0 is pressed, if the error message is due to 
                    //on-board EEPROM then LED status will not change; otherwise
                    //if the error message is due to MCP9700A temperature sensor
                    //then the LED's will be lighted as follows
                    // LED_1 = ON and LED_2 = OFF
                    //Note: This extra step is needed in the case only when error
                    //code is for EEPROM or Temp Sensor
                    /***********************************************************/
                    case 254:
                        LEDStatus(LED_1, LED_2);
                        if((previouscasevalue == 1) | (previouscasevalue == 2))
                        {
                            result = TRUE;
                            while(result == TRUE)
                            {
                                if(PUSH_BUTTON_RB0 == 0)
                                {

                                    //RB0 is pressed
                                    while(PUSH_BUTTON_RB0 == 0);

                                    result = FALSE;
                                }

                                if(PUSH_BUTTON_RB2 == 0)
                                {   //RB2 is pressed
                                    while(PUSH_BUTTON_RB2 == 0);
                                    if(previouscasevalue == 1)
                                    {
                                        LED_1 = 0;
                                        LED_2 = 1;
                                    }
                                    else
                                    {
                                        LED_1 = 1;
                                        LED_2 = 0;
                                    }

                                    LEDStatus(LED_1, LED_2);
                                    result = FALSE;
                                }
                            };
                        }

                        case_value++;
                        break;

                    default:
                        break;
                }
            };

            //To Exit Self Test Mode, Hit MCLR
            while(1)
            { };
        }
    }   //End of Self Test
}    


/*********************************************************************
* Function:         BOOL LEDTest(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    This function verifies the LED's on the Wireless
*					Development Kit
*
* Note:			    
**********************************************************************/
BOOL LEDTest(void)
{
    BYTE    result = 0, j = 0;
    WORD    i = 0;
    BOOL    status;
    LED_1 = 1;
    LED_2 = 1;

    while(result == 0)
    {
        if(PORTBbits.RB0 == 0)
        {
            while(PORTBbits.RB0 == 0);

            //RB0 is pressed
            //Continue with next step
            LEDStatus(FALSE, FALSE);
            result = 1;
            status = TRUE;
        }
        else if(PORTBbits.RB2 == 0)
        {
            while(PORTBbits.RB2 == 0);

            //RB2 is pressed
            LEDStatus(FALSE, FALSE);
            result = 1;
            status = FALSE;
        }
        else
        {
            i++;
            if(i == 5000)
            {
                i = 0;
                j++;
                if(j % 2 == 1)
                {
                    LED_1 = 0;
                    LED_2 = 0;
                }
                else
                {
                    LED_1 = 1;
                    LED_2 = 1;
                }
            }
        }
    };
    return (status);
}

/*********************************************************************
* Function:         BOOL EEPROMTest(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    This function writes a byte to the EEPROM and 
*					readbacks to verify that the EEPROM on Wireless
*					Development Kit is working
*
* Note:			    
**********************************************************************/
BOOL EEPROMTest(void)
{
    BYTE    readback;

    //On board EEPROM 25LC256 - Uses PICtail SPI interface
    //Enable WREN
    EE_nCS_TRIS = 0;
    EE_nCS = 1;

    //initial state
    EE_nCS = 0;
    SPIPut(0x06);   //WREN
    EE_nCS = 1;
    Nop();

    //Write Data
    EE_nCS = 0;
    SPIPut(0x02);   //WRITE
    SPIPut(0x00);
    SPIPut(0x01);   //address 0x0001
    SPIPut(0x01);   //data  0x01
    EE_nCS = 1;

    DelayMs(10);
    EE_nCS = 0;
    SPIPut(0x03);   //READ
    SPIPut(0x00);
    SPIPut(0x01);
    readback = SPIGet();
    EE_nCS = 1;
    if(readback != 0x01)
    {
        return (FALSE);
    }
    else
    {
        return (TRUE);
    }
}

/*********************************************************************
* Function:         BOOL MCP9700Test(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Test Pass/Fail indication
*
* Side Effects:	    none
*
* Overview:		    This funtion tests the temperature sensor on the
*      				Wireless Development Kit and determines whether
*					the module is working or not
* Note:			    
**********************************************************************/
BOOL MCP9700Test(void)
{
    return (TRUE);
}

/*********************************************************************
* Function:         BOOL PICTailSPITest(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Test Pass/Fail indication
*
* Side Effects:	    none
*
* Overview:		    This is used for validating the PICtail connections
*                   on the Wireless Development Kit 
*
* Note:			    
**********************************************************************/
BOOL PICTailSPITest(void)
{
    BYTE    readback;

    //Read the EEPROM
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

            //Printf("\r\nMac Address EEPROM Verified\r\n");
            return (TRUE);
        }
        else
        {
            return (FALSE);
        }
    }
}

/*********************************************************************
* Function:         void LEDStatus(BOOL LED1Status, BOOL LED2Status)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Test Pass/Fail indication
*
* Side Effects:	    none
*
* Overview:		    This is used to indicate the Test Pass/Fail using LED's
*
* Note:			    
**********************************************************************/
void LEDStatus(BOOL LED1Status, BOOL LED2Status)
{
    if(LED1Status)
    {
        LED_1 = 1;
    }
    else
    {
        LED_1 = 0;
    }

    if(LED2Status)
    {
        LED_2 = 1;
    }
    else
    {
        LED_2 = 0;
    }
}

