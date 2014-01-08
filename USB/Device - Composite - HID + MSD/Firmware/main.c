/********************************************************************
 FileName:      main.c
 Dependencies:  See INCLUDES section
 Processor:     PIC18, PIC24, dsPIC, and PIC32 USB Microcontrollers
 Hardware:      This demo is natively intended to be used on Microchip USB demo
                boards supported by the MCHPFSUSB stack.  See release notes for
                support matrix.  This demo can be modified for use on other 
                hardware platforms.
 Complier:      Microchip C18 (for PIC18), XC16 (for PIC24/dsPIC), XC32 (for PIC32)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Initial release
  2.1   Updated for simplicity and to use common
                     coding style
  2.7b  Improvements to USBCBSendResume(), to make it easier to use.
  2.9f  Adding new part support
  2.9j  Updates to support new bootloader features (ex: app version 
        fetching).
********************************************************************/

/** INCLUDES *******************************************************/
#include "USB/usb.h"
#include "HardwareProfile.h"
#include "MDD File System/Internal Flash.h"

#include "USB/usb_function_msd.h"
#include "USB/usb_function_hid.h"

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

#elif defined(PICDEM_FS_USB_K50)
        #pragma config PLLSEL   = PLL3X     // 3X PLL multiplier selected
        #pragma config CFGPLLEN = OFF       // PLL turned on during execution
        #pragma config CPUDIV   = NOCLKDIV  // 1:1 mode (for 48MHz CPU)
        #pragma config LS48MHZ  = SYS48X8   // Clock div / 8 in Low Speed USB mode
        #pragma config FOSC     = INTOSCIO  // HFINTOSC selected at powerup, no clock out
        #pragma config PCLKEN   = OFF       // Primary oscillator driver
        #pragma config FCMEN    = OFF       // Fail safe clock monitor
        #pragma config IESO     = OFF       // Internal/external switchover (two speed startup)
        #pragma config nPWRTEN  = OFF       // Power up timer
        #pragma config BOREN    = SBORDIS   // BOR enabled
        #pragma config nLPBOR   = ON        // Low Power BOR
        #pragma config WDTEN    = SWON      // Watchdog Timer controlled by SWDTEN
        #pragma config WDTPS    = 32768     // WDT postscalar
        #pragma config PBADEN   = OFF       // Port B Digital/Analog Powerup Behavior
        #pragma config SDOMX    = RC7       // SDO function location
        #pragma config LVP      = OFF       // Low voltage programming
        #pragma config MCLRE    = ON        // MCLR function enabled (RE3 disabled)
        #pragma config STVREN   = ON        // Stack overflow reset
        //#pragma config ICPRT  = OFF       // Dedicated ICPORT program/debug pins enable
        #pragma config XINST    = OFF       // Extended instruction set

#elif defined(PIC18F87J50_PIM)				// Configuration bits for PIC18F87J50 FS USB Plug-In Module board
        #pragma config XINST    = OFF   	// Extended instruction set
        #pragma config STVREN   = ON      	// Stack overflow reset
        #pragma config PLLDIV   = 3         // (12 MHz crystal used on this board)
        #pragma config WDTEN    = OFF      	// Watch Dog Timer (WDT)
        #pragma config CP0      = OFF      	// Code protect
        #pragma config CPUDIV   = OSC1      // OSC1 = divide by 1 mode
        #pragma config IESO     = OFF      	// Internal External (clock) Switchover
        #pragma config FCMEN    = OFF      	// Fail Safe Clock Monitor
        #pragma config FOSC     = HSPLL     // Firmware must also set OSCTUNE<PLLEN> to start PLL!
        #pragma config WDTPS    = 32768
//      #pragma config WAIT     = OFF      	// Commented choices are
//      #pragma config BW       = 16      	// only available on the
//      #pragma config MODE     = MM      	// 80 pin devices in the 
//      #pragma config EASHFT   = OFF      	// family.
        #pragma config MSSPMSK  = MSK5
//      #pragma config PMPMX    = DEFAULT
//      #pragma config ECCPMX   = DEFAULT
        #pragma config CCP2MX   = DEFAULT   
        
// Configuration bits for PIC18F97J94 PIM and PIC18F87J94 PIM
#elif defined(PIC18F97J94_PIM) || defined(PIC18F87J94_PIM)
        #pragma config STVREN   = ON      	// Stack overflow reset
        #pragma config XINST    = OFF   	// Extended instruction set
        #pragma config BOREN    = ON        // BOR Enabled
        #pragma config BORV     = 0         // BOR Set to "2.0V" nominal setting
        #pragma config CP0      = OFF      	// Code protect disabled
        #pragma config FOSC     = FRCPLL    // Firmware should also enable active clock tuning for this setting
        #pragma config SOSCSEL  = LOW       // SOSC circuit configured for crystal driver mode
        #pragma config CLKOEN   = OFF       // Disable clock output on RA6
        #pragma config IESO     = OFF      	// Internal External (clock) Switchover
        #pragma config PLLDIV   = NODIV     // 4 MHz input (from 8MHz FRC / 2) provided to PLL circuit
        #pragma config POSCMD   = NONE      // Primary osc disabled, using FRC
        #pragma config FSCM     = CSECMD    // Clock switching enabled, fail safe clock monitor disabled
        #pragma config WPDIS    = WPDIS     // Program memory not write protected
        #pragma config WPCFG    = WPCFGDIS  // Config word page of program memory not write protected
        #pragma config IOL1WAY  = OFF       // IOLOCK can be set/cleared as needed with unlock sequence
        #pragma config LS48MHZ  = SYSX2     // Low Speed USB clock divider
        #pragma config WDTCLK   = LPRC      // WDT always uses INTOSC/LPRC oscillator
        #pragma config WDTEN    = ON        // WDT disabled; SWDTEN can control WDT
        #pragma config WINDIS   = WDTSTD    // Normal non-window mode WDT.
        #pragma config VBTBOR   = OFF       // VBAT BOR disabled
      
#elif defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
     #pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
     #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
     #pragma config STVREN = ON          //stack overflow/underflow reset enabled
     #pragma config XINST = OFF          //Extended instruction set disabled
     #pragma config CPUDIV = OSC1        //No CPU system clock divide
     #pragma config CP0 = OFF            //Program memory is not code-protected
     #pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
     #pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
     #pragma config IESO = OFF           //Two-Speed Start-up disabled
     #pragma config WDTPS = 32768        //1:32768
     #pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
     #pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
     #pragma config DSWDTEN = OFF        //Disabled
     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
     #pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
     #pragma config WPEND = PAGE_0       //Start protection at page 0
     #pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored 
     #if defined(PIC18F47J53_PIM)
        #pragma config CFGPLLEN = OFF
     #else
        #pragma config T1DIG = ON           //Sec Osc clock source may be selected
        #pragma config LPT1OSC = OFF        //high power Timer1 mode
     #endif
#elif defined(LOW_PIN_COUNT_USB_DEVELOPMENT_KIT)
        #pragma config CPUDIV = NOCLKDIV
        #pragma config USBDIV = OFF
        #pragma config FOSC   = HS
        #pragma config PLLEN  = ON
        #pragma config FCMEN  = OFF
        #pragma config IESO   = OFF
        #pragma config PWRTEN = OFF
        #pragma config BOREN  = ON
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
    #if defined(__PIC24FJ256GB110__)
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_ON)
    #elif defined(__PIC24FJ256GB210__)
        _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
    #elif defined(__PIC24FJ64GB004__)
        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
        _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
    #elif defined(__32MX460F512L__) || defined(__32MX795F512L__)
        #pragma config UPLLEN   = ON        // USB PLL Enabled
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
    #elif defined(__dsPIC33EP512MU810__) || defined (__PIC24EP512GU810__)
        _FOSCSEL(FNOSC_FRC);
        _FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
        _FWDT(FWDTEN_OFF);
    #else
        #error No hardware board defined, see "HardwareProfile.h" and __FILE__
    #endif
#elif defined(PIC24F_STARTER_KIT)
    _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
    _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
#elif defined(PIC24FJ256DA210_DEV_BOARD)
    _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
    _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
#elif defined(PIC32_USB_STARTER_KIT)
        #pragma config UPLLEN   = ON        // USB PLL Enabled
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
#elif defined(DSPIC33E_USB_STARTER_KIT)
    _FOSCSEL(FNOSC_FRC);
    _FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
    _FWDT(FWDTEN_OFF);
#elif defined(PIC24FJ64GB502_MICROSTICK)
    _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
    _CONFIG2(I2C1SEL_PRI & IOL1WAY_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
    _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
    _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
#else
    #error No hardware board defined, see "HardwareProfile.h" and __FILE__
#endif



/** VARIABLES ******************************************************/
#if defined(__18CXX)
#pragma udata

#pragma udata USB_VARS
#endif
unsigned char ReceivedDataBuffer[64];
unsigned char ToSendDataBuffer[64];

#if defined(__18CXX)
#pragma udata
#endif

USB_HANDLE USBOutHandle = 0;  //Needs to be initialized to 0 at startup.
USB_HANDLE USBInHandle = 0;   //Needs to be initialized to 0 at startup.
BOOL blinkStatusValid = TRUE;

//The LUN variable definition is critical to the MSD function driver.  This
//  array is a structure of function pointers that are the functions that 
//  will take care of each of the physical media.  For each additional LUN
//  that is added to the system, an entry into this array needs to be added
//  so that the stack can know where to find the physical layer functions.
//  In this example the media initialization function is named 
//  "MediaInitialize", the read capacity function is named "ReadCapacity",
//  etc.  
LUN_FUNCTIONS LUN[MAX_LUN + 1] = 
{
    {
        &MDD_IntFlash_MediaInitialize,
        &MDD_IntFlash_ReadCapacity,
        &MDD_IntFlash_ReadSectorSize,
        &MDD_IntFlash_MediaDetect,
        &MDD_IntFlash_SectorRead,
        &MDD_IntFlash_WriteProtectState,
        &MDD_IntFlash_SectorWrite
    }
};

/* Standard Response to INQUIRY command stored in ROM 	*/
const ROM InquiryResponse inq_resp = {
	0x00,		// peripheral device is connected, direct access block device
	0x80,           // removable
	0x04,	 	// version = 00=> does not conform to any standard, 4=> SPC-2
	0x02,		// response is in format specified by SPC-2
	0x20,		// n-4 = 36-4=32= 0x20
	0x00,		// sccs etc.
	0x00,		// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
			// but as in case of other device, we are just using 00
	0x00,		// 00 obsolete, 0x80 for basic task queueing
	{'M','i','c','r','o','c','h','p'
    },
	// this is the T10 assigned Vendor ID
	{'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
    },
	{'0','0','0','1'
    }
};

/** PRIVATE PROTOTYPES *********************************************/
static void InitializeSystem(void);
void USBDeviceTasks(void);
void ProcessIO(void);
void YourHighPriorityISRCode(void);
void YourLowPriorityISRCode(void);
void USBCBSendResume(void);
void UserInit(void);
WORD_VAL ReadPOT(void);
void BlinkUSBStatus(void);

/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
	//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
	//the reset, high priority interrupt, and low priority interrupt
	//vectors.  However, the Microchip HID bootloader occupies the
	//0x00-0xFFF program memory region.  Therefore, the bootloader code remaps 
	//these vectors to new locations as indicated below.  This remapping is 
	//only necessary if you wish to be able to (optionally) program the hex file 
	//generated from this project with the USB bootloader.  
	#define REMAPPED_RESET_VECTOR_ADDRESS			0x1000
	#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
	#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018
	#define APP_VERSION_ADDRESS                     0x1016 //Fixed location, so the App FW image version can be read by the bootloader.
	#define APP_SIGNATURE_ADDRESS                   0x1006 //Signature location that must be kept at blaknk value (0xFFFF) in this project (has special purpose for bootloader).

    //--------------------------------------------------------------------------
    //Application firmware image version values, as reported to the bootloader
    //firmware.  These are useful so the bootloader can potentially know if the
    //user is trying to program an older firmware image onto a device that
    //has already been programmed with a with a newer firmware image.
    //Format is APP_FIRMWARE_VERSION_MAJOR.APP_FIRMWARE_VERSION_MINOR.
    //The valid minor version is from 00 to 99.  Example:
    //if APP_FIRMWARE_VERSION_MAJOR == 1, APP_FIRMWARE_VERSION_MINOR == 1,
    //then the version is "1.01"
    #define APP_FIRMWARE_VERSION_MAJOR  1   //valid values 0-255
    #define APP_FIRMWARE_VERSION_MINOR  0   //valid values 0-99
    //--------------------------------------------------------------------------
	
	#pragma romdata AppVersionAndSignatureSection = APP_VERSION_ADDRESS
	ROM unsigned char AppVersion[2] = {APP_FIRMWARE_VERSION_MINOR, APP_FIRMWARE_VERSION_MAJOR};
	#pragma romdata AppSignatureSection = APP_SIGNATURE_ADDRESS
	ROM unsigned short int SignaturePlaceholder = 0xFFFF;
	
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
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}
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
	#pragma code
	
	
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
        #if defined(USB_INTERRUPT)
	        USBDeviceTasks();
        #endif
	
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
	
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 
#endif




/** DECLARATIONS ***************************************************/
#if defined(__18CXX)
    #pragma code
#endif

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
    InitializeSystem();

    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif

    while(1)
    {
        #if defined(USB_POLLING)
		// Check bus status and service USB interrupts.
        USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
        				  // this function periodically.  This function will take care
        				  // of processing and responding to SETUP transactions 
        				  // (such as during the enumeration process when you first
        				  // plug in).  USB hosts require that USB devices should accept
        				  // and process SETUP packets in a timely fashion.  Therefore,
        				  // when using polling, this function should be called 
        				  // regularly (such as once every 1.8ms or faster** [see 
        				  // inline code comments in usb_device.c for explanation when
        				  // "or faster" applies])  In most cases, the USBDeviceTasks() 
        				  // function does not take very long to execute (ex: <100 
        				  // instruction cycles) before it returns.
        #endif
    				  

		// Application-specific tasks.
		// Application related code may be added here, or in the ProcessIO() function.
        ProcessIO();        
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
static void InitializeSystem(void)
{
    #if (defined(__18CXX) & !defined(PIC18F87J50_PIM) & !defined(PIC18F97J94_FAMILY))
        ADCON1 |= 0x0F;                 // Default all pins to digital
    #elif defined(__C30__) || defined __XC16__
    	#if defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
    		ANSA = 0x0000;
    		ANSB = 0x0000;
    		ANSC = 0x0000;
    		ANSD = 0x0000;
    		ANSE = 0x0000;
    		ANSF = 0x0000;
    		ANSG = 0x0000;
     #elif defined(__dsPIC33EP512MU810__) || defined (__PIC24EP512GU810__)
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

        #else
        	AD1PCFGL = 0xFFFF;
        #endif        
    #elif defined(__C32__)
        AD1PCFG = 0xFFFF;
    #endif

    #if defined(PIC18F87J50_PIM) || defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
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
        WDTCONbits.ADSHR = 1;			// Select alternate SFR location to access ANCONx registers
        ANCON0 = 0xFF;                  // Default all pins to digital
        ANCON1 = 0xFF;                  // Default all pins to digital
        WDTCONbits.ADSHR = 0;			// Select normal SFR locations
    #endif

    #if defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
        //Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
        //use the ANCONx registers to control this, which is different from other devices which
        //use the ADCON1 register for this purpose.
        ANCON0 = 0x7F;                  // Default all pins to digital (except AN7: temp sensor)
        ANCON1 = 0xBF;                  // Default all pins to digital. Bandgap on.
    #endif
    
    #if defined(PIC18F97J94_FAMILY)
        //Configure I/O pins for digital input mode.
        ANCON1 = 0xFF;
        ANCON2 = 0xFF;
        ANCON3 = 0xFF;
        #if(USB_SPEED_OPTION == USB_FULL_SPEED)
            //Enable INTOSC active clock tuning if full speed
            ACTCON = 0x90; //Enable active clock self tuning for USB operation
            while(OSCCON2bits.LOCK == 0);   //Make sure PLL is locked/frequency is compatible
                                            //with USB operation (ex: if using two speed 
                                            //startup or otherwise performing clock switching)
        #endif
    #endif
    
    #if defined(PIC18F45K50_FAMILY)
        //Configure oscillator settings for clock settings compatible with USB 
        //operation.  Note: Proper settings depends on USB speed (full or low).
        #if(USB_SPEED_OPTION == USB_FULL_SPEED)
            OSCTUNE = 0x80; //3X PLL ratio mode selected
            OSCCON = 0x70;  //Switch to 16MHz HFINTOSC
            OSCCON2 = 0x10; //Enable PLL, SOSC, PRI OSC drivers turned off
            while(OSCCON2bits.PLLRDY != 1);   //Wait for PLL lock
            *((unsigned char*)0xFB5) = 0x90;  //Enable active clock tuning for USB operation
        #endif
    #endif


#if defined(PIC24FJ64GB004_PIM) || defined(PIC24FJ256DA210_DEV_BOARD)
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

	#if defined(__32MX460F512L__)|| defined(__32MX795F512L__)
    // Configure the PIC32 core for the best performance
    // at the operating frequency. The operating frequency is already set to 
    // 60MHz through Device Config Registers
    SYSTEMConfigPerformance(60000000);
	#endif


#if defined(__dsPIC33EP512MU810__)|| defined(__PIC24EP512GU810__)

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

    UserInit();

    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    //variables to known states.
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
    //Initialize all of the LED pins
    mInitAllLEDs();
    
    //Initialize all of the push buttons
    mInitAllSwitches();
    
    //initialize the variable holding the handle for the last
    // transmission
    USBOutHandle = 0;
    USBInHandle = 0;

    blinkStatusValid = TRUE;
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
    //Blink the LEDs according to the USB device status
    if(blinkStatusValid)
    {
        BlinkUSBStatus();
    }

    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

    if(!HIDRxHandleBusy(USBOutHandle))
    {   
        switch(ReceivedDataBuffer[0])
        {
            case 0x80:  //Toggle LEDs
		        blinkStatusValid = FALSE;
		        if(mGetLED_1() == mGetLED_2())
                {
                    mLED_1_Toggle();
                    mLED_2_Toggle();
                }
                else
                {
                    if(mGetLED_1())
                    {
                        mLED_2_On();
                    }
                    else
                    {
                        mLED_2_Off();
                    }
                }
                break;
            case 0x81:  //Get push button state
                //First check to make sure the endpoint is available before modifying the data buffer contents
                if(!HIDTxHandleBusy(USBInHandle))
                {
                    //Prepare a response packet to send to the host
                    ToSendDataBuffer[0] = 0x81;				//Echo back to the host PC the command we are fulfilling in the first byte.  In this case, the Get Pushbutton State command.
    				if(sw2 == 1)							//pushbutton not pressed, pull up resistor on circuit board is pulling the PORT pin high
    				{
    					ToSendDataBuffer[1] = 0x01;			
    				}
    				else									//sw2 must be == 0, pushbutton is pressed and overpowering the pull up resistor
    				{
    					ToSendDataBuffer[1] = 0x00;
    				}
    				//Prepare the USB module to send the data packet to the host
                    USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
                }
                break;
            case 0x37:	//Read POT command.  Uses ADC to measure an analog voltage on one of the ANxx I/O pins, and returns the result to the host
                {
                    WORD_VAL w;
                    
                    //Check to make sure the endpoint/buffer is free before we modify the contents
	                if(!HIDTxHandleBusy(USBInHandle))
	                {
	                    mInitPOT();
	                    w = ReadPOT();					//Use ADC to read the I/O pin voltage.  See the relevant HardwareProfile - xxxxx.h file for the I/O pin that it will measure.
														//Some demo boards, like the PIC18F87J50 FS USB Plug-In Module board, do not have a potentiometer (when used stand alone).
														//This function call will still measure the analog voltage on the I/O pin however.  To make the demo more interesting, it
														//is suggested that an external adjustable analog voltage should be applied to this pin.
						ToSendDataBuffer[0] = 0x37;  	//Echo back to the host the command we are fulfilling in the first byte.  In this case, the Read POT (analog voltage) command.
						ToSendDataBuffer[1] = w.v[0];  	//Measured analog voltage LSB
						ToSendDataBuffer[2] = w.v[1];  	//Measured analog voltage MSB

                        //Prepare the USB module to send the data packet to the host
	                    USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
	                }					
                }
                break;
        }
        //Re-arm the OUT endpoint, so we can receive the next OUT data packet 
        //that the host may try to send us.
        USBOutHandle = HIDRxPacket(HID_EP, (BYTE*)&ReceivedDataBuffer, 64);
    }

    MSDTasks(); 

   
}//end ProcessIO



/******************************************************************************
 * Function:        WORD_VAL ReadPOT(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          WORD_VAL - the 10-bit right justified POT value
 *
 * Side Effects:    ADC buffer value updated
 *
 * Overview:        This function reads the POT and leaves the value in the 
 *                  ADC buffer register
 *
 * Note:            None
 *****************************************************************************/
WORD_VAL ReadPOT(void)
{
    WORD_VAL w;

    #if defined(__18CXX)
        #if defined(PIC18F97J94_PIM) 
            ADCON1Lbits.SAMP = 1;           // Start AD sampling/convert sequence
            while(ADCON1Lbits.DONE == 0);   // Wait for result complete
            w.v[0] = ADCBUF3L;
            w.v[1] = ADCBUF3H;       
        #elif defined(PIC18F87J94_PIM)
            ADCON1Lbits.SAMP = 1;           // Start AD sampling/convert sequence
            while(ADCON1Lbits.DONE == 0);   // Wait for result complete
            w.v[0] = ADCBUF0L;
            w.v[1] = ADCBUF0H;       
        #else
            ADCON0bits.GO = 1;              // Start AD conversion
            while(ADCON0bits.NOT_DONE);     // Wait for conversion
            w.v[0] = ADRESL;
            w.v[1] = ADRESH;
        #endif

    #elif defined(__C30__) || defined(__C32__) || defined __XC16__
        #if defined(DSPIC33E_USB_STARTER_KIT) || defined(PIC24FJ64GB502_MICROSTICK)
            /*******************************
             * The dsPIC33E USB starter kit
             * and PIC24FJ64GB502 Microstick
             * do not have a pot. We simply
             * return 0 here.
             ******************************/
             w.Val = 0;
            return(w);
        #elif defined(PIC24FJ256GB110_PIM) || defined(PIC24FJ256DA210_DEV_BOARD) || defined(PIC24FJ256GB210_PIM)
            AD1CHS = 0x5;           //MUXA uses AN5

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete
        #elif defined(PIC24FJ64GB004_PIM)
            AD1CHS = 0x7;           //MUXA uses AN5

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete
        #elif defined(PIC24F_STARTER_KIT)
            AD1CHS = 0x0;           //MUXA uses AN0

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

        #elif defined(PIC32MX460F512L_PIM) || defined(PIC32_USB_STARTER_KIT) || defined(PIC32MX795F512L_PIM)
            AD1PCFG = 0xFFFB; // PORTB = Digital; RB2 = analog
            AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling ...
            // and starts converting
            AD1CHS = 0x00020000; // Connect RB2/AN2 as CH0 input ..
            // in this example RB2/AN2 is the input
            AD1CSSL = 0;
            AD1CON3 = 0x0002; // Manual Sample, Tad = internal 6 TPB
            AD1CON2 = 0;
            AD1CON1SET = 0x8000; // turn ADC ON

            AD1CON1SET = 0x0002; // start sampling ...
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1CLR = 0x0002; // start Converting
            while (!(AD1CON1 & 0x0001));// conversion done?
        #elif defined (__dsPIC33EP512MU810__) || defined (__PIC24EP512GU810__)
           
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete
            
        #else
            #error
        #endif

        w.Val = ADC1BUF0;

    #endif

    return w;
}//end ReadPOT

/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs 
 *                  corresponding to the USB device state.
 *
 * Note:            mLED macros can be found in HardwareProfile.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/
void BlinkUSBStatus(void)
{
#if defined(PIC24FJ256DA210_DEV_BOARD)
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
    #define BLINK_INTERVAL 20000
    #define BLANK_INTERVAL 200000

    static WORD blink_count=0;
    static DWORD loop_count = 0;
    
    if(loop_count == 0)
    {
        if(blink_count != 0)
        {
            loop_count = BLINK_INTERVAL;
            if(mGetLED_1())
            {
                mLED_1_Off();
                blink_count--;
            }
            else
            {
                mLED_1_On();
            }
        }
        else
        {
            loop_count = BLANK_INTERVAL;
            switch(USBDeviceState)
            {
                case ATTACHED_STATE:
                    blink_count = 1;
                    break;
                case POWERED_STATE:
                    blink_count = 2;
                    break;
                case DEFAULT_STATE:
                    blink_count = 3;
                    break;
                case ADR_PENDING_STATE:
                    blink_count = 4;
                    break;
                case ADDRESS_STATE:
                    blink_count = 5;
                    break;
                case CONFIGURED_STATE:
                    blink_count = 6;
                    break;
                case DETACHED_STATE:
                    //fall through
                default:
                    blink_count = 0;
                    break;
            }
        }
    }
    else
    {
        loop_count--;
    }

#else
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
    static WORD led_count=0;
    
    if(led_count == 0)led_count = 10000U;
    led_count--;

    #define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if(USBSuspendControl == 1)
    {
        if(led_count==0)
        {
            mLED_1_Toggle();
            if(mGetLED_1())
            {
                mLED_2_On();
            }
            else
            {
                mLED_2_Off();
            }
        }//end if
    }
    else
    {
        if(USBDeviceState == DETACHED_STATE)
        {
            mLED_Both_Off();
        }
        else if(USBDeviceState == ATTACHED_STATE)
        {
            mLED_Both_On();
        }
        else if(USBDeviceState == POWERED_STATE)
        {
            mLED_Only_1_On();
        }
        else if(USBDeviceState == DEFAULT_STATE)
        {
            mLED_Only_2_On();
        }
        else if(USBDeviceState == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                mLED_1_Toggle();
                mLED_2_Off();
            }//end if
        }
        else if(USBDeviceState == CONFIGURED_STATE)
        {
            if(led_count==0)
            {
                mLED_1_Toggle();
                if(mGetLED_1())
                {
                    mLED_2_Off();
                }
                else
                {
                    mLED_2_On();
                }
            }//end if
        }
    }
#endif
}//end BlinkUSBStatus


// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA* each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

// Note *: The "usb_20.pdf" specs indicate 500uA or 2.5mA, depending upon device classification. However,
// the USB-IF has officially issued an ECN (engineering change notice) changing this to 2.5mA for all 
// devices.  Make sure to re-download the latest specifications to get all of the newest ECNs.

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
        Sleep();
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
	// 10+ milliseconds of wakeup time, after which the device must be 
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).  
	// Make sure the selected oscillator settings are consistent with USB 
    // operation before returning from this function.
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
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
    USBCheckMSDRequest();
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
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    #if (MSD_DATA_IN_EP == MSD_DATA_OUT_EP)
        USBEnableEndpoint(MSD_DATA_IN_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    #else
        USBEnableEndpoint(MSD_DATA_IN_EP,USB_IN_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
        USBEnableEndpoint(MSD_DATA_OUT_EP,USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    #endif

    //enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);

    USBMSDInit();
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
    switch((INT)event)
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

            //Check if the host recently did a clear endpoint halt on the MSD OUT endpoint.
            //In this case, we want to re-arm the MSD OUT endpoint, so we are prepared
            //to receive the next CBW that the host might want to send.
            //Note: If however the STALL was due to a CBW not valid condition, 
            //then we are required to have a persistent STALL, where it cannot 
            //be cleared (until MSD reset recovery takes place).  See MSD BOT 
            //specs v1.0, section 6.6.1.
            if(MSDWasLastCBWValid() == FALSE)
            {
                //Need to re-stall the endpoints, for persistent STALL behavior.
    			USBStallEndpoint(MSD_DATA_IN_EP, IN_TO_HOST);
      			USBStallEndpoint(MSD_DATA_OUT_EP, OUT_FROM_HOST);                 
            }
            else
            {   
                //Check if the host cleared halt on the bulk out endpoint.  In this
                //case, we should re-arm the endpoint, so we can receive the next CBW.
                if((USB_HANDLE)pdata == USBGetNextHandle(MSD_DATA_OUT_EP, OUT_FROM_HOST))
                {
                    USBMSDOutHandle = USBRxOnePacket(MSD_DATA_OUT_EP, (BYTE*)&msd_cbw, MSD_OUT_EP_SIZE);
                }    
            }    
            break;
        default:
            break;
    }      
    return TRUE; 
}

/** EOF main.c *************************************************/
