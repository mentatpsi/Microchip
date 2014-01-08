/*********************************************************************
 *
 *	Hardware specific definitions
 *
 *********************************************************************
 * FileName:        HardwareProfile_xxx.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        Microchip C32 v1.00 or higher
 *					Microchip C30 v3.01 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright � 2002-2011 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product (�Device�) which is 
 *      integrated into Licensee�s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Date		    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 02/24/11	    For Graphics Library Version 3.00 
 ********************************************************************/

#ifndef __HARDWARE_PROFILE_H
    #define __HARDWARE_PROFILE_H

/*********************************************************************
* PIC Device Specific includes
*********************************************************************/
    #include "Compiler.h"

/*********************************************************************
* GetSystemClock() returns system clock frequency.
*
* GetPeripheralClock() returns peripheral clock frequency.
*
* GetInstructionClock() returns instruction clock frequency.
*
********************************************************************/

/*********************************************************************
* Macro: #define	GetSystemClock() 
*
* Overview: This macro returns the system clock frequency in Hertz.
*			* value is 8 MHz x 4 PLL for PIC24
*			* value is 8 MHz/2 x 18 PLL for PIC32
*
********************************************************************/
    #if defined(__PIC24F__)
        #define GetSystemClock()    (32000000ul)
    #elif defined(__PIC32MX__)
        #define GetSystemClock()    (80000000ul)
    #elif defined(__dsPIC33F__) || defined(__PIC24H__)
        #define GetSystemClock()    (80000000ul)
    #elif defined(__dsPIC33E__) || defined(__PIC24E__)
    	#define GetSystemClock()    (120000000ul)
    #endif

/*********************************************************************
* Macro: #define	GetPeripheralClock() 
*
* Overview: This macro returns the peripheral clock frequency 
*			used in Hertz.
*			* value for PIC24 is <PRE>(GetSystemClock()/2) </PRE> 
*			* value for PIC32 is <PRE>(GetSystemClock()/(1<<OSCCONbits.PBDIV)) </PRE>
*
********************************************************************/
    #if defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__) || defined(__dsPIC33E__) || defined(__PIC24E__)
        #define GetPeripheralClock()    (GetSystemClock() / 2)
    #elif defined(__PIC32MX__)
        #define GetPeripheralClock()    (GetSystemClock() / (1 << OSCCONbits.PBDIV))
    #endif

/*********************************************************************
* Macro: #define	GetInstructionClock() 
*
* Overview: This macro returns instruction clock frequency 
*			used in Hertz.
*			* value for PIC24 is <PRE>(GetSystemClock()/2) </PRE> 
*			* value for PIC32 is <PRE>(GetSystemClock()) </PRE> 
*
********************************************************************/
    #if defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__) || defined(__dsPIC33E__) || defined(__PIC24E__)
        #define GetInstructionClock()   (GetSystemClock() / 2)
    #elif defined(__PIC32MX__)
        #define GetInstructionClock()   (GetSystemClock())
    #endif


/* ################################################################## */
/*********************************************************************
* START OF GRAPHICS RELATED MACROS
********************************************************************/
/* ################################################################## */

/*********************************************************************
* AUTO GENERATED CODE 
********************************************************************/

#define WFM_ST_ADR			0x108A 			// Waveform start address for new GenEx

// Waveforms modes
#define WFM_INIT			0x0000			// INIT waveform '
#define WFM_DU				0x0100			// Monochrome waveform (3/4-bit)
#define WFM_GU8				0x0200			// (similar to partial update, no flashing)
#define WFM_GC8				0x0300			// (8-level gray shades with flashing)




/*********************************************************************
* SELECT INTERFACE TYPE 
********************************************************************/		       
#define USE_8BIT_PMP
//#define USE_BUSY_LINE

/*********************************************************************
* SELECT HOST BOARD 
********************************************************************/
#define EXPLORER_16

#define GFX_USE_DISPLAY_CONTROLLER_S1D13522
#define GFX_PICTAIL_EPD_V1
//End Auto Generated Code


// Required by Graphics Library
#define DISP_VER_RESOLUTION		320
#define DISP_HOR_RESOLUTION		240
#define DISP_ORIENTATION		90

#define DISP_SCREEN_SIZE                ((DWORD)DISP_HOR_RESOLUTION * (DWORD)DISP_VER_RESOLUTION)
#define IMG_BUF_ST_ADR                  (DWORD)(DISP_SCREEN_SIZE + (DISP_SCREEN_SIZE >> 1))
#define PIP_BUF_ST_ADR                  ((DWORD)DISP_SCREEN_SIZE * 2)

/*********************************************************************
* END OF AUTO GENERATED CODE 
********************************************************************/

/*********************************************************************
* External Memory Programmer Settings
*********************************************************************/
/*
    USE_COMM_PKT_MEDIA_SERIAL_PORT - to use UART port as the flash programming port
    USE_COMM_PKT_MEDIA_USB - to use USB port as the flash programming port
    
    When non of the two are defined prior to this check, the default
    programming port is used. 
    EXPLORER_16 - uses UART as default port
    PIC24FJ256DA210_DEV_BOARD - use USB as the default port
    PIC_SK, MEB_BOARD - use USB as the default port
    
*/
// error checks
#if defined (USE_COMM_PKT_MEDIA_SERIAL_PORT) && defined (USE_COMM_PKT_MEDIA_USB)
    #error "Cannot have two programming ports"
#endif    

#if !defined (USE_COMM_PKT_MEDIA_SERIAL_PORT)
    // assign the default external flash programming port
    #if defined (EXPLORER_16)  
        #define USE_COMM_PKT_MEDIA_SERIAL_PORT
        #define BAUDRATE2               115200UL
        #define BRG_DIV2                4
        #define BRGH2                   1
    #endif
#else
        #define BAUDRATE2               115200UL
        #define BRG_DIV2                4
        #define BRGH2                   1
#endif

#if !defined (USE_COMM_PKT_MEDIA_USB)
	// assign the default external flash programming port
    #if defined (PIC24FJ256DA210_DEV_BOARD)      || \
        (defined (MEB_BOARD) && defined (_USB))  || \
        (defined (PIC32_SK) && defined (_USB))   || \
        (defined (PIC_SK) && defined (__dsPIC33EP512MU810__)) || \
        (defined (PIC_SK) && defined (__PIC24EP512GU810__))
        
        #define USE_COMM_PKT_MEDIA_USB
        //#define USE_SELF_POWER_SENSE_IO   
        #define tris_self_power     TRISAbits.TRISA2    // Input
        #define self_power          1

        //#define USE_USB_BUS_SENSE_IO
        #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
        #define USB_BUS_SENSE       U1OTGSTATbits.SESVD // Special considerations required if using SESVD for this purpose.  See documentation.
    #endif
#else
        //#define USE_SELF_POWER_SENSE_IO   
        #define tris_self_power     TRISAbits.TRISA2    // Input
        #define self_power          1

        //#define USE_USB_BUS_SENSE_IO
        #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
        #define USB_BUS_SENSE       U1OTGSTATbits.SESVD // Special considerations required if using SESVD for this purpose.  See documentation.
#endif

// set the default packet size
#if defined (USE_COMM_PKT_MEDIA_USB) || defined (USE_COMM_PKT_MEDIA_SERIAL_PORT)
    #if !defined (COMM_PKT_RX_MAX_SIZE)
        #define COMM_PKT_RX_MAX_SIZE    (1024)
    #endif 
#endif    

/*********************************************************************
* ERROR CHECKING
*********************************************************************/
/*
   This error check are intended for Microchip Development
   Boards and Modules. When using custom hardware, remove this part
   of the hardware profile.
*/

/*
   This check verifies that the selected PMP interface supports
   16-bit interface.
*/
#if defined (__PIC24FJ128GA010__) || defined (__PIC24FJ256GA110__) ||	\
	defined (__PIC24FJ256GB110__) || defined (__dsPIC33F__)        ||   \
	defined (__PIC24HJ128GP504__)
	#if defined (USE_16BIT_PMP)	
		#error "The device selected does not support 16 bit PMP interface."
	#endif
#endif

/*********************************************************************
* HARDWARE DEVICES SELECTION
*********************************************************************/
/*
    This portion selects the different components available to 
    the application depending on the hardware platform selected.
    * EXPLORER_16 - Explorer 16 Development Board (DM240001)
    * PIC_SK      - PIC32 Starter Kit or USB Starter Kit or Ethernet Starter Kit, and Starter Kit I/O Expansion Board 
*/


#if defined(GFX_PICTAIL_EPD_V1)
       #define      USE_SST25VF064                    // use the 64 Mbit SPI Serial Flash 
#endif


/*********************************************************************
* STARTER KIT SPECIFIC MACROS
*********************************************************************/
#ifdef PIC_SK
	#if defined (__PIC32MX__)
		   #define PIC32_SK 
	#elif defined (__dsPIC33E__) || (__PIC24E__)
		   #define dsPIC33E_SK
	#endif
#endif

	   
/*********************************************************************
* DISPLAY INTERFACE 
*********************************************************************/
/*
   Select the correct Parallel Master Port Interface Driver.
   This selection is valid only for parallel interface. When using
   display drivers that uses serial interface, this portion should be
   commented out or removed.
*/

#if !defined( USE_SPI_CHANNEL )
	// EPMP is exclusive to GB210 devices and PMP to some devices
	#if defined (__PIC24FJ256DA210__)
		// EPMP is used by graphics controller but if EPMP is used 
		// to access external memory then we will need EPMP driver.
		#define USE_GFX_EPMP
	#elif defined (__PIC24FJ256GB210__)
		#define USE_GFX_EPMP
	#else
		#define USE_GFX_PMP
	#endif
#endif

/*********************************************************************
* PARALLEL MASTER PORT INTERFACE TIMING 
*********************************************************************/
	// When using the Parallel Master Port (or Enhanced Parallel Master Port) the 
	// PMP timing must be defined for the following:
	// PMP_DATA_SETUP_TIME or EPMPCSx_DATA_SETUP (for DWAITB) 
	// PMP_DATA_WAIT_TIME or EPMPCSx_DATA_WAIT_TIME (for DWAITM) 
	// and PMP_DATA_HOLD_TIME or EPMPCSx_DATA_HOLD_TIME (for DWAITE)
    // where x is 1 or 2 (EPMP has 2 chip selects)
	// All values are timing parameters in ns.
	// The timing is dependent on the display controller sampling period if 
	// interfacing with the display controller or device AC timing requirements
	// if interfacing with a device (such as SRAM or Parallel Flash memory).
	// Refer to your specific display controller or device to determine the 
	// optimum (fastest) timing requirement for your interface. 

#if defined (USE_GFX_PMP)  

        #define PMP_DATA_SETUP_TIME                 (8)    
        #define PMP_DATA_WAIT_TIME                  (75)  // See Epson S1D13522 data sheet for details
        #define PMP_DATA_HOLD_TIME                  (5)     

#endif //#if defined (USE_GFX_PMP) 

#if defined (USE_GFX_EPMP)     

        #define EPMPCS1_DATA_SETUP_TIME             (8) 
       	#define EPMPCS1_DATA_WAIT_TIME          	(59)  // See Epson S1D13522 data sheet for details
        #define EPMPCS1_DATA_HOLD_TIME              (5)   
                       
#endif //#if defined (USE_GFX_PMP) || defined (USE_GFX_EPMP)

/*********************************************************************
* SPI (SERIAL) INTERFACE SETUP & TIMING 
*********************************************************************/
#if defined ( USE_SPI_CHANNEL )
	
	#define SPI_CKE 		SPI_CKE_ACT_IDLE  // SPI_CKE_IDLE_ACT, SPI_CKE_ACT_IDLE
	#define SPI_CKP			SPI_CKP_ACT_HIGH  // SPI_CKP_ACT_LOW, SPI_CKP_ACT_HIGH
	#define SPI_SMP			SPI_SMP_PHASE_MID // SPI_SMP_PHASE_MID, SPI_SMP_PHASE_END 
	#define SPI_MODE		SPI_MODE_16BITS
	#define SPI_MSTEN		SPI_MST_MODE_ENABLE

#ifdef __PIC32MX__
	#define SPI_BAUDRATE    1
#else
	#define SPI_SPRE		7 
	#define SPI_PPRE		3 
#endif

	#define EPD_SPI_STATt(i)	 SPI##i##STAT		
	#define EPD_SPI_STAT(i)      EPD_SPI_STATt(i)

	#define EPD_SPI_STATbitst(i) SPI##i##STATbits	
	#define EPD_SPI_STATbits(i)  EPD_SPI_STATbitst(i)

#ifdef __PIC32MX__
    #define EPD_SPI_CONt(i)       SPI##i##CON
    #define EPD_SPI_CON(i)        EPD_SPI_CONt(i)
    
    #define EPD_SPI_CONbitst(i)   SPI##i##CONbits
    #define EPD_SPI_CONbits(i)    EPD_SPI_CONbitst(i)
    
    #define EPD_SPI_BRGt(i)       SPI##i##BRG
    #define EPD_SPI_BRG(i)        EPD_SPI_BRGt(i)
    
    #define EPD_SPI_BRGbitst(i)   SPI##i##BRGbits
    #define EPD_SPI_BRGbits(i)    EPD_SPI_BRGbitst(i)
#else
    #define EPD_SPI_CONt(i)       SPI##i##CON1
    #define EPD_SPI_CON(i)        EPD_SPI_CONt(i)
    
    #define EPD_SPI_CONbitst(i)   SPI##i##CON1bits
    #define EPD_SPI_CONbits(i)    EPD_SPI_CONbitst(i)
    
    #define EPD_SPI_CON2t(i)      SPI##i##CON2
    #define EPD_SPI_CON2(i)       EPD_SPI_CON2t(i)
    
    #define EPD_SPI_CON2bitst(i)  SPI##i##CON2bits
    #define EPD_SPI_CON2bits(i)   EPD_SPI_CON2bitst(i)
#endif	

    #define EPD_SPI_BUFt(i)       SPI##i##BUF
    #define EPD_SPI_BUF(i)        EPD_SPI_BUFt(i)
    
    #define EPD_SPI_IFt(i)		  SPI##i##IF
    #define EPD_SPI_IF(i)		  EPD_SPI_IFt(i)
#endif //	#if defined ( USE_SPI_CHANNEL )				
	
/*********************************************************************
* HARDWARE PROFILE FOR DISPLAY CONTROLLER INTERFACE
*********************************************************************/
/* 
    The following defines the different IO signals used to connect
    to the display controller.
    The hardware profile used is dependent on the PIC device 
    and the development board used.
    
    The following are used to control the following signals to the
    Display Controller:
    1. Reset Signal - this signal controls the reset pin of the display
            controller if available.
            
            Example:
            #define DisplayResetConfig()        TRISAbits.TRISA4 = 0    
            #define DisplayResetEnable()        LATAbits.LATA4 = 0
            #define DisplayResetDisable()       LATAbits.LATA4 = 1

    2. Command/Data Signal - this signal indicates to the controller
            if the command or data is being sent to the display controller.

            Example:
            #define DisplayCmdDataConfig()      TRISAbits.TRISA1 = 0
            #define DisplaySetCommand()         LATAbits.LATA1 = 0
            #define DisplaySetData()            LATAbits.LATA1 = 1

    3. Controller Chip Select Signal - this signal controls the chip select
            pin of the display controller is available.

            Example:
            #define DisplayConfig()             TRISBbits.TRISB15 = 0             
            #define DisplayEnable()             LATBbits.LATB15 = 0
            #define DisplayDisable()            LATBbits.LATB15 = 1
            
    4. Display Module Power Control Signal - this signal turns on or off
            the display module if available. In some cases, this can be an 
            IO port that will enable the power circuitry of your 
            display module.

            Example:
            #define DisplayPowerConfig()        TRISAbits.TRISA5 = 0       
            #define DisplayPowerOn()            LATAbits.LATA5 = 1           
            #define DisplayPowerOff()           LATAbits.LATA5 = 0

    5. Display Backlight Brightness Control - this controls the backlight
            brightness or ON or OFF states if available. If brightness is 
            controllable, this signal may be driven by some PWM signal. 

            Example:
            #define DisplayBacklightConfig()    TRISDbits.TRISD0 = 0  
            #define DisplayBacklightOn()        LATDbits.LATD0 = 0
            #define DisplayBacklightOff()       LATDbits.LATD0 = 1               
    
*/

// -----------------------------------
// When using EXPLORER_16
// -----------------------------------
    
#if defined (EXPLORER_16) || defined (PIC_SK)

    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        // When using the supported dsPIC and PIC24H devices

        // Definitions for reset pin
        #define DisplayResetConfig()        TRISAbits.TRISA4 = 0    
        #define DisplayResetEnable()        LATAbits.LATA4 = 0
        #define DisplayResetDisable()       LATAbits.LATA4 = 1

        // Definitions for RS pin
        #define DisplayCmdDataConfig()      TRISAbits.TRISA1 = 0
        #define DisplaySetCommand()         LATAbits.LATA1 = 0
        #define DisplaySetData()            LATAbits.LATA1 = 1

        // Definitions for CS pin
        #define DisplayConfig()             TRISBbits.TRISB15 = 0             
        #define DisplayEnable()             LATBbits.LATB15 = 0
        #define DisplayDisable()            LATBbits.LATB15 = 1

        // Definitions for FLASH CS pin
        #define DisplayFlashConfig()       
        #define DisplayFlashEnable()       
        #define DisplayFlashDisable()      

        // Definitions for POWER ON pin
        #define DisplayPowerConfig()       
        #define DisplayPowerOn()           
        #define DisplayPowerOff()          

        // Definitions for backlight control pin
        #define DisplayBacklightConfig()      
        #define DisplayBacklightOn()        
        #define DisplayBacklightOff()          

        
    #elif defined (__PIC32MX__)
        // When using the supported PIC32 devices

        /********
         * PIC32 will use the SET and CLR SFRs for the I/O.
         * These are atomic settings that are recommended when
         * modifying SFRs
         ********/
        // Definitions for reset pin
        #define DisplayResetConfig()        TRISCCLR = _TRISC_TRISC1_MASK    
        #define DisplayResetEnable()        LATCCLR = _LATC_LATC1_MASK
        #define DisplayResetDisable()       LATCSET = _LATC_LATC1_MASK

        // Definitions for RS pin
        #define DisplayCmdDataConfig()      TRISCCLR = _TRISC_TRISC2_MASK
        #define DisplaySetCommand()         LATCCLR = _LATC_LATC2_MASK
        #define DisplaySetData()            LATCSET = _LATC_LATC2_MASK

        // Definitions for CS pin
		#if defined (USE_SPI_CHANNEL)
			#if (USE_SPI_CHANNEL == 1)
				#if defined(__32MX460F512L__) || defined(__32MX795F512L__)
					// For this PIM (MA320002), RD10 & RF6 are connected together.
					// For E-Paper PICTail, RD10 is used for HCS_L & RF6 is used for SCK1.
					// In order to use serial interface on this PIM, EXPLORER_16 J6
					// pin 103 has to be masked out (disconnected). 
					#define SPI_CS_CFG		TRISDCLR = _TRISD_TRISD9_MASK
					#define SPI_CS_SET		LATDSET = _LATD_LATD9_MASK
					#define SPI_CS_CLR		LATDCLR = _LATD_LATD9_MASK
				#else
					#define SPI_CS_CFG		TRISBCLR = _TRISB_TRISB2_MASK
					#define SPI_CS_SET		LATBSET = _LATB_LATB2_MASK
					#define SPI_CS_CLR		LATBCLR = _LATB_LATB2_MASK
				#endif
			#else	// assumed to be 2
				#define SPI_CS_CFG			TRISGCLR = _TRISG_TRISG9_MASK
				#define SPI_CS_SET			LATGSET = _LATG_LATG9_MASK
				#define SPI_CS_CLR			LATGCLR = _LATG_LATG9_MASK
			#endif

			#if defined (USE_SST25VF064)
				// if more than one SPI channel used include drv_spi.h
		        #define DisplayConfig()      		SPI_CS_CFG
				#define DisplayEnable()        		{ while(!SPILock(USE_SPI_CHANNEL)); SPI_CS_CLR; }
				#define DisplayDisable()            { SPIUnLock(USE_SPI_CHANNEL);       SPI_CS_SET; }
			#else
				#define DisplayConfig()		SPI_CS_CFG
				#define DisplayEnable()		SPI_CS_CLR
				#define DisplayDisable()	SPI_CS_SET
			#endif
		#else
        	#define DisplayConfig()			TRISDCLR = _TRISD_TRISD10_MASK
        	#define DisplayEnable()			LATDCLR = _LATD_LATD10_MASK
        	#define DisplayDisable()		LATDSET = _LATD_LATD10_MASK
		#endif
	        
//        #define BusyLineConfig()

        // Definitions for FLASH CS pin
        #define DisplayFlashConfig()          
        #define DisplayFlashEnable()        
        #define DisplayFlashDisable()       

        // Definitions for POWER ON pin
        #define DisplayPowerConfig()        
        #define DisplayPowerOn()            
        #define DisplayPowerOff()            
        
        // Definitions for backlight control pin
        #define DisplayBacklightConfig()
        #define DisplayBacklightOn()
        #define DisplayBacklightOff() 

    #elif defined (__PIC24F__) || defined (__dsPIC33E__) || defined (__PIC24E__)
        /* When using the supported PIC24F devices
            PIC24FJ128GA010
            PIC24FJ256GA110
            PIC24FJ256GB110
            PIC24FJ256GB210
           NOTE: PIC24FJ256DA210 is not on the list since
                 this part has a built in display controller
                 and it directly interfaces to the display 
                 panel.
        */
        
        // Definitions for reset pin
        #define DisplayResetConfig()        TRISCbits.TRISC1 = 0    
        #define DisplayResetEnable()        LATCbits.LATC1 = 0
        #define DisplayResetDisable()       LATCbits.LATC1 = 1

        // Definitions for RS pin
        #define DisplayCmdDataConfig()      TRISCbits.TRISC2 = 0
        #define DisplaySetCommand()         LATCbits.LATC2 = 0
        #define DisplaySetData()            LATCbits.LATC2 = 1


		#if defined (USE_SPI_CHANNEL)
			// Definitions for CS pin
			#if (USE_SPI_CHANNEL == 1)
				#define SPI_CS_TRIS   TRISBbits.TRISB2
				#define SPI_CS_LAT    LATBbits.LATB2
			#else	// assumed to be 2
				#define SPI_CS_TRIS   TRISGbits.TRISG9
				#define SPI_CS_LAT    LATGbits.LATG9
			#endif
			
			/* The followings are currently not used
			#define SPI_SCK_TRIS  TRISBbits.TRISB0
			#define SPI_SDO_TRIS  TRISFbits.TRISF8
			#define SPI_SDI_TRIS  TRISDbits.TRISD2
			
			#define SPI_SS_ANS   ANSBbits.ANSB2
			
			#define SPI_SDI_ANS   ANSDbits.ANSD2
			*/

			#if defined (USE_SST25VF064)
				// if more than one SPI channel used include drv_spi.h
		        #define DisplayConfig()      		SPI_CS_TRIS  = 0
				#define DisplayEnable()        		{ while(!SPILock(USE_SPI_CHANNEL)); SPI_CS_LAT = 0; }
				#define DisplayDisable()            { SPIUnLock(USE_SPI_CHANNEL);       SPI_CS_LAT = 1; }
			#else	
				#define DisplayConfig()      		SPI_CS_TRIS  = 0
				#define DisplayEnable()        		SPI_CS_LAT = 0 
				#define DisplayDisable()            SPI_CS_LAT = 1 
			#endif
			
// Moved to the end because HRDY is always connected and has to be configured as input to avoid signal contention
//			#if defined( USE_BUSY_LINE )
				// PIM PIC24FJ256GB210: Remove jumper JP2 and put one on JP4 !!!
//				#define BusyLineConfig()			TRISGbits.TRISG1 = 1
//				#define EPD_BUSY_LINE				(PORTGbits.RG1 == 0)
//			#else
//				#define BusyLineConfig()
//			#endif
		#else	
	        // Definitions for CS pin
	        #define DisplayConfig()             TRISDbits.TRISD10 = 0             
	        #define DisplayEnable()             LATDbits.LATD10 = 0 
	        #define DisplayDisable()            LATDbits.LATD10 = 1 
	        
//	        #define BusyLineConfig()
	    #endif
        
        // Definitions for FLASH CS pin
        #define DisplayFlashConfig()         
        #define DisplayFlashEnable()        
        #define DisplayFlashDisable()       

        // Definitions for POWER ON pin
        #define DisplayPowerConfig()        
        #define DisplayPowerOn()            
        #define DisplayPowerOff()           
        
        // Definitions for backlight control pin
        #define DisplayBacklightConfig()
        #define DisplayBacklightOn()
        #define DisplayBacklightOff()
        

    #else
    
        #error "Selected Device is not supported"
        
    #endif

    #define BusyLineConfig()				TRISAbits.TRISA2 = 1
	#if defined( USE_BUSY_LINE )
		#define EPD_BUSY_LINE				(PORTAbits.RA2 == 0)
	#endif
// end of #if defined (EXPLORER_16)

#endif // end of #elif defined (MEB_BOARD)




/*********************************************************************
* HARDWARE PROFILE FOR THE SPI FLASH MEMORY
*********************************************************************/
/*
	These are the hardware settings for the SPI peripherals used.
*/

#if defined (USE_MCHP25LC256) 

    /*********************************************************************
	* Serial Bus SPI EEPROM (MCHP 25LC256) On Explorer 16  
	*********************************************************************/
	// When using Explorer 16, the on board EEPROM SPI (25LC256) is present 
	// so we define the chip select signal used. This is needed even 
	// if the memory is not used. Set to de-asserted state to avoid conflicts
	// when not used. That is why the EXPLORER_16 check is also needed.
	// see...#if defined (USE_MCHP25LC256) || defined (EXPLORER_16)

    /* Define all the SPI channels that will be used here.
       These will be used to determine how the SPI Driver (drv_spi)
       will be compiled.
    */
    // Chip Select signal 

    #if (MCHP25LC256_SPI_CHANNEL == 1)
        #define SPI_CHANNEL_1_ENABLE
    #elif (MCHP25LC256_SPI_CHANNEL == 2)
        #define SPI_CHANNEL_2_ENABLE
    #elif (MCHP25LC256_SPI_CHANNEL == 3)
        #define SPI_CHANNEL_3_ENABLE
    #elif (MCHP25LC256_SPI_CHANNEL == 4)
        #define SPI_CHANNEL_4_ENABLE
    #endif

    // Clock, SDO and SDI signals
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define MCHP25LC256_SCK_TRIS TRISCbits.TRISC2
        #define MCHP25LC256_SDO_TRIS TRISCbits.TRISC0
        #define MCHP25LC256_SDI_TRIS TRISCbits.TRISC1
    #else
	    #define MCHP25LC256_SCK_TRIS TRISGbits.TRISG6
	    #define MCHP25LC256_SDO_TRIS TRISGbits.TRISG8
	    #define MCHP25LC256_SDI_TRIS TRISGbits.TRISG7
    #endif    

#endif // #if defined (USE_MCHP25LC256)

// The MCHP25LC256 chip select signal, even if not used must be
// driven to high so it does not interfere with other SPI peripherals
// that uses the same SPI signals. 
#if  defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__) || defined(__dsPIC33E__) || defined(__PIC24E__)
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define MCHP25LC256_CS_TRIS  TRISAbits.TRISA0
        #define MCHP25LC256_CS_LAT   LATAbits.LATA0
    #elif defined(__PIC24FJ256GB110__)
// This PIM has RD12 rerouted to RG0
        #define MCHP25LC256_CS_TRIS  TRISGbits.TRISG0
        #define MCHP25LC256_CS_LAT   LATGbits.LATG0
    #else
        #define MCHP25LC256_CS_TRIS  TRISDbits.TRISD12
        #define MCHP25LC256_CS_LAT   LATDbits.LATD12
    #endif
#elif defined(__PIC32MX__)
    #define MCHP25LC256_CS_TRIS  TRISDbits.TRISD12
    #define MCHP25LC256_CS_LAT   LATDbits.LATD12
#endif

#if defined (USE_SST25VF064) 

	/*********************************************************************
	* SPI Flash Memory on E-Paper Display Epson Pictail Plus Board 
	*********************************************************************/
	// Set up the signals used to communicate to the SPI Flash device 

    // define the SPI channel to be used
//    #if defined (EXPLORER_16)
        #define SST25_SPI_CHANNEL 2
//    #endif	

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


    // Chip Select, SCLK, SDI and SDO signals used 
    #if defined (EXPLORER_16)
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
            #define SST25_CS_TRIS   TRISAbits.TRISA8
            #define SST25_CS_LAT    LATAbits.LATA8
            
            #define SST25_SCK_TRIS  TRISCbits.TRISC2
            #define SST25_SDO_TRIS  TRISCbits.TRISC0
            #define SST25_SDI_TRIS  TRISCbits.TRISC1
            
        #else
			// For the moment, always use ALT_FLASH_CS (EXPLORER_16 J6-Pin 94, RD1)
			// for accessing SST25VF064 on the E-Paper PICTail board.
            #define SST25_CS_TRIS   TRISDbits.TRISD1
            #define SST25_CS_LAT    LATDbits.LATD1
            
            #define SST25_SCK_TRIS  TRISGbits.TRISG6
            #define SST25_SDO_TRIS  TRISGbits.TRISG8
            #define SST25_SDI_TRIS  TRISGbits.TRISG7
            
            #if defined (__dsPIC33E__) || defined(__PIC24E__)
				#define SST25_SCK_LAT  	LATGbits.LATG6
	    		#define SST25_SCK_ANS   ANSELGbits.ANSG6
	    		#define SST25_SDO_ANS   ANSELGbits.ANSG8
	    		#define SST25_SDO_LAT  	LATGbits.LATG8
	        	#define SST25_SDI_LAT  	LATGbits.LATG7  
	        	#define SST25_SDI_ANS   ANSELGbits.ANSG7       		
    		#else            
    			#define SST25_SDI_ANS   ANSGbits.ANSG7
    		#endif
        #endif
    #elif  defined(PIC_SK)
            #define SST25_CS_TRIS   TRISGbits.TRISG9 //TRISDbits.TRISD1
            #define SST25_CS_LAT    LATGbits.LATG9 //LATDbits.LATD1
            
            #define SST25_SCK_TRIS  TRISGbits.TRISG6
            #define SST25_SDO_TRIS  TRISGbits.TRISG8
            #define SST25_SDI_TRIS  TRISGbits.TRISG7    
    #endif

#endif // #if defined (USE_SST25VF064)


/*********************************************************************
* IOS FOR THE SWITCHES (SIDE BUTTONS)
*********************************************************************/
typedef enum
{
    HW_BUTTON_PRESS = 0,
    HW_BUTTON_RELEASE = 1
}HW_BUTTON_STATE;

 
#if defined (EXPLORER_16)
	#if defined (__PIC24FJ256GB210__)
        #define HardwareButtonInit()        (ANSBbits.ANSB14 = 0) 
        #define GetHWButtonS1()             (PORTGbits.RG15)
        #define GetHWButtonS2()             (PORTDbits.RD3)
        #define GetHWButtonS3()             (PORTBbits.RB14)
    #elif defined(__PIC24FJ128GA010__) || defined(__PIC32MX)
        #define HardwareButtonInit()        (AD1PCFGbits.PCFG14 = 1) //((*(WORD*)(0x032C)) = 0x4000)
        #define GetHWButtonS1()             (PORTCbits.RC3)
        #define GetHWButtonS2()             (PORTDbits.RD3)
        #define GetHWButtonS3()             (PORTBbits.RB14)
    #endif
#elif defined(PIC_SK)
//       #define HardwareButtonInit()   
        #define GetHWButtonS1()     0 //TBD
        #define GetHWButtonS2()     0
        #define GetHWButtonS3()     0
        
 /*     #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define HardwareButtonInit()
        #define GetHWButtonProgram()    (_RA9)
        #define GetHWButtonScanDown()   (_RA9)
        #define GetHWButtonScanUp()		(HW_BUTTON_RELEASE)
        #define GetHWButtonCR()			(HW_BUTTON_RELEASE)
        #define GetHWButtonFocus()  	(HW_BUTTON_RELEASE)*/
      //#elif defined (PIC32_SK) 
        #ifdef USE_16BIT_PMP
            #define HardwareButtonInit()
            #define GetHWButtonProgram()        (HW_BUTTON_RELEASE)
            #define GetHWButtonScanDown()       (HW_BUTTON_RELEASE)
            #define GetHWButtonScanUp()         (HW_BUTTON_RELEASE)
            #define GetHWButtonCR()             (HW_BUTTON_RELEASE)
            #define GetHWButtonFocus()          (HW_BUTTON_RELEASE)
        #else
            #define HardwareButtonInit()        (CNPUESET = _CNPUE_CNPUE16_MASK | _CNPUE_CNPUE15_MASK | _CNPUE_CNPUE19_MASK)
            #define GetHWButtonProgram()        (PORTDbits.RD6)
            #define GetHWButtonScanDown()       (HW_BUTTON_RELEASE)
            #define GetHWButtonScanUp()         (HW_BUTTON_RELEASE)  
            #define GetHWButtonCR()             (PORTDbits.RD6)
            #define GetHWButtonFocus()          (PORTDbits.RD7 & PORTDbits.RD13)
        #endif
/*
      #else
        #define HardwareButtonInit()
        #ifdef USE_16BIT_PMP
            #define GetHWButtonProgram()        (PORTAbits.RA7)
            #define GetHWButtonScanDown()       (HW_BUTTON_RELEASE)
            #define GetHWButtonScanUp()         (HW_BUTTON_RELEASE)
            #define GetHWButtonCR()             (HW_BUTTON_RELEASE)
            #define GetHWButtonFocus()          (HW_BUTTON_RELEASE)
        #else
            #define GetHWButtonProgram()        (PORTDbits.RD7)
            #define GetHWButtonScanDown()       (PORTDbits.RD13)
            #define GetHWButtonScanUp()         (PORTAbits.RA7)  
            #define GetHWButtonCR()             (PORTDbits.RD6)
            #define GetHWButtonFocus()          (PORTDbits.RD7)
        #endif
    #endif
    */
#endif 


/*********************************************************************
* POT MACROS
*********************************************************************/
#if defined (EXPLORER_16) || defined (PIC_SK)  
	#if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
		// ADC channel constants
		// Potentiometer 
		#define ADC_POT         0

		// ADC Port Control Bits
		#define ADC_POT_TRIS    TRISAbits.TRISA0
		#define ADC_POT_PCFG    AD1PCFGLbits.PCFG0
		
	#elif defined(__PIC32MX__)

		// ADC channel constants
		// Potentiometer and Temperature sensor
        #if !defined (PIC32_SK)
            // potentiometer is not present in PIC32 Starter Kits
            #define ADC_POT			ADC_CH0_POS_SAMPLEA_AN5
        #endif    
           
		// ADC Port Control Bits
        #if !defined (PIC32_SK)
            // potentiometer is not present in PIC32 Starter Kits
            #define ADC_POT_PCFG    AD1PCFGbits.PCFG5
        #endif           
    #else //#else defined (__PIC24F__) || defined(__dsPIC33E__) || defined(__PIC24E__)

		// ADC channel constants
		// Potentiometer and Temperature sensor
		#if defined(__PIC24F__)
		    #define ADC_POT         5

		// ADC Port Control Bits
   		#if defined(__PIC24FJ256GB210__)
        	#define ADC_POT_PCFG	ANSBbits.ANSB5
           #else
       		#define ADC_POT_PCFG    AD1PCFGbits.PCFG5
           #endif
		#endif
	#endif
#endif // end of defined (EXPLORER_16) || defined (PIC_SK)


/*********************************************************************
* IOS FOR THE UART
*********************************************************************/
#if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    #define TX_TRIS TRISCbits.TRISC1
    #define RX_TRIS TRISCbits.TRISC0
#elif defined(__PIC24FJ256DA210__)    
    #define TX_TRIS TRISFbits.TRISF3
    #define RX_TRIS TRISDbits.TRISD0
#else
    #define TX_TRIS TRISFbits.TRISF5
    #define RX_TRIS TRISFbits.TRISF4
#endif

/*********************************************************************
* RTCC DEFAULT INITIALIZATION (these are values to initialize the RTCC
*********************************************************************/
#define RTCC_DEFAULT_DAY        02      // 2nd
#define RTCC_DEFAULT_MONTH      06      // June
#define RTCC_DEFAULT_YEAR       11      // 2011
#define RTCC_DEFAULT_WEEKDAY    05      // Friday
#define RTCC_DEFAULT_HOUR       10      // 10:10:01
#define RTCC_DEFAULT_MINUTE     10
#define RTCC_DEFAULT_SECOND     01

/*********************************************************************
* LEDs CONFIGURATION
*********************************************************************/
#if defined (EXPLORER_16)
	//	A2 is connected to HRDY output from S1D13522, so it has to be set as input
	#define InitAllLEDs()       LATA &= 0x00, TRISA &= 0x04 

	#define LED0_Off()          LATAbits.LATA0 = 0
	#define LED0_On()           LATAbits.LATA0 = 1 
	#define LED1_Off()          LATAbits.LATA1 = 0
	#define LED1_On()           LATAbits.LATA1 = 1
	#define LED2_Off()          LATAbits.LATA2 = 0
	#define LED2_On()           LATAbits.LATA2 = 1 
	#define LED3_Off()          LATAbits.LATA3 = 0
	#define LED3_On()           LATAbits.LATA3 = 1
	#define LED4_Off()          LATAbits.LATA4 = 0
	#define LED4_On()           LATAbits.LATA4 = 1 
	#define LED5_Off()          LATAbits.LATA5 = 0
	#define LED5_On()           LATAbits.LATA5 = 1
	#define LED6_Off()          LATAbits.LATA6 = 0
	#define LED6_On()           LATAbits.LATA6 = 1 
	#define LED7_Off()          LATAbits.LATA7 = 0
	#define LED7_On()           LATAbits.LATA7 = 1
	#define LEDAll_On()         LATA |= 0xFF
	#define LEDAll_Off()        LATA &= 0xFF00
	#define LEDPattern(x)      (LEDAll_Off(), LATA |= (unsigned char)(x))
#else
	#define InitAllLEDs()
#endif	

#endif // __HARDWARE_PROFILE_H



