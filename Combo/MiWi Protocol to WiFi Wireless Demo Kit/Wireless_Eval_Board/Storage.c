/*********************************************************************
 *
 *  Application to Demo HTTP2 Server
 *  Support for HTTP2 module in Microchip TCP/IP Stack
 *	 -Implements the application
 *	 -Reference: RFC 1002
 *
 *********************************************************************
 * FileName:        CustomHTTPApp.c
 * Dependencies:    TCP/IP stack
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Chris Smith       	11/16/11	    Original
 ********************************************************************/
#include "Storage.h"
#include "XEEPROM.h"


BOOL GetRoomInfo(ROOM_RECORD *roomInfo, BYTE index)
{
    BYTE *buffer;
    DWORD address;

    buffer = (BYTE *)roomInfo;
    
    address = (ROOM_RECORD_SIZE * index) + ROOM_RECORD_BEGIN;

    if(XEEReadArray(address, buffer, ROOM_RECORD_SIZE))
        return 1;
    else
        return 0;

}


BOOL AddRoomInfo(ROOM_RECORD *roomInfo)
{
    BYTE *buffer;
    BYTE cnt;
    BYTE type;
    BYTE res;
    DWORD address;
    ROOM_RECORD currInfo;

    cnt = 0;
    res = 0;
    do{
        // Check current record index flags
        GetRoomInfo(&currInfo, cnt);
        type = currInfo.recType;
        if(type == BLANK_REC)
        {
                roomInfo->recType = VALID_REC;
                buffer = (BYTE *)roomInfo;
                address = (ROOM_RECORD_SIZE * cnt) + ROOM_RECORD_BEGIN;
                XEEBeginWrite(address);
                XEEWriteArray(buffer, ROOM_RECORD_SIZE);
                XEEEndWrite();
                res = 1;
        }
        else
        {
            cnt++;
        }
    }while(res == 0 && cnt < MAX_NUM_ROOMS);

    return res;

}


BOOL RemoveRoomInfo(BYTE index)
{
    DWORD address;

    address = (ROOM_RECORD_SIZE * index) + ROOM_RECORD_BEGIN;

    XEEBeginWrite(address);

    XEEWrite(BLANK_REC);

    XEEEndWrite();

    return 1;
}

void EraseRoomInfo(void)
{
    BYTE *buffer;
    DWORD address;
    ROOM_RECORD roomInfo;
    int index;

    for(index = 0; index < MAX_NUM_ROOMS; index++)
    {
        roomInfo.recType = 0xFF;
        buffer = (BYTE *)&roomInfo;
        address = (ROOM_RECORD_SIZE * index) + ROOM_RECORD_BEGIN;
        XEEBeginWrite(address);
        XEEWriteArray(buffer, ROOM_RECORD_SIZE);
        XEEEndWrite();
    }
}
