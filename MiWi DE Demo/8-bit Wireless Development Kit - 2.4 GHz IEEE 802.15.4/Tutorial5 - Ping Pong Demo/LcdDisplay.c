/*********************************************************************
 *
 *       LCD Access Routines
 *
 *********************************************************************
 * FileName:        LcdDisplay.c
 * Dependencies:    Compiler.h, HardwareProfile.h
 * Processor:       PIC18, PIC24, PIC32, dsPIC30F, dsPIC33F
 * Compiler:        Microchip C18 v3.02 or higher
 *					Microchip C30 v2.01 or higher
 *                  Microchip C32 v1.02 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2002-2010 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product (“Device”) which is 
 *      integrated into Licensee’s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT 
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
 * Author               Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Sushma Myneni      4/03/06		Original
 *********************************************************************/
#define __LCDDISPLAY_C

#include <string.h>
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "LcdDisplay.h"
#include "TimeDelay.h"
#include "WirelessProtocols/Console.h"
#include "Transceivers/Transceivers.h"

#if defined(SENSOR_PORT_UART)
    #define LCD_SLAVEADDRESS            0x3E
    #define LCD_WRITE_CMD               0x32
    #define LCD_INIT_CMD                0x81
    #define LCDCMD_FIRSTLINE_ADDRESS    0x00

BYTE    LCDCommand[4];
BYTE    CommandLen = 0;
BYTE    TextLen = 0;
BYTE    CommandPtr = 0;
BYTE    TextPtr = 0;
#else
    #include "WirelessProtocols/MSPI.h"
#endif

// LCDText is a 32 byte shadow of the LCD text.  Write to it and
// then call LCDUpdate() to copy the string into the LCD module.
BYTE    LCDText[16 * 2 + 1];

#if defined(SENSOR_PORT_UART)

/* */
void I2CTask(void)
{
    if(CommandPtr < CommandLen || TextPtr < TextLen)
    {
        while(SSP2CON2bits.SEN == 1);
        if(CommandPtr < CommandLen)
        {
            SSP2BUF = LCDCommand[CommandPtr];

            //if (SSP2CON2bits.ACKSTAT == 0)
            {
                CommandPtr++;
            }

            if(CommandPtr == CommandLen && TextLen == 0)
            {

                //DelayMs(300);
                SSP2CON2bits.PEN = 1;
                DelayMs(300);
            }
        }
        else if(TextPtr < TextLen)
        {
            if(LCDText[TextPtr] == 0)
            {
                LCDText[TextPtr] = ' ';
            }

            SSP2BUF = LCDText[TextPtr];

            //if (SSP2CON2bits.ACKSTAT == 0)
            {
                TextPtr++;
            }

            if(TextPtr == TextLen)
            {

                //DelayMs(300);
                SSP2CON2bits.PEN = 1;
                DelayMs(300);
            }
        }
    }
}

/* */
void I2CWrite(BYTE Commandlen, BYTE BufferLen)
{
    while(SSP2CON2bits.PEN == 1);
    CommandPtr = 0;
    TextPtr = 0;
    CommandLen = Commandlen;
    TextLen = BufferLen;
    SSP2CON2bits.SEN = 1;
}

#endif

/* */
void LCDInit(void)
{
    #if defined(SENSOR_PORT_UART)
    TRISD |= 0x53;
            //PORTEbits.RDPU = 1;

    PIR3bits.SSP2IF = 0;    //Clear SSP IF
    SSP2STAT = 0x00;        //Enable Slewrate Control, Clearing flags

    //SSP2ADD = 0x19;					//Fclk = 100kHz
    SSP2ADD = 0xF0;
    SSP2CON1 = 0x28;        //Enable Master Mode
    SSP2CON2 = 0x00;
    PIE3bits.SSP2IE = 1;

    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    DelayMs(300);

    LCDCommand[0] = LCD_SLAVEADDRESS << 1;
    LCDCommand[1] = LCD_INIT_CMD;
    LCDCommand[2] = 0x00;

    I2CWrite(3, 0);

    DelayMs(200);
        #if defined(ENABLE_MANUALBACKLIGHT)
    LCDCommand[0] = LCD_SLAVEADDRESS << 1;
    LCDCommand[1] = 0x51;
    LCDCommand[2] = 0x08;

    I2CWrite(3, 0);
    DelayMs(200);
        #endif
    #else
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;

    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD2 = 1;
    TRISDbits.TRISD4 = 0;
    TRISCbits.TRISC6 = 0;

    SPISSPSTAT = 0x80;      //0xC0;	
    SPISSPCON = 0x22;       //0x21;
    SPInCS_LAT = 0;
    #endif
}

/* */
void LCDErase(void)
{
    BYTE    i;

    for(i = 0; i < 32; i++)
    {
        LCDText[i] = ' ';
    }
}

/* */
void LCDUpdate(void)
{
    #if defined(SENSOR_PORT_UART)
    LCDCommand[0] = LCD_SLAVEADDRESS << 1;
    LCDCommand[1] = LCD_WRITE_CMD;
    LCDCommand[2] = LCDCMD_FIRSTLINE_ADDRESS;
    LCDCommand[3] = 32;
    I2CWrite(4, 32);
    #else

    BYTE    i;

    DelayMs(10);
    SPIPut2(0x32);  //LCD Subsystem Write Data Command
    DelayMs(5);
    SPIPut2(0);     //Screen Address
    DelayMs(5);
    SPIPut2(32);    //Data length (16 + 3)
    for(i = 0; i < 32; i++)
    {
        DelayMs(5);
        if(LCDText[i] == 0)
        {
            SPIPut2(' ');
        }
        else
        {
            SPIPut2(LCDText[i]);
        }
    }

    DelayMs(300);
    #endif
}

#if defined(ENABLE_MANUALBACKLIGHT)

/* */
void LCDBacklightON(void)
{
        #if defined(SENSOR_PORT_UART)
    LCDCommand[0] = LCD_SLAVEADDRESS << 1;
    LCDCommand[1] = 0x44;
    LCDCommand[2] = 0x00;

    I2CWrite(3, 0);
    DelayMs(200);

    LCDCommand[0] = LCD_SLAVEADDRESS << 1;
    LCDCommand[1] = 0x42;
    LCDCommand[2] = 0x00;

    I2CWrite(3, 0);
    DelayMs(200);
        #else
        #endif
}

/* */
void LCDBacklightOFF(void)
{
        #if defined(SENSOR_PORT_UART)
    LCDCommand[0] = LCD_SLAVEADDRESS << 1;
    LCDCommand[1] = 0x44;
    LCDCommand[2] = 0x00;

    I2CWrite(3, 0);
    DelayMs(200);
        #else
        #endif
}

#endif
