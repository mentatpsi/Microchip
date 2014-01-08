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
*
* Author        Date        Comment
*************************************************************************
* T. Lawrence  2011/01/24  Initial code ported from AN1310.
************************************************************************/

#ifndef COMM_H
#define COMM_H

#include <QThread>
#include <QTimer>

#include "../HIDAPI/hidapi.h"
#include "Device.h"

// Device Vendor and Product IDs
#define VID 0x04d8
#define PID 0x003C


#define USB_PACKET_SIZE 64
#define USB_PACKET_SIZE_WITH_REPORT_ID (USB_PACKET_SIZE + 1)


// Packet commands
#define QUERY_DEVICE        0x02
#define UNLOCK_CONFIG       0x03
#define ERASE_DEVICE        0x04
#define PROGRAM_DEVICE      0x05
#define PROGRAM_COMPLETE    0x06
#define GET_DATA            0x07
#define RESET_DEVICE        0x08
#define SIGN_FLASH			0x09	//The host PC application should send this command after the verify operation has completed successfully.  If checksums are used instead of a true verify (due to ALLOW_GET_DATA_COMMAND being commented), then the host PC application should send SIGN_FLASH command after is has verified the checksums are as exected. The firmware will then program the SIGNATURE_WORD into flash at the SIGNATURE_ADDRESS.
#define QUERY_EXTENDED_INFO 0x0C    //Used by host PC app to get additional info about the device, beyond the basic NVM layout provided by the query device command

// Maximum number of memory regions that can be bootloaded
#define MAX_DATA_REGIONS    0x06


#define MAX_ERASE_BLOCK_SIZE 8196   //Increase this in the future if any microcontrollers with bigger than 8196 byte erase block is implemented


/*!
 * Provides low level HID bootloader communication.
 */
class Comm : public QObject
{
    Q_OBJECT

signals:
    void SetProgressBar(int newValue);


protected:
    hid_device *boot_device;
    bool connected;

public:

    explicit Comm();
    ~Comm();

    static const int SyncWaitTime;

    enum ErrorCode
    {
        Success = 0, NotConnected, Fail, IncorrectCommand, Timeout, Other = 0xFF
    };

    QString ErrorString(ErrorCode errorCode) const;

    #pragma pack(1)
    struct MemoryRegion
    {
        unsigned char type;
        unsigned long int address;
        unsigned long int size;
    };

    struct BootInfo
    {
        unsigned char command;
        unsigned char bytesPerPacket;
        unsigned char deviceFamily;
        MemoryRegion memoryRegions[MAX_DATA_REGIONS];
        unsigned char versionFlag;
        unsigned char pad[7];
    };

    //Structure for the response to the QUERY_EXTENDED_INFO command
    union ExtendedQueryInfo
    {
        unsigned char command;
        struct
        {
            unsigned char command;
            unsigned short int bootloaderVersion;
            unsigned short int applicationVersion;
            unsigned long signatureAddress;
            unsigned short int signatureValue;
            unsigned long erasePageSize;
            unsigned char config1LMask;
            unsigned char config1HMask;
            unsigned char config2LMask;
            unsigned char config2HMask;
            unsigned char config3LMask;
            unsigned char config3HMask;
            unsigned char config4LMask;
            unsigned char config4HMask;
            unsigned char config5LMask;
            unsigned char config5HMask;
            unsigned char config6LMask;
            unsigned char config6HMask;
            unsigned char config7LMask;
            unsigned char config7HMask;
            unsigned char pad[USB_PACKET_SIZE_WITH_REPORT_ID - 29];
        }PIC18;
        struct
        {
            unsigned char command;
            unsigned short int bootloaderVersion;
            unsigned short int applicationVersion;
            unsigned long signatureAddress;
            unsigned short int signatureValue;
            unsigned long erasePageSize;
            #warning Replace with real stuff when implemented.
            //unsigned short int configxxMask...
            //unsigned char pad[USB_PACKET_SIZE_WITH_REPORT_ID - XX];
        }PIC24;
    };



    struct WritePacket
    {
        unsigned char report;
        unsigned char command;
        union {
            unsigned long int address;
            unsigned char LockedValue;
        };
        unsigned char bytesPerPacket;
        unsigned char data[58];
    };
    struct ReadPacket
    {
        unsigned char command;
        unsigned long int address;
        unsigned char bytesPerPacket;
        unsigned char data[59];
    };
    #pragma pack()

    void PollUSB(void);

    ErrorCode open(void);

    void close(void);
    bool isConnected(void);
    void Reset(void);

    ErrorCode GetData(unsigned long address, unsigned char bytesPerPacket, unsigned char bytesPerAddress,
                      unsigned char bytesPerWord, unsigned long endAddress, unsigned char *data);
    ErrorCode Program(unsigned long address, unsigned char bytesPerPacket, unsigned char bytesPerAddress,
                      unsigned char bytesPerWord, unsigned char deviceFamily, unsigned long endAddress, unsigned char *data);
    ErrorCode Erase(void);
    ErrorCode LockUnlockConfig(bool lock);
    ErrorCode ReadBootloaderInfo(BootInfo* bootInfo);
    ErrorCode ReadExtendedQueryInfo(ExtendedQueryInfo* extendedBootInfo);
    ErrorCode SignFlash(void);
    ErrorCode SendPacket(unsigned char *data, int size);
    ErrorCode ReceivePacket(unsigned char *data, int size);
};

#endif // COMM_H
