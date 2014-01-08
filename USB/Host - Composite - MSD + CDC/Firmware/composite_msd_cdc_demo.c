/******************************************************************************

   USB Composite Host MSD + CDC Demo

This is a simple demo to show how a embedded Composite host can be implemented.
This demo comprises of CDC host and MSD host. To run the host demo
"USB Device - Composite - MSD + CDC" demo is programmed on the device end.

This demo runs on an Explorer 16 (DM240001) with a PIC24FJ256GB110 (USB) PIM
(MA240014) and a USB PICtail Plus Daughter Board (AC164127).

The device end can be programmed on any of the development boards for which
"USB Device - Composite - MSD + CDC" demo is implemented . 

Program device demo board with "USB Device - Composite - MSD + CDC" demo firmware.
This demo acts as a device in this configuration. Program the host firmware on
the Explorer 16 board. The composite host demo works in two steps.

Step 1 - The MSD interface opens a file "test.txt" with text
         "This is from Composite Host." as the content of the file.

Step 2 - On pressing switch 'S6' on host i.e Explorer 16 board the demo send a
character on the CDC interface. The same character is echoed back by the device
firmware. On reception the character is displayed on the LCD display on the
Explorer 16 demo board.

Timer 3 is configured to poll the CDC IN tranfer request from the device.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       composite_msd_cdc_demo.c
* Dependencies:    None
* Processor:       PIC24FJ256GB110
* Compiler:        C30 v2.01
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

Author          Date    Comments
--------------------------------------------------------------------------------
ADG          15-Sep-2008 First release

*******************************************************************************/
//DOM-IGNORE-END
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "usb_config.h"
#include "LCDBlocking.h"
#include "USB/usb.h"
#include "USB/usb_host_cdc.h"
#include "USB/usb_host_cdc_interface.h"

#include "USB/usb_host_msd.h"
#include "USB/usb_host_msd_scsi.h"
#include "MDD File System/FSIO.h"

#if defined( __PIC32MX__ )
#endif

#define DEBUG_MODE
#ifdef DEBUG_MODE
    #include "uart2.h"
#endif
// *****************************************************************************
// *****************************************************************************
// Constants
// *****************************************************************************
// *****************************************************************************

// We are taking Timer 3  to schedule input report transfers

#define STOP_TIMER_IN_IDLE_MODE     0x2000
#define TIMER_SOURCE_INTERNAL       0x0000
#define TIMER_ON                    0x8000
#define GATED_TIME_DISABLED         0x0000
#define TIMER_16BIT_MODE            0x0000

#if defined( __C30__ ) || defined __XC16__
    #define TIMER_PRESCALER_1               0x0000
    #define TIMER_PRESCALER_8               0x0010
    #define TIMER_PRESCALER_64              0x0020
    #define TIMER_PRESCALER_256             0x0030
    #define TIMER_INTERRUPT_PRIORITY        0x0002
#elif defined( __PIC32MX__ )
    #define TIMER_PRESCALER_1               0x0000
    #define TIMER_PRESCALER_2               0x0010
    #define TIMER_PRESCALER_4               0x0020
    #define TIMER_PRESCALER_8               0x0030
    #define TIMER_PRESCALER_16              0x0040
    #define TIMER_PRESCALER_32              0x0050
    #define TIMER_PRESCALER_64              0x0060
    #define TIMER_PRESCALER_256             0x0070
#else
    #error No timer constants
#endif

#define PIC32MX_TIMER2_INTERRUPT            0x00000100
#define PIC32MX_TIMER3_INTERRUPT            0x00001000

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
     #elif defined(__dsPIC33EP512MU810__)||defined (__PIC24EP512GU810__)
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
// Data Structures
// *****************************************************************************
// *****************************************************************************

typedef enum _APPL_STATE
{
    DEMO_INITIALIZE,
    DEVICE_NOT_CONNECTED,
    DEVICE_CONNECTED, /* Device Enumerated  - Report Descriptor Parsed */
    READY_TO_TX_RX, 
    MSD_SAVE_TEST_FILE,
    MSD_SAVE_TEST_FILE_WAIT,
    CDC_GET_IN_DATA,
    CDC_GET_IN_DATA_WAIT,
    CDC_SEND_OUT_DATA,
    CDC_SEND_OUT_DATA_WAIT,
    ERROR_REPORTED      /* need to add application states for data interface */
} APPL_STATE;


// *****************************************************************************
// *****************************************************************************
// Internal Function Prototypes
// *****************************************************************************
// *****************************************************************************
void InitializeTimer( void );
BOOL InitializeSystem ( void );

// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************
#define MAX_ALLOWED_CURRENT             (500)         // Maximum power we can supply in mA

// Macro to fill a line of LCD data.
#define FILL_LCD_LINE(l,p) {int i; for(i=0;i<16;i++)LcdData[(l)-1][i]=(p)[i];}

#define MAX_NO_OF_IN_BYTES  64
#define MAX_NO_OF_OUT_BYTES 64 


// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

volatile APPL_STATE  APPL_Demo_State;

volatile BOOL deviceAttached;
// MSD Interface data
FSFILE * myFile;
BYTE     myData[512];
size_t   numBytes;

// CDC Interface data
BYTE     USB_CDC_IN_Data_Array[MAX_NO_OF_IN_BYTES];
BYTE     USB_CDC_OUT_Data_Array[MAX_NO_OF_OUT_BYTES];

BYTE     LCDIndex;
BYTE     LcdData[2][16]; // LCD Display data
WORD     SwitchState;    // Bitmap of Switch states (only 3 & 6 are used)
BOOL     displayOnce = FALSE;

// Initial data for the LCD display.
const char *LcdDeviceAttached[] = { "Composite Host  ","Device Attached "};
const char *LcdDeviceDetached[] = { "Composite Host  ","Device Detached "};

const char *LcdDeviceNotSupported[] = { "Attached Device","Not Supported"};

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
#elif defined(__dsPIC33EP512MU810__)||defined (__PIC24EP512GU810__)
   

    // Configure the device PLL to obtain 60 MIPS operation. The crystal
    // frequency is 8MHz. Divide 8MHz by 2, multiply by 60 and divide by
    // 2. This results in Fosc of 120MHz. The CPU clock frequency is
    // Fcy = Fosc/2 = 60MHz. Wait for the Primary PLL to lock and then
    // configure the auxilliary PLL to provide 48MHz needed for USB 
    // Operation.

	PLLFBD = 58;				/* M  = 60	*/
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

    // The dsPIC33EP512MU810 features Peripheral Pin
    // select. The following statements map UART2 to 
    // device pins which would connect to the the 
    // RX232 transciever on the Explorer 16 board.

     RPINR19 = 0;
     RPINR19 = 0x64;
     RPOR9bits.RP101R = 0x3;

   
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

    TRISA = 0x0000;
    TRISD = 0x00C0;

    // Init LCD
    LCDInit();

    // Init LEDs
    mInitAllLEDs();
    mLED_3_Off();
    mLED_4_Off();

    // Init Switches
    mInitAllSwitches();
    SwitchState = sw6;

    // Init UART
    UART2Init();

    InitializeTimer(); // start 10ms timer to schedule input reports

    // Set Default demo state

    return TRUE;
} // InitializeSystem



/*************************************************************************
 * Function:        Switch6WasPressed
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
BOOL Switch6WasPressed(void)
{
    WORD sw6_state;

    sw6_state = sw6;
  
    if( sw6_state != SwitchState)
    {
        SwitchState = sw6_state;

        if(SwitchState)
            return TRUE;                // Was pressed
    }

    return FALSE;                       // Was not pressed

} // Switch6WasPressed


/****************************************************************************
  Function:
     void InitializeTimer( void )

  Description:
    This function initializes the tick timer.  It configures and starts the
    timer, and enables the timer interrupt.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void InitializeTimer( void )
{
    WORD timerPeriod;

    #if defined( __C30__ ) || defined __XC16__
        IPC2bits.T3IP = TIMER_INTERRUPT_PRIORITY;
        IFS0bits.T3IF = 0;
        TMR3 = 0;
        
         // TIMER_PERIOD is equivalent to 10micro sec , appl needs to calculate
         // the timerPeriod from the expected BAUD Rate of the device.
         // timerPeriod = TIMER_PERIOD *(100000/Baudrate in Bytes perSec)
        timerPeriod = 40000;
            // adjust the timer presaclar if poll rate is too high
        // 20000 counts correspond to 10ms
        PR3 = timerPeriod;
        T3CON = TIMER_ON | STOP_TIMER_IN_IDLE_MODE | TIMER_SOURCE_INTERNAL |
                GATED_TIME_DISABLED | TIMER_16BIT_MODE | TIMER_PRESCALER;
        IEC0bits.T3IE = 1;

    #elif defined( __PIC32MX__ )

        // TIMER_PERIOD is equivalent to 10micro sec , appl needs to calculate
        // the timerPeriod from the expected BAUD Rate of the device.
        // timerPeriod = TIMER_PERIOD *(100000/Baudrate in Bytes perSec)
        timerPeriod = 40000;
        // adjust the timer presaclar if poll rate is too high
        // 20000 counts correspond to 10ms
        //OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_8, TIMER_PERIOD);
        OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_16, timerPeriod);

        // set up the timer interrupt with a priority of 2
        ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_2);
    #else
        #error Cannot initialize timer.
    #endif
    return;
}

void USBHostCDC_Clear_Out_DATA_Array(void)
{
    BYTE i;

    for(i=0;i<MAX_NO_OF_OUT_BYTES;i++)
        USB_CDC_OUT_Data_Array[i] = 0;
}


//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    switch( (INT)event )
    {
        case EVENT_VBUS_REQUEST_POWER:
            // The data pointer points to a byte that represents the amount of power
            // requested in mA, divided by two.  If the device wants too much power,
            // we reject it.
            if (((USB_VBUS_POWER_EVENT_DATA*)data)->current <= (MAX_ALLOWED_CURRENT / 2))
            {
                return TRUE;
            }
            else
            {
                UART2PrintString( "\r\n***** USB Error - device requires too much current *****\r\n" );
            }
            break;

        case EVENT_VBUS_RELEASE_POWER:
            // Turn off Vbus power.
            // The PIC24F with the Explorer 16 cannot turn off Vbus through software.
            return TRUE;
            break;

        case EVENT_HUB_ATTACH:
            UART2PrintString( "\r\n***** USB Error - hubs are not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            UART2PrintString( "\r\n***** USB Error - device is not supported *****\r\n" );
            // Init LCD message
            FILL_LCD_LINE(1, &LcdDeviceNotSupported[0][0]);
            FILL_LCD_LINE(2, &LcdDeviceNotSupported[1][0]);
            
            // display message
            LCDWriteLine(1, (char *)&LcdData[0][0] );
            LCDWriteLine(2, (char *)&LcdData[1][0] );
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

        case EVENT_DETACH:                   // USB cable has been detached (data: BYTE, address of device)
            displayOnce = FALSE;
            deviceAttached = FALSE;
            APPL_Demo_State = DEVICE_NOT_CONNECTED;
            return TRUE;
            break;

// CDC Specific events

        case EVENT_CDC_NONE:
        case EVENT_CDC_ATTACH:
        case EVENT_CDC_COMM_READ_DONE:
        case EVENT_CDC_COMM_WRITE_DONE:
        case EVENT_CDC_DATA_READ_DONE:
        case EVENT_CDC_DATA_WRITE_DONE:
            return TRUE;
            break;
        case EVENT_CDC_NAK_TIMEOUT:
                APPL_Demo_State = READY_TO_TX_RX;
                return TRUE;
            break;
        
        case EVENT_CDC_RESET:
            #ifndef MINIMUM_BUILD
                UART2PrintString( "API: CDC Reset performed.\r\n" );
            #endif
            return TRUE;
            break;

        default :
            break;
    }
    return FALSE;
}

//******************************************************************************
//******************************************************************************
// Main
//******************************************************************************
//******************************************************************************
BYTE ErrorDriver;
BYTE NumOfBytesRcvd;
BYTE counter;
int main (void)
{
    BYTE i;
    static unsigned char key = 96;
        
    // Initialize the processor and peripherals.
    if ( InitializeSystem() != TRUE )
    {
        UART2PrintString( "\r\n\r\nCould not initialize Demo App - system.  Halting.\r\n\r\n" );
        while (1);
    }
    // Initialize USB layers
    USBInitialize( 0 );
    APPL_Demo_State = DEMO_INITIALIZE;

    while(1)
    {
        USBTasks();

        switch(APPL_Demo_State)
        {
            case DEMO_INITIALIZE:
                         APPL_Demo_State = DEVICE_NOT_CONNECTED;
                         break;


            case DEVICE_NOT_CONNECTED:
                         USBTasks();
                         if(displayOnce == FALSE)
                         {
                            // Init LCD message
                            FILL_LCD_LINE(1, &LcdDeviceDetached[0][0]);
                            FILL_LCD_LINE(2, &LcdDeviceDetached[1][0]);
                         
                            // display message
                            LCDWriteLine(1, (char *)&LcdData[0][0] );
                            LCDWriteLine(2, (char *)&LcdData[1][0] );
                            displayOnce = TRUE;
                         }

                         if(USBHostCDC_ApiDeviceDetect() && USBHostMSDSCSIMediaDetect()) /* TRUE if device is enumerated without error */
                         {
                            APPL_Demo_State = DEVICE_CONNECTED;
                            displayOnce = FALSE;
                         }

                break;
            case DEVICE_CONNECTED:
                         deviceAttached = TRUE;
                         // Init LCD message
                         FILL_LCD_LINE(1, &LcdDeviceAttached[0][0]);
                         FILL_LCD_LINE(2, &LcdDeviceAttached[1][0]);

                         // display message
                         LCDWriteLine(1, (char *)&LcdData[0][0] );
                         LCDWriteLine(2, (char *)&LcdData[1][0] );
                         APPL_Demo_State = MSD_SAVE_TEST_FILE;
               break;
            case MSD_SAVE_TEST_FILE:
                         //See if the device is attached and in the right format
                         if(FSInit())
                         {
                             //  Opening a file in mode "w" will create the file if it doesn't
                             //  exist.  If the file does exist it will delete the old file
                             //  and create a new one that is blank.
                             myFile = FSfopen("test.txt","w");
                         
                             //Write some data to the new file.
                             FSfwrite("This is from Composite Host.",1,28,myFile);
                             
                             //Always make sure to close the file so that the data gets
                             //  written to the drive.
                             FSfclose(myFile);
                             APPL_Demo_State = READY_TO_TX_RX;
                         }
                   break;

            case CDC_GET_IN_DATA:
                         if(USBHostCDC_Api_Get_IN_Data(MAX_NO_OF_IN_BYTES, USB_CDC_IN_Data_Array))
                         {
                             APPL_Demo_State = CDC_GET_IN_DATA_WAIT;
                         }
                         else
                         { 
                             APPL_Demo_State = READY_TO_TX_RX;
                         }
                break;
            
            case CDC_GET_IN_DATA_WAIT:
                         if(USBHostCDC_ApiTransferIsComplete(&ErrorDriver,&NumOfBytesRcvd))
                         {   /* check for error */
                             if(!ErrorDriver)
                             {
                                 if(NumOfBytesRcvd > 0)
                                 {
                                    for(i=0; i<NumOfBytesRcvd; i++)
                                     {
                                        UART2PutChar(USB_CDC_IN_Data_Array[i]);
                                        // update LCD  Buffer
                                            if(LCDIndex >= 32)
                                             {
                                                LCDIndex = 0;
                                                // Clear Display
                                                FILL_LCD_LINE(1, "                  ");
                                                FILL_LCD_LINE(2, "                  ");
//                                              ClearLCDBuffers();
                                                LCDErase();
                                             }
                                        if(LCDIndex < 16)
                                         {
                                            LcdData[0][LCDIndex] = USB_CDC_IN_Data_Array[i];
                                            LCDIndex++;
                                         }
                                         else
                                         {
                                            LcdData[1][LCDIndex-16] = USB_CDC_IN_Data_Array[i];
                                            LCDIndex++;
                                         }
                                     }
                                         // display message
                                         LCDWriteLine(1, (char *)&LcdData[0][0] );
                                         LCDWriteLine(2, (char *)&LcdData[1][0] );
                                 }
                                 APPL_Demo_State = READY_TO_TX_RX;
                             }
                             else
                             {
                                 APPL_Demo_State = READY_TO_TX_RX;
                             }
                         }   
                break;

            case CDC_SEND_OUT_DATA:
                         if(USBHostCDC_Api_Send_OUT_Data(1,USB_CDC_OUT_Data_Array))
                         {
                             APPL_Demo_State = CDC_SEND_OUT_DATA_WAIT;

                         }
                         else
                         { 
                             APPL_Demo_State = READY_TO_TX_RX; //TODO : Check error
                         }
               break;

            case CDC_SEND_OUT_DATA_WAIT:
                         if(USBHostCDC_ApiTransferIsComplete(&ErrorDriver,&NumOfBytesRcvd))
                         {
                             USBHostCDC_Clear_Out_DATA_Array();    
                             APPL_Demo_State = READY_TO_TX_RX;
                         }
               break;
           case READY_TO_TX_RX:
                         if(Switch6WasPressed())
                         {
                             APPL_Demo_State = CDC_SEND_OUT_DATA;
                             USB_CDC_OUT_Data_Array[0] = key++;
                             if(key == 122)
                             {
                                 key = 96;
                             }
                         }
                break;

           default :
                   break;
            }
        }
}

/****************************************************************************
  Function:
    void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)

  Description:
    Timer ISR, used to update application state. If no transfers are pending
    new input request is scheduled.
  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/
#if defined( __C30__ ) || defined __XC16__
void __attribute__((__interrupt__, auto_psv)) _T3Interrupt( void )
#elif defined( __PIC32MX__ )
void __ISR(_TIMER_3_VECTOR, ipl2) _T3Interrupt(void)
#else
#error Cannot prototype timer interrupt
#endif
{
     #if defined( __C30__ ) || defined __XC16__
    if (IFS0bits.T3IF)
    #elif defined( __PIC32MX__ )
    if (IFS0 & PIC32MX_TIMER3_INTERRUPT)
    #else
    #error Cannot check timer interrupt
    #endif
    {
         // Clear the interrupt flag
        #if defined( __C30__ ) || defined __XC16__
            IFS0bits.T3IF   = 0;
        #elif defined( __PIC32MX__ )
            mT3ClearIntFlag();
        #else
            #error Cannot clear timer interrupt.
        #endif
        if(READY_TO_TX_RX == APPL_Demo_State)
        {
            APPL_Demo_State = CDC_GET_IN_DATA; // If no report is pending schedule new request
        }
    }
}
