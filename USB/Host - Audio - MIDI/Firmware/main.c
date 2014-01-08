/******************************************************************************
            USB MIDI Demo, Host

This file provides the main entry point to the Microchip USB MIDI
Host demo.  This demo shows how a PIC24F system could be used to
act as the host, converting UART MIDI packets to USB MIDI packets,
and vice versa.

******************************************************************************/

/******************************************************************************
* Filename:        main.c
* Dependancies:    USB Host Driver with MIDI Client Driver
* Processor:       PIC24F256GB2xx
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
#include "USB/usb_host_midi.h"
#include "timer.h"
#include "uart2.h"

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
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
    #elif defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
        _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
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
    #pragma config DEBUG    = ON            // Background Debugger Enable

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

// Application specific
#define MIDI_USB_BUFFER_SIZE                (BYTE)4
#define MIDI_UART_BUFFER_SIZE               (BYTE)64
#define NUM_MIDI_PKTS_IN_USB_PKT            (BYTE)1


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
    STATE_INITIALIZE = 0,        // Initialize the app when a device is attached
    STATE_IDLE,                 // Inactive State
    STATE_READY,
    STATE_ERROR                 // An error has occured
} DEMO_STATE;

typedef enum
{
    TX_DATA = 0,
    TX_DATA_WAIT,
    RX_DATA,
    RX_DATA_WAIT,
    TX_REAL_TIME_DATA,
    TX_REAL_TIME_DATA_WAIT
} TX_RX_STATE;

typedef struct
{
    TX_RX_STATE             TransferState;      // The transfer state of the endpoint
    BYTE                    numOfMIDIPackets;   // Each USB Packet sent from a device has the possibility of holding more than one MIDI packet,
                                                //  so this is to keep track of how many MIDI packets are within a USB packet (between 1 and 16, or 4 and 64 bytes)
    USB_AUDIO_MIDI_PACKET*  bufferStart;        // The 2D buffer for the endpoint. There are MIDI_USB_BUFFER_SIZE USB buffers that are filled with numOfMIDIPackets
                                                //  MIDI packets. This allows for MIDI_USB_BUFFER_SIZE USB packets to be saved, with a possibility of up to 
                                                //  numOfMIDIPackets MID packets within each USB packet.
    USB_AUDIO_MIDI_PACKET*  pBufReadLocation;   // Pointer to USB packet that is being read from
    USB_AUDIO_MIDI_PACKET*  pBufWriteLocation;  // Pointer to USB packet that is being written to
}ENDPOINT_BUFFER;


// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

//Testing Purposes
ENDPOINT_BUFFER *endpointOne;
ENDPOINT_BUFFER *endpointTwo;



void*       deviceHandle;   // Handle to the attached device
DEMO_STATE  DemoState;      // Current state of the demo application
BIT_STATE   LEDState;       // Bitmap of LED states (only 3 & 4 are used)
WORD        SwitchState;    // Bitmap of Switch states (only 3 & 6 are used)

ENDPOINT_BUFFER*    endpointBuffers;    // A pointer to a dynamically allocated array that contains a buffer for each endpoint

//A circular buffer for storing received bytes from the host, out the UART to the attached device on the MIDI Out port
BYTE        MIDIOutTxBuffer[MIDI_UART_BUFFER_SIZE];
BYTE*       pTxBufReadLocation;
DWORD*      pTxBufWriteLocation;        // This is a double word (4 bytes), since we will be writing a double word at a time
BYTE        MIDIOutByteCount;

//A circular buffer for storing received bytes from the MIDI (UART), to be sent do attached USB MIDI device
BYTE        MIDIInRxBuffer[MIDI_UART_BUFFER_SIZE];
BYTE*       pRxUARTBufReadLocation;
BYTE*       pRxUARTBufWriteLocation;
BYTE        UARTInByteCount;

BYTE        MIDIRxCommandState;
BOOL        SysExCommandPending;
BOOL        BytePending;
BYTE        RxRunningStatusByte;
BYTE        TxRunningStatusByte;

// MIDI packet used to translate MIDI UART to MIDI USB, with flag
USB_AUDIO_MIDI_PACKET UARTTranslatedToUSB;
BOOL        UARTmidiPacketTranslated;

// MIDI packet used to translate MIDI UART to MIDI USB for real time messages, with flag, and buffer
USB_AUDIO_MIDI_PACKET UARTRealTimeToUSB;
USB_AUDIO_MIDI_PACKET UARTRealTimeToUSBBuffer;
BOOL        UARTRealTimePacketTranslated;

BOOL        somethingToSend;

WORD        NumGets;
WORD        NumSends;


// *****************************************************************************
// *****************************************************************************
// Private Prototypes
// *****************************************************************************
// *****************************************************************************

BOOL InitializeSystem(void);
BOOL Switch3WasPressed(void);
BOOL Switch6WasPressed(void);
void BlinkStatus(void);
void ManageDemoState(void);
void MIDIInJackTasks(void);
void MIDIOutJackTasks(void);
BOOL USB_ApplicationEventHandler(BYTE address, USB_EVENT event, void *data, DWORD size);


// ******************************************************************************
// ******************************************************************************
// UART2 Receive Interrupt
// ******************************************************************************
// ******************************************************************************
#if defined(__C30__) || defined __XC16__
void __attribute__ ((__interrupt__,no_auto_psv)) _U2RXInterrupt(void)
#elif defined(__PIC32MX__)
#pragma interrupt _U2RxInterrupt ipl5 vector 41
void _U2RxInterrupt( void )
#else
    #error Cannot define UART2 RX interrupt vector.
#endif
{
    // Clear interrupt flag
    IFS1bits.U2RXIF = 0;
    
    //Check for UART receive overrun.  This would normally only occur if the 
    //MIDI In jack device is broadcasting continuous traffic (ex: clock messages), 
    //while the USB enumeration sequence is still in progress.  
    if(U2STAbits.OERR == 1)
    {
        *pRxUARTBufWriteLocation = U2RXREG;
        *pRxUARTBufWriteLocation = U2RXREG;  
        U2STAbits.OERR = 0;
        //Now re-initialize state machine.  Can't predict what state the MIDI device
        //is actually in (since MIDI In jacks only receive, cannot transmit).  
        //Therefore, assume default states where we can recover automatically.
        MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;  //Abadon whatever command was in progress and wait for the next status/command byte   
    }// if(U2STAbits.OERR == 1)
    else    
    {
        if(U2STAbits.FERR == 0)
        {
            //Empty the FIFO UART buffer into our larger FIFO circular buffer, so we won't potentially lose any information
            //and cause a receive overrun. 
            while(U2STAbits.URXDA == 1)
            {
                *pRxUARTBufWriteLocation = U2RXREG;
                
                //Check for real time status messages.  If so need to prepare an immediate USB packet.
                //Realtime messages are 0xF8 to 0xFF, although 0xF9 and 0xFD aren't implemented
                //and should be ignored if they are received.
                //Don't mess with the state machine or other variables.  A real time message
                //can interrupt some other MIDI message flow, without disrupting handling state.
                if(*pRxUARTBufWriteLocation >= 0xF8)
                {
                    if((*pRxUARTBufWriteLocation != 0xF9) && (*pRxUARTBufWriteLocation != 0xFD))
                    {
                        //Prepare to send an immediate USB packet.  These real time messages don't
                        //have any following data bytes.  Therefore, the USB packet can be prepared
                        //directly.
                        UARTRealTimeToUSBBuffer.Val = 0;
                        UARTRealTimeToUSBBuffer.CodeIndexNumber = MIDI_CIN_1_BYTE_MESSAGE;
                        UARTRealTimeToUSBBuffer.MIDI_0 = *pRxUARTBufWriteLocation;
                        UARTRealTimePacketTranslated = TRUE;
                        
                    }// if((*pRxUARTBufWriteLocation != 0xF9) && (*pRxUARTBufWriteLocation != 0xFD))
                    return;
                }// if(*pRxUARTBufWriteLocation >= 0xF8)
                
                pRxUARTBufWriteLocation++;
                UARTInByteCount++;
                if((unsigned int)pRxUARTBufWriteLocation >= ((unsigned int)&MIDIInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pRxUARTBufWriteLocation = &MIDIInRxBuffer[0];
                }
            }// while(U2STAbits.URXDA == 1)
        }// if(U2STAbits.FERR == 0)
        else
        {
            //A UART Framing error occurred...  Valuable data was presumably
            //just lost.  Not much we can do about this, other than make sure
            //we maintain the state machine in such a way as we can recover 
            //automatically after the event.
            *pRxUARTBufWriteLocation = U2RXREG;  
            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;  //Abadon whatever command was in progress and wait for the next status/command byte
        }// else
    }
}    


// ******************************************************************************
// ******************************************************************************
// Main
// ******************************************************************************
// ******************************************************************************

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
        UART2PrintString( "\r\n\r\nCould not initialize USB MIDI Demo App - system.  Halting.\r\n\r\n" );
        while (1);
    }
    if ( USBHostInit(0) == TRUE )
    {
        // Removing to prevent MIDI out traffic //UART2PrintString( "\r\n\r\n***** USB MIDI Demo App Initialized *****\r\n\r\n" );
    }
    else
    {
        UART2PrintString( "\r\n\r\nCould not initialize USB MIDI Demo App - USB.  Halting.\r\n\r\n" );
        while (1);
    }

    // Main Processing Loop
    while (1)
    {
        // Maintain USB Host State
        USBHostTasks();
        // Maintain Demo Application State
        ManageDemoState();
    }

    return 0;

} // main


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
    #if defined( __PIC24FJ256GB110__ ) || defined(__PIC24FJ256GB210__)
        // PPS - Configure U2RX - put on pin 49 (RP10)
        RPINR19bits.U2RXR = 10;

        // PPS - Configure U2TX - put on pin 50 (RP17)
        RPOR8bits.RP17R = 5;
        
        OSCCON = 0x3302;    // Enable secondary oscillator
        CLKDIV = 0x0000;    // Set PLL prescaler (1:1)

        TRISD = 0x00C0;

   #elif defined(__PIC24FJ64GB004__)
        OSCCON = 0x3302;    // Enable secondary oscillator
        CLKDIV = 0x0000;    // Set PLL prescaler (1:1)

    	//On the PIC24FJ64GB004 Family of USB microcontrollers, the PLL will not power up and be enabled
    	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
    	//This allows the device to power up at a lower initial operating frequency, which can be
    	//advantageous when powered from a source which is not gauranteed to be adequate for 32MHz
    	//operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
    	//power up the PLL.
        {
            unsigned int pll_startup_counter = 6000;
            CLKDIVbits.PLLEN = 1;
            while(pll_startup_counter--);
        }
    
        //Device switches over automatically to PLL output after PLL is locked and ready.
    
        // PPS - Configure U2RX - put on RC3/pin 36 (RP19)
        RPINR19bits.U2RXR = 19;

        // PPS - Configure U2TX - put on RC9/pin 5 (RP25)
        RPOR12bits.RP25R = 5;

        TRISCbits.TRISC9 = 0;

        AD1PCFG = 0xFFFF;   // Set analog pins to digital.

   #elif defined(__PIC24FJ256DA210__)
        OSCCON = 0x3302;    // Enable secondary oscillator
        CLKDIV = 0x0000;    // Set PLL prescaler (1:1)

    	//On the PIC24FJ256DA210 Family of USB microcontrollers, the PLL will not power up and be enabled
    	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
    	//This allows the device to power up at a lower initial operating frequency, which can be
    	//advantageous when powered from a source which is not gauranteed to be adequate for 32MHz
    	//operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
    	//power up the PLL.
        {
            unsigned int pll_startup_counter = 6000;
            CLKDIVbits.PLLEN = 1;
            while(pll_startup_counter--);
        }
    
        //Device switches over automatically to PLL output after PLL is locked and ready.
    
        // PPS - Configure U2RX - put on RC14/pin 74 (RPI37)
        RPINR19bits.U2RXR = 37;

        // PPS - Configure U2TX - put on RF3/pin 51 (RP16)
        RPOR8bits.RP16R = 5;

        TRISFbits.TRISF3 = 0;

    #elif defined(__PIC32MX__)
    {
        int value;
        #if defined(RUN_AT_60MHZ)
            // Use OSCCON default
        #else
            OSCCONCLR = 0x38000000; //PLLODIV
            #if defined(RUN_AT_48MHZ)
                OSCCONSET = 0x08000000; //PLLODIV /2
            #elif defined(RUN_AT_24MHZ)
                OSCCONSET = 0x10000000; //PLLODIV /4
            #else
                #error Cannot set OSCCON
            #endif
        #endif

        value = SYSTEMConfigWaitStatesAndPB( GetSystemClock() );

        // Enable the cache for the best performance
        CheKseg0CacheOn();

        INTEnableSystemMultiVectoredInt();

        value = OSCCON;
        while (!(value & 0x00000020))
        {
            value = OSCCON;    // Wait for PLL lock to stabilize
        }

        INTEnableInterrupts();

        AD1PCFG = 0xFFFF;   // Set analog pins to digital.
        TRISF = 0x00;
    }
    #else
        #error Cannot initialize
    #endif

    // Init LEDs
    mInitAllLEDs();
    mLED_1_Off();
    mLED_2_Off();

    // Init Switches
    mInitAllSwitches();
    SwitchState = IOPORT_BIT_6|IOPORT_BIT_7;

    // Init UART
    UART2Init();
    // Turn interrupt for UART receive on, with highest priority
    IEC1bits.U2RXIE = 1;
    IPC7 |= 0x0700;

    // Set Default demo and MIDI Command states
    DemoState = STATE_INITIALIZE;
    MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;
    
    // Setup circular FIFO for MIDI Out port
    MIDIOutByteCount = 0;
    pTxBufReadLocation = &MIDIOutTxBuffer[0];
    pTxBufWriteLocation = (DWORD*)&MIDIOutTxBuffer[0];
    
    // Setup circular FIFO for MIDI In port
    UARTInByteCount = 0;
    pRxUARTBufReadLocation = &MIDIInRxBuffer[0];
    pRxUARTBufWriteLocation = &MIDIInRxBuffer[0];
    
    // Clear UART translation packet and real-time packet/buffer
    UARTTranslatedToUSB.Val = 0;
    UARTRealTimeToUSB.Val = 0;
    UARTRealTimeToUSBBuffer.Val = 0;
    
    
    // Set device handle to NULL
    deviceHandle = NULL;
    
    // Clear status flags
    SysExCommandPending = FALSE;
    UARTmidiPacketTranslated = FALSE;
    UARTRealTimePacketTranslated = FALSE;

    NumGets = 0;
    NumSends = 0;

    return TRUE;
} // InitializeSystem


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
        led_count = 10000U;

    led_count--;

    if(DemoState <= STATE_IDLE)
    {
        mLED_1_Off();
        mLED_2_Off();
    }
    else if(DemoState == STATE_READY)
    {
        if (led_count == 0)
        {
            mLED_1_Toggle();
            mLED_2 = !mLED_1;       // Alternate blink
        }
    }
    else    // STATE_ERROR
    {
        if (led_count == 0)
        {
            mLED_1_Toggle();
            mLED_2 = mLED_1;        // Both blink at the same time
            led_count = 50;
        }
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
    BYTE currentEndpoint;
    
    BlinkStatus();

    switch (DemoState)
    {
        case STATE_INITIALIZE:
            DemoState = STATE_IDLE;
            break;
    
        /** Idle State:  Loops here until attach **/
        case STATE_IDLE:
            break;
            
        /** Ready State: Loops here until detach or error **/
        case STATE_READY:
            //-----------------------------------------------------------------------------
            //Check if we received any RX data from the MIDI device attached (MIDI In port)
            //-----------------------------------------------------------------------------
            MIDIInJackTasks();
            
            //-----------------------------------------------------------------------------
            //Check if any TX data needs to be sent to the MIDI device attached (MIDI
            // Out port)
            //-----------------------------------------------------------------------------
            MIDIOutJackTasks();
        
            for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
            {
                switch(endpointBuffers[currentEndpoint].TransferState)
                {
                    /** Send IN Packet: Loops here until successfuly sends packet **/
                    case RX_DATA:
                        // Then clear current endpoint's buffer, and try to read
                        memset(endpointBuffers[currentEndpoint].pBufWriteLocation, 0x00, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET));
                        if(USBHostMIDIRead(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufWriteLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET) ) == USB_SUCCESS )
                        {
                            // If successful, then update the transfer state to wait for the transaction to complete    
                            endpointBuffers[currentEndpoint].TransferState = RX_DATA_WAIT;
                        }
                        break;

                    /** Send OUT Packet: Loops here until successfuly sends packet **/
                    case TX_DATA:
                        // See if there is a real-time message to send. These have a higher priority
                        if(UARTRealTimePacketTranslated == TRUE)
                        {
                            // If so, then clear the flag and write the buffer to the transmit packet
                            UARTRealTimeToUSB.Val = UARTRealTimeToUSBBuffer.Val;
                            UARTRealTimePacketTranslated = FALSE;
                            
                            // And change state to that case, so next time it will get transmitted
                            endpointBuffers[currentEndpoint].TransferState = TX_REAL_TIME_DATA;
                        }
                            
                        // Otherwise, try to send if there's something in the buffer
                        else if(endpointBuffers[currentEndpoint].pBufReadLocation != endpointBuffers[currentEndpoint].pBufWriteLocation)
                        {
                            if(USBHostMIDIWrite(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufReadLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
                            {
                                // Update the transfer state to wait for the transaction to complete    
                                endpointBuffers[currentEndpoint].TransferState = TX_DATA_WAIT;
                            }
                        }    
                        break;
                        
                    case TX_REAL_TIME_DATA:
                        // Try to send the real-time data
                        if(USBHostMIDIWrite(deviceHandle, currentEndpoint, &UARTRealTimeToUSB, sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
                        {
                            endpointBuffers[currentEndpoint].TransferState = TX_REAL_TIME_DATA_WAIT;
                        }
                        break;
                    
                    /** Get IN Data: Loops here for this endpoint until successfully receives data packet **/
                    case RX_DATA_WAIT:
                        // If the transfer is complete
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            // Update the transfer state to try and receive another packet
                            endpointBuffers[currentEndpoint].TransferState = RX_DATA;
                            
                            // Then increment the write location pointer to the next USB buffer
                            endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                            
                            // Check to see if we've gotten to the end of the USB buffer array
                            if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart
                            >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                            {
                                // If so, then loop it back to the beginning of the array
                                endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                            }
                        }
                        break;
                    
                    /** Send OUT Data: Loops here for this endpoint until successfully sends data packet **/
                    case TX_DATA_WAIT:
                        // If the transfer is complete
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            // Update the transfer state to try and send another packet
                            endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                            
                            // Then increment the read location pointer to the next USB buffer
                            endpointBuffers[currentEndpoint].pBufReadLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                                
                            // Check to see if we've gotten to the end of the USB buffer array
                            if(endpointBuffers[currentEndpoint].pBufReadLocation - endpointBuffers[currentEndpoint].bufferStart
                            >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                            {
                                // If so, then loop it back to the beginning of the array
                                endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
                            }
                        }        
                        break;
                        
                    case TX_REAL_TIME_DATA_WAIT:
                        // If the transfer is complete
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            // Update the transfer state back to trying to send another packet
                            endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                        }
                        break;    
                }
            }    
            break;
            
        /** Error state:  Hold here until detached **/
        case STATE_ERROR:
            break;
    
        default:
            DemoState = STATE_INITIALIZE;
            break;
    }
} // ManageDemoState


//-----------------------------------------------------------------------------
//Check the MIDI UART FIFO buffer to see if any MIDI traffic was received by
// the interrupt. If so, we need to copy it from the UART FIFO buffer to the USB FIFO buffer,
// and translate it into the USB MIDI packet.
//-----------------------------------------------------------------------------
void MIDIInJackTasks(void)
{
    static BYTE ExpectedDataBytes;
    static BYTE WriteIndex;
    
    //If we have something within the UART MIDI Rx Buffer
    if(UARTInByteCount > 0)
    {
        //Check if we received a new status (MIDI command) byte.  If so, regardless of previous state
        //jump to the state where we are starting to receive a new MIDI command packet.
        if(*pRxUARTBufReadLocation & 0x80) 
        {
            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;
            RxRunningStatusByte = *pRxUARTBufReadLocation;   //Save the Status byte.
            
            //Reset write index pointer to to start of MIDI_x field of USB packet buffer,
            //unless we just recevied the MIDI_STATUS_SYSEX_END, where we will still need the old
            //WriteIndex value to know the proper CIN byte value of the next USB packet.
            if(*pRxUARTBufReadLocation != MIDI_STATUS_SYSEX_END)
            {
                WriteIndex = 2;
                UARTTranslatedToUSB.Val = 0;   //All pad bits/bytes = 0 according to MIDI-Audio USB specs.
            }// if(*pRxUARTBufReadLocation != MIDI_STATUS_SYSEX_END)
            
            //We are eventually going to consume the byte we just received, 
            //before returning in this scenario.
            pRxUARTBufReadLocation++;
            UARTInByteCount--;
            if((unsigned int)pRxUARTBufReadLocation >= ((unsigned int)&MIDIInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
            {
                pRxUARTBufReadLocation = &MIDIInRxBuffer[0];
            }
        }// if(*pRxUARTBufReadLocation & 0x80) 
        else if(MIDIRxCommandState == STATE_WAIT_STATUS_BYTE)   //Now check for "Running Status"
        {
            //If we get to here, this means we just received the first data byte,
            //that was part of a running status command.
            //In this case, we don't want to consume the ReceivedByte yet.  For this
            //iteration of this [if(UARTInByteCount > 0)] code, we just need to re-process 
            //the previous RxRunningStatusByte (ex: don't write to buffer here).  Then on the
            //next iteration we will actually consume the newest ReceivedByte.
        }// else if(MIDIRxCommandState == STATE_WAIT_STATUS_BYTE)
        else
        {
            //We are going to consume the (non-status) byte we just received, in this scenario.
        }// else
        
        //Check if we already received the Status (MIDI command) byte already, and
        //are currently waiting/expecting to receive data bytes.
        if(MIDIRxCommandState == STATE_WAITING_BYTES)
        {
            //Check if we are handling a SysEx command, or a normal command.
            if(SysExCommandPending == TRUE)
            {
                //Handle the variable data length SysEx command.
                *((BYTE*)&UARTTranslatedToUSB + WriteIndex) = *pRxUARTBufReadLocation;
                pRxUARTBufReadLocation++;
                UARTInByteCount--;
                if((unsigned int)pRxUARTBufReadLocation >= ((unsigned int)&MIDIInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pRxUARTBufReadLocation = &MIDIInRxBuffer[0];
                }
                if(WriteIndex < 3u)
                {
                    WriteIndex++;
                }// if(WriteIndex < 3u)
                else if(WriteIndex == 3)
                {
                    //We have accumulated a full USB-MIDI packet worth of data.
                    //Prepare state machine to send the packet.
                    UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_START; //Might also be continuing instead of starting.
                    
                    UARTmidiPacketTranslated = TRUE; //Set boolean flag here to let USB code know it should prepare a packet.
                    WriteIndex = 1; //Move write pointer/index back to start of MIDI_x field of USB packet buffer
                }// else if(WriteIndex == 3)                 
            }// if(SysExCommandPending == TRUE)
            else
            {    
                *(BYTE*)(((BYTE*)&UARTTranslatedToUSB) + WriteIndex) = *pRxUARTBufReadLocation;
                pRxUARTBufReadLocation++;
                UARTInByteCount--;
                if((unsigned int)pRxUARTBufReadLocation >= ((unsigned int)&MIDIInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pRxUARTBufReadLocation = &MIDIInRxBuffer[0];
                }
                if(ExpectedDataBytes != 0)
                {
                    WriteIndex++;
                    ExpectedDataBytes--;
                    //Check if we finished receiving all expected bytes in the MIDI command
                    if(ExpectedDataBytes == 0)
                    {
                        UARTmidiPacketTranslated = TRUE; //Set boolean flag here so we know to add packet to buffer OUT
                        MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;                
                    }// if(ExpectedDataBytes == 0)
                }// if(ExpectedDataBytes != 0)
            }// else
        }// if(MIDIRxCommandState == STATE_WAITING_BYTES)
        else if(MIDIRxCommandState == STATE_WAIT_STATUS_BYTE)
        {
            ExpectedDataBytes = 0;  //Will get updated with correct value, once we have parsed the status byte and know how many data bytes to expect.
            MIDIRxCommandState = STATE_WAITING_BYTES;   //Initialize UART recevier state machine
            
            //We presumably have just received a status byte (MIDI command byte)
            if(RxRunningStatusByte & 0x80) //Check if we received a new status (MIDI command) byte, indicated by bit 7 of UART byte being set
            {
                //Check if it was a system common command of some kind (upper nibble = 0xF)
                if((RxRunningStatusByte & 0xF0) == 0xF0) 
                {
                    UARTTranslatedToUSB.MIDI_0 = RxRunningStatusByte;
                    //Check for system common commands
                    switch(RxRunningStatusByte)
                    {
                        case(MIDI_STATUS_SYSEX_START):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_START;
                            ExpectedDataBytes = 255;    //Not really expecting 255.  SysEx is variable/unknown length.
                            SysExCommandPending = TRUE; //Let state machine know to perform special handling for this command.
                            break;
                        case(MIDI_STATUS_SYSEX_END):
                            //This case is special, since the actual data payload size is not fixed.
                            //However, when we get this byte, this means the SysEx command is completing,
                            //and therefore we can determine the residual number of bytes we need to send
                            //based on the WriteIndex variable.
                            if(WriteIndex == 1)
                                UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_1;
                            else if(WriteIndex == 2)
                                UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_2;
                            else if(WriteIndex == 3)
                                UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_3;
                            //Complete the request here, rather than the normal handler.
                            WriteIndex = 2; //Move write pointer/index back to start of buffer for next MIDI command
                            UARTmidiPacketTranslated = TRUE; //Set boolean flag here to let USB code know it should prepare a packet.
                            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;  
                            SysExCommandPending = FALSE;        //No longer handling a SysEx data flow.
                            ExpectedDataBytes = 0;
                            break;
                        case(MIDI_STATUS_MTC_QFRAME):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_2_BYTE_MESSAGE;
                            ExpectedDataBytes = 1;
                            break;
                        case(MIDI_STATUS_SONG_POSITION):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_3_BYTE_MESSAGE;
                            ExpectedDataBytes = 2;
                            break;
                        case(MIDI_STATUS_SONG_SELECT):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_2_BYTE_MESSAGE;
                            ExpectedDataBytes = 1;
                            break;
                        case(MIDI_STATUS_TUNE_REQUEST):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_1_BYTE_MESSAGE;
                            ExpectedDataBytes = 0;
                            break;
                        default:
                            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;
                            ExpectedDataBytes = 0;                            
                            break;
                    }//switch(RxRunningStatusByte)    
                    
                    //Check if we finished receiving all expected bytes in the MIDI command.
                    //This would be the case for system commands that consist only of the status byte. 
                    if((ExpectedDataBytes == 0) && (MIDIRxCommandState = STATE_WAITING_BYTES))  //Not note really waiting for extra bytes in this scenario.
                    {
                        //Prepare state machine to prepare USB transmit packet.
                        UARTmidiPacketTranslated = TRUE; //Set boolean flag here to let USB code know it should prepare a packet.
                        MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;                
                    } 
                }//if((RxRunningStatusByte & 0xF0) == 0xF0) 
                else
                {
                    //Must have been a normal MIDI command status byte
                    UARTTranslatedToUSB.MIDI_0 = RxRunningStatusByte;
                    WriteIndex = 2;
                    RxRunningStatusByte &= 0xF0;   //Strip off the channel bits to get the command by itself
                    switch(RxRunningStatusByte)
                    {
                        case(MIDI_CIN_NOTE_OFF << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_NOTE_OFF;
                            break;
                        case(MIDI_CIN_NOTE_ON << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_NOTE_ON;
                            break;
                        case(MIDI_CIN_POLY_KEY_PRESS << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_POLY_KEY_PRESS;
                            break;
                        case(MIDI_CIN_CONTROL_CHANGE << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
                            break;
                        case(MIDI_CIN_PROGRAM_CHANGE << 4):
                            ExpectedDataBytes = 1;      //Expect 1 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PROGRAM_CHANGE;
                            break;
                        case(MIDI_CIN_CHANNEL_PREASURE << 4):
                            ExpectedDataBytes = 1;      //Expect 1 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CHANNEL_PREASURE;
                            break;
                        case(MIDI_CIN_PITCH_BEND_CHANGE << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PITCH_BEND_CHANGE;
                            break;
                        default:
                            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;
                            ExpectedDataBytes = 0;
                            break;
                    }//switch(RxRunningStatusByte)                           
                }//else        
            }//if(RxRunningStatusByte & 0x80) 
        }//else if(MIDIRxCommandState == STATE_WAIT_STATUS_BYTE)
    }//if(UARTInByteCount > 0)
    
    // If the UART to USB translation has completed, then add packet to the OUT endpoint buffer(s)
    if(UARTmidiPacketTranslated == TRUE)
    {
        BYTE currentEndpoint;
        
        // Clear the flag
        UARTmidiPacketTranslated = FALSE;
        
        // Loop through the endpoints
        for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
        {
            // If this is an OUT endpoint
            if(USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint) == OUT)
            {
                // Then add the translated MIDI packet to its buffer and increment (we're only doing one MIDI packet per USB packet)
                endpointBuffers[currentEndpoint].pBufWriteLocation->Val = UARTTranslatedToUSB.Val;
                endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                
                // Check to see if we've gotten to the end of the USB buffer array
                if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart
                >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                {
                    // If so, then loop it back to the beginning of the array
                    endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                }
                
                // If we want to send the data to just one endpoint, then we'll break, otherwise we'll spam all of the OUT endpoints
                #ifdef SEND_TO_ONE_EP
                    break;
                #endif
            }// if(USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint) == OUT)
        }// for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)    
    }// if(UARTmidiPacketTranslated == TRUE)
}//void MIDIInJackTasks    
    
    

//-----------------------------------------------------------------------------
//Check if any valid data is currently waiting in the endpoint buffers that needs
// to be sent over the UART.
//-----------------------------------------------------------------------------
void MIDIOutJackTasks(void)
{
    BYTE currentEndpoint;
    
    // First, go through each IN endpoint and add packets from its USB buffer to the UART OUT FIFO
    for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
    {
        // Check to see if this is an IN endpoint, and if the buffer has any packets to be read
        if((USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint) == IN)
        && (endpointBuffers[currentEndpoint].pBufReadLocation != endpointBuffers[currentEndpoint].pBufWriteLocation))
        {
            BYTE midiPkt;            
                
            // If so, then parse through the entire USB packet for each individual MIDI packet
            for(midiPkt = 0; midiPkt < endpointBuffers[currentEndpoint].numOfMIDIPackets; midiPkt++)
            {
                if(endpointBuffers[currentEndpoint].pBufReadLocation->Val == 0ul)
                {
                    // If there's nothing in this MIDI packet, then skip the rest of the USB packet
                    endpointBuffers[currentEndpoint].pBufReadLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets - midiPkt;
                    break;
                }    
                else
                {
                    // Otherwise add it to the UART OUT FIFO and increment both pointers
                    *pTxBufWriteLocation = endpointBuffers[currentEndpoint].pBufReadLocation->Val;
                    pTxBufWriteLocation++;
                    endpointBuffers[currentEndpoint].pBufReadLocation++;
                    MIDIOutByteCount += 4;
                    
                    // Check to see if we've gotten to the end of the UART OUT FIFO array
                    if((unsigned int)pTxBufWriteLocation >= ((unsigned int)&MIDIOutTxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                    {
                        // If so, then loop it back to the beginning of the array
                        pTxBufWriteLocation = (DWORD*)&MIDIOutTxBuffer[0];
                    }
                }
            }
            
            // After parsing through entire USB packet, check to see if we've gotten to the end of the USB array
            if(endpointBuffers[currentEndpoint].pBufReadLocation - endpointBuffers[currentEndpoint].bufferStart
            >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
            {
                // If so, then loop it back to the beginning of the array
                endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
            }
        }
    }        
    
    
    // Next, continue sending UART data from the FIFO UART OUT buffer
    if((MIDIOutByteCount != 0u) && (U2STAbits.TRMT == 1)) 
    {
        //Check if the byte was not on a 4-byte boundary.  If so, that means we
        //are currently pointing to a Cable#/CIN byte, which we shouldn't 
        //re-transmit over the UART MIDI output port.  Just gobble it up/throw
        //it away, since we are currently only implementing support for one MIDI
        //OUT jack anyway.
        //All other MIDI status/data bytes we re-transmit directly.
        switch((unsigned int)(pTxBufReadLocation - &MIDIOutTxBuffer[0]) & 0x0003)
        {
            case 0:
                //In this case, we are currently pointing to the Cable#/CIN byte.
                //Just gobble it up from the FIFO and perform error case checking.
                pTxBufReadLocation++;
                //Check if time for read FIFO pointer wrap around
                if(pTxBufReadLocation >= (&MIDIOutTxBuffer[0] + MIDI_UART_BUFFER_SIZE))
                {
                    pTxBufReadLocation = &MIDIOutTxBuffer[0];
                }    
                MIDIOutByteCount--; //Consumed a byte from the circular FIFO
                
                //Now error check, to make sure that the byte we are currently pointing
                //a is a status byte (bit #7 set).  It always should be, unless the
                //host sends an illegal MIDI data packet (not 32-bit fixed width
                //like specifications require).  
                if(!(*pTxBufReadLocation & 0x80))
                {
                    //In this scenario, we should flush the FIFO and re-inilize our
                    //pointers.  This is draconian, but the data in the buffer
                    //probably isn't trustworthy.  Additionally, during operation 
                    //the FIFO wouldn't normally contain more than 1 MIDI command 
                    //at a time anyway, so this shouldn't result in any major loss
                    //of data.
                    pTxBufWriteLocation = (DWORD*)&MIDIOutTxBuffer[0];
                    pTxBufReadLocation = &MIDIOutTxBuffer[0];
                    MIDIOutByteCount = 0;
                }
                break;
            case 1:
                if(*pTxBufReadLocation == TxRunningStatusByte)
                {
                    pTxBufReadLocation++;
                    MIDIOutByteCount--;
                    break;
                }   
                else if(*pTxBufReadLocation < 0xF8u)
                {
                    TxRunningStatusByte = *pTxBufReadLocation;
                }
            default:  
            case 2:
            case 3:
                U2TXREG = *pTxBufReadLocation;
                pTxBufReadLocation++;
                //Check if time for read FIFO pointer wrap around
                if((unsigned int)pTxBufReadLocation >= ((unsigned int)&MIDIOutTxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pTxBufReadLocation = &MIDIOutTxBuffer[0];
                }    
                MIDIOutByteCount--; //Consumed a byte from the circular FIFO
                break;
        }//switch((unsigned int)(pTxBufReadLocation - &MIDIOutTxBuffer[0]) & 0x0003)
    }//if((MIDIOutByteCount != 0u) && (PIR3bits.TX2IF == 1)) 
}//void MIDITasks(void)

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
 * Output:          deviceHandle (global)
 *                  Updates device handle pointer.
 *
 *                  endpointBuffers (global)
 *                  Allocates or Deallocates endpoint buffers.
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
    BYTE currentEndpoint;
    ENDPOINT_DIRECTION direction;
    
    
    // Handle specific events.
    switch ((INT)event)
    {
        case EVENT_MIDI_ATTACH:
            deviceHandle = data;
            DemoState = STATE_READY;
            
            endpointBuffers = malloc( sizeof(ENDPOINT_BUFFER) * USBHostMIDINumberOfEndpoints(deviceHandle) );
            
            for( currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++ )
            {
                direction = USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint);
                // For OUT endpoints
                if(direction == OUT)
                {   
                    // We only want to send NUM_MIDI_PKTS_IN_USB_PKT MIDI packet per USB packet
                    endpointBuffers[currentEndpoint].numOfMIDIPackets = NUM_MIDI_PKTS_IN_USB_PKT;
                    
                    // And we want to start it off transmitting data
                    endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                }
                // For IN endpoints
                else if (direction == IN)
                {
                    // We will accept however many will fit inside the maximum USB packet size
                    endpointBuffers[currentEndpoint].numOfMIDIPackets = USBHostMIDISizeOfEndpoint(deviceHandle, currentEndpoint) / sizeof(USB_AUDIO_MIDI_PACKET);
                    
                    // And we want to start it off trying to read data
                    endpointBuffers[currentEndpoint].TransferState = RX_DATA;
                }
                else
                {
                    continue;
                }
                
                // Allocate the 2D buffer, and keep track of the write and read locations
                endpointBuffers[currentEndpoint].bufferStart = malloc( sizeof(USB_AUDIO_MIDI_PACKET) * endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE );
                endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
                endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
            }    
            
            // Testing Purposes
            endpointOne = &endpointBuffers[0];
            endpointTwo = endpointBuffers+1;
            
            //UART2PrintString( "MIDI demo device attached - event, deviceAddress=" );
            //UART2PutDec( address );
            //UART2PrintString( "\r\n" );
            //UART2PrintString( "\r\n" );
            return TRUE;

        case EVENT_MIDI_DETACH:
            for( currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++ )
            {
                free(endpointBuffers[currentEndpoint].bufferStart);
                endpointBuffers[currentEndpoint].bufferStart = NULL;
                endpointBuffers[currentEndpoint].pBufReadLocation = NULL;
                endpointBuffers[currentEndpoint].pBufWriteLocation = NULL;
            }
            
            free(endpointBuffers);
            endpointBuffers = NULL;
            
            deviceHandle = NULL;
            DemoState = STATE_INITIALIZE;
            return TRUE;

        case EVENT_MIDI_TRANSFER_DONE:  // The main state machine will poll the driver.
        case EVENT_VBUS_REQUEST_POWER:
        case EVENT_VBUS_RELEASE_POWER:
        case EVENT_HUB_ATTACH:
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
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


/*************************************************************************
 * EOF main.c
 */
