/*********************************************************************
 *
 *	Hardware specific definitions
 *
 *********************************************************************
 * FileName:        HardwareProfile.h
 * Dependencies:    None
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.10 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.34 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2010 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
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
 * Author               Date		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder		09/01/2010	Converted to a stub
 ********************************************************************/

// These definitions are set in the MPLAB Project settings.  If you are starting
// a new project, you should start by modifying one of the pre-existing .mcp 
// files.  To modify the macro used, in MPLAB IDE, click on Project -> Build 
// Options... -> Project -> MPLAB XXX C Compiler -> Preprocessor Macros -> 
// Add.... Note that you may also have to add this macro to the assembler 
// (MPLAB XXX Assembler tab).
#if defined(YOUR_BOARD)

	#include "Configs/HWP YOUR_BOARD.h"

#else
    // Include defines for Graphics library
#if defined(CFG_INCLUDE_DA210_BRD_16PMP_QVGAv1)
    // Truly 3.2" 320x240 display
    #include "Configs/HWP_DA210_BRD_16PMP_QVGAv1.h"
#elif defined(CFG_INCLUDE_DA210_BRD_16PMP_WQVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_WQVGAv1.h"
#elif defined(__PIC32MX__)
    // Graphics Hardware Configurations
    #include "Configs/HWP_MEB_PIC32_ETH_SK_16PMP.h"
#else
    // Truly 3.2" 320x240 display
    #include "Configs/HWP_DA210_BRD_16PMP_QVGAv1.h"
    // Powertip 4.3" 480x272 display
    //#include "Configs/HWP_DA210_BRD_16PMP_WQVGAv1.h"
#endif

#if defined(CFG_INCLUDE_DA210_BRD_ENC28)
    // DM240312 PIC24FJ256DA210 Development Board, PIC24FJ256DA210, AC164123 Ethernet PICtail Plus (ENC28J60)
    #include "Configs/HWP DA210_BRD_ENC28.h"
#elif defined(CFG_INCLUDE_DA210_BRD_ENC624)
    // DM240312 PIC24FJ256DA210 Development Board, PIC24FJ256DA210, AC164132 Fast 100Mbps Ethernet PICtail/PICtail Plus (ENC624J600) in SPI mode (using J2 side edge connector)
    #include "Configs/HWP DA210_BRD_ENC624.h"
#elif defined(CFG_INCLUDE_PIC32_ETH_SK_ETH795)
    // DM320004 PIC32 Ethernet Starter Kit, PIC32MX795F512L, internal Ethernet (National DP83848 PHY)
    #include "Configs/HWP PIC32_ETH_SK_ETH795.h"
#endif
#endif

