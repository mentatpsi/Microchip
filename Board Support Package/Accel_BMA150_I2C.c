/*******************************************************************************
  Accelerometer Library Interface Definition

  Summary:
    This file contains the interface definition for the Accelerometer library.
    
  Description:
    This library provides a low-level abstraction of the Accelerometer device.  It 
    can be used to simplify low-level access to the device without the necessity of 
    interacting directly with the communication module's registers, thus hiding differences from 
    one serial device variant to another.
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
FileName:       Accelerometer.h
Dependencies:   See includes
Processor:      PIC32MX

Complier:       Microchip MPLAB C32 v1.06 or higher
Company:        Microchip Technology Inc.

Copyright © 2008-2009 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

/********************************************************************
 Section: Includes 
********************************************************************/
#include <Accelerometer.h>
#include <plib.h>
#include "ACCEL_BMA150Config.h"
#include <stdint.h>

#if defined (USE_ACCELEROMETER_BMA150)

/********************************************************************
 Section: Communication Interface 
********************************************************************/
#ifdef ACCEL_BMA150_USE_I2C1

#define ACCEL_BMA150_I2C_MODULE           (I2C1)
#define ACCEL_BMA150_I2C_VECTOR_LOCATION  _I2C_1_VECTOR
#define ACCEL_BMA150_I2C_SOURCE           (INT_I2C1M)
#define ACCEL_BMA150_I2C_VECTOR           (INT_I2C_1_VECTOR)

#elif defined (ACCEL_BMA150_USE_I2C2)

#define ACCEL_BMA150_I2C_MODULE           (I2C2)
#define ACCEL_BMA150_I2C_VECTOR_LOCATION  _I2C_2_VECTOR
#define ACCEL_BMA150_I2C_SOURCE           (INT_I2C2M)
#define ACCEL_BMA150_I2C_VECTOR           (INT_I2C_2_VECTOR)

#elif defined (ACCEL_BMA150_USE_I2C1A)

#define ACCEL_BMA150_I2C_MODULE           (I2C1A)
#define ACCEL_BMA150_I2C_VECTOR_LOCATION  _I2C_1A_VECTOR
#define ACCEL_BMA150_I2C_SOURCE           (INT_I2C1AM)
#define ACCEL_BMA150_I2C_VECTOR           (INT_I2C_1A_VECTOR)

#elif defined (ACCEL_BMA150_USE_I2C2A)

#define ACCEL_BMA150_I2C_MODULE           (I2C2A)
#define ACCEL_BMA150_I2C_VECTOR_LOCATION  _I2C_2A_VECTOR
#define ACCEL_BMA150_I2C_SOURCE           (INT_I2C2AM)
#define ACCEL_BMA150_I2C_VECTOR           (INT_I2C_2A_VECTOR)

#elif defined (ACCEL_BMA150_USE_I2C3A)

#define ACCEL_BMA150_I2C_MODULE           (I2C3A)
#define ACCEL_BMA150_I2C_VECTOR_LOCATION  _I2C_3A_VECTOR
#define ACCEL_BMA150_I2C_SOURCE           (INT_I2C3AM)
#define ACCEL_BMA150_I2C_VECTOR           (INT_I2C_3A_VECTOR)

#else

#error "Please define the communicaiton interface in ACCEL_BMA150Config.h"

#endif

/********************************************************************
 Section: Private Macros 
********************************************************************/
#define     ACCEL_BMA150_WRITE_ADDR     (0x70)
#define     ACCEL_BMA150_READ_ADDR      (0x71)
                                        
#define     ACCEL_BMA150_CHIP_ID_ADDR   (0x00)
#define     ACCEL_BMA150_VERSION_ADDR   (0x01)
#define     ACCEL_BMA150_ACC_X_LSB_ADDR (0x02)
#define     ACCEL_BMA150_ACC_X_MSB_ADDR (0x03)
#define     ACCEL_BMA150_ACC_Y_LSB_ADDR (0x04)
#define     ACCEL_BMA150_ACC_Y_MSB_ADDR (0x05)
#define     ACCEL_BMA150_ACC_Z_LSB_ADDR (0x06)
#define     ACCEL_BMA150_ACC_Z_MSB_ADDR (0x07)
#define     ACCEL_BMA150_ADDR15         (0x15)
#define     ACCEL_BMA150_ADDR14         (0x14)
#define     ACCEL_BMA150_ADDR13         (0x13)
#define     ACCEL_BMA150_ADDR12         (0x12)
#define     ACCEL_BMA150_ADDR11         (0x11)
#define     ACCEL_BMA150_ADDR10         (0x10)
#define     ACCEL_BMA150_ADDR0F         (0x0F)
#define     ACCEL_BMA150_ADDR0E         (0x0E)
#define     ACCEL_BMA150_ADDR0D         (0x0D)
#define     ACCEL_BMA150_ADDR0C         (0x0C)
#define     ACCEL_BMA150_ADDR0B         (0x0B)
#define     ACCEL_BMA150_ADDR0A         (0x0A)
#define     ACCEL_BMA150_ADDR09         (0x09)

#define     ACCEL_BMA150_CHIP_ID        (0x02)
#define     ACCEL_BMA150_THES           (0x00)

#define     ACCEL_BMA150_RANGE_2G       (0)
#define     ACCEL_BMA150_RANGE_4G       (1)
#define     ACCEL_BMA150_RANGE_8G       (2)

#define     ACCEL_BMA150_BW_25          (0x00)
#define     ACCEL_BMA150_BW_50          (0x01)
#define     ACCEL_BMA150_BW_100         (0x02)
#define     ACCEL_BMA150_BW_190         (0x03)
#define     ACCEL_BMA150_BW_375         (0x04)
#define     ACCEL_BMA150_BW_750         (0x05)
#define     ACCEL_BMA150_BW_1500        (0x06)

#define     ACCEL_CMD_START             0x00
#define     ACCEL_CMD_RESTART           0x01
#define     ACCEL_CMD_STOP              0x02
#define     ACCEL_CMD_TX_BYTE           0x03 
#define     ACCEL_CMD_RX_BYTE           0x04
#define     ACCEL_CMD_ACK               0x05
#define     ACCEL_CMD_EN_RX             0x06
#define     ACCEL_CMD_ACK_POLL          0x07
#define     ACCEL_DONE                  0xFF
#define     ACCEL_INVALID_IDX           0xFFFFFFFF

#define     ACCEL_BMA150_AXIS_BIT       (10)
/********************************************************************
 Section: Private structures 
********************************************************************/
typedef struct
{
    uint8_t   data;
    uint8_t   cmd;
}ACCEL_CMD_DATA;

typedef union
{
    //Address 0x00
    struct
    {
        uint8_t chip_id :3;
        uint8_t :5;
    } ;

    //Address 0x01
    struct
    {
        uint8_t ml_version :4;
        uint8_t al_version :4;
    } ;

    //Address 0x02
    struct
    {
        uint8_t new_data_x :1;
        uint8_t :5;
        uint8_t acc_x :2;
    } ;

    //Address 0x03
    struct
    {
        uint8_t acc_x :8;
    } ;

    //Address 0x04
    struct
    {
        uint8_t new_data_y :1;
        uint8_t :5;
        uint8_t acc_y :2;
    } ;

    //Address 0x05
    struct
    {
        uint8_t acc_y :8;
    } ;

    //Address 0x06
    struct
    {
        uint8_t new_data_z :1;
        uint8_t :5;
        uint8_t acc_z :2;
    } ;

    //Address 0x07
    struct
    {
        uint8_t acc_z :8;
    } ;

    //Address 0x14
    struct
    {
        uint8_t bandwidth :3;  
        uint8_t range :2;
		uint8_t :3;
    } ;

    uint8_t val;
} ACCEL_BMA150_REG;

typedef struct
{
    ACCEL_BMA150_REG    acc_x_lsb;
    ACCEL_BMA150_REG    acc_x_msb;
    ACCEL_BMA150_REG    acc_y_lsb;
    ACCEL_BMA150_REG    acc_y_msb;
    ACCEL_BMA150_REG    acc_z_lsb;
    ACCEL_BMA150_REG    acc_z_msb;
}ACCEL_X_Y_Z_DATA;

typedef enum
{
    ACCEL_FALSE = 0,
    ACCEL_TRUE
}ACCEL_BOOL;
/********************************************************************
 Section: Gobal Structures 
********************************************************************/
static ACCEL_CMD_DATA       accelData[96];
static uint32_t             accelIdx = ACCEL_INVALID_IDX;
static uint8_t              *rxBuffer;
static ACCEL_X_Y_Z_DATA     accelXYZData;
/********************************************************************
 Section: Code 
********************************************************************/
/********************************************************************
 Funciton: __ISR(EEPROM_I2C_VECTOR_LOCATION, ipl2) ACCEL_BMA150InterruptHandler(void)
********************************************************************/
#if defined(ACCEL_USE_EXTERNAL_INTERUPT_HANDLER) || defined(ACCEL_USE_POLLING)
void ACCEL_BMA150InterruptHandler(void)
#else 
__ISR(ACCEL_BMA150_I2C_VECTOR_LOCATION, ipl2) ACCEL_BMA150InterruptHandler(void)
#endif
{
    ACCEL_BOOL polling;

    if(INTGetFlag(ACCEL_BMA150_I2C_SOURCE))
    {
        polling = ACCEL_FALSE;

        switch(accelData[accelIdx].cmd)
        {
        case ACCEL_CMD_START:
            I2CStart(ACCEL_BMA150_I2C_MODULE);
            break;

        case ACCEL_CMD_RESTART:
            I2CRepeatStart(ACCEL_BMA150_I2C_MODULE);
            break;

        case ACCEL_CMD_STOP:
            I2CStop(ACCEL_BMA150_I2C_MODULE);
            break;

        case ACCEL_CMD_TX_BYTE:
            I2CSendByte(ACCEL_BMA150_I2C_MODULE, accelData[accelIdx].data);
            break;

        case ACCEL_CMD_EN_RX:
            I2CReceiverEnable(ACCEL_BMA150_I2C_MODULE, ACCEL_TRUE);
            break;

        case ACCEL_CMD_RX_BYTE:
            *rxBuffer = I2CGetByte(ACCEL_BMA150_I2C_MODULE);
            rxBuffer++;
            break;

        case ACCEL_CMD_ACK:
            I2CAcknowledgeByte(ACCEL_BMA150_I2C_MODULE, accelData[accelIdx].data);
            break;

        case ACCEL_CMD_ACK_POLL:
            polling = ACCEL_TRUE;
            if(!I2CByteWasAcknowledged(ACCEL_BMA150_I2C_MODULE))
            {
                accelIdx -= 4;
            }
           break;
        
        default:
            accelData[accelIdx].cmd = ACCEL_DONE;
        case ACCEL_DONE:
#ifndef ACCEL_USE_EXTERNAL_INTERUPT_HANDLER
            INTEnable(ACCEL_BMA150_I2C_SOURCE, INT_DISABLED);
            break;
#else 
            accelIdx = ACCEL_INVALID_IDX;
            return;
#endif
        }

        INTClearFlag(ACCEL_BMA150_I2C_SOURCE);

        if(accelData[accelIdx].cmd != ACCEL_DONE)
        {
            if((accelData[accelIdx].cmd == ACCEL_CMD_RX_BYTE) || (polling))
            {
                INTSetFlag(ACCEL_BMA150_I2C_SOURCE);  
            }

            accelIdx++;
        }else
        {
            accelIdx = ACCEL_INVALID_IDX;
        }
    }

}
/*******************************************************************************
  Function:  inline ACCEL_RESULT IsBusy(void)
  *****************************************************************************/
inline int IsBusy(void)
{
    uint32_t  tempAccelIdx;
    uint32_t  int_status;

    int_status = INTDisableInterrupts();
    tempAccelIdx = accelIdx;
    INTRestoreInterrupts(int_status);

    return (tempAccelIdx != ACCEL_INVALID_IDX);

}
/*******************************************************************************
  Function:  inline uint8_t ReadByte(uint8_t start_idx, uint8_t addr)
  *****************************************************************************/
inline uint8_t ReadByteSequence(uint8_t start_idx, uint8_t addr)
{

    accelData[start_idx++].cmd = ACCEL_CMD_START;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = ACCEL_BMA150_WRITE_ADDR;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = addr;

    accelData[start_idx++].cmd = ACCEL_CMD_STOP;

    accelData[start_idx++].cmd = ACCEL_CMD_START;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = ACCEL_BMA150_READ_ADDR;

    accelData[start_idx++].cmd = ACCEL_CMD_EN_RX;

    accelData[start_idx++].cmd = ACCEL_CMD_RX_BYTE;

    accelData[start_idx++].cmd = ACCEL_CMD_STOP;

    return start_idx;
}
/*******************************************************************************
  Function:  inline uint8_t WriteByteSequence(uint8_t start_idx, uint8_t addr, uint8_t data)
  *****************************************************************************/
inline uint8_t WriteByteSequence(uint8_t start_idx, uint8_t addr, uint8_t data)
{
   accelData[start_idx++].cmd = ACCEL_CMD_START;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = ACCEL_BMA150_WRITE_ADDR;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = addr;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = data;

    accelData[start_idx++].cmd = ACCEL_CMD_STOP;

    return start_idx;
}
/********************************************************************
 Funciton: UINT8 EEPROM_24LC01HWReadByte(size_t addr)
********************************************************************/
inline void ACCELStartCommandSequence(void)
{
    uint32_t  int_status;

    int_status = INTDisableInterrupts();

    accelIdx = 0;

    if(!INTGetEnable(ACCEL_BMA150_I2C_SOURCE))
        INTSetFlag(ACCEL_BMA150_I2C_SOURCE);  
    
#ifndef ACCEL_USE_POLLING
    INTEnable(ACCEL_BMA150_I2C_SOURCE, INT_ENABLED);
#endif

    INTRestoreInterrupts(int_status);

}
/*******************************************************************************
  Function:  inline uint8_t WriteByteSequence(uint8_t start_idx, uint8_t addr, uint8_t data)
  *****************************************************************************/
inline ACCEL_RESULT ReadChipID(void)
{
    ACCEL_BMA150_REG    reg;
    uint8_t                idx;

    idx = ReadByteSequence(0, ACCEL_BMA150_CHIP_ID_ADDR);

    rxBuffer = (uint8_t *)&reg;

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

    if(reg.chip_id != ACCEL_BMA150_CHIP_ID)
        return ACCEL_INVALID;

    return ACCEL_VALID;
}
/*******************************************************************************
  Function:  inline void ReadChipVersion(void)
  *****************************************************************************/
inline void ReadChipVersion(void)
{
    ACCEL_BMA150_REG    reg;
    uint8_t                idx;

    idx = ReadByteSequence(0, ACCEL_BMA150_VERSION_ADDR);

    rxBuffer = (uint8_t *)&reg;

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

}

/*******************************************************************************
  Function:  inline void SetRangeAndBandwidth(void)
  *****************************************************************************/
inline void SetRangeAndBandwidth(void)
{
    ACCEL_BMA150_REG    reg;
    uint8_t                idx;

    idx = ReadByteSequence(0, ACCEL_BMA150_ADDR14);

    rxBuffer = (uint8_t *)&reg;

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

    reg.range       = ACCEL_BMA150_RANGE_2G;
	reg.bandwidth   = ACCEL_BMA150_BW_50;

    idx = WriteByteSequence(0, ACCEL_BMA150_ADDR14, reg.val);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif
}
/*******************************************************************************
  Function:  inline void SetThershold(void)
  *****************************************************************************/
inline void SetThershold(void)
{
    ACCEL_BMA150_REG    reg;
    uint8_t                idx;

    idx = ReadByteSequence(0, ACCEL_BMA150_ADDR11);

    rxBuffer = (uint8_t *)&reg;

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

    idx = WriteByteSequence(0, ACCEL_BMA150_ADDR14, ACCEL_BMA150_THES);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

    idx = ReadByteSequence(0, ACCEL_BMA150_ADDR11);

    rxBuffer = (uint8_t *)&reg;

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif
}
/*******************************************************************************
  Function:  inline void StartXYZAccelConversion(void)
  *****************************************************************************/
inline void StartXYZAccelConversion(void)
{
    uint8_t idx;

    rxBuffer = (uint8_t *)&accelXYZData;

    idx = ReadByteSequence(0, ACCEL_BMA150_ACC_X_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_X_MSB_ADDR);
    
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_Y_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_Y_MSB_ADDR);

    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_Z_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_Z_MSB_ADDR);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  inline void StartXYAccelConversion(void)
  *****************************************************************************/
inline void StartXYAccelConversion(void)
{
    uint8_t idx;

    rxBuffer = (uint8_t *)&accelXYZData;

    idx = ReadByteSequence(0, ACCEL_BMA150_ACC_X_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_X_MSB_ADDR);
    
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_Y_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_Y_MSB_ADDR);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  inline void StartXAccelConversion(void)
  *****************************************************************************/
inline void StartXAccelConversion(void)
{
    uint8_t idx;

    rxBuffer = (uint8_t *)&accelXYZData;

    idx = ReadByteSequence(0, ACCEL_BMA150_ACC_X_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_X_MSB_ADDR);
    
    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  inline void StartYAccelConversion(void)
  *****************************************************************************/
inline void StartYAccelConversion(void)
{
    uint8_t idx;

    rxBuffer = (uint8_t *)&accelXYZData.acc_y_lsb.val;

    idx = ReadByteSequence(0, ACCEL_BMA150_ACC_Y_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_Y_MSB_ADDR);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  inline void StartZAccelConversion(void)
  *****************************************************************************/
inline void StartZAccelConversion(void)
{
    uint8_t idx;

    rxBuffer = (uint8_t *)&accelXYZData.acc_z_lsb.val;

    idx = ReadByteSequence(0, ACCEL_BMA150_ACC_Z_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA150_ACC_Z_MSB_ADDR);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELInitialize(ACCEL_INIT *initialization)
  *****************************************************************************/
ACCEL_RESULT ACCELInitialize(ACCEL_INIT *initialization)
{
    I2CSetFrequency(ACCEL_BMA150_I2C_MODULE, initialization->sourceClock, initialization->dataRate);
    I2CEnable(ACCEL_BMA150_I2C_MODULE, ACCEL_TRUE);

#ifndef ACCEL_USE_EXTERNAL_INTERUPT_HANDLER 
#ifndef ACCEL_USE_POLLING
    INTSetVectorPriority(ACCEL_BMA150_I2C_VECTOR, INT_PRIORITY_LEVEL_2);
#endif
#endif

    if(ReadChipID() == ACCEL_INVALID)
        return ACCEL_INVALID;

    SetRangeAndBandwidth();

    ReadChipVersion();

    SetThershold();

    accelXYZData.acc_x_lsb.new_data_x = 0;
    accelXYZData.acc_y_lsb.new_data_y = 0;
    accelXYZData.acc_z_lsb.new_data_z = 0;

    return ACCEL_VALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELTask(void)
  *****************************************************************************/
ACCEL_RESULT ACCELTask(void)
{
    if(IsBusy())
    {
        ACCEL_BMA150InterruptHandler();

        // one last check to see if we are busy
        if(IsBusy())
            return ACCEL_VALID;
    }

    return ACCEL_INVALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetXYZAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y, ACCEL_DATA *acc_z)
  *****************************************************************************/
ACCEL_RESULT ACCELGetXYZAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y, ACCEL_DATA *acc_z)
{
    if(IsBusy())
        return ACCEL_INVALID;

    if((accelXYZData.acc_x_lsb.new_data_x) && (accelXYZData.acc_y_lsb.new_data_y) && (accelXYZData.acc_z_lsb.new_data_z))
    {
        uint32_t data;
        uint8_t size = 0;

        if(ACCEL_BMA150_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
            size = (uint8_t)(ACCEL_BMA150_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));

        data = (uint32_t)accelXYZData.acc_x_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_x_lsb.val;
        data >>= 6;
        
        *acc_x = (ACCEL_DATA)(data >> size) ;

        data = (uint32_t)accelXYZData.acc_y_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_y_lsb.val;
        data >>= 6;
        
        *acc_y = (ACCEL_DATA)(data >> size);

        data = (uint32_t)accelXYZData.acc_z_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_z_lsb.val;
        data >>= 6;
        
        *acc_z = (ACCEL_DATA)(data >> size);

        accelXYZData.acc_x_lsb.new_data_x = 0;
        accelXYZData.acc_y_lsb.new_data_y = 0;
        accelXYZData.acc_z_lsb.new_data_z = 0;

        return ACCEL_VALID;
    }

    StartXYZAccelConversion();

    return ACCEL_INVALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetXYAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y)
  *****************************************************************************/
ACCEL_RESULT ACCELGetXYAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y)
{
    if(IsBusy())
        return ACCEL_INVALID;

    if((accelXYZData.acc_x_lsb.new_data_x) && (accelXYZData.acc_y_lsb.new_data_y))
    {
        uint32_t data;
        uint8_t size = 0;

        if(ACCEL_BMA150_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
            size = (uint8_t)(ACCEL_BMA150_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));

        data = (uint32_t)accelXYZData.acc_x_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_x_lsb.val;
        data >>= 6;
        
        *acc_x = (ACCEL_DATA)(data >> size) ;

        data = (uint32_t)accelXYZData.acc_y_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_y_lsb.val;
        data >>= 6;
        
        *acc_y = (ACCEL_DATA)(data >> size);


        accelXYZData.acc_x_lsb.new_data_x = 0;
        accelXYZData.acc_y_lsb.new_data_y = 0;
        accelXYZData.acc_z_lsb.new_data_z = 0;

        return ACCEL_VALID;
    }

    StartXYAccelConversion();

    return ACCEL_INVALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetXAxis(ACCEL_DATA *acc_x)
  *****************************************************************************/
ACCEL_RESULT ACCELGetXAxis(ACCEL_DATA *acc_x)
{
    if(IsBusy())
        return ACCEL_INVALID;

    if(accelXYZData.acc_x_lsb.new_data_x)
    {
        uint32_t data;
        uint8_t size = 0;

        if(ACCEL_BMA150_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
            size = (uint8_t)(ACCEL_BMA150_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));

        data = (uint32_t)accelXYZData.acc_x_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_x_lsb.val;
        data >>= 6;
        
        *acc_x = (ACCEL_DATA)(data >> size) ;

        accelXYZData.acc_x_lsb.new_data_x = 0;
        accelXYZData.acc_y_lsb.new_data_y = 0;
        accelXYZData.acc_z_lsb.new_data_z = 0;

        return ACCEL_VALID;
    }

    StartXAccelConversion();

    return ACCEL_INVALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetYAxis(ACCEL_DATA *acc_y)
  *****************************************************************************/
ACCEL_RESULT ACCELGetYAxis(ACCEL_DATA *acc_y)
{
    if(IsBusy())
        return ACCEL_INVALID;

    if(accelXYZData.acc_y_lsb.new_data_y)
    {
        uint32_t data;
        uint8_t size = 0;

        if(ACCEL_BMA150_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
            size = (uint8_t)(ACCEL_BMA150_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));

        data = (uint32_t)accelXYZData.acc_y_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_y_lsb.val;
        data >>= 6;
        
        *acc_y = (ACCEL_DATA)(data >> size) ;

        accelXYZData.acc_x_lsb.new_data_x = 0;
        accelXYZData.acc_y_lsb.new_data_y = 0;
        accelXYZData.acc_z_lsb.new_data_z = 0;

        return ACCEL_VALID;
    }

    StartYAccelConversion();

    return ACCEL_INVALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetZAxis(ACCEL_DATA *acc_z)
  *****************************************************************************/
ACCEL_RESULT ACCELGetZAxis(ACCEL_DATA *acc_z)
{
    if(IsBusy())
        return ACCEL_INVALID;

    if(accelXYZData.acc_z_lsb.new_data_z)
    {
        uint32_t data;
        uint8_t size = 0;

        if(ACCEL_BMA150_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
            size = (uint8_t)(ACCEL_BMA150_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));

        data = (uint32_t)accelXYZData.acc_z_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_z_lsb.val;
        data >>= 6;
        
        *acc_z = (ACCEL_DATA)(data >> size) ;

        accelXYZData.acc_x_lsb.new_data_x = 0;
        accelXYZData.acc_y_lsb.new_data_y = 0;
        accelXYZData.acc_z_lsb.new_data_z = 0;

        return ACCEL_VALID;
    }

    StartZAccelConversion();

    return ACCEL_INVALID;
}

#endif //#if defined (USE_ACCELEROMETER_BMA150)