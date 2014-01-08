/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Epson S1D13517 LCD controllers driver
 *  to be used with GFX 3e PICtail board
 *****************************************************************************
 * FileName:        S1D13517.h
 * Dependencies:    p24Fxxxx.h or plib.h
 * Processor:       PIC24, PIC32
 * Compiler:        MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *****************************************************************************/
#ifndef _S1D13517_H
    #define _S1D13517_H

    #ifdef USE_TRANSITION_EFFECTS
        #include "Transitions.h"
    #endif

    #ifdef USE_PALETTE
        #include "Graphics/Palette.h"
    #endif

	typedef    enum {
	GFX_PAGE0 = 0,
	GFX_PAGE1,
	GFX_PAGE2,
	GFX_PAGE3,
	GFX_PAGE4,
	GFX_PAGE5,
	GFX_PAGE6,
	GFX_PAGE7,
	GFX_PAGE8,
	GFX_PAGE9,
	GFX_PAGE10,
	GFX_PAGE11,
	GFX_PAGE12,
	GFX_PAGE13,
	GFX_PAGE14,
	GFX_PAGE15,
	GFX_NUM_OF_PAGES
	} GFX_PAGE;

    #ifndef DISP_HOR_RESOLUTION
        #error DISP_HOR_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_VER_RESOLUTION
        #error DISP_VER_RESOLUTION must be defined in HardwareProfile.h
    #endif
     
    #ifndef COLOR_DEPTH
        #error COLOR_DEPTH must be defined in HardwareProfile.h
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
    #if (COLOR_DEPTH != 16) && (COLOR_DEPTH != 24)
        #error This driver supports 16 and 24 BPP only.
    #endif
    #if (DISP_HOR_RESOLUTION % 8) != 0
        #error Horizontal resolution must be divisible by 8.
    #endif
    #if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180) && (DISP_ORIENTATION != 90) && (DISP_ORIENTATION != 270)
        #error The display orientation selected is not supported. It can be only 0,90,180 or 270.
    #endif

void  SetReg(WORD index, BYTE value);
BYTE  GetReg(WORD index);

/*********************************************************************
* Function: Alpha Macros
********************************************************************/
#define 	NUM_ALPHA_LEVELS 	0x20			// Specific to device
#define 	ALPHA_DELTA 		(NUM_ALPHA_LEVELS << 5) / 100
/*********************************************************************
* Function: BYTE Percentage2Alpha(BYTE alphaPercentage)
********************************************************************/
extern inline BYTE __attribute__ ((always_inline)) Percentage2Alpha(BYTE alphaPercentage)
{
    DWORD percent = (DWORD)(alphaPercentage);
    percent *= ALPHA_DELTA;
    percent >>= 5;

    return (BYTE)percent;
}

/*********************************************************************
* Function:  void DisplayBrightness(WORD level)
*
* Overview: Sets LCD Backlight Level
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void DisplayBrightness(WORD level);

typedef enum { GFX_MAIN_LAYER, GFX_PIP_LAYER, GFX_CURSOR_LAYER } GFX_LAYER;
///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Creates a layer. 
///
///	This function is used to create an additional layer that can be used for PIP, 
/// Cursor or other porpouse.
///
/// \param	Layer		    Corresponds with layer. 
///                         This value can be either GFX_PIP_LAYER or GFX_CURSOR_LAYER.
///
/// \param  Config			This value makes configuration for the Layer that 
///                         can be combained with the following flags:
///								GFX_LAYER_TRANS_EN
///								GFX_LAYER_OUTPUT_NORMAL
///								GFX_LAYER_OUTPUT_XOR
///								GFX_LAYER_OUTPUT_XORNOT
///								GFX_LAYER_OUTPUT_NOT
///							and alpha color 4 bit value. For instance, Config can be 
///							( GFX_LAYER_TRANS_EN | GFX_LAYER_OUTPUT_NORMAL | 0xA )
///							where 0xA is value of transparency.  
///
/// \param	XStart			This value configures the X start position for the Layer
///
/// \param	YStart			This value configures the Y start position for the Layer
///
/// \param	Width			This value configures the width for the Layer
///							 
///
/// \param	Height			This value configures the height for the Layer
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_CreateLayer( GFX_LAYER Layer, WORD Config, WORD XStart,  WORD YStart,  WORD Width,  WORD Height );

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Moves a layer.
///
///	This function is used to move an additional layer, PIP or Cursor.
///
/// \param	Layer		    Corresponds with layer. 
///                         This value can be either GFX_PIP_LAYER or GFX_CURSOR_LAYER.
///
/// \param	X				This value of new X start position for the Layer
///
/// \param	Y			     This value of new Y start position for the Layer
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_MoveLayer( GFX_LAYER Layer, WORD X, WORD Y );

void GFX_DRIVER_LayerStartAddress( GFX_LAYER Layer, WORD page, WORD X, WORD Y );

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Shows a layer.
///
///	This function is used to show an additional layer, PIP or Cursor.
///
/// \param	Layer		    Corresponds with layer. 
///                         This value can be either GFX_PIP_LAYER or GFX_CURSOR_LAYER.
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_ShowLayer( GFX_LAYER Layer );

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Hides a layer.
///
///	This function is used to hide an additional layer, PIP or Cursor.
///
/// \param	Layer		    Corresponds with layer. 
///                         This value can be either GFX_PIP_LAYER or GFX_CURSOR_LAYER.
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_HideLayer( GFX_LAYER Layer );

/*********************************************************************
* Overview: SSD1926 registers definitions.
*********************************************************************/	
	#define REG00_PROD_CODE        0x00     // Product Code Register [READONLY]
	#define REG02_READBACK         0x02     // Configuration Readback Register [READONLY]
	#define REG04_PLL_DDIVIDER     0x04     // PLL D-Divider Register
	#define REG06_PLL_0            0x06     // PLL Setting Register 0
	#define REG08_PLL_1            0x08     // PLL Setting Register 1
	#define REG0A_PLL_2            0x0A     // PLL Setting Register 2
	#define REG0C_PLL_NDIVIDER     0x0C     // PLL N-Divider Register
	#define REG0E_SS_CONTROL_0     0x0E     // SS Control Register 0
	#define REG10_SS_CONTROL_1     0x10     // SS Control Register 1
	#define REG12_CLK_SRC_SELECT   0x12     // Clock Source Select Register
	#define REG14_LCD_PANEL_TYPE   0x14     // LCD Panel Type Register
	#define REG16_HDISP_WIDTH      0x16     // Horizontal Display Width Register
	#define REG18_HNDP_PERIOD      0x18     // Horizontal Non-Display Period Register
	#define REG1A_VDISP_HEIGHT_0   0x1A     // Vertical Display Height Register 0
	#define REG1C_VDISP_HEIGHT_1   0x1C     // Vertical Display Height Register 1
	#define REG1E_VNDP_PERIOD      0x1E     // Vertical Non-Display Period Register
	#define REG20_PHS_PULSE_WIDTH  0x20     // PHS Pulse Width (HSW) Register
	#define REG22_PHS_PULSE_START  0x22     // PHS Pulse Start Position (HPS) Register
	#define REG24_PVS_PULSE_WIDTH  0x24     // PVS Pulse Width (VSW) Register
	#define REG26_PVS_PULSE_START  0x26     // PVS Pulse Start Position (VPS) Register
	#define REG28_PCLK_POLARITY    0x28     // PCLK Polarity Register
	#define REG2A_DSP_MODE         0x2A     // Display Mode Register
	#define REG2C_PIP1_DSP_SA_0    0x2C     // PIP1 Display Start Address Register 0
	#define REG2E_PIP1_DSP_SA_1    0x2E     // PIP1 Display Start Address Register 1
	#define REG30_PIP1_DSP_SA_2    0x30     // PIP1 Display Start Address Register 2
	#define REG32_PIP1_WIN_X_SP    0x32     // PIP1 Window X Start Position Register
	#define REG34_PIP1_WIN_Y_SP_0  0x34     // PIP1 Window Y Start Position Register 0
	#define REG36_PIP1_WIN_Y_SP_1  0x36     // PIP1 Window Y Start Position Register 1
	#define REG38_PIP1_WIN_X_EP    0x38     // PIP1 Window X End Position Register
	#define REG3A_PIP1_WIN_Y_EP_0  0x3A     // PIP1 Window Y End Position Register 0
	#define REG3C_PIP1_WIN_Y_EP_1  0x3C     // PIP1 Window Y End Position Register 1
	#define REG3E_PIP2_DSP_SA_0    0x3E     // PIP2 Display Start Address Register 0
	#define REG40_PIP2_DSP_SA_1    0x40     // PIP2 Display Start Address Register 1
	#define REG42_PIP2_DSP_SA_2    0x42     // PIP2 Display Start Address Register 2
	#define REG44_PIP2_WIN_X_SP    0x44     // PIP2 Window X Start Position Register
	#define REG46_PIP2_WIN_Y_SP_0  0x46     // PIP2 Window Y Start Position Register 0
	#define REG48_PIP2_WIN_Y_SP_1  0x48     // PIP2 Window Y Start Position Register 1
	#define REG4A_PIP2_WIN_X_EP    0x4A     // PIP2 Window X End Position Register
	#define REG4C_PIP2_WIN_Y_EP_0  0x4C     // PIP2 Window Y End Position Register 0
	#define REG4E_PIP2_WIN_Y_EP_1  0x4E     // PIP2 Window Y End Position Register 1
	#define REG50_DISPLAY_CONTROL  0x50     // Display Control Register [WRITEONLY]
	#define REG52_INPUT_MODE       0x52     // Input Mode Register
	#define REG54_TRANSP_KEY_RED   0x54     // Transparency Key Color Red Register
	#define REG56_TRANSP_KEY_GREEN 0x56     // Transparency Key Color Green Register
	#define REG58_TRANSP_KEY_BLUE  0x58     // Transparency Key Color Blue Register
	#define REG5A_WRITE_WIN_X_SP   0x5A     // Write Window X Start Position Register
	#define REG5C_WRITE_WIN_Y_SP_0 0x5C     // Write Window Start Position Register 0
	#define REG5E_WRITE_WIN_Y_SP_1 0x5E     // Write Window Start Position Register 1
	#define REG60_WRITE_WIN_X_EP   0x60     // Write Window X End Position Register
	#define REG62_WRITE_WIN_Y_EP_0 0x62     // Write Window Y End Position Register 0
	#define REG64_WRITE_WIN_Y_EP_1 0x64     // Write Window Y End Position Register 1
	#define REG66_MEM_DATA_PORT_0  0x66     // Memory Data Port Register 0 [WRITEONLY]
	#define REG67_MEM_DATA_PORT_1  0x67     // Memory Data Port Register 1 [WRITEONLY]
	#define REG68_POWER_SAVE       0x68     // Power Save Register
	#define REG6A_N_DISP_PER_CTRS  0x6A     // Non-Display Period Control/Status Register
	#define REG6C_GPO_0            0x6C     // General Purpose Output Register 0
	#define REG6E_GPO_1            0x6E     // General Purpose Output Register 1
	#define REG70_PWM_CONTROL      0x70     // PWM Control Register
	#define REG72_PWM_HIGH_DC_0    0x72     // PWM High Duty Cycle Register 0
	#define REG74_PWM_HIGH_DC_1    0x74     // PWM High Duty Cycle Register 1
	#define REG76_PWM_HIGH_DC_2    0x76     // PWM High Duty Cycle Register 2
	#define REG78_PWM_HIGH_DC_3    0x78     // PWM High Duty Cycle Register 3
	#define REG7A_PWM_LOW_DC_0     0x7A     // PWM Low Duty Cycle Register 0
	#define REG7C_PWM_LOW_DC_1     0x7C     // PWM Low Duty Cycle Register 1
	#define REG7E_PWM_LOW_DC_2     0x7E     // PWM Low Duty Cycle Register 2
	#define REG80_PWM_LOW_DC_3     0x80     // PWM Low Duty Cycle Register 3
	#define REG82_SDRAM_CONTROL_0  0x82     // SDRAM Control Register 0
	#define REG84_SDRAM_STATUS_0   0x84     // SDRAM Status Register 0 [WRITEONLY]
	#define REG86_SDRAM_STATUS_1   0x86     // SDRAM Status Register 1 [READONLY]
	#define REG88_SDRAM_MRS_VAL_0  0x88     // SDRAM MRS Value Register 0
	#define REG8A_SDRAM_MRS_VAL_1  0x8A     // SDRAM MRS Value Register 1
	#define REG8C_SDRAM_RFS_CNT_0  0x8C     // SDRAM Refresh Counter Register 0
	#define REG8E_SDRAM_RFS_CNT_1  0x8E     // SDRAM Refresh Counter Register 1
	#define REG90_SDRAM_BUF_SIZE   0x90     // SDRAM Write Buffer Memory Size Register
	#define REG92_SDRAM_DEBUG      0x92     // SDRAM Debug Register
	#define REG94_ALP_CONTROL      0x94     // Alpha-Blend Control Register [WRITEONLY]
	#define REG96_ALP_STATUS       0x96     // Alpha-Blend Status Register [READONLY]
	#define REG98_ALP_HR_SIZE      0x98     // Alpha-Blend Horizontal Size Register
	#define REG9A_ALP_VR_SIZE_0    0x9A     // Alpha-Blend Vertical Size Register 0
	#define REG9C_ALP_VR_SIZE_1    0x9C     // Alpha-Blend Vertical Size Register 1
	#define REG9E_ALP_VALUE        0x9E     // Alpha-Blend Value Register
	#define REGA0_ALP_IN_IMG1_SA_0 0xA0     // Alpha-Blend Input Image 1 Start Address Register 0
	#define REGA2_ALP_IN_IMG1_SA_1 0xA2     // Alpha-Blend Input Image 1 Start Address Register 1
	#define REGA4_ALP_IN_IMG1_SA_2 0xA4     // Alpha-Blend Input Image 1 Start Address Register 2
	#define REGA6_ALP_IN_IMG2_SA_0 0xA6     // Alpha-Blend Input Image 2 Start Address Register 0
	#define REGA8_ALP_IN_IMG2_SA_1 0xA8     // Alpha-Blend Input Image 2 Start Address Register 1
	#define REGAA_ALP_IN_IMG2_SA_2 0xAA     // Alpha-Blend Input Image 2 Start Address Register 2
	#define REGAC_ALP_OUT_IMG_SA_0 0xAC     // Alpha-Blend Output Image Start Address Register 0
	#define REGAE_ALP_OUT_IMG_SA_1 0xAE     // Alpha-Blend Output Image Start Address Register 1
	#define REGB0_ALP_OUT_IMG_SA_2 0xB0     // Alpha-Blend Output Image Start Address Register 2
	#define REGB2_INTERRUPT_CTRL   0xB2     // Interrupt Control Register
	#define REGB4_INTERRUPT_STAT   0xB4     // Interrupt Status Register [READONLY]
	#define REGB6_INTERRUPT_CLEAR  0xB6     // Interrupt Clear Register [WRITEONLY]
	#define REGFLAG_BASE           0xF0     // Special reserved flags beyond this point
	#define REGFLAG_DELAY          0xFC     // PLL Register Programming Delay (in us)
	#define REGFLAG_OFF_DELAY      0xFD     // LCD Panel Power Off Delay (in ms)
	#define REGFLAG_ON_DELAY       0xFE     // LCD Panel Power On Delay (in ms)
	#define REGFLAG_END_OF_TABLE   0xFF     // End of Registers Marker

#if defined (USE_DOUBLE_BUFFERING)

typedef struct
{
    WORD X;
    WORD Y;
    WORD W;
    WORD H;
} RectangleArea;

    #define GFX_BUFFER1 (0)
    #define GFX_BUFFER2 (5)
    #define PIP_BUFFER  (6)

    #define GFX_MAX_INVALIDATE_AREAS 5
#endif

#endif // _S1D13517_H
