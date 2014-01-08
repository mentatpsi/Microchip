/******************************************************************************
 *    SSD1926 hardware JPEG decoder driver
 ******************************************************************************
 * FileName:        SSD1926_JPEG.h
 * Dependencies:    see included files below
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        C30 v2.01/C32 v0.00.18
 * Company:         Microchip Technology, Inc.
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 
Author                 Date           Comments
--------------------------------------------------------------------------------
Sean Justice        15_Sept-2008      First release
Anton Alkhimenok    06_Jun-2009       Ported to PIC24
*******************************************************************************/
#ifndef _SSD_JPEG_DECODER_HEADER_FILE
    #define _SSD_JPEG_DECODER_HEADER_FILE

// Include
    #include <GenericTypeDefs.h>

// JPEG FIFO Size and Location
    #define SSD_JPEG_FIFO_BLK_SIZE      (DWORD) (4 * 1024)
    #define SSD_JPEG_FIFO_BLK_NUMBER    (DWORD) 16
    #define SSD_JPEG_FIFO_SIZE          SSD_JPEG_FIFO_BLK_SIZE * SSD_JPEG_FIFO_BLK_NUMBER
    #define SSD_JPEG_FIFO_START_ADDR    (DWORD) (0x30000)

// JPEG Decoder Parameters structure
typedef struct _SSD_JPEG_DECODE
{
    void    *stream;
    UINT16  image_width;
    UINT16  image_height;
    DWORD   display_width;
    DWORD   display_height;
    UINT16  op_mode;
    DWORD   fifo_addr;
    DWORD   fifo_addr_end;
    DWORD   size;
    DWORD   bytes_read;
} SSD_JPEG_DECODE;

// JPEG Decoder Error Codes
typedef enum _SSD_JPEG_ERR
{
    NO_ERR,
    ERR_TIMEOUT,
    ERR_IMCOMPLETE,
    ERR_NO_DATA,
    ERR_DECODE,
    ERR_CODEC_INT
} SSD_JPEG_ERR;

/**************************************************************************
  Function:
    void SSD1926SetRGB(void)
  Conditions:
    None.
  Input:
    None.
  Return:
    None.
  Side Effects:
    None.
  Description:
    Sets main and floating windows to RGB mode
  Remarks:
    This mode is used by the graphics library.
  **************************************************************************/
void SSD1926SetRGB(void);

/**************************************************************************
  Function:
    void SSD1926SetYUV(void)
  Conditions:
    None.
  Input:
    None.
  Return:
    None.
  Side Effects:
    Objects drawn by graphics library cannot be displayed correctly in this mode.
  Description:
    Sets main and floating windows to YUV mode
  Remarks:
    Usually JPEG files use YUV format.
  **************************************************************************/
void SSD1926SetYUV(void);

/**************************************************************************
  Function:
    void JPEGPutImage(char* filename)
  Conditions:
    FSInit() file system initialization must be done, 
    SSD1926SetYUV() YUV mode must be set.
  Input:
    filename - pointer to the file name string.
  Return:
    None.
  Side Effects:
    None.
  Description:
    Displays the JPEG image form the JPEG file specified in the screen center.
  Remarks:
    Usually JPEG files use YUV format.
  **************************************************************************/
BYTE JPEGPutImage(char *filename);

// SSD1926 JPEG Registers
    #define SSD_REG_FRC_FRAME_CTL               0x334
    #define SSD_REG_FRC_ENABLE                  0x336

    #define SSD_JPEG_REG_RESIZER_CTL            0x360
    #define SSD_JPEG_REG_RESIZER_STARTX_0       0x364
    #define SSD_JPEG_REG_RESIZER_STARTX_1       0x365
    #define SSD_JPEG_REG_RESIZER_STARTY_0       0x366
    #define SSD_JPEG_REG_RESIZER_STARTY_1       0x367
    #define SSD_JPEG_REG_RESIZER_ENDX_0         0x368
    #define SSD_JPEG_REG_RESIZER_ENDX_1         0x369
    #define SSD_JPEG_REG_RESIZER_ENDY_0         0x36a
    #define SSD_JPEG_REG_RESIZER_ENDY_1         0x36b
    #define SSD_JPEG_REG_RESIZER_OP_0           0x36c
    #define SSD_JPEG_REG_RESIZER_OP_1           0x36e

    #define SSD_JPEG_REG_CTRL                   0x380
    #define SSD_JPEG_REG_STATUS                 0x382
    #define SSD_JPEG_REG_STATUS1                0x383

    #define SSD_JPEG_REG_RAW_STATUS             0x384
    #define SSD_JPEG_REG_RAW_STATUS1            0x385

    #define SSD_JPEG_REG_INTR_CTL0              0x386
    #define SSD_JPEG_REG_INTR_CTL1              0x387

    #define SSD_JPEG_REG_START_STOP             0x38a

    #define SSD_JPEG_REG_FIFO_CTL               0x3a0
    #define SSD_JPEG_REG_FIFO_STATUS            0x3a2
    #define SSD_JPEG_REG_FIFO_SIZE              0x3a4

    #define SSD_JPEG_REG_ENCODE_SIZE_LIMIT_0    0x3B0
    #define SSD_JPEG_REG_ENCODE_SIZE_LIMIT_1    0x3B1
    #define SSD_JPEG_REG_ENCODE_SIZE_LIMIT_2    0x3B2

    #define SSD_JPEG_REG_ENCODE_SIZE_RESULT_0   0x3b4
    #define SSD_JPEG_REG_ENCODE_SIZE_RESULT_1   0x3b5
    #define SSD_JPEG_REG_ENCODE_SIZE_RESULT_2   0x3b6

    #define SSD_JPEG_REG_FILE_SIZE_0            0x3B8
    #define SSD_JPEG_REG_FILE_SIZE_1            0x3B9
    #define SSD_JPEG_REG_FILE_SIZE_2            0x3BA

    #define SSD_JPEG_REG_DECODE_X_SIZE          0x3d8
    #define SSD_JPEG_REG_DECODE_Y_SIZE          0x3dc

    #define SSD_JPEG_REG_OP_MODE_ENC            0x400
    #define SSD_JPEG_REG_OP_MODE                0x401

    #define SSD_JPEG_REG_CMD                    0x402

    #define REG_DRI_SETTING                     0x40a

    #define SSD_JPEG_REG_DECODE_VALUE           0x404

    #define SSD_JPEG_REG_Y_PIXEL_SIZE_0         0x40c
    #define SSD_JPEG_REG_Y_PIXEL_SIZE_1         0x40d
    #define SSD_JPEG_REG_X_PIXEL_SIZE_0         0x40e
    #define SSD_JPEG_REG_X_PIXEL_SIZE_1         0x40f

    #define SSD_JPEG_REG_SRC_START_ADDR_0       0x410
    #define SSD_JPEG_REG_SRC_START_ADDR_1       0x411
    #define SSD_JPEG_REG_SRC_START_ADDR_2       0x412

//#define SSD_JPEG_REG_SRC_START_ADDR_3                       0x413
    #define SSD_JPEG_REG_DEST_START_ADDR_0  0x414
    #define SSD_JPEG_REG_DEST_START_ADDR_1  0x415
    #define SSD_JPEG_REG_DEST_START_ADDR_2  0x416
    #define SSD_JPEG_REG_DEST_START_ADDR_3  0x417

    #define SSD_JPEG_REG_RST_MARKER         0x41c

    #define SSD_JPEG_REG_RST_MARKER_STATUS  0x41e

    #define SSD_JPEG_REG_INSERT_MARKER00    0x420
    #define SSD_JPEG_REG_INSERT_MARKER01    0x422
    #define SSD_JPEG_REG_MARKER_LENGTH00    0x424
    #define SSD_JPEG_REG_MARKER_LENGTH01    0x426

    #define SSD_JPEG_REG_MARKER_DATA_00     0x428
    #define SSD_JPEG_REG_MARKER_DATA_01     0x42a
    #define SSD_JPEG_REG_MARKER_DATA_02     0x42c
    #define SSD_JPEG_REG_MARKER_DATA_03     0x42e
    #define SSD_JPEG_REG_MARKER_DATA_04     0x430
    #define SSD_JPEG_REG_MARKER_DATA_05     0x432
    #define SSD_JPEG_REG_MARKER_DATA_06     0x434
    #define SSD_JPEG_REG_MARKER_DATA_07     0x436
    #define SSD_JPEG_REG_MARKER_DATA_08     0x438
    #define SSD_JPEG_REG_MARKER_DATA_09     0x43a
    #define SSD_JPEG_REG_MARKER_DATA_10     0x43c
    #define SSD_JPEG_REG_MARKER_DATA_11     0x43e
    #define SSD_JPEG_REG_MARKER_DATA_12     0x440
    #define SSD_JPEG_REG_MARKER_DATA_13     0x442
    #define SSD_JPEG_REG_MARKER_DATA_14     0x444
    #define SSD_JPEG_REG_MARKER_DATA_15     0x446
    #define SSD_JPEG_REG_MARKER_DATA_16     0x448
    #define SSD_JPEG_REG_MARKER_DATA_17     0x44a
    #define SSD_JPEG_REG_MARKER_DATA_18     0x44c
    #define SSD_JPEG_REG_MARKER_DATA_19     0x44e
    #define SSD_JPEG_REG_MARKER_DATA_20     0x450
    #define SSD_JPEG_REG_MARKER_DATA_21     0x452
    #define SSD_JPEG_REG_MARKER_DATA_22     0x454
    #define SSD_JPEG_REG_MARKER_DATA_23     0x456
    #define SSD_JPEG_REG_MARKER_DATA_24     0x458
    #define SSD_JPEG_REG_MARKER_DATA_25     0x45a
    #define SSD_JPEG_REG_MARKER_DATA_26     0x45c
    #define SSD_JPEG_REG_MARKER_DATA_27     0x45e
    #define SSD_JPEG_REG_MARKER_DATA_28     0x460
    #define SSD_JPEG_REG_MARKER_DATA_29     0x462
    #define SSD_JPEG_REG_MARKER_DATA_30     0x464
    #define SSD_JPEG_REG_MARKER_DATA_31     0x466

    #define SSD_JPEG_REG_SOI_CONST_00       0x468
    #define SSD_JPEG_REG_SOI_CONST_01       0x46a

    #define SSD_JPEG_REG_JFIF_CONST_00      0x46c
    #define SSD_JPEG_REG_JFIF_CONST_01      0x46e
    #define SSD_JPEG_REG_JFIF_CONST_02      0x470
    #define SSD_JPEG_REG_JFIF_CONST_03      0x472
    #define SSD_JPEG_REG_JFIF_CONST_04      0x474
    #define SSD_JPEG_REG_JFIF_CONST_05      0x476
    #define SSD_JPEG_REG_JFIF_CONST_06      0x478
    #define SSD_JPEG_REG_JFIF_CONST_07      0x47a
    #define SSD_JPEG_REG_JFIF_CONST_08      0x47c
    #define SSD_JPEG_REG_JFIF_CONST_09      0x47e
    #define SSD_JPEG_REG_JFIF_CONST_10      0x480
    #define SSD_JPEG_REG_JFIF_CONST_11      0x482
    #define SSD_JPEG_REG_JFIF_CONST_12      0x484
    #define SSD_JPEG_REG_JFIF_CONST_13      0x486
    #define SSD_JPEG_REG_JFIF_CONST_14      0x488
    #define SSD_JPEG_REG_JFIF_CONST_15      0x48a
    #define SSD_JPEG_REG_JFIF_CONST_16      0x48c
    #define SSD_JPEG_REG_JFIF_CONST_17      0x48e

    #define SSD_JPEG_REG_LUM_DC_HT_CONST_00 0x490
    #define SSD_JPEG_REG_LUM_DC_HT_CONST_01 0x492
    #define SSD_JPEG_REG_LUM_DC_HT_CONST_02 0x494
    #define SSD_JPEG_REG_LUM_DC_HT_CONST_03 0x496
    #define SSD_JPEG_REG_LUM_DC_HT_CONST_04 0x498

    #define SSD_JPEG_REG_CHR_DC_HT_CONST_00 0x4a0
    #define SSD_JPEG_REG_CHR_DC_HT_CONST_01 0x4a2
    #define SSD_JPEG_REG_CHR_DC_HT_CONST_02 0x4a4
    #define SSD_JPEG_REG_CHR_DC_HT_CONST_03 0x4a6
    #define SSD_JPEG_REG_CHR_DC_HT_CONST_04 0x4a8

    #define SSD_JPEG_REG_LUM_AC_HT_CONST_00 0x4b0
    #define SSD_JPEG_REG_LUM_AC_HT_CONST_01 0x4b2
    #define SSD_JPEG_REG_LUM_AC_HT_CONST_02 0x4b4
    #define SSD_JPEG_REG_LUM_AC_HT_CONST_03 0x4b6
    #define SSD_JPEG_REG_LUM_AC_HT_CONST_04 0x4b8

    #define SSD_JPEG_REG_CHR_AC_HT_CONST_00 0x4c0
    #define SSD_JPEG_REG_CHR_AC_HT_CONST_01 0x4c2
    #define SSD_JPEG_REG_CHR_AC_HT_CONST_02 0x4c4
    #define SSD_JPEG_REG_CHR_AC_HT_CONST_03 0x4c6
    #define SSD_JPEG_REG_CHR_AC_HT_CONST_04 0x4c8

    #define SSD_JPEG_REG_LUM_QT_CONST_00    0x4d0
    #define SSD_JPEG_REG_LUM_QT_CONST_01    0x4d2
    #define SSD_JPEG_REG_LUM_QT_CONST_02    0x4d4
    #define SSD_JPEG_REG_LUM_QT_CONST_03    0x4d6
    #define SSD_JPEG_REG_LUM_QT_CONST_04    0x4d8

    #define SSD_JPEG_REG_CHR_QT_CONST_00    0x4e0
    #define SSD_JPEG_REG_CHR_QT_CONST_01    0x4e2
    #define SSD_JPEG_REG_CHR_QT_CONST_02    0x4e4
    #define SSD_JPEG_REG_CHR_QT_CONST_03    0x4e6
    #define SSD_JPEG_REG_CHR_QT_CONST_04    0x4e8

    #define SSD_JPEG_REG_SOF_CONST_00       0x4f0
    #define SSD_JPEG_REG_SOF_CONST_01       0x4f2
    #define SSD_JPEG_REG_SOF_CONST_02       0x4f4
    #define SSD_JPEG_REG_SOF_CONST_03       0x4f6
    #define SSD_JPEG_REG_SOF_CONST_04       0x4f8

    #define SSD_JPEG_REG_QUANT_T0_00        0x500
    #define SSD_JPEG_REG_QUANT_T0_01        0x502
    #define SSD_JPEG_REG_QUANT_T0_02        0x504
    #define SSD_JPEG_REG_QUANT_T0_03        0x506
    #define SSD_JPEG_REG_QUANT_T0_04        0x508
    #define SSD_JPEG_REG_QUANT_T0_05        0x50a
    #define SSD_JPEG_REG_QUANT_T0_06        0x50c
    #define SSD_JPEG_REG_QUANT_T0_07        0x50e
    #define SSD_JPEG_REG_QUANT_T0_08        0x510
    #define SSD_JPEG_REG_QUANT_T0_09        0x512
    #define SSD_JPEG_REG_QUANT_T0_10        0x514
    #define SSD_JPEG_REG_QUANT_T0_11        0x516
    #define SSD_JPEG_REG_QUANT_T0_12        0x518
    #define SSD_JPEG_REG_QUANT_T0_13        0x51a
    #define SSD_JPEG_REG_QUANT_T0_14        0x51c
    #define SSD_JPEG_REG_QUANT_T0_15        0x51e
    #define SSD_JPEG_REG_QUANT_T0_16        0x520
    #define SSD_JPEG_REG_QUANT_T0_17        0x522
    #define SSD_JPEG_REG_QUANT_T0_18        0x524
    #define SSD_JPEG_REG_QUANT_T0_19        0x526
    #define SSD_JPEG_REG_QUANT_T0_20        0x528
    #define SSD_JPEG_REG_QUANT_T0_21        0x52a
    #define SSD_JPEG_REG_QUANT_T0_22        0x52c
    #define SSD_JPEG_REG_QUANT_T0_23        0x52e
    #define SSD_JPEG_REG_QUANT_T0_24        0x530
    #define SSD_JPEG_REG_QUANT_T0_25        0x532
    #define SSD_JPEG_REG_QUANT_T0_26        0x534
    #define SSD_JPEG_REG_QUANT_T0_27        0x536
    #define SSD_JPEG_REG_QUANT_T0_28        0x538
    #define SSD_JPEG_REG_QUANT_T0_29        0x53a
    #define SSD_JPEG_REG_QUANT_T0_30        0x53c
    #define SSD_JPEG_REG_QUANT_T0_31        0x53e
    #define SSD_JPEG_REG_QUANT_T0_32        0x540
    #define SSD_JPEG_REG_QUANT_T0_33        0x542
    #define SSD_JPEG_REG_QUANT_T0_34        0x544
    #define SSD_JPEG_REG_QUANT_T0_35        0x546
    #define SSD_JPEG_REG_QUANT_T0_36        0x548
    #define SSD_JPEG_REG_QUANT_T0_37        0x54a
    #define SSD_JPEG_REG_QUANT_T0_38        0x54c
    #define SSD_JPEG_REG_QUANT_T0_39        0x54e
    #define SSD_JPEG_REG_QUANT_T0_40        0x550
    #define SSD_JPEG_REG_QUANT_T0_41        0x552
    #define SSD_JPEG_REG_QUANT_T0_42        0x554
    #define SSD_JPEG_REG_QUANT_T0_43        0x556
    #define SSD_JPEG_REG_QUANT_T0_44        0x558
    #define SSD_JPEG_REG_QUANT_T0_45        0x55a
    #define SSD_JPEG_REG_QUANT_T0_46        0x55c
    #define SSD_JPEG_REG_QUANT_T0_47        0x55e
    #define SSD_JPEG_REG_QUANT_T0_48        0x560
    #define SSD_JPEG_REG_QUANT_T0_49        0x562
    #define SSD_JPEG_REG_QUANT_T0_50        0x564
    #define SSD_JPEG_REG_QUANT_T0_51        0x566
    #define SSD_JPEG_REG_QUANT_T0_52        0x568
    #define SSD_JPEG_REG_QUANT_T0_53        0x56a
    #define SSD_JPEG_REG_QUANT_T0_54        0x56c
    #define SSD_JPEG_REG_QUANT_T0_55        0x56e
    #define SSD_JPEG_REG_QUANT_T0_56        0x570
    #define SSD_JPEG_REG_QUANT_T0_57        0x572
    #define SSD_JPEG_REG_QUANT_T0_58        0x574
    #define SSD_JPEG_REG_QUANT_T0_59        0x576
    #define SSD_JPEG_REG_QUANT_T0_60        0x578
    #define SSD_JPEG_REG_QUANT_T0_61        0x57a
    #define SSD_JPEG_REG_QUANT_T0_62        0x57c
    #define SSD_JPEG_REG_QUANT_T0_63        0x57e

    #define SSD_JPEG_REG_QUANT_T1_00        0x580
    #define SSD_JPEG_REG_QUANT_T1_01        0x582
    #define SSD_JPEG_REG_QUANT_T1_02        0x584
    #define SSD_JPEG_REG_QUANT_T1_03        0x586
    #define SSD_JPEG_REG_QUANT_T1_04        0x588
    #define SSD_JPEG_REG_QUANT_T1_05        0x58a
    #define SSD_JPEG_REG_QUANT_T1_06        0x58c
    #define SSD_JPEG_REG_QUANT_T1_07        0x58e
    #define SSD_JPEG_REG_QUANT_T1_08        0x590
    #define SSD_JPEG_REG_QUANT_T1_09        0x592
    #define SSD_JPEG_REG_QUANT_T1_10        0x594
    #define SSD_JPEG_REG_QUANT_T1_11        0x596
    #define SSD_JPEG_REG_QUANT_T1_12        0x598
    #define SSD_JPEG_REG_QUANT_T1_13        0x59a
    #define SSD_JPEG_REG_QUANT_T1_14        0x59c
    #define SSD_JPEG_REG_QUANT_T1_15        0x59e
    #define SSD_JPEG_REG_QUANT_T1_16        0x5a0
    #define SSD_JPEG_REG_QUANT_T1_17        0x5a2
    #define SSD_JPEG_REG_QUANT_T1_18        0x5a4
    #define SSD_JPEG_REG_QUANT_T1_19        0x5a6
    #define SSD_JPEG_REG_QUANT_T1_20        0x5a8
    #define SSD_JPEG_REG_QUANT_T1_21        0x5aa
    #define SSD_JPEG_REG_QUANT_T1_22        0x5ac
    #define SSD_JPEG_REG_QUANT_T1_23        0x5ae
    #define SSD_JPEG_REG_QUANT_T1_24        0x5b0
    #define SSD_JPEG_REG_QUANT_T1_25        0x5b2
    #define SSD_JPEG_REG_QUANT_T1_26        0x5b4
    #define SSD_JPEG_REG_QUANT_T1_27        0x5b6
    #define SSD_JPEG_REG_QUANT_T1_28        0x5b8
    #define SSD_JPEG_REG_QUANT_T1_29        0x5ba
    #define SSD_JPEG_REG_QUANT_T1_30        0x5bc
    #define SSD_JPEG_REG_QUANT_T1_31        0x5be
    #define SSD_JPEG_REG_QUANT_T1_32        0x5c0
    #define SSD_JPEG_REG_QUANT_T1_33        0x5c2
    #define SSD_JPEG_REG_QUANT_T1_34        0x5c4
    #define SSD_JPEG_REG_QUANT_T1_35        0x5c6
    #define SSD_JPEG_REG_QUANT_T1_36        0x5c8
    #define SSD_JPEG_REG_QUANT_T1_37        0x5ca
    #define SSD_JPEG_REG_QUANT_T1_38        0x5cc
    #define SSD_JPEG_REG_QUANT_T1_39        0x5ce
    #define SSD_JPEG_REG_QUANT_T1_40        0x5d0
    #define SSD_JPEG_REG_QUANT_T1_41        0x5d2
    #define SSD_JPEG_REG_QUANT_T1_42        0x5d4
    #define SSD_JPEG_REG_QUANT_T1_43        0x5d6
    #define SSD_JPEG_REG_QUANT_T1_44        0x5d8
    #define SSD_JPEG_REG_QUANT_T1_45        0x5da
    #define SSD_JPEG_REG_QUANT_T1_46        0x5dc
    #define SSD_JPEG_REG_QUANT_T1_47        0x5de
    #define SSD_JPEG_REG_QUANT_T1_48        0x5e0
    #define SSD_JPEG_REG_QUANT_T1_49        0x5e2
    #define SSD_JPEG_REG_QUANT_T1_50        0x5e4
    #define SSD_JPEG_REG_QUANT_T1_51        0x5e6
    #define SSD_JPEG_REG_QUANT_T1_52        0x5e8
    #define SSD_JPEG_REG_QUANT_T1_53        0x5ea
    #define SSD_JPEG_REG_QUANT_T1_54        0x5ec
    #define SSD_JPEG_REG_QUANT_T1_55        0x5ee
    #define SSD_JPEG_REG_QUANT_T1_56        0x5f0
    #define SSD_JPEG_REG_QUANT_T1_57        0x5f2
    #define SSD_JPEG_REG_QUANT_T1_58        0x5f4
    #define SSD_JPEG_REG_QUANT_T1_59        0x5f6
    #define SSD_JPEG_REG_QUANT_T1_60        0x5f8
    #define SSD_JPEG_REG_QUANT_T1_61        0x5fa
    #define SSD_JPEG_REG_QUANT_T1_62        0x5fc
    #define SSD_JPEG_REG_QUANT_T1_63        0x5fe

    #define SSD_JPEG_REG_DC_T0_R0_00        0x600
    #define SSD_JPEG_REG_DC_T0_R0_01        0x602
    #define SSD_JPEG_REG_DC_T0_R0_02        0x604
    #define SSD_JPEG_REG_DC_T0_R0_03        0x606
    #define SSD_JPEG_REG_DC_T0_R0_04        0x608
    #define SSD_JPEG_REG_DC_T0_R0_05        0x60a
    #define SSD_JPEG_REG_DC_T0_R0_06        0x60c
    #define SSD_JPEG_REG_DC_T0_R0_07        0x60e
    #define SSD_JPEG_REG_DC_T0_R0_08        0x610
    #define SSD_JPEG_REG_DC_T0_R0_09        0x612
    #define SSD_JPEG_REG_DC_T0_R0_10        0x614
    #define SSD_JPEG_REG_DC_T0_R0_11        0x616
    #define SSD_JPEG_REG_DC_T0_R0_12        0x618
    #define SSD_JPEG_REG_DC_T0_R0_13        0x61a
    #define SSD_JPEG_REG_DC_T0_R0_14        0x61c
    #define SSD_JPEG_REG_DC_T0_R0_15        0x61e

    #define SSD_JPEG_REG_DC_T0_R1_00        0x620
    #define SSD_JPEG_REG_DC_T0_R1_01        0x622
    #define SSD_JPEG_REG_DC_T0_R1_02        0x624
    #define SSD_JPEG_REG_DC_T0_R1_03        0x626
    #define SSD_JPEG_REG_DC_T0_R1_04        0x628
    #define SSD_JPEG_REG_DC_T0_R1_05        0x62a
    #define SSD_JPEG_REG_DC_T0_R1_06        0x62c
    #define SSD_JPEG_REG_DC_T0_R1_07        0x62e
    #define SSD_JPEG_REG_DC_T0_R1_08        0x630
    #define SSD_JPEG_REG_DC_T0_R1_09        0x632
    #define SSD_JPEG_REG_DC_T0_R1_10        0x634
    #define SSD_JPEG_REG_DC_T0_R1_11        0x636

    #define SSD_JPEG_REG_AC_T0_R0_00        0x640
    #define SSD_JPEG_REG_AC_T0_R0_01        0x642
    #define SSD_JPEG_REG_AC_T0_R0_02        0x644
    #define SSD_JPEG_REG_AC_T0_R0_03        0x646
    #define SSD_JPEG_REG_AC_T0_R0_04        0x648
    #define SSD_JPEG_REG_AC_T0_R0_05        0x64a
    #define SSD_JPEG_REG_AC_T0_R0_06        0x64c
    #define SSD_JPEG_REG_AC_T0_R0_07        0x64e
    #define SSD_JPEG_REG_AC_T0_R0_08        0x650
    #define SSD_JPEG_REG_AC_T0_R0_09        0x652
    #define SSD_JPEG_REG_AC_T0_R0_10        0x654
    #define SSD_JPEG_REG_AC_T0_R0_11        0x656
    #define SSD_JPEG_REG_AC_T0_R0_12        0x658
    #define SSD_JPEG_REG_AC_T0_R0_13        0x65a
    #define SSD_JPEG_REG_AC_T0_R0_14        0x65c
    #define SSD_JPEG_REG_AC_T0_R0_15        0x65e

    #define SSD_JPEG_REG_AC_T0_R1_00        0x660
    #define SSD_JPEG_REG_AC_T0_R1_01        0x662
    #define SSD_JPEG_REG_AC_T0_R1_02        0x664
    #define SSD_JPEG_REG_AC_T0_R1_03        0x666
    #define SSD_JPEG_REG_AC_T0_R1_04        0x668
    #define SSD_JPEG_REG_AC_T0_R1_05        0x66a
    #define SSD_JPEG_REG_AC_T0_R1_06        0x66c
    #define SSD_JPEG_REG_AC_T0_R1_07        0x66e
    #define SSD_JPEG_REG_AC_T0_R1_08        0x670
    #define SSD_JPEG_REG_AC_T0_R1_09        0x672
    #define SSD_JPEG_REG_AC_T0_R1_10        0x674
    #define SSD_JPEG_REG_AC_T0_R1_11        0x676
    #define SSD_JPEG_REG_AC_T0_R1_12        0x678
    #define SSD_JPEG_REG_AC_T0_R1_13        0x67a
    #define SSD_JPEG_REG_AC_T0_R1_14        0x67c
    #define SSD_JPEG_REG_AC_T0_R1_15        0x67e
    #define SSD_JPEG_REG_AC_T0_R1_16        0x680
    #define SSD_JPEG_REG_AC_T0_R1_17        0x682
    #define SSD_JPEG_REG_AC_T0_R1_18        0x684
    #define SSD_JPEG_REG_AC_T0_R1_19        0x686
    #define SSD_JPEG_REG_AC_T0_R1_20        0x688
    #define SSD_JPEG_REG_AC_T0_R1_21        0x68a
    #define SSD_JPEG_REG_AC_T0_R1_22        0x68c
    #define SSD_JPEG_REG_AC_T0_R1_23        0x68e
    #define SSD_JPEG_REG_AC_T0_R1_24        0x690
    #define SSD_JPEG_REG_AC_T0_R1_25        0x692
    #define SSD_JPEG_REG_AC_T0_R1_26        0x694
    #define SSD_JPEG_REG_AC_T0_R1_27        0x696
    #define SSD_JPEG_REG_AC_T0_R1_28        0x698
    #define SSD_JPEG_REG_AC_T0_R1_29        0x69a
    #define SSD_JPEG_REG_AC_T0_R1_30        0x69c
    #define SSD_JPEG_REG_AC_T0_R1_31        0x69e
    #define SSD_JPEG_REG_AC_T0_R1_32        0x6a0
    #define SSD_JPEG_REG_AC_T0_R1_33        0x6a2
    #define SSD_JPEG_REG_AC_T0_R1_34        0x6a4
    #define SSD_JPEG_REG_AC_T0_R1_35        0x6a6
    #define SSD_JPEG_REG_AC_T0_R1_36        0x6a8
    #define SSD_JPEG_REG_AC_T0_R1_37        0x6aa
    #define SSD_JPEG_REG_AC_T0_R1_38        0x6ac
    #define SSD_JPEG_REG_AC_T0_R1_39        0x6ae
    #define SSD_JPEG_REG_AC_T0_R1_40        0x6b0
    #define SSD_JPEG_REG_AC_T0_R1_41        0x6b2
    #define SSD_JPEG_REG_AC_T0_R1_42        0x6b4
    #define SSD_JPEG_REG_AC_T0_R1_43        0x6b6
    #define SSD_JPEG_REG_AC_T0_R1_44        0x6b8
    #define SSD_JPEG_REG_AC_T0_R1_45        0x6ba
    #define SSD_JPEG_REG_AC_T0_R1_46        0x6bc
    #define SSD_JPEG_REG_AC_T0_R1_47        0x6be
    #define SSD_JPEG_REG_AC_T0_R1_48        0x6c0
    #define SSD_JPEG_REG_AC_T0_R1_49        0x6c2
    #define SSD_JPEG_REG_AC_T0_R1_50        0x6c4
    #define SSD_JPEG_REG_AC_T0_R1_51        0x6c6
    #define SSD_JPEG_REG_AC_T0_R1_52        0x6c8
    #define SSD_JPEG_REG_AC_T0_R1_53        0x6ca
    #define SSD_JPEG_REG_AC_T0_R1_54        0x6cc
    #define SSD_JPEG_REG_AC_T0_R1_55        0x6ce
    #define SSD_JPEG_REG_AC_T0_R1_56        0x6d0
    #define SSD_JPEG_REG_AC_T0_R1_57        0x6d2
    #define SSD_JPEG_REG_AC_T0_R1_58        0x6d4
    #define SSD_JPEG_REG_AC_T0_R1_59        0x6d6
    #define SSD_JPEG_REG_AC_T0_R1_60        0x6d8
    #define SSD_JPEG_REG_AC_T0_R1_61        0x6da
    #define SSD_JPEG_REG_AC_T0_R1_62        0x6dc
    #define SSD_JPEG_REG_AC_T0_R1_63        0x6de
    #define SSD_JPEG_REG_AC_T0_R1_64        0x6e0
    #define SSD_JPEG_REG_AC_T0_R1_65        0x6e2
    #define SSD_JPEG_REG_AC_T0_R1_66        0x6e4
    #define SSD_JPEG_REG_AC_T0_R1_67        0x6e6
    #define SSD_JPEG_REG_AC_T0_R1_68        0x6e8
    #define SSD_JPEG_REG_AC_T0_R1_69        0x6ea
    #define SSD_JPEG_REG_AC_T0_R1_70        0x6ec
    #define SSD_JPEG_REG_AC_T0_R1_71        0x6ee
    #define SSD_JPEG_REG_AC_T0_R1_72        0x6f0
    #define SSD_JPEG_REG_AC_T0_R1_73        0x6f2
    #define SSD_JPEG_REG_AC_T0_R1_74        0x6f4
    #define SSD_JPEG_REG_AC_T0_R1_75        0x6f6
    #define SSD_JPEG_REG_AC_T0_R1_76        0x6f8
    #define SSD_JPEG_REG_AC_T0_R1_77        0x6fa
    #define SSD_JPEG_REG_AC_T0_R1_78        0x6fc
    #define SSD_JPEG_REG_AC_T0_R1_79        0x6fe
    #define SSD_JPEG_REG_AC_T0_R1_80        0x700
    #define SSD_JPEG_REG_AC_T0_R1_81        0x702
    #define SSD_JPEG_REG_AC_T0_R1_82        0x704
    #define SSD_JPEG_REG_AC_T0_R1_83        0x706
    #define SSD_JPEG_REG_AC_T0_R1_84        0x708
    #define SSD_JPEG_REG_AC_T0_R1_85        0x70a
    #define SSD_JPEG_REG_AC_T0_R1_86        0x70c
    #define SSD_JPEG_REG_AC_T0_R1_87        0x70e
    #define SSD_JPEG_REG_AC_T0_R1_88        0x710
    #define SSD_JPEG_REG_AC_T0_R1_89        0x712
    #define SSD_JPEG_REG_AC_T0_R1_90        0x714
    #define SSD_JPEG_REG_AC_T0_R1_91        0x716
    #define SSD_JPEG_REG_AC_T0_R1_92        0x718
    #define SSD_JPEG_REG_AC_T0_R1_93        0x71a
    #define SSD_JPEG_REG_AC_T0_R1_94        0x71c
    #define SSD_JPEG_REG_AC_T0_R1_95        0x71e
    #define SSD_JPEG_REG_AC_T0_R1_96        0x720
    #define SSD_JPEG_REG_AC_T0_R1_97        0x722
    #define SSD_JPEG_REG_AC_T0_R1_98        0x724
    #define SSD_JPEG_REG_AC_T0_R1_99        0x726
    #define SSD_JPEG_REG_AC_T0_R1_100       0x728
    #define SSD_JPEG_REG_AC_T0_R1_101       0x72a
    #define SSD_JPEG_REG_AC_T0_R1_102       0x72c
    #define SSD_JPEG_REG_AC_T0_R1_103       0x72e
    #define SSD_JPEG_REG_AC_T0_R1_104       0x730
    #define SSD_JPEG_REG_AC_T0_R1_105       0x732
    #define SSD_JPEG_REG_AC_T0_R1_106       0x734
    #define SSD_JPEG_REG_AC_T0_R1_107       0x736
    #define SSD_JPEG_REG_AC_T0_R1_108       0x738
    #define SSD_JPEG_REG_AC_T0_R1_109       0x73a
    #define SSD_JPEG_REG_AC_T0_R1_110       0x73c
    #define SSD_JPEG_REG_AC_T0_R1_111       0x73e
    #define SSD_JPEG_REG_AC_T0_R1_112       0x740
    #define SSD_JPEG_REG_AC_T0_R1_113       0x742
    #define SSD_JPEG_REG_AC_T0_R1_114       0x744
    #define SSD_JPEG_REG_AC_T0_R1_115       0x746
    #define SSD_JPEG_REG_AC_T0_R1_116       0x748
    #define SSD_JPEG_REG_AC_T0_R1_117       0x74a
    #define SSD_JPEG_REG_AC_T0_R1_118       0x74c
    #define SSD_JPEG_REG_AC_T0_R1_119       0x74e
    #define SSD_JPEG_REG_AC_T0_R1_120       0x750
    #define SSD_JPEG_REG_AC_T0_R1_121       0x752
    #define SSD_JPEG_REG_AC_T0_R1_122       0x754
    #define SSD_JPEG_REG_AC_T0_R1_123       0x756
    #define SSD_JPEG_REG_AC_T0_R1_124       0x758
    #define SSD_JPEG_REG_AC_T0_R1_125       0x75a
    #define SSD_JPEG_REG_AC_T0_R1_126       0x75c
    #define SSD_JPEG_REG_AC_T0_R1_127       0x75e
    #define SSD_JPEG_REG_AC_T0_R1_128       0x760
    #define SSD_JPEG_REG_AC_T0_R1_129       0x762
    #define SSD_JPEG_REG_AC_T0_R1_130       0x764
    #define SSD_JPEG_REG_AC_T0_R1_131       0x766
    #define SSD_JPEG_REG_AC_T0_R1_132       0x768
    #define SSD_JPEG_REG_AC_T0_R1_133       0x76a
    #define SSD_JPEG_REG_AC_T0_R1_134       0x76c
    #define SSD_JPEG_REG_AC_T0_R1_135       0x76e
    #define SSD_JPEG_REG_AC_T0_R1_136       0x770
    #define SSD_JPEG_REG_AC_T0_R1_137       0x772
    #define SSD_JPEG_REG_AC_T0_R1_138       0x774
    #define SSD_JPEG_REG_AC_T0_R1_139       0x776
    #define SSD_JPEG_REG_AC_T0_R1_140       0x778
    #define SSD_JPEG_REG_AC_T0_R1_141       0x77a
    #define SSD_JPEG_REG_AC_T0_R1_142       0x77c
    #define SSD_JPEG_REG_AC_T0_R1_143       0x77e
    #define SSD_JPEG_REG_AC_T0_R1_144       0x780
    #define SSD_JPEG_REG_AC_T0_R1_145       0x782
    #define SSD_JPEG_REG_AC_T0_R1_146       0x784
    #define SSD_JPEG_REG_AC_T0_R1_147       0x786
    #define SSD_JPEG_REG_AC_T0_R1_148       0x788
    #define SSD_JPEG_REG_AC_T0_R1_149       0x78a
    #define SSD_JPEG_REG_AC_T0_R1_150       0x78c
    #define SSD_JPEG_REG_AC_T0_R1_151       0x78e
    #define SSD_JPEG_REG_AC_T0_R1_152       0x790
    #define SSD_JPEG_REG_AC_T0_R1_153       0x792
    #define SSD_JPEG_REG_AC_T0_R1_154       0x794
    #define SSD_JPEG_REG_AC_T0_R1_155       0x796
    #define SSD_JPEG_REG_AC_T0_R1_156       0x798
    #define SSD_JPEG_REG_AC_T0_R1_157       0x79a
    #define SSD_JPEG_REG_AC_T0_R1_158       0x79c
    #define SSD_JPEG_REG_AC_T0_R1_159       0x79e
    #define SSD_JPEG_REG_AC_T0_R1_160       0x7a0
    #define SSD_JPEG_REG_AC_T0_R1_161       0x7a2

    #define SSD_JPEG_REG_DC_T1_R0_00        0x800
    #define SSD_JPEG_REG_DC_T1_R0_01        0x802
    #define SSD_JPEG_REG_DC_T1_R0_02        0x804
    #define SSD_JPEG_REG_DC_T1_R0_03        0x806
    #define SSD_JPEG_REG_DC_T1_R0_04        0x808
    #define SSD_JPEG_REG_DC_T1_R0_05        0x80a
    #define SSD_JPEG_REG_DC_T1_R0_06        0x80c
    #define SSD_JPEG_REG_DC_T1_R0_07        0x80e
    #define SSD_JPEG_REG_DC_T1_R0_08        0x810
    #define SSD_JPEG_REG_DC_T1_R0_09        0x812
    #define SSD_JPEG_REG_DC_T1_R0_10        0x814
    #define SSD_JPEG_REG_DC_T1_R0_11        0x816
    #define SSD_JPEG_REG_DC_T1_R0_12        0x818
    #define SSD_JPEG_REG_DC_T1_R0_13        0x81a
    #define SSD_JPEG_REG_DC_T1_R0_14        0x81c
    #define SSD_JPEG_REG_DC_T1_R0_15        0x81e

    #define SSD_JPEG_REG_DC_T1_R1_00        0x820
    #define SSD_JPEG_REG_DC_T1_R1_01        0x822
    #define SSD_JPEG_REG_DC_T1_R1_02        0x824
    #define SSD_JPEG_REG_DC_T1_R1_03        0x826
    #define SSD_JPEG_REG_DC_T1_R1_04        0x828
    #define SSD_JPEG_REG_DC_T1_R1_05        0x82a
    #define SSD_JPEG_REG_DC_T1_R1_06        0x82c
    #define SSD_JPEG_REG_DC_T1_R1_07        0x82e
    #define SSD_JPEG_REG_DC_T1_R1_08        0x830
    #define SSD_JPEG_REG_DC_T1_R1_09        0x832
    #define SSD_JPEG_REG_DC_T1_R1_10        0x834
    #define SSD_JPEG_REG_DC_T1_R1_11        0x836

    #define SSD_JPEG_REG_AC_T1_R0_00        0x840
    #define SSD_JPEG_REG_AC_T1_R0_01        0x842
    #define SSD_JPEG_REG_AC_T1_R0_02        0x844
    #define SSD_JPEG_REG_AC_T1_R0_03        0x846
    #define SSD_JPEG_REG_AC_T1_R0_04        0x848
    #define SSD_JPEG_REG_AC_T1_R0_05        0x84a
    #define SSD_JPEG_REG_AC_T1_R0_06        0x84c
    #define SSD_JPEG_REG_AC_T1_R0_07        0x84e
    #define SSD_JPEG_REG_AC_T1_R0_08        0x850
    #define SSD_JPEG_REG_AC_T1_R0_09        0x852
    #define SSD_JPEG_REG_AC_T1_R0_10        0x854
    #define SSD_JPEG_REG_AC_T1_R0_11        0x856
    #define SSD_JPEG_REG_AC_T1_R0_12        0x858
    #define SSD_JPEG_REG_AC_T1_R0_13        0x85a
    #define SSD_JPEG_REG_AC_T1_R0_14        0x85c
    #define SSD_JPEG_REG_AC_T1_R0_15        0x85e

    #define SSD_JPEG_REG_AC_T1_R1_00        0x860
    #define SSD_JPEG_REG_AC_T1_R1_01        0x862
    #define SSD_JPEG_REG_AC_T1_R1_02        0x864
    #define SSD_JPEG_REG_AC_T1_R1_03        0x866
    #define SSD_JPEG_REG_AC_T1_R1_04        0x868
    #define SSD_JPEG_REG_AC_T1_R1_05        0x86a
    #define SSD_JPEG_REG_AC_T1_R1_06        0x86c
    #define SSD_JPEG_REG_AC_T1_R1_07        0x86e
    #define SSD_JPEG_REG_AC_T1_R1_08        0x870
    #define SSD_JPEG_REG_AC_T1_R1_09        0x872
    #define SSD_JPEG_REG_AC_T1_R1_10        0x874
    #define SSD_JPEG_REG_AC_T1_R1_11        0x876
    #define SSD_JPEG_REG_AC_T1_R1_12        0x878
    #define SSD_JPEG_REG_AC_T1_R1_13        0x87a
    #define SSD_JPEG_REG_AC_T1_R1_14        0x87c
    #define SSD_JPEG_REG_AC_T1_R1_15        0x87e
    #define SSD_JPEG_REG_AC_T1_R1_16        0x880
    #define SSD_JPEG_REG_AC_T1_R1_17        0x882
    #define SSD_JPEG_REG_AC_T1_R1_18        0x884
    #define SSD_JPEG_REG_AC_T1_R1_19        0x886
    #define SSD_JPEG_REG_AC_T1_R1_20        0x888
    #define SSD_JPEG_REG_AC_T1_R1_21        0x88a
    #define SSD_JPEG_REG_AC_T1_R1_22        0x88c
    #define SSD_JPEG_REG_AC_T1_R1_23        0x88e
    #define SSD_JPEG_REG_AC_T1_R1_24        0x890
    #define SSD_JPEG_REG_AC_T1_R1_25        0x892
    #define SSD_JPEG_REG_AC_T1_R1_26        0x894
    #define SSD_JPEG_REG_AC_T1_R1_27        0x896
    #define SSD_JPEG_REG_AC_T1_R1_28        0x898
    #define SSD_JPEG_REG_AC_T1_R1_29        0x89a
    #define SSD_JPEG_REG_AC_T1_R1_30        0x89c
    #define SSD_JPEG_REG_AC_T1_R1_31        0x89e
    #define SSD_JPEG_REG_AC_T1_R1_32        0x8a0
    #define SSD_JPEG_REG_AC_T1_R1_33        0x8a2
    #define SSD_JPEG_REG_AC_T1_R1_34        0x8a4
    #define SSD_JPEG_REG_AC_T1_R1_35        0x8a6
    #define SSD_JPEG_REG_AC_T1_R1_36        0x8a8
    #define SSD_JPEG_REG_AC_T1_R1_37        0x8aa
    #define SSD_JPEG_REG_AC_T1_R1_38        0x8ac
    #define SSD_JPEG_REG_AC_T1_R1_39        0x8ae
    #define SSD_JPEG_REG_AC_T1_R1_40        0x8b0
    #define SSD_JPEG_REG_AC_T1_R1_41        0x8b2
    #define SSD_JPEG_REG_AC_T1_R1_42        0x8b4
    #define SSD_JPEG_REG_AC_T1_R1_43        0x8b6
    #define SSD_JPEG_REG_AC_T1_R1_44        0x8b8
    #define SSD_JPEG_REG_AC_T1_R1_45        0x8ba
    #define SSD_JPEG_REG_AC_T1_R1_46        0x8bc
    #define SSD_JPEG_REG_AC_T1_R1_47        0x8be
    #define SSD_JPEG_REG_AC_T1_R1_48        0x8c0
    #define SSD_JPEG_REG_AC_T1_R1_49        0x8c2
    #define SSD_JPEG_REG_AC_T1_R1_50        0x8c4
    #define SSD_JPEG_REG_AC_T1_R1_51        0x8c6
    #define SSD_JPEG_REG_AC_T1_R1_52        0x8c8
    #define SSD_JPEG_REG_AC_T1_R1_53        0x8ca
    #define SSD_JPEG_REG_AC_T1_R1_54        0x8cc
    #define SSD_JPEG_REG_AC_T1_R1_55        0x8ce
    #define SSD_JPEG_REG_AC_T1_R1_56        0x8d0
    #define SSD_JPEG_REG_AC_T1_R1_57        0x8d2
    #define SSD_JPEG_REG_AC_T1_R1_58        0x8d4
    #define SSD_JPEG_REG_AC_T1_R1_59        0x8d6
    #define SSD_JPEG_REG_AC_T1_R1_60        0x8d8
    #define SSD_JPEG_REG_AC_T1_R1_61        0x8da
    #define SSD_JPEG_REG_AC_T1_R1_62        0x8dc
    #define SSD_JPEG_REG_AC_T1_R1_63        0x8de
    #define SSD_JPEG_REG_AC_T1_R1_64        0x8e0
    #define SSD_JPEG_REG_AC_T1_R1_65        0x8e2
    #define SSD_JPEG_REG_AC_T1_R1_66        0x8e4
    #define SSD_JPEG_REG_AC_T1_R1_67        0x8e6
    #define SSD_JPEG_REG_AC_T1_R1_68        0x8e8
    #define SSD_JPEG_REG_AC_T1_R1_69        0x8ea
    #define SSD_JPEG_REG_AC_T1_R1_70        0x8ec
    #define SSD_JPEG_REG_AC_T1_R1_71        0x8ee
    #define SSD_JPEG_REG_AC_T1_R1_72        0x8f0
    #define SSD_JPEG_REG_AC_T1_R1_73        0x8f2
    #define SSD_JPEG_REG_AC_T1_R1_74        0x8f4
    #define SSD_JPEG_REG_AC_T1_R1_75        0x8f6
    #define SSD_JPEG_REG_AC_T1_R1_76        0x8f8
    #define SSD_JPEG_REG_AC_T1_R1_77        0x8fa
    #define SSD_JPEG_REG_AC_T1_R1_78        0x8fc
    #define SSD_JPEG_REG_AC_T1_R1_79        0x8fe
    #define SSD_JPEG_REG_AC_T1_R1_80        0x900
    #define SSD_JPEG_REG_AC_T1_R1_81        0x902
    #define SSD_JPEG_REG_AC_T1_R1_82        0x904
    #define SSD_JPEG_REG_AC_T1_R1_83        0x906
    #define SSD_JPEG_REG_AC_T1_R1_84        0x908
    #define SSD_JPEG_REG_AC_T1_R1_85        0x90a
    #define SSD_JPEG_REG_AC_T1_R1_86        0x90c
    #define SSD_JPEG_REG_AC_T1_R1_87        0x90e
    #define SSD_JPEG_REG_AC_T1_R1_88        0x910
    #define SSD_JPEG_REG_AC_T1_R1_89        0x912
    #define SSD_JPEG_REG_AC_T1_R1_90        0x914
    #define SSD_JPEG_REG_AC_T1_R1_91        0x916
    #define SSD_JPEG_REG_AC_T1_R1_92        0x918
    #define SSD_JPEG_REG_AC_T1_R1_93        0x91a
    #define SSD_JPEG_REG_AC_T1_R1_94        0x91c
    #define SSD_JPEG_REG_AC_T1_R1_95        0x91e
    #define SSD_JPEG_REG_AC_T1_R1_96        0x920
    #define SSD_JPEG_REG_AC_T1_R1_97        0x922
    #define SSD_JPEG_REG_AC_T1_R1_98        0x924
    #define SSD_JPEG_REG_AC_T1_R1_99        0x926
    #define SSD_JPEG_REG_AC_T1_R1_100       0x928
    #define SSD_JPEG_REG_AC_T1_R1_101       0x92a
    #define SSD_JPEG_REG_AC_T1_R1_102       0x92c
    #define SSD_JPEG_REG_AC_T1_R1_103       0x92e
    #define SSD_JPEG_REG_AC_T1_R1_104       0x930
    #define SSD_JPEG_REG_AC_T1_R1_105       0x932
    #define SSD_JPEG_REG_AC_T1_R1_106       0x934
    #define SSD_JPEG_REG_AC_T1_R1_107       0x936
    #define SSD_JPEG_REG_AC_T1_R1_108       0x938
    #define SSD_JPEG_REG_AC_T1_R1_109       0x93a
    #define SSD_JPEG_REG_AC_T1_R1_110       0x93c
    #define SSD_JPEG_REG_AC_T1_R1_111       0x93e
    #define SSD_JPEG_REG_AC_T1_R1_112       0x940
    #define SSD_JPEG_REG_AC_T1_R1_113       0x942
    #define SSD_JPEG_REG_AC_T1_R1_114       0x944
    #define SSD_JPEG_REG_AC_T1_R1_115       0x946
    #define SSD_JPEG_REG_AC_T1_R1_116       0x948
    #define SSD_JPEG_REG_AC_T1_R1_117       0x94a
    #define SSD_JPEG_REG_AC_T1_R1_118       0x94c
    #define SSD_JPEG_REG_AC_T1_R1_119       0x94e
    #define SSD_JPEG_REG_AC_T1_R1_120       0x950
    #define SSD_JPEG_REG_AC_T1_R1_121       0x952
    #define SSD_JPEG_REG_AC_T1_R1_122       0x954
    #define SSD_JPEG_REG_AC_T1_R1_123       0x956
    #define SSD_JPEG_REG_AC_T1_R1_124       0x958
    #define SSD_JPEG_REG_AC_T1_R1_125       0x95a
    #define SSD_JPEG_REG_AC_T1_R1_126       0x95c
    #define SSD_JPEG_REG_AC_T1_R1_127       0x95e
    #define SSD_JPEG_REG_AC_T1_R1_128       0x960
    #define SSD_JPEG_REG_AC_T1_R1_129       0x962
    #define SSD_JPEG_REG_AC_T1_R1_130       0x964
    #define SSD_JPEG_REG_AC_T1_R1_131       0x966
    #define SSD_JPEG_REG_AC_T1_R1_132       0x968
    #define SSD_JPEG_REG_AC_T1_R1_133       0x96a
    #define SSD_JPEG_REG_AC_T1_R1_134       0x96c
    #define SSD_JPEG_REG_AC_T1_R1_135       0x96e
    #define SSD_JPEG_REG_AC_T1_R1_136       0x970
    #define SSD_JPEG_REG_AC_T1_R1_137       0x972
    #define SSD_JPEG_REG_AC_T1_R1_138       0x974
    #define SSD_JPEG_REG_AC_T1_R1_139       0x976
    #define SSD_JPEG_REG_AC_T1_R1_140       0x978
    #define SSD_JPEG_REG_AC_T1_R1_141       0x97a
    #define SSD_JPEG_REG_AC_T1_R1_142       0x97c
    #define SSD_JPEG_REG_AC_T1_R1_143       0x97e
    #define SSD_JPEG_REG_AC_T1_R1_144       0x980
    #define SSD_JPEG_REG_AC_T1_R1_145       0x982
    #define SSD_JPEG_REG_AC_T1_R1_146       0x984
    #define SSD_JPEG_REG_AC_T1_R1_147       0x986
    #define SSD_JPEG_REG_AC_T1_R1_148       0x988
    #define SSD_JPEG_REG_AC_T1_R1_149       0x98a
    #define SSD_JPEG_REG_AC_T1_R1_150       0x98c
    #define SSD_JPEG_REG_AC_T1_R1_151       0x98e
    #define SSD_JPEG_REG_AC_T1_R1_152       0x990
    #define SSD_JPEG_REG_AC_T1_R1_153       0x992
    #define SSD_JPEG_REG_AC_T1_R1_154       0x994
    #define SSD_JPEG_REG_AC_T1_R1_155       0x996
    #define SSD_JPEG_REG_AC_T1_R1_156       0x998
    #define SSD_JPEG_REG_AC_T1_R1_157       0x99a
    #define SSD_JPEG_REG_AC_T1_R1_158       0x99c
    #define SSD_JPEG_REG_AC_T1_R1_159       0x99e
    #define SSD_JPEG_REG_AC_T1_R1_160       0x9a0
    #define SSD_JPEG_REG_AC_T1_R1_161       0x9a2

    #define SSD_JPEG_REG_QTABLE_CONST_0     0x9b0
    #define SSD_JPEG_REG_QTABLE_CONST_1     0x9b2
    #define SSD_JPEG_REG_QTABLE_CONST_2     0x9b4
    #define SSD_JPEG_REG_QTABLE_CONST_3     0x9b6

    #define SSD_JPEG_REG_QTABLE0_SAMPLE     0x9b8
    #define SSD_JPEG_REG_QTABLE1_SAMPLE     0x9bc
    #define SSD_JPEG_REG_QTABLE2_SAMPLE     0x9c0

    #define SSD_JPEG_REG_DRI_CONST_0        0x9c4
    #define SSD_JPEG_REG_DRI_CONST_1        0x9c6
    #define SSD_JPEG_REG_DRI_CONST_2        0x9c8
    #define SSD_JPEG_REG_DRI_CONST_3        0x9ca

    #define SSD_JPEG_REG_SOS_CONST_0        0x9cc
    #define SSD_JPEG_REG_SOS_CONST_1        0x9ce
    #define SSD_JPEG_REG_SOS_CONST_2        0x9d0
    #define SSD_JPEG_REG_SOS_CONST_3        0x9d2
    #define SSD_JPEG_REG_SOS_CONST_4        0x9d4

    #define SSD_JPEG_REG_EOI_CONST_0        0x9e4
    #define SSD_JPEG_REG_EOI_CONST_1        0x9e6
    #define SSD_JPEG_REG_EOI_CONST_2        0x9e8
    #define SSD_JPEG_REG_EOI_CONST_3        0x9ea
    #define SSD_JPEG_REG_EOI_CONST_4        0x9ec

    #define SSD_JPEG_REG_VERT_PIX_SIZE0     0x9f0
    #define SSD_JPEG_REG_VERT_PIX_SIZE1     0x9f2
    #define SSD_JPEG_REG_HORI_PIX_SIZE0     0x9f4
    #define SSD_JPEG_REG_HORI_PIX_SIZE1     0x9f6
    #define SSD_JPEG_REG_DRI_CONFIG0        0x9f8
    #define SSD_JPEG_REG_DRI_CONFIG1        0x9fa
#endif // _SSD_JPEG_DECODER_HEADER_FILE
