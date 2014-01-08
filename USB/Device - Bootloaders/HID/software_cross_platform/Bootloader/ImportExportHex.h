/************************************************************************
* Copyright (c) 2005-2009,  Microchip Technology Inc.
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
#ifndef IMPORTEXPORTHEX_H
#define IMPORTEXPORTHEX_H

#include <QString>
#include "DeviceData.h"
#include "Device.h"

/*!
 * Reads HEX files into an in-memory DeviceData object.
 */
class HexImporter
{
public:
    enum ErrorCode { Success = 0, CouldNotOpenFile, NoneInRange, ErrorInHexFile, InsufficientMemory };
    //Definitions for the record type bytes in Intel 32-bit .HEX file formatted firmware images
    enum HEX32_RECORD
    {
        DATA = 0x00,
        END_OF_FILE = 0x01,
        EXTENDED_SEGMENT_ADDR = 0x02,
        EXTENDED_LINEAR_ADDR = 0x04,
    };

    //Definitions for the program memory "word write" size for different microcontroller families
    enum PROG_WORD_WRITE_SIZE
    {
        PIC18_PROG_WORD_WRITE_SIZE = 0x02,  //Word writes perform two byte writes on these devices.
        PIC24_PROG_WORD_WRITE_SIZE = 0x04,  //3 useful bytes per "word write", but the empty phantom byte makes 4 bytes total
        PIC32_PROG_WORD_WRITE_SIZE = 0x04,
    };



    HexImporter(void);
    ~HexImporter(void);

    ErrorCode ImportHexFile(QString fileName, DeviceData* data, Device* device);

    bool hasEndOfFileRecord;    // hex file does have an end of file record
    bool hasConfigBits;         // hex file has config bit settings
    bool fileExceedsFlash;      // hex file records exceed device memory constraints

    QList<DeviceData::MemoryRange> rawimport;


protected:
    //int ParseHex(char* characters, int length);
    //unsigned char computeChecksum(char* fileLine);


};

#endif // IMPORTEXPORTHEX_H
