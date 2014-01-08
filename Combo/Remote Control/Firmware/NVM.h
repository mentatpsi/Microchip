/********************************************************************
* FileName:		NVM.h
* Dependencies: none
* Processor:	PIC18, PIC24F, PIC24H, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
*
* Copyright © 2007-2008 Microchip Technology Inc.  All rights reserved.
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
* SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
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
*  Defines interface to access EUI EEPROM
*
* Change History:
*  Rev          Date         Author    Description
*  2.0          4/15/2009    yfy       MiMAC and MiApp revision
*  1.0-1.1.2    12/10/2009   yfy       RF4CE modification
********************************************************************/

#include "ConfigApp.h"
#include "SystemProfile.h"
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "WirelessTransceivers/RF4CE.h"

#ifndef RF4CE_NVM_HEADER
#define RF4CE_NVM_HEADER

#include "SST25VF016.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"

#define RF4CE_PANID_OFFSET                  RF4CE_NVM_START_ADDR
#define RF4CE_PANID_SIZE                    2
#define RF4CE_DEFAULT_CHANNEL_OFFSET        RF4CE_PANID_OFFSET + RF4CE_PANID_SIZE
#define RF4CE_DEFAULT_CHANNEL_SIZE          2
#define RF4CE_PAIR_TABLE_OFFSET             RF4CE_DEFAULT_CHANNEL_OFFSET + RF4CE_DEFAULT_CHANNEL_SIZE
#define RF4CE_PAIR_TABLE_SIZE               MAX_PAIR_SIZE * sizeof(PAIR_TABLE_ENTRY)
#define RF4CE_SHORT_ADDR_OFFSET             RF4CE_PAIR_TABLE_OFFSET + RF4CE_PAIR_TABLE_SIZE
#define RF4CE_SHORT_ADDR_SIZE               2
#define RF4CE_FRAME_COUNTER_OFFSET          RF4CE_SHORT_ADDR_OFFSET + RF4CE_SHORT_ADDR_SIZE
#define RF4CE_FRAME_COUNTER_SIZE            4
#define RF4CE_POWER_SAVE_OFFSET             RF4CE_FRAME_COUNTER_OFFSET + RF4CE_FRAME_COUNTER_SIZE
#define RF4CE_POWER_SAVE_SIZE               1


#define NVMInit()                           {                                           \
                                                extern DRV_SPI_INIT_DATA SPI_Init_Data; \
                                                SST25Init(&SPI_Init_Data);              \
                                            }
#define NVMClearMemory()                    SST25SectorErase(RF4CE_NVM_START_ADDR)

#define nvmGetMyPANID( x )                  SST25ReadArray( RF4CE_PANID_OFFSET, (BYTE *)x, sizeof(WORD_VAL))
#define nvmPutMyPANID( x )                  SST25WriteArray(RF4CE_PANID_OFFSET, (BYTE *)x, sizeof(WORD_VAL))

#define nvmGetMyDefaultChannel( x )         SST25ReadArray( RF4CE_DEFAULT_CHANNEL_OFFSET, (BYTE *)x, 1)
#define nvmPutMyDefaultChannel( x )         SST25WriteArray(RF4CE_DEFAULT_CHANNEL_OFFSET, (BYTE *)x, 1)

#define nvmGetPairTable( x )                SST25ReadArray( RF4CE_PAIR_TABLE_OFFSET, (BYTE *)x, (WORD)MAX_PAIR_SIZE * sizeof(PAIR_TABLE_ENTRY))
#define nvmPutPairTable( x )                SST25WriteArray(RF4CE_PAIR_TABLE_OFFSET, (BYTE *)x, (WORD)MAX_PAIR_SIZE * sizeof(PAIR_TABLE_ENTRY))
#define nvmPutPairTableIndex(x, y)          SST25WriteArray(RF4CE_PAIR_TABLE_OFFSET + ((WORD)y * sizeof(PAIR_TABLE_ENTRY)), (BYTE *)x, sizeof(PAIR_TABLE_ENTRY)) 

#define nvmGetMyShortAddress( x )           SST25ReadArray( RF4CE_SHORT_ADDR_OFFSET, (BYTE *)x, sizeof(WORD_VAL))
#define nvmPutMyShortAddress( x )           SST25WriteArray(RF4CE_SHORT_ADDR_OFFSET, (BYTE *)x, sizeof(WORD_VAL))

#define nvmGetFrameCounter( x )             SST25ReadArray( RF4CE_FRAME_COUNTER_OFFSET, (BYTE *)x, 4)
#define nvmPutFrameCounter( x )             SST25WriteArray(RF4CE_FRAME_COUNTER_OFFSET, (BYTE *)x, 4)

#define nvmGetPowerSaveMode( x )            SST25ReadArray( RF4CE_POWER_SAVE_OFFSET, (BYTE *)x, 1)
#define nvmPutPowerSaveMode( x )            SST25WriteArray(RF4CE_POWER_SAVE_OFFSET, (BYTE *)x, 1)

#endif    
    


