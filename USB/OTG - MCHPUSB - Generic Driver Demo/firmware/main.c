/************************************************************************
 FileName:		main.c
 Dependencies:	See INCLUDES section
 Processor:		PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: 
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
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

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
********************************************************************/

/** INCLUDES *******************************************************/
#include "HardwareProfile.h"
#include "USB/usb.h"
#include "USB/usb_host_generic.h"
#include "USB/usb_function_generic.h"
#include "LCDBlocking.h"
#include "timer.h"
#include "user.h"                              // Modifiable
#include "USB/usb_otg.h"

#if defined(__PIC32MX__)
    #include <plib.h>
#endif

#include "uart2.h"


/** CONFIGURATION **************************************************/
#if defined(EXPLORER_16)
    #ifdef __PIC24FJ256GB110__ //Defined by MPLAB when using 24FJ256GB110 device
        _CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & PLL_96MHZ_ON & PLLDIV_DIV2 & IESO_OFF) // Primary HS OSC with PLL, USBPLL /2
        _CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)   // JTAG off, watchdog timer off

     #elif __PIC32MX__
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
        #error No hardware board defined, see "HardwareProfile.h"
    #endif
#else
    #error No hardware board defined, see "HardwareProfile.h"
#endif



/** VARIABLES ******************************************************/
#pragma udata
extern USB_HANDLE USBGenericOutHandle;
extern USB_HANDLE USBGenericInHandle;
extern DATA_PACKET INPacket;
extern DATA_PACKET OUTPacket;

/** PRIVATE PROTOTYPES *********************************************/
void InitializeSystem(void);
void USBDeviceTasks(void);
void YourHighPriorityISRCode(void);
void YourLowPriorityISRCode(void);
void USBCBSendResume(void);



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



BYTE        deviceAddress;  // Address of the device on the USB
DATA_PACKET DataPacket;     // Data to send to the device
DEMO_STATE  DemoState;      // Current state of the demo application
BYTE        LcdData[2][16]; // LCD Display data
BIT_STATE   LEDState;       // Bitmap of LED states (only 3 & 4 are used)
WORD        SwitchState;    // Bitmap of Switch states (only 3 & 6 are used)

const char *LcdInitData[] = { "Host -.- Dev -.-", "--.-C ----- Ohms" };

// Macro to fill a line of LCD data.
#define FILL_LCD_LINE(l,p) {int i; for(i=0;i<16;i++)LcdData[(l)-1][i]=(p)[i];}

//static void InitializeSystem(void);
//void USBTasks(void);

/** DECLARATIONS ***************************************************/
#pragma code

/******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/

int main(void)
{  
    #if defined( __PIC24FJ256GB110__ )
            // Configure U2RX - put on pin 49 (RP10)
            RPINR19bits.U2RXR = 10;

            // Configure U2TX - put on pin 50 (RP17)
            RPOR8bits.RP17R = 5;

            CLKDIV = 0x0000;    // Set PLL prescaler (1:1)
            OSCCON = 0x3302;    // Enable secondary oscillator

    #elif defined(__PIC32MX__)
             int  value;

            value = SYSTEMConfigWaitStatesAndPB(GetSystemClock());
            
            // Enable the cache for the best performance
            CheKseg0CacheOn();

            INTEnableSystemMultiVectoredInt();

            value = OSCCON;
            while (!(value & 0x00000020))
            {
                value = OSCCON;    // Wait for PLL lock to stabilize
            }

            SYSKEY = 0x0; // ensure OSCCON is locked
            SYSKEY = 0xAA996655; // Write Key1 to SYSKEY
            SYSKEY = 0x556699AA; // Write Key2 to SYSKEY
            // OSCCON is now unlocked
            // make the desired change
            OSCCONSET = 2; // enable SOSC
            // Relock the SYSKEY
            SYSKEY = 0x0; // Write

            AD1PCFG = 0xFFFF;   // Set analog pins to digital.
            TRISF   = 0x00;
    #endif
    
    TRISA = 0x0000;
    TRISD = 0x00C0;
    LCDInit();
    UART2Init();
    USBOTGInitialize();
    InitializeSystem();
    
    while(1)
    {  
        //SW3 Toggles a Session or Requests a Session
        if (Switch3WasPressed() && !USBOTGRoleSwitch())
        {
             //If A side Host and Currently a Host Then
             if (USBOTGDefaultRoleIs()== ROLE_HOST && USBOTGCurrentRoleIs()== ROLE_HOST)
             {
                //Toggle Session
                USBOTGSession(TOGGLE_SESSION);
             }

             //If A side Host and Currently a Device Then
             else if (USBOTGDefaultRoleIs() == ROLE_HOST && USBOTGCurrentRoleIs() == ROLE_DEVICE)
             {
                //Indicate Error Message
                #ifdef DEBUG_MODE
                        UART2PrintString( "\r\n*****USB OTG A Error - Session Toggle - Cannot Toggle Session When Role Is Peripheral Device *****\r\n" ); 
                #endif
             }

            else if (USBOTGDefaultRoleIs() == ROLE_DEVICE && USBOTGCurrentRoleIs() == ROLE_HOST)
            {
                //Indicate Error Message
                #ifdef DEBUG_MODE
                        UART2PrintString( "\r\n*****USB OTG B Error - SRP - Cannot Perform SRP while Role Is Host *****\r\n" ); 
                #endif
            }
             
            //Else if B side Device
            else if (USBOTGDefaultRoleIs() == ROLE_DEVICE && USBOTGCurrentRoleIs() == ROLE_DEVICE)
            {
                USBOTGRequestSession();
            }
        }

        //SW6 Suspends Host and Initiates Role Switch
        if (Switch6WasPressed() && !USBOTGRoleSwitch())
        {
            //If Current Role Is Host Then
            if (USBOTGCurrentRoleIs() == ROLE_HOST)
            {
                //Initiate Role Switch To Device
                USBOTGSelectRole(ROLE_DEVICE);
            }
            else
            {
                //Indicate Error Message
                #ifdef DEBUG_MODE
                    if (USBOTGDefaultRoleIs()== ROLE_HOST)
                    {   
                        UART2PrintString( "\r\n*****USB OTG A Error - Cannot Suspend - Current Role Is Device *****\r\n" ); 
                    }
                    else if (USBOTGDefaultRoleIs()== ROLE_DEVICE)
                    {
                         UART2PrintString( "\r\n*****USB OTG B Error - Cannot Suspend - Current Role Is Device *****\r\n" ); 
                    }
                #endif
            }     
        }

       //If Role Switch Occurred Then
       if (USBOTGRoleSwitch())
       {
           //Re-Initialize
           InitializeSystem();

           //Clear Role Switch Flag
           USBOTGClearRoleSwitch();
        }

        //If currently a Device and HNP is not Active Then
        if (USBOTGCurrentRoleIs() == ROLE_DEVICE && !USBOTGHnpIsActive())
        {
            //Call Device Tasks
            USBDeviceTasks();

             //If A Role Switch Happened While In DeviceTasks
            if (USBOTGCurrentRoleIs() == ROLE_HOST)
            {
                //Initialize Host Stack
                USBOTGInitializeHostStack();
            }
            else
            {
                //Call Process IO
                ProcessIO();
            }
        }
        
        //If currently a Host and HNP is not Active Then
        else if (USBOTGCurrentRoleIs() == ROLE_HOST && !USBOTGHnpIsActive()) 
        {
            //Call Host Tasks
            USBHostTasks();

            //If A Role Switch Happened While In HostTasks
            if (USBOTGCurrentRoleIs() == ROLE_DEVICE)
            {
                //Initialize Device Stack
                USBOTGInitializeDeviceStack();
            }
            else
            {
                //Call Manage Demo
                ManageDemoState();
            }
        }
    }//end while
}//end main


/********************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.                  
 *
 * Note:            None
 *******************************************************************/
void InitializeSystem(void)
{

    if (USBOTGCurrentRoleIs()== ROLE_DEVICE)
    {
        #if defined(__C30__) || defined __XC16__
            AD1PCFGL = 0xFFFF;
        #endif
        
    //	The USB specifications require that USB peripheral devices must never source
    //	current onto the Vbus pin.  Additionally, USB peripherals should not source
    //	current on D+ or D- when the host/hub is not actively powering the Vbus line.
    //	When designing a self powered (as opposed to bus powered) USB peripheral
    //	device, the firmware should make sure not to turn on the USB module and D+
    //	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
    //	firmware needs some means to detect when Vbus is being powered by the host.
    //	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
    // 	can be used to detect when Vbus is high (host actively powering), or low
    //	(host is shut down or otherwise not supplying power).  The USB firmware
    // 	can then periodically poll this I/O pin to know when it is okay to turn on
    //	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
    //	peripheral device, it is not possible to source current on D+ or D- when the
    //	host is not actively providing power on Vbus. Therefore, implementing this
    //	bus sense feature is optional.  This firmware can be made to use this bus
    //	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
    //	HardwareProfile.h file.    
        #if defined(USE_USB_BUS_SENSE_IO)
        tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
        #endif
        
    //	If the host PC sends a GetStatus (device) request, the firmware must respond
    //	and let the host know if the USB peripheral device is currently bus powered
    //	or self powered.  See chapter 9 in the official USB specifications for details
    //	regarding this request.  If the peripheral device is capable of being both
    //	self and bus powered, it should not return a hard coded value for this request.
    //	Instead, firmware should check if it is currently self or bus powered, and
    //	respond accordingly.  If the hardware has been configured like demonstrated
    //	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
    //	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
    //	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
    //	has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
    //	to it in HardwareProfile.h.
        #if defined(USE_SELF_POWER_SENSE_IO)
        tris_self_power = INPUT_PIN;	// See HardwareProfile.h
        #endif

        LCDErase();
        UserInit();
         
        SwitchState = IOPORT_BIT_6|IOPORT_BIT_7;
       
    }

    else if (USBOTGCurrentRoleIs() == ROLE_HOST)
    {
        #if defined(__C30__) || defined __XC16__
            // Init LEDs
            mInitAllLEDs();
            mLED_9_Off();
            mLED_10_Off();

            SwitchState = IOPORT_BIT_6|IOPORT_BIT_7;

            // Set Default demo state
            DemoState = DEMO_INITIALIZE;

        #elif defined(__PIC32MX__)
            // Init LEDs
            mInitAllLEDs();
            mLED_1_Off();
            mLED_2_Off();

             // Init Switches
            //mInitAllSwitches();
            SwitchState = IOPORT_BIT_6|IOPORT_BIT_7;

            // Set Default demo state
            DemoState = DEMO_INITIALIZE;
        #endif
    }
}//end InitializeSystem

// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:
	
	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is 
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause 
	//things to not work as intended.	
	

    #if defined(__C30__) || defined __XC16__
    #if 0
        U1EIR = 0xFFFF;
        U1IR = 0xFFFF;
        U1OTGIR = 0xFFFF;
        IFS5bits.USB1IF = 0;
        IEC5bits.USB1IE = 1;
        U1OTGIEbits.ACTVIE = 1;
        U1OTGIRbits.ACTVIF = 1;
        TRISA &= 0xFF3F;
        LATAbits.LATA6 = 1;
        Sleep();
        LATAbits.LATA6 = 0;
    #endif
    #endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
    #if !defined(self_powered)
        if(U1OTGIRbits.ACTVIF)
        {
            LATAbits.LATA7 = 1;
        
            IEC5bits.USB1IE = 0;
            U1OTGIEbits.ACTVIE = 0;
            IFS5bits.USB1IF = 0;
        
            //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
            USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
            //USBSuspendControl = 0;
            LATAbits.LATA7 = 0;
        }
    #endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// a few milliseconds of wakeup time, after which the device must be 
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.
	
	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *****************************************************************************/
void USBCBCheckOtherReq(void)
{
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *****************************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/******************************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *****************************************************************************/
void USBCBInitEP(void)
{
    USBEnableEndpoint(USBGEN_EP_NUM,USB_OUT_ENABLED|USB_IN_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    USBGenericOutHandle = USBGenRead(USBGEN_EP_NUM,(BYTE*)&OUTPacket,USBGEN_EP_SIZE);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior, 
 *                  as a USB device that has not been armed to perform remote 
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *                  
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are 
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex: 
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup. 
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in 
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    //First verify that the host has armed us to perform remote wakeup.
    //It does this by sending a SET_FEATURE request to enable remote wakeup,
    //usually just before the host goes to standby mode (note: it will only
    //send this SET_FEATURE request if the configuration descriptor declares
    //the device as remote wakeup capable, AND, if the feature is enabled
    //on the host (ex: on Windows based hosts, in the device manager 
    //properties page for the USB device, power management tab, the 
    //"Allow this device to bring the computer out of standby." checkbox 
    //should be checked).
    if(USBGetRemoteWakeupStatus() == TRUE) 
    {
        //Verify that the USB bus is in fact suspended, before we send
        //remote wakeup signalling.
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();
            
            //Clock switch to settings consistent with normal USB operation.
            USBCBWakeFromSuspend();
            USBSuspendControl = 0; 
            USBBusIsSuspended = FALSE;  //So we don't execute this code again, 
                                        //until a new suspend condition is detected.

            //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            //device must continuously see 5ms+ of idle on the bus, before it sends
            //remote wakeup signalling.  One way to be certain that this parameter
            //gets met, is to add a 2ms+ blocking delay here (2ms plus at 
            //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
            //5ms+ total delay since start of idle).
            delay_count = 3600U;        
            do
            {
                delay_count--;
            }while(delay_count);
            
            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1;       // Start RESUME signaling
            delay_count = 1800U;        // Set RESUME line for 1-13 ms
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0;       //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}

BOOL USB_ApplicationEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
        BYTE i;
    #endif

    // Handle specific events.
    switch ( (int)event )
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


/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch( (int)event)
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was 
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was 
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT 
            //      endpoints).
            break;
        default:
            break;
    }      
    return TRUE; 
}


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
        //DemoState = DEMO_STATE_SEND_SET_LED;  //MR-For OTG Generic Demo
        DemoState = DEMO_STATE_GET_TEMPERATURE;
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
        DemoState = DEMO_STATE_GET_TEMPERATURE;
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

    DelayMs(1); // 1ms delay
    }

} // ManageDemoState

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

    sw3_state = PORTD & IOPORT_BIT_6;

    if( sw3_state != (SwitchState & IOPORT_BIT_6) )
    {
        SwitchState &= ~IOPORT_BIT_6;
        SwitchState |= sw3_state;

        DelayMs(10);

        if(SwitchState & IOPORT_BIT_6)
         {
		return TRUE;                // Was pressed
         }
	 
    }

    return FALSE;                       // Was not pressed

} // Switch3WasPressed

BOOL Switch6WasPressed(void)
{
    WORD sw6_state;

    sw6_state = PORTD & IOPORT_BIT_7;

    if( sw6_state != (SwitchState & IOPORT_BIT_7) )
    {
        SwitchState &= ~IOPORT_BIT_7;
        SwitchState |= sw6_state;

        DelayMs(10);

        if(SwitchState & IOPORT_BIT_7)
            return TRUE;                // Was pressed
    }

    return FALSE;                       // Was not pressed

}//end Switch6WasPressed

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


/** EOF main.c ***************************************************************/
