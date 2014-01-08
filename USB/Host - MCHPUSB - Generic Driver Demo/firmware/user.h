/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        user.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original.
 * Bud Caldwell         3/9/07      Modified for PIC32
 ********************************************************************/

#ifndef _USER_H_
#define _USER_H_

/** D E F I N I T I O N S ****************************************************/
/* PICDEM FS USB Demo Version */
#define DEMO_FW_MINOR_VERSION   0x00    //Demo Version 1.00
#define DEMO_FW_MAJOR_VERSION   0x01

#define DATA_PACKET_LENGTH  8

#define POT_FULL_VALUE   10000

/* Commands */
#define READ_VERSION     0x00
#define CMD_INVALID      0xAA
#define ID_BOARD         0x31
#define UPDATE_LED       0x32
#define SET_TEMP_REAL    0x33
#define RD_TEMP          0x34
#define SET_TEMP_LOGGING 0x35
#define RD_TEMP_LOGGING  0x36
#define RD_POT           0x37
#define RESET            0xFF

#define SW3_STATE        0x08
#define SW6_STATE        0x40

/** S T R U C T U R E S ******************************************************/
typedef union DATA_PACKET
{
    BYTE _byte[DATA_PACKET_LENGTH];  //For byte access
    WORD _word[DATA_PACKET_LENGTH/2];//For word access(DATA_PACKET_LENGTH must be even)
    struct
    {
        BYTE CMD;
        BYTE len;
    };
    struct
    {
        BYTE res1;
        BYTE ID;
    };
    struct
    {
        BYTE res2;
        BYTE led_num;
        BYTE led_status;
    };
    struct
    {
        BYTE res3;
        WORD word_data;
    };
} DATA_PACKET;

#endif //_USER_H_
