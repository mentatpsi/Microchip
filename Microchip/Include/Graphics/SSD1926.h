/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Solomon Systech. SSD1926 LCD controllers driver
 *  to be used with GFX 3 PICtail board
 *****************************************************************************
 * FileName:        SSD1926.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
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
 *
 * Date			Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 08/27/08		...
 * 30/07/09		Added Palette Support
 * 03/03/10		Enabled accelerated Circle() function.
 * 02/14/11     - for Graphics Library Version 3.00
 *              - Removed USE_DRV_xxxx switches. This is not needed anymore
 *                since Primitive Lyer implements weak attributes on 
 *                Primitive Routines that can be implemented in hardware.
 *              - Removed Circle Function implementation (SSD1926 bug)
 *              - Moved common APIs to DisplayDriver.h. DisplayDriver.h
 *                is now a common file that should be included in the
 *                the driver source code.
 *              - Added #define GFX_LCD_TFT  0x01 // Type TFT Display 
 *                      #define GFX_LCD_CSTN 0x03 // Type Color STN Display 
 *                      #define GFX_LCD_MSTN 0x02 // Type Mono STN Display 
 *                to select type of display panel.
 *****************************************************************************/
 
#ifndef _SSD1926_H
    #define _SSD1926_H

    #include "HardwareProfile.h"
    #include "GenericTypeDefs.h"

    #ifdef USE_PALETTE
        #include "Graphics/Palette.h"
    #endif

/*********************************************************************
* Error Checking
*********************************************************************/
    #ifndef DISP_HOR_RESOLUTION
        #error DISP_HOR_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_VER_RESOLUTION
        #error DISP_VER_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_ORIENTATION
        #error DISP_ORIENTATION must be defined in HardwareProfile.h
    #endif

/*********************************************************************
* Overview: Horizontal synchronization timing in pixels
*                  (from the glass datasheet).
*********************************************************************/

    #ifndef DISP_HOR_PULSE_WIDTH
        #error DISP_HOR_PULSE_WIDTH must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_HOR_BACK_PORCH
        #error DISP_HOR_BACK_PORCH must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_HOR_FRONT_PORCH
        #error DISP_HOR_FRONT_PORCH must be defined in HardwareProfile.h
    #endif

/*********************************************************************
* Overview: Vertical synchronization timing in lines
*                  (from the glass datasheet).
*********************************************************************/
    #ifndef DISP_VER_PULSE_WIDTH
        #error DISP_VER_PULSE_WIDTH must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_VER_BACK_PORCH
        #error DISP_VER_BACK_PORCH must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_VER_FRONT_PORCH
        #error DISP_VER_FRONT_PORCH must be defined in HardwareProfile.h
    #endif

/*********************************************************************
* PARAMETERS VALIDATION
*********************************************************************/
    #if COLOR_DEPTH != 16
        #error This driver supports 16 BPP only.
    #endif
    #if (DISP_HOR_RESOLUTION % 8) != 0
        #error Horizontal resolution must be divisible by 8.
    #endif
    #if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180) && (DISP_ORIENTATION != 90) && (DISP_ORIENTATION != 270)
        #error The display orientation selected is not supported. It can be only 0,90,180 or 270.
    #endif

/*********************************************************************
* Function:  void SetReg(WORD index, BYTE value);
*
* PreCondition: none
*
* Input: index - register number
*        value - data to be written to the register
*
* Output: none
*
* Side Effects: none
*
* Overview: Writes the given value to the register defined by index.
*
* Note: none
*
********************************************************************/
void            SetReg(WORD index, BYTE value);


/*********************************************************************
* Function:  BYTE GetReg(WORD index)
*
* PreCondition: none
*
* Input: index - register number
*
* Output: none
*
* Side Effects: none
*
* Overview: returns graphics controller register value (byte access)
*
* Note: none
*
********************************************************************/
BYTE            GetReg(WORD index);

/*********************************************************************
* Function:  void DisplayBrightness(WORD level)
*
* PreCondition: none
*
* Input: level - Brightness level. Valid values are 0 to 100.
*			- 0: brightness level is zero or display is turned off
*			- 1: brightness level is maximum 
*
* Output: none
*
* Side Effects: none
*
* Overview: Sets the brightness of the display.
*
* Note: none
*
********************************************************************/
void            DisplayBrightness(WORD level);

/*********************************************************************
* Overview: SSD1926 registers definitions.
*********************************************************************/
    #define REG_PLL_CONFIG_0                    0x126
    #define REG_PLL_CONFIG_1                    0x127
    #define REG_PLL_CONFIG_2                    0x12b

    #define REG_DISP_BUFFER_SIZE                0x01
    #define REG_CONFIG_READBACK                 0x02
    #define REG_REVISION_CODE                   0x03
    #define REG_MEMCLK_CONFIG                   0x04
    #define REG_PCLK_CONFIG                     0x05
    #define REG_VCLK_CONFIG_0                   0x06
    #define REG_VCLK_CONFIG_1                   0x07
    #define REG_LUT_BLUE_WRITE_DATA             0x08
    #define REG_LUT_GREEN_WRITE_DATA            0x09
    #define REG_LUT_RED_WRITE_DATA              0x0a
    #define REG_LUT_WRITE_ADDR                  0x0b
    #define REG_LUT_BLUE_READ_DATA              0x0c
    #define REG_LUT_GREEN_READ_DATA             0x0d
    #define REG_LUT_RED_READ_DATA               0x0e
    #define REG_LUT_READ_ADDR                   0x0f
    #define REG_PANEL_TYPE                      0x10
    #define REG_MOD_RATE                        0x11
    #define REG_HORIZ_TOTAL_0                   0x12
    #define REG_HORIZ_TOTAL_1                   0x13
    #define REG_HDP                             0x14
    #define REG_HDP_START_POS0                  0x16
    #define REG_HDP_START_POS1                  0x17
    #define REG_VERT_TOTAL0                     0x18
    #define REG_VERT_TOTAL1                     0x19
    #define REG_VDP0                            0x1c
    #define REG_VDP1                            0x1d
    #define REG_VDP_START_POS0                  0x1e
    #define REG_VDP_START_POS1                  0x1f
    #define REG_HSYNC_PULSE_WIDTH               0x20
    #define REG_LLINE_PULSE_START_SUBPIXEL_POS  0x21
    #define REG_HSYNC_PULSE_START_POS0          0x22
    #define REG_HSYNC_PULSE_START_POS1          0x23
    #define REG_VSYNC_PULSE_WIDTH               0x24
    #define REG_VSYNC_PULSE_START_POS0          0x26
    #define REG_VSYNC_PULSE_START_POS1          0x27
    #define REG_POST_PROCESSING_SATURATION      0x2c
    #define REG_POST_PROCESSING_BRIGHTNESS      0x2d
    #define REG_POST_PROCESSING_CONTRAST        0x2e
    #define REG_POST_PROCESSING_CTRL            0x2f

    #define REG_FPFRAME_START_OFFSET0           0x30
    #define REG_FPFRAME_START_OFFSET1           0x31
    #define REG_FPFRAME_STOP_OFFSET0            0x34
    #define REG_FPFRAME_STOP_OFFSET1            0x35
    #define REG_LSHIFT_POLARITY                 0x38
    #define REG_GPIO1_PULSE_START               0x3c
    #define REG_GPIO1_PULSE_STOP                0x3e
    #define REG_GPIO2_PULSE_DELAY               0x40
    #define REG_LCD_SUBPIXEL_ALIGNMENT          0x42
    #define REG_STN_COLOR_DEPTH                 0x45

    #define REG_INTERRUPT_FLAG                  0x48
    #define REG_INTERRUPT_ENABLE                0x4A

    #define REG_DYN_DITHER_CONTROL              0x50

    #define REG_DISPLAY_MODE                    0x70
    #define REG_SPECIAL_EFFECTS                 0x71
    #define REG_RGB_SETTING                     0x1a4

    #define REG_MAIN_WIN_DISP_START_ADDR0       0x74
    #define REG_MAIN_WIN_DISP_START_ADDR1       0x75
    #define REG_MAIN_WIN_DISP_START_ADDR2       0x76
    #define REG_MAIN_WIN_ADDR_OFFSET0           0x78
    #define REG_MAIN_WIN_ADDR_OFFSET1           0x79
    #define REG_FLOAT_WIN_DISP_START_ADDR0      0x7c
    #define REG_FLOAT_WIN_DISP_START_ADDR1      0x7d
    #define REG_FLOAT_WIN_DISP_START_ADDR2      0x7e
    #define REG_FLOAT_WIN_ADDR_OFFSET0          0x80
    #define REG_FLOAT_WIN_ADDR_OFFSET1          0x81
    #define REG_FLOAT_WIN_X_START_POS0          0x84
    #define REG_FLOAT_WIN_X_START_POS1          0x85
    #define REG_FLOAT_WIN_Y_START_POS0          0x88
    #define REG_FLOAT_WIN_Y_START_POS1          0x89
    #define REG_FLOAT_WIN_X_END_POS0            0x8c
    #define REG_FLOAT_WIN_X_END_POS1            0x8d
    #define REG_FLOAT_WIN_Y_END_POS0            0x90
    #define REG_FLOAT_WIN_Y_END_POS1            0x91
    #define REG_POWER_SAVE_CONFIG               0xa0
    #define REG_SOFTWARE_RESET                  0xa2
    #define REG_SCRATCH_PAD0                    0xa4
    #define REG_SCRATCH_PAD1                    0xa5
    #define REG_GPIO_CONFIG0                    0xa8
    #define REG_GPIO_CONFIG1                    0xa9
    #define REG_GPIO_STATUS_CONTROL0            0xac
    #define REG_GPIO_STATUS_CONTROL1            0xad
    #define REG_PWM_CV_CLOCK_CONTROL            0xb0
    #define REG_PWM_CV_CLOCK_CONFIG             0xb1
    #define REG_CV_CLOCK_BURST_LENGTH           0xb2
    #define REG_PWM_CLOCK_DUTY_CYCLE            0xb3

    #define REG_PWM1_CLOCK_CONTROL              0xb4
    #define REG_PWM1_CLOCK_CONFIG               0xb5
    #define REG_PWM1_CLOCK_DUTY_CYCLE           0xb7

    #define REG_PWM2_CLOCK_CONTROL              0xb8
    #define REG_PWM2_CLOCK_CONFIG               0xb9
    #define REG_PWM2_CLOCK_DUTY_CYCLE           0xbb

    #define REG_PWM3_CLOCK_CONTROL              0xbc
    #define REG_PWM3_CLOCK_CONFIG               0xbd
    #define REG_PWM3_CLOCK_DUTY_CYCLE           0xbf

    #define REG_CURSOR_FEATURE                  0xc0
    #define REG_CURSOR1_BLINK_TOTAL0            0xc4
    #define REG_CURSOR1_BLINK_TOTAL1            0xc5
    #define REG_CURSOR1_BLINK_ON0               0xc8
    #define REG_CURSOR1_BLINK_ON1               0xc9
    #define REG_CURSOR1_MEM_START0              0xcc
    #define REG_CURSOR1_MEM_START1              0xcd
    #define REG_CURSOR1_MEM_START2              0xce
    #define REG_CURSOR1_POSX0                   0xd0
    #define REG_CURSOR1_POSX1                   0xd1
    #define REG_CURSOR1_POSY0                   0xd4
    #define REG_CURSOR1_POSY1                   0xd5
    #define REG_CURSOR1_HORIZ_SIZE_0            0xd8
    #define REG_CURSOR1_HORIZ_SIZE_1            0xd9
    #define REG_CURSOR1_VERT_SIZE_0             0xdc
    #define REG_CURSOR1_VERT_SIZE_1             0xdd
    #define REG_CURSOR1_COL_IND1_0              0xe0
    #define REG_CURSOR1_COL_IND1_1              0xe1
    #define REG_CURSOR1_COL_IND1_2              0xe2
    #define REG_CURSOR1_COL_IND1_3              0xe3
    #define REG_CURSOR1_COL_IND2_0              0xe4
    #define REG_CURSOR1_COL_IND2_1              0xe5
    #define REG_CURSOR1_COL_IND2_2              0xe6
    #define REG_CURSOR1_COL_IND2_3              0xe7
    #define REG_CURSOR1_COL_IND3_0              0xe8
    #define REG_CURSOR1_COL_IND3_1              0xe9
    #define REG_CURSOR1_COL_IND3_2              0xea
    #define REG_CURSOR1_COL_IND3_3              0xeb
    #define REG_CURSOR2_BLINK_TOTAL0            0xec
    #define REG_CURSOR2_BLINK_TOTAL1            0xed
    #define REG_CURSOR2_BLINK_ON0               0xf0
    #define REG_CURSOR2_BLINK_ON1               0xf1
    #define REG_CURSOR2_MEM_START0              0xf4
    #define REG_CURSOR2_MEM_START1              0xf5
    #define REG_CURSOR2_MEM_START2              0xf6
    #define REG_CURSOR2_POSX0                   0xf8
    #define REG_CURSOR2_POSX1                   0xf9
    #define REG_CURSOR2_POSY0                   0xfc
    #define REG_CURSOR2_POSY1                   0xfd
    #define REG_CURSOR2_HORIZ_SIZE_0            0x100
    #define REG_CURSOR2_HORIZ_SIZE_1            0x101
    #define REG_CURSOR2_VERT_SIZE_0             0x104
    #define REG_CURSOR2_VERT_SIZE_1             0x105
    #define REG_CURSOR2_COL_IND1_0              0x108
    #define REG_CURSOR2_COL_IND1_1              0x109
    #define REG_CURSOR2_COL_IND1_2              0x10a
    #define REG_CURSOR2_COL_IND1_3              0x10b
    #define REG_CURSOR2_COL_IND2_0              0x10c
    #define REG_CURSOR2_COL_IND2_1              0x10d
    #define REG_CURSOR2_COL_IND2_2              0x10e
    #define REG_CURSOR2_COL_IND2_3              0x10f
    #define REG_CURSOR2_COL_IND3_0              0x110
    #define REG_CURSOR2_COL_IND3_1              0x111
    #define REG_CURSOR2_COL_IND3_2              0x112
    #define REG_CURSOR2_COL_IND3_3              0x113

    #define REG_MAIN_REFLESH                    0x12c

    #define REG_PCLK_FREQ_RATIO_0               0x158
    #define REG_PCLK_FREQ_RATIO_1               0x159
    #define REG_PCLK_FREQ_RATIO_2               0x15a

    #define REG_DV_OP_MODE                      0x160
    #define REG_DV_FRAME_SAMPLING               0x161

    #define REG_DV_NFRAME_POS_0                 0x162
    #define REG_DV_NFRAME_POS_1                 0x163
    #define REG_DV_JHORI_SIZE_0                 0x164
    #define REG_DV_JHORI_SIZE_1                 0x165
    #define REG_DV_JVERT_SIZE_0                 0x168
    #define REG_DV_JVERT_SIZE_1                 0x169

    #define REG_DV_JMEM_STR_0                   0x16c
    #define REG_DV_JMEM_STR_1                   0x16d
    #define REG_DV_JMEM_STR_2                   0x16e
    #define REG_DV_JMEM_STR_3                   0x16f

    #define REG_DV_VHDEC_RATIO                  0x170
    #define REG_DV_VVDEC_RATIO                  0x171
    #define REG_DV_JHDEC_RATIO                  0x172
    #define REG_DV_JVDEC_RATIO                  0x173

    #define REG_DV_HORI_PERIOD_0                0x174
    #define REG_DV_HORI_PERIOD_1                0x175

    #define REG_DV_HORI_MAX_0                   0x17c
    #define REG_DV_HORI_MAX_1                   0x17d

    #define REG_DV_VERT_MAX_0                   0x180
    #define REG_DV_VERT_MAX_1                   0x181

    #define REG_DV_HCROP_STR_0                  0x184   //x
    #define REG_DV_HCROP_STR_1                  0x185   //x
    #define REG_DV_VCROP_STR_0                  0x188   //x
    #define REG_DV_VCROP_STR_1                  0x189   //x
    #define REG_DV_HCROP_SIZE_0                 0x18c
    #define REG_DV_HCROP_SIZE_1                 0x18d

    #define REG_DV_VCROP_SIZE_0                 0x190
    #define REG_DV_VCROP_SIZE_1                 0x191

    #define REG_DV_FRAME_PULSE_WIDTH            0x194
    #define REG_DV_VMEM_STR_ADDR1_0             0x19c
    #define REG_DV_VMEM_STR_ADDR1_1             0x19d
    #define REG_DV_VMEM_STR_ADDR1_2             0x19e
    #define REG_DV_VMEM_STR_ADDR1_3             0x19f

    #define REG_DV_VMEM_STR_ADDR2_0             0x1a0
    #define REG_DV_VMEM_STR_ADDR2_1             0x1a1
    #define REG_DV_VMEM_STR_ADDR2_2             0x1a2
    #define REG_DV_VMEM_STR_ADDR2_3             0x1a3

    #define REG_DV_OFORMAT                      0x1a4
    #define REG_DV_ALPHA                        0x1a5
    #define REG_DV_SUBPIXEL_MODE                0x1a6
    #define REG_DV_CSC_MODE                     0x1a8
    #define REG_DV_Y                            0x1a9
    #define REG_DV_CB                           0x1aa
    #define REG_DV_CR                           0x1ab

    #define REG_DV_TV_0                         0x1ac
    #define REG_DV_TV_1                         0x1ad
    #define REG_DV_TV_2                         0x1ae

    #define REG_DV_ENB                          0x1af

    #define REG_DV_DV0_START_ADDR_0             0x1b0
    #define REG_DV_DV0_START_ADDR_1             0x1b1
    #define REG_DV_DV0_START_ADDR_2             0x1b2

    #define REG_DV_DV1_START_ADDR_0             0x1b4
    #define REG_DV_DV1_START_ADDR_1             0x1b5
    #define REG_DV_DV1_START_ADDR_2             0x1b6

    #define REG_2D_1d0                      0x1d0
    #define REG_2D_1d1                      0x1d1
    #define REG_2D_1d2                      0x1d2
    #define REG_2D_1d4                      0x1d4
    #define REG_2D_1d5                      0x1d5
    #define REG_2D_1d6                      0x1d6
    #define REG_2D_1d8                      0x1d8
    #define REG_2D_1d9                      0x1d9
    #define REG_2D_1dc                      0x1dc
    #define REG_2D_1dd                      0x1dd
    #define REG_2D_1de                      0x1de
    #define REG_2D_1e4                      0x1e4
    #define REG_2D_1e5                      0x1e5
    #define REG_2D_1e8                      0x1e8
    #define REG_2D_1e9                      0x1e9
    #define REG_2D_1ec                      0x1ec
    #define REG_2D_1ed                      0x1ed
    #define REG_2D_1f0                      0x1f0
    #define REG_2D_1f1                      0x1f1
    #define REG_2D_1f4                      0x1f4
    #define REG_2D_1f5                      0x1f5
    #define REG_2D_1f6                      0x1f6
    #define REG_2D_1f8                      0x1f8
    #define REG_2D_1f9                      0x1f9
    #define REG_2D_1fc                      0x1fc
    #define REG_2D_1fd                      0x1fd
    #define REG_2D_1fe                      0x1fe
    #define REG_2D_204                      0x204
    #define REG_2D_205                      0x205
    #define REG_2D_206                      0x206
    #define REG_2D_208                      0x208
    #define REG_2D_209                      0x209
    #define REG_2D_214                      0x214
    #define REG_2D_215                      0x215
    #define REG_2D_218                      0x218
    #define REG_2D_219                      0x219
    #define REG_2D_220                      0x220

    #define REG_2D_CMD_1                    0x1d0
    #define REG_2D_CMD_2                    0x1d1
    #define REG_2D_CMD_FIFO_STATUS          0x1d2
    #define REG_2D_SRC_WND_START_ADDR0      0x1d4
    #define REG_2D_SRC_WND_START_ADDR1      0x1d5
    #define REG_2D_SRC_WND_START_ADDR2      0x1d6
    #define REG_2D_SRC_WND_ADDR_OFFSET0     0x1d8
    #define REG_2D_SRC_WND_ADDR_OFFSET1     0x1d9
    #define REG_2D_SRC_WND_COLOR_MODE       0x1dc
    #define REG_2D_DEST_WND_COLOR_MODE      0x1dd
    #define REG_2D_SRC_WND_WIDTH0           0x1e4
    #define REG_2D_SRC_WND_WIDTH1           0x1e5
    #define REG_2D_SRC_WND_HEIGHT0          0x1e8
    #define REG_2D_SRC_WND_HEIGHT1          0x1e9
    #define REG_2D_DEST_WND_WIDTH0          0x1ec
    #define REG_2D_DEST_WND_WIDTH1          0x1ed
    #define REG_2D_DEST_WND_HEIGHT0         0x1f0
    #define REG_2D_DEST_WND_HEIGHT1         0x1f1
    #define REG_2D_DEST_WND_START_ADDR0     0x1f4
    #define REG_2D_DEST_WND_START_ADDR1     0x1f5
    #define REG_2D_DEST_WND_START_ADDR2     0x1f6
    #define REG_2D_DEST_WND_ADDR_OFFSET0    0x1f8
    #define REG_2D_DEST_WND_ADDR_OFFSET1    0x1f9
    #define REG_2D_WRTIE_PATTERN0           0x1fc
    #define REG_2D_WRTIE_PATTERN1           0x1fd
    #define REG_2D_WRTIE_PATTERN2           0x1fe
    #define REG_2D_BRUSH_WND_START_ADDR0    0x204
    #define REG_2D_BRUSH_WND_START_ADDR1    0x205
    #define REG_2D_BRUSH_WND_START_ADDR2    0x206
    #define REG_2D_BRUSH_WND_ADDR_OFFSET0   0x208
    #define REG_2D_BRUSH_WND_ADDR_OFFSET1   0x209
    #define REG_2D_BRUSH_WND_WIDTH0         0x214
    #define REG_2D_BRUSH_WND_WIDTH1         0x215
    #define REG_2D_BRUSH_WND_HEIGHT0        0x218
    #define REG_2D_BRUSH_WND_HEIGHT1        0x219
    #define REG_2D_CMD_FIFO_INT_EN          0x21c
    #define REG_2D_CMD_FIFO_INT_STATUS      0x21e
    #define REG_2D_CMD_FIFO_FLAG            0x220
    #define REG_2D_CMD_FIFO_IND             0x222

    #define REG_I2C_DATA_OUT                0x230
    #define REG_I2C_CTL                     0x231
    #define REG_I2C_EN_OUT                  0x232
    #define REG_I2C_BAUD                    0x233
    #define REG_I2C_STATUS                  0x234
    #define REG_I2C_INTERRUPT               0x235
    #define REG_I2C_DATA_READY              0x236
    #define REG_I2C_DATA_IN                 0x237

    #define REG_Sub_mode                    0x250
    #define REG_Sub_CLK_Divide              0x252
    #define REG_Sub_Hori_Size               0x254
    #define REG_Sub_Vert_Size               0x25c
    #define REG_Sub_Type                    0x260
    #define REG_Sub_Bpp                     0x261
    #define REG_Sub_TFT_Start               0x262
    #define REG_Sub_SCLK_Divide             0x263

    #define REG_Sub_Ctl_0                   0x270
    #define REG_Sub_Ctl_1                   0x271
    #define REG_Sub_YVU                     0x268
    #define REG_Sub_Y_Offset                0x269
    #define REG_Sub_Cb_Offset               0x26a
    #define REG_Sub_Cr_Offset               0x26b
    #define REG_Sub_Data_0                  0x26c
    #define REG_Sub_Data_1                  0x26d
    #define REG_Sub_RS                      0x26e
    #define REG_Sub_Start_Adr_0             0x274
    #define REG_Sub_Start_Adr_1             0x275
    #define REG_Sub_Start_Adr_2             0x276
    #define REG_Sub_Offset_0                0x278
    #define REG_Sub_Offset_1                0x279
    #define REG_Sub_Ready                   0x27d

    #define REG_LCD0_Rise_0                 0x2b0
    #define REG_LCD0_Rise_1                 0x2b1
    #define REG_LCD0_Fall_0                 0x2b4
    #define REG_LCD0_Fall_1                 0x2b5
    #define REG_LCD0_Period_0               0x2b8
    #define REG_LCD0_Period_1               0x2b9
    #define REG_LCD0_Ctl_0                  0x2bc
    #define REG_LCD0_Ctl_1                  0x2bd
    #define REG_LCD1_Rise_0                 0x2c0
    #define REG_LCD1_Rise_1                 0x2c1
    #define REG_LCD1_Fall_0                 0x2c4
    #define REG_LCD1_Fall_1                 0x2c5
    #define REG_LCD1_Period_0               0x2c8
    #define REG_LCD1_Period_1               0x2c9
    #define REG_LCD1_Ctl_0                  0x2cc
    #define REG_LCD1_Ctl_1                  0x2cd
    #define REG_LCD2_Rise_0                 0x2d0
    #define REG_LCD2_Rise_1                 0x2d1
    #define REG_LCD2_Fall_0                 0x2d4
    #define REG_LCD2_Fall_1                 0x2d5
    #define REG_LCD2_Period_0               0x2d8
    #define REG_LCD2_Period_1               0x2d9
    #define REG_LCD2_Ctl_0                  0x2dc
    #define REG_LCD2_Ctl_1                  0x2dd
    #define REG_LCD3_Rise_0                 0x2e0
    #define REG_LCD3_Rise_1                 0x2e1
    #define REG_LCD3_Fall_0                 0x2e4
    #define REG_LCD3_Fall_1                 0x2e5
    #define REG_LCD3_Period_0               0x2e8
    #define REG_LCD3_Period_1               0x2e9
    #define REG_LCD3_Ctl_0                  0x2ec
    #define REG_LCD3_Ctl_1                  0x2ed
    #define REG_LCD4_Rise_0                 0x2f0
    #define REG_LCD4_Rise_1                 0x2f1
    #define REG_LCD4_Fall_0                 0x2f4
    #define REG_LCD4_Fall_1                 0x2f5
    #define REG_LCD4_Period_0               0x2f8
    #define REG_LCD4_Period_1               0x2f9
    #define REG_LCD4_Ctl_0                  0x2fc
    #define REG_LCD4_Ctl_1                  0x2fd
    #define REG_LCD5_Rise_0                 0x300
    #define REG_LCD5_Rise_1                 0x301
    #define REG_LCD5_Fall_0                 0x304
    #define REG_LCD5_Fall_1                 0x305
    #define REG_LCD5_Period_0               0x308
    #define REG_LCD5_Period_1               0x309
    #define REG_LCD5_Ctl_0                  0x30c
    #define REG_LCD5_Ctl_1                  0x30d
    #define REG_LCD6_Rise_0                 0x310
    #define REG_LCD6_Rise_1                 0x311
    #define REG_LCD6_Fall_0                 0x314
    #define REG_LCD6_Fall_1                 0x315
    #define REG_LCD6_Period_0               0x318
    #define REG_LCD6_Period_1               0x319
    #define REG_LCD6_Ctl_0                  0x31c
    #define REG_LCD6_Ctl_1                  0x31d
    #define REG_LCD7_Rise_0                 0x320
    #define REG_LCD7_Rise_1                 0x321
    #define REG_LCD7_Fall_0                 0x324
    #define REG_LCD7_Fall_1                 0x325
    #define REG_LCD7_Period_0               0x328
    #define REG_LCD7_Period_1               0x329
    #define REG_LCD7_Ctl_0                  0x32c
    #define REG_LCD7_Ctl_1                  0x32d

    #define REG_FRC_FRAME_CTL               0x334
    #define REG_FRC_ENABLE                  0x336

    #define REG_JPEG_RESIZER_CTL            0x360
    #define REG_JPEG_RESIZER_STARTX_0       0x364
    #define REG_JPEG_RESIZER_STARTX_1       0x365
    #define REG_JPEG_RESIZER_STARTY_0       0x366
    #define REG_JPEG_RESIZER_STARTY_1       0x367
    #define REG_JPEG_RESIZER_ENDX_0         0x368
    #define REG_JPEG_RESIZER_ENDX_1         0x369
    #define REG_JPEG_RESIZER_ENDY_0         0x36a
    #define REG_JPEG_RESIZER_ENDY_1         0x36b
    #define REG_JPEG_RESIZER_OP_0           0x36c
    #define REG_JPEG_RESIZER_OP_1           0x36e

    #define REG_JPEG_CTRL                   0x380
    #define REG_JPEG_STATUS                 0x382
    #define REG_JPEG_STATUS1                0x383

    #define REG_JPEG_RAW_STATUS             0x384
    #define REG_JPEG_RAW_STATUS1            0x385

    #define REG_JPEG_INTR_CTL0              0x386
    #define REG_JPEG_INTR_CTL1              0x387

    #define REG_JPEG_START_STOP             0x38a

    #define REG_JPEG_FIFO_CTL               0x3a0
    #define REG_JPEG_FIFO_STATUS            0x3a2
    #define REG_JPEG_FIFO_SIZE              0x3a4

    #define REG_JPEG_ENCODE_SIZE_LIMIT_0    0x3B0
    #define REG_JPEG_ENCODE_SIZE_LIMIT_1    0x3B1
    #define REG_JPEG_ENCODE_SIZE_LIMIT_2    0x3B2

    #define REG_JPEG_ENCODE_SIZE_RESULT_0   0x3b4
    #define REG_JPEG_ENCODE_SIZE_RESULT_1   0x3b5
    #define REG_JPEG_ENCODE_SIZE_RESULT_2   0x3b6

    #define REG_JPEG_FILE_SIZE_0            0x3B8
    #define REG_JPEG_FILE_SIZE_1            0x3B9
    #define REG_JPEG_FILE_SIZE_2            0x3BA

    #define REG_JPEG_DECODE_X_SIZE          0x3d8
    #define REG_JPEG_DECODE_Y_SIZE          0x3dc

    #define REG_JPEG_OP_MODE_ENC            0x400
    #define REG_JPEG_OP_MODE                0x401

    #define REG_JPEG_CMD                    0x402

    #define REG_DRI_SETTING                 0x40a

    #define REG_JPEG_DECODE_VALUE           0x404

    #define REG_JPEG_Y_PIXEL_SIZE_0         0x40c
    #define REG_JPEG_Y_PIXEL_SIZE_1         0x40d
    #define REG_JPEG_X_PIXEL_SIZE_0         0x40e
    #define REG_JPEG_X_PIXEL_SIZE_1         0x40f

    #define REG_JPEG_SRC_START_ADDR_0       0x410
    #define REG_JPEG_SRC_START_ADDR_1       0x411
    #define REG_JPEG_SRC_START_ADDR_2       0x412
    #define REG_JPEG_SRC_START_ADDR_3       0x413

    #define REG_JPEG_DEST_START_ADDR_0      0x414
    #define REG_JPEG_DEST_START_ADDR_1      0x415
    #define REG_JPEG_DEST_START_ADDR_2      0x416
    #define REG_JPEG_DEST_START_ADDR_3      0x417

    #define REG_JPEG_RST_MARKER             0x41c

    #define REG_JPEG_RST_MARKER_STATUS      0x41e

    #define REG_JPEG_INSERT_MARKER00        0x420
    #define REG_JPEG_INSERT_MARKER01        0x422
    #define REG_JPEG_MARKER_LENGTH00        0x424
    #define REG_JPEG_MARKER_LENGTH01        0x426

    #define REG_JPEG_MARKER_DATA_00         0x428
    #define REG_JPEG_MARKER_DATA_01         0x42a
    #define REG_JPEG_MARKER_DATA_02         0x42c
    #define REG_JPEG_MARKER_DATA_03         0x42e
    #define REG_JPEG_MARKER_DATA_04         0x430
    #define REG_JPEG_MARKER_DATA_05         0x432
    #define REG_JPEG_MARKER_DATA_06         0x434
    #define REG_JPEG_MARKER_DATA_07         0x436
    #define REG_JPEG_MARKER_DATA_08         0x438
    #define REG_JPEG_MARKER_DATA_09         0x43a
    #define REG_JPEG_MARKER_DATA_10         0x43c
    #define REG_JPEG_MARKER_DATA_11         0x43e
    #define REG_JPEG_MARKER_DATA_12         0x440
    #define REG_JPEG_MARKER_DATA_13         0x442
    #define REG_JPEG_MARKER_DATA_14         0x444
    #define REG_JPEG_MARKER_DATA_15         0x446
    #define REG_JPEG_MARKER_DATA_16         0x448
    #define REG_JPEG_MARKER_DATA_17         0x44a
    #define REG_JPEG_MARKER_DATA_18         0x44c
    #define REG_JPEG_MARKER_DATA_19         0x44e
    #define REG_JPEG_MARKER_DATA_20         0x450
    #define REG_JPEG_MARKER_DATA_21         0x452
    #define REG_JPEG_MARKER_DATA_22         0x454
    #define REG_JPEG_MARKER_DATA_23         0x456
    #define REG_JPEG_MARKER_DATA_24         0x458
    #define REG_JPEG_MARKER_DATA_25         0x45a
    #define REG_JPEG_MARKER_DATA_26         0x45c
    #define REG_JPEG_MARKER_DATA_27         0x45e
    #define REG_JPEG_MARKER_DATA_28         0x460
    #define REG_JPEG_MARKER_DATA_29         0x462
    #define REG_JPEG_MARKER_DATA_30         0x464
    #define REG_JPEG_MARKER_DATA_31         0x466

    #define REG_JPEG_SOI_CONST_00           0x468
    #define REG_JPEG_SOI_CONST_01           0x46a

    #define REG_JPEG_JFIF_CONST_00          0x46c
    #define REG_JPEG_JFIF_CONST_01          0x46e
    #define REG_JPEG_JFIF_CONST_02          0x470
    #define REG_JPEG_JFIF_CONST_03          0x472
    #define REG_JPEG_JFIF_CONST_04          0x474
    #define REG_JPEG_JFIF_CONST_05          0x476
    #define REG_JPEG_JFIF_CONST_06          0x478
    #define REG_JPEG_JFIF_CONST_07          0x47a
    #define REG_JPEG_JFIF_CONST_08          0x47c
    #define REG_JPEG_JFIF_CONST_09          0x47e
    #define REG_JPEG_JFIF_CONST_10          0x480
    #define REG_JPEG_JFIF_CONST_11          0x482
    #define REG_JPEG_JFIF_CONST_12          0x484
    #define REG_JPEG_JFIF_CONST_13          0x486
    #define REG_JPEG_JFIF_CONST_14          0x488
    #define REG_JPEG_JFIF_CONST_15          0x48a
    #define REG_JPEG_JFIF_CONST_16          0x48c
    #define REG_JPEG_JFIF_CONST_17          0x48e

    #define REG_JPEG_LUM_DC_HT_CONST_00     0x490
    #define REG_JPEG_LUM_DC_HT_CONST_01     0x492
    #define REG_JPEG_LUM_DC_HT_CONST_02     0x494
    #define REG_JPEG_LUM_DC_HT_CONST_03     0x496
    #define REG_JPEG_LUM_DC_HT_CONST_04     0x498

    #define REG_JPEG_CHR_DC_HT_CONST_00     0x4a0
    #define REG_JPEG_CHR_DC_HT_CONST_01     0x4a2
    #define REG_JPEG_CHR_DC_HT_CONST_02     0x4a4
    #define REG_JPEG_CHR_DC_HT_CONST_03     0x4a6
    #define REG_JPEG_CHR_DC_HT_CONST_04     0x4a8

    #define REG_JPEG_LUM_AC_HT_CONST_00     0x4b0
    #define REG_JPEG_LUM_AC_HT_CONST_01     0x4b2
    #define REG_JPEG_LUM_AC_HT_CONST_02     0x4b4
    #define REG_JPEG_LUM_AC_HT_CONST_03     0x4b6
    #define REG_JPEG_LUM_AC_HT_CONST_04     0x4b8

    #define REG_JPEG_CHR_AC_HT_CONST_00     0x4c0
    #define REG_JPEG_CHR_AC_HT_CONST_01     0x4c2
    #define REG_JPEG_CHR_AC_HT_CONST_02     0x4c4
    #define REG_JPEG_CHR_AC_HT_CONST_03     0x4c6
    #define REG_JPEG_CHR_AC_HT_CONST_04     0x4c8

    #define REG_JPEG_LUM_QT_CONST_00        0x4d0
    #define REG_JPEG_LUM_QT_CONST_01        0x4d2
    #define REG_JPEG_LUM_QT_CONST_02        0x4d4
    #define REG_JPEG_LUM_QT_CONST_03        0x4d6
    #define REG_JPEG_LUM_QT_CONST_04        0x4d8

    #define REG_JPEG_CHR_QT_CONST_00        0x4e0
    #define REG_JPEG_CHR_QT_CONST_01        0x4e2
    #define REG_JPEG_CHR_QT_CONST_02        0x4e4
    #define REG_JPEG_CHR_QT_CONST_03        0x4e6
    #define REG_JPEG_CHR_QT_CONST_04        0x4e8

    #define REG_JPEG_SOF_CONST_00           0x4f0
    #define REG_JPEG_SOF_CONST_01           0x4f2
    #define REG_JPEG_SOF_CONST_02           0x4f4
    #define REG_JPEG_SOF_CONST_03           0x4f6
    #define REG_JPEG_SOF_CONST_04           0x4f8

    #define REG_JPEG_QUANT_T0_00            0x500
    #define REG_JPEG_QUANT_T0_01            0x502
    #define REG_JPEG_QUANT_T0_02            0x504
    #define REG_JPEG_QUANT_T0_03            0x506
    #define REG_JPEG_QUANT_T0_04            0x508
    #define REG_JPEG_QUANT_T0_05            0x50a
    #define REG_JPEG_QUANT_T0_06            0x50c
    #define REG_JPEG_QUANT_T0_07            0x50e
    #define REG_JPEG_QUANT_T0_08            0x510
    #define REG_JPEG_QUANT_T0_09            0x512
    #define REG_JPEG_QUANT_T0_10            0x514
    #define REG_JPEG_QUANT_T0_11            0x516
    #define REG_JPEG_QUANT_T0_12            0x518
    #define REG_JPEG_QUANT_T0_13            0x51a
    #define REG_JPEG_QUANT_T0_14            0x51c
    #define REG_JPEG_QUANT_T0_15            0x51e
    #define REG_JPEG_QUANT_T0_16            0x520
    #define REG_JPEG_QUANT_T0_17            0x522
    #define REG_JPEG_QUANT_T0_18            0x524
    #define REG_JPEG_QUANT_T0_19            0x526
    #define REG_JPEG_QUANT_T0_20            0x528
    #define REG_JPEG_QUANT_T0_21            0x52a
    #define REG_JPEG_QUANT_T0_22            0x52c
    #define REG_JPEG_QUANT_T0_23            0x52e
    #define REG_JPEG_QUANT_T0_24            0x530
    #define REG_JPEG_QUANT_T0_25            0x532
    #define REG_JPEG_QUANT_T0_26            0x534
    #define REG_JPEG_QUANT_T0_27            0x536
    #define REG_JPEG_QUANT_T0_28            0x538
    #define REG_JPEG_QUANT_T0_29            0x53a
    #define REG_JPEG_QUANT_T0_30            0x53c
    #define REG_JPEG_QUANT_T0_31            0x53e
    #define REG_JPEG_QUANT_T0_32            0x540
    #define REG_JPEG_QUANT_T0_33            0x542
    #define REG_JPEG_QUANT_T0_34            0x544
    #define REG_JPEG_QUANT_T0_35            0x546
    #define REG_JPEG_QUANT_T0_36            0x548
    #define REG_JPEG_QUANT_T0_37            0x54a
    #define REG_JPEG_QUANT_T0_38            0x54c
    #define REG_JPEG_QUANT_T0_39            0x54e
    #define REG_JPEG_QUANT_T0_40            0x550
    #define REG_JPEG_QUANT_T0_41            0x552
    #define REG_JPEG_QUANT_T0_42            0x554
    #define REG_JPEG_QUANT_T0_43            0x556
    #define REG_JPEG_QUANT_T0_44            0x558
    #define REG_JPEG_QUANT_T0_45            0x55a
    #define REG_JPEG_QUANT_T0_46            0x55c
    #define REG_JPEG_QUANT_T0_47            0x55e
    #define REG_JPEG_QUANT_T0_48            0x560
    #define REG_JPEG_QUANT_T0_49            0x562
    #define REG_JPEG_QUANT_T0_50            0x564
    #define REG_JPEG_QUANT_T0_51            0x566
    #define REG_JPEG_QUANT_T0_52            0x568
    #define REG_JPEG_QUANT_T0_53            0x56a
    #define REG_JPEG_QUANT_T0_54            0x56c
    #define REG_JPEG_QUANT_T0_55            0x56e
    #define REG_JPEG_QUANT_T0_56            0x570
    #define REG_JPEG_QUANT_T0_57            0x572
    #define REG_JPEG_QUANT_T0_58            0x574
    #define REG_JPEG_QUANT_T0_59            0x576
    #define REG_JPEG_QUANT_T0_60            0x578
    #define REG_JPEG_QUANT_T0_61            0x57a
    #define REG_JPEG_QUANT_T0_62            0x57c
    #define REG_JPEG_QUANT_T0_63            0x57e

    #define REG_JPEG_QUANT_T1_00            0x580
    #define REG_JPEG_QUANT_T1_01            0x582
    #define REG_JPEG_QUANT_T1_02            0x584
    #define REG_JPEG_QUANT_T1_03            0x586
    #define REG_JPEG_QUANT_T1_04            0x588
    #define REG_JPEG_QUANT_T1_05            0x58a
    #define REG_JPEG_QUANT_T1_06            0x58c
    #define REG_JPEG_QUANT_T1_07            0x58e
    #define REG_JPEG_QUANT_T1_08            0x590
    #define REG_JPEG_QUANT_T1_09            0x592
    #define REG_JPEG_QUANT_T1_10            0x594
    #define REG_JPEG_QUANT_T1_11            0x596
    #define REG_JPEG_QUANT_T1_12            0x598
    #define REG_JPEG_QUANT_T1_13            0x59a
    #define REG_JPEG_QUANT_T1_14            0x59c
    #define REG_JPEG_QUANT_T1_15            0x59e
    #define REG_JPEG_QUANT_T1_16            0x5a0
    #define REG_JPEG_QUANT_T1_17            0x5a2
    #define REG_JPEG_QUANT_T1_18            0x5a4
    #define REG_JPEG_QUANT_T1_19            0x5a6
    #define REG_JPEG_QUANT_T1_20            0x5a8
    #define REG_JPEG_QUANT_T1_21            0x5aa
    #define REG_JPEG_QUANT_T1_22            0x5ac
    #define REG_JPEG_QUANT_T1_23            0x5ae
    #define REG_JPEG_QUANT_T1_24            0x5b0
    #define REG_JPEG_QUANT_T1_25            0x5b2
    #define REG_JPEG_QUANT_T1_26            0x5b4
    #define REG_JPEG_QUANT_T1_27            0x5b6
    #define REG_JPEG_QUANT_T1_28            0x5b8
    #define REG_JPEG_QUANT_T1_29            0x5ba
    #define REG_JPEG_QUANT_T1_30            0x5bc
    #define REG_JPEG_QUANT_T1_31            0x5be
    #define REG_JPEG_QUANT_T1_32            0x5c0
    #define REG_JPEG_QUANT_T1_33            0x5c2
    #define REG_JPEG_QUANT_T1_34            0x5c4
    #define REG_JPEG_QUANT_T1_35            0x5c6
    #define REG_JPEG_QUANT_T1_36            0x5c8
    #define REG_JPEG_QUANT_T1_37            0x5ca
    #define REG_JPEG_QUANT_T1_38            0x5cc
    #define REG_JPEG_QUANT_T1_39            0x5ce
    #define REG_JPEG_QUANT_T1_40            0x5d0
    #define REG_JPEG_QUANT_T1_41            0x5d2
    #define REG_JPEG_QUANT_T1_42            0x5d4
    #define REG_JPEG_QUANT_T1_43            0x5d6
    #define REG_JPEG_QUANT_T1_44            0x5d8
    #define REG_JPEG_QUANT_T1_45            0x5da
    #define REG_JPEG_QUANT_T1_46            0x5dc
    #define REG_JPEG_QUANT_T1_47            0x5de
    #define REG_JPEG_QUANT_T1_48            0x5e0
    #define REG_JPEG_QUANT_T1_49            0x5e2
    #define REG_JPEG_QUANT_T1_50            0x5e4
    #define REG_JPEG_QUANT_T1_51            0x5e6
    #define REG_JPEG_QUANT_T1_52            0x5e8
    #define REG_JPEG_QUANT_T1_53            0x5ea
    #define REG_JPEG_QUANT_T1_54            0x5ec
    #define REG_JPEG_QUANT_T1_55            0x5ee
    #define REG_JPEG_QUANT_T1_56            0x5f0
    #define REG_JPEG_QUANT_T1_57            0x5f2
    #define REG_JPEG_QUANT_T1_58            0x5f4
    #define REG_JPEG_QUANT_T1_59            0x5f6
    #define REG_JPEG_QUANT_T1_60            0x5f8
    #define REG_JPEG_QUANT_T1_61            0x5fa
    #define REG_JPEG_QUANT_T1_62            0x5fc
    #define REG_JPEG_QUANT_T1_63            0x5fe

    #define REG_JPEG_DC_T0_R0_00            0x600
    #define REG_JPEG_DC_T0_R0_01            0x602
    #define REG_JPEG_DC_T0_R0_02            0x604
    #define REG_JPEG_DC_T0_R0_03            0x606
    #define REG_JPEG_DC_T0_R0_04            0x608
    #define REG_JPEG_DC_T0_R0_05            0x60a
    #define REG_JPEG_DC_T0_R0_06            0x60c
    #define REG_JPEG_DC_T0_R0_07            0x60e
    #define REG_JPEG_DC_T0_R0_08            0x610
    #define REG_JPEG_DC_T0_R0_09            0x612
    #define REG_JPEG_DC_T0_R0_10            0x614
    #define REG_JPEG_DC_T0_R0_11            0x616
    #define REG_JPEG_DC_T0_R0_12            0x618
    #define REG_JPEG_DC_T0_R0_13            0x61a
    #define REG_JPEG_DC_T0_R0_14            0x61c
    #define REG_JPEG_DC_T0_R0_15            0x61e

    #define REG_JPEG_DC_T0_R1_00            0x620
    #define REG_JPEG_DC_T0_R1_01            0x622
    #define REG_JPEG_DC_T0_R1_02            0x624
    #define REG_JPEG_DC_T0_R1_03            0x626
    #define REG_JPEG_DC_T0_R1_04            0x628
    #define REG_JPEG_DC_T0_R1_05            0x62a
    #define REG_JPEG_DC_T0_R1_06            0x62c
    #define REG_JPEG_DC_T0_R1_07            0x62e
    #define REG_JPEG_DC_T0_R1_08            0x630
    #define REG_JPEG_DC_T0_R1_09            0x632
    #define REG_JPEG_DC_T0_R1_10            0x634
    #define REG_JPEG_DC_T0_R1_11            0x636

    #define REG_JPEG_AC_T0_R0_00            0x640
    #define REG_JPEG_AC_T0_R0_01            0x642
    #define REG_JPEG_AC_T0_R0_02            0x644
    #define REG_JPEG_AC_T0_R0_03            0x646
    #define REG_JPEG_AC_T0_R0_04            0x648
    #define REG_JPEG_AC_T0_R0_05            0x64a
    #define REG_JPEG_AC_T0_R0_06            0x64c
    #define REG_JPEG_AC_T0_R0_07            0x64e
    #define REG_JPEG_AC_T0_R0_08            0x650
    #define REG_JPEG_AC_T0_R0_09            0x652
    #define REG_JPEG_AC_T0_R0_10            0x654
    #define REG_JPEG_AC_T0_R0_11            0x656
    #define REG_JPEG_AC_T0_R0_12            0x658
    #define REG_JPEG_AC_T0_R0_13            0x65a
    #define REG_JPEG_AC_T0_R0_14            0x65c
    #define REG_JPEG_AC_T0_R0_15            0x65e

    #define REG_JPEG_AC_T0_R1_00            0x660
    #define REG_JPEG_AC_T0_R1_01            0x662
    #define REG_JPEG_AC_T0_R1_02            0x664
    #define REG_JPEG_AC_T0_R1_03            0x666
    #define REG_JPEG_AC_T0_R1_04            0x668
    #define REG_JPEG_AC_T0_R1_05            0x66a
    #define REG_JPEG_AC_T0_R1_06            0x66c
    #define REG_JPEG_AC_T0_R1_07            0x66e
    #define REG_JPEG_AC_T0_R1_08            0x670
    #define REG_JPEG_AC_T0_R1_09            0x672
    #define REG_JPEG_AC_T0_R1_10            0x674
    #define REG_JPEG_AC_T0_R1_11            0x676
    #define REG_JPEG_AC_T0_R1_12            0x678
    #define REG_JPEG_AC_T0_R1_13            0x67a
    #define REG_JPEG_AC_T0_R1_14            0x67c
    #define REG_JPEG_AC_T0_R1_15            0x67e
    #define REG_JPEG_AC_T0_R1_16            0x680
    #define REG_JPEG_AC_T0_R1_17            0x682
    #define REG_JPEG_AC_T0_R1_18            0x684
    #define REG_JPEG_AC_T0_R1_19            0x686
    #define REG_JPEG_AC_T0_R1_20            0x688
    #define REG_JPEG_AC_T0_R1_21            0x68a
    #define REG_JPEG_AC_T0_R1_22            0x68c
    #define REG_JPEG_AC_T0_R1_23            0x68e
    #define REG_JPEG_AC_T0_R1_24            0x690
    #define REG_JPEG_AC_T0_R1_25            0x692
    #define REG_JPEG_AC_T0_R1_26            0x694
    #define REG_JPEG_AC_T0_R1_27            0x696
    #define REG_JPEG_AC_T0_R1_28            0x698
    #define REG_JPEG_AC_T0_R1_29            0x69a
    #define REG_JPEG_AC_T0_R1_30            0x69c
    #define REG_JPEG_AC_T0_R1_31            0x69e
    #define REG_JPEG_AC_T0_R1_32            0x6a0
    #define REG_JPEG_AC_T0_R1_33            0x6a2
    #define REG_JPEG_AC_T0_R1_34            0x6a4
    #define REG_JPEG_AC_T0_R1_35            0x6a6
    #define REG_JPEG_AC_T0_R1_36            0x6a8
    #define REG_JPEG_AC_T0_R1_37            0x6aa
    #define REG_JPEG_AC_T0_R1_38            0x6ac
    #define REG_JPEG_AC_T0_R1_39            0x6ae
    #define REG_JPEG_AC_T0_R1_40            0x6b0
    #define REG_JPEG_AC_T0_R1_41            0x6b2
    #define REG_JPEG_AC_T0_R1_42            0x6b4
    #define REG_JPEG_AC_T0_R1_43            0x6b6
    #define REG_JPEG_AC_T0_R1_44            0x6b8
    #define REG_JPEG_AC_T0_R1_45            0x6ba
    #define REG_JPEG_AC_T0_R1_46            0x6bc
    #define REG_JPEG_AC_T0_R1_47            0x6be
    #define REG_JPEG_AC_T0_R1_48            0x6c0
    #define REG_JPEG_AC_T0_R1_49            0x6c2
    #define REG_JPEG_AC_T0_R1_50            0x6c4
    #define REG_JPEG_AC_T0_R1_51            0x6c6
    #define REG_JPEG_AC_T0_R1_52            0x6c8
    #define REG_JPEG_AC_T0_R1_53            0x6ca
    #define REG_JPEG_AC_T0_R1_54            0x6cc
    #define REG_JPEG_AC_T0_R1_55            0x6ce
    #define REG_JPEG_AC_T0_R1_56            0x6d0
    #define REG_JPEG_AC_T0_R1_57            0x6d2
    #define REG_JPEG_AC_T0_R1_58            0x6d4
    #define REG_JPEG_AC_T0_R1_59            0x6d6
    #define REG_JPEG_AC_T0_R1_60            0x6d8
    #define REG_JPEG_AC_T0_R1_61            0x6da
    #define REG_JPEG_AC_T0_R1_62            0x6dc
    #define REG_JPEG_AC_T0_R1_63            0x6de
    #define REG_JPEG_AC_T0_R1_64            0x6e0
    #define REG_JPEG_AC_T0_R1_65            0x6e2
    #define REG_JPEG_AC_T0_R1_66            0x6e4
    #define REG_JPEG_AC_T0_R1_67            0x6e6
    #define REG_JPEG_AC_T0_R1_68            0x6e8
    #define REG_JPEG_AC_T0_R1_69            0x6ea
    #define REG_JPEG_AC_T0_R1_70            0x6ec
    #define REG_JPEG_AC_T0_R1_71            0x6ee
    #define REG_JPEG_AC_T0_R1_72            0x6f0
    #define REG_JPEG_AC_T0_R1_73            0x6f2
    #define REG_JPEG_AC_T0_R1_74            0x6f4
    #define REG_JPEG_AC_T0_R1_75            0x6f6
    #define REG_JPEG_AC_T0_R1_76            0x6f8
    #define REG_JPEG_AC_T0_R1_77            0x6fa
    #define REG_JPEG_AC_T0_R1_78            0x6fc
    #define REG_JPEG_AC_T0_R1_79            0x6fe
    #define REG_JPEG_AC_T0_R1_80            0x700
    #define REG_JPEG_AC_T0_R1_81            0x702
    #define REG_JPEG_AC_T0_R1_82            0x704
    #define REG_JPEG_AC_T0_R1_83            0x706
    #define REG_JPEG_AC_T0_R1_84            0x708
    #define REG_JPEG_AC_T0_R1_85            0x70a
    #define REG_JPEG_AC_T0_R1_86            0x70c
    #define REG_JPEG_AC_T0_R1_87            0x70e
    #define REG_JPEG_AC_T0_R1_88            0x710
    #define REG_JPEG_AC_T0_R1_89            0x712
    #define REG_JPEG_AC_T0_R1_90            0x714
    #define REG_JPEG_AC_T0_R1_91            0x716
    #define REG_JPEG_AC_T0_R1_92            0x718
    #define REG_JPEG_AC_T0_R1_93            0x71a
    #define REG_JPEG_AC_T0_R1_94            0x71c
    #define REG_JPEG_AC_T0_R1_95            0x71e
    #define REG_JPEG_AC_T0_R1_96            0x720
    #define REG_JPEG_AC_T0_R1_97            0x722
    #define REG_JPEG_AC_T0_R1_98            0x724
    #define REG_JPEG_AC_T0_R1_99            0x726
    #define REG_JPEG_AC_T0_R1_100           0x728
    #define REG_JPEG_AC_T0_R1_101           0x72a
    #define REG_JPEG_AC_T0_R1_102           0x72c
    #define REG_JPEG_AC_T0_R1_103           0x72e
    #define REG_JPEG_AC_T0_R1_104           0x730
    #define REG_JPEG_AC_T0_R1_105           0x732
    #define REG_JPEG_AC_T0_R1_106           0x734
    #define REG_JPEG_AC_T0_R1_107           0x736
    #define REG_JPEG_AC_T0_R1_108           0x738
    #define REG_JPEG_AC_T0_R1_109           0x73a
    #define REG_JPEG_AC_T0_R1_110           0x73c
    #define REG_JPEG_AC_T0_R1_111           0x73e
    #define REG_JPEG_AC_T0_R1_112           0x740
    #define REG_JPEG_AC_T0_R1_113           0x742
    #define REG_JPEG_AC_T0_R1_114           0x744
    #define REG_JPEG_AC_T0_R1_115           0x746
    #define REG_JPEG_AC_T0_R1_116           0x748
    #define REG_JPEG_AC_T0_R1_117           0x74a
    #define REG_JPEG_AC_T0_R1_118           0x74c
    #define REG_JPEG_AC_T0_R1_119           0x74e
    #define REG_JPEG_AC_T0_R1_120           0x750
    #define REG_JPEG_AC_T0_R1_121           0x752
    #define REG_JPEG_AC_T0_R1_122           0x754
    #define REG_JPEG_AC_T0_R1_123           0x756
    #define REG_JPEG_AC_T0_R1_124           0x758
    #define REG_JPEG_AC_T0_R1_125           0x75a
    #define REG_JPEG_AC_T0_R1_126           0x75c
    #define REG_JPEG_AC_T0_R1_127           0x75e
    #define REG_JPEG_AC_T0_R1_128           0x760
    #define REG_JPEG_AC_T0_R1_129           0x762
    #define REG_JPEG_AC_T0_R1_130           0x764
    #define REG_JPEG_AC_T0_R1_131           0x766
    #define REG_JPEG_AC_T0_R1_132           0x768
    #define REG_JPEG_AC_T0_R1_133           0x76a
    #define REG_JPEG_AC_T0_R1_134           0x76c
    #define REG_JPEG_AC_T0_R1_135           0x76e
    #define REG_JPEG_AC_T0_R1_136           0x770
    #define REG_JPEG_AC_T0_R1_137           0x772
    #define REG_JPEG_AC_T0_R1_138           0x774
    #define REG_JPEG_AC_T0_R1_139           0x776
    #define REG_JPEG_AC_T0_R1_140           0x778
    #define REG_JPEG_AC_T0_R1_141           0x77a
    #define REG_JPEG_AC_T0_R1_142           0x77c
    #define REG_JPEG_AC_T0_R1_143           0x77e
    #define REG_JPEG_AC_T0_R1_144           0x780
    #define REG_JPEG_AC_T0_R1_145           0x782
    #define REG_JPEG_AC_T0_R1_146           0x784
    #define REG_JPEG_AC_T0_R1_147           0x786
    #define REG_JPEG_AC_T0_R1_148           0x788
    #define REG_JPEG_AC_T0_R1_149           0x78a
    #define REG_JPEG_AC_T0_R1_150           0x78c
    #define REG_JPEG_AC_T0_R1_151           0x78e
    #define REG_JPEG_AC_T0_R1_152           0x790
    #define REG_JPEG_AC_T0_R1_153           0x792
    #define REG_JPEG_AC_T0_R1_154           0x794
    #define REG_JPEG_AC_T0_R1_155           0x796
    #define REG_JPEG_AC_T0_R1_156           0x798
    #define REG_JPEG_AC_T0_R1_157           0x79a
    #define REG_JPEG_AC_T0_R1_158           0x79c
    #define REG_JPEG_AC_T0_R1_159           0x79e
    #define REG_JPEG_AC_T0_R1_160           0x7a0
    #define REG_JPEG_AC_T0_R1_161           0x7a2

    #define REG_JPEG_DC_T1_R0_00            0x800
    #define REG_JPEG_DC_T1_R0_01            0x802
    #define REG_JPEG_DC_T1_R0_02            0x804
    #define REG_JPEG_DC_T1_R0_03            0x806
    #define REG_JPEG_DC_T1_R0_04            0x808
    #define REG_JPEG_DC_T1_R0_05            0x80a
    #define REG_JPEG_DC_T1_R0_06            0x80c
    #define REG_JPEG_DC_T1_R0_07            0x80e
    #define REG_JPEG_DC_T1_R0_08            0x810
    #define REG_JPEG_DC_T1_R0_09            0x812
    #define REG_JPEG_DC_T1_R0_10            0x814
    #define REG_JPEG_DC_T1_R0_11            0x816
    #define REG_JPEG_DC_T1_R0_12            0x818
    #define REG_JPEG_DC_T1_R0_13            0x81a
    #define REG_JPEG_DC_T1_R0_14            0x81c
    #define REG_JPEG_DC_T1_R0_15            0x81e

    #define REG_JPEG_DC_T1_R1_00            0x820
    #define REG_JPEG_DC_T1_R1_01            0x822
    #define REG_JPEG_DC_T1_R1_02            0x824
    #define REG_JPEG_DC_T1_R1_03            0x826
    #define REG_JPEG_DC_T1_R1_04            0x828
    #define REG_JPEG_DC_T1_R1_05            0x82a
    #define REG_JPEG_DC_T1_R1_06            0x82c
    #define REG_JPEG_DC_T1_R1_07            0x82e
    #define REG_JPEG_DC_T1_R1_08            0x830
    #define REG_JPEG_DC_T1_R1_09            0x832
    #define REG_JPEG_DC_T1_R1_10            0x834
    #define REG_JPEG_DC_T1_R1_11            0x836

    #define REG_JPEG_AC_T1_R0_00            0x840
    #define REG_JPEG_AC_T1_R0_01            0x842
    #define REG_JPEG_AC_T1_R0_02            0x844
    #define REG_JPEG_AC_T1_R0_03            0x846
    #define REG_JPEG_AC_T1_R0_04            0x848
    #define REG_JPEG_AC_T1_R0_05            0x84a
    #define REG_JPEG_AC_T1_R0_06            0x84c
    #define REG_JPEG_AC_T1_R0_07            0x84e
    #define REG_JPEG_AC_T1_R0_08            0x850
    #define REG_JPEG_AC_T1_R0_09            0x852
    #define REG_JPEG_AC_T1_R0_10            0x854
    #define REG_JPEG_AC_T1_R0_11            0x856
    #define REG_JPEG_AC_T1_R0_12            0x858
    #define REG_JPEG_AC_T1_R0_13            0x85a
    #define REG_JPEG_AC_T1_R0_14            0x85c
    #define REG_JPEG_AC_T1_R0_15            0x85e

    #define REG_JPEG_AC_T1_R1_00            0x860
    #define REG_JPEG_AC_T1_R1_01            0x862
    #define REG_JPEG_AC_T1_R1_02            0x864
    #define REG_JPEG_AC_T1_R1_03            0x866
    #define REG_JPEG_AC_T1_R1_04            0x868
    #define REG_JPEG_AC_T1_R1_05            0x86a
    #define REG_JPEG_AC_T1_R1_06            0x86c
    #define REG_JPEG_AC_T1_R1_07            0x86e
    #define REG_JPEG_AC_T1_R1_08            0x870
    #define REG_JPEG_AC_T1_R1_09            0x872
    #define REG_JPEG_AC_T1_R1_10            0x874
    #define REG_JPEG_AC_T1_R1_11            0x876
    #define REG_JPEG_AC_T1_R1_12            0x878
    #define REG_JPEG_AC_T1_R1_13            0x87a
    #define REG_JPEG_AC_T1_R1_14            0x87c
    #define REG_JPEG_AC_T1_R1_15            0x87e
    #define REG_JPEG_AC_T1_R1_16            0x880
    #define REG_JPEG_AC_T1_R1_17            0x882
    #define REG_JPEG_AC_T1_R1_18            0x884
    #define REG_JPEG_AC_T1_R1_19            0x886
    #define REG_JPEG_AC_T1_R1_20            0x888
    #define REG_JPEG_AC_T1_R1_21            0x88a
    #define REG_JPEG_AC_T1_R1_22            0x88c
    #define REG_JPEG_AC_T1_R1_23            0x88e
    #define REG_JPEG_AC_T1_R1_24            0x890
    #define REG_JPEG_AC_T1_R1_25            0x892
    #define REG_JPEG_AC_T1_R1_26            0x894
    #define REG_JPEG_AC_T1_R1_27            0x896
    #define REG_JPEG_AC_T1_R1_28            0x898
    #define REG_JPEG_AC_T1_R1_29            0x89a
    #define REG_JPEG_AC_T1_R1_30            0x89c
    #define REG_JPEG_AC_T1_R1_31            0x89e
    #define REG_JPEG_AC_T1_R1_32            0x8a0
    #define REG_JPEG_AC_T1_R1_33            0x8a2
    #define REG_JPEG_AC_T1_R1_34            0x8a4
    #define REG_JPEG_AC_T1_R1_35            0x8a6
    #define REG_JPEG_AC_T1_R1_36            0x8a8
    #define REG_JPEG_AC_T1_R1_37            0x8aa
    #define REG_JPEG_AC_T1_R1_38            0x8ac
    #define REG_JPEG_AC_T1_R1_39            0x8ae
    #define REG_JPEG_AC_T1_R1_40            0x8b0
    #define REG_JPEG_AC_T1_R1_41            0x8b2
    #define REG_JPEG_AC_T1_R1_42            0x8b4
    #define REG_JPEG_AC_T1_R1_43            0x8b6
    #define REG_JPEG_AC_T1_R1_44            0x8b8
    #define REG_JPEG_AC_T1_R1_45            0x8ba
    #define REG_JPEG_AC_T1_R1_46            0x8bc
    #define REG_JPEG_AC_T1_R1_47            0x8be
    #define REG_JPEG_AC_T1_R1_48            0x8c0
    #define REG_JPEG_AC_T1_R1_49            0x8c2
    #define REG_JPEG_AC_T1_R1_50            0x8c4
    #define REG_JPEG_AC_T1_R1_51            0x8c6
    #define REG_JPEG_AC_T1_R1_52            0x8c8
    #define REG_JPEG_AC_T1_R1_53            0x8ca
    #define REG_JPEG_AC_T1_R1_54            0x8cc
    #define REG_JPEG_AC_T1_R1_55            0x8ce
    #define REG_JPEG_AC_T1_R1_56            0x8d0
    #define REG_JPEG_AC_T1_R1_57            0x8d2
    #define REG_JPEG_AC_T1_R1_58            0x8d4
    #define REG_JPEG_AC_T1_R1_59            0x8d6
    #define REG_JPEG_AC_T1_R1_60            0x8d8
    #define REG_JPEG_AC_T1_R1_61            0x8da
    #define REG_JPEG_AC_T1_R1_62            0x8dc
    #define REG_JPEG_AC_T1_R1_63            0x8de
    #define REG_JPEG_AC_T1_R1_64            0x8e0
    #define REG_JPEG_AC_T1_R1_65            0x8e2
    #define REG_JPEG_AC_T1_R1_66            0x8e4
    #define REG_JPEG_AC_T1_R1_67            0x8e6
    #define REG_JPEG_AC_T1_R1_68            0x8e8
    #define REG_JPEG_AC_T1_R1_69            0x8ea
    #define REG_JPEG_AC_T1_R1_70            0x8ec
    #define REG_JPEG_AC_T1_R1_71            0x8ee
    #define REG_JPEG_AC_T1_R1_72            0x8f0
    #define REG_JPEG_AC_T1_R1_73            0x8f2
    #define REG_JPEG_AC_T1_R1_74            0x8f4
    #define REG_JPEG_AC_T1_R1_75            0x8f6
    #define REG_JPEG_AC_T1_R1_76            0x8f8
    #define REG_JPEG_AC_T1_R1_77            0x8fa
    #define REG_JPEG_AC_T1_R1_78            0x8fc
    #define REG_JPEG_AC_T1_R1_79            0x8fe
    #define REG_JPEG_AC_T1_R1_80            0x900
    #define REG_JPEG_AC_T1_R1_81            0x902
    #define REG_JPEG_AC_T1_R1_82            0x904
    #define REG_JPEG_AC_T1_R1_83            0x906
    #define REG_JPEG_AC_T1_R1_84            0x908
    #define REG_JPEG_AC_T1_R1_85            0x90a
    #define REG_JPEG_AC_T1_R1_86            0x90c
    #define REG_JPEG_AC_T1_R1_87            0x90e
    #define REG_JPEG_AC_T1_R1_88            0x910
    #define REG_JPEG_AC_T1_R1_89            0x912
    #define REG_JPEG_AC_T1_R1_90            0x914
    #define REG_JPEG_AC_T1_R1_91            0x916
    #define REG_JPEG_AC_T1_R1_92            0x918
    #define REG_JPEG_AC_T1_R1_93            0x91a
    #define REG_JPEG_AC_T1_R1_94            0x91c
    #define REG_JPEG_AC_T1_R1_95            0x91e
    #define REG_JPEG_AC_T1_R1_96            0x920
    #define REG_JPEG_AC_T1_R1_97            0x922
    #define REG_JPEG_AC_T1_R1_98            0x924
    #define REG_JPEG_AC_T1_R1_99            0x926
    #define REG_JPEG_AC_T1_R1_100           0x928
    #define REG_JPEG_AC_T1_R1_101           0x92a
    #define REG_JPEG_AC_T1_R1_102           0x92c
    #define REG_JPEG_AC_T1_R1_103           0x92e
    #define REG_JPEG_AC_T1_R1_104           0x930
    #define REG_JPEG_AC_T1_R1_105           0x932
    #define REG_JPEG_AC_T1_R1_106           0x934
    #define REG_JPEG_AC_T1_R1_107           0x936
    #define REG_JPEG_AC_T1_R1_108           0x938
    #define REG_JPEG_AC_T1_R1_109           0x93a
    #define REG_JPEG_AC_T1_R1_110           0x93c
    #define REG_JPEG_AC_T1_R1_111           0x93e
    #define REG_JPEG_AC_T1_R1_112           0x940
    #define REG_JPEG_AC_T1_R1_113           0x942
    #define REG_JPEG_AC_T1_R1_114           0x944
    #define REG_JPEG_AC_T1_R1_115           0x946
    #define REG_JPEG_AC_T1_R1_116           0x948
    #define REG_JPEG_AC_T1_R1_117           0x94a
    #define REG_JPEG_AC_T1_R1_118           0x94c
    #define REG_JPEG_AC_T1_R1_119           0x94e
    #define REG_JPEG_AC_T1_R1_120           0x950
    #define REG_JPEG_AC_T1_R1_121           0x952
    #define REG_JPEG_AC_T1_R1_122           0x954
    #define REG_JPEG_AC_T1_R1_123           0x956
    #define REG_JPEG_AC_T1_R1_124           0x958
    #define REG_JPEG_AC_T1_R1_125           0x95a
    #define REG_JPEG_AC_T1_R1_126           0x95c
    #define REG_JPEG_AC_T1_R1_127           0x95e
    #define REG_JPEG_AC_T1_R1_128           0x960
    #define REG_JPEG_AC_T1_R1_129           0x962
    #define REG_JPEG_AC_T1_R1_130           0x964
    #define REG_JPEG_AC_T1_R1_131           0x966
    #define REG_JPEG_AC_T1_R1_132           0x968
    #define REG_JPEG_AC_T1_R1_133           0x96a
    #define REG_JPEG_AC_T1_R1_134           0x96c
    #define REG_JPEG_AC_T1_R1_135           0x96e
    #define REG_JPEG_AC_T1_R1_136           0x970
    #define REG_JPEG_AC_T1_R1_137           0x972
    #define REG_JPEG_AC_T1_R1_138           0x974
    #define REG_JPEG_AC_T1_R1_139           0x976
    #define REG_JPEG_AC_T1_R1_140           0x978
    #define REG_JPEG_AC_T1_R1_141           0x97a
    #define REG_JPEG_AC_T1_R1_142           0x97c
    #define REG_JPEG_AC_T1_R1_143           0x97e
    #define REG_JPEG_AC_T1_R1_144           0x980
    #define REG_JPEG_AC_T1_R1_145           0x982
    #define REG_JPEG_AC_T1_R1_146           0x984
    #define REG_JPEG_AC_T1_R1_147           0x986
    #define REG_JPEG_AC_T1_R1_148           0x988
    #define REG_JPEG_AC_T1_R1_149           0x98a
    #define REG_JPEG_AC_T1_R1_150           0x98c
    #define REG_JPEG_AC_T1_R1_151           0x98e
    #define REG_JPEG_AC_T1_R1_152           0x990
    #define REG_JPEG_AC_T1_R1_153           0x992
    #define REG_JPEG_AC_T1_R1_154           0x994
    #define REG_JPEG_AC_T1_R1_155           0x996
    #define REG_JPEG_AC_T1_R1_156           0x998
    #define REG_JPEG_AC_T1_R1_157           0x99a
    #define REG_JPEG_AC_T1_R1_158           0x99c
    #define REG_JPEG_AC_T1_R1_159           0x99e
    #define REG_JPEG_AC_T1_R1_160           0x9a0
    #define REG_JPEG_AC_T1_R1_161           0x9a2

    #define REG_JPEG_QTABLE_CONST_0         0x9b0
    #define REG_JPEG_QTABLE_CONST_1         0x9b2
    #define REG_JPEG_QTABLE_CONST_2         0x9b4
    #define REG_JPEG_QTABLE_CONST_3         0x9b6

    #define REG_JPEG_QTABLE0_SAMPLE         0x9b8
    #define REG_JPEG_QTABLE1_SAMPLE         0x9bc
    #define REG_JPEG_QTABLE2_SAMPLE         0x9c0

    #define REG_JPEG_DRI_CONST_0            0x9c4
    #define REG_JPEG_DRI_CONST_1            0x9c6
    #define REG_JPEG_DRI_CONST_2            0x9c8
    #define REG_JPEG_DRI_CONST_3            0x9ca

    #define REG_JPEG_SOS_CONST_0            0x9cc
    #define REG_JPEG_SOS_CONST_1            0x9ce
    #define REG_JPEG_SOS_CONST_2            0x9d0
    #define REG_JPEG_SOS_CONST_3            0x9d2
    #define REG_JPEG_SOS_CONST_4            0x9d4

    #define REG_JPEG_EOI_CONST_0            0x9e4
    #define REG_JPEG_EOI_CONST_1            0x9e6
    #define REG_JPEG_EOI_CONST_2            0x9e8
    #define REG_JPEG_EOI_CONST_3            0x9ea
    #define REG_JPEG_EOI_CONST_4            0x9ec

    #define REG_JPEG_VERT_PIX_SIZE0         0x9f0
    #define REG_JPEG_VERT_PIX_SIZE1         0x9f2
    #define REG_JPEG_HORI_PIX_SIZE0         0x9f4
    #define REG_JPEG_HORI_PIX_SIZE1         0x9f6
    #define REG_JPEG_DRI_CONFIG0            0x9f8
    #define REG_JPEG_DRI_CONFIG1            0x9fa
    
    #define GFX_LCD_TFT  0x01			// Type TFT Display 
    #define GFX_LCD_CSTN 0x03			// Type Color STN Display 
    #define GFX_LCD_MSTN 0x02			// Type Mono STN Display 
    
#endif // _SSD1926_H
