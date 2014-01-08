/************************************************************************
* Copyright (c) 2009-2010,  Microchip Technology Inc.
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

#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QVariant>
#include <QLinkedList>
#include <QList>

#include "DeviceData.h"

/*!
 * Provides microcontroller device specific parameters, address calculations, and
 * assembly code tools.
 */
class Device
{
public:
    enum Families
    {
        Unknown = 0,
        PIC18 = 0x01,
        PIC24 = 0x02,
        PIC32 = 0x03,
        PIC16 = 0x04
    };

    Device(DeviceData* data);

    void setUnknown(void);

    Families family;

    unsigned int bytesPerPacket;
    unsigned int bytesPerWordFLASH;
    unsigned int bytesPerWordEEPROM;
    unsigned int bytesPerWordConfig;

    unsigned int bytesPerAddressFLASH;
    unsigned int bytesPerAddressEEPROM;
    unsigned int bytesPerAddressConfig;

    bool hasEeprom(void);
    bool hasConfig(void);

    bool hasConfigAsFlash(void);
    bool hasConfigAsFuses(void);

    unsigned int GetDeviceAddressFromHexAddress(unsigned int hexAddress, DeviceData* pData, unsigned char& type, bool& includedInProgrammableRange, bool& addressWasEndofRange, unsigned int& bytesPerAddressAndType, unsigned int& endDeviceAddressofRegion, unsigned char*& pPCRAMBuffer);

protected:
    DeviceData *deviceData;
};

#endif // DEVICE_H
