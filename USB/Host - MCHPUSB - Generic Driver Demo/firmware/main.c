/******************************************************************************
            USB Custom Demo, Host

This file provides the main entry point to the Microchip USB Custom
Host demo.  This demo shows how a PIC24F system could be used to
act as the host, controlling a USB device running the Microchip Custom
Device demo.

******************************************************************************/

/******************************************************************************
* Filename:        main.c
* Dependancies:    USB Host Driver with Generic Client Driver
* Processor:       PIC24F256GB1xx
* Hardware:        Explorer 16 with USB PICtail Plus
* Compiler:        C30 v2.01/C32 v0.00.18
* Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the “Company”) for its PICmicro® Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.


*******************************************************************************/

#include <stdlib.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_generic.h"
#include "user.h"
#include "LCDBlocking.h"
#include "timer.h"

// *****************************************************************************
// *****************************************************************************
// Configuration Bits
// *****************************************************************************
// *****************************************************************************

#if defined __C30__ || defined __XC16__
    #if defined(__PIC24FJ256GB110__)
        _CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & PLL_96MHZ_ON & PLLDIV_DIV2 & IESO_OFF) // Primary HS OSC with PLL, USBPLL /2
        _CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)   // JTAG off, watchdog timer off
    #elif defined(__PIC24FJ64GB004__)
        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
        _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
    #elif defined(__PIC24FJ256GB106__)
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
    #elif defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
        _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
    #elif defined(__dsPIC33EP512MU810__)||defined(__PIC24EP512GU810__)
        _FOSCSEL(FNOSC_FRC);
        _FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
        _FWDT(FWDTEN_OFF);    
    #endif

#elif defined( __PIC32MX__ )

    #pragma config UPLLEN   = ON            // USB PLL Enabled
    #pragma config FPLLMUL  = MUL_15        // PLL Multiplier
    #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
    #pragma config FPLLODIV = DIV_1         // PLL Output Divider
    #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
    #pragma config FWDTEN   = OFF           // Watchdog Timer
    #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
    #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
    #pragma config OSCIOFNC = OFF           // CLKO Enable
    #pragma config POSCMOD  = HS            // Primary Oscillator
    #pragma config IESO     = OFF           // Internal/External Switch-over
    #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
    #pragma config FNOSC    = PRIPLL        // Oscillator Selection
    #pragma config CP       = OFF           // Code Protect
    #pragma config BWP      = OFF           // Boot Flash Write Protect
    #pragma config PWP      = OFF           // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
    

#else

    #error Cannot define configuration bits.

#endif
// *****************************************************************************
// *****************************************************************************
// Constants
// *****************************************************************************
// *****************************************************************************

#define IOPORT_BIT_15                       (1 << 15)
#define IOPORT_BIT_14                       (1 << 14)
#define IOPORT_BIT_13                       (1 << 13)
#define IOPORT_BIT_12                       (1 << 12)
#define IOPORT_BIT_11                       (1 << 11)
#define IOPORT_BIT_10                       (1 << 10)
#define IOPORT_BIT_9                        (1 << 9)
#define IOPORT_BIT_8                        (1 << 8)
#define IOPORT_BIT_7                        (1 << 7)
#define IOPORT_BIT_6                        (1 << 6)
#define IOPORT_BIT_5                        (1 << 5)
#define IOPORT_BIT_4                        (1 << 4)
#define IOPORT_BIT_3                        (1 << 3)
#define IOPORT_BIT_2                        (1 << 2)
#define IOPORT_BIT_1                        (1 << 1)
#define IOPORT_BIT_0                        (1 << 0)


// *****************************************************************************
// *****************************************************************************
// Data Structures
// *****************************************************************************
// *****************************************************************************

// Switch & LED States
typedef union
{
    BYTE    byte;
    struct
    {
        BYTE    b0 : 1;
        BYTE    b1 : 1;
        BYTE    b2 : 1;
        BYTE    b3 : 1;
        BYTE    b4 : 1;
        BYTE    b5 : 1;
        BYTE    b6 : 1;
        BYTE    b7 : 1;
    }bits;
}BIT_STATE;


// Application States
typedef enum
{
    DEMO_INITIALIZE = 0,                // Initialize the app when a device is attached
    DEMO_STATE_IDLE,                    // Inactive State


    DEMO_STATE_GET_DEV_VERSION,         // New device attached, send Get-Dev-FW-Version command
    DEMO_STATE_WAITING_VER_REQ,         // Waiting for Get Version command to complete
    DEMO_STATE_READ_DEV_VERSION,        // Start reading Dev FW version data
    DEMO_STATE_WAITING_READ_VER,        // Waiting for read of version data to complete
    DEMO_STATE_VERIFY_DEV_FW_VER,       // Dev FW Ver received, verify & display it

    DEMO_STATE_GET_TEMPERATURE,         // Send get-temperature command
    DEMO_STATE_WAITING_GET_TEMP,        // Waiting for get-temperature command to complete
    DEMO_STATE_READ_TEMPERATURE,        // Start reading temperature data
    DEMO_STATE_WAITING_READ_TEMP,       // Waiting for read of temperature data to complete
    DEMO_STATE_DISPLAY_TEMPERATURE,     // Temperature data received, display it

    DEMO_STATE_GET_POT,                 // Send get-POT command
    DEMO_STATE_WAITING_GET_POT,         // Waiting for get-POT command to complete
    DEMO_STATE_READ_POT,                // Start reading POT data
    DEMO_STATE_WAITING_READ_POT,        // Waiting for read of POT data to complete
    DEMO_STATE_DISPLAY_POT,             // POT data received, display it

    DEMO_STATE_SEND_SET_LED,            // Send set-LED command
    DEMO_STATE_WAITING_SET_LED,         // Waiting for set-LED command to complete
    DEMO_STATE_READ_SET_LED_RESP,       // Start reading set-LED response
    DEMO_STATE_WAITING_LED_RESP,        // Waiting for read of LED response to complete
    DEMO_STATE_UPDATE_LED_STATE,        // Set-LED command successful, update state.

    DEMO_STATE_ERROR                    // An error has occured

} DEMO_STATE;


// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************

// Macro to fill a line of LCD data.
#define FILL_LCD_LINE(l,p) {int i; for(i=0;i<16;i++)LcdData[(l)-1][i]=(p)[i];}


// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

BYTE        deviceAddress;  // Address of the device on the USB
DATA_PACKET DataPacket;     // Data to send to the device
DEMO_STATE  DemoState;      // Current state of the demo application
BYTE        LcdData[2][16]; // LCD Display data
BIT_STATE   LEDState;       // Bitmap of LED states (only 3 & 4 are used)
WORD        SwitchState;    // Bitmap of Switch states (only 3 & 6 are used)

// Initial data for the LCD display.
const char *LcdInitData[] = { "Host -.- Dev -.-", "--.-C ----- Ohms" };


//******************************************************************************
//******************************************************************************
// Local Routines
//******************************************************************************
//******************************************************************************

/*************************************************************************
 * Function:        InitializeSystem
 *
 * Preconditions:   None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         TRUE if successful, FALSE if not.
 *
 * Side Effects:    See below
 *
 * Overview:        This routine initializes the processor and peripheral,
 *                  setting clock speeds and enabling any required
 *                  features.
 *************************************************************************/

BOOL InitializeSystem ( void )
{
    #if defined(__dsPIC33EP512MU810__)||defined(__PIC24EP512GU810__)

    // Configure the device PLL to obtain 60 MIPS operation. The crystal
    // frequency is 8MHz. Divide 8MHz by 2, multiply by 60 and divide by
    // 2. This results in Fosc of 120MHz. The CPU clock frequency is
    // Fcy = Fosc/2 = 60MHz. Wait for the Primary PLL to lock and then
    // configure the auxilliary PLL to provide 48MHz needed for USB 
    // Operation.

    PLLFBD = 38;				/* M  = 60	*/
    CLKDIVbits.PLLPOST = 0;		/* N1 = 2	*/
    CLKDIVbits.PLLPRE = 0;		/* N2 = 2	*/
    OSCTUN = 0;			

    /*	Initiate Clock Switch to Primary
     *	Oscillator with PLL (NOSC= 0x3)*/

    __builtin_write_OSCCONH(0x03);		
    __builtin_write_OSCCONL(0x01);

    while (OSCCONbits.COSC != 0x3);       

        // Configuring the auxiliary PLL, since the primary
        // oscillator provides the source clock to the auxiliary
        // PLL, the auxiliary oscillator is disabled. Note that
        // the AUX PLL is enabled. The input 8MHz clock is divided
        // by 2, multiplied by 24 and then divided by 2. Wait till 
        // the AUX PLL locks.

        ACLKCON3 = 0x24C1;   
        ACLKDIV3 = 0x7;

        ACLKCON3bits.ENAPLL = 1;
        while(ACLKCON3bits.APLLCK != 1); 

        ANSELA = 0x0000;
        ANSELB = 0x0000;
        ANSELC = 0x0000;
        ANSELD = 0x0000;
        ANSELE = 0x0000;
        ANSELG = 0x0000;

        // The dsPIC33EP512MU810 features Peripheral Pin
        // select. The following statements map UART2 to 
        // device pins which would connect to the the 
        // RX232 transciever on the Explorer 16 board.

        RPINR19 = 0;
        RPINR19 = 0x64;
        RPOR9bits.RP101R = 0x3;

#endif
    
    #if defined( __PIC24FJ256GB110__ )
        // Configure U2RX - put on pin 49 (RP10)
        RPINR19bits.U2RXR = 10;

        // Configure U2TX - put on pin 50 (RP17)
        RPOR8bits.RP17R = 5;

        OSCCON = 0x3302;    // Enable secondary oscillator
        CLKDIV = 0x0000;    // Set PLL prescaler (1:1)

        TRISA = 0x0000;
        TRISD = 0x00C0;

   #elif defined(__PIC24FJ64GB004__)
	//On the PIC24FJ64GB004 Family of USB microcontrollers, the PLL will not power up and be enabled
	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
	//This allows the device to power up at a lower initial operating frequency, which can be
	//advantageous when powered from a source which is not gauranteed to be adequate for 32MHz
	//operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
	//power up the PLL.
    {
        unsigned int pll_startup_counter = 600;
        CLKDIVbits.PLLEN = 1;
        while(pll_startup_counter--);
    }

    #elif defined(__PIC32MX__)
        {
            int  value;
    
            value = SYSTEMConfigWaitStatesAndPB( GetSystemClock() );
    
            // Enable the cache for the best performance
            CheKseg0CacheOn();
    
            INTEnableSystemMultiVectoredInt();
    
            value = OSCCON;
            while (!(value & 0x00000020))
            {
                value = OSCCON;    // Wait for PLL lock to stabilize
            }
        }
    #endif

    // Init LCD
    LCDInit();

    // Init LEDs
    mInitAllLEDs();
    mLED_9_Off();
    mLED_10_Off();

    // Init Switches
    //mInitAllSwitches();
    SwitchState = IOPORT_BIT_6|IOPORT_BIT_7;

    // Init UART
    UART2Init();

    // Set Default demo state
    DemoState = DEMO_INITIALIZE;

    return TRUE;
} // InitializeSystem


/*************************************************************************
 * Function:        CheckForNewAttach
 *
 * Preconditions:   None
 *
 * Input:           None
 *
 * Output:          deviceAddress (global)
 *                  Updates the device address when an attach is found.
 *
 * Returns:         TRUE if a new device has been attached.  FALSE,
 *                  otherwise.
 *
 * Side Effects:    Prints attach message
 *
 * Overview:        This routine checks to see if a new device has been
 *                  attached.  If it has, it records the address.
 *************************************************************************/

BOOL CheckForNewAttach ( void )
{
    // Try to get the device address, if we don't have one.
    if (deviceAddress == 0)
    {
        GENERIC_DEVICE_ID DevID;

        DevID.vid   = 0x04D8;
        DevID.pid   = 0x000C;
        #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
            DevID.serialNumberLength = 0;
            DevID.serialNumber = NULL;
        #endif

        if (USBHostGenericGetDeviceAddress(&DevID))
        {
            deviceAddress = DevID.deviceAddress;
            UART2PrintString( "Generic demo device attached - polled, deviceAddress=" );
            UART2PutDec( deviceAddress );
            UART2PrintString( "\r\n" );
            return TRUE;
        }
    }

    return FALSE;

} // CheckForNewAttach


/*************************************************************************
 * Function:        InitLcdMessage
 *
 * Preconditions:   Assumes that INTEnableSystemMultiVectoredInt and
 *                  Ex16LCDInit have been called.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         None
 *
 * Side Effects:    The LCD display and message strings have been modified
 *
 * Overview:        Displays the App's base LCD message
 *************************************************************************/

void InitLcdMessage ( void )
{
    // Init LCD message
    FILL_LCD_LINE(1, &LcdInitData[0][0]);
    FILL_LCD_LINE(2, &LcdInitData[1][0]);
    LcdData[0][5] = '0' + USB_HOST_FW_MAJOR_VER;
    LcdData[0][7] = '0' + USB_HOST_FW_MINOR_VER;

    // display message
    LCDWriteLine(1, (char *)&LcdData[0][0] );
    LCDWriteLine(2, (char *)&LcdData[1][0] );

} // InitLcdMessage


/*************************************************************************
 * Function:        ValidateAndDisplayDeviceFwVersion
 *
 * Preconditions:   Assumes the device FW Version response packet has been
 *                  received.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         TRUE if the Device FW data is valid, FALSE if not.
 *
 * Side Effects:    The LCD display and message strings have been modified.
 *                  Also displays a message on the serial port.
 *
 * Overview:        Validates data in the packet buffer against expected
 *                  device firmware version and displays it on the LCD and
 *                  serial port.
 *************************************************************************/

BOOL ValidateAndDisplayDeviceFwVersion ( void )
{
    if ( USBHostGenericGetRxLength(deviceAddress) == 4             &&
         DataPacket.CMD                   == READ_VERSION          &&
         DataPacket._byte[3]              >= DEMO_FW_MAJOR_VERSION    )
    {
        // Display Device FW version on LCD
        LcdData[0][13] = '0' + DataPacket._byte[3];
        LcdData[0][15] = '0' + DataPacket._byte[4];
        LCDWriteLine(1, (char *)&LcdData[0][0] );

        // Display device FW version on terminal
        UART2PrintString( "Device firmware version " );
        UART2PutDec( DataPacket._byte[3] );
        UART2PrintString( "." );
        UART2PutDec( DataPacket._byte[4] );
        UART2PrintString( "\r\n" );

        return TRUE;
    }
    else
    {
        UART2PrintString( "Device Firmware Version Error!\r\n" );
        LCDWriteLine( 2, "Dev FW Ver Error" );
        return FALSE;
    }

} // ValidateAndDisplayDeviceFwVersion


/*************************************************************************
 * Function:        DisplayTemperature
 *
 * Preconditions:   Assumes the device FW Version response packet has been
 *                  received.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         None
 *
 * Side Effects:    The LCD display and message strings have been modified.
 *                  Also displays a message on the serial port.
 *
 * Overview:        Displays the temperature data in the packet buffer on
 *                  the LCD.
 *************************************************************************/

void DisplayTemperature ( void )
{
    DWORD Temperature;
    BYTE   Digit;

    // Calculate the temperature in degrees C
    Temperature  =  DataPacket._byte[1];
    Temperature |= (DataPacket._byte[2] << 8);
    Temperature += 6400;
    Temperature /= 41;

    // Translate to ASCII
    Digit = Temperature % 10;
    Temperature   /= 10;
    LcdData[1][3]  = '0' + Digit;
    Digit = Temperature % 10;
    Temperature   /= 10;
    LcdData[1][1]  = '0' + Digit;
    Digit = Temperature % 10;
    Temperature   /= 10;
    LcdData[1][0]  = '0' + Digit;

    // Display on LCD
    LCDWriteLine(2, (char *)&LcdData[1][0] );

} // DisplayTemperature


/*************************************************************************
 * Function:        DisplayPot
 *
 * Preconditions:   Assumes the device FW Version response packet has been
 *                  received.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         None
 *
 * Side Effects:    The LCD display and message strings have been modified.
 *                  Also displays a message on the serial port.
 *
 * Overview:        Displays the potentiometer data in the packet buffer on
 *                  the LCD.
 *************************************************************************/

void DisplayPot ( void )
{
    DWORD POT_Val;
    BYTE   Digit;
    int    i;

    // Calculate actual POT value
    POT_Val  =  DataPacket._byte[1];
    POT_Val |= (DataPacket._byte[2] << 8);
    POT_Val *= POT_FULL_VALUE+10;
    POT_Val /= 1024;

    // Translate to ASCII
    for (i=0; i < 5; i++)
    {
        Digit             = POT_Val % 10;
        POT_Val          /= 10;
        LcdData[1][10-i]  = '0' + Digit;
    }

    // Display on LCD
    LCDWriteLine(2, (char *)&LcdData[1][0] );

} // DisplayPot


/*************************************************************************
 * Function:        Switch3WasPressed & Switch6WasPressed
 *
 * Preconditions:   Assumes SwitchState & Switches have been initialized
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         TRUE if the switch has been pressed, FALSE otherwise
 *
 * Side Effects:    SwitchState has been updated
 *
 * Overview:        Determines if the switched named has been pressed
 *                  since last time the routine was called.
 *************************************************************************/

BOOL Switch3WasPressed(void)
{
    WORD sw3_state;

    sw3_state = sw3_port & sw3_bit;

    if( sw3_state != (SwitchState & sw3_bit) )
    {
        SwitchState &= ~sw3_bit;
        SwitchState |= sw3_state;

        if(SwitchState & sw3_bit)
            return TRUE;                // Was pressed
    }

    return FALSE;                       // Was not pressed

} // Switch3WasPressed

BOOL Switch6WasPressed(void)
{
    WORD sw6_state;

    sw6_state = sw6_port & sw6_bit;

    if( sw6_state != (SwitchState & sw6_bit) )
    {
        SwitchState &= ~sw6_bit;
        SwitchState |= sw6_state;

        if(SwitchState & sw6_bit)
            return TRUE;                // Was pressed
    }

    return FALSE;                       // Was not pressed

}//end Switch6WasPressed


/******************************************************************************
 * Function:        void BlinkStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkStatus turns on and off LEDs corresponding to
 *                  the demo state.
 *
 * Note:
 *****************************************************************************/
void BlinkStatus(void)
{
    static unsigned int led_count=0;

    if(led_count == 0)
        led_count = 3000;

    led_count--;

    if(DemoState <= DEMO_STATE_IDLE)
    {
        mLED_5_Off();
        mLED_6_Off();
    }
    else if(DemoState <= DEMO_STATE_VERIFY_DEV_FW_VER)
    {
        mLED_5_On();
        mLED_6_On();
    }
    else if(DemoState < DEMO_STATE_ERROR)
    {
        if (led_count == 0)
        {
            mLED_5_Toggle();
            mLED_6 = !mLED_5;       // Alternate blink
        }
    }
    else    // DEMO_STATE_ERROR
    {
        if (led_count == 0)
        {
            mLED_5_Toggle();
            mLED_6 = mLED_5;        // Both blink at the same time
            led_count = 50;
        }

        DelayMs(1); // 1ms delay
    }

}//end BlinkStatus


/*************************************************************************
 * Function:        ManageDemoState
 *
 * Preconditions:   The DemoState global variable must be initialized to
 *                  DEMO_STATE_IDLE (0).  (This occurs on reset.)
 *
 * Input:           DemoState (global)
 *                  Actions selected based value of DemoState on function
 *                  entry.
 *
 *                  deviceAddress (global)
 *                  May use device address to access device, depending on
 *                  state.
 *
 *                  DataPacket (global)
 *                  May read data from packet buffer, depending on state.
 *
 * Output:          DemoState (global)
 *                  Updates demo state as appropriate.
 *
 *                  DataPacket (global)
 *                  May cause data in the packet buffer to be updated,
 *                  depending on state.
 *
 * Returns:         None
 *
 * Side Effects:    Depend on state transition
 *
 * Overview:        This routine maintains the state of the application,
 *                  updateing global data and taking actions as necessary
 *                  to maintain the custom demo operations.
 *************************************************************************/
void ManageDemoState ( void )
{
    BYTE RetVal;

    BlinkStatus();

    // Watch for device timeouts
//    if (MSTimerGetTime() > DEMO_TIMEOUT_LIMIT)
//    {
//        if (DemoState == DEMO_STATE_IDLE)
//        {
//            LCDWriteLine( 2, "Awaiting Device" );
//        }
//        else
//        {
//            UART2PrintString( "Device Time-out Error!\r\n" );
//            LCDWriteLine( 2, "Dev Time-out Err" );
//            DemoState = DEMO_STATE_ERROR;
//        }
//    }

    // Watch for device detaching
    if (USBHostGenericDeviceDetached(deviceAddress) && deviceAddress != 0)
    {
        UART2PrintString( "Generic demo device detached - polled\r\n" );
        DemoState = DEMO_INITIALIZE;
        deviceAddress   = 0;
    }

    switch (DemoState)
    {
    case DEMO_INITIALIZE:
        InitLcdMessage();
        DemoState = DEMO_STATE_IDLE;
        break;

    /** Idle State:  Loops here until attach **/
    case DEMO_STATE_IDLE:
        if (CheckForNewAttach())
        {
            DemoState = DEMO_STATE_GET_DEV_VERSION;
        }
        break;

    /** Sequence: Read Dev FW Version **/
    case DEMO_STATE_GET_DEV_VERSION:
        // Send the Read Version command
        DataPacket.CMD = READ_VERSION;
        DataPacket.len = 2;
        if (!USBHostGenericTxIsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericWrite(deviceAddress, &DataPacket, 2)) == USB_SUCCESS )
            {
                DemoState = DEMO_STATE_WAITING_VER_REQ;
            }
            else
            {
                UART2PrintString( "1 Device Write Error 0x" );
                UART2PutHex(RetVal);
                UART2PrintString( "\r\n" );
                LCDWriteLine( 2, "Dev Write Error " );
            }
        }
        break;

    case DEMO_STATE_WAITING_VER_REQ:
        if (!USBHostGenericTxIsBusy(deviceAddress) )
            DemoState = DEMO_STATE_READ_DEV_VERSION;
        break;

    case DEMO_STATE_READ_DEV_VERSION:
        if (!USBHostGenericRxIsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericRead(deviceAddress, &DataPacket, 4)) == USB_SUCCESS )
            {
                DemoState = DEMO_STATE_WAITING_READ_VER;
            }
            else
            {
                UART2PrintString( "1 Device Read Error 0x" );
                UART2PutHex(RetVal);
                UART2PrintString( "\r\n" );
                LCDWriteLine( 2, "Dev Read Error  " );
            }
        }
        break;

    case DEMO_STATE_WAITING_READ_VER:
        if (!USBHostGenericRxIsBusy(deviceAddress))
            DemoState = DEMO_STATE_VERIFY_DEV_FW_VER;
        break;

    case DEMO_STATE_VERIFY_DEV_FW_VER:
        if (ValidateAndDisplayDeviceFwVersion())
            DemoState = DEMO_STATE_GET_TEMPERATURE;
        else
            DemoState = DEMO_STATE_ERROR;
        break;

    /** Sequence: Read Temperature Sensor Data **/
    case DEMO_STATE_GET_TEMPERATURE:
        // Send the Read Temperature command
        DataPacket.CMD = RD_TEMP;
        DataPacket.len = 2;
        if (!USBHostGenericTxIsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericWrite(deviceAddress, &DataPacket, 2)) == USB_SUCCESS)
            {
                DemoState = DEMO_STATE_WAITING_GET_TEMP;
            }
            else
            {
                UART2PrintString( "2 Device Write Error 0x" );
                UART2PutHex(RetVal);
                UART2PrintString( "\r\n" );
                LCDWriteLine( 2, "Dev Write Error " );
            }
        }
        break;

    case DEMO_STATE_WAITING_GET_TEMP:
        if (!USBHostGenericTxIsBusy(deviceAddress) )
        {
            DemoState = DEMO_STATE_READ_TEMPERATURE;
        }
        break;

    case DEMO_STATE_READ_TEMPERATURE:
        if (!USBHostGenericRxIsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericRead(deviceAddress, &DataPacket, 3)) == USB_SUCCESS)
            {
                DemoState = DEMO_STATE_WAITING_READ_TEMP;
            }
            else
            {
                UART2PrintString( "2 Device Read Error 0x" );
                UART2PutHex(RetVal);
                UART2PrintString( "\r\n" );
                LCDWriteLine( 2, "Dev Read Error  " );
            }
        }
        break;

    case DEMO_STATE_WAITING_READ_TEMP:
        if (!USBHostGenericRxIsBusy(deviceAddress))
        {
            DemoState = DEMO_STATE_DISPLAY_TEMPERATURE;
        }
        break;

    case DEMO_STATE_DISPLAY_TEMPERATURE:
        DisplayTemperature();
        DemoState = DEMO_STATE_GET_POT;
        break;

    /** Sequence: Read POT Sensor Data **/
    case DEMO_STATE_GET_POT:
        // Send the Read POT command
        DataPacket.CMD = RD_POT;
        DataPacket.len = 2;
        if (!USBHostGenericTxIsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericWrite(deviceAddress, &DataPacket, 2)) == USB_SUCCESS)
            {
                DemoState = DEMO_STATE_WAITING_GET_POT;
            }
            else
            {
                UART2PrintString( "3 Device Write Error 0x" );
                UART2PutHex(RetVal);
                UART2PrintString( "\r\n" );
                LCDWriteLine( 2, "Dev Write Error " );
            }
        }
        break;

    case DEMO_STATE_WAITING_GET_POT:
        if (!USBHostGenericTxIsBusy(deviceAddress) )
            DemoState = DEMO_STATE_READ_POT;
        break;

    case DEMO_STATE_READ_POT:
        if (!USBHostGenericRxIsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericRead(deviceAddress, &DataPacket, 3)) == USB_SUCCESS)
            {
                DemoState = DEMO_STATE_WAITING_READ_POT;
            }
            else
            {
                UART2PrintString( "3 Device Read Error 0x" );
                UART2PutHex(RetVal);
                UART2PrintString( "\r\n" );
                LCDWriteLine( 2, "Dev Read Error  " );
            }
        }
        break;

    case DEMO_STATE_WAITING_READ_POT:
        if (!USBHostGenericRxIsBusy(deviceAddress))
            DemoState = DEMO_STATE_DISPLAY_POT;
        break;

    case DEMO_STATE_DISPLAY_POT:
        DisplayPot();
        DemoState = DEMO_STATE_SEND_SET_LED;
        break;

    /** Sequence:  Update LEDs **/
    case DEMO_STATE_SEND_SET_LED:
        // Send the set-LED command
        DataPacket.CMD = UPDATE_LED;
        DataPacket.len = 3;
        if (Switch3WasPressed())
        {
            DataPacket.led_num    = 3;  // LED 3 on original PIC18 FS USB board
            DataPacket.led_status = LEDState.bits.b3 ^ 1;
        }
        else if (Switch6WasPressed())
        {
            DataPacket.led_num    = 4;  // LED 4 on original PIC18 FS USB board
            DataPacket.led_status = LEDState.bits.b4 ^ 1;
        }
        else
        {
            DemoState = DEMO_STATE_GET_TEMPERATURE;
            break;
        }
        if (!USBHostGenericTxIsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericWrite(deviceAddress, &DataPacket, 3)) == USB_SUCCESS)
            {
                DemoState = DEMO_STATE_WAITING_SET_LED;
            }
            else
            {
                UART2PrintString( "4 Device Write Error 0x" );
                UART2PutHex(RetVal);
                UART2PrintString( "\r\n" );
                LCDWriteLine( 2, "Dev Write Error " );
            }
        }
        break;

    case DEMO_STATE_WAITING_SET_LED:
        if (!USBHostGenericTxIsBusy(deviceAddress) )
            DemoState = DEMO_STATE_READ_SET_LED_RESP;
        break;

    case DEMO_STATE_READ_SET_LED_RESP:
        if (!USBHostGenericRxIsBusy(deviceAddress))
        {
            DataPacket.CMD = CMD_INVALID;
            if ( (RetVal=USBHostGenericRead(deviceAddress, &DataPacket, 1)) == USB_SUCCESS)
            {
                DemoState = DEMO_STATE_WAITING_LED_RESP;
            }
            else
            {
                UART2PrintString( "4 Device Read Error 0x" );
                UART2PutHex(RetVal);
                UART2PrintString( "\r\n" );
                LCDWriteLine( 2, "Dev Read Error  " );
            }
        }
        break;

    case DEMO_STATE_WAITING_LED_RESP:
        if (!USBHostGenericRxIsBusy(deviceAddress))
            DemoState = DEMO_STATE_UPDATE_LED_STATE;
        break;

    case DEMO_STATE_UPDATE_LED_STATE:
        if (DataPacket.CMD == UPDATE_LED)   // CMD updated by read from device
        {
            if (DataPacket.led_num == 3)    // led_num left-over from set-LED command send
            {
                LEDState.bits.b3 ^= 1;
//                mLED_10_Toggle();
            }
            else if (DataPacket.led_num == 4)
            {
                LEDState.bits.b4 ^= 1;
//                mLED_9_Toggle();
            }
        }
        DemoState = DEMO_STATE_GET_TEMPERATURE;
        break;

    /** Error state:  Hold here until detached **/
    case DEMO_STATE_ERROR:                          // To Do: Flash LEDs
        break;

    default:
        DemoState = DEMO_INITIALIZE;
        break;
    }

    DelayMs(1); // 1ms delay

} // ManageDemoState


//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

/*************************************************************************
 * Function:        USB_ApplicationEventHandler
 *
 * Preconditions:   The USB must be initialized.
 *
 * Input:           event       Identifies the bus event that occured
 *
 *                  data        Pointer to event-specific data
 *
 *                  size        Size of the event-specific data
 *
 * Output:          deviceAddress (global)
 *                  Updates device address when an attach or detach occurs.
 *
 *                  DemoState (global)
 *                  Updates the demo state as appropriate when events occur.
 *
 * Returns:         TRUE if the event was handled, FALSE if not
 *
 * Side Effects:    Event-specific actions have been taken.
 *
 * Overview:        This routine is called by the Host layer or client
 *                  driver to notify the application of events that occur.
 *                  If the event is recognized, it is handled and the
 *                  routine returns TRUE.  Otherwise, it is ignored (or
 *                  just "sniffed" and the routine returns FALSE.
 *************************************************************************/

BOOL USB_ApplicationEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
        BYTE i;
    #endif

    // Handle specific events.
    switch ( (INT)event )
    {
        case EVENT_GENERIC_ATTACH:
            if (size == sizeof(GENERIC_DEVICE_ID))
            {
                deviceAddress   = ((GENERIC_DEVICE_ID *)data)->deviceAddress;
                DemoState = DEMO_STATE_GET_DEV_VERSION;
                UART2PrintString( "Generic demo device attached - event, deviceAddress=" );
                UART2PutDec( deviceAddress );
                UART2PrintString( "\r\n" );
                #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
                    for (i=1; i<((GENERIC_DEVICE_ID *)data)->serialNumberLength; i++)
                    {
                        UART2PutChar( ((GENERIC_DEVICE_ID *)data)->serialNumber[i] );
                    }
                #endif
                UART2PrintString( "\r\n" );
                return TRUE;
            }
            break;

        case EVENT_GENERIC_DETACH:
            deviceAddress   = 0;
            DemoState = DEMO_INITIALIZE;
            UART2PrintString( "Generic demo device detached - event\r\n" );
            return TRUE;

        case EVENT_GENERIC_TX_DONE:           // The main state machine will poll the driver.
        case EVENT_GENERIC_RX_DONE:
            return TRUE;

        case EVENT_VBUS_REQUEST_POWER:
            // We'll let anything attach.
            return TRUE;

        case EVENT_VBUS_RELEASE_POWER:
            // We aren't keeping track of power.
            return TRUE;

        case EVENT_HUB_ATTACH:
            UART2PrintString( "\r\n***** USB Error - hubs are not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            UART2PrintString( "\r\n***** USB Error - device is not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_CANNOT_ENUMERATE:
            UART2PrintString( "\r\n***** USB Error - cannot enumerate device *****\r\n" );
            return TRUE;
            break;

        case EVENT_CLIENT_INIT_ERROR:
            UART2PrintString( "\r\n***** USB Error - client driver initialization error *****\r\n" );
            return TRUE;
            break;

        case EVENT_OUT_OF_MEMORY:
            UART2PrintString( "\r\n***** USB Error - out of heap memory *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            UART2PrintString( "\r\n***** USB Error - unspecified *****\r\n" );
            return TRUE;
            break;

        case EVENT_SUSPEND:
        case EVENT_DETACH:
        case EVENT_RESUME:
        case EVENT_BUS_ERROR:
            return TRUE;
            break;

        default:
            break;
    }

    return FALSE;

} // USB_ApplicationEventHandler


//******************************************************************************
//******************************************************************************
// Main
//******************************************************************************
//******************************************************************************

/*************************************************************************
 * Function:        main
 *
 * Preconditions:   None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         Never exits
 *
 * Side Effects:    Runs the application
 *
 * Overview:        This is the USB Custom Demo Application's main entry
 *                  point.
 *************************************************************************/

int main ( void )
{
    // Initialize the processor and peripherals.
    if ( InitializeSystem() != TRUE )
    {
        UART2PrintString( "\r\n\r\nCould not initialize USB Custom Demo App - system.  Halting.\r\n\r\n" );
        while (1);
    }
    if ( USBHostInit(0) == TRUE )
    {
        UART2PrintString( "\r\n\r\n***** USB Custom Demo App Initialized *****\r\n\r\n" );
    }
    else
    {
        UART2PrintString( "\r\n\r\nCould not initialize USB Custom Demo App - USB.  Halting.\r\n\r\n" );
        while (1);
    }

    // Main Processing Loop
    while (1)
    {
        // This demo does not check for overcurrent conditions.  See the
        // USB Host - Data Logger for an example of overcurrent detection
        // with the PIC24F and the USB PICtail Plus.

        // Maintain USB Host State
        USBHostTasks();
        // Maintain Demo Application State
        ManageDemoState();
    }

    return 0;

} // main


/*************************************************************************
 * EOF main.c
 */

