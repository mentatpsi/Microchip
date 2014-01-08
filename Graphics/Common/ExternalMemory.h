/*********************************************************************
 * GRAPHICS PICTail PLUS BOARD MEMORY PROGRAMMER
 * This program receives Intel HEX file from UART and programs
 * flash memory installed on Graphics PICTail board.
 *
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        FlashProgrammer.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
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
 *****************************************************************************/

/*****************************************************************************
 * Section: Description
 *
 * This module is used to program an external memory source.  The data is sent
 * via the PC utility external_memory_programmer.  Using the comm_pkt protocol, 
 * the data is parsed and programed to the external memory.  
 *
 * This module is used by serval different Graphics demos and is part of the 
 * common directory under the Graphics demo.
 *****************************************************************************/

#ifndef _EXTERNAL_MEMORY_H
#define _EXTERNAL_MEMORY_H

/*****************************************************************************
 * Section: Includes
 *****************************************************************************/
#include "Compiler.h"
#include "HardwareProfile.h"
#include "SST39VF040.h" // memory on Graphics PICTail Board 2
#include "SST25VF016.h" // memory on Graphics PICTail Board 3
#include "SST39LF400.h" // parallel flash memory on PIC24FJ256DA210 Development Board
    

/*****************************************************************************
 * Section: Macros
 *****************************************************************************/
#if defined (USE_SST25VF016) || defined (USE_SST25VF064)
    #define FLASHInit(initData)             SST25Init(initData);
	#define ChipErase()                     SST25ChipErase();
	#define WriteArray(address, pdata, len) SST25WriteArray(address, pdata, len)
    #define ReadArray(address, pdata, len)  SST25ReadArray(address, pdata, len)
#endif	

#if defined (USE_SST39LF400)
	// parallel flash
    #define FLASHInit()             		SST39LF400Init(NULL);
    #define ChipErase()                     SST39LF400ChipErase();
    #define WriteArray(address, pdata, len) SST39LF400WriteArray(address, pdata, len)
    #define ReadArray(address, pdata, len)  SST39LF400ReadArray(address, pdata, len)
#endif    

    
#endif 
