/********************************************************************
 FileName:      HardwareProfile.h
 Dependencies:  See INCLUDES section
 Processor:     PIC24FJ256DA210 Microcontroller
 Hardware:      The code is natively intended to be used on the 
                  following hardware platforms: 
                    Universal Remote Control Demo Board
                  The firmware may be modified for use on other hardware 
                    platforms by editing this file (HardwareProfile.h)
 Compiler:  	Microchip C30 (for PIC24)
 Company:       Microchip Technology, Inc.

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
  1.0   01/10/2012   ...
********************************************************************/

#ifndef _HARDWARE_PROFILE_H
#define _HARDWARE_PROFILE_H

/*********************************************************************
* PIC Device Specific includes
*********************************************************************/
#include "Compiler.h"

/*****************************************************************************
 * Section:  Includes
 *****************************************************************************/
#include "GenericTypeDefs.h"
#include "ConfigApp.h"

#define CLOCK_FREQ (32000000ul)
/*********************************************************************
* Macro: #define	GetSystemClock() 
*
* Overview: This macro returns the system clock frequency in Hertz.
*			* value is 8 MHz x 4 PLL for PIC24
*			* value is 8 MHz/2 x 18 PLL for PIC32
*
********************************************************************/
#define GetSystemClock()    (CLOCK_FREQ)

/*********************************************************************
* Macro: #define	GetPeripheralClock() 
*
* Overview: This macro returns the peripheral clock frequency 
*			used in Hertz.
*			* value for PIC24 is <PRE>(GetSystemClock()/2) </PRE> 
*			* value for PIC32 is <PRE>(GetSystemClock()/(1<<OSCCONbits.PBDIV)) </PRE>
*
********************************************************************/
#define GetPeripheralClock()    (GetSystemClock() / 2)

/*********************************************************************
* Macro: #define	GetInstructionClock() 
*
* Overview: This macro returns instruction clock frequency 
*			used in Hertz.
*			* value for PIC24 is <PRE>(GetSystemClock()/2) </PRE> 
*			* value for PIC32 is <PRE>(GetSystemClock()) </PRE> 
*
********************************************************************/
#define GetInstructionClock()   (GetSystemClock() / 2)

/*****************************************************************************
 * Section:  Display Driver Configuration
 *****************************************************************************/

#define HI_REMOTE_CONTROL_BOARD 

#define PIC24FJ256DA210_DEV_BOARD
#define GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210
#define GFX_USE_DISPLAY_PANEL_TFT_G320240_91_E
#define GFX_GCLK_DIVIDER 38 
#define GFX_DISPLAY_BUFFER_START_ADDRESS 0x00004B00ul
#define GFX_DISPLAY_BUFFER_LENGTH 0x0004B000ul

// Error Checks
#if !defined(HI_REMOTE_CONTROL_BOARD)
	#error "Error: Graphics Hardware Platform is not correctly defined! One of the three (HI_REMOTE_CONTROL_BOARD) must be defined."
#endif

#if !defined (__PIC24FJ256DA210__)
	#error "This demo is only used for the PIC24FJ256DA210 Device."
#endif

// Display Settings
// Using TFT_G320240_91_E Display Panel
	#if defined (GFX_USE_DISPLAY_PANEL_TFT_G320240_91_E) 
		#define DISP_ORIENTATION		90
        #define DISP_HOR_RESOLUTION		320
        #define DISP_VER_RESOLUTION		240
        #define DISP_DATA_WIDTH			18
        #define DISP_HOR_PULSE_WIDTH    40
        #define DISP_HOR_BACK_PORCH     28
        #define DISP_HOR_FRONT_PORCH    20
        #define DISP_VER_PULSE_WIDTH    2  
        #define DISP_VER_BACK_PORCH     16 
        #define DISP_VER_FRONT_PORCH    4 
    	#define GFX_LCD_TYPE            GFX_LCD_TFT
        
		#if (DISPLAY_CONTROLLER == MCHP_DA210)
            #define GFX_DISPLAYENABLE_ENABLE
            #define GFX_HSYNC_ENABLE
            #define GFX_VSYNC_ENABLE
            #define GFX_CLOCK_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_DISPLAYENABLE_POLARITY          GFX_ACTIVE_HIGH 
            #define GFX_HSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_VSYNC_POLARITY                  GFX_ACTIVE_LOW
        #endif
        
	#endif // #if defined (GFX_USE_DISPLAY_PANEL_TFT_G320240_91_E) 

// -----------------------------------
// Other driver dependencies
// -----------------------------------
#if defined (GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)
    #define USE_TCON_SSD1289
    #define USE_TCON_MODULE
    
#elif defined (GFX_USE_DISPLAY_PANEL_TFT_G320240_91_E)
    #define USE_TCON_HX8238
    #define USE_TCON_MODULE

#else
    #define GFX_USE_TCON_CUSTOM
#endif	

//#define USE_ALTERNATE_SPI_DRIVER

/*********************************************************************
* USB Device used to program Flash Memory
*********************************************************************/
    #define COMM_PKT_RX_MAX_SIZE    (1024)
    #define USE_COMM_PKT_MEDIA_USB
    
    //#define USE_SELF_POWER_SENSE_IO
    #define tris_self_power     TRISAbits.TRISA2    // Input
    #define self_power          1
    
    //#define USE_USB_BUS_SENSE_IO
    //#define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
    #define USB_BUS_SENSE       U1OTGSTATbits.SESVD // Special considerations required if using SESVD for this purpose.  See documentation.

/*********************************************************************
* HARDWARE PROFILE FOR THE TIMING CONTROLLER
*********************************************************************/
/*
	Timing Controllers (TCON) are used by some displays.
	The display selected will determine the TCON that will be used
	in the build. 
	When using SSD1926, the TCON is connected through the SSD1926 
	GPIO and SPI transfers is BitBanged on these GPIO ports.
	Refer to SSD1926 implementation for details.
*/
	/*********************************************************************
	* I/Os for the TCON 
	*********************************************************************/

	/* ----------------------------------------- */
    #if defined (GFX_USE_DISPLAY_PANEL_TFT_G320240_91_E)
	/* ----------------------------------------- */
		
		#define TCON_CS_LAT      LATAbits.LATA14   //_RA14
		#define TCON_CS_TRIS     TRISAbits.TRISA14 //_TRISA14
		#define TCON_CS_DIG()
	    
		#define TCON_SCL_LAT     LATDbits.LATD10   //_RD10
		#define TCON_SCL_TRIS    TRISDbits.TRISD10 //_TRISD10
		#define TCON_SCL_DIG()
	    
		#define TCON_SDO_LAT     LATDbits.LATD11   //_RD11
		#define TCON_SDO_TRIS    TRISDbits.TRISD11 //_TRISD11
	    #define TCON_SDO_DIG()

		#define TCON_DC_LAT      LATFbits.LATF3    //_RF3
		#define TCON_DC_TRIS     TRISFbits.TRISF3  //_TRISF3
		#define TCON_DC_DIG()    
		
	#endif // #if defined (GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)

/*********************************************************************
* Section: Display Panel 
*********************************************************************/
    // Definitions for POWER ON pin
    #define DisplayPowerConfig()	    TRISGbits.TRISG14 = 0         
    #define DisplayPowerOn() 		    LATGbits.LATG14 = 1           
    #define DisplayPowerOff() 		    LATGbits.LATG14 = 0          
    
    // Definitions for backlight control pin
    #define DisplayBacklightConfig()    TRISDbits.TRISD8 = 0
    #define DisplayBacklightOn()        LATDbits.LATD8 = 1
    #define DisplayBacklightOff()       LATDbits.LATD8 = 0

/*********************************************************************
* Section: Resistive Touch Screen A/D and I/O Specific Macros 
*********************************************************************/
    #define USE_TOUCHSCREEN_RESISTIVE
    #define RESISTIVETOUCH_MANUAL_SAMPLE_MODE
    
    // ADC Input Channel Selector
    #define TOUCH_ADC_INPUT_SEL   AD1CHS
    
    // ADC Sample Start 
    #define TOUCH_ADC_START   AD1CON1bits.SAMP 
    
    // ADC Status
    #define TOUCH_ADC_DONE   AD1CON1bits.DONE
    
    // ADC channel constants
    #define ADC_XPOS    12
    #define ADC_YPOS	13
    
    // ADC Port Control Bits
    #define ADPCFG_XPOS	ANSBbits.ANSB12
    #define ADPCFG_YPOS	ANSBbits.ANSB13
   
   
    // X port definitions
    #define ResistiveTouchScreen_XPlus_Drive_High()         LATBbits.LATB12   = 1
    #define ResistiveTouchScreen_XPlus_Drive_Low()          LATBbits.LATB12   = 0
    #define ResistiveTouchScreen_XPlus_Config_As_Input()    TRISBbits.TRISB12 = 1
    #define ResistiveTouchScreen_XPlus_Config_As_Output()   TRISBbits.TRISB12 = 0
		
    #define ResistiveTouchScreen_XMinus_Drive_High()        LATBbits.LATB14   = 1   
    #define ResistiveTouchScreen_XMinus_Drive_Low()         LATBbits.LATB14   = 0
    #define ResistiveTouchScreen_XMinus_Config_As_Input()   TRISBbits.TRISB14 = 1  
    #define ResistiveTouchScreen_XMinus_Config_As_Output()  TRISBbits.TRISB14 = 0

	// Y port definitions
    #define ResistiveTouchScreen_YPlus_Drive_High()         LATBbits.LATB13   = 1
    #define ResistiveTouchScreen_YPlus_Drive_Low()          LATBbits.LATB13   = 0
    #define ResistiveTouchScreen_YPlus_Config_As_Input()    TRISBbits.TRISB13 = 1
    #define ResistiveTouchScreen_YPlus_Config_As_Output()   TRISBbits.TRISB13 = 0
		
    #define ResistiveTouchScreen_YMinus_Drive_High()        LATBbits.LATB15   = 1   
    #define ResistiveTouchScreen_YMinus_Drive_Low()         LATBbits.LATB15   = 0
    #define ResistiveTouchScreen_YMinus_Config_As_Input()   TRISBbits.TRISB15 = 1  
    #define ResistiveTouchScreen_YMinus_Config_As_Output()  TRISBbits.TRISB15 = 0
	    
    /*********************************************************************
    * Touch Screen X and Y orientation
    *********************************************************************/
    #define TOUCHSCREEN_RESISTIVE_FLIP_Y
    
    /*********************************************************************
    * Touch Screen Non-Volatile Memory Storage Macros
    *********************************************************************/
    // Addresses for calibration and version values in SPI Flash on Graphics PICtail 3 & PIC24FJ256DA210 Development Board.
    // Or Addresses for calibration and version values in Parallel Flash on PIC24FJ256DA210 Development Board.
    #define ADDRESS_RESISTIVE_TOUCH_VERSION	(unsigned long)0xFFFFFFFE
    #define ADDRESS_RESISTIVE_TOUCH_ULX   (unsigned long)0xFFFFFFFC
    #define ADDRESS_RESISTIVE_TOUCH_ULY   (unsigned long)0xFFFFFFFA
    #define ADDRESS_RESISTIVE_TOUCH_URX   (unsigned long)0xFFFFFFF8
    #define ADDRESS_RESISTIVE_TOUCH_URY   (unsigned long)0xFFFFFFF6

    #define ADDRESS_RESISTIVE_TOUCH_LLX   (unsigned long)0xFFFFFFF4
    #define ADDRESS_RESISTIVE_TOUCH_LLY   (unsigned long)0xFFFFFFF2
    #define ADDRESS_RESISTIVE_TOUCH_LRX   (unsigned long)0xFFFFFFF0
    #define ADDRESS_RESISTIVE_TOUCH_LRY   (unsigned long)0xFFFFFFEE
    
    // define the functions to call for the non-volatile memory
    // check out touch screen module for definitions of the following function pointers
    // used: NVM_READ_FUNC, NVM_WRITE_FUNC & NVM_SECTORERASE_FUNC
    #define NVMSectorErase					((NVM_SECTORERASE_FUNC)&SST25SectorErase)
    #define NVMWrite 						((NVM_WRITE_FUNC)&SST25WriteWord)
    #define NVMRead 						((NVM_READ_FUNC)&SST25ReadWord)

/*****************************************************************************
 * Section:  PPS Configuration
 *****************************************************************************/
    #define UnlockPPS()     __builtin_write_OSCCONL(OSCCON & 0xBF)
    #define LockPPS()       __builtin_write_OSCCONL(OSCCON | 0x40)
    
    #define SPI1ConfigurePPS()  {\
                                    RPOR11bits.RP22R = 8;\
    	                            RPOR12bits.RP25R = 7;\
    	                            RPINR20bits.SDI1R = 42;\
                                }
    
    #define INT1ConfigurePPS()  RPINR0bits.INT1R = 35
    
    
    #define SPI2ConfigurePPS()  {\
                                    RPOR1bits.RP3R = 11;\
                                    RPOR6bits.RP12R = 10;\
                                    RPINR22bits.SDI2R = 16;\
                                }
    
    // Assign RP0 to TX (output),  Assign RP1 to RX (input)
    #define UART2ConfigurePPS() {\
    								RPOR0bits.RP0R = 5;\
    								RPINR19bits.U2RXR = 1;\
                                }

    // OC1 PWM (output to RP2)
    #define OC1ConfigurePPS()   {\
    								RPOR1bits.RP2R = 18;\
                                }

							 	                                
								
/*****************************************************************************
 * Section:  Accelerometer
 *****************************************************************************/
                                            
/*****************************************************************************
 * Section:  LEDs
 *****************************************************************************/
#define SetLeftGreenLEDDirection()    {TRISAbits.TRISA10 = 0;}
#define SetRightGreenLEDDirection()   {TRISEbits.TRISE4  = 0;}
#define SetRedLEDDirection()          {TRISAbits.TRISA9  = 0;}

#define CrearLeftGreenLEDDirection()   {TRISAbits.TRISA10 = 1;}
#define CrearRightGreenLEDDirection()  {TRISEbits.TRISE4  = 1;}
#define CrearRedLEDDirection()         {TRISAbits.TRISA9  = 1;}

#define SetLeftGreenOn()              {LATAbits.LATA10 = 1;}
#define SetRightGreenOn()             {LATEbits.LATE4  = 1;}
#define SetRedOn()                    {LATAbits.LATA9  = 1;}

#define SetLeftGreenOff()             {LATAbits.LATA10 = 0;}
#define SetRightGreenOff()            {LATEbits.LATE4  = 0;}
#define SetRedOff()                   {LATAbits.LATA9  = 0;}

#define SetLeftGreenToggle()          {LATAbits.LATA10 ^= 1;}
#define SetRightGreenToggle()         {LATEbits.LATE4  ^= 1;}
#define SetRedToggle()                {LATAbits.LATA9  ^= 1;}

/*****************************************************************************
 * Section:  Joystick Switch
 *****************************************************************************/

#define SetJKButtonDirection()     {TRISEbits.TRISE5 = 0; LATEbits.LATE5 = 0;}
 

#define SetJKLeftDirection()        {TRISGbits.TRISG1 = 1;  CNPU5bits.CN78PUE = 1;}
#define SetJKRightDirection()       {TRISGbits.TRISG0 = 1;  CNPU5bits.CN77PUE = 1;}
#define SetJKUpDirection()          {TRISFbits.TRISF4 = 1;  CNPU2bits.CN17PUE = 1;}
#define SetJKDownDirection()        {TRISFbits.TRISF5 = 1;  CNPU2bits.CN18PUE = 1;}
#define SetJKCenterDirection()      {TRISEbits.TRISE8 = 1;  CNPU5bits.CN66PUE = 1;}

#define GetJKLeftPin()              PORTGbits.RG1
#define GetJKRightPin()             PORTGbits.RG0
#define GetJKUpPin()                PORTFbits.RF4
#define GetJKDownPin()              PORTFbits.RF5
#define GetJKCenterPin()            PORTEbits.RE8

#define IsJKButtonPressed()        ((GetJKLeftPin() == 0)||(GetJKRightPin() == 0)||(GetJKUpPin() == 0)||(GetJKDownPin() == 0)||(GetJKCenterPin() == 0))

#define JKLeftInterrruptConfig()    {CNEN5bits.CN78IE = 1;}
#define JKRightInterruptConfig()    {CNEN5bits.CN77IE = 1;}
#define JKUpInterruptConfig()       {CNEN2bits.CN17IE = 1;}
#define JKDownInterruptConfig()     {CNEN2bits.CN18IE = 1;}
#define JKCenterInterruptConfig()   {CNEN5bits.CN66IE = 1;}

#define JKIntEnable()               IEC1bits.CNIE = 1;
#define JKIntDisable()              IEC1bits.CNIE = 0;

/*********************************************************************
* Section: Buttons (Wake Button) 
*********************************************************************/
typedef enum
{
    HW_BUTTON_PRESS = 0,
    HW_BUTTON_RELEASE = 1
}HW_BUTTON_STATE;

#define SetWakeButtonDirection()    (TRISDbits.TRISD0)
#define IsWakeButtonPressed()       (PORTDbits.RD0 == 1)
#define WakeButtonInterruptEdge()   (INTCON2bits.INT0EP)

// interrupt on release of wake button   
#define WakeButtonInit()        {\
                                    WakeButtonInterruptEdge() = 1; \
                                    SetWakeButtonDirection(); \
                                }
#define HardwareButtonInit()    


/*****************************************************************************
 * Section:  Display Haptic I/O
 *****************************************************************************/
#define DisplayHapticShutdownDirection()    TRISFbits.TRISF1 = 0   
#define DisplayHapticShutdownON()           LATFbits.LATF1 = 1
#define DisplayHapticShutDownOFF()          LATFbits.LATF1 = 0

#define DisplayHapticInputDirection()       TRISDbits.TRISD5 = 0
#define DisplayHapticInputON()              LATDbits.LATD5 = 1
#define DisplayHapticInputOFF()             LATDbits.LATD5 = 0

/*****************************************************************************
 * Section:  UART
 *****************************************************************************/
#define TX_TRIS TRISBbits.TRISB0
#define RX_TRIS TRISBbits.TRISB1

/*****************************************************************************
 * Section:  EEPROM
 *****************************************************************************/
#define USE_MCHP25AA02E48

#define MCHP25AA02E48_SPI_CHANNEL 2

#define MCHP25AA02E48_CS_TRIS   TRISEbits.TRISE1
#define MCHP25AA02E48_CS_LAT    LATEbits.LATE1
    
#define MCHP25AA02E48_SCK_TRIS  TRISDbits.TRISD10
#define MCHP25AA02E48_SDO_TRIS  TRISDbits.TRISD11
#define MCHP25AA02E48_SDI_TRIS  TRISFbits.TRISF3
    
// macros to re-direct DRVSPIxxx functions in MCHP25AA02E48 driver 
// to use the ones in spi_drv.c and spi_drv.h
#define DRVSPILock(channel)         SPILock(channel)
#define DRVSPIUnLock(channel)       SPIUnLock(channel)
#define DRVSPIPut(channel, data)    SPIPut(channel, data)
#define DRVSPIGet(channel)          SPIGet(channel)

#define SPIEEPROMConfigurePins() {                                  \
                                        MCHP25AA02E48_CS_LAT = 1;   \
                                        MCHP25AA02E48_CS_TRIS = 0;  \
                                        MCHP25AA02E48_SCK_TRIS = 0; \
                                        MCHP25AA02E48_SDO_TRIS = 0; \
                                        MCHP25AA02E48_SDI_TRIS = 1; \
                                }

#define RF4CE_FRAME_COUNTER_EEPROM_NVM_ADDRESS      (0)
#define RF4CE_DEFAULT_CHANNEL_EEPROM_NVM_ADDRESS    (4)
#define RF4CE_POWER_SAVE_MODE_EEPROM_NVM_ADDRESS    (5)
/*****************************************************************************
 * Section:  SPI Flash
 *****************************************************************************/
    
    #define USE_SST25VF016

    #define SST25_SPI_CHANNEL 2

    /* Define all the SPI channels that will be used here.
       These will be used to determine how the SPI Driver (drv_spi)
       will be compiled.
    */
    #if (SST25_SPI_CHANNEL == 1)
        #define SPI_CHANNEL_1_ENABLE
    #elif (SST25_SPI_CHANNEL == 2)
        #define SPI_CHANNEL_2_ENABLE
    #elif (SST25_SPI_CHANNEL == 3)
        #define SPI_CHANNEL_3_ENABLE
    #elif (SST25_SPI_CHANNEL == 4)
        #define SPI_CHANNEL_4_ENABLE
    #endif

    #define SST25_CS_TRIS   TRISFbits.TRISF0
    #define SST25_CS_LAT    LATFbits.LATF0
    
    #define SST25_SCK_TRIS  TRISDbits.TRISD10
    #define SST25_SDO_TRIS  TRISDbits.TRISD11
    #define SST25_SDI_TRIS  TRISFbits.TRISF3
    

    #define SPIFlashConfigurePins() {                           \
                                            SST25_CS_LAT = 1;   \
                                            SST25_CS_TRIS = 0;  \
                                            SST25_SCK_TRIS = 0; \
                                            SST25_SDO_TRIS = 0; \
                                            SST25_SDI_TRIS = 1; \
                                    }


#define RF4CE_NVM_START_ADDR                    0x1FE000ul
#define RF4CE_NOR_NVM_START_ADDR                0x1FE000ul
#define RF4CE_SHORT_ADDRESS_NOR_NVM_ADDRESS     (RF4CE_NOR_NVM_START_ADDR)
#define RF4CE_PAN_ID_NOR_NVM_ADDRESS            (RF4CE_SHORT_ADDRESS_NOR_NVM_ADDRESS + 2l)
#define RF4CE_PAIR_TABLE_NOR_NVM_ADDRESS        (RF4CE_PAN_ID_NOR_NVM_ADDRESS + 2l)

// NVM memory is also used to store the current active (selected) device.
// user can change this active device anytime and must be current with 
// NVM so when power is suddenly removed, the selected device is retained.
#define RF4CE_NVM_ACTIVE_DEVICE_ADDRESS         0x1FD000ul


/*****************************************************************************
 * Section:  RTCC default time and date
 *****************************************************************************/
#define RTCC_DEFAULT_DAY        11      // 11th
#define RTCC_DEFAULT_MONTH      11      // November
#define RTCC_DEFAULT_YEAR       11      // 2011
#define RTCC_DEFAULT_WEEKDAY    01      // Monday
#define RTCC_DEFAULT_HOUR       10      // 10:10:01
#define RTCC_DEFAULT_MINUTE     10
#define RTCC_DEFAULT_SECOND     01

/*****************************************************************************
 * Section:  Transceiver Configuration
 *****************************************************************************/


#define RFConfigureHardwareIO() {\
                                    PHY_CS          = 1;\
                                    PHY_CS_TRIS     = 0;\
                                    PHY_RESETn      = 1;\
                                    PHY_RESETn_TRIS = 0;\
                                    PHY_WAKE        = 1;\
                                    PHY_WAKE_TRIS   = 0;\
                                }

#define RFConfigureSPI()        {\
                                    RF_SPICON1 = RF_SPICON1_VALUE;\
                                    RF_SPICON2 = RF_SPICON2_VALUE;\
                                    RF_SPISTAT = RF_SPISTAT_VALUE;\
                                }


#define RFConfigureInterrupt()  {\
                                    RF_INT_PIN = 1;\
                                    RF_INT_TRIS = 1;\
                                    RFIE_EDGE = 1;\
                                    RFIF = 0;\
                                    RFIE = 1;\
                                }

#define RFIF        IFS1bits.INT1IF
#define RFIE        IEC1bits.INT1IE
#define RFIE_EDGE   INTCON2bits.INT1EP
#define RF_INT_PIN  PORTAbits.RA15
#define RF_INT_TRIS TRISAbits.TRISA15

#define PHY_CS          LATDbits.LATD13
#define PHY_CS_TRIS     TRISDbits.TRISD13
#define PHY_RESETn      LATDbits.LATD7
#define PHY_RESETn_TRIS TRISDbits.TRISD7
#define PHY_WAKE        LATDbits.LATD6
#define PHY_WAKE_TRIS   TRISDbits.TRISD6

#define RF_SPICON1_VALUE    (0x033B)
#define RF_SPICON2_VALUE    (0)
#define RF_SPISTAT_VALUE    (0x8000)

#define RF_SCLK_RPO         (8)
#define RF_SDO_RPO          (7)
#define RF_SDI_RPI          (42)
#define RF_EX_INT_RPI       (35)

#define RF_SPICON1      SPI1CON1
#define RF_SPICON2      SPI1CON2
#define RF_SPISTAT      SPI1STAT

#define TMRL TMR1

#endif
