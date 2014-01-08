/*******************************************************************

	main.c - PRGA level demonstration program

	P. R. Glassel & Associates
	30255 Fir Trail
	Stacy, MN 55079
	Web:	www.prga.com
	Email:	sales@prga.com
	Ph:		651-408-9007

	Abstract:
	This demonstration application implements a bubble-level using 
	the accelerometer and display of the Microchip Pic18 Starter
	Kit.  

	Author:	J. W. Hartley
	Revision:	13-Apr-10	Initial edition - Modified from Microchip HID demo  
				15-Jun-10	Update comments, Released internally for review
				18-Jun-10	Update comments, add additional shift function
				21-Jun-10	Update comments
********************************************************************/

/********************************************************************
 FileName:     main.c
 Dependencies: See INCLUDES section
 Processor:   PIC18 or PIC24 USB Microcontrollers
 Hardware:    The code is natively intended to be used on the following
        hardware platforms: PICDEM™ FS USB Demo Board, 
        PIC18F87J50 FS USB Plug-In Module, or
        Explorer 16 + PIC24 USB PIM.  The firmware may be
        modified for use on other USB platforms by editing the
        HardwareProfile.h file.
 Complier:    Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:   Microchip Technology, Inc.

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
				

#ifndef USBMOUSE_C
#define USBMOUSE_C

/** INCLUDES *******************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "./USB/usb.h"

#include "HardwareProfile.h"	// this includes "HardwareProfile - PIC18F Starter Kit.h"

#include "./USB/usb_function_hid.h"

#include "mtouch.h"

#include "BMA150.h"

#include "oled.h"

#include "soft_start.h"

// PRGA Prototypes 
#include "PRGABMA150.h"
#include "PRGAoled.h"

/** CONFIGURATION **************************************************/

   //Watchdog Timer Enable bit:
     #pragma config WDTEN = OFF          //WDT disabled (control is placed on SWDTEN bit)
   //PLL Prescaler Selection bits:
     #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
   //Stack Overflow/Underflow Reset Enable bit:
     #pragma config STVREN = ON            //Reset on stack overflow/underflow enabled
   //Extended Instruction Set Enable bit:
     #pragma config XINST = OFF          //Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
   //CPU System Clock Postscaler:
     #pragma config CPUDIV = OSC1        //No CPU system clock divide
   //Code Protection bit:
     #pragma config CP0 = OFF            //Program memory is not code-protected
   //Oscillator Selection bits:
     #pragma config OSC = ECPLL          //HS oscillator, PLL enabled, HSPLL used by USB
   //Secondary Clock Source T1OSCEN Enforcement:
     #pragma config T1DIG = ON           //Secondary Oscillator clock source may be selected
   //Low-Power Timer1 Oscillator Enable bit:
     #pragma config LPT1OSC = OFF        //Timer1 oscillator configured for higher power operation
   //Fail-Safe Clock Monitor Enable bit:
     #pragma config FCMEN = OFF           //Fail-Safe Clock Monitor disabled
   //Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit:
     #pragma config IESO = OFF           //Two-Speed Start-up disabled
   //Watchdog Timer Postscaler Select bits:
     #pragma config WDTPS = 32768        //1:32768
   //DSWDT Reference Clock Select bit:
     #pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as reference clock
   //RTCC Reference Clock Select bit:
     #pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as reference clock
   //Deep Sleep BOR Enable bit:
     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep (does not affect operation in non-Deep Sleep modes)
   //Deep Sleep Watchdog Timer Enable bit:
     #pragma config DSWDTEN = OFF        //Disabled
   //Deep Sleep Watchdog Timer Postscale Select bits:
     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
   //IOLOCK One-Way Set Enable bit:
     #pragma config IOL1WAY = OFF        //The IOLOCK bit (PPSCON<0>) can be set and cleared as needed
   //MSSP address mask:
     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
   //Write Protect Program Flash Pages:
     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
   //Write Protection End Page (valid when WPDIS = 0):
     #pragma config WPEND = PAGE_0       //Write/Erase protect Flash Memory pages starting at page 0 and ending with page WPFP[5:0]
   //Write/Erase Protect Last Page In User Flash bit:
     #pragma config WPCFG = OFF          //Write/Erase Protection of last page Disabled
   //Write Protect Disable bit:
     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored


/** VARIABLES ******************************************************/
BYTE old_sw2,old_sw3;
BOOL emulate_mode;
BYTE movement_length;
BYTE vector = 0;

unsigned char ReceivedDataBuffer[64];
unsigned char ToSendDataBuffer[64];

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;
unsigned char DemoIntroState = 0;

/*******  PRGA Level Demo Variables ******/
#define OLED_CENTER_X 64
#define OLED_CENTER_Y 32

short	PRGA_XLRaw_X;		// The current Raw 10-bit XL values 
short	PRGA_XLRaw_Y;

short	PRGA_XLOffset_X;	// Calculated offset from the Calibration
short	PRGA_XLOffset_Y;

short	PRGA_XLCal_L_X;		// These are the intermediate Calibration Values 
short	PRGA_XLCal_L_Y;		// These are stored seperately because the calibration is
short	PRGA_XLCal_R_X;		// un-ordered, i.e. L then R or R then L
short	PRGA_XLCal_R_Y;

BOOL	PRGA_L_Button;		// Left button Press - store 1st cal value
BOOL	PRGA_R_Button;		// Right button Press - store & complete 2nd cal value
BOOL	PRGA_LX_CalDone;	// LX Cal is done
BOOL	PRGA_RX_CalDone;	// RX Cal is done
BOOL	PRGA_LY_CalDone;	// LY Cal is done
BOOL	PRGA_RY_CalDone;	// RY Cal is done
BOOL	L_Button_Active;	// The "L" button is being pressed
BOOL	R_Button_Active;	// The "R" button is being pressed

char	PRGA_Radius;

short	PRGA_wtemp;
short	PRGA_FiltTemp;

WORD	PRGA_PotVoltage;


/** PRIVATE PROTOTYPES *********************************************/
static void InitializeSystem(void);
void ProcessIO(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
 
void DemoIntroduction(void);
BOOL CheckButtonPressed(void);

// Private Prototypes of Level-Demo added functions
void PRGAxlCal(void);
void PRGAxlDisplay(void);
void PRGADrawCircle( char X, char Y, char Radius  );
void PRGAExpandOctant( unsigned char X, unsigned char Y, char CenterX, char CenterY);
short PRGA_Div2N( short In, unsigned char N );

/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
  //On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
  //the reset, high priority interrupt, and low priority interrupt
  //vectors.  However, the current Microchip USB bootloader 
  //examples are intended to occupy addresses 0x00-0x7FF or
  //0x00-0xFFF depending on which bootloader is used.  Therefore,
   //the bootloader code remaps these vectors to new locations
  //as indicated below.  This remapping is only necessary if you
  //wish to program the hex file generated from this project with
  //the USB bootloader.  If no bootloader is used, edit the
  //usb_config.h file and comment out the following defines:
  //#define PROGRAMMABLE_WITH_SD_BOOTLOADER
  
  #if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)
    #define REMAPPED_RESET_VECTOR_ADDRESS     0xA000
    #define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS  0xA008
    #define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS 0xA018
  #else 
    #define REMAPPED_RESET_VECTOR_ADDRESS     0x00
    #define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS  0x08
    #define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS 0x18
  #endif
  
  #if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)
  extern void _startup (void);        // See c018i.c in your C18 compiler dir
  #pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
  void _reset (void)
  {
      _asm goto _startup _endasm
  }
  #endif
  #pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
  void Remapped_High_ISR (void)
  {
       _asm goto YourHighPriorityISRCode _endasm
  }
  #pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
  void Remapped_Low_ISR (void)
  {
       _asm goto YourLowPriorityISRCode _endasm
  }
  
  #if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)
  //Note: If this project is built while one of the bootloaders has
  //been defined, but then the output hex file is not programmed with
  //the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
  //As a result, if an actual interrupt was enabled and occured, the PC would jump
  //to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
  //executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
  //(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
  //would effective reset the application.
  
  //To fix this situation, we should always deliberately place a 
  //"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
  //"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
  //hex file of this project is programmed with the bootloader, these sections do not
  //get bootloaded (as they overlap the bootloader space).  If the output hex file is not
  //programmed using the bootloader, then the below goto instructions do get programmed,
  //and the hex file still works like normal.  The below section is only required to fix this
  //scenario.
  #pragma code HIGH_INTERRUPT_VECTOR = 0x08
  void High_ISR (void)
  {
       _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
  }
  #pragma code LOW_INTERRUPT_VECTOR = 0x18
  void Low_ISR (void)
  {
       _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
  }
  #endif  //end of "#if defined(||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER))"

  #pragma code
  
  
  //These are your actual interrupt handling routines.
  #pragma interrupt YourHighPriorityISRCode
  void YourHighPriorityISRCode()
  {
    //Check which interrupt flag caused the interrupt.
    //Service the interrupt
    //Clear the interrupt flag
    //Etc.
  
  } //This return will be a "retfie fast", since this is in a #pragma interrupt section 
  #pragma interruptlow YourLowPriorityISRCode
  void YourLowPriorityISRCode()
  {
    //Check which interrupt flag caused the interrupt.
    //Service the interrupt
    //Clear the interrupt flag
    //Etc.
  
  } //This return will be a "retfie", since this is in a #pragma interruptlow section 
#endif


/** DECLARATIONS ***************************************************/
#pragma code
#define ROM_STRING rom unsigned char*

/********************************************************************
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
void main(void)
{
    InitializeSystem();

    while(1)
    {
    // Check bus status and service USB interrupts.
        USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
                  // this function periodically.  This function will take care
                  // of processing and responding to SETUP transactions 
                  // (such as during the enumeration process when you first
                  // plug in).  USB hosts require that USB devices should accept
                  // and process SETUP packets in a timely fashion.  Therefore,
                  // when using polling, this function should be called 
                  // frequently (such as once about every 100 microseconds) at any
                  // time that a SETUP packet might reasonably be expected to
                  // be sent by the host to your device.  In most cases, the
                  // USBDeviceTasks() function does not take very long to
                  // execute (~50 instruction cycles) before it returns.
              

    // Application-specific tasks.
    // Application related code may be added here, or in the ProcessIO() function.
        ProcessIO();        
    }//end while
}//end main

void DemoIntroduction(void)
{
    switch(DemoIntroState)
    {
        case 0:
            ResetDevice();  
        
            FillDisplay(0x00);

            oledPutROMString((ROM_STRING)" PIC18F Starter Kit  ",0,0);
            oledPutROMString((ROM_STRING)"PRGA Level Demo v1.0 ",1,0);
            oledPutROMString((ROM_STRING)"Press Menu to proceed",6,0);

            DemoIntroState = 1;
            break;

        case 1:
            if(CheckButtonPressed() == TRUE)
            {
                DemoIntroState++;
            }
            break;

        case 2:
            FillDisplay(0x00);
            oledPutROMString((ROM_STRING)"In this demo, the    ",0,0);
            oledPutROMString((ROM_STRING)"  board will act like",1,0);
            oledPutROMString((ROM_STRING)"  a level            ",2,0);
            oledPutROMString((ROM_STRING)"                     ",3,0);
            oledPutROMString((ROM_STRING)"                     ",4,0);
            oledPutROMString((ROM_STRING)"                     ",5,0);
            oledPutROMString((ROM_STRING)"Press Menu to proceed",6,0);
            DemoIntroState = 3;
            break;

        case 3:
            if(CheckButtonPressed() == TRUE)
            {
                DemoIntroState++;
            }
            break;

        case 4:
            oledPutROMString((ROM_STRING)"Connect the board to ",0,0);
            oledPutROMString((ROM_STRING)"  the host computer  ",1,0);
            oledPutROMString((ROM_STRING)"  and run the        ",2,0);
            oledPutROMString((ROM_STRING)"  HID PnP Demo.exe   ",3,0);
            oledPutROMString((ROM_STRING)"                     ",4,0);
            oledPutROMString((ROM_STRING)"                     ",5,0);
            oledPutROMString((ROM_STRING)"Press Menu to proceed",6,0);
            DemoIntroState = 5;
            break;

        case 5:
            if(CheckButtonPressed() == TRUE)
            {
                DemoIntroState++;
            }
            break;

        case 6:
            oledPutROMString((ROM_STRING)"The PC application is",0,0);
            oledPutROMString((ROM_STRING)"  reading raw data   ",1,0);
            oledPutROMString((ROM_STRING)"  from the capacitive",2,0);
            oledPutROMString((ROM_STRING)"  pads, accelerometer",3,0);
            oledPutROMString((ROM_STRING)"  and potentiometer. ",4,0);
            oledPutROMString((ROM_STRING)"                     ",5,0);
            oledPutROMString((ROM_STRING)"Press Menu to proceed",6,0);
            DemoIntroState++;
            break;
        case 7:
            if(CheckButtonPressed() == TRUE)
            {
                DemoIntroState++;
            }
            break;
        case 8:
            oledPutROMString((ROM_STRING)"Hold the menu button ",0,0);
            oledPutROMString((ROM_STRING)"  to return to the   ",1,0);
            oledPutROMString((ROM_STRING)"  demo menu.         ",2,0);
            oledPutROMString((ROM_STRING)"To Calibrate         ",3,0);
            oledPutROMString((ROM_STRING)"  Press L or R       ",4,0);
            oledPutROMString((ROM_STRING)"                     ",5,0);
            oledPutROMString((ROM_STRING)"Press Menu to proceed",6,0);
            DemoIntroState++;		// this causes the DemoIntro to skip
            break;
       case 9:
            if(CheckButtonPressed() == TRUE)
            {
                DemoIntroState++;
            }
            break;
	   case 10:
            oledPutROMString((ROM_STRING)"Please open the .exe ",0,0);
            oledPutROMString((ROM_STRING)" program in the demo ",1,0);
            oledPutROMString((ROM_STRING)" directory.  Once    ",2,0);
            oledPutROMString((ROM_STRING)" complete,           ",3,0);
            oledPutROMString((ROM_STRING)"                     ",4,0);
            oledPutROMString((ROM_STRING)"Press L to start     ",5,0);
            oledPutROMString((ROM_STRING)"  Calibration        ",6,0);
			DemoIntroState++;		// this causes the DemoIntro to skip
       case 11:
            if(CheckButtonPressed() == TRUE)
            {
                DemoIntroState++;
            }
            break;
        default:
            break;
    }
}

BOOL  CheckButtonPressed(void)		// Check for an S1 switch closure
{
    static char buttonPressed = FALSE;
    static unsigned long buttonPressCounter = 0;

    if(PORTBbits.RB0 == 0)
    {
        if(buttonPressCounter++ > 7000) 
        {
            buttonPressCounter = 0;
            buttonPressed = TRUE;
        }
    }
    else
    {
        if(buttonPressed == TRUE)
        {
            if(buttonPressCounter == 0)
            {
                buttonPressed = FALSE;
                return TRUE;
            }
            else
            {
                buttonPressCounter--;
            }
        }
    }

    return FALSE;
}

/******************************************************************************
  Function:
    void BL_CheckLoaderEnabled( void )

  Description:
    This routine is called to determine if the boot loader should attempt to
    load a new application.

  Precondition:
	None

  Parameters:
    None

  Returns:
    None


  Remarks:
    This function must be implemented.
******************************************************************************/
void BL_CheckLoaderEnabled ( void )
{
#if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)

	if (CheckButtonPressed() == TRUE)	
	{
		_asm
		RESET
		_endasm
	}
	return;
#endif
}

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
static void InitializeSystem(void)
{
    #if (defined(__18CXX) & !defined(PIC18F87J50_PIM))
        ADCON1 |= 0x0F;                 // Default all pins to digital
    #elif defined(__C30__)
        AD1PCFGL = 0xFFFF;
    #elif defined(__C32__)
        AD1PCFG = 0xFFFF;
    #endif

    #if defined(PIC18F87J50_PIM) || defined(PIC18F46J50_PIM)
  //On the PIC18F87J50 Family of USB microcontrollers, the PLL will not power up and be enabled
  //by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
  //This allows the device to power up at a lower initial operating frequency, which can be
  //advantageous when powered from a source which is not gauranteed to be adequate for 48MHz
  //operation.  On these devices, user firmware needs to manually set the OSCTUNE<PLLEN> bit to
  //power up the PLL.
    {
        unsigned int pll_startup_counter = 600;
        OSCTUNEbits.PLLEN = 1;  //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
        while(pll_startup_counter--);
    }
    //Device switches over automatically to PLL output after PLL is locked and ready.
    #endif

    #if defined(PIC18F87J50_PIM)
  //Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
  //use the ANCONx registers to control this, which is different from other devices which
  //use the ADCON1 register for this purpose.
    WDTCONbits.ADSHR = 1;     // Select alternate SFR location to access ANCONx registers
    ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital
    WDTCONbits.ADSHR = 0;     // Select normal SFR locations
    #endif

    #if defined(PIC18F46J50_PIM)
  //Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
  //use the ANCONx registers to control this, which is different from other devices which
  //use the ADCON1 register for this purpose.
    ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital
    #endif
    
//  The USB specifications require that USB peripheral devices must never source
//  current onto the Vbus pin.  Additionally, USB peripherals should not source
//  current on D+ or D- when the host/hub is not actively powering the Vbus line.
//  When designing a self powered (as opposed to bus powered) USB peripheral
//  device, the firmware should make sure not to turn on the USB module and D+
//  or D- pull up resistor unless Vbus is actively powered.  Therefore, the
//  firmware needs some means to detect when Vbus is being powered by the host.
//  A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
//  can be used to detect when Vbus is high (host actively powering), or low
//  (host is shut down or otherwise not supplying power).  The USB firmware
//  can then periodically poll this I/O pin to know when it is okay to turn on
//  the USB module/D+/D- pull up resistor.  When designing a purely bus powered
//  peripheral device, it is not possible to source current on D+ or D- when the
//  host is not actively providing power on Vbus. Therefore, implementing this
//  bus sense feature is optional.  This firmware can be made to use this bus
//  sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
//  HardwareProfile.h file.    
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
    #endif
    
//  If the host PC sends a GetStatus (device) request, the firmware must respond
//  and let the host know if the USB peripheral device is currently bus powered
//  or self powered.  See chapter 9 in the official USB specifications for details
//  regarding this request.  If the peripheral device is capable of being both
//  self and bus powered, it should not return a hard coded value for this request.
//  Instead, firmware should check if it is currently self or bus powered, and
//  respond accordingly.  If the hardware has been configured like demonstrated
//  on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
//  currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
//  is used for this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
//  has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
//  to it in HardwareProfile.h.
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;  // See HardwareProfile.h
    #endif
    
    USBDeviceInit();  //usb_device.c.  Initializes USB module SFRs and firmware
              //variables to known states.
    UserInit();

}//end InitializeSystem



/******************************************************************************
 * Function:        void UserInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine should take care of all of the demo code
 *                  initialization that is required.
 *
 * Note:            
 *
 *****************************************************************************/
void UserInit(void)
{
	/* Initialize the mTouch library */
	mTouchInit();

	/* Call the mTouch callibration function */
	mTouchCalibrate();

	/* Initialize the accelerometer */
	InitBma150(); 
	PRGAInitBma150();		// Set up XL chip for PRGA Level Demo


	//initialize the variable holding the handle for the last
	// transmission
	USBOutHandle = 0;
	USBInHandle = 0;

	// initialize some flags for the Level demo button detection
	PRGA_L_Button=FALSE;
	PRGA_R_Button=FALSE;

	PRGA_XLRaw_X = 0;
	PRGA_XLRaw_Y = 0;

	L_Button_Active = FALSE;	// Flags for Key-Pressed lockout. The "L" button
	R_Button_Active = FALSE;	// The "R" button
}//end UserInit

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void ProcessIO(void)
{   
	if(DemoIntroState == 0xFF)
    {
	    BL_CheckLoaderEnabled();	// With the app running, has the MENU button been press?
    }								// if so, then restart the Boot Loader
	
    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	// Soft Start the APP_VDD
    if(AppPowerReady() == FALSE) return;

    DemoIntroduction();			// This cycles through the info screens

    if(!HIDRxHandleBusy(USBOutHandle))        //Check if data was received from the host.
    {   
		switch(ReceivedDataBuffer[0])       //Look at the data the host sent, to see what kind of application specific command it sent.
        {
            case 0x80:  //mTouch callibration command
		        mTouchCalibrate();
                break;

            case 0x20:  
                {
                  WORD potVoltage;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      /* Select ADC channel */
                      ADCON0bits.CHS = 4; 
          
                      /* Make sure A/D interrupt is not set */
                      PIR1bits.ADIF = 0; 
                      
                      /* Begin A/D conversion */
                      ADCON0bits.GO=1; 
                      //Wait for A/D convert complete
                      while(!PIR1bits.ADIF);
                    
                      /* Get the value from the A/D */
                      potVoltage = ADRES;
          
                      ToSendDataBuffer[0] = 0x20;
                      ToSendDataBuffer[1] = (BYTE)(potVoltage);
                      ToSendDataBuffer[2] = (BYTE)(potVoltage>>8);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);

						PRGA_PotVoltage = (potVoltage + PRGA_PotVoltage)>>1;
                  }         
                }
                break;

            case 0x30:  
                {
                  WORD w;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      w = mTouchReadButton(0);		// the "R" button

                      ToSendDataBuffer[0] = 0x30;
                      ToSendDataBuffer[1] = (BYTE)w;
                      ToSendDataBuffer[2] = (BYTE)(w>>8);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);


					  if(DemoIntroState<=10)		// ignore the calibration buttons until 
							break;					// the instructions have been given

					 if(L_Button_Active) break;		// the other button is active, so ignore this one

					  if(w>750)						// The R-button is not being pressed, have the buttons beem pressed and released? 
					  {
						R_Button_Active=FALSE;		// clear the R-button state
						PRGAxlCal();				// Do the calibration if complete
						break;
					  }

					  if((w<600)&&(!PRGA_R_Button)) 	// the R-button is down for the first time
					  {
						PRGA_XLCal_R_X = PRGA_XLRaw_X;		// Save the raw data
						PRGA_XLCal_R_Y = PRGA_XLRaw_Y;		// Save the raw data
						PRGA_R_Button=TRUE;					// signal that the R button is down

						if( !PRGA_L_Button )				// if the L-button is up
						{
							FillDisplay(0x00);		// Erase/Clear the display
			            	oledPutROMString((ROM_STRING)"                     ",0,0);
            				oledPutROMString((ROM_STRING)"                     ",1,0);
            				oledPutROMString((ROM_STRING)"                     ",2,0);
            				oledPutROMString((ROM_STRING)"Rotate the level     ",3,0);
            				oledPutROMString((ROM_STRING)"  180 degrees        ",4,0);
            				oledPutROMString((ROM_STRING)"  and press L        ",5,0);
            				oledPutROMString((ROM_STRING)"                     ",6,0);
						}
						R_Button_Active=TRUE;	// signal the R-button active, so that we don't get confused with the other switch
					  }
                  }
                }
                break;

            case 0x31:  
                {
                  WORD w;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      w = mTouchReadButton(1);		// the "Down" button/slider?

                      ToSendDataBuffer[0] = 0x31;
                      ToSendDataBuffer[1] = (BYTE)w;
                      ToSendDataBuffer[2] = (BYTE)(w>>8);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
                  }         
                }
                break;
            
      case 0x32:  
                {
                  WORD w;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      w = mTouchReadButton(2);		// the "UP" button/slider?

                      ToSendDataBuffer[0] = 0x32;
                      ToSendDataBuffer[1] = (BYTE)w;
                      ToSendDataBuffer[2] = (BYTE)(w>>8);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
                  }         
                }
                break;

            case 0x33:  
                {
                  WORD w;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      w = mTouchReadButton(3);		// the "L" button

                      ToSendDataBuffer[0] = 0x33;
                      ToSendDataBuffer[1] = (BYTE)w;
                      ToSendDataBuffer[2] = (BYTE)(w>>8);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);

					  if(DemoIntroState<=10)		// ignore the calibration buttons until 
							break;					// the instructions have been given 

					 if(R_Button_Active) break;		// the other button is active, so ignore this one

					  if(w>750)						// L-button not active
					  {
						L_Button_Active=FALSE;		// set the L-button OFF
						PRGAxlCal();				// Do the calibration if complete
						break;
					  }

					  if((w<600)&&(!PRGA_L_Button))			// this executes the cal when the button is pressed.
					  {
						PRGA_XLCal_L_X = PRGA_XLRaw_X;		// Save the raw data
						PRGA_XLCal_L_Y = PRGA_XLRaw_Y;		// Save the raw data
						PRGA_L_Button = TRUE;

						if( !PRGA_R_Button )
						{
							FillDisplay(0x00);		// Erase/Clear the display
			            	oledPutROMString((ROM_STRING)"Rotate the level     ",0,0);
            				oledPutROMString((ROM_STRING)"  180 degrees        ",1,0);
            				oledPutROMString((ROM_STRING)"  and press R        ",2,0);
            				oledPutROMString((ROM_STRING)"                     ",3,0);
            				oledPutROMString((ROM_STRING)"                     ",4,0);
            				oledPutROMString((ROM_STRING)"                     ",5,0);
            				oledPutROMString((ROM_STRING)"                     ",6,0);
						}			
						L_Button_Active=TRUE;	// signal the L-button active, so that we don't get confused with the other switch			
					  }  
				  }
                }
                break;

            case 0x40:  
                {
                  BMA150_REG regX_MSB;
                  BMA150_REG regX_LSB;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      regX_LSB.val = BMA150_ReadByte(BMA150_ACC_X_LSB);
                      regX_MSB.val = BMA150_ReadByte(BMA150_ACC_X_MSB);
          
                      ToSendDataBuffer[0] = 0x40;
                      ToSendDataBuffer[1] = (BYTE)(regX_LSB.val>>6);
                      ToSendDataBuffer[2] = (BYTE)(regX_MSB.val);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);

						// The Raw value is filtered, but not offset corrected
						// patch the 2 bytes from the XL together in a signed 16-bit value
						// The XL values are 10-bit, so we have 6 bits of head-room
						// Be sure to sign extend things.
						// Remember, the C18 compiler does not sign replicate on right bit-shifts (>>).

					  PRGA_wtemp = (char)ToSendDataBuffer[2] ;	// force "signed" conversion to promote the sign
					  PRGA_wtemp = (PRGA_wtemp<<2) + ToSendDataBuffer[1];	// add in the bottom bits

						// IIR LP filter: NewOut = (1/16 New + 15/16 LastOut)*16
						// The implementation maintains the output value as 16* to preserve the necessary precision				  
					  PRGA_FiltTemp = PRGA_Div2N(PRGA_XLRaw_X, 4);		// calculate Old from 16*Old

					  PRGA_XLRaw_X = PRGA_wtemp + PRGA_XLRaw_X - PRGA_FiltTemp ;
                  }         
                }
                break;

            case 0x41:  
                {
                  BMA150_REG regY_MSB;		// these are unsigned bytes
                  BMA150_REG regY_LSB;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      regY_LSB.val = BMA150_ReadByte(BMA150_ACC_Y_LSB);
                      regY_MSB.val = BMA150_ReadByte(BMA150_ACC_Y_MSB);
          
                      ToSendDataBuffer[0] = 0x41;
                      ToSendDataBuffer[1] = (BYTE)(regY_LSB.val>>6);
                      ToSendDataBuffer[2] = (BYTE)(regY_MSB.val);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);

						// The Raw value is filtered, but not offset corrected
						// patch the 2 bytes from the XL together in a signed 16-bit value
						// The XL values are 10-bit, so we have 6 bits of head-room
						// Be sure to sign extend things.
						// Remember, the C18 compiler does not sign replicate on right bit-shifts (>>).

					  PRGA_wtemp = (char)ToSendDataBuffer[2] ;	// to promote the sign
					  PRGA_wtemp = (PRGA_wtemp<<2) + ToSendDataBuffer[1];

						// IIR LP filter: NewOut = (1/16 New + 15/16 LastOut)*16
						// The implementation maintains the output value as 16* to preserve the necessary precision
					  PRGA_FiltTemp = PRGA_Div2N( PRGA_XLRaw_Y,4);		// calculate Old from 16*Old

					  PRGA_XLRaw_Y = PRGA_wtemp + PRGA_XLRaw_Y - PRGA_FiltTemp ;

					  if(DemoIntroState == 0xFF)
					  		PRGAxlDisplay();		// when we get a new Y value, update the Level display
                  }         
                }
                break;

            case 0x42:  
                {
                  BMA150_REG regZ_MSB;
                  BMA150_REG regZ_LSB;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      regZ_LSB.val = BMA150_ReadByte(BMA150_ACC_Z_LSB);
                      regZ_MSB.val = BMA150_ReadByte(BMA150_ACC_Z_MSB);
          
                      ToSendDataBuffer[0] = 0x42;
                      ToSendDataBuffer[1] = (BYTE)(regZ_LSB.val>>6);
                      ToSendDataBuffer[2] = (BYTE)(regZ_MSB.val);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
                  }         
                }
                break;

            case 0x43:  
                {
                  BMA150_REG regT_MSB;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      regT_MSB.val = BMA150_ReadByte(BMA150_TEMP);
          
                      ToSendDataBuffer[0] = 0x43;
                      ToSendDataBuffer[1] = (BYTE)regT_MSB.val;
                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
                  }         
                }
                break;

        }
        //Re-arm the OUT endpoint for the next packet
        USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
    }
}  //end ProcessIO

/***************************************************
// Trial right shift function #1

short PRGA_Div2N( short In, unsigned char N )	// This does signed 16-bit right-shifts
{												// length, optimized: 186 bytes
	short	Temp;

	Temp = In;					// copy to preserve the initial sign
	if( In<0 )
		Temp = -Temp;			// make Temp positive

	Temp = Temp>>N;				// the shift works for positive numbers

	if( In<0 )
		Temp = -Temp;			// correct the sign
	return( Temp );
}
****************************************************/

short	PRGA_Div2N(short In, unsigned char N)	// Another way to do the right-shifts
{												// length, optimized: 114 bytes
	WORD_BITS Temp;					// from GenericTypeDefs.h

	Temp.Val = In;					// copy to hide the conversions

	while( N !=0 )					// use a pre-test to avoid N=0 problems
	{
		N--;						// Decrement the shift count.
		Temp.Val = Temp.Val>>1;		// shift right 1-bit, bit-15=0
		if(Temp.bits.b14)			// if the sign bit is set,...
			Temp.bits.b15=1;		//   then propagate it
	}
	return( (short)Temp.Val );
}


//********************
//  Complete the XL callibration once both buttons - L and R - have been pressed
//********************  
void PRGAxlCal(void)
{
	if(PRGA_L_Button && PRGA_R_Button)
	{							// offset = average of the 180deg samples
		PRGA_XLOffset_X = PRGA_Div2N(PRGA_XLCal_L_X + PRGA_XLCal_R_X, 1) ;
		PRGA_XLOffset_Y = PRGA_Div2N(PRGA_XLCal_L_Y + PRGA_XLCal_R_Y, 1) ;

		PRGA_L_Button = FALSE;		// Clear the states for the buttons-pressed
		PRGA_R_Button = FALSE;

		DemoIntroState = 0xFF;	// start looking at the menu button to restart
	}
}


void PRGAxlDisplay(void)		// This displays the bubble at the current XL value 
{ 
	char	PRGA_bCenter_X;
	char	PRGA_bCenter_Y;
	unsigned char	x,y;

	if(PRGA_L_Button || PRGA_R_Button) return;	// Don't update display if doing a Calibration

	PRGA_Radius = 11;						// Set the Radius of the bubble

		// Since the display is only 128x64, byte values work for the display calculations

		// The Center of the bubble is defined by the calibrated XL value
		// the Display and the accelerometer are on different coordinate orientation. XL_X=DisplayY
		// also, the display is mirror-image to the XL

		// the XL gain is reduced from 16x to 2x and corrected for the calibration offset
 	PRGA_bCenter_Y = -PRGA_Div2N(PRGA_XLRaw_X - PRGA_XLOffset_X, 3);
	PRGA_bCenter_X = -PRGA_Div2N(PRGA_XLRaw_Y - PRGA_XLOffset_Y, 3);

	if( PRGA_bCenter_Y > 32 - PRGA_Radius)		// limit the Y inputs to the bubble
		PRGA_bCenter_Y = 32 - PRGA_Radius;
	else if (PRGA_bCenter_Y< -32 + PRGA_Radius )
		PRGA_bCenter_Y = -32 + PRGA_Radius;

	if(PRGA_bCenter_X > 64 - PRGA_Radius)		// limit the X inputs to the bubble 
		PRGA_bCenter_X = 64 - PRGA_Radius;
	else if (PRGA_bCenter_X< -64 + PRGA_Radius )
		PRGA_bCenter_X = -64 + PRGA_Radius;

	FillDisplay(0x00);										// Erase/Clear the display
	PRGAWritePixel( OLED_CENTER_X,  OLED_CENTER_Y, 0x01);	// put a dot at the center
	PRGADrawCircle( 0, 0, PRGA_Radius+3 );					// Now draw the reference circle

	for(x=0; x<128; x++) 						// Draw a square around the edge of the display
	{
		PRGAWritePixel( x,  0, 0x01);			// draw the left side
		PRGAWritePixel( x, 63, 0x01);			//   and right side
	}
	for(y=0; y<64; y++)
	{
		PRGAWritePixel( 0,    y, 0x01);			// draw the top
		PRGAWritePixel( 127,  y, 0x01);			//   and bottom
	}

	PRGADrawCircle( PRGA_bCenter_X, PRGA_bCenter_Y, PRGA_Radius  );	// draw the moving "bubble"
}

void PRGADrawCircle( char X, char Y, char Radius  )	// x,y is center of circle relative to center of display
{
	char	PRGA_bCirX;
	char	PRGA_bCirY;
	char	PRGA_bCirD;

			// Draw the circle -  VanAken/Bresenham algorithm
			// ref: Van Aken,J & Novak,M, "Curve-Drawing Algorithms for Raster Displays" 
			//		ACM Trans. Graphics, Vol 4,No. 2, April 1985, pp:147-169.
			// This computes an octant, then we mirror the values to complete the circle
			// the circle is drawn centered at 0,0 then translated to x,y on the display
	PRGA_bCirX = Radius;		// This is the initial point on the circle
	PRGA_bCirY = 0;
	PRGA_bCirD = 1 - Radius;
	do
	{
			//Translate the circle pixel from (0,0) to (0,0 + tilt + display-coord.) coordinates
		PRGAExpandOctant( PRGA_bCirX, PRGA_bCirY,  X, Y );		// this draws the pixels

		PRGA_bCirY++;					//increment Y for next point
		if(PRGA_bCirD<0)
			PRGA_bCirD += PRGA_bCirY + PRGA_bCirY;	// don't change X for next point
		else
		{
			PRGA_bCirX--;
			PRGA_bCirD += (PRGA_bCirY - PRGA_bCirX)<<1;
		}
		PRGA_bCirD++;
	}while(PRGA_bCirX>=PRGA_bCirY);
}

			//Draw the circle-octant pixels and their mirrors to complete the translated circle
void PRGAExpandOctant( unsigned char X, unsigned char Y, char CenterX, char CenterY)
{
	PRGAWritePixel( CenterX + X+OLED_CENTER_X, CenterY + Y+OLED_CENTER_Y, 0x01 );
	PRGAWritePixel( CenterX - X+OLED_CENTER_X, CenterY + Y+OLED_CENTER_Y, 0x01 );
	PRGAWritePixel( CenterX - X+OLED_CENTER_X, CenterY - Y+OLED_CENTER_Y, 0x01 );
	PRGAWritePixel( CenterX + X+OLED_CENTER_X, CenterY - Y+OLED_CENTER_Y, 0x01  );
	PRGAWritePixel( CenterX + Y+OLED_CENTER_X, CenterY + X+OLED_CENTER_Y, 0x01  );
	PRGAWritePixel( CenterX - Y+OLED_CENTER_X, CenterY + X+OLED_CENTER_Y, 0x01  );
	PRGAWritePixel( CenterX - Y+OLED_CENTER_X, CenterY - X+OLED_CENTER_Y, 0x01  );
	PRGAWritePixel( CenterX + Y+OLED_CENTER_X, CenterY - X+OLED_CENTER_Y, 0x01  );
}

//******** End of PRGA Level Demo functions


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
	//Turn off the App Vdd
	AppPowerDisable();
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
 *          In this example the interrupt is only used when the device
 *          goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
    #if !defined(self_powered)
        if(U1OTGIRbits.ACTVIF)
        {
            IEC5bits.USB1IE = 0;
            U1OTGIEbits.ACTVIE = 0;
            IFS5bits.USB1IF = 0;
        
            //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
            USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
            //USBSuspendControl = 0;
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
 *          suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *          mode, the host may wake the device back up by sending non-
 *          idle state signalling.
 *          
 *          This call back is invoked when a wakeup from USB suspend 
 *          is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	while(AppPowerReady() == FALSE)
	{
		// Soft Start the APP_VDD	
	}
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
 *          firmware must process the request and respond
 *          appropriately to fulfill the request.  Some of
 *          the SETUP packets will be for standard
 *          USB "chapter 9" (as in, fulfilling chapter 9 of
 *          the official USB specifications) requests, while
 *          others may be specific to the USB device class
 *          that is being implemented.  For example, a HID
 *          class device needs to be able to respond to
 *          "GET REPORT" type of requests.  This
 *          is not a standard USB chapter 9 request, and 
 *          therefore not handled by usb_device.c.  Instead
 *          this request should be handled by class specific 
 *          firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
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
 *          called when a SETUP, bRequest: SET_DESCRIPTOR request
 *          arrives.  Typically SET_DESCRIPTOR requests are
 *          not used in most applications, and it is
 *          optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
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
 *          SET_CONFIGURATION (wValue not = 0) request.  This 
 *          callback function should initialize the endpoints 
 *          for the device's usage according to the current 
 *          configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
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
 *          peripheral devices to wake up a host PC (such
 *          as if it is in a low power suspend to RAM state).
 *          This can be a very useful feature in some
 *          USB applications, such as an Infrared remote
 *          control receiver.  If a user presses the "power"
 *          button on a remote control, it is nice that the
 *          IR receiver can detect this signalling, and then
 *          send a USB "command" to the PC to wake up.
 *          
 *          The USBCBSendResume() "callback" function is used
 *          to send this special USB signalling which wakes 
 *          up the PC.  This function may be called by
 *          application firmware to wake up the PC.  This
 *          function should only be called when:
 *          
 *          1.  The USB driver used on the host PC supports
 *            the remote wakeup capability.
 *          2.  The USB configuration descriptor indicates
 *            the device is remote wakeup capable in the
 *            bmAttributes field.
 *          3.  The USB host PC is currently sleeping,
 *            and has previously sent your device a SET 
 *            FEATURE setup packet which "armed" the
 *            remote wakeup capability.   
 *
 *          This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            Interrupt vs. Polling
 *                  -Primary clock
 *                  -Secondary clock ***** MAKE NOTES ABOUT THIS *******
 *                   > Can switch to primary first by calling USBCBWakeFromSuspend()
 
 *                  The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
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
    
    USBResumeControl = 1;                // Start RESUME signaling
    
    delay_count = 1800U;                // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}


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
    switch(event)
    {
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
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
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }      
    return TRUE; 
}
/** EOF main.c *************************************************/
#endif
