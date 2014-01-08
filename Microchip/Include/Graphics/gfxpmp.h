    
/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  PMP driver
 *****************************************************************************
 * FileName:        pmp.c
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
 * Date			Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/12/10		...
 * 02/24/11     Replace Device_Init() to DriverInterfaceInit()
 * 03/14/11     Modified PMP timing macros to use PMP_DATA_SETUP_TIME, 
 *              PMP_DATA_WAIT_TIME, and PMP_DATA_HOLD_TIME.
 *****************************************************************************/
#ifndef _GFX_PMP_H_FILE
#define _GFX_PMP_H_FILE

#include "HardwareProfile.h"
#include "Compiler.h"
#include "TimeDelay.h"

#ifdef USE_GFX_PMP

// Note:
/*
    All functions here are defined as inline functions for performance.
    When debugging this portion it is best to look at the assembly output 
    and debug from there.
*/

    // error checking
    #ifndef PMP_DATA_SETUP_TIME
        #error "Define PMP_DATA_SETUP_TIME in HardwareProfile.h for data set up before read/write strobe timing"
    #endif    
    #ifndef PMP_DATA_WAIT_TIME
        #error "Define PMP_DATA_WAIT_TIME in HardwareProfile.h for read/write strobe wait states"
    #endif    
    #ifndef PMP_DATA_HOLD_TIME
        #error "Define PMP_DATA_HOLD_TIME in HardwareProfile.h for data hold after read/write strobe"
    #endif    
    
#ifdef __PIC32MX__
#define PMDIN1              PMDIN
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
#define PMPWaitBusy()   while(PMMODEbits.BUSY); 

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
#if defined (USE_16BIT_PMP)

extern inline void __attribute__ ((always_inline)) DeviceWrite(WORD data) 
{ 
	PMDIN1 = data;
	PMPWaitBusy();
}

#else//if defined (USE_8BIT_PMP)

extern inline void __attribute__ ((always_inline)) DeviceWrite(BYTE data)
{
	PMDIN1 = data;
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
#if defined (USE_16BIT_PMP)

extern inline WORD __attribute__ ((always_inline)) DeviceRead()
{
WORD value;
	value = PMDIN1;
	PMPWaitBusy();
	PMCONbits.PMPEN = 0; // disable PMP
	value = PMDIN1;
	PMPWaitBusy();
	PMCONbits.PMPEN = 1; // enable  PMP
	return value;
}

#else//if defined (USE_8BIT_PMP)


extern inline BYTE __attribute__ ((always_inline)) DeviceRead(){
BYTE value;
	value = PMDIN1;
	PMPWaitBusy();
	PMCONbits.PMPEN = 0; // disable PMP
	value = PMDIN1;
	PMCONbits.PMPEN = 1; // enable  PMP
	return value;
}

#endif


/*********************************************************************
* Macros:  SingleDeviceRead()
*
* PreCondition:  none
*
* Input: none
*
* Output: data read
*
* Side Effects: none
*
* Overview: a single read is performed. This is useful in issuing 
*			one read acess only.
*
* Note: chip select should be enabled
*
********************************************************************/
#if defined (USE_16BIT_PMP)
extern inline WORD __attribute__ ((always_inline)) SingleDeviceRead()
{
WORD value;
	value = PMDIN1;
	PMPWaitBusy();
	return value;
}

#else//if defined (USE_8BIT_PMP)
extern inline BYTE __attribute__ ((always_inline)) SingleDeviceRead()
{
BYTE value;
	value = PMDIN1;
	PMPWaitBusy();
	return value;
}

#endif

/*********************************************************************
* Macros:  DeviceReadWord()
*
* PreCondition:  none
*
* Input: none
*
* Output: data read
*
* Side Effects: none
*
* Overview: Reads a word from the device. Depending on the interface
*			(byte or word) it will always return a word.
*
* Note: chip select should be enabled
*
********************************************************************/
#if defined (USE_16BIT_PMP)
extern inline WORD __attribute__ ((always_inline)) DeviceReadWord()
{
WORD value;
	value = PMDIN1;
	PMPWaitBusy();
	return value;
}

#elif defined (USE_8BIT_PMP)
extern inline WORD __attribute__ ((always_inline)) DeviceReadWord()
{
WORD value;
BYTE temp;
	value = PMDIN1;
	value = value << 8;
	PMPWaitBusy();
	temp = PMDIN1;
	value = value & temp;
	PMPWaitBusy();
	return value;
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
    // variable for PMP timing calculation
	// GetPeripheralClock() is in Mhz. pClockPeriod will be in nanoseconds.
    DWORD pClockPeriod = (1000000000ul) / GetPeripheralClock();

	DisplayResetEnable();               // hold in reset by default
    DisplayResetConfig();               // enable RESET line
    DisplayCmdDataConfig();             // enable RS line
    DisplayDisable();                   // not selected by default
    DisplayConfig();                    // enable chip select line
    DisplayBacklightOff();              // initially set the backlight to off
    DisplayBacklightConfig();           // set the backlight control pin

    // PMP setup
    PMMODE = 0;
    PMAEN = 0;
    PMCON = 0;
    PMMODEbits.MODE = 2;                // Intel 80 master interface

    #if (PMP_DATA_SETUP_TIME == 0)
        PMMODEbits.WAITB = 0;
    #else    
        if (PMP_DATA_SETUP_TIME <= pClockPeriod)
            PMMODEbits.WAITB = 0;
        else if (PMP_DATA_SETUP_TIME > pClockPeriod)
            PMMODEbits.WAITB = (PMP_DATA_SETUP_TIME / pClockPeriod) + 1;
    #endif
    
    #if (PMP_DATA_WAIT_TIME == 0)
        PMMODEbits.WAITM = 0;
    #else    
        if (PMP_DATA_WAIT_TIME <= pClockPeriod)
            PMMODEbits.WAITM = 1;
        else if (PMP_DATA_WAIT_TIME > pClockPeriod)
            PMMODEbits.WAITM = (PMP_DATA_WAIT_TIME / pClockPeriod) + 1;
    #endif
    
    #if (PMP_DATA_HOLD_TIME == 0)
        PMMODEbits.WAITE = 0;
    #else
        if (PMP_DATA_HOLD_TIME <= pClockPeriod)
            PMMODEbits.WAITE = 0;
        else if (PMP_DATA_HOLD_TIME > pClockPeriod)
            PMMODEbits.WAITE = (PMP_DATA_HOLD_TIME / pClockPeriod) + 1;
    #endif

    #if defined(USE_16BIT_PMP)
    PMMODEbits.MODE16 = 1;              // 16 bit mode
    #elif defined(USE_8BIT_PMP)
    PMMODEbits.MODE16 = 0;              // 8 bit mode
    #endif
  
    PMCONbits.PTRDEN = 1;               // enable RD line
    PMCONbits.PTWREN = 1;               // enable WR line
    PMCONbits.PMPEN = 1;                // enable PMP

    DisplayResetDisable();              // release from reset
	
	// hard delay inserted here for devices that needs delays after reset.
	// Value will vary from device to device, please refer to the specific 
	// device data sheet for details.
    Delay10us(20);
}
#endif //#ifdef USE_GFX_PMP
#endif //#ifndef _GFX_PMP_H_FILE
