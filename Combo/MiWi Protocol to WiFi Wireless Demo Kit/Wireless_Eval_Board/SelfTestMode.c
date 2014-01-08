/*********************************************************************
 *
 *  Self Test Procedure - Used to test all the hardware components on
 *                        the board
 *
 *********************************************************************
 * FileName:        SelfTestMode.c
 * Dependencies:    TCP/IP stack
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
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
 * Sushma Myneni       	02/21/13	    Original v(1.0)
 ********************************************************************/
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
#include "LCD_ST7032.h"
#include "SelfTestMode.h"
#include "MiWi_SPI.h"
#include "MainDemo.h"



WORD Read_VBGResult(void) {
    WORD ADCValue;

    AD1CON1 = 0x80E4;
    AD1CON2 = 0x00;
    AD1CON3 = 0x1F05;

    AD1CON1SET = 0x8000;

    AD1CON1SET = 0x0002;
    DelayMs(100);
    AD1CON1CLR = 0x0002;
    while (!(AD1CON1 & 0x0001));
    ADCValue = ADC1BUF0;

    return ADCValue;
}

BYTE ReadTempSensor(void) {

    WORD ADCValue;
    double tempV;
    BYTE temp;
    BYTE SIGN = 0x00;

    ANALOG_TEMP_TRIS = 1;
    mPORTBSetPinsAnalogIn(BIT_10);
    //Configure ADC
    AD1PCFG = 0xFBFF; //All port B digital; RB10 analog
    AD1CON1 = 0x0000;

    AD1CHS = 0x000A0000;

    AD1CSSL = 0;
    AD1CON3 = 0x0002;
    AD1CON2 = 0x0000;


    AD1CON1SET = 0x8000;

    AD1CON1SET = 0x0002;
    Delay10us(10);
    AD1CON1CLR = 0x0002;
    while (!(AD1CON1 & 0x0001));
    ADCValue = ADC1BUF0;
    tempV = ADCValue;
    tempV = (3.3 * tempV);
    if (tempV > 500)
        tempV = ((tempV - 500.0) / 10.0);
    else {
        tempV = ((500 - tempV) / 10.0);
        SIGN = 0x80;
    }
    temp = (BYTE) tempV;
    temp = (temp | SIGN);
    //Turn off TempSensor
    mPORTBSetPinsDigitalIn(BIT_10);
    return temp;
}

WORD ReadAnalogPOT(void) {
    WORD ADCValue;

    ANALOG_POT_TRIS = 1;
    mPORTBSetPinsAnalogIn(BIT_11);
    //Configure ADC
    AD1PCFG = 0xF7FF; //All port B digital; RB11 analog
    AD1CON1 = 0x0000;

    AD1CHS = 0x000B0000;

    AD1CSSL = 0;
    AD1CON3 = 0x1F05;
    AD1CON2 = 0;


    AD1CON1SET = 0x8000;

    AD1CON1SET = 0x0002;
    DelayMs(100);
    AD1CON1CLR = 0x0002;
    while (!(AD1CON1 & 0x0001));
    ADCValue = ADC1BUF0;

    //Turn off AnalogPOT
    mPORTBSetPinsDigitalIn(BIT_11);

    return ADCValue;
}

BOOL TestFlash() {
    BYTE readV;
    //LCD and SPI Flash Share the same SPI3
    ConfigureFlash_SPI();


    SPIFLASH_CS_IO = 1;
    SPIFLASH_CS_IO = 0;
    Flash_SPIPut(SPI_READ_ID);
    Flash_SPIPut(0x00);
    Flash_SPIPut(0x00);
    Flash_SPIPut(0x00);

    readV = Flash_SPIGet();
    SPIFLASH_CS_IO = 1;
    //LCD and SPI Flash Share the same SPI3; Reset to defaults
    ConfigureLCD_SPI();

    if ((readV == 0xBF) || (readV == 0x8D))
        return TRUE;
    else
        return FALSE;

}

void Flash_SPIPut(BYTE v) {
    BYTE tmp;

    SpiChnPutC(SPI_CHANNEL3, v);
    tmp = SpiChnGetC(SPI_CHANNEL3);
}

BYTE Flash_SPIGet(void) {
    BYTE data;

    SpiChnPutC(SPI_CHANNEL3, 0x00);
    data = SpiChnGetC(SPI_CHANNEL3);

    return data;
}

BOOL TestEEPROM() {
    DWORD address = 0;
    BYTE val = 0;
    XEEInit();
    XEEBeginWrite(address);
    XEEWrite(0x55);
    XEEEndWrite();
    XEEBeginRead(address);
    val = XEERead();
    XEEEndRead();
    if (val == 0x55)
        return TRUE;
    else
        return FALSE;
}




BOOL TestMRF24J40() {
    BYTE value;

    value = PHYReadReg(READ_ORDER);
    if (value != 0xFF)
        return FALSE;
    else {
        PHYWriteReg(WRITE_ORDER, 0x22);
        value = PHYReadReg(READ_ORDER);
        if (value != 0x22)
            return FALSE;
        else {
            PHY_RESETn = 0;
            DelayMs(2);
            PHY_RESETn = 1;
            DelayMs(2);
            value = PHYReadReg(READ_ORDER);
            if (value == 0xFF)
                return TRUE;
            else
                return FALSE;

        }
    }

}

void PHYWriteReg(BYTE address, BYTE value)
{
    volatile BYTE tmpRFIE = RFIE;

        RFIE = 0;
        PHY_CS = 0;
        SPIPut(address);
        SPIPut(value);
        PHY_CS = 1;
        RFIE = tmpRFIE;
}

BYTE PHYReadReg(BYTE address)
    {
        BYTE toReturn;
        volatile BYTE tmpRFIE = RFIE;

        RFIE = 0;
        PHY_CS = 0;
        SPIPut(address);
        toReturn = SPIGet();
        PHY_CS = 1;
        RFIE = tmpRFIE;

        return toReturn;
    }



void UARTInit(void)
{
    //UARTTX_TRIS = 0;
    //UARTRX_TRIS = 1;
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
}

/****************************************************************************
  Function:
    void UARTTxBuffer(char *buffer, UINT32 size)

  Description:
    This routine sends data out the Sensor UART port.

  Precondition:
    None

  Parameters:
    None - None

  Returns:
    None

  Remarks:
    None
 ***************************************************************************/
void UARTTxBuffer(char *buffer, UINT32 size) {
    while (size) {
        while (!UARTTransmitterIsReady(TX_UART))
            ;

        UARTSendDataByte(TX_UART, *buffer);

        buffer++;
        size--;
    }

    while (!UARTTransmissionHasCompleted(TX_UART))
        ;
}

void SelfTestMode() {
    char value = 0;
    BOOL result = FALSE;
    WORD testValue = 0;
    BYTE bValue;
    char* buf[32];
    BYTE i;



    //Test LCD
    ConfigureLCD_SPI();
    LCDInit();
    LCDBacklightON();
    LCDErase();
    sprintf((char *) LCDText, (char*) " Self Test Mode ");
    sprintf((char *) &(LCDText[16]), (char*) "Wireless EvalBrd");
    LCDUpdate();
    DelayMs(1000);



    //Test LEDs
    LED0_ON();
    LCDErase();
    sprintf((char *) LCDText, (char*) "LED0 On?");
    sprintf((char *) &(LCDText[16]), (char*) "SW0:Yes");
    LCDUpdate();

    while (1) {
        if (BUTTON1_IO == 0u) {
            while (BUTTON1_IO == 0u);
            break;
        }
    }
    LED0_OFF();
    //LED0 Verified

    //Continue with LED1
    LED1_ON();
    LCDErase();
    sprintf((char *) LCDText, (char*) "LED1 On?");
    sprintf((char *) &(LCDText[16]), (char*) "SW1:Yes");
    LCDUpdate();

    while (1) {
        if (BUTTON0_IO == 0u) {
            while (BUTTON0_IO == 0u);
            break;
        }
    }
    LED1_OFF();
    //LED1 Verified

    //Continue with LED2
    LED2_ON();
    LCDErase();
    sprintf((char *) LCDText, (char*) "LED2 On?");
    sprintf((char *) &(LCDText[16]), (char*) "SW2:Yes");
    LCDUpdate();

    while (1) {
        if (BUTTON2_IO == 0u) {
            while (BUTTON2_IO == 0u);
            break;
        }
    }
    LED2_OFF();
    //LEDs Verified
    //Switches/PushButtons Verified

    //Test Ananlog POT
    //Turn analog pot clock wise and check if the reading is accurate
    LCDErase();
    sprintf((char *) LCDText, (char*) "Test Analog POT");
    sprintf((char *) &(LCDText[16]), (char*) "SW0:Yes SW1:Exit");
    LCDUpdate();

    while (1) {
        if (BUTTON1_IO == 0u) {
            while (BUTTON1_IO == 0u);
            while (1) {
                testValue = ReadAnalogPOT();
                //Display value on the LCD
                LCDErase();
                sprintf((char *) LCDText, (char*) "POT : %d", testValue);
                sprintf((char *) &(LCDText[16]), (char*) "Press SW1 2 Exit");
                LCDUpdate();
                if (BUTTON0_IO == 0u) {
                    while (BUTTON0_IO == 0u);
                    break;
                }
            }
            break;
        }
    }

    //Test Temperature Sensor
    LCDErase();
    sprintf((char *) LCDText, (char*) "Test Temp Sensor");
    sprintf((char *) &(LCDText[16]), (char*) "SW0:Yes SW1:Exit");
    LCDUpdate();

    while (1) {
        if (BUTTON1_IO == 0u) {
            while (BUTTON1_IO == 0u);
            sprintf((char *) LCDText, (char*) "Temp:          "); //reset the lcd display 1st line
            LCDUpdate();
            while (1) {
                bValue = ReadTempSensor();
                i = ((bValue & 0x80) >> 7);
                bValue = (bValue & 0x7F);
                //Display value on the LCD
                //LCDErase();
                if (i == 0)
                    sprintf((char *) LCDText, (char*) "Temp : %d (C)", bValue);
                else
                    sprintf((char *) LCDText, (char*) "Temp : - %d (C)", bValue);
                //sprintf((char *)&(LCDText[16]), (char*)"Press SW1 2 Exit");
                LCDUpdate();
                for (i = 0; i < 255; i++) {
                    DelayMs(1);
                    if (BUTTON0_IO == 0u)
                        break;
                }
                if (BUTTON0_IO == 0u) {
                    while (BUTTON0_IO == 0u);
                    break;
                }
            }
            break;
        }

    }




    //Test EEPROM
    LCDErase();
    sprintf((char *) LCDText, (char*) "Validating EEPROM");
    LCDUpdate();

    result = TestEEPROM();

    if (result == TRUE) {
        LCDErase();
        sprintf((char *) LCDText, (char*) "EEPROM Test:PassPress SW0");
        LCDUpdate();
    } else {
        LCDErase();
        sprintf((char *) LCDText, (char*) "EEPROM Test Fail");
        LCDUpdate();
    }
    while (1) {
        if (BUTTON1_IO == 0) {
            while (BUTTON1_IO == 0);
            break;
        }
    }

    //Test Flash

    LCDErase();
    sprintf((char *) LCDText, (char*) "Validating Flash");
    LCDUpdate();

    result = TestFlash();

    if (result == TRUE) {
        LCDErase();
        sprintf((char *) LCDText, (char*) "Flash Test:Pass Press SW0");
        LCDUpdate();
    } else {
        LCDErase();
        sprintf((char *) LCDText, (char*) "Flash Test Fail");
        LCDUpdate();
    }
    while (1) {
        if (BUTTON1_IO == 0) {
            while (BUTTON1_IO == 0);
            break;
        }
    }

    //Test MRF24J40MA
#if defined(MRF24J40)
    LCDErase();
    sprintf((char *) LCDText, (char*) "Validating MiWi Radio");
    LCDUpdate();

    result = TestMRF24J40();

    if (result == TRUE) {
        LCDErase();
        sprintf((char *) LCDText, (char*) "Radio Test:Pass Press SW0");
        LCDUpdate();
    } else {
        LCDErase();
        sprintf((char *) LCDText, (char*) "MRF24J40 Test Fail");
        LCDUpdate();
    }
    while (1) {
        if (BUTTON1_IO == 0) {
            while (BUTTON1_IO == 0);
            break;
        }
    }
#endif

    LCDErase();
    sprintf((char *) LCDText, (char*) "Testing UART");
    LCDUpdate();

    // Configure Sensor Serial Port
    UARTConfigure(TX_UART, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(TX_UART, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(TX_UART, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(TX_UART, GetPeripheralClock(), 9600);
    UARTEnable(TX_UART, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    TickInit();
#if defined(STACK_USE_MPFS2)
    MPFSInit();
#endif

    // Initialize Stack and application related NV variables into AppConfig.
    //InitAppConfig();



    // Initialize core stack layers (MAC, ARP, TCP, UDP) and
    // application modules (HTTP, SNMP, etc.)
    StackInit();
    UARTInit();
    // Verify MRF24W MAC Address
    if (AppConfig.MyMACAddr.v[0] == 0x00 && AppConfig.MyMACAddr.v[1] == 0x1E) {
        DelayMs(300);
        LCDErase();
        sprintf((char *) LCDText, (char*) "Printing Label");
        LCDUpdate();

        //********************************************************************
        // Prints a label using ESC/P commands to a Brother PT-9800PCN printer
        //********************************************************************
        // Send ESC/P Commands to setup printer
        ////UARTTxBuffer("\033ia\000\033@\033X\002", 9); // ESC i a 0 = Put Printer in ESC/P Mode
        // ESC @ = Reset Printer to Default settings
        // ESC X 2 = Specify Character Size
        // Send the Info to Print for the MAC Address label
        UARTTxBuffer("MRF24WG0MA\r", 11);
        sprintf((char *) buf, "MAC: %02X%02X%02X%02X%02X%02X", AppConfig.MyMACAddr.v[0], AppConfig.MyMACAddr.v[1], AppConfig.MyMACAddr.v[2], AppConfig.MyMACAddr.v[3], AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
        UARTTxBuffer((char *) buf, strlen((const char *) buf));

        // Print the label
        ////UARTTxBuffer("\f", 1);



        // Toggle LED's
        while (1) {
            LED0 = value;
            LED1 = value >> 1;
            LED2 = value >> 2;

            DelayMs(400);

            if (value == 8)
                value = 0;
            else
                value++;

        }
    } else // MRF24WG0MA Failure
    {
        while (1) {
            LEDS_ON();
            DelayMs(700);
            LEDS_OFF();
            DelayMs(700);
        }
    }

}
