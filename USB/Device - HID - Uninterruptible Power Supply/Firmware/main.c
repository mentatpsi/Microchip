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
  2.7   Minor change to POWER_SUMMARY_RUN_TIME_TO_EMPTY reporting.   
  2.7b  Improvements to USBCBSendResume(), to make it easier to use.                
  2.9f  Adding new part support
  2.9j  Updates to support new bootloader features (ex: app version 
        fetching).
********************************************************************/

#ifndef MAIN_C
#define MAIN_C

/** INCLUDES *******************************************************/
#include "./USB/usb.h"
#include "HardwareProfile.h"
#include "./USB/usb_function_hid.h"
#include "reports.h"

/** CONFIGURATION **************************************************/
#if defined(PICDEM_FS_USB)      // Configuration bits for PICDEM FS USB Demo Board (based on PIC18F4550)
        #pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
        #if (USB_SPEED_OPTION == USB_FULL_SPEED)
            #pragma config CPUDIV   = OSC1_PLL2  
        #else
            #pragma config CPUDIV   = OSC3_PLL4   
        #endif
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
        #pragma config EBTRB  = OFF        

#elif	defined(PIC16F1_LPC_USB_DEVELOPMENT_KIT)
    // PIC16F1459 configuration bit settings:
    #if defined (USE_INTERNAL_OSC)  //Definition in the hardware profile
        __CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
        __CONFIG(WRT_OFF & CPUDIV_NOCLKDIV & USBLSCLK_48MHz & PLLMULT_3x & PLLEN_ENABLED & STVREN_ON &  BORV_LO & LPBOR_OFF & LVP_OFF);
    #else
        __CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
        __CONFIG(WRT_OFF & CPUDIV_NOCLKDIV & USBLSCLK_48MHz & PLLMULT_4x & PLLEN_ENABLED & STVREN_ON &  BORV_LO & LPBOR_OFF & LVP_OFF);
    #endif

#elif defined(EXPLORER_16)
    #if defined(__PIC24FJ256GB110__)
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_ON)
    #elif defined(PIC24FJ256GB210_PIM)
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
#endif

//The ReceivedDataBuffer[] and ToSendDataBuffer[] arrays are used as
//USB packet buffers in this firmware.  Therefore, they must be located in
//a USB module accessible portion of microcontroller RAM.
#if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
    #pragma udata USB_VARIABLES=0x260
#elif defined(__18F2455) || defined(__18F2550) || defined(__18F4455) || defined(__18F4550)\
    || defined(__18F2458) || defined(__18F2453) || defined(__18F4558) || defined(__18F4553)\
    || defined(__18LF24K50) || defined(__18F24K50) || defined(__18LF25K50)\
    || defined(__18F25K50) || defined(__18LF45K50) || defined(__18F45K50)
    #pragma udata USB_VARIABLES=0x500
#elif defined(__18F4450) || defined(__18F2450)
    #pragma udata USB_VARIABLES=0x480
#elif defined(__18CXX)
    #pragma udata
#endif

#if defined(__XC8)
    #if defined(_18F14K50) || defined(_18F13K50) || defined(_18LF14K50) || defined(_18LF13K50)
        #define IN_DATA_BUFFER_ADDRESS 0x260
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + HID_INT_IN_EP_SIZE)
        #define FEATURE_DATA_BUFFER_ADDRESS (OUT_DATA_BUFFER_ADDRESS + HID_INT_OUT_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
        #define FEATURE_DATA_BUFFER_ADDRESS_TAG @FEATURE_DATA_BUFFER_ADDRESS
    #elif  defined(_18F2455)   || defined(_18F2550)   || defined(_18F4455)  || defined(_18F4550)\
        || defined(_18F2458)   || defined(_18F2453)   || defined(_18F4558)  || defined(_18F4553)\
        || defined(_18LF24K50) || defined(_18F24K50)  || defined(_18LF25K50)\
        || defined(_18F25K50)  || defined(_18LF45K50) || defined(_18F45K50)
        #define IN_DATA_BUFFER_ADDRESS 0x500
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + HID_INT_IN_EP_SIZE)
        #define FEATURE_DATA_BUFFER_ADDRESS (OUT_DATA_BUFFER_ADDRESS + HID_INT_OUT_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
        #define FEATURE_DATA_BUFFER_ADDRESS_TAG @FEATURE_DATA_BUFFER_ADDRESS
    #elif defined(_18F4450) || defined(_18F2450)
        #define IN_DATA_BUFFER_ADDRESS 0x480
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + HID_INT_IN_EP_SIZE)
        #define FEATURE_DATA_BUFFER_ADDRESS (OUT_DATA_BUFFER_ADDRESS + HID_INT_OUT_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
        #define FEATURE_DATA_BUFFER_ADDRESS_TAG @FEATURE_DATA_BUFFER_ADDRESS
    #elif defined(_16F1459)
        #define IN_DATA_BUFFER_ADDRESS 0x2050
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + HID_INT_IN_EP_SIZE)
        #define FEATURE_DATA_BUFFER_ADDRESS (OUT_DATA_BUFFER_ADDRESS + HID_INT_OUT_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
        #define FEATURE_DATA_BUFFER_ADDRESS_TAG @FEATURE_DATA_BUFFER_ADDRESS
    #else
        #define IN_DATA_BUFFER_ADDRESS_TAG
        #define OUT_DATA_BUFFER_ADDRESS_TAG
        #define FEATURE_DATA_BUFFER_ADDRESS_TAG
    #endif
#else
    #define IN_DATA_BUFFER_ADDRESS_TAG
    #define OUT_DATA_BUFFER_ADDRESS_TAG
    #define FEATURE_DATA_BUFFER_ADDRESS_TAG
#endif

unsigned char ReceivedDataBuffer[HID_INT_OUT_EP_SIZE] IN_DATA_BUFFER_ADDRESS_TAG;  //USB packet buffer, must reside in USB module accessible RAM
unsigned char ToSendDataBuffer[HID_INT_IN_EP_SIZE] OUT_DATA_BUFFER_ADDRESS_TAG;     //USB packet buffer, must reside in USB module accessible RAM
BYTE hid_feature_data[USB_EP0_BUFF_SIZE] FEATURE_DATA_BUFFER_ADDRESS_TAG;

#if defined(__18CXX)
    #pragma idata
#endif
USB_HANDLE USBOutHandle = 0;    //USB handle.  Must be initialized to 0 at startup.
USB_HANDLE USBInHandle = 0;     //USB handle.  Must be initialized to 0 at startup.
BOOL blinkStatusValid = TRUE;
BYTE capacity = 100;

#if defined(__18CXX)
    #pragma udata
#endif


/** PRIVATE PROTOTYPES *********************************************/
void BlinkUSBStatus(void);
BOOL Switch2IsPressed(void);
BOOL Switch3IsPressed(void);
static void InitializeSystem(void);
void ProcessIO(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void USBCBSendResume(void);
WORD_VAL ReadPOT(void);

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

#elif defined(__C30__) || defined __XC16__
    #if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
        /*
         *	ISR JUMP TABLE
         *
         *	It is necessary to define jump table as a function because C30 will
         *	not store 24-bit wide values in program memory as variables.
         *
         *	This function should be stored at an address where the goto instructions 
         *	line up with the remapped vectors from the bootloader's linker script.
         *  
         *  For more information about how to remap the interrupt vectors,
         *  please refer to AN1157.  An example is provided below for the T2
         *  interrupt with a bootloader ending at address 0x1400
         */
//        void __attribute__ ((address(0x1404))) ISRTable(){
//        
//        	asm("reset"); //reset instruction to prevent runaway code
//        	asm("goto %0"::"i"(&_T2Interrupt));  //T2Interrupt's address
//        }
    #endif
#elif defined(_PIC14E)
    	//These are your actual interrupt handling routines.
	void interrupt ISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
        #if defined(USB_INTERRUPT)

	        USBDeviceTasks();
        #endif
	}
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
    #if defined(_PIC14E)
        //Configure all pins for digital mode, except RB4, which has a POT on it
        ANSELA = 0x00;
        #if defined(_16F1459) || defined(_16LF1459)
            ANSELB = 0x10;  //RB4 has a POT on it, on the Low Pin Count USB Dev Kit board
        #endif
        ANSELC = 0x00;
        #if defined (USE_INTERNAL_OSC)
            OSCTUNE = 0;
            OSCCON = 0xFC;          //16MHz HFINTOSC with 3x PLL enabled (48MHz operation)
            ACTCON = 0x90;          //Enable active clock tuning with USB
        #endif
    #endif
    
    #if (defined(__18CXX) & !defined(PIC18F87J50_PIM) & !defined(PIC18F97J94_FAMILY))
        ADCON1 |= 0x0F;                 // Default all pins to digital
    #elif defined(__C30__) || defined __XC16__
        #if defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256GB106__)
            AD1PCFGL = 0xFFFF;
        #endif
    #elif defined(__C32__)
        AD1PCFG = 0xFFFF;
    #endif
    
    #if defined(__32MX460F512L__)|| defined(__32MX795F512L__)
    // Configure the PIC32 core for the best performance
    // at the operating frequency. The operating frequency is already set to 
    // 60MHz through Device Config Registers
    SYSTEMConfigPerformance(60000000);
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


    #if defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
	//Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
	//use the ANCONx registers to control this, which is different from other devices which
	//use the ADCON1 register for this purpose.
    ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital
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
//	has been defined in HardwareProfile - (platform).h, and that an appropriate I/O pin 
//  has been mapped	to it.
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
 * Function:        void UserGetReportHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    If the USBEP0Transmit() function is not called
 *                  in this function then the resulting GET_REPORT
 *                  will be STALLed.
 *
 * Overview:        This function is called by the HID function driver
 *                  in response to a GET_REPORT command.
 *
 * Note:            This function is called from the USB stack in
 *                  response to a class specific control transfer requests 
 *                  arriving over EP0.  Therefore, this function executes in the
 *                  same context as the USBDeviceTasks() function executes (which 
 *                  may be an interrupt handler, or a main loop context function, 
 *                  depending upon usb_config.h options).
 *******************************************************************/
void UserSetReportHandler(void)
{
    switch(SetupPkt.W_Value.byte.LB)
    {
        case POWER_SUMMARY_REMAINING_CAPACITY_LIMIT:
            //Receive the EP0 data but set the notification function pointer
            //  to NULL in order to ignore the data.  Note that the contents of
            //  ReceivedDataBuffer will be updated with the SET_REPORT data
            USBEP0Receive((BYTE*)ReceivedDataBuffer,SetupPkt.wLength,NULL);
            break;
        default:
            break;
    }
}

/********************************************************************
 * Function:        void UserGetReportHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    If either the USBEP0SendRAMPtr() or USBEP0Transmit()
 *                  functions are not called in this function then the 
 *                  requesting GET_REPORT will be STALLed
 *
 * Overview:        This function is called by the HID function driver
 *                  in response to a GET_REPORT command.
 *
 * Note:            This function is called from the USB stack in
 *                  response to a class specific control transfer request 
 *                  arriving over EP0.  Therefore, this function executes in the
 *                  same context as the USBDeviceTasks() function executes (which 
 *                  may be an interrupt handler, or a main loop context function, 
 *                  depending upon usb_config.h options).
 *                  If the firmware needs more time to process the request,
 *                  here would be a good place to use the USBDeferStatusStage()
 *                  USB stack API function.      
 *******************************************************************/
void UserGetReportHandler(void)
{
    switch(SetupPkt.W_Value.v[0])
    {
        //*********************************************************************
        //  Title:
        //    PowerSummaryID Report
        //
        //  Description:
        //    The PowerSummaryID report provides an ID for the PowerSummary
        //    collection.  This number is arbitrary and can be any constant as
        //    long as it is unique for the device.  Note - the STRING_INDEX()
        //    entry is used in the HID report.  The value returned by this
        //    function should match that number
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_POWER_SUMMARY_ID        |
        //         |---------------------------------------------------------------|
        //      1  |                         PowerSummaryID                        |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_POWER_SUMMARY_ID:     //PowerSummaryID
            hid_feature_data[0] = POWER_SUMMARY_POWER_SUMMARY_ID;
            hid_feature_data[1] = 1;
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    iName Report
        //
        //  Description:
        //    iName is the string index into the string descriptor array that 
        //    describes the name of the product.  Note - the STRING_INDEX()
        //    entry is used in the HID report.  The value returned by this
        //    function should match that number
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_I_NAME                  |
        //         |---------------------------------------------------------------|
        //      1  |                         iName                                 |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_I_NAME:
            hid_feature_data[0] = POWER_SUMMARY_I_NAME;
            hid_feature_data[1] = 1;      //iName
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    iProduct Report
        //
        //  Description:
        //    iProduct is the string index into the string descriptor array that 
        //    describes the product.  Note - the STRING_INDEX()
        //    entry is used in the HID report.  The value returned by this
        //    function should match that number
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_I_PRODUCT               |
        //         |---------------------------------------------------------------|
        //      1  |                         iProduct                              |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_I_PRODUCT:
            hid_feature_data[0] = POWER_SUMMARY_I_PRODUCT;
            hid_feature_data[1] = 2;      //iProduct
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    iSerialNumber Report
        //
        //  Description:
        //    iSerialNumber is the string index into the string descriptor array 
        //    that gives the products unique serial number.  Note - the STRING_INDEX()
        //    entry is used in the HID report.  The value returned by this
        //    function should match that number
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_I_SERIAL_NUMBER         |
        //         |---------------------------------------------------------------|
        //      1  |                         iSerialNumber                         |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_I_SERIAL_NUMBER:
            hid_feature_data[0] = POWER_SUMMARY_I_SERIAL_NUMBER;
            hid_feature_data[1] = 4;      //iSerialNumber
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    iDeviceChemistry Report
        //
        //  Description:
        //    iDeviceChemistry is the string index into the string descriptor
        //    array that describes the chemistry of the battery in use.
        //    Note - the STRING_INDEX() entry is used in the HID report.  
        //    The value returned by this function should match that number
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_I_DEVICE_CHEMISTRY      |
        //         |---------------------------------------------------------------|
        //      1  |                         iDeviceChemistry                      |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_I_DEVICE_CHEMISTRY:
            hid_feature_data[0] = POWER_SUMMARY_I_DEVICE_CHEMISTRY;
            hid_feature_data[1] = 3;      //iDeviceChemistery
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    iOEMInformation Report
        //
        //  Description:
        //    iOEMInformation is the string index into the string descriptor
        //    array that describes the OEM information about the device
        //    Note - the STRING_INDEX() entry is used in the HID report.  
        //    The value returned by this function should match that number
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_I_OEM_INFORMATION       |
        //         |---------------------------------------------------------------|
        //      1  |                         iOEMInformation                       |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_I_OEM_INFORMATION:
            hid_feature_data[0] = POWER_SUMMARY_I_OEM_INFORMATION;
            hid_feature_data[1] = 1;      //iOEMInformation
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    iManufacturerName Report
        //
        //  Description:
        //    iManufacturerName is the string index into the string descriptor
        //    array that contains the Manufacturer's name
        //    Note - the STRING_INDEX() entry is used in the HID report.  
        //    The value returned by this function should match that number
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_I_MANUFACTURER_NAME     |
        //         |---------------------------------------------------------------|
        //      1  |                         iManufacturerName                     |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_I_MANUFACTURER_NAME:
            hid_feature_data[0] = POWER_SUMMARY_I_MANUFACTURER_NAME;
            hid_feature_data[1] = 1;      //iManufacturerName
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    iManufacturer Report
        //
        //  Description:
        //    iManufacturer is the string index into the string descriptor
        //    array that contains the Manufacturer info
        //    Note - the STRING_INDEX() entry is used in the HID report.  
        //    The value returned by this function should match that number
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_I_MANUFACTURER          |
        //         |---------------------------------------------------------------|
        //      1  |                         iManufacturer                         |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_I_MANUFACTURER:
            hid_feature_data[0] = POWER_SUMMARY_I_MANUFACTURER;
            hid_feature_data[1] = 1;      //iManufacturer
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    Voltage Information Report
        //
        //  Description:
        //    This report gives information about the voltage of the device
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_VOLTAGE_INFO            |
        //         |---------------------------------------------------------------|
        //      1  |                         ConfigVoltage (byte 0)                |
        //         |---------------------------------------------------------------|
        //      2  |                         ConfigVoltage (byte 1)                |
        //         |---------------------------------------------------------------|
        //      3  |                         Voltage (byte 0)                      |
        //         |---------------------------------------------------------------|
        //      4  |                         Voltage (byte 1)                      |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_VOLTAGE_INFO:
            hid_feature_data[0] = POWER_SUMMARY_VOLTAGE_INFO;
            hid_feature_data[1] = 0;          //ConfigVoltage - Nominal Voltage
            hid_feature_data[2] = 5;
            
            hid_feature_data[3] = 0xFF;          //Voltage - Actual Voltage
            hid_feature_data[4] = 4;

            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,5,USB_EP0_NO_OPTIONS); 
            break;

        //*********************************************************************
        //  Title:
        //    Current Information Report
        //
        //  Description:
        //    This report gives information about the current of the device
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_VOLTAGE_INFO            |
        //         |---------------------------------------------------------------|
        //      1  |                         ConfigCurrent (byte 0)                |
        //         |---------------------------------------------------------------|
        //      2  |                         ConfigCurrent (byte 1)                |
        //         |---------------------------------------------------------------|
        //      3  |                         Current (byte 0)                      |
        //         |---------------------------------------------------------------|
        //      4  |                         Current (byte 1)                      |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_CURRENT_INFO:
            hid_feature_data[0] = POWER_SUMMARY_VOLTAGE_INFO;
            hid_feature_data[1] = 0;          //ConfigCurrent - Nominal Current
            hid_feature_data[2] = 5;
            
            hid_feature_data[3] = 0xFF;          //Current - Actual Current
            hid_feature_data[4] = 4;

            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,5,USB_EP0_NO_OPTIONS); 
            break;

        //*********************************************************************
        //  Title:
        //    PresentStatus Report
        //
        //  Description:
        //    The present status report gives information about the current state
        //    of the device.
        //
        //  Format:
        //    byte |       bit 7      |       bit 6       |      bit 5      |       bit 4       |    bit 3    |      bit 2      |           bit 1           |           bit 0             |
        //         |----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_STATUS_INFO                                                                                                                    |
        //         |----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        //      1  | ShutdownImminent | ShutdownRequested | OverTemperature | OverCharged       | Overload    | InternalFailure | Good                      | Present                     |  
        //         |----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        //      2  |  BatteryPresent  | ACPresent         | NeedReplacement | ConditionningFlag | Discharging | Charging        | RemainingTimeLimitExpired | BelowRemainingCapacityLimit |
        //         ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_STATUS_INFO:
            hid_feature_data[0] = POWER_SUMMARY_STATUS_INFO;
            //check the pushbutton
            if(sw2 == 0)
            {
                //if it is pressed then we are simulating a discharge state
                //  no AC present.

                //If the capacity is less than 5% 
                if(capacity < 5)
                {
                    //then report that a shutdown is imminent
                    hid_feature_data[1] = 0xC3;
                }
                else
                {
                    hid_feature_data[1] = 0x03;
                }
                hid_feature_data[2] = 0x88;
            }
            else
            {
                //We are AC powered
                hid_feature_data[1] = 0x03;
    
                //if the capacity is at 100%
                if(capacity == 100)
                {
                    //Then we can say that we are no longer charging
                    hid_feature_data[2] = 0xC0;
                }
                else
                {
                    //otherwise continue charging
                    hid_feature_data[2] = 0xC4;
                }
            }
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,3,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    Capacity Mode Report
        //
        //  Description:
        //    CapacityMode describes in what format all of the other capacity
        //    reports are reported (mAH, mwH, percentage, etc).  See section 
        //    4.2.3 of the HID Power device specification (v1) for more information
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_CAPACITY_MODE           |
        //         |---------------------------------------------------------------|
        //      1  |                         CapacityMode                          |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_CAPACITY_MODE:
            hid_feature_data[0] = POWER_SUMMARY_CAPACITY_MODE;
            hid_feature_data[1] = 2;   //CapacityMode
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    DesignCapacity Report
        //
        //  Description:
        //    DesignCapacity reflects the capacity that the battery was designed
        //    for in the units specified by the CapacityMode report
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_DESIGN_CAPACITY         |
        //         |---------------------------------------------------------------|
        //      1  |                         DesignCapacity                        |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_DESIGN_CAPACITY:        //DesignCapacity
            hid_feature_data[0] = POWER_SUMMARY_DESIGN_CAPACITY;
            hid_feature_data[1] = 100;
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    RemainingCapacity Report
        //
        //  Description:
        //    RemainingCapacity reflects the capacity left in the battery 
        //    in the units specified by the CapacityMode report
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_REMAINING_CAPACITY      |
        //         |---------------------------------------------------------------|
        //      1  |                         RemainingCapacity                     |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_REMAINING_CAPACITY:        //RemainingCapacity
            hid_feature_data[0]=POWER_SUMMARY_REMAINING_CAPACITY;
            hid_feature_data[1] = capacity;
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    FullChargeCapacity Report
        //
        //  Description:
        //    FullChargeCapacity reflects the capacity of the battery when it
        //    is fully charged in the units specified by the CapacityMode report
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_DESIGN_CAPACITY         |
        //         |---------------------------------------------------------------|
        //      1  |                         FullChargeCapacity                    |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_FULL_CHARGE_CAPACITY:       //full charge capacity
            hid_feature_data[0] = POWER_SUMMARY_FULL_CHARGE_CAPACITY;
            hid_feature_data[1] = 100;
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    RemainingCapacityLimit Report
        //
        //  Description:
        //    RemainingCapacityLimit reflects the capacity at which the battery
        //    will warn the PC that the power is passed the limit.
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |       Report ID = POWER_SUMMARY_REMAINING_CAPACITY_LIMIT      |
        //         |---------------------------------------------------------------|
        //      1  |                         RemainingCapacityLimit                |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_REMAINING_CAPACITY_LIMIT:        //RemainingCapacityLimit
            hid_feature_data[0] = POWER_SUMMARY_REMAINING_CAPACITY_LIMIT;
            hid_feature_data[1] = 0x05;
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    WarningCapacityLimit Report
        //
        //  Description:
        //    WarningCapacityLimit reflects a capacity at which the unit will
        //    warn the PC of a low battery in units specified in the CapacityMode
        //    report
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_WARNING_CAPACITY_LIMIT  |
        //         |---------------------------------------------------------------|
        //      1  |                         WarningCapacityLimit                  |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_WARNING_CAPACITY_LIMIT:        //WarningCapacityLimit
            hid_feature_data[0] = POWER_SUMMARY_WARNING_CAPACITY_LIMIT;
            hid_feature_data[1] = 20;       //20%
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    CapacityGranularity1 Report
        //
        //  Description:
        //    CapacityGranularity1 reflects a capacity at which the unit will
        //    warn the PC of a low battery in units specified in the CapacityMode
        //    report
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_CAPACITY_GRANULARITY_1  |
        //         |---------------------------------------------------------------|
        //      1  |                         CapacityGranularity1                  |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_CAPACITY_GRANULARITY_1:        //CapacityGranularity1
            hid_feature_data[0] = POWER_SUMMARY_CAPACITY_GRANULARITY_1;
            hid_feature_data[1] = 5;        //5% less
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    CapacityGranularity2 Report
        //
        //  Description:
        //    CapacityGranularity2 reflects a capacity at which the unit will
        //    warn the PC of a low battery in units specified in the CapacityMode
        //    report
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_CAPACITY_GRANULARITY_2  |
        //         |---------------------------------------------------------------|
        //      1  |                         CapacityGranularity2                  |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_CAPACITY_GRANULARITY_2:        //CapacityGranularity2
            hid_feature_data[0] = POWER_SUMMARY_CAPACITY_GRANULARITY_2;
            hid_feature_data[1] = 5;        //5% less
            USBEP0SendRAMPtr((BYTE*)&hid_feature_data,2,USB_EP0_NO_OPTIONS);
            break;

        //*********************************************************************
        //  Title:
        //    RunTimeToEmpty Report
        //
        //  Description:
        //    This report sends the number of seconds of capacity
        //
        //  Format:
        //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
        //         |---------------------------------------------------------------|
        //      0  |             Report ID = POWER_SUMMARY_RUN_TIME_TO_EMPTY       |
        //         |---------------------------------------------------------------|
        //      1  |                         RunTimeToEmpty (byte 0)               |
        //         |---------------------------------------------------------------|
        //      2  |                         RunTimeToEmpty (byte 1)               |
        //         -----------------------------------------------------------------
        //      3  |                         RunTimeToEmpty (byte 2)               |
        //         -----------------------------------------------------------------
        //*********************************************************************
        case POWER_SUMMARY_RUN_TIME_TO_EMPTY:
            {
                WORD_VAL RunTimeToEmpty;

                //RunTimeToEmpty.Val = (60u * 60u * 1u * capacity)/100u;	//Basically want this, but since runtime division is expensive, better to do the below instead.
                RunTimeToEmpty.Val = (36u * (WORD)capacity);
                //60 sec/min * 60 min/hr * 1hr capacity(typically) * current_capacity/100 = seconds of capacity left

                hid_feature_data[0] = POWER_SUMMARY_RUN_TIME_TO_EMPTY;
                hid_feature_data[1] = RunTimeToEmpty.v[0];
                hid_feature_data[2] = RunTimeToEmpty.v[1];
                hid_feature_data[3] = 0u;
                USBEP0SendRAMPtr((BYTE*)&hid_feature_data,4,USB_EP0_NO_OPTIONS)
            }
            break;

        //Doing nothing will result in all other report IDs to be STALLed
        default:
            break;
    }
}

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
    static BYTE reportToSend = POWER_SUMMARY_STATUS_INFO;
    static BYTE old_sw2 = 1;
    static unsigned long i = 0;

    //Blink the LEDs according to the USB device status
    if(blinkStatusValid)
    {
        BlinkUSBStatus();
    }

    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
    
    //If the last transmission is already complete
    if(!HIDTxHandleBusy(USBInHandle))
    {
        //if the button is pressed
        if(sw2 != old_sw2)
        {
            //save the button value
            old_sw2 = sw2;

            //Set the state machine variable to send the 
            //  POWER_SUMMARY_STATUS_INFO report
            reportToSend = POWER_SUMMARY_STATUS_INFO;

            i=0;  //Reset delay count
        }

        //INTPUT Report sending state machine.
        switch(reportToSend)
        {   
            //*********************************************************************
            //  Title:
            //    PresentStatus Report
            //
            //  Description:
            //    The present status report gives information about the current state
            //    of the device.
            //
            //  Format:
            //    byte |       bit 7      |       bit 6       |      bit 5      |       bit 4       |    bit 3    |      bit 2      |           bit 1           |           bit 0             |
            //         |----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
            //      0  |             Report ID = POWER_SUMMARY_STATUS_INFO                                                                                                                    |
            //         |----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
            //      1  | ShutdownImminent | ShutdownRequested | OverTemperature | OverCharged       | Overload    | InternalFailure | Good                      | Present                     |  
            //         |----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
            //      2  |  BatteryPresent  | ACPresent         | NeedReplacement | ConditionningFlag | Discharging | Charging        | RemainingTimeLimitExpired | BelowRemainingCapacityLimit |
            //         ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //*********************************************************************
            case POWER_SUMMARY_STATUS_INFO:
                ToSendDataBuffer[0] = reportToSend;

                //Check the pushbutton
                if(sw2 == 0)
                {
                    //If the button is pressed then simulate that we are 
                    //  in a discharge state

                    //If the capacity is less than 5% then indicate that
                    //  a shutdown is imminent and request the computer shutdown
                    if(capacity < 5)
                    {
                        ToSendDataBuffer[1] = 0xC3;
                    }
                    else
                    {
                        ToSendDataBuffer[1] = 0x03;
                    }

                    //Indicate the battery is present but discharging, no AC
                    ToSendDataBuffer[2] = 0x88;
                }
                else
                {
                    //If the button isn't pressed then we are AC powered

                    ToSendDataBuffer[1] = 0x03;

                    if(capacity == 100)
                    {
                        //if capacity is at 100%, we can stop charging the
                        //  battery.  
                        ToSendDataBuffer[2] = 0xC0;
                    }
                    else
                    {
                        //if capacity is less than 100%, we need to charge the 
                        //  battery
                        ToSendDataBuffer[2] = 0xC4;
                    }
                }

                reportToSend = POWER_SUMMARY_REMAINING_CAPACITY;
                
                USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],3);
                break;

            //*********************************************************************
            //  Title:
            //    RemainingCapacity Report
            //
            //  Description:
            //    RemainingCapacity reflects the capacity left in the battery 
            //    in the units specified by the CapacityMode report
            //
            //  Format:
            //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
            //         |---------------------------------------------------------------|
            //      0  |             Report ID = POWER_SUMMARY_REMAINING_CAPACITY      |
            //         |---------------------------------------------------------------|
            //      1  |                         RemainingCapacity                     |
            //         -----------------------------------------------------------------
            //*********************************************************************
            case POWER_SUMMARY_REMAINING_CAPACITY:
                //we are using a counter here to simulate a time delay
                if(i==0)
                {
                    //if the counter has expired then send a RemainingCapacity Report
                    ToSendDataBuffer[0] = reportToSend;
                    ToSendDataBuffer[1] = capacity;
                    USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],2);
    
                    //Reset the counter
                    i=50000;

                    //If the button is pressed
                    if(old_sw2 == 0)
                    {
                        //We are discharging - no AC

                        //And we still have capacity left
                        if(capacity != 1)
                        {
                            //Decrease the remaining capacity
                            capacity--;
                        }
                        //Send the run time to empty report next
                        reportToSend = POWER_SUMMARY_RUN_TIME_TO_EMPTY;
                    }
                    else
                    {
                        //if the button isn't pressed (we are line powered)

                        //If we aren't at max capacity (100%)
                        if(capacity != 100)
                        {
                            //increase the capacity
                            capacity++;

                            reportToSend = POWER_SUMMARY_RUN_TIME_TO_EMPTY;
                        }
                        else
                        {
                            //if we are at max capacity we don't need to send
                            //  any more reports until something changes
                            reportToSend = NO_MORE_REPORTS;
                        }
                    }
                }
                else
                {
                    i--;
                }
                break;

            //*********************************************************************
            //  Title:
            //    RunTimeToEmpty Report
            //
            //  Description:
            //    This report sends the number of seconds of capacity
            //
            //  Format:
            //    byte | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
            //         |---------------------------------------------------------------|
            //      0  |             Report ID = POWER_SUMMARY_RUN_TIME_TO_EMPTY       |
            //         |---------------------------------------------------------------|
            //      1  |                         RunTimeToEmpty (byte 0)               |
            //         |---------------------------------------------------------------|
            //      2  |                         RunTimeToEmpty (byte 1)               |
            //         -----------------------------------------------------------------
            //      3  |                         RunTimeToEmpty (byte 2)               |
            //         -----------------------------------------------------------------
            //*********************************************************************
            case POWER_SUMMARY_RUN_TIME_TO_EMPTY:
            {
                WORD_VAL RunTimeToEmpty;

                //RunTimeToEmpty.Val = (60u * 60u * 1u * capacity)/100u;		//Want this but division is expensive, so we do the below instead.
                RunTimeToEmpty.Val = (36u * (WORD)capacity);
                //60 sec/min * 60 min/hr * 1hr capacity(typically) * current_capacity/100 = seconds of capacity left

                ToSendDataBuffer[0] = reportToSend;
                ToSendDataBuffer[1] = RunTimeToEmpty.v[0];
                ToSendDataBuffer[2] = RunTimeToEmpty.v[1];
                ToSendDataBuffer[3] = 0u;

                USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],4);

                reportToSend = POWER_SUMMARY_REMAINING_CAPACITY;
                break;
            }

            case NO_MORE_REPORTS:
                //Fall through
            default:
                break;
        }
    }    
}//end ProcessIO

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
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus



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
	

}




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
 *                        int event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           int event - the type of event
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
BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size)
{
    switch( event )
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

/** EOF main.c *************************************************/
#endif
