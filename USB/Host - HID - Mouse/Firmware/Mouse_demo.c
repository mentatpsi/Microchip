/******************************************************************************

    USB Mouse Host Application Demo

Description:
    This file contains the basic USB Mouse application demo. Purpose of the demo
    is to demonstrate the capability of HID host . Any Low speed/Full Speed
    USB Mouse can be connected to the PICtail USB adapter along with 
    Explorer 16 demo board. This file schedules the HID ransfers, and interprets
    the report received from the mouse. X & Y axis coordinates, Left & Right Click
    received from the mouse are diaplayed on the the LCD display mounted on the
    Explorer 16 board. Demo gives a fair idea of the HID host and user should be
    able to incorporate necessary changes for the required application.
    Below is the format in which the information received from mouse is displayed
    on the LCD didsplay.
     ----------------
    |X: 0x--  Y: 0x--|
    |LFT: -   RHT: - |
     ----------------

Summary:
 This file contains the basic USB mouse application.

Remarks:
    This demo requires Explorer 16 board and the USB PICtail plus connector.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       Mouse_demo.c
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

*******************************************************************************

 Change History:
  Rev    Description
  ----   -----------
  2.6a   fixed bug in LCDDisplayString() that could cause the string print to
         terminate early.

*******************************************************************************/
//DOM-IGNORE-END
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "usb_config.h"
#include "lcd_demo.h"
#include "USB/usb.h"
#include "USB/usb_host_hid_parser.h"
#include "USB/usb_host_hid.h"

#if defined(__dsPIC33EP512MU810__)||defined(__PIC24EP512GU810__)

/* While using the dsPIC33EPxx USB PIM
 * the LCD on the Explorer 16 board may
 * not function as expected. The demo 
 * uses the Explorer 16 UART connector
 * to display data. Connect the UART port
 * to a PC COM port and run hyperterminal
 * at 57600 baud. */
#define DEBUG_MODE

#endif

//#define DEBUG_MODE
// *****************************************************************************
// *****************************************************************************
// Constants
// *****************************************************************************
// *****************************************************************************

// We are taking Timer 3  to schedule input report transfers

// NOTE - The datasheet doesn't state this, but the timer does get reset to 0
// after a period register match.  So we don't have to worry about resetting
// the timer manually.

#define STOP_TIMER_IN_IDLE_MODE     0x2000
#define TIMER_SOURCE_INTERNAL       0x0000
#define TIMER_ON                    0x8000
#define GATED_TIME_DISABLED         0x0000
#define TIMER_16BIT_MODE            0x0000

#define TIMER_PRESCALER_1           0x0000
#define TIMER_PRESCALER_8           0x0010
#define TIMER_PRESCALER_64          0x0020
#define TIMER_PRESCALER_256         0x0030
#define TIMER_INTERRUPT_PRIORITY    0x0001


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
    #else    
    #endif
#elif defined( __PIC32MX__ )
    #pragma config UPLLEN   = ON            // USB PLL Enabled
    #pragma config FPLLMUL  = MUL_20        // PLL Multiplier
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

typedef enum _APP_STATE
{
    DEVICE_NOT_CONNECTED,
    DEVICE_CONNECTED, /* Device Enumerated  - Report Descriptor Parsed */
    READY_TO_TX_RX_REPORT,
    GET_INPUT_REPORT, /* perform operation on received report */
    INPUT_REPORT_PENDING,
    ERROR_REPORTED 
} APP_STATE;

typedef struct _HID_REPORT_BUFFER
{
    WORD  Report_ID;
    WORD  ReportSize;
//    BYTE* ReportData;
    BYTE  ReportData[4];
    WORD  ReportPollRate;
}   HID_REPORT_BUFFER;

// *****************************************************************************
// *****************************************************************************
// Internal Function Prototypes
// *****************************************************************************
// *****************************************************************************
BYTE App_DATA2ASCII(BYTE a);
void AppInitialize(void);
BOOL AppGetParsedReportDetails(void);
void App_Detect_Device(void);
void App_ProcessInputReport(void);
BOOL USB_HID_DataCollectionHandler(void);

void LCDDisplayString(BYTE* data, BYTE lineNum);
void LCD_Display_Routine(BYTE data, BYTE HIDData);

// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************
#define MAX_ALLOWED_CURRENT             (500)         // Maximum power we can supply in mA
#define MINIMUM_POLL_INTERVAL           (0x0A)        // Minimum Polling rate for HID reports is 10ms

#define USAGE_PAGE_BUTTONS              (0x09)

#define USAGE_PAGE_GEN_DESKTOP          (0x01)


#define MAX_ERROR_COUNTER               (10)


#define LCD_LINE_ONE                    (1)
#define LCD_LINE_TWO                    (2)

// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

APP_STATE App_State_Mouse = DEVICE_NOT_CONNECTED;

HID_DATA_DETAILS Appl_Mouse_Buttons_Details;
HID_DATA_DETAILS Appl_XY_Axis_Details;

HID_REPORT_BUFFER  Appl_raw_report_buffer;

HID_USER_DATA_SIZE Appl_Button_report_buffer[3];
HID_USER_DATA_SIZE Appl_XY_report_buffer[3];

BYTE LCD_DATA_LINE_ONE[16] = "X: 0x00 Y: 0x00 ";
BYTE LCD_DATA_LINE_TWO[16] = "LFT: 0  RHT: 0  ";

BYTE ErrorDriver;
BYTE ErrorCounter;
BYTE NumOfBytesRcvd;

BOOL ReportBufferUpdated;
BOOL LED_Key_Pressed = FALSE;
BOOL DisplayConnectOnce = FALSE;
BOOL DisplayDeatachOnce = FALSE;

BYTE currCharPos;
BYTE FirstKeyPressed ;
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

		case EVENT_HID_RPT_DESC_PARSED:
			 #ifdef APPL_COLLECT_PARSED_DATA
			     return(APPL_COLLECT_PARSED_DATA());
		     #else
				 return TRUE;
			 #endif
			break;

        default:
            break;
    }
    return FALSE;
}

//******************************************************************************
//******************************************************************************
// Main
//******************************************************************************
//******************************************************************************


int main (void)
{
    BYTE i;

    #if defined (__C30__) || defined __XC16__
        #if defined(__dsPIC33EP512MU810__)||(__PIC24EP512GU810__)
    
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
            // PPS - Configure U2RX - put on pin 49 (RP10)
            RPINR19bits.U2RXR = 10;
            
            // PPS - Configure U2TX - put on pin 50 (RP17)
            RPOR8bits.RP17R = 5;
            
            OSCCON = 0x3302;    // Enable secondary oscillator
            CLKDIV = 0x0000;    // Set PLL prescaler (1:1)
            TRISD = 0x00C0;
        #endif
        #if defined(__PIC24FJ64GB004__)
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
    
        //Device switches over automatically to PLL output after PLL is locked and ready.
        #endif
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
        
        AD1PCFG = 0xFFFF;   // Set analog pins to digital.
        TRISF   = 0x00;
    TRISD = 0x00C0;
    #else
        #error Cannot initialize.
    #endif

    UART2Init();

       PMMODE = 0x03ff;
     // Enable PMP Module, No Address & Data Muxing,
     // Enable RdWr Port, Enable Enb Port, No Chip Select,
     // Select RdWr and Enb signals Active High
       PMCON = 0x8383;
     // Enable A0
       PMAEN = 0x0001;
       LCDInit();
       mInitAllLEDs();
    #ifdef DEBUG_MODE
        UART2PrintString( "\r\n\r\n***** Microchip Explorer " );
        UART2PrintString( "USB MOuse Host Demo " );
        UART2PrintString( " *****\r\n\r\n" );
    #endif

        
        // Initialize USB layers
        USBInitialize( 0 );
        while(1)
        {
            USBTasks();
            App_Detect_Device();
            switch(App_State_Mouse)
            {
                case DEVICE_NOT_CONNECTED:
                             USBTasks();
                             if(DisplayDeatachOnce == FALSE)
                             {
                                LCDClear();
                                LCDL1Home();
                                LCDDisplayString((BYTE*)"Device Detached ", LCD_LINE_ONE);
                                DisplayDeatachOnce = TRUE;
                             }
                             if(USBHostHID_ApiDeviceDetect()) /* True if report descriptor is parsed with no error */
                             {
                                App_State_Mouse = DEVICE_CONNECTED;
                                DisplayConnectOnce = FALSE;
                             }
                    break;
                case DEVICE_CONNECTED:
                           App_State_Mouse = READY_TO_TX_RX_REPORT;
                           if(DisplayConnectOnce == FALSE)
                           {
                              LCDClear();
                              LCDL1Home();
                              LCDDisplayString((BYTE*)"Explorer16 Board", LCD_LINE_ONE);
                              LCDDisplayString((BYTE*)"USB HIDHost Demo", LCD_LINE_TWO);
                              DisplayConnectOnce = TRUE;
                              DisplayDeatachOnce = FALSE;
                           }

                    break;
                case READY_TO_TX_RX_REPORT:
                             if(!USBHostHID_ApiDeviceDetect())
                             {
                                App_State_Mouse = DEVICE_NOT_CONNECTED;
                             }
                             else
                             {
                                App_State_Mouse = GET_INPUT_REPORT;
                             }

                    break;
                case GET_INPUT_REPORT:
                            if(USBHostHID_ApiGetReport(Appl_raw_report_buffer.Report_ID,0,
                                                        Appl_raw_report_buffer.ReportSize, Appl_raw_report_buffer.ReportData))
                            {
                               /* Host may be busy/error -- keep trying */
                            }
                            else
                            {
                                App_State_Mouse = INPUT_REPORT_PENDING;
                            }
                            USBTasks();
                    break;
                case INPUT_REPORT_PENDING:
                           if(USBHostHID_ApiTransferIsComplete(&ErrorDriver,&NumOfBytesRcvd))
                            {
                                if(ErrorDriver ||(NumOfBytesRcvd != Appl_raw_report_buffer.ReportSize ))
                                {
                                  ErrorCounter++ ; 
                                  if(MAX_ERROR_COUNTER <= ErrorDriver)
                                     App_State_Mouse = ERROR_REPORTED;
                                  else
                                     App_State_Mouse = READY_TO_TX_RX_REPORT;
                                }
                                else
                                {
                                  ErrorCounter = 0; 
                                  ReportBufferUpdated = TRUE;
                                  App_State_Mouse = READY_TO_TX_RX_REPORT;

                                  if(DisplayConnectOnce == TRUE)
                                    {
                                        for(i=0;i<Appl_raw_report_buffer.ReportSize;i++)
                                        {
                                            if(Appl_raw_report_buffer.ReportData[i] != 0)
                                            {
                                                LCDClear();
                                                LCDL1Home();
                                                DisplayConnectOnce = FALSE;
                                            }
                                        }
                                    }

                                  App_ProcessInputReport();
                                }
                            }
                    break;

               case ERROR_REPORTED:
                    break;
                default:
                    break;

            }
        }
}

/****************************************************************************
  Function:
    void App_ProcessInputReport(void)

  Description:
    This function processes input report received from HID device.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
***************************************************************************/
void App_ProcessInputReport(void)
{
    BYTE  data;
   /* process input report received from device */
    USBHostHID_ApiImportData(Appl_raw_report_buffer.ReportData, Appl_raw_report_buffer.ReportSize
                          ,Appl_Button_report_buffer, &Appl_Mouse_Buttons_Details);
    USBHostHID_ApiImportData(Appl_raw_report_buffer.ReportData, Appl_raw_report_buffer.ReportSize
                          ,Appl_XY_report_buffer, &Appl_XY_Axis_Details);

 // X-axis
    data = (Appl_XY_report_buffer[0] & 0xF0) >> 4;
    LCD_DATA_LINE_ONE[5] = App_DATA2ASCII(data);
    data = (Appl_XY_report_buffer[0] & 0x0F);
    LCD_DATA_LINE_ONE[6] = App_DATA2ASCII(data);

 // Y-axis
    data = (Appl_XY_report_buffer[1] & 0xF0) >> 4;
    LCD_DATA_LINE_ONE[13] = App_DATA2ASCII(data);
    data = (Appl_XY_report_buffer[1] & 0x0F);
    LCD_DATA_LINE_ONE[14] = App_DATA2ASCII(data);
    
    if(Appl_Button_report_buffer[0] == 1)
    {
        if(LCD_DATA_LINE_TWO[5] == '0')
            LCD_DATA_LINE_TWO[5] =  '1';
        else
            LCD_DATA_LINE_TWO[5] =  '0';
    }
    if(Appl_Button_report_buffer[1] == 1)
    {
        if(LCD_DATA_LINE_TWO[13] == '0')
            LCD_DATA_LINE_TWO[13] =  '1';
        else
            LCD_DATA_LINE_TWO[13] =  '0';
    }

    LCDDisplayString((BYTE*)LCD_DATA_LINE_ONE, LCD_LINE_ONE);
    LCDDisplayString((BYTE*)LCD_DATA_LINE_TWO, LCD_LINE_TWO);

    #ifdef DEBUG_MODE
    {
        unsigned int i;

        UART2PrintString( "\n\rHID: Raw Report  " );
        for(i=0;i<(Appl_raw_report_buffer.ReportSize);i++)
        {
            UART2PutHex( Appl_raw_report_buffer.ReportData[i]);
            UART2PrintString( "-" );
        }
        UART2PrintString( "\n\r  Left Bt :  " ); UART2PutHex( Appl_Button_report_buffer[0]);
        UART2PrintString( "\n\r  Right Bt :  " ); UART2PutHex( Appl_Button_report_buffer[1]);
    
        UART2PrintString( "\n\r  X-Axis :  " ); UART2PutHex( Appl_XY_report_buffer[0]);
        UART2PrintString( "\n\r  Y-Axis :  " ); UART2PutHex( Appl_XY_report_buffer[1]);

    }
    #endif

}

/****************************************************************************
  Function:
    BYTE App_HID2ASCII(BYTE a)
  Description:
    This function converts the HID code of the key pressed to coressponding
    ASCII value. For Key strokes like Esc, Enter, Tab etc it returns 0.

  Precondition:
    None

  Parameters:
    BYTE a          -   HID code for the key pressed

  Return Values:
    BYTE            -   ASCII code for the key pressed

  Remarks:
    None
***************************************************************************/
BYTE App_DATA2ASCII(BYTE a) //convert USB HID code (buffer[2 to 7]) to ASCII code
{
   if(a<=0x9)
    {
       return(a+0x30);
    }

   if(a>=0xA && a<=0xF) 
    {
       return(a+0x37);
    }

   return(0);
}


/****************************************************************************
  Function:
    void App_Detect_Device(void)

  Description:
    This function monitors the status of device connected/disconnected

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
***************************************************************************/
void App_Detect_Device(void)
{
  if(!USBHostHID_ApiDeviceDetect())
  {
     App_State_Mouse = DEVICE_NOT_CONNECTED;
  }
}



/****************************************************************************
  Function:
    void LCD_Display_Routine(BYTE data , BYTE HIDData)
  Description:
    This function displays the key strokes on the LCD  mounted on Explorer16
    demo board. 

  Precondition:
    None

  Parameters:
    BYTE data       -   ASCII code for the key pressed
    BYTE HIDData    -   HID code for the key pressed, this is needed to take
                        action for keys like Esc, Enter, Tab etc.

  Return Values:
    None

  Remarks:
***************************************************************************/

void LCD_Display_Routine(BYTE data , BYTE HIDData)
{
    BYTE LineNum;
    BYTE CharPos;
    
    LineNum = ((currCharPos & 0x30) >> 4);

    if((LineNum == 1) && (CharPos == 0x0))
    {
        LCDL2Home();
        currCharPos = 0x10;
    }else if((LineNum == 2) && (CharPos == 0x0))
    {
        LCDClear();
        LCDL1Home();
        currCharPos = 0;
    }

   if(currCharPos > 0x20)
    {
        LCDClear();
        LCDL1Home();
        currCharPos = 0;
    }
   
}

/****************************************************************************
  Function:
    void LCDDisplayString(BYTE* data, BYTE lineNum)
  Description:
    This function displays the string on the LCD 

  Precondition:
    None

  Parameters:
    BYTE* data      -   Array of characters to be displayed on the LCD
    BYTE lineNum    -   LCD_LINE_ONE : To display on Line one to the LCD
                        LCD_LINE_TWO : To display on Line two to the LCD

  Return Values:
    None

  Remarks:
***************************************************************************/
void LCDDisplayString(BYTE* data, BYTE lineNum)
{
   BYTE index = 0;
   if(lineNum == 1)
    {
      LCDL1Home();
      while((*data != '\0') && (index < 16))
      {
         LCDPut(*data );
         index++;
         data++;
      }
    }
    if(lineNum == 2)
    {
      LCDL2Home();
      while((*data != '\0') && (index < 16))
      {
         LCDPut(*data );
         index++;
         data++;
      }
    }
}


/****************************************************************************
  Function:
    BOOL USB_HID_DataCollectionHandler(void)
  Description:
    This function is invoked by HID client , purpose is to collect the 
    details extracted from the report descriptor. HID client will store
    information extracted from the report descriptor in data structures.
    Application needs to create object for each report type it needs to 
    extract.
    For ex: HID_DATA_DETAILS Appl_ModifierKeysDetails;
    HID_DATA_DETAILS is defined in file usb_host_hid_appl_interface.h
    Each member of the structure must be initialized inside this function.
    Application interface layer provides functions :
    USBHostHID_ApiFindBit()
    USBHostHID_ApiFindValue()
    These functions can be used to fill in the details as shown in the demo
    code.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    TRUE    - If the report details are collected successfully.
    FALSE   - If the application does not find the the supported format.

  Remarks:
    This Function name should be entered in the USB configuration tool
    in the field "Parsed Data Collection handler".
    If the application does not define this function , then HID cient 
    assumes that Application is aware of report format of the attached
    device.
***************************************************************************/
BOOL USB_HID_DataCollectionHandler(void)
{
  BYTE NumOfReportItem = 0;
  BYTE i;
  USB_HID_ITEM_LIST* pitemListPtrs;
  USB_HID_DEVICE_RPT_INFO* pDeviceRptinfo;
  HID_REPORTITEM *reportItem;
  HID_USAGEITEM *hidUsageItem;
  BYTE usageIndex;
  BYTE reportIndex;

  pDeviceRptinfo = USBHostHID_GetCurrentReportInfo(); // Get current Report Info pointer
  pitemListPtrs = USBHostHID_GetItemListPointers();   // Get pointer to list of item pointers

  BOOL status = FALSE;
   /* Find Report Item Index for Modifier Keys */
   /* Once report Item is located , extract information from data structures provided by the parser */
   NumOfReportItem = pDeviceRptinfo->reportItems;
   for(i=0;i<NumOfReportItem;i++)
    {
       reportItem = &pitemListPtrs->reportItemList[i];
       if((reportItem->reportType==hidReportInput) && (reportItem->dataModes == (HIDData_Variable|HIDData_Relative))&&
           (reportItem->globals.usagePage==USAGE_PAGE_GEN_DESKTOP))
        {
           /* We now know report item points to modifier keys */
           /* Now make sure usage Min & Max are as per application */
            usageIndex = reportItem->firstUsageItem;
            hidUsageItem = &pitemListPtrs->usageItemList[usageIndex];

            reportIndex = reportItem->globals.reportIndex;
            Appl_XY_Axis_Details.reportLength = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
            Appl_XY_Axis_Details.reportID = (BYTE)reportItem->globals.reportID;
            Appl_XY_Axis_Details.bitOffset = (BYTE)reportItem->startBit;
            Appl_XY_Axis_Details.bitLength = (BYTE)reportItem->globals.reportsize;
            Appl_XY_Axis_Details.count=(BYTE)reportItem->globals.reportCount;
            Appl_XY_Axis_Details.interfaceNum= USBHostHID_ApiGetCurrentInterfaceNum();
        }
        else if((reportItem->reportType==hidReportInput) && (reportItem->dataModes == HIDData_Variable)&&
           (reportItem->globals.usagePage==USAGE_PAGE_BUTTONS))
        {
           /* We now know report item points to modifier keys */
           /* Now make sure usage Min & Max are as per application */
            usageIndex = reportItem->firstUsageItem;
            hidUsageItem = &pitemListPtrs->usageItemList[usageIndex];

            reportIndex = reportItem->globals.reportIndex;
            Appl_Mouse_Buttons_Details.reportLength = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
            Appl_Mouse_Buttons_Details.reportID = (BYTE)reportItem->globals.reportID;
            Appl_Mouse_Buttons_Details.bitOffset = (BYTE)reportItem->startBit;
            Appl_Mouse_Buttons_Details.bitLength = (BYTE)reportItem->globals.reportsize;
            Appl_Mouse_Buttons_Details.count=(BYTE)reportItem->globals.reportCount;
            Appl_Mouse_Buttons_Details.interfaceNum= USBHostHID_ApiGetCurrentInterfaceNum();
        }
    }

   if(pDeviceRptinfo->reports == 1)
    {
        Appl_raw_report_buffer.Report_ID = 0;
        Appl_raw_report_buffer.ReportSize = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
//        Appl_raw_report_buffer.ReportData = (BYTE*)malloc(Appl_raw_report_buffer.ReportSize);
        Appl_raw_report_buffer.ReportPollRate = pDeviceRptinfo->reportPollingRate;
        status = TRUE;
    }

    return(status);
}

