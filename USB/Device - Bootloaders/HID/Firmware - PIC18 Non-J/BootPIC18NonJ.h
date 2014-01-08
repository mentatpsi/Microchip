/*********************************************************************
 *
 *   Microchip USB HID Bootloader v1.01 for PIC18F and PIC18LF versions of:
 *	 PIC18F4553/4458/2553/2458
 *	 PIC18F4550/4455/2550/2455
 *	 PIC18F4450/2450
 *	 PIC18F14K50/13K50
 *
 *********************************************************************
 * FileName:        BootPIC18NonJ.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") for its PIC(R) Microcontroller is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PIC Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * File version     Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 1.0				06/03/2008	Original
 * 2.9j             06/11/13    Added some definitions. 
 ********************************************************************/
#ifndef BOOTPIC18NONJ_H
#define BOOTPIC18NONJ_H

/** P U B L I C  P R O T O T Y P E S *****************************************/
void UserInit(void);
void ProcessIO(void);
void ClearWatchdog(void);
void DisableUSBandExecuteLongDelay(void);



#define APP_SIGNATURE_ADDRESS            0x1006  //0x1006 and 0x1007 contains the "signature" WORD, indicating successful erase/program/verify operation
#define APP_SIGNATURE_VALUE              0x600D  //leet "GOOD", implying that the erase/program was a success and the bootloader intentionally programmed the APP_SIGNATURE_ADDRESS with this value
#define APP_VERSION_ADDRESS              0x1016  //0x1016 and 0x1017 should contain the application image firmware version number



#endif //BOOTPIC18NONJ_H
