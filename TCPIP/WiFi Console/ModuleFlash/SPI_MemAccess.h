/*********************************************************************
 *
 *                  SPI and Memory specific definitions
 *
 *********************************************************************
 * FileName:        SPI_MemAccess.h
 * Dependencies:    None
 * Processor:       PIC32
 * Compiler:        Microchip C32 v1.11b or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2012 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and
 *      ENC28J60.h ported to a non-Microchip device used in
 *      conjunction with a Microchip ethernet controller for the
 *      sole purpose of interfacing with the ethernet controller.
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 6/15/2015	Original
 ********************************************************************/
#ifndef SPIMEMACCESS_H
#define SPIMEMACCESS_H

#include "GenericTypeDefs.h"

//#define PATCH_SIGNATURE            (UINT32)(~(0x00043fbc))
#define BAUD_RATE                  (19200)

#define HTOWFL(a) (((a & 0x000000ff) << 24) | ((a & 0x0000ff00) << 8) | ((a & 0x00ff0000) >> 8) | ((a & 0xff000000) >> 24))

// common SPI flash commands
#define SERIAL_FLASH_WRSR          (0x01)
#define SERIAL_FLASH_READ          (0x03)
#define SERIAL_FLASH_WRDI          (0x04)
#define SERIAL_FLASH_RDSR          (0x05)
#define SERIAL_FLASH_WREN          (0x06)
#define SERIAL_FLASH_CHIP_ERASE    (0x60)
#define SERIAL_FLASH_AAI_PROGRAM   (0xAF)

#define FLASH_SIZE_IN_BYTES        (0x20000)
#define CALIBRATION_DATA_ADDR      (0x1FF80)


#define WF_SetCE_N(level)                                           \
    /* configure I/O as ouput         */                            \
    WF_HIBERNATE_TRIS = 0;                                          \
    /* set pin to desired level       */                            \
    WF_HIBERNATE_IO   = level    

#define WF_SetRST_N(level)                                          \
    /* configure the I/O as an output   */                          \
    WF_RESET_TRIS = 0;                                              \
    /* set pin to desired level         */                          \
    WF_RESET_IO   = level    


// 68K memory access registers
#define GPIO_DATA_EN_REG              0xF00108
#define GPIO_SEL0_REG                 0xF00120
#define GPIO_SEL1_REG                 0xF00124
#define GPIO_SEL2_REG                 0xF00128

// SPI between the wifi device and the serial flash
#define F_SPI_CTRL_REG                0x00F00200
#define F_SPI_WRT_DATA_REG            0x00F00208
#define F_SPI_RD_DATA_REG             0x00F0020C

#define Flash_ChipEnableOn()          MemWriteU32(F_SPI_CTRL_REG, F_SPI_CTRL_DEFAULT_CONFIG | F_SPI_CTRL_CS_MASK)
#define Flash_ChipEnableOff()         MemWriteU32(F_SPI_CTRL_REG, F_SPI_CTRL_DEFAULT_CONFIG)

#define F_SPI_CTRL_ACCESS_SIZE_MASK   0x04    //  (0 = 8-bit, 1 = 16-bit)
#define F_SPI_CTRL_CS_MASK            0x02    //  (1 = enables the flash device)
#define F_SPI_CTRL_ACTIVE_MASK        0x01    //  (1 = running, reset to 0 by hardware)
#define F_SPI_CTRL_DEFAULT_CONFIG     (0x40)

#define F_SPI_ACTIVATE_8BIT_OP        (F_SPI_CTRL_DEFAULT_CONFIG | F_SPI_CTRL_ACTIVE_MASK | F_SPI_CTRL_CS_MASK)
#define F_SPI_ACTIVATE_16BIT_OP       (F_SPI_CTRL_DEFAULT_CONFIG | F_SPI_CTRL_ACTIVE_MASK | F_SPI_CTRL_CS_MASK | F_SPI_CTRL_ACCESS_SIZE_MASK)

#define HOST_16_BIT_REG_TYPE_MASK     (2 << 24)
#define HOST_32_BIT_REG_TYPE_MASK     (4 << 24)

#define CTRL_WRITE_MASK               0x0000   // configure to write device memory
#define CTRL_READ_MASK                0x0002   // configure to read device memory
#define CTRL_ACTIVE_MASK              0x0001   // activate memory window control
  
#define CTRL_1_BYTE_MASK              0x0010   // mask used to write or read 8-bit words from memory
#define CTRL_2_LS_BYTE_MASK           0x0030   // mask used to write or LS 2 bytes from a 32-bit memory word
#define CTRL_2_MS_BYTE_MASK           0x00C0   // mask used to write or MS 2 bytes from a 32-bit memory word
#define CTRL_4_BYTE_MASK              0x00F0   // mask used to write or read 32-bit words from memory

#define CTRL_WRITE_32_BIT_MASK        (CTRL_WRITE_MASK | CTRL_ACTIVE_MASK | CTRL_4_BYTE_MASK)
#define CTRL_READ_32_BIT_MASK         (CTRL_READ_MASK  | CTRL_ACTIVE_MASK | CTRL_4_BYTE_MASK)

// 32-bit Host Registers 
#define HOST_MEM_WIN_CTRL_REG         0x37
#define HOST_MEM_WIN_BASE_REG         0x38  // 0x38 - write all 4 bytes to this address to update win base register
#define HOST_MEM_WIN_DATA_REG         0x3A  // 0x3a - write all 4 bytes to this address to update win data register

#define WF_READ_REGISTER_MASK         ((UINT8)(0x40))
#define WF_WRITE_REGISTER_MASK        ((UINT8)(0x00))


/*---------------------------------*/
/* MRF24WB0M 16-bit Host Registers */
/*---------------------------------*/
#define WF_HOST_RAW0_CTRL1_REG      ((UINT16)(0x26))
#define WF_HOST_RAW0_STATUS_REG     ((UINT16)(0x28))
#define WF_HOST_RAW1_CTRL1_REG      ((UINT16)(0x2a))
#define WF_HOST_INTR2_REG           ((UINT16)(0x2d)) /* 16-bit register containing 2nd level interrupt bits */
#define WF_HOST_INTR2_MASK_REG      ((UINT16)(0x2e))
#define WF_HOST_WFIFO_BCNT0_REG     ((UINT16)(0x2f)) /* 16-bit register containing available write size for fifo 0 (data)   */
                                                     /* (LS 12 bits contain the length)                                     */
                                                       
#define WF_HOST_WFIFO_BCNT1_REG     ((UINT16)(0x31)) /* 16-bit register containing available write size for fifo 1 (mgmt)   */
                                                     /* (LS 12 bits contain the length)                                     */

#define WF_HOST_RFIFO_BCNT0_REG     ((UINT16)(0x33)) /* 16-bit register containing number of bytes in read fifo 0 (data rx) */
                                                     /* (LS 12 bits contain the length)                                     */
#define WF_HOST_RESET_REG           ((UINT16)(0x3c))
#define WF_HOST_RESET_MASK          ((UINT16)(0x0001))
                                                       
#define WF_PSPOLL_H_REG             ((UINT16)(0x3d)) /* 16-bit register used to control low power mode                      */
#define WF_INDEX_ADDR_REG           ((UINT16)(0x3e)) /* 16-bit register to move the data window                             */
#define WF_INDEX_DATA_REG           ((UINT16)(0x3f)) /* 16-bit register to read or write address-indexed register           */

/*----------------------------------------------------------------------------------------*/
/* MRF24WB0M registers accessed via the WF_INDEX_ADDR_REG and WF_INDEX_DATA_REG registers */
/*----------------------------------------------------------------------------------------*/
#define WF_HW_STATUS_REG            ((UINT16)(0x2a)) /* 16-bit read only register providing hardware status bits */
#define WF_CONFIG_CTRL0_REG         ((UINT16)(0x2e)) /* 16-bit register used to initiate Hard reset              */
#define WF_LOW_POWER_STATUS_REG     ((UINT16)(0x3e)) /* 16-bit register read to determine when low power is done */

/* This bit mask is used in the HW_STATUS_REG to determine */
/* when the MRF24WB0M has completed its hardware reset.       */
/*  0 : MRF24WB0M is in reset                                 */
/*  1 : MRF24WB0M is not in reset                             */
#define WF_HW_STATUS_NOT_IN_RESET_MASK ((UINT16)(0x1000)) 

typedef enum
{
    WF_LOW  = 0,
    WF_HIGH = 1
} tWFPinLevel;    


// SPI 68K Memory Access Functions


UINT32  Read32BitWFRegisterr_Update(UINT8 regId);
void    Write32BitWFRegister_Update(UINT8 regId, UINT32 value);

UINT32  MemReadU32(UINT32 address);
void    MemWriteU32(UINT32 address, UINT32 value);

void    FlashBusSend8(UINT8 val);
void    FlashBusSend16(UINT16 val);

void    ReadSerialFlashArray(UINT8 *buffer, UINT32 address, UINT32 length);
void    WriteSerialFlashArray(UINT8 *buffer, UINT32 address, UINT32 length);

void    FlashWriteEnable(void);
UINT8   ReadFlashStatus(void);

void    FlashEraseChip(void);
void    SectorErase(UINT32 address);

void    WFEnableMemAccess(void);
void    WFDisableMemAccess(void);

void    WF_SpiInit_Update(void);
void    ConfigureSpiMRF24WB0M(void);
void    WF_SpiEnableChipSelect(void);
void    WF_SpiDisableChipSelect(void);
void    WFSpiTxRx(UINT8   *p_txBuf, 
               UINT16  txLen, 
               UINT8   *p_rxBuf,
               UINT16  rxLen);
void    WFChipReset(void);

#endif
