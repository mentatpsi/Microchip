/********************************************************************
* FileName:	MSPI.h
* Dependencies: none
* Processor:	PIC32
* Hardware:
* Complier:     Microchip C32 v1.02 or higher
* Company:	Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
*
* Copyright � 2007-2011 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute
* Software only when embedded on a Microchip microcontroller or digital
* signal controller and used with a Microchip radio frequency transceiver,
* which are integrated into your product or third party product (pursuant
* to the terms in the accompanying license agreement).
*
* You should refer to the license agreement accompanying this Software for
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY,
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES,
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS,
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
*********************************************************************
* File Description:
*
*  This file provides the interface to the SPI
*
* Change History:
*  Rev   Date         Description
*  0.1   11/09/2011   Initial revision
********************************************************************/

#ifndef _MIWI_SPI_H_
#define _MIWI_SPI_H_

/************************ HEADERS **********************************/

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include <plib.h>

/************************ FUNCTION PROTOTYPES **********************/
void SPIPut(BYTE v);
BYTE SPIGet(void);

void SPIPut2(BYTE v);
BYTE SPIGet2(void);

/************************ MACROS ***********************************/
#define SPIInit()   SSPIF = 1

#endif
