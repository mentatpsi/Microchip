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
#include "./USB/usb.h"
#include "HardwareProfile.h"
#include "./USB/usb_function_hid.h"
#include "mtouch.h"
#include "BMA150.h"
#include "oled.h"
#include "soft_start.h"

/** CONFIGURATION **************************************************/
#if defined(PICDEM_FS_USB)      // Configuration bits for PICDEM FS USB Demo Board (based on PIC18F4550)
        #pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
        #pragma config CPUDIV   = OSC1_PLL2   
        #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
        #pragma config FOSC     = HSPLL_HS
        #pragma config FCMEN    = OFF
        #pragma config IESO     = OFF
        #pragma config PWRT     = OFF
        #pragma config BOR      = ON
        #pragma config BORV     = 3
        #pragma config VREGEN   = ON      //USB Voltage Regulator
        #pragma config WDT      = OFF
        #pragma config WDTPS    = 32768
        #pragma config MCLRE    = ON
        #pragma config LPT1OSC  = OFF
        #pragma config PBADEN   = OFF
//      #pragma config CCP2MX   = ON
        #pragma config STVREN   = ON
        #pragma config LVP      = OFF
//      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
        #pragma config XINST    = OFF       // Extended Instruction Set
        #pragma config CP0      = OFF
        #pragma config CP1      = OFF
//      #pragma config CP2      = OFF
//      #pragma config CP3      = OFF
        #pragma config CPB      = OFF
//      #pragma config CPD      = OFF
        #pragma config WRT0     = OFF
        #pragma config WRT1     = OFF
//      #pragma config WRT2     = OFF
//      #pragma config WRT3     = OFF
        #pragma config WRTB     = OFF       // Boot Block Write Protection
        #pragma config WRTC     = OFF
//      #pragma config WRTD     = OFF
        #pragma config EBTR0    = OFF
        #pragma config EBTR1    = OFF
//      #pragma config EBTR2    = OFF
//      #pragma config EBTR3    = OFF
        #pragma config EBTRB    = OFF


#elif defined(PIC18F87J50_PIM)        // Configuration bits for PIC18F87J50 FS USB Plug-In Module board
        #pragma config XINST    = OFF     // Extended instruction set
        #pragma config STVREN   = ON        // Stack overflow reset
        #pragma config PLLDIV   = 3         // (12 MHz crystal used on this board)
        #pragma config WDTEN    = OFF       // Watch Dog Timer (WDT)
        #pragma config CP0      = OFF       // Code protect
        #pragma config CPUDIV   = OSC1      // OSC1 = divide by 1 mode
        #pragma config IESO     = OFF       // Internal External (clock) Switchover
        #pragma config FCMEN    = OFF       // Fail Safe Clock Monitor
        #pragma config FOSC     = HSPLL     // Firmware must also set OSCTUNE<PLLEN> to start PLL!
        #pragma config WDTPS    = 32768
//      #pragma config WAIT     = OFF       // Commented choices are
//      #pragma config BW       = 16        // only available on the
//      #pragma config MODE     = MM        // 80 pin devices in the 
//      #pragma config EASHFT   = OFF       // family.
        #pragma config MSSPMSK  = MSK5
//      #pragma config PMPMX    = DEFAULT
//      #pragma config ECCPMX   = DEFAULT
        #pragma config CCP2MX   = DEFAULT   

#elif defined(PIC18F46J50_PIM)
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

   
#elif defined(LOW_PIN_COUNT_USB_DEVELOPMENT_KIT)
        #pragma config CPUDIV = NOCLKDIV
        #pragma config USBDIV = OFF
        #pragma config FOSC   = HS
        #pragma config PLLEN  = ON
        #pragma config FCMEN  = OFF
        #pragma config IESO   = OFF
        #pragma config PWRTEN = OFF
        #pragma config BOREN  = OFF
        #pragma config BORV   = 30
        #pragma config WDTEN  = OFF
        #pragma config WDTPS  = 32768
        #pragma config MCLRE  = OFF
        #pragma config HFOFST = OFF
        #pragma config STVREN = ON
        #pragma config LVP    = OFF
        #pragma config XINST  = OFF
        #pragma config BBSIZ  = OFF
        #pragma config CP0    = OFF
        #pragma config CP1    = OFF
        #pragma config CPB    = OFF
        #pragma config WRT0   = OFF
        #pragma config WRT1   = OFF
        #pragma config WRTB   = OFF
        #pragma config WRTC   = OFF
        #pragma config EBTR0  = OFF
        #pragma config EBTR1  = OFF
        #pragma config EBTRB  = OFF                                                  // CONFIG7H
        
#elif defined(EXPLORER_16)
    #ifdef __PIC24FJ256GB110__ //Defined by MPLAB when using 24FJ256GB110 device
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( 0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_ON)
    #elif defined(__32MX460F512L__)
        #ifdef USB_A0_SILICON_WORK_AROUND
            #pragma config UPLLEN   = OFF       // USB PLL Enabled (A0 bit inverted)
        #else
            #pragma config UPLLEN   = ON        // USB PLL Enabled
        #endif
        
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
        #error No hardware board defined, see "HardwareProfile.h" and __FILE__
    #endif
#elif defined(PIC24F_STARTER_KIT)
    _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
    _CONFIG2( 0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
#else
    #error No hardware board defined, see "HardwareProfile.h" and __FILE__
#endif



/** VARIABLES ******************************************************/
#pragma udata
BYTE old_sw2,old_sw3;
BOOL emulate_mode;
BYTE movement_length;
BYTE vector = 0;

#if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
    #pragma udata usbram2
#elif defined(__18F2455) || defined(__18F2550) || defined(__18F4455) || defined(__18F4550)\
    || defined(__18F4450) || defined(__18F2450)\
    || defined(__18F2458) || defined(__18F2453) || defined(__18F4558) || defined(__18F4553)
    #pragma udata USB_VARIABLES=0x500
#else
    #pragma udata
#endif

unsigned char ReceivedDataBuffer[64];
unsigned char ToSendDataBuffer[64];
#pragma udata

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;
unsigned char DemoIntroState = 0;


/** PRIVATE PROTOTYPES *********************************************/
static void InitializeSystem(void);
void ProcessIO(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();

void DemoIntroduction(void);
BOOL CheckButtonPressed(void);

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
#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{
//    //This can be used for user entry into the bootloader  
//    #if defined(__C30__) 
//        mInitSwitch2();
//        if(sw2 == 0)
//        {
//            EnterBootloader();
//        }
//    #endif

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
            oledPutROMString((ROM_STRING)"HID Custom Demo v1.0 ",1,0);
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
            oledPutROMString((ROM_STRING)"  a USB custom HID   ",2,0);
            oledPutROMString((ROM_STRING)"  device.            ",3,0);
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
            oledPutROMString((ROM_STRING)"                     ",3,0);
            oledPutROMString((ROM_STRING)"                     ",4,0);
            oledPutROMString((ROM_STRING)"                     ",5,0);
            oledPutROMString((ROM_STRING)"                     ",6,0);
            DemoIntroState = 0xFF;
            break;

        default:
            break;
    }
}

BOOL CheckButtonPressed(void)
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

    //initialize the variable holding the handle for the last
    // transmission
    USBOutHandle = 0;
    USBInHandle = 0;
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
	    BL_CheckLoaderEnabled();
    }
	
    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	// Soft Start the APP_VDD
    if(AppPowerReady() == FALSE) return;

    DemoIntroduction();

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
                  }         
                }
                break;

            case 0x30:  
                {
                  WORD w;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      w = mTouchReadButton(0);

                      ToSendDataBuffer[0] = 0x30;
                      ToSendDataBuffer[1] = (BYTE)w;
                      ToSendDataBuffer[2] = (BYTE)(w>>8);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
                  }         
                }
                break;

            case 0x31:  
                {
                  WORD w;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      w = mTouchReadButton(1);

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
                      w = mTouchReadButton(2);

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
                      w = mTouchReadButton(3);

                      ToSendDataBuffer[0] = 0x33;
                      ToSendDataBuffer[1] = (BYTE)w;
                      ToSendDataBuffer[2] = (BYTE)(w>>8);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
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
                  }         
                }
                break;

            case 0x41:  
                {
                  BMA150_REG regY_MSB;
                  BMA150_REG regY_LSB;

                  if(!HIDTxHandleBusy(USBInHandle))
                  {
                      regY_LSB.val = BMA150_ReadByte(BMA150_ACC_Y_LSB);
                      regY_MSB.val = BMA150_ReadByte(BMA150_ACC_Y_MSB);
          
                      ToSendDataBuffer[0] = 0x41;
                      ToSendDataBuffer[1] = (BYTE)(regY_LSB.val>>6);
                      ToSendDataBuffer[2] = (BYTE)(regY_MSB.val);

                      USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
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

    
}//end ProcessIO



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
