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
 ********************************************************************/

#ifndef PICDEM_FS_DEMO_H
#define PICDEM_FS_DEMO_H

/** I N C L U D E S **********************************************************/
#include "GenericTypeDefs.h"
#include "usb_config.h"

extern volatile unsigned char usbgen_out[USBGEN_EP_SIZE];
extern volatile unsigned char usbgen_in[USBGEN_EP_SIZE];

/** D E F I N I T I O N S ****************************************************/
/* PICDEM FS USB Demo Version */
#define MINOR_VERSION   0x00    //Demo Version 1.00
#define MAJOR_VERSION   0x01

/* Temperature Mode */
#define TEMP_REAL_TIME  0x00
#define TEMP_LOGGING    0x01

typedef enum
{
    READ_VERSION    = 0x00,
    /*
    READ_FLASH      = 0x01,
    WRITE_FLASH     = 0x02,
    ERASE_FLASH     = 0x03,
    READ_EEDATA     = 0x04,
    WRITE_EEDATA    = 0x05,
    READ_CONFIG     = 0x06,
    WRITE_CONFIG    = 0x07,
    */
    ID_BOARD        = 0x31,
    UPDATE_LED      = 0x32,
    SET_TEMP_REAL   = 0x33,
    RD_TEMP         = 0x34,
    SET_TEMP_LOGGING= 0x35,
    RD_TEMP_LOGGING = 0x36,
    RD_POT          = 0x37,
    RESET           = 0xFF
}TYPE_CMD;

/** S T R U C T U R E S ******************************************************/
typedef union DATA_PACKET
{
    BYTE _byte[USBGEN_EP_SIZE];  //For byte access
    WORD _word[USBGEN_EP_SIZE/2];//For word access(USBGEN_EP_SIZE msut be even)
    struct
    {
        BYTE CMD;
        BYTE len;
    };
    struct
    {
        unsigned :8;
        BYTE ID;
    };
    struct
    {
        unsigned :8;
        BYTE led_num;
        BYTE led_status;
    };
    struct
    {
        unsigned :8;
        WORD word_data;
    };
} DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/
void UserInit(void);
void ProcessIO(void);

#endif //PICDEM_FS_DEMO_H
