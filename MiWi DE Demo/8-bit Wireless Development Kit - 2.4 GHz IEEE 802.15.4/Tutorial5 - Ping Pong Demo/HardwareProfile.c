/*****************************************************************************
 *
 *              HardwareProfile.c -- Hardware Profile
 *
 *****************************************************************************
 * FileName:        HardwareProfile.c
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
 *  0.1   2/17/2009    Initial revision
 *  3.1   5/28/2010    MiWi DE 3.1
 *****************************************************************************/
#include "SystemProfile.h"
#include "Compiler.h"
#include "WirelessProtocols/Console.h"
#include "LcdDisplay.h"
#include "TimeDelay.h"
#include "HardwareProfile.h"

#if defined(__18F4620)
    #pragma romdata CONFIG1H = 0x300001
const rom unsigned char config1H = 0 b00000110;     // HSPLL oscillator
    #pragma romdata CONFIG2L = 0x300002
const rom unsigned char config2L = 0 b00011111;     // Brown-out Reset Enabled in hardware @ 2.0V, PWRTEN disabled
    #pragma romdata CONFIG2H = 0x300003
const rom unsigned char config2H = 0 b00010000;     // HW WD disabled, 1:32 prescaler
    #pragma romdata CONFIG3H = 0x300005
const rom unsigned char config3H = 0 b10000000;     // PORTB digital on RESET
    #pragma romdata CONFIG4L = 0x300006
const rom unsigned char config4L = 0 b11000001;     // DEBUG disabled,

// XINST enabled
// LVP disabled
// STVREN enabled
#elif defined(__18F2520)
    #pragma romdata CONFIG1H = 0x300001
const rom unsigned char config1H = 0 b00001000;     // use internal oscillator
    #pragma romdata CONFIG2L = 0x300002
const rom unsigned char config2L = 0 b00011111;     // Brown-out Reset Enabled in hardware @ 2.0V, PWRTEN disabled
    #pragma romdata CONFIG2H = 0x300003
const rom unsigned char config2H = 0 b00010110;     // HW WD disabled, 1:32 prescaler
    #pragma romdata CONFIG3H = 0x300005
const rom unsigned char config3H = 0 b00000011;     // PORTB digital as PORT
    #pragma romdata CONFIG4L = 0x300006
const rom unsigned char config4L = 0 b10000001;     // DEBUG disabled,

// XINST disabled
// LVP disabled
// STVREN enabled
#elif defined(__18F87J11)
    #pragma config FOSC = HSPLL, WDTEN = OFF, WDTPS = 128, XINST = ON
#elif defined(__18F46J50)
    #pragma config OSC = INTOSC, WDTEN = OFF, XINST = ON, WDTPS = 2048, PLLDIV = 2, CPUDIV = OSC2_PLL2
#elif defined(__PIC24F__)

// Explorer 16 board
_CONFIG2(FNOSC_PRI & POSCMOD_XT)                    // Primary XT OSC with 4x PLL
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF & WDTPS_PS1024)    // JTAG off, watchdog timer off
#elif defined(__dsPIC33F__) || defined(__PIC24H__)

// Explorer 16 board
_FOSCSEL(FNOSC_PRI);                                //primary osc
_FOSC(OSCIOFNC_OFF & POSCMD_XT)                     // XT Osc
_FWDT(FWDTEN_OFF & WDTPOST_PS2)                     // Disable Watchdog timer

// JTAG should be disabled as well
#elif defined(__dsPIC30F__)

// dsPICDEM 1.1 board
_FOSC(XT_PLL16)                 // XT Osc + 16X PLL
_FWDT(WDT_OFF & WDTPOST_PS2)    // Disable Watchdog timer
_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF)
#elif defined(__PIC32MX__)
    #define PIC32MX_SPI1_SDO_SCK_MASK_VALUE (0x00000140)
    #define PIC32MX_SPI1_SDI_MASK_VALUE     (0x00000080)

//#define PIC32MX_INT2_MASK_VALUE              (0x00000010)
//#define PIC32MX_INT1_MASK_VALUE              (0x00000008)
    #define PIC32MX_INT1_MASK_VALUE (0x00000100)

/* MAX SPI CLOCK FREQ SUPPORTED FOR MIWI TRANSCIEVER */
    #define MAX_SPI_CLK_FREQ_FOR_P2P    (1000000)
    #pragma config FPLLMUL = MUL_16, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
    #pragma config POSCMOD = XT, FNOSC = PRIPLL, FPBDIV = DIV_4, WDTPS = PS1024
#endif
#define DEBOUNCE_TIME   0x00003FFF

BOOL PUSH_BUTTON_pressed;
MIWI_TICK   PUSH_BUTTON_press_time;

/*********************************************************************
 * Function:        void BoardInit( void )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Board is initialized for P2P usage
 *
 * Overview:        This function configures the board 
 *
 * Note:            This routine needs to be called before the function 
 *                  to initialize P2P stack or any other function that
 *                  operates on the stack
 ********************************************************************/
void BoardInit(void)
{

    // primary internal oscillator
    OSCCON = 0x7B;
    WDTCONbits.SWDTEN = 0;
    
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = 0;

        #if defined(MRF49XA)
    RPINR3 = 6;
        #endif
        #if defined(MRF24J40)
    RPINR1 = 4;
        #endif
        #if defined(MRF89XA)
    RPINR3 = 6;
        #endif
        #if !defined(SENSOR_PORT_UART)

    // use LCD
    RPINR21 = 19;           //Mapping SDI2 to RD2
    RPOR23 = 9;             //Mapping SDO2 to RD6
    RPOR21 = 10;            //Mapping SCK2 to RD4
        #endif
        #if defined(SENSOR_PORT_UART)

    // use UART
    RPINR16 = 19;
    RPOR17 = 5;
        #endif
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = 1;

    ANCON0 = 0xFF;
    ANCON1 = 0x3F;
    INTCON2bits.RBPU = 0;

    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;

    TRISD = 0xFF;
    TRISE = 0xFF;

    LATA = 0;
    LATB = 0;

    //LATC = 0;
    LATD = 0;
    LATE = 0;

    TRISDbits.TRISD2 = 0;
    LATDbits.LATD2 = 1;
    TRISCbits.TRISC6 = 0;
    LATCbits.LATC6 = 0;

    // set I/O ports
    BUTTON_RB0_TRIS = 1;
    BUTTON_RB2_TRIS = 1;
    LED_1_TRIS = 0;
    LED_2_TRIS = 0;

            #if defined(MRF24J40) || defined(MRF49XA)
    PHY_CS_TRIS = 0;
    PHY_CS = 1;
    PHY_RESETn_TRIS = 0;
    PHY_RESETn = 1;
    RF_INT_TRIS = 1;
    #endif
    #if defined(MRF89XA)
        PHY_RESETn_TRIS = 1;
        IRQ1_INT_TRIS = 1;
        IRQ0_INT_TRIS = 1;
    
        #if defined(USE_IRQ0_AS_INTERRUPT)
            //Enable the effective INT edge
            PHY_IRQ0_En = 1;
            PHY_IRQ0 = 0;
         #endif
         
    #endif
    SDI_TRIS = 1;
    SDO_TRIS = 0;
    SCK_TRIS = 0;

    #if defined(HARDWARE_SPI)
        SSP1STAT = 0xC0;
        SSP1CON1 = 0x21;
    #else
        SPI_SDO = 0;
        SPI_SCK = 0;
    #endif
    #if defined(MRF49XA)
        nFSEL_TRIS = 0;
        FINT_TRIS = 1;
        nFSEL = 1;              // nFSEL inactive
        INTCON2bits.INTEDG3 = 0;
    #endif
    #if defined(MRF24J40)
        PHY_WAKE_TRIS = 0;
        PHY_WAKE = 1;
        INTCON2bits.INTEDG1 = 0;
    #endif
    
    #if defined(MRF89XA)
        Config_nCS_TRIS = 0;    //nCS_Config as output
        Data_nCS_TRIS = 0;      //nCS_DATA as output
        Config_nCS_TRIS = 0;
        Data_nCS = 1;
        Config_nCS = 1;
        INTCON2bits.INTEDG3 = 1;
    #endif
        RF_EEnCS_TRIS = 0;
        RF_EEnCS = 1;
    #if defined(ENABLE_NVM)
        EE_nCS_TRIS = 0;
        EE_nCS = 1;
    #endif
    INTCONbits.GIEH = 1;

    #if defined(MRF24J40) || defined(MRF49XA)
        RFIF = 0;
        RFIE = 1;
    #endif
    
    #if defined(SENSOR_PORT_LCD)
        LCDInit();
    #endif
    ConsoleInit();                  //Initializes the Console
}

/*********************************************************************
 * Function:        void LCDDisplay(char *text, BYTE value, BOOL delay)
 *
 * PreCondition:    LCD has been initialized
 *
 * Input:           text - text message to be displayed on LCD
 *                  value - the text message allows up to one byte 
 *                          of variable in the message
 *                  delay - whether need to display the message for
 *                          2 second without change
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function display the text message on the LCD, 
 *                  including up to one BYTE variable, if applicable.
 *
 * Note:            This routine is only effective if Explorer16 or
 *                  PIC18 Explorer demo boards are used
 ********************************************************************/
void LCDDisplay(char *text, BYTE value, BOOL delay)
{
        #if defined(EXPLORER16) || defined(PIC18_EXPLORER) || \
            (defined(EIGHT_BIT_WIRELESS_BOARD) && defined(SENSOR_PORT_LCD))
    LCDErase();

            #if defined(PIC18_EXPLORER) || defined(EIGHT_BIT_WIRELESS_BOARD)
    sprintf((char *)LCDText, (far rom char *) text, value);
            #elif defined(EXPLORER16)
    sprintf((char *)LCDText, (const char *)text, value);
            #endif
    LCDUpdate();

    // display the message for 2 seconds
    if(delay)
    {
        BYTE    i;
        for(i = 0; i < 8; i++)
        {
            DelayMs(250);
        }
    }

        #endif
}



    #if defined(EIGHT_BIT_WIRELESS_BOARD) && defined(SENSOR_PORT_UART) && defined(SENSOR_PORT_LCD)
void    I2CTask(void);
    #endif
    #if defined(__18CXX)

/* */
void UserInterruptHandler(void)
{
            #if defined(EIGHT_BIT_WIRELESS_BOARD) && defined(SENSOR_PORT_UART) && defined(SENSOR_PORT_LCD)
    if(PIR3bits.SSP2IF)
    {
        PIR3bits.SSP2IF = 0;
        I2CTask();
        DelayMs(5);
    }

            #endif
}

    #endif
