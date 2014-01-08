/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  EPMP driver
 *****************************************************************************
 * FileName:        gfxepmp.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/12/10	    ...
 * 03/09/11     - Modified dependencies.
 *              - For Graphics Library Version 3.00
 *              - Removed calls to Time Delay function. They should be 
 *                set/called in the driver.
 *              - Replace Device_Init() to DriverInterfaceInit()
 * 03/14/11     - Modified PMP timing macros to use EPMPCSx_DATA_SETUP_TIME, 
 *                EPMPCSx_DATA_WAIT_TIME, and EPMPCSx_DATA_HOLD_TIME.
 *              - Added initialization of EPMP CS2.
 * 03/27/11     Added checks for PIC24FJ256DA210 to remove errors when 
 *              using non DA part.
 *****************************************************************************/
#ifndef _GFX_EPMP_H_FILE
#define _GFX_EPMP_H_FILE


#include "HardwareProfile.h"
#include "Compiler.h"
#include "TimeDelay.h"

#ifdef USE_GFX_EPMP

#ifdef USE_16BIT_PMP
extern volatile __eds__ WORD __attribute__((eds,noload)) epmp_data;
#else
extern volatile __eds__ BYTE __attribute__((eds,noload)) epmp_data;
#endif

// Note:
/*
    All functions here are defined as inline functions for performance.
    When debugging this portion it is best to look at the assembly output 
    and debug from there.
*/

    // error checking
    #if !defined (EPMPCS1_DATA_SETUP_TIME) && !defined (EPMPCS2_DATA_SETUP_TIME)
        #error "Define EPMPCS1_DATA_SETUP_TIME and/or EPMPCS2_DATA_SETUP_TIME in HardwareProfile.h for data set up before read/write strobe timing"
    #endif    
    #if !defined (EPMPCS1_DATA_WAIT_TIME) && !defined (EPMPCS2_DATA_WAIT_TIME)
        #error "Define EPMPCS1_DATA_WAIT_TIME and/or EPMPCS2_DATA_WAIT_TIME in HardwareProfile.h for read/write strobe wait states"
    #endif    
    #if !defined (EPMPCS1_DATA_HOLD_TIME) && !defined (EPMPCS2_DATA_HOLD_TIME)
        #error "Define EPMPCS1_DATA_HOLD_TIME and/or EPMPCS2_DATA_HOLD_TIME in HardwareProfile.h for data hold after read/write strobe"
    #endif    

/*********************************************************************
* Macros:  PMPWaitBusy()
*
* Overview: waits for PMP cycle end.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define PMPWaitBusy()   while(PMCON2bits.BUSY); 

/*********************************************************************
 * Section: Deprecated Macros
 ********************************************************************/
#ifdef RS_LAT_BIT

#warning "RS_LAT_BIT is being deprecated. Define DisplaySetCommand(), DisplaySetData() & DisplayCmdDataConfig() in HardwareProfile.h and use them instead."

extern inline void __attribute__ ((always_inline)) DisplaySetCommand()
{
	RS_LAT_BIT = 0;
}

extern inline void __attribute__ ((always_inline)) DisplaySetData()
{
	RS_LAT_BIT = 1;
}

extern inline void __attribute__ ((always_inline)) DisplayCmdDataConfig()
{
	RST_TRIS_BIT = 0;
}

#endif

#ifdef CS_LAT_BIT

#warning "CS_LAT_BIT is being deprecated. Define DisplayEnable(), DisplayDisable() & DisplayConfig() in HardwareProfile.h and use them instead."

extern inline void __attribute__ ((always_inline)) DisplayEnable()
{
	CS_LAT_BIT = 0;
}

extern inline void __attribute__ ((always_inline)) DisplayDisable()
{
	CS_LAT_BIT = 1;
}

extern inline void __attribute__ ((always_inline)) DisplayConfig()
{
	CS_TRIS_BIT = 0;
}

#endif

#ifdef RST_LAT_BIT

#warning "RST_LAT_BIT is being deprecated. Define DisplayResetEnable(), DisplayResetDisable() & DisplayResetConfig() in HardwareProfile.h and use them instead."

extern inline void __attribute__ ((always_inline)) DisplayResetEnable()
{
	RST_LAT_BIT = 0;
}

extern inline void __attribute__ ((always_inline)) DisplayResetDisable()
{
	RST_LAT_BIT = 1;
}

extern inline void __attribute__ ((always_inline)) DisplayResetConfig()
{
	RST_TRIS_BIT = 0;
}

#endif

/*********************************************************************
* Macros:  DeviceWrite(data)
*
* PreCondition:  none
*
* Input: data - value to be written to RAM
*
* Output: none
*
* Side Effects: none
*
* Overview: writes data into controller's RAM
*
* Note: chip select should be enabled
*
********************************************************************/
#ifdef USE_16BIT_PMP

extern inline void __attribute__ ((always_inline)) DeviceWrite(WORD data) 
{ 
	epmp_data = data;
	PMPWaitBusy();
}

#else

extern inline void __attribute__ ((always_inline)) DeviceWrite(BYTE data)
{
	epmp_data = data;
	PMPWaitBusy();
}

#endif

/*********************************************************************
* Macros:  DeviceRead()
*
* PreCondition:  none
*
* Input: none
*
* Output: data read
*
* Side Effects: none
*
* Overview: reads data from controller's RAM
*
* Note: chip select should be enabled
*
********************************************************************/
#ifdef USE_16BIT_PMP

extern inline WORD __attribute__ ((always_inline)) DeviceRead()
{
WORD value;
	value = epmp_data;
	PMPWaitBusy();
	return PMDIN1;
}

#else


extern inline BYTE __attribute__ ((always_inline)) DeviceRead(){
BYTE value;
	value = epmp_data;
	PMPWaitBusy();
	return PMDIN1;
}

#endif


/*********************************************************************
* Function:  DeviceInit()
*
* PreCondition:  none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes the device
*
* Note: none
*
********************************************************************/
extern inline void __attribute__ ((always_inline)) DriverInterfaceInit(void)
{ 
#if defined (EPMPCS1_ADDRESS_LINE_COUNT) || defined (EPMPCS2_ADDRESS_LINE_COUNT)
    WORD i, addrEnable, addrCount;
#endif
    
	// variable for PMP timing calculation
	// GetPeripheralClock() is in Mhz. pClockPeriod will be in nanoseconds.
    WORD pClockPeriod = (1000000000ul) / GetPeripheralClock();

	DisplayResetEnable();               // hold in reset by default
    DisplayResetConfig();               // enable RESET line
    DisplayCmdDataConfig();             // enable RS line
    DisplayDisable();                   // not selected by default
    DisplayConfig();                    // enable chip select line
    DisplayBacklightOff();              // initially set the backlight to off
    DisplayBacklightConfig();           // set the backlight control pin

	ANSDbits.ANSD7 = 0;			// PMD15
	ANSDbits.ANSD6 = 0;			// PMD14
	ANSFbits.ANSF0 = 0;			// PMD11

	ANSBbits.ANSB15 = 0;		// PMA0
	ANSBbits.ANSB14 = 0;		// PMA1
	ANSGbits.ANSG9  = 0;		// PMA2
	ANSBbits.ANSB13 = 0;		// PMA10
	ANSBbits.ANSB12 = 0;		// PMA11
	ANSBbits.ANSB11 = 0;		// PMA12
	ANSBbits.ANSB10 = 0;		// PMA13
	ANSAbits.ANSA7 = 0;			// PMA17
	ANSGbits.ANSG6 = 0;			// PMA18

    
    // EPMP setup
	PMCON1bits.ADRMUX = 0;	    // address is not multiplexed
	PMCON1bits.MODE = 3;        // master mode
	PMCON1bits.CSF = 0;         // PMCS1 pin used for chip select 1, PMCS2 pin used for chip select 2
	PMCON1bits.ALP = 0;         // set address latch strobes to high active level 
	PMCON1bits.ALMODE = 0;      // "smart" address strobes are not used
	PMCON1bits.BUSKEEP = 0;     // bus keeper is not used
	
	PMCON2bits.RADDR = 0xff;	// set CS2 end address
	
#if defined (__PIC24FJ256DA210__)	
	PMCON2bits.MSTSEL = 0;		// set CPU as Master
#endif
    
    //enable the address lines if required
    #if defined (EPMPCS1_ADDRESS_LINE_COUNT) || defined (EPMPCS2_ADDRESS_LINE_COUNT)
        // decide how many address lines to use
        #if (EPMPCS1_ADDRESS_LINE_COUNT > EPMPCS2_ADDRESS_LINE_COUNT)
            addrCount = EPMPCS1_ADDRESS_LINE_COUNT;
        #else     
            addrCount = EPMPCS2_ADDRESS_LINE_COUNT;
        #endif
            
        for(addrEnable = 1, i = 0; i < addrCount; i++)
        {
            if (i <= 15)
                PMCON4 |= addrEnable;
            else
                PMCON3 |= addrEnable;
            
            // change the addrEnable back to 1 when it reaches 16 count
            if (addrEnable == 0x8000)
                addrEnable = 0x0001;
            else
                addrEnable = addrEnable << 1;    
        }    
    #else    
    
        PMCON4 = 0;                 // PMA0 - PMA15 address lines are disabled
        PMCON3 |= 0;                // PMA16 - PMA17 address line is disabled
        
    #endif    

    #if defined (EPMPCS1_DATA_SETUP_TIME)

#if defined (__PIC24FJ256DA210__)	
	    PMCS1BS = (GFX_EPMP_CS1_BASE_ADDRESS >> 8);
#else	  
	    PMCS1BS = 0x0200; 	        // CS1 start address
	    PMCS2BS = 0xff00;			// set CS1 end address and CS2 start address  
#endif	    

    	PMCS1CFbits.CSDIS = 0;      // enable CS
    	PMCS1CFbits.CSP = 0;        // CS active low
    	PMCS1CFbits.CSPTEN = 1;     // enable CS port
    	PMCS1CFbits.BEP = 0;        // byte enable active low
    	PMCS1CFbits.WRSP = 0;       // write strobe active low
    	PMCS1CFbits.RDSP = 0;       // read strobe active low
    	PMCS1CFbits.SM = 0;         // read and write strobes on separate lines 
    
        #ifdef USE_16BIT_PMP
        	PMCS1CFbits.PTSZ = 2;       // data bus width is 16-bit 
        #else
        	PMCS1CFbits.PTSZ = 0;       // data bus width is 8-bit 
        #endif
    
    	PMCS1MDbits.ACKM = 0;        // PMACK is not used

    
        if (EPMPCS1_DATA_SETUP_TIME < (pClockPeriod/4))
            PMCS1MDbits.DWAITB = 0;
        else if (EPMPCS1_DATA_SETUP_TIME >= (pClockPeriod/4))
        {
            PMCS1MDbits.DWAITB = (EPMPCS1_DATA_SETUP_TIME / pClockPeriod);
            if ((EPMPCS1_DATA_SETUP_TIME % pClockPeriod) > 0)
                PMCS1MDbits.DWAITB += 1;
        }        
                
        if (EPMPCS1_DATA_WAIT_TIME < ((pClockPeriod*3)/4))
            PMCS1MDbits.DWAITM = 0;
        else if (EPMPCS1_DATA_WAIT_TIME >= ((pClockPeriod*3)/4))
        {
            PMCS1MDbits.DWAITM = (EPMPCS1_DATA_WAIT_TIME / pClockPeriod);
            if ((EPMPCS1_DATA_WAIT_TIME % pClockPeriod) > 0)
                PMCS1MDbits.DWAITM += 1;
        }        

        if (EPMPCS1_DATA_HOLD_TIME <= (pClockPeriod/4))
            PMCS1MDbits.DWAITE = 0;
        else if (EPMPCS1_DATA_HOLD_TIME >= (pClockPeriod/4))
        {
            PMCS1MDbits.DWAITE = (EPMPCS1_DATA_HOLD_TIME / pClockPeriod);
            if ((EPMPCS1_DATA_HOLD_TIME % pClockPeriod) > 0)
                PMCS1MDbits.DWAITE += 1;
        }    
        
    #endif // #if defined (EPMPCS1_DATA_SETUP_TIME)
    
    #if defined (EPMPCS2_DATA_SETUP_TIME)

#if defined (__PIC24FJ256DA210__)	
    	PMCS2BS = (GFX_EPMP_CS2_BASE_ADDRESS >> 8);
#endif    	

    	PMCS2CFbits.CSDIS = 0;      // enable CS
    	PMCS2CFbits.CSP = 0;        // CS active low
    	PMCS2CFbits.CSPTEN = 1;     // enable CS port
    	PMCS2CFbits.BEP = 0;        // byte enable active low
    	PMCS2CFbits.WRSP = 0;       // write strobe active low
    	PMCS2CFbits.RDSP = 0;       // read strobe active low
    	PMCS2CFbits.SM = 0;         // read and write strobes on separate lines 
    
        #ifdef USE_16BIT_PMP
        	PMCS2CFbits.PTSZ = 2;       // data bus width is 16-bit 
        #else
        	PMCS2CFbits.PTSZ = 0;       // data bus width is 8-bit 
        #endif
    
    	PMCS2MDbits.ACKM = 0;        // PMACK is not used

        if (EPMPCS2_DATA_SETUP_TIME < (pClockPeriod/4))
            PMCS2MDbits.DWAITB = 0;
        else if (EPMPCS2_DATA_SETUP_TIME >= (pClockPeriod/4))
        {
            PMCS2MDbits.DWAITB = (EPMPCS2_DATA_SETUP_TIME / pClockPeriod);
            if ((EPMPCS2_DATA_SETUP_TIME % pClockPeriod) > 0)
                PMCS2MDbits.DWAITB += 1;
        }        
                
        if (EPMPCS2_DATA_WAIT_TIME < ((pClockPeriod*3)/4))
            PMCS2MDbits.DWAITM = 0;
        else if (EPMPCS2_DATA_WAIT_TIME >= ((pClockPeriod*3)/4))
        {
            PMCS2MDbits.DWAITM = (EPMPCS2_DATA_WAIT_TIME / pClockPeriod);
            if ((EPMPCS2_DATA_WAIT_TIME % pClockPeriod) > 0)
                PMCS2MDbits.DWAITM += 1;
        }        

        if (EPMPCS2_DATA_HOLD_TIME <= (pClockPeriod/4))
            PMCS2MDbits.DWAITE = 0;
        else if (EPMPCS2_DATA_HOLD_TIME >= (pClockPeriod/4))
        {
            PMCS2MDbits.DWAITE = (EPMPCS2_DATA_HOLD_TIME / pClockPeriod);
            if ((EPMPCS2_DATA_HOLD_TIME % pClockPeriod) > 0)
                PMCS2MDbits.DWAITE += 1;
        }    

    #endif // #if defined (EPMPCS2_DATA_SETUP_TIME)

	PMCON3bits.PTWREN = 1;      // enable write strobe port
	PMCON3bits.PTRDEN = 1;      // enable read strobe port
	PMCON3bits.PTBE0EN = 0;     // disable byte enable port
	PMCON3bits.PTBE1EN = 0;     // disable byte enable port
	PMCON3bits.AWAITM = 0;      // don't care
	PMCON3bits.AWAITE = 0;      // don't care
	
	PMCON1bits.PMPEN = 1;

    DisplayResetDisable();                         // release from reset

	// hard delay inserted here for devices that needs delays after reset.
	// Value will vary from device to device, please refer to the specific 
	// device data sheet for details.
    Delay10us(20);
	
}

#endif // USE_GFX_EPMP
#endif //#ifndef _GFX_EPMP_H_FILE



