/********************************************************************
 FileName:		MultiTouch_MultiModes.c
 Dependencies:	See INCLUDES section
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
  2.5   Initial release.  Adapted from the mouse in a
        circle demo firmware included in MCHPFSUSB v2.4a

  2.6   Initial release.  Adapted from the Multi-Touch
        digitizer demo in the MCHPFSUSB v2.5b Framework.
        This version has been enhanced by supporting
        multiple device modes, for improved functionality
        in legacy operating systems.  See the comments near
        the DEFAULT_DEVICE_MODE definition in usb_config.h
        for additional details.  Also see the report
        descriptor inline comments.

  2.7   Minor changes to usb_descriptors.c (to fix USB20CV issue),
  	    and to interrupt vector handling (pertaining to clock
        switching to 31kHz INTRC during USB suspend).
        Also minor changes to code in main.c, to change the 
        demo to match the newly updated report descriptor.
        
  2.7a  This is a modified version of the HID Milti-Touch/Multi-Modes
        demo.  This version was modified from the MCHPFSUSB Framework
        v2.7 version of this project.  This version implements the 
        report descriptor changes needed to support up to 3 simultaneous         					  					 
        contact points.  The demo firmware (that emulates contacts/gestures)
        has also been updated slightly, so that the switch case 11
        sends the data for three valid contact points to the host.
        
        To use/demonstrate that all three contact points are working,
        program the firmware onto a demo board and then plug the device
        into a Microsoft Windows 7 based machine.  Then open the 
        built in Microsoft Paint program that comes with Windows 7.
        
        Standard gestures in Windows 7 do not need/nor use three contact
        points (only 2 needed).  However, MS Paint (in Windoes 7) appears
        to support recognition of more simultaneous contact points.
        
        Upon pressing the pushbutton on the demo board to cycle through
        to the Emulate_Digitizer() switch case 11, three simultaneous
        lines will appear on the drawing canvas in MS Paint, one for
        each contact point.  One moves +X direction, one moves -Y direction,
        and one moves in the +X/-Y diagonal direction.  To see this effect,
        it is best to clear the canvas of any previous markings, prior to
        pressing the pushbutton to activate switch case 11.       
        
  2.7b  Minor change to configuration descriptor and main code,
        so as to enable and demonstrate	USB remote wakeup capability.
        
  2.9f  Adding new part support

  2.9j  Updates to support new bootloader features (ex: app version 
        fetching).
********************************************************************/

/********************************************************************
To use this demo, program a device with the firmware from this project.
Then plug the device into a Windows 7 or Windows Vista SP1+ based PC.

MSDN documentation for the "WM_GESTURE" messages indicate that they are
currently only supported in Windows 7.  WM_GESTURE messages can be used
by PC applications to receive information related to a user's input.  
However, Windows Vista SP1 still has some digitizer input 
capabilities, and can therefore still be used to experiment with this demo.
The demo experience will be superior under Windows 7 however, as not
all touch related capabilities and features are enabled in Windows Vista.

The device should enumerate as a HID class multi-touch capable
digitizer device.  Windows 7 and Windows Vista features associated 
with "Windows Touch" should automatically become activated.  
Under default versions of Windows XP, the device should still enumerate 
as a HID class device, but no new digitizer input functionality will 
be activated.

Digitizer tablets send absolute contact position information
to the host.  This is a bit different compared to conventional mouse
devices, in that mice send relative X/Y direction movement information,
instead of an absolute location.

Since the demo boards that this firmware is intended to be run on
do not include an actual multi-touch tablet area, the firmware simply
emulates the tablet by sending the same data (and formatting) that a tablet
would actually be used to produce.  The demo board pushbutton "sw3"
(see definition in the HardwareProfile - (board name).h file)
can be used to make the demo board send simulated gesture info to the host.

To use this demo, open up an Internet Explorer window, and go to some website 
(ex: www.microchip.com/usb).
The web page opened should be large enough that both vertical and 
horizontal scrollbars will become active when zoomed in. Click on some 
link, and then click the back button.  The purpose of this is to enable both 
the forward and back buttons in the web broswer.  
This demo will send gesture information to the OS that can be interpreted as 
forward and back "flick" gestures, which can be used to control the web browser.

Now try pressing the pushbutton on the demo board repeatedly.  Each press
of the pushbutton should send a bunch of IN packets to the host, which contain
the information that would typically be generated by a human user interacting 
with a multi-touch capable digitizer screen.

The firmware will send traffic to emulate both single touch and multi-touch 
(up to 2 simultaneous contacts) gestures, based on the number of times the
pushbutton has been pressed.  Under Windows 7 (Vista will be somewhat different),
the following gestures will be sent and should be recognized by Internet Explorer:

1.  Emulate a single touch "back" gesture operation.  
		This is done by pressing only one finger down, and by moving it right quickly (X plus)
2.  Emulate a single touch "forward" gesture operation.  
		This is done by pressing only one finger down, and by moving it left quickly (X minus)
3.  Emulate a multi-touch zoom-in operation ("un-pinch" index and thumb fingers)
4.  Emulate a multi-touch zoom-in operation ("un-pinch" index and thumb fingers)
5.  Emulate a multi-touch zoom-in operation ("un-pinch" index and thumb fingers)
6.  Emulate a multi-touch horizontal scroll right operation 
		This is done by using two fingers simultaneously and moving them both to the left (X minus)
7.  Emulate a multi-touch horizontal scroll left operation
		This is done by using two fingers simultaneously and moving them both to the right (X plus)
8.  Emulate a single touch scroll down operation.  
		This is done by pressing only one finger down, and by moving it down (Y plus)
9.  Emulate a single touch scroll up operation.  
		This is done by pressing only one finger down, and by moving it up (Y minus)
10. Emulate a multi-touch zoom-out operation (pinch two fingers together)

11.  Emulate a condition where three contact points are all simultaneously valid.
    Contact 1 moves +X direction.  Contact 2 moves -Y direction, and Contact 3 moves
    +X/-Y direction (diagonally).  Use MS Paint in Windows 7 to watch this effect.

12+. Upon pressing the button 12 times, the firmware will go back to state 1. Subsequent 
		presses of the button will step through and repeat the above gestures 1-11.

Note: The above demo zooms in three times in a row (presses 3-5).  The purpose of this
is to zoom in sufficiently so that the horizontal and vertical scoll bars will become
active.  The gestures associated with button presses 6-9 perform scroll operations,
but only if the scroll bars are active.

Note2: This demo was tested and developed primarily on 64-bit Windows 7 beta build 7000, 
with some additional testing on Windows Vista SP1.  At the time of this development,
Windows 7 is not yet a finalized and released product.  If you have trouble getting
this demo to work correctly, check to see if there is a later version of this demo
available from the Microchip USB design center: www.microchip.com/usb

Additionally, try adjusting some of the user adjustable controls for 
Pen and Touch Input Devices.  These adjustments can be found from the control panel 
within Windows.


This demo is configured by default to support up to two simultaneous contacts,
by using "parallel reporting" (all data for each contact is contained in each
HID report packet sent to the host).  The demo can be modified to support additional
contacts if the report descriptor in usb_config.h is modified, and the appropriate
contact data is sent to the host for each HID report.


********************************************************************/

#ifndef USBMULTITOUCH_MULTI_MODES_C
#define USBMULTITOUCH_MULTI_MODES_C

/** INCLUDES *******************************************************/
#include "./USB/usb.h"
#include "HardwareProfile.h"
#include "./USB/usb_function_hid.h"

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
//	PIC18F14K50
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
    #if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50)
        #pragma udata HID_VARS=0x260
    #elif defined(__18F2455) || defined(__18F2550) || defined(__18F4455) || defined(__18F4550)\
        || defined(__18F2458) || defined(__18F2453) || defined(__18F4558) || defined(__18F4553)\
        || defined(__18LF24K50) || defined(__18F24K50) || defined(__18LF25K50)\
        || defined(__18F25K50) || defined(__18LF45K50) || defined(__18F45K50)
        #pragma udata USB_VARIABLES=0x500
    #elif defined(__18F4450) || defined(__18F2450)
        #pragma udata USB_VARIABLES=0x480
    #else
        #pragma udata
    #endif
#endif

#if defined(__XC8)
    #if defined(_18F14K50) || defined(_18F13K50) || defined(_18LF14K50) || defined(_18LF13K50)
        #define IN_DATA_BUFFER_ADDRESS 0x260
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + HID_INT_IN_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
    #elif  defined(_18F2455)   || defined(_18F2550)   || defined(_18F4455)  || defined(_18F4550)\
        || defined(_18F2458)   || defined(_18F2453)   || defined(_18F4558)  || defined(_18F4553)\
        || defined(_18LF24K50) || defined(_18F24K50)  || defined(_18LF25K50)\
        || defined(_18F25K50)  || defined(_18LF45K50) || defined(_18F45K50)
        #define IN_DATA_BUFFER_ADDRESS 0x500
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + HID_INT_IN_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
    #elif defined(_18F4450) || defined(_18F2450)
        #define IN_DATA_BUFFER_ADDRESS 0x480
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + HID_INT_IN_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
    #elif defined(_16F1459)
        #define IN_DATA_BUFFER_ADDRESS 0x20A0
        #define OUT_DATA_BUFFER_ADDRESS 0x20F0
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
    #else
        #define IN_DATA_BUFFER_ADDRESS_TAG
        #define OUT_DATA_BUFFER_ADDRESS_TAG
    #endif
#else
    #define IN_DATA_BUFFER_ADDRESS_TAG
    #define OUT_DATA_BUFFER_ADDRESS_TAG
#endif

unsigned char hid_report_in[HID_INT_IN_EP_SIZE] IN_DATA_BUFFER_ADDRESS_TAG;
unsigned char hid_report_out[HID_INT_OUT_EP_SIZE] OUT_DATA_BUFFER_ADDRESS_TAG;

#if defined(__18CXX)
    #pragma udata
#endif

BYTE UIESave;
WORD led_count;
WORD DebounceCounter;
BOOL SwitchDebouncing;
BYTE TimesPressed;		//Keeps track of how many times the button was pressed, to decide what to do next
BOOL FlickPending;		//TRUE if we are busy sending a flick, FALSE if no data is being sent right now
BYTE ReportsSent;
BOOL CoordinatesChanging;
WORD XCoordinate1;
WORD YCoordinate1;
WORD XCoordinate2;
WORD YCoordinate2;
WORD XCoordinate3;
WORD YCoordinate3;
WORD XOrigin1;
WORD YOrigin1;
WORD XOrigin2;
WORD YOrigin2;
WORD XOrigin3;
WORD YOrigin3;
WORD SOFCount;			//Can get updated in interrupt context if using USB interrupts instead of polling
WORD SOFCountIntSafe;	//Shadow holding variable for the SOFCounter.  This number will not get updated in the interrupt context.
WORD SOFCountIntSafeOld;
USB_HANDLE lastTransmission;
//Mouse emulation mode variables
BOOL EmulateMouseMovement;
BYTE movement_length;
BYTE vector;
//The direction that the mouse will move in
ROM signed char dir_table[]={-4,-4,-4, 0, 4, 4, 4, 0};

BOOL HIDApplicationModeChanging;
BYTE DeviceIdentifier;
BYTE DeviceMode;
//DeviceMode variable values.  See also the usb_config.h "DEFAULT_DEVICE_MODE" definition.
#define MOUSE_MODE 					0x00
#define SINGLE_TOUCH_DIGITIZER_MODE	0x01
#define MULTI_TOUCH_DIGITIZER_MODE	0x02

#if !defined(pBDTEntryIn)
	extern volatile BDT_ENTRY *pBDTEntryIn[USB_MAX_EP_NUMBER+1];
#endif

/** PRIVATE PROTOTYPES *********************************************/
void BlinkUSBStatus(void);
void DebounceSwitch3(void);
void Emulate_Digitizer(void);
void Emulate_Mouse(void);
void CheckForOverUnderFlow(void);
static void InitializeSystem(void);
void ProcessIO(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void USBCBSendResume(void);

void UserSetReportHandler(void);
void USBHIDCBSetReportComplete(void);

/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
	//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
	//the reset, high priority interrupt, and low priority interrupt
	//vectors. However, if using the HID Bootloader, the bootloader
	//firmware will occupy the 0x00-0xFFF region.  Therefore,
	//the bootloader code remaps these vectors to new locations:
	//0x1000, 0x1008, 0x1018 respectively.  This remapping is only necessary 
	//if you wish to program the hex file generated from this project with
	//the USB bootloader. 
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
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}


	//--------------------------------------------------------------------
	//NOTE: See PIC18InterruptVectors.asm for important code and details
	//relating to the interrupt vectors and interrupt vector remapping.
	//Special considerations apply if clock switching to the 31kHz INTRC 
	//during USB suspend.
	//--------------------------------------------------------------------	


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
    led_count = 0x0000;		//Initialize LED blink counter to 0 for consistant inital behavior.
    
    //Initialize all of the push buttons
    mInitAllSwitches();

	//Used for sw3 pushbutton debouncing
    TimesPressed = 0;		//Tracks number of times the pushbutton is pressed
    DebounceCounter = 0;	//Used for pushbutton debounce
    SwitchDebouncing = FALSE;

    //initialize the variable holding the handle for the last
    // transmission
    lastTransmission = 0;

	HIDApplicationModeChanging = FALSE;

	//Initialize device mode and digitizer emulation variables.
	//--------------------------------------------------------
	DeviceIdentifier = 0x01;
	DeviceMode = DEFAULT_DEVICE_MODE;	//Set the device mode (ex: mouse, single-touch digitizer, multi-touch digitizer) to a default value.  Set the default in the usb_config.h file.
    FlickPending = FALSE; 				//Not currently sending a gesture
    
	//Initialize Mouse mode variables
	//--------------------------------------------------------
    movement_length = 0xFF;
	//State variable for deciding if the mouse data sent to the host should
	//move the cursor or not.
    EmulateMouseMovement = TRUE;
    
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
    BlinkUSBStatus();

    //Check if we should send USB remote wakeup signalling to the host.
    if(sw3 == 0)    //User is pressing pushbutton
    {
        USBCBSendResume();  //Note: This function does nothing and returns 
                            //immediately, if we are not currently in a remote
                            //wakeup capable/armed bus state.
    }   

    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	//Switch sw3 is used for various user inputs in all of the device modes 
	//(ex: mouse, single-touch digitizer, multi-touch digitizer).  Check if sw3
	//needs debouncing and if so, perform non-blocking debounce operation.
	DebounceSwitch3();	

	//Don't want to send any report packets on EP1 IN to the host when
	//the host is in the process of sending a control transfer (ex: SET_REPORT)
	//and is changing the device mode.  Need to wait until the control transfer
	//is complete and the new device mode setting has taken effect.
	if(HIDApplicationModeChanging == TRUE)
	{
		return;	
	}	

	//Check what HID device mode we are currently in, to decide what type of reports
	//should be sent to the host. 
	if(DeviceMode == MOUSE_MODE)
	{
		//If the user presses the sw3 pushbutton, start or stop sending mouse 
		//movement data
	    if((sw3 == 0) && (SwitchDebouncing == FALSE))
	    {
		    SwitchDebouncing = TRUE;
		    //If the mouse was moving, stop moving the cursor, if it wasn't 
		    //moving, start sending data to move mouse cursor in a circle
		    EmulateMouseMovement = !EmulateMouseMovement;
		} 
		
		//Prepare the mouse data and send it to the host.
		Emulate_Mouse();
		
	}//if(DeviceMode == MOUSE_MODE)
	else if((DeviceMode == MULTI_TOUCH_DIGITIZER_MODE) || (DeviceMode == SINGLE_TOUCH_DIGITIZER_MODE))
	{	
		//Need to be careful when reading the SOFCounter variable.  If USB interrupts are enabled,
		//the SOFCounter can potentially get updated while you are still reading from it.
	    USBMaskInterrupts();
		SOFCountIntSafe = SOFCount;		
	    USBUnmaskInterrupts();	
		//We use the SOFCounterIntSafe variable as a form of clock.  Microsoft's
		//Windows Touch related docoumentation implies that Windows can use 
		//"additional heuristics" in determining if data sent from a touch device
		//is valid or not.  If for instance the reported contact point suddenly 
		//jumps a great distance (apparent velocity greatly exceeding anything that 
		//a real human contact would produce), the operating system may conceivably 
		//choose to discard the evidently impossible data.
		//Therefore, it is important that data sent to the host be "realistic"
		//to ensure that it gets processed correctly.  In order to make this
		//easier to accomplish in this demo, we use the SOF packets (sent once every 1ms
		//on a full speed USB bus) as a timer, so that we can use it to vary our reported
		//contact point X and Y coordinates in a smooth real time "believable" (constant 
		//velocity) manner.  In a real application with a real digitizer touch pad, it 
		//may not necessarily be required to independently keep track of "time", since 
		//presumably the input data is being generated in a realisitic real-time fashion
		//by the human interacting with it.  Specific requirements will however vary 
		//based on the type of touch contact point sensing technology employed (ex: 
		//optical, capacitive, resistive, etc.).
	
		//Check if the pushbutton is currently pressed.
		//If the user presses the pushbutton, send a flurry of packets emulating a 
		//touch (or multi-touch) gesture
	    if((sw3 == 0) && (SwitchDebouncing == FALSE))		
	    {	
			SwitchDebouncing = TRUE;
			FlickPending = TRUE;	
			ReportsSent = 0;
		    USBMaskInterrupts();
			SOFCount = 0;
		    USBUnmaskInterrupts();
			SOFCountIntSafe = 0;		
			SOFCountIntSafeOld = 0;
			CoordinatesChanging = TRUE;
			//Number of times the pushbutton has been pressed by the user.
			//TimesPressed determines what gesture will be sent to the host.
			TimesPressed++;				  
			if(TimesPressed >= 12u)
			{
				TimesPressed = 1; //Go back to first gesture to repeat the sequence.
			}	
	
			//Emulated thumb finger contact point originating at center of screen
			XOrigin1 = 2400u;	//From report descriptor, valid X-coordinate values are 0-4800
			YOrigin1 = 1500u;	//From report descriptor, valid Y-coordinate values are 0-3000
			
			//Emulated index finger positioned to upper right of thumb	
			XOrigin2 = XOrigin1 + 400u;
			YOrigin2 = YOrigin1 - 400u;

            //Place third contact point somwhere else (in this case upper left of contact 1), 
            //far enough away from contact 1 and contact 2, so that the PC software knows 
            //it is a unique and sensible/valid contact point.  
			XOrigin3 = XOrigin1 - 1000u;
			YOrigin3 = YOrigin1 - 800u;
		
			XCoordinate1 = XOrigin1;
			YCoordinate1 = YOrigin1;
			XCoordinate2 = XOrigin2;
			YCoordinate2 = YOrigin2;			
			XCoordinate3 = XOrigin3;
			YCoordinate3 = YOrigin3;			
	    }
	
		//Send data for a gesture if needed
		if(FlickPending == TRUE)
		{
			Emulate_Digitizer();   
		}
		
	}//else if(DeviceMode == MULTI_TOUCH_DIGITIZER_MODE)


}//end ProcessIO


/******************************************************************************
 * Function:        void DebounceSwitch3(void)
 *
 * PreCondition:    Switch sw3 should be already initialized.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    
 *
 * Overview:        This is a non-blocking function that checks the sw3 pin
 *					state.  It uses a counter to make sure the user is no longer
 *					pressing the button, and the value has been steady for a long
 *					time before it "re-enables" the button so that it may
 *					register the next user press action.
 *
 * Note:            
 *****************************************************************************/
void DebounceSwitch3(void)
{
	//Perform non-blocking switch debounce operation.
	//User must remove finger from button for awhile before the button becomes
	//active again.
	if(sw3 == 0)
	{
		//Immediately reset the counter if the pin is (still) low.
		DebounceCounter = 0;	
	}
    else if((sw3 == 1) && (SwitchDebouncing == TRUE))
    {
	    //Keep track of how long it has been continuously pressed without interruption.
	    DebounceCounter++;	
	}
	if((SwitchDebouncing == TRUE) && (DebounceCounter == 1000))
	{
		//Button was continuously released long enough that it must be done bouncing
		DebounceCounter = 0;
		SwitchDebouncing = FALSE;	
	}	
}	

/******************************************************************************
 * Function:        void Emulate_Mouse(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    The ownership of the USB buffers will change according
 *                  to the required operation
 *
 * Overview:        This routine will emulate the function of the mouse.  It
 *					does this by sending IN packets of data to the host.
 *					
 *					The generic function HIDTxPacket() is used to send HID
 *					IN packets over USB to the host.
 *
 * Note:            The HID input reports for this mode should be formatted as
 					follows (based on the report descriptor top level mouse 
 					collection):
 					
	//byte[0] = Report ID = MOUSE_DATA_REPORT_ID
	//byte[1] = contains bit fields for various input information typically generated by mouse buttons or touch pad tap operations. '1' is the active value (ex: pressed), '0' is the non active value
	//		bit0 = Button 1. 
	//		bit1 = Button 2.  
	//		bit2 = Button 3.
	//		bits3-7 = pad bits, no valid data.
	//byte[2] = Relative X coordinate change since the last report was sent
	//byte[3] = Relative Y coordinate change since the last report was sent
 *
 *****************************************************************************/
void Emulate_Mouse(void)
{   
	//This function sends packets to the host that make the mouse cursor
	//move in a semi-circular pattern.  In a real application, which has been
	//designed to support multiple device modes (to include mouse mode), the
	//input packet data would typically be generated from the touch-screen
	//user interface.  Since a mouse does not report absolute position 
	//information to the host (it only sends relative change in X and Y
	//coordinates since the last packet, instead of absolute X and Y 
	//coordinates), this firmware should be replaced with code that uses the
	//touch interface to function like a touch pad.

	//Make sure that the endpoint is not busy before modifying the hid_report_in buffer.
    if(!USBHandleBusy(lastTransmission))
    {
		//Prepare the HID input report containing the mouse movement data.
		hid_report_in[0] = MOUSE_DATA_REPORT_ID;		//This is the report ID byte.  Mouse report ID = 0x04 from report descriptor.
		
	    if(EmulateMouseMovement == TRUE)
	    {
	        //go 14 times in the same direction before changing direction
	        if(movement_length > 14u)
	        {
	            hid_report_in[1] = 0u;								//Button click data
	            hid_report_in[2] = dir_table[vector & 0x07];           // X-Vector
	            hid_report_in[3] = dir_table[(vector+2) & 0x07];       // Y-Vector
	            //go to the next direction in the table
	            vector++;
	            //reset the counter for when to change again
	            movement_length = 0;
	        }//end if(movement_length > 14u)
	    }
	    else
	    {
	        //don't move the mouse
	        hid_report_in[1] = hid_report_in[2] = hid_report_in[3] = 0;
	    }

        //Send the 4 byte packet over USB to the host.
        lastTransmission = HIDTxPacket(HID_EP, (BYTE*)hid_report_in, 0x04);

        //increment the counter of when to change the data sent
        movement_length++;

	}//if(!USBHandleBusy(lastTransmission))
}//end Emulate_Mouse


/******************************************************************************
 * Function:     	void Emulate_Digitizer(void)
 *
 * PreCondition: 	None
 *
 * Input:        	None
 *
 * Output:       	None
 *
 * Side Effects: 	The ownership of the USB buffers will change according
 *               	to the required operation
 *
 * Overview:     	This routine will emulate the function of a human interacting
 *					with a touch digitizer panel.  It does this by sending IN packets 
 *					of data to the host, containing data simulating that which would 
 *					be generated by real human contact(s).
 *					
 *					The generic function HIDTxPacket() is used to send HID
 *					IN packets over USB to the host.
 *					
 *					This function is used to emulate the digitizer behavior, in
 *					both the single-touch and multi-touch device modes (DeviceMode variable).
 *					The formatting and content of the data being sent to the host depends
 *					on the current device mode.  On OSes that support multi-touch
 *					capability, the host will configure the DeviceMode to
 *					the MULTI_TOUCH_DIGITIZER_MODE. By default however, it is best
 *					if the device powers up with some other default mode (mouse mode
 *					or single-touch digizer), so as to provide improved functionality
 *					on legacy operating systems (that don't support multi-touch
 *					digitizers, and would not know how to interpret/use the reports).
 * 
 *
 * Note:   			Reports of data should be sent to the host matching the Report 
 *					Descriptor and the current DeviceMode.
 *					The HID report (IN) packets sent to the host should be formatted like the
 *					report descriptor in the usb_descriptors.c file.  See the usb_descriptors.c
 *					file for additional comments.  For this demo, we use the following report 
 *					format:
 *					         
 *	//Report format for use in multi-touch device mode, which can support up to two
 *	//simultaneous contacts, using the report descriptor provided in usb_descriptors.c:
 *		
 *	//Byte[0] = Report ID == use literal value = MULTI_TOUCH_DATA_REPORT_ID when sending contact reports.
 * 	//First contact point info in bytes 1-6.
 *	//Byte[1] = Bits7-2: pad bits (unused), Bit1:In Range, Bit0:Tip Switch
 *	//Byte[2] = Contact identifier number (assigned by firmware, see report descriptor)
 *	//Byte[3] = X-coordinate LSB
 *	//Byte[4] = X-coordinate MSB
 *	//Byte[5] = Y-coordinate LSB
 *	//Byte[6] = Y-coordinate MSB
 *
 *	//Second contact point info in bytes 7-12
 *	//Byte[7] = Bits7-2: pad bits (unused), Bit1:In Range, Bit0:Tip Switch
 *	//Byte[8] = Contact identifier number (assigned by firmware, see report descriptor)
 *	//Byte[9] = X-coordinate LSB
 *	//Byte[10]= X-coordinate MSB
 *	//Byte[11]= Y-coordinate LSB
 *	//Byte[12]= Y-coordinate MSB
 *
 *	//Third contact point info in bytes 13-18
 *	//Byte[13] = Bits7-2: pad bits (unused), Bit1:In Range, Bit0:Tip Switch
 *	//Byte[14] = Contact identifier number (assigned by firmware, see report descriptor)
 *	//Byte[15] = X-coordinate LSB
 *	//Byte[16] = X-coordinate MSB
 *	//Byte[17] = Y-coordinate LSB
 *	//Byte[18] = Y-coordinate MSB
 *
 *	//Contacts valid
 *	//Byte[19]= 8-bit number indicating how many of the above contact points are valid.  
 *				If only the first contact is valid, report "1" here.  If 2 are 
 *				valid, report "2".  If three are valid, report "3".
 *
 *	When operating in the DeviceMode == SINGLE_TOUCH_DIGITIZER_MODE
 *	The input data reports are different.  In this mode, the device should only keep
 *	track of and send information to the host for the first contact point.  In this
 *	mode, the report sent to the host should be formatted as:
 *	//byte[0] = Report ID = SINGLE_TOUCH_DATA_REPORT_ID
 *	//byte[1] = contains bit fields for various input information typically generated by an input pen or human finger. '1' is the active value (ex: pressed), '0' is the non active value
 *	//		bit0 = Tip switch. At the end of a pen input device would normally be a pressure senstive switch.  Asserting this performs an operation analogous to a "left click" on a mouse
 *	//		bit1 = In range indicator.  
 *	//		bit2 though bit7 = Pad bits.  Values not used for anything.
 *	//byte[2] = Pad byte.  Value not used for anything.
 *	//byte[3] = X coordinate LSB value of contact point
 *	//byte[4] = X coordinate MSB value of contact point
 *	//byte[5] = Y coordinate LSB value of contact point
 *	//byte[6] = Y coordinate MSB value of contact point
 * 
 *****************************************************************************/
void Emulate_Digitizer(void)
{   
	//Make sure the endpoint buffer (in this case hid_report_in[] is not busy 
	//before we modify the contents of the buffer for the next transmission.
    if(!USBHandleBusy(lastTransmission))		
    {
        //Find the time delay in milliseconds (number of SOF packets) since 
        //the last transmission.  We will use this delta number to establish 
        //how much our X/Y coordinates should change (for simulated constant 
        //velocity movement).
        WORD SOFDelta = (SOFCountIntSafe - SOFCountIntSafeOld);	

		//Report ID for multi-touch contact information reports (based on report descriptor)
		hid_report_in[0] = MULTI_TOUCH_DATA_REPORT_ID;		//Report ID in byte[0]
	
		hid_report_in[1] = 0b00000010;	//Contact1: In-range, but tip switch not pressed.
		hid_report_in[7] = 0b00000010;	//Contact2: In-range, but tip switch not pressed.
		hid_report_in[13]= 0b00000010;	//Contact3: In-range, but tip switch not pressed.
		hid_report_in[19] = 2;			//Assume two contacts valid. (unless we set hid_report_in[19] = 1 or 3 later on in this function)
	
		//Decide how (or if) we want to change our X and Y Coordinates.
		if(CoordinatesChanging == TRUE)
		{
			hid_report_in[1] = 0b00000011;	//Contact1: Tip switch and in range indicators set.
			hid_report_in[7] = 0b00000011;	//Contact2: Tip switch and in range indicators set.
    		hid_report_in[13]= 0b00000011;	//Contact3: Tip switch and in range indicators set.

			//Decide what type of data to send to host, based on how many times the
			//user pressed the pushbutton.
			switch(TimesPressed)	
			{
				case 1:	//Emulate a single touch "back" flick operation.  
					//This is done by pressing only one finger down, and by moving it right quickly (X plus)
					SOFDelta = SOFDelta * 8;	//To get faster movement, we scale our time delay.
					XCoordinate1 = XCoordinate1 + SOFDelta;
					if(XCoordinate1 > (XOrigin1 + 246u))
						CoordinatesChanging = FALSE;
					hid_report_in[19] = 1;		//Only the first contact point is valid	(this is a single touch gesture)
					break;	
				case 2:	//Emulate a single touch "forward" flick operation.  
					//This is done by pressing only one finger down, and by moving it left quickly (X minus)
					SOFDelta = SOFDelta * 8;	//To get faster movement, we scale our time delay.
					XCoordinate1 = XCoordinate1 - SOFDelta;
					if(XCoordinate1 < (XOrigin1 - 246u))
						CoordinatesChanging = FALSE;
					hid_report_in[19] = 1;		//Only the first contact point is valid	(this is a single touch gesture)
					break;	
				case 3:	//Emulate a multi-touch zoom-in operation ("un-pinch" index and thumb fingers)
					//Sweep Contact1 (emulated thumb finger) coordinates to move lower left (X minus, Y plus)
					SOFDelta = SOFDelta >> 1;	//To get slower movement, we scale our time delay.
					XCoordinate1 = XCoordinate1 - SOFDelta;
					if(XCoordinate1 < (XOrigin1 - 352u))
						CoordinatesChanging = FALSE;	
					YCoordinate1 = YCoordinate1 + SOFDelta;
					if(YCoordinate1 > (YOrigin1 + 300u))
						CoordinatesChanging = FALSE;	
					//Sweep Contact2 (emulated index finger) coordinates to move upper right (X plus, Y minus)
					XCoordinate2 = XCoordinate2 + SOFDelta;
					if(XCoordinate2 > (XOrigin2 + 352u))
						CoordinatesChanging = FALSE;	
					YCoordinate2 = YCoordinate2 - SOFDelta;
					if(YCoordinate2 < (YOrigin2 - 300u))
						CoordinatesChanging = FALSE;							
					break;
				case 4:	//Emulate a multi-touch zoom-in operation ("un-pinch" index and thumb fingers)
					//Sweep Contact1 (emulated thumb finger) coordinates to move lower left (X minus, Y plus)
					SOFDelta = SOFDelta >> 1;	//To get slower movement, we scale our time delay.
					XCoordinate1 = XCoordinate1 - SOFDelta;
					if(XCoordinate1 < (XOrigin1 - 352u))
						CoordinatesChanging = FALSE;	
					YCoordinate1 = YCoordinate1 + SOFDelta;
					if(YCoordinate1 > (YOrigin1 + 300u))
						CoordinatesChanging = FALSE;	
					//Sweep Contact2 (emulated index finger) coordinates to move upper right (X plus, Y minus)
					XCoordinate2 = XCoordinate2 + SOFDelta;
					if(XCoordinate2 > (XOrigin2 + 352u))
						CoordinatesChanging = FALSE;	
					YCoordinate2 = YCoordinate2 - SOFDelta;
					if(YCoordinate2 < (YOrigin2 - 300u))
						CoordinatesChanging = FALSE;							
					break;
				case 5:	//Emulate a multi-touch zoom-in operation ("un-pinch" index and thumb fingers)
					//Sweep Contact1 (emulated thumb finger) coordinates to move lower left (X minus, Y plus)
					SOFDelta = SOFDelta >> 1;	//To get slower movement, we scale our time delay.
					XCoordinate1 = XCoordinate1 - SOFDelta;
					if(XCoordinate1 < (XOrigin1 - 352u))
						CoordinatesChanging = FALSE;	
					YCoordinate1 = YCoordinate1 + SOFDelta;
					if(YCoordinate1 > (YOrigin1 + 300u))
						CoordinatesChanging = FALSE;	
					//Sweep Contact2 (emulated index finger) coordinates to move upper right (X plus, Y minus)
					XCoordinate2 = XCoordinate2 + SOFDelta;
					if(XCoordinate2 > (XOrigin2 + 352u))
						CoordinatesChanging = FALSE;	
					YCoordinate2 = YCoordinate2 - SOFDelta;
					if(YCoordinate2 < (YOrigin2 - 300u))
						CoordinatesChanging = FALSE;							
					break;
				case 6:	//Emulate a two-finger horizontal scroll right operation
					//Tap both fingers down simultaneously and move them both to the left (X minus)
					XCoordinate1 = XCoordinate1 - SOFDelta;
					if(XCoordinate1 < (XOrigin1 - 586u))
						CoordinatesChanging = FALSE;
					XCoordinate2 = XCoordinate2 - SOFDelta;
					if(XCoordinate2 < (XOrigin2 - 586u))
						CoordinatesChanging = FALSE;
					break;
				case 7:	//Emulate a two-finger horizontal scroll left operation
					//Tap both fingers down simultaneously and move them both to the right (X plus)
					XCoordinate1 = XCoordinate1 + SOFDelta;
					if(XCoordinate1 > (XOrigin1 + 586u))
						CoordinatesChanging = FALSE;
					XCoordinate2 = XCoordinate2 + SOFDelta;
					if(XCoordinate2 > (XOrigin2 + 586u))
						CoordinatesChanging = FALSE;
					break;
				case 8:	//Emulate a single touch scroll down operation.  
					//This is done by pressing only one finger down, and by moving it down (Y plus)
					YCoordinate1 = YCoordinate1 + SOFDelta;
					if(YCoordinate1 > (YOrigin1 + 500u))
						CoordinatesChanging = FALSE;
					hid_report_in[13] = 1;		//Only the first contact point is valid	
					break;	
				case 9:	//Emulate a single touch scroll up operation.  
					//This is done by pressing only one finger down, and by moving it up (Y minus)
					YCoordinate1 = YCoordinate1 - SOFDelta;
					if(YCoordinate1 < (YOrigin1 - 500u))
						CoordinatesChanging = FALSE;
					hid_report_in[19] = 1;		//Only the first contact point is valid	
					break;	
				case 10://Emulate a multi-touch zoom-out operation (pinch two fingers together)
					//Sweep Contact1 (emulated thumb) coordinates to move upper right (X plus, Y minus)
					SOFDelta = SOFDelta >> 1;	//To get slower movement, we scale our time delay.
					XCoordinate1 = XCoordinate1 + SOFDelta;
					if(XCoordinate1 > (XOrigin1 + 352u))
						CoordinatesChanging = FALSE;	
					YCoordinate1 = YCoordinate1 - SOFDelta;
					if(YCoordinate1 < (YOrigin1 - 300u))
						CoordinatesChanging = FALSE;	
					//Sweep Contact2 (emulated index finger) coordinates to move lower left (X minus, Y plus)
					XCoordinate2 = XCoordinate2 - SOFDelta;
					if(XCoordinate2 < (XOrigin2 - 352u))
						CoordinatesChanging = FALSE;	
					YCoordinate2 = YCoordinate2 + SOFDelta;
					if(YCoordinate2 > (YOrigin2 + 300u))
						CoordinatesChanging = FALSE;							
					break;
				case 11:    //Three contact point valid example case.  Since Windows 7 built in gestures only need/use 2 contact points,
				            //some other PC application (which is capable of using the WM_GESTURE and other windows messages) to receive
				            //all three contact points will be necessary to demonstrate this functionality.
				    SOFDelta = SOFDelta >> 1;	//To get slower movement, we scale our time delay.
                    //For this example case, lets sweep contact 1 from left to right (+X direction).  For contact 2,
                    //lets sweep contact in the -Y direction (towards the top of the screen).  For contact 3,
                    //lets sweep the contact point in both the +X and -Y directions (diagonally up/right of screen).
                    XCoordinate1 = XCoordinate1 + SOFDelta;
                    YCoordinate2 = YCoordinate2 - SOFDelta;
                    XCoordinate3 = XCoordinate3 + SOFDelta;
                    YCoordinate3 = YCoordinate3 - SOFDelta;
				    hid_report_in[19] = 3;      //Three contacts valid.
				    break;
				default:
					break;
			} //end switch(TimesPressed)		
		} //end if(CoordinatesChanging == TRUE)

		//Make sure none of the above math operations inadvertently exceeded the 
		//dimensions of our touch panel.
		CheckForOverUnderFlow();	
	
		//First contact info in bytes 1-6
		hid_report_in[2] = 1;						//Contact ID
		hid_report_in[3] = XCoordinate1;			//X-coord LSB
		hid_report_in[4] = (XCoordinate1 >> 8);		//X-coord MSB
		hid_report_in[5] = YCoordinate1;			//Y-coord LSB
		hid_report_in[6] = (YCoordinate1 >> 8);		//Y-coord MSB

		//Second contact info in bytes 7-12
		//hid_report_in[] bytes 0, 1, 7, and 13 were initialized earlier in this function
		if(DeviceMode == MULTI_TOUCH_DIGITIZER_MODE)
		{
    		//Second contact point in bytes 7-12
			hid_report_in[8] = 2;						//Contact ID
			hid_report_in[9] = XCoordinate2;			//X-coord LSB
			hid_report_in[10] = (XCoordinate2 >> 8);	//X-coord MSB
			hid_report_in[11] = YCoordinate2;			//Y-coord LSB
			hid_report_in[12] = (YCoordinate2 >> 8);	//Y-coord MSB

    		//Third contact point in bytes 13-18
			hid_report_in[14] = 3;						//Contact ID
			hid_report_in[15] = XCoordinate3;			//X-coord LSB
			hid_report_in[16] = (XCoordinate3 >> 8);	//X-coord MSB
			hid_report_in[17] = YCoordinate3;			//Y-coord LSB
			hid_report_in[18] = (YCoordinate3 >> 8);	//Y-coord MSB

			//Arm the endpoint so that the packet can be sent over USB to the host.
			//The HIDTxPacket() function initializes the BDT entry for the specified endpoint.
			//Always make sure that the USBHandleBusy() returned false before calling
			//the HIDTxPacket() function (this check was done at the top of Emulate_Digitizer()).
	       	lastTransmission = HIDTxPacket(HID_EP, (BYTE*)hid_report_in, 20);

		}
		else //must be DeviceMode == SINGLE_TOUCH_DIGITIZER_MODE
		{
			hid_report_in[0] = SINGLE_TOUCH_DATA_REPORT_ID;		//Report ID in byte[0].
			//The hid_report_in[2] byte is the contact ID in MULTI_TOUCH_DIGITIZER_MODE.  
			//In SINGLE_TOUCH_DIGITIZER_MODE, this is a pad byte with no valid data.
			hid_report_in[2] = 0x00;
			
			//Send the packet now.
			lastTransmission = HIDTxPacket(HID_EP, (BYTE*)hid_report_in, 7);
		}		
		

       	//Counter used to keep track of how many packets have been sent, to decide what to do next
       	ReportsSent++;
	   	if(ReportsSent == 70)
	   	{
		   	//but we still want to send a few more packets with the tip switch off, and no movement
	   		CoordinatesChanging = FALSE;	
	   	}
		if(ReportsSent == 80)
		{
			//We finished sending the flick + "idle".  No need to send more packets for now.
			FlickPending = FALSE;			
		}
		
		//Save the SOF (time) value so we can use it to know the elapsed time 
		//until the next instance that Emulate_Digitizer() gets called.
		SOFCountIntSafeOld = SOFCountIntSafe;	
	
	}//end if(HIDTxHandleBusy(lastTransmission) == 0)
    
}//end Emulate_Digitizer


void CheckForOverUnderFlow(void)
{
	//Code to check for overflow and underflow conditions, to make sure we aren't
	//inadvertently trying to report out of range X or Y coordinates.
	
	//If underflow occurred, the MSb (negative) bit will be set. The coordinates
	//are stored as WORDs (16-bit unsigned integers)
	if(XCoordinate1 & 0x8000)
		XCoordinate1 = 0x0000;
	if(YCoordinate1 & 0x8000)	
		YCoordinate1 = 0x0000;	
	if(XCoordinate2 & 0x8000)
		XCoordinate2 = 0x0000;
	if(YCoordinate2 & 0x8000)	
		YCoordinate2 = 0x0000;	
	if(XCoordinate3 & 0x8000)
		XCoordinate3 = 0x0000;
	if(YCoordinate3 & 0x8000)	
		YCoordinate3 = 0x0000;	
	
	//Check for overflow conditions.  If overflow occurred, the coordinate will 
	//be higher than the allowed values indicated in the report descriptor.
	if(XCoordinate1 > 4800u)
		XCoordinate1 = 4800u;
	if(YCoordinate1 > 3000u)
		YCoordinate1 = 3000u;				
	if(XCoordinate2 > 4800u)
		XCoordinate2 = 4800u;
	if(YCoordinate2 > 3000u)
		YCoordinate2 = 3000u;	
	if(XCoordinate3 > 4800u)
		XCoordinate3 = 4800u;
	if(YCoordinate3 > 3000u)
		YCoordinate3 = 3000u;	
}	




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
    #define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if(USBSuspendControl == 1)	//If device is in USB suspend, then need to
    {							//make LEDs take minimal average power.
    	if(led_count == 0)
    		led_count = 5;
    	led_count--;

	    mLED_Both_Off();
	    if(led_count==0)
	    {
		    mLED_Both_On();		//Very breifly turn on the LEDs, with very low duty cycle
		    Nop();				//so as to avoid consuming excess average current.
		    Nop();
   		    mLED_Both_Off();		
		}  
    }
    else
    { 
    	if(led_count == 0)
		{
    		led_count = 10000U;
    	}
    	led_count--;

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

	//Alternatively, the microcontorller may use clock switching to reduce current consumption.
	#if defined(__18CXX)
	//Save state of enabled interrupt enable bits.
	UIESave = UIE;
	
	//Disable all interrupt enable bits (except UIR, ACTVIF, and UIR, URSTIF for receiving the USB resume signalling)
	UIE = 0x05;		//ACTVIF and URSTIF still enabled.  All others disabled.
	
	//Configure device for low power consumption
	mLED_1_Off();
	mLED_2_Off();
	led_count = 3;
	#if defined(OSCTUNE)
		OSCTUNEbits.INTSRC = 0;		//31kHz from INTRC, less accurate but lower power
	#endif
	
	#if !defined(PIC18F97J94_FAMILY)
	    OSCCON = 0x03;		//Sleep on sleep, 31kHz selected as microcontroller clock source
	#else
	    OSCCON = 0x06;      //Sleep on sleep, 500kHz from FRC selected
	#endif
	
	//Should configure all I/O pins for lowest power consumption.
	//Typically this is done by driving unused I/O pins as outputs and driving them high or low.
	//In this example, this is not done however, in case the user is expecting the I/O pins
	//to remain tri-state and has hooked something up to them.
	//Leaving the I/O pins floating will waste power and should not be done in a
	//real application.
	#endif

	//IMPORTANT NOTE: If the microcontroller goes to sleep in the above code, do not
	//clear the USBActivityIF (ACTVIF) bit here (after waking up/resuming from sleep).  This bit is 
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
	// 10+ milliseconds of wakeup time, after which the device must be 
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).  
	// Make sure the selected oscillator settings are consistant with USB operation 
	// before returning from this function.

	//Switch clock back to main clock source necessary for USB operation
	//Previous clock source was something low frequency as set in the 
	//USBCBSuspend() function.
	#if defined(__18CXX)
        #if defined(PIC18F97J94_FAMILY)
            OSCCON3 = 0x01; //8MHz FRC / 2 = 4MHz output
            OSCCON = 0x01;  //FRC with PLL selected 
            while(OSCCON2bits.LOCK == 0);   //Wait for PLL lock       
    	#elif defined(PIC18F45K50_FAMILY)
            OSCCON = 0x70;  //Switch to 16MHz HFINTOSC (+ PLL)
            while(OSCCON2bits.PLLRDY != 1);   //Wait for PLL lock
        #else
        	OSCCON = 0x60;		//Primary clock source selected.
            //Adding a software start up delay will ensure
            //that the primary oscillator and PLL are running before executing any other
            //code.  If the PLL isn't being used, (ex: primary osc = 48MHz externally applied EC)
            //then this code adds a small unnecessary delay, but it is harmless to execute anyway.
        	{
                unsigned int pll_startup_counter = 800;	//Long delay at 31kHz, but ~0.8ms at 48MHz
                while(pll_startup_counter--);			//Clock will switch over while executing this delay loop
            }
            //Now restore the interrupt enable bits that we previously disabled in the USBCBSuspend() function,
            //when we first entered USB suspend state.
            UIE = UIESave;
        #endif
	#endif

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

	//Using SOF packets (which arrive at 1ms intervals) for time
	//keeping purposes in this demo.  This enables this demo to send constant
	//velocity contact point X/Y coordinate movement.
    SOFCount++;	
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
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
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

// *****************************************************************************
// ************** USB Class Specific Callback Function(s) **********************
// *****************************************************************************

/********************************************************************
 * Function:        void UserGetReportHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        UserGetReportHandler() is used to respond to
 *					the HID device class specific GET_REPORT control
 *					transfer request (starts with SETUP packet on EP0 OUT).  
 *                  This get report handler callback function is placed
 *					here instead of in usb_function_hid.c, since this code
 *					is "custom" and needs to be specific to this particular 
 *					application example.  For this HID digitizer device,
 *					we need to be able to correctly respond to 
 *					GET_REPORT(feature)	requests.  The proper response to this
 *					type of request depends on the HID report descriptor (in 
 *					usb_descriptors.c).
 * Note:            
 *******************************************************************/
void UserGetReportHandler(void)
{
    static BYTE FeatureReport[3];
    BYTE bytesToSend;
    
	//Check if request was for a feature report with report ID = VALID_CONTACTS_FEATURE_REPORT_ID = 0x02.
	//wValue MSB = 0x03 is Feature report (see HID1_11.pdf specifications), 
	//LSB = VALID_CONTACTS_FEATURE_REPORT_ID is Report ID "0x02".  This feature report ID
	//is used for reporting the maximum number of supported simultaneous contacts (in multi-touch mode).
	if(SetupPkt.wValue == (0x0300 + VALID_CONTACTS_FEATURE_REPORT_ID))	
	{
		static BYTE FeatureReport[2];
		
		//Prepare a response packet for the host
		FeatureReport[0] = VALID_CONTACTS_FEATURE_REPORT_ID;
		FeatureReport[1] = MAX_VALID_CONTACT_POINTS;	//Three contacts valid for this multi-touch demo (can be increased by increasing this number and editing report descriptor and report data payload)
		
		//Determine number of bytes to send to the host
		if(SetupPkt.wLength < 2u)
		{
			bytesToSend = SetupPkt.wLength;
		}
		else
		{
			//Size of the feature report.  Byte 0 is Report ID = 0x02, Byte 1 is
			//the maximum contacts value, which is "2" for this demo
			bytesToSend = 2;		
		}	

		//Now send the reponse packet data to the host, via the control transfer on EP0
		USBEP0SendRAMPtr((BYTE*)&FeatureReport, bytesToSend, USB_EP0_RAM);
	}
	else if(SetupPkt.wValue == (0x0300 + DEVICE_MODE_FEATURE_REPORT_ID))	//Report ID = 0x03 = Configuration report, which is used to select between mouse, single/multi digitizer modes.
	{
		//Prepare a response packet for the host
		FeatureReport[0] = DEVICE_MODE_FEATURE_REPORT_ID;
		FeatureReport[1] = DeviceMode;	
		FeatureReport[2] = DeviceIdentifier;
		
		//Determine number of bytes to send to the host
		if(SetupPkt.wLength < 3u)
		{
			bytesToSend = SetupPkt.wLength;
		}
		else
		{
			//Size of the feature report.  Byte 0 is report ID, byte 1 is the 
			//current DeviceMode, Byte 2 is the DeviceIdenfier
			bytesToSend = 3;		
		}

		//Now send the reponse packet data to the host, via the control transfer on EP0
		USBEP0SendRAMPtr((BYTE*)&FeatureReport, bytesToSend, USB_EP0_RAM);
	}		
}

/********************************************************************
 * Function:        void UserSetReportHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        UserSetReportHandler() is used to respond to
 *					the HID device class specific SET_REPORT control
 *					transfer request (starts with SETUP packet on EP0 OUT).  
 *                  This get report handler callback function is placed
 *					here instead of in usb_function_hid.c, since this code
 *					is "custom" and needs to be specific to this particular 
 *					application example.  For this HID digitizer device, which 
 *					supports multiple device modes, we need to be able to 
 *					correctly respond to 
 *					SET_FEATURE requests.  The proper response to this
 *					type of request depends on the HID report descriptor (in 
 *					usb_descriptors.c).
 * Note:            
 *******************************************************************/
void UserSetReportHandler(void)
{
	if(SetupPkt.wValue == (0x0300 + DEVICE_MODE_FEATURE_REPORT_ID))	//Host is setting the device mode (ex: mouse, single-touch digitizer, multi-touch digitizer)
	{
		//Temporarily stop sending HID report data packets on EP1 IN until
		//the new mode has been fully set/takes effect.
		HIDApplicationModeChanging = TRUE;

		//If firmware gets to here, the host PC is trying to change the device mode.
		//If the EP1 IN endpoint was already armed and filled with report data (from 
		//the previous mode setting, ex: a mouse report), then the first packet
		//sent to the host after the SET_REPORT control transfer completes might
		//contain the wrong data/be for the wrong mode.  To ensure that all packets
		//are consistent to the currently configured mode, even during switchover
		//events, special handling should be done to dis-arm EP1 IN until the new
		//mode setting is complete.
		
		//Check if the EP1 IN endpoint was already armed.  If so, need to abandon the
		//operation to avoid sending any packets, until the new mode has been set.
		if(USBHandleBusy(lastTransmission))	//effectively: if(EP1 IN UOWN bit == 1)
		{
            //In this case we need to terminate the pending USB transfers on the
            //  HID endpoint.
            USBCancelIO(HID_EP);
		}	
		
		//Prepare EP0 to receive the control transfer data (the device mode to set)
		USBEP0Receive((BYTE*)&hid_report_out, SetupPkt.wLength, USBHIDCBSetReportComplete);	//Host will send two bytes.  After the two bytes are successfully received, call the USBHIDCBSetReportComplete() callback function. 		
	}	
}	


//Secondary callback function that gets called when the above
//control transfer,  USBEP0Receive(...,...,USBHIDCBSetReportComplete) completes.
void USBHIDCBSetReportComplete(void)
{
	//The hid_report_out[0] byte contains the Report ID that is getting set.
	//The hid_report_out[1] byte contains the Device Mode
	//The hid_report_out[2] byte contains the DeviceIdentifier

	if(hid_report_out[0] == DEVICE_MODE_FEATURE_REPORT_ID)	//configuration setting feature Report ID = 0x03 = DEVICE_MODE_FEATURE_REPORT_ID
	{	
		//Set the device variables now, so the firmware can behave according to the new
		//settings that have been specified by the host.
	
		//Error checking.  Make sure the host is trying to choose a valid setting.
		if((hid_report_out[1] == MOUSE_MODE) || (hid_report_out[1] == SINGLE_TOUCH_DIGITIZER_MODE) || (hid_report_out[1] == MULTI_TOUCH_DIGITIZER_MODE))	
		{
			DeviceMode = hid_report_out[1];		//The DeviceMode variable is used else where in the application firmware to determine the behavior/type of input reports sent to the host.
			DeviceIdentifier = hid_report_out[2];
		}	
	}
	
	//The new device mode setting has been set.  Okay to start sending HID report
	//packets again on EP1 IN now.
	HIDApplicationModeChanging = FALSE;	
}	


/** EOF MultiTouch_MultiModes.c *************************************************/
#endif
