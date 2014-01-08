/************************************************************************
* Copyright (c) 2009-2011,  Microchip Technology Inc.
*
* Microchip licenses this software to you solely for use with Microchip
* products.  The software is owned by Microchip and its licensors, and
* is protected under applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
* MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
* EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
* OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
* TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
* OR OTHER SIMILAR COSTS.
*
* To the fullest extent allowed by law, Microchip and its licensors
* liability shall not exceed the amount of fees, if any, that you
* have paid directly to Microchip to use this software.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*/
#ifndef DEVICEDATA_H
#define DEVICEDATA_H

// Types of memory regions
#define PROGRAM_MEMORY      0x01
#define EEPROM_MEMORY       0x02
#define CONFIG_MEMORY       0x03
#define USERID_MEMORY       0x04
#define END_OF_TYPES_LIST   0xFF
#define BOOTLOADER_V1_01_OR_NEWER_FLAG   0xA5   //Tacked on in region Type6 byte, to indicate when using newer version of bootloader with extended query info available


#include <QVector>


/*!
 * Provides in-memory, PC representation of microcontroller device memory contents.
 */
class DeviceData
{
    public:
        DeviceData();
        ~DeviceData();

        struct MemoryRange
        {
            unsigned char type;
            unsigned int start;
            unsigned int end;
            unsigned int dataBufferLength;
            unsigned char* pDataBuffer;
        };

        QList<DeviceData::MemoryRange> ranges;
};

#endif // DEVICEDATA_H
