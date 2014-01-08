/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Solomon Systech. SSD1926 LCD controller driver
 *  to be used with GFX 3 PICtail board
 *****************************************************************************
 * FileName:        SSD1926.c
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright � 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 08/27/08
 * 06/25/09     dsPIC & PIC24H support 
 * 07/30/09		Added Palette Support
 * 02/05/10		Fixed GetPixel() bug
 * 03/03/10		Fixed Circle() bug 
 * 03/26/10		Fixed Line2D() bug 
 * 07/28/10		Add USE_DRV_PUTIMAGE label to PutImage() related functions 
 *				to fix build error when driver PutImage() is not used.
 * 01/27/11     Fixed Bar() bug when clipping region is enabled. 
 * 02/11/11     Added backlight enabling after display controller and 
 *              Timing Controller (TCON) initialization. 
 * 02/14/11     - Removed USE_DRV_xxxx switches. This is not needed anymore
 *                since Primitive Layer implements weak attributes on 
 *                Primitive Routines that can be implemented in hardware.
 *              - Removed Circle Function implementation (SSD1926 bug)
 *              - Replace color data type from WORD_VAL to GFX_COLOR
 *              - Replace DeviceInit() to DriverInterfaceInit()
 *              - Optimized PutImageXBPP() & PutImageXBPPExt() for performance.
 *              - Added transparent color feature in PutImageXBPP() and
 *                PutImageXBPPExt().
 *              - Added DisplayBrightness() to control the backlight.
 *              - Added GFX_LCD_TYPE to select type of display
 * 04/01/12     - Added putimagepartial support
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined(GFX_USE_DISPLAY_CONTROLLER_SSD1926)

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/SSD1926.h"
#include "Graphics/gfxtcon.h"
#include "Graphics/Primitive.h"
  
#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_EPMP)
    #include "Graphics/gfxepmp.h"
#endif    

// Clipping region control
SHORT       _clipRgn;

// Clipping region borders
SHORT       _clipLeft;
SHORT       _clipTop;
SHORT       _clipRight;
SHORT       _clipBottom;

// Color
GFX_COLOR   _color;
#ifdef USE_TRANSPARENT_COLOR
GFX_COLOR   _colorTransparent;
SHORT       _colorTransparentEnable;
#endif

#define RED8(color16)   (BYTE) ((color16 & 0xF800) >> 8)
#define GREEN8(color16) (BYTE) ((color16 & 0x07E0) >> 3)
#define BLUE8(color16)  (BYTE) ((color16 & 0x001F) << 3)

#ifdef USE_PALETTE
extern void *_palette;
static BYTE PaletteBpp = 16;
extern BYTE blPaletteChangeError;
extern void *pPendingPalette;
extern WORD PendingStartEntry;
extern WORD PendingLength;
#endif

/*********************************************************************
* Macro:  WritePixel(color)
*
* PreCondition: none
*
* Input: color 
*
* Output: none
*
* Side Effects: none
*
* Overview: writes pixel at the current address
*
* Note: chip select should be enabled
*
********************************************************************/
#ifdef USE_16BIT_PMP
#define WritePixel(color)	DeviceWrite(color)
#else
#ifdef USE_PALETTE
#define WritePixel(color)	DeviceWrite(color)
#else
#define WritePixel(color)	{ DeviceWrite(((WORD_VAL)color).v[1]); DeviceWrite(((WORD_VAL)color).v[0]);}
#endif
#endif

/*********************************************************************
* Function:  void SetAddress(DWORD address)
*
* PreCondition: none
*
* Input: address - address 
*
* Output: none
*
* Side Effects: none
*
* Overview: sets the address for read/write operations
*
* Note: chip select should be enabled
*
********************************************************************/
void SetAddress(DWORD address)
{
    #ifdef USE_16BIT_PMP
	WORD_VAL    temp;

    DisplaySetCommand(); // set RS line to low for command

    temp.v[0] = ((DWORD_VAL) address).v[1];
    temp.v[1] = ((DWORD_VAL) address).v[2] | 0x80;
	DeviceWrite(temp.Val);
	temp.v[0] = 0x01;
    temp.v[1] = ((DWORD_VAL) address).v[0];
	DeviceWrite(temp.Val);

	DisplaySetData();    // set RS line to high for data

    #else

    DisplaySetCommand(); // set RS line to low for command

    DeviceWrite(((DWORD_VAL) address).v[2] | 0x80);
    DeviceWrite(((DWORD_VAL) address).v[1]);
    DeviceWrite(((DWORD_VAL) address).v[0]);

    DisplaySetData();    // set RS line to high for data

    #endif
}

/*********************************************************************
* Function:  void  SetReg(WORD index, BYTE value)
*
* PreCondition: none
*
* Input: index - register number
*        value - value to be set
*
* Output: none
*
* Side Effects: none
*
* Overview: sets graphics controller register (byte access)
*
* Note: none
*
********************************************************************/
void SetReg(WORD index, BYTE value)
{
    #ifdef USE_16BIT_PMP

    DisplaySetCommand(); // set RS line to low for command
    DisplayEnable();     // enable SSD1926

    DeviceWrite(((WORD_VAL) index).v[1]);
    DeviceWrite(index << 8);

	DisplaySetData();    // set RS line to high for data

    if(index & 0x0001)
		DeviceWrite(value);
    else
		DeviceWrite(value << 8);

	DisplayDisable();   // disable SSD1926

	#else

    DisplaySetCommand(); // set RS line to low for command
    DisplayEnable();     // enable SSD1926

    DeviceWrite(0x00);    // register access
    DeviceWrite(((WORD_VAL) index).v[1]);
    DeviceWrite(((WORD_VAL) index).v[0]);

	DisplaySetData();    // set RS line to high for data
	DeviceWrite(value);

	DisplayDisable();   // disable SSD1926
    #endif
}

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
BYTE GetReg(WORD index)
{
#ifdef USE_16BIT_PMP

	WORD    value;

    DisplaySetCommand(); // set RS line to low for command
    DisplayEnable();     // enable SSD1926

    DeviceWrite(((WORD_VAL) index).v[1]);
    DeviceWrite(index << 8);

	DisplaySetData();    // set RS line to high for data

	value = DeviceRead();
	value = DeviceRead();

	DisplayDisable();   // disable SSD1926

    if(index & 0x0001)
        value &= 0x00ff;
    else
        value = (value >> 8) & 0x00ff;

#else
    BYTE   value;

	DisplaySetCommand(); // set RS line to low for command
    DisplayEnable();     // enable SSD1926

    DeviceWrite(0x00);    // register access
    DeviceWrite(((WORD_VAL) index).v[1]);
    DeviceWrite(((WORD_VAL) index).v[0]);

	DisplaySetData();    // set RS line to high for data

	value = DeviceRead();
	value = DeviceRead();

	DisplayDisable();   // disable SSD1926
#endif

	return (value);
}

/*********************************************************************
* Function:  void DisplayBrightness(WORD level)
*
* PreCondition: none
*
* Input: level - Brightness level. Valid values are 0 to 100.
*			-   0: brightness level is zero or display is turned off
*			- 100: brightness level is maximum 
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
void DisplayBrightness(WORD level)
{
    // If the brightness can be controlled (for example through PWM)
    // add code that will control the PWM here.

    if (level > 0)
    {
        DisplayBacklightOn();           
    }    
    else if (level == 0)
    {
        DisplayBacklightOff();
    }    
        
}    

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/
void ResetDevice(void)
{
    /* 
        This is the reset routine of the display controller. 
        ResetDevice() recommended sequence:
        1. Initialize the interface to the controller  - DriverInterfaceInit()
        2. Initialize the registers of the controller make the enabling of 
           the display control last. In other words, the sync signals 
           are enabled only after all the registers that needs to be set 
           for the synchronization control are initialized.
        3. Before, the display is turned on, initialize the display buffer 
           to a single color so you will not see a snowy image on the screen. 
           This is done here using:
              SetColor(0);    
              ClearDevice();
           where 0 is the black color.
        4. Enable the display controller refresh of the screen
           This is done here using:
              SetReg(REG_POWER_SAVE_CONFIG, 0x00);    //  wake up      
        5. Initialize the Timing Controller (TCON) if needed.      
        6. Enable the Backlight 
        
        Most display panels will have the power-up sequence:
        1. Enable Analog Power Supply
        2. Enable Digital Power Supply - to start synchronization signals
        3. Enable Backlight - to show the contents of the display buffer
        For power-down, the reverse is followed.
        
    */

    /////////////////////////////////////////////////////////////////////
    // Initialize the device
    /////////////////////////////////////////////////////////////////////
	DriverInterfaceInit();

    // give time for the controller to power up
    DelayMs(500);

    /////////////////////////////////////////////////////////////////////
    // PLL SETUP
    // Crystal frequency x M / N = 80 MHz
    // for 4 MHz crystal:
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_PLL_CONFIG_0, 0x0a);         // set N = 10
    SetReg(REG_PLL_CONFIG_1, 0xc8);         // set M = 200
    SetReg(REG_PLL_CONFIG_2, 0xae);         // must be programmed to 0xAE
    SetReg(REG_PLL_CONFIG_0, 0x8a);         // enable PLL
    
    /////////////////////////////////////////////////////////////////////
    // VIDEO BUFFER MEMORY CLOCK SETUP
    // Memory frequency = PLL frequency / (MCLK + 1)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_MEMCLK_CONFIG, 0x00);        // set MCLK = 0 (80 MHz)
    
    /////////////////////////////////////////////////////////////////////

    // PIXEL OUTPUT CLOCK SETUP (LCD_SHIFT SIGNAL)
    // Pixel clock = Memory frequency * (PCLK + 1) / 0x100000
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_PCLK_FREQ_RATIO_0, 0x00);    // set PCLK = 0x020000
    SetReg(REG_PCLK_FREQ_RATIO_1, 0x00);    // Pixel clock = 10 MHz
    SetReg(REG_PCLK_FREQ_RATIO_2, 0x02);

    /////////////////////////////////////////////////////////////////////
    // Panel Configuration (reg 10h)
    // TFT display with 18 bit or 24-bit RGB parallel interface.
    /////////////////////////////////////////////////////////////////////
    BYTE panelType  = 0; 
    BYTE panelWidth = 0; 
    
    #if (DISP_DATA_WIDTH == 18)
        panelWidth |= 0x20;
    #elif (DISP_DATA_WIDTH == 24)
        panelWidth |= 0x30;
    #else
       #error "Define DISP_DATA_WIDTH in HardwareProfile.h (valid values: 18, 24)"
    #endif
    
    #if (GFX_LCD_TYPE == GFX_LCD_TFT)
        panelType |= 0x41;
    #elif (GFX_LCD_TYPE == GFX_LCD_CSTN)
        panelType |= 0x40;
    #elif (GFX_LCD_TYPE == GFX_LCD_MSTN)
        panelType |= 0x00;
    #else
       #error "Define GFX_LCD_TYPE in HardwareProfile.h (valid values: GFX_LCD_TFT, GFX_LCD_CSTN, GFX_LCD_MSTN)"
    #endif

    SetReg(REG_PANEL_TYPE, panelType|panelWidth);

    /////////////////////////////////////////////////////////////////////
    // Horizontal total HT (reg 12h)
    /////////////////////////////////////////////////////////////////////
    #define HT  (DISP_HOR_PULSE_WIDTH + DISP_HOR_BACK_PORCH + DISP_HOR_FRONT_PORCH + DISP_HOR_RESOLUTION)
    SetReg(REG_HORIZ_TOTAL_0, HT / 8);
    SetReg(REG_HORIZ_TOTAL_1, HT % 8);

    /////////////////////////////////////////////////////////////////////
    // Horizontal display period HDP (reg 14h)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_HDP, DISP_HOR_RESOLUTION / 8 - 1);

    /////////////////////////////////////////////////////////////////////
    // Horizontal display period start HDPS (regs 16h, 17h)
    /////////////////////////////////////////////////////////////////////
	#define HDPS (DISP_HOR_PULSE_WIDTH + DISP_HOR_BACK_PORCH)    
	SetReg(REG_HDP_START_POS0, HDPS & 0x00FF);
    SetReg(REG_HDP_START_POS1, (HDPS >> 8) & 0x00FF);

    /////////////////////////////////////////////////////////////////////
    // Horizontal syncronization pulse width HPW (reg 20h)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_HSYNC_PULSE_WIDTH, DISP_HOR_PULSE_WIDTH - 1);

    /////////////////////////////////////////////////////////////////////
    // Vertical total VT (regs 18h, 19h)
    /////////////////////////////////////////////////////////////////////
    #define VT  (DISP_VER_PULSE_WIDTH + DISP_VER_BACK_PORCH + DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION)
    SetReg(REG_VERT_TOTAL0, VT & 0x00FF);
    SetReg(REG_VERT_TOTAL1, (VT >> 8) & 0x00FF);

    /////////////////////////////////////////////////////////////////////
    // Vertical display period VDP (regs 1ch, 1dh)
    /////////////////////////////////////////////////////////////////////
    #define VDP (DISP_VER_RESOLUTION - 1)
    SetReg(REG_VDP0, VDP & 0x00FF);
    SetReg(REG_VDP1, (VDP >> 8) & 0x00FF);

    /////////////////////////////////////////////////////////////////////
    // Vertical display period start VDPS (regs 1eh, 1fh)
    /////////////////////////////////////////////////////////////////////
	#define VDPS  (DISP_VER_PULSE_WIDTH + DISP_VER_BACK_PORCH)    
	SetReg(REG_VDP_START_POS0, VDPS & 0x00FF);
    SetReg(REG_VDP_START_POS1, (VDPS >> 8) & 0x00FF);

    /////////////////////////////////////////////////////////////////////
    // Vertical syncronization pulse width VPW (reg 24h)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_VSYNC_PULSE_WIDTH, DISP_VER_PULSE_WIDTH - 1);

    /////////////////////////////////////////////////////////////////////
    // PALETTE INIT
    #ifdef USE_PALETTE
    PaletteInit();
    #endif

    /////////////////////////////////////////////////////////////////////
    // ROTATION MODE
    #if (DISP_ORIENTATION == 0)
        #define WIN_START_ADDR  0ul
        #define ROTATION        0

    #elif (DISP_ORIENTATION == 90)
        #ifndef USE_PALETTE
            #define WIN_START_ADDR  ((((DWORD) GetMaxX() + 1) >> 1) - 1)
        #else
            #define WIN_START_ADDR  (((((DWORD) GetMaxX() + 1) * PaletteBpp) >> 5) - 1)
        #endif
        #define ROTATION    1

    #elif (DISP_ORIENTATION == 180)
        #ifndef USE_PALETTE
            #define WIN_START_ADDR  (((((DWORD) GetMaxX() + 1) * (GetMaxY() + 1)) >> 1) - 1)
        #else
            #define WIN_START_ADDR  (((((DWORD) GetMaxX() + 1) * (GetMaxY() + 1) * PaletteBpp) >> 5) - 1)
        #endif
        #define ROTATION    2

    #elif (DISP_ORIENTATION == 270)
        #ifndef USE_PALETTE
            #define WIN_START_ADDR  ((((DWORD) GetMaxX() + 1) * GetMaxY()) >> 1)
        #else
            #define WIN_START_ADDR  ((((DWORD) GetMaxX() + 1) * GetMaxY() * PaletteBpp) >> 5)
        #endif
        #define ROTATION    3
    #endif

    /////////////////////////////////////////////////////////////////////
    // Special Effects Register (reg 71h)
    /////////////////////////////////////////////////////////////////////
    #ifndef USE_PALETTE
    SetReg(REG_SPECIAL_EFFECTS, 0x40 | ROTATION);
    #else
    SetReg(REG_SPECIAL_EFFECTS, 0x00 | ROTATION);
    #endif

    /////////////////////////////////////////////////////////////////////
    // Main Window Display Start Address (regs 74h, 75h, 76h)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_MAIN_WIN_DISP_START_ADDR0, WIN_START_ADDR & 0x00FF);
    SetReg(REG_MAIN_WIN_DISP_START_ADDR1, (WIN_START_ADDR >> 8) & 0x00FF);
    SetReg(REG_MAIN_WIN_DISP_START_ADDR2, (WIN_START_ADDR >> 16) & 0x00FF);

    /////////////////////////////////////////////////////////////////////
    // Main Window Display Offset (regs 78h, 79h)
    /////////////////////////////////////////////////////////////////////
    #ifndef USE_PALETTE
        #define WIN_OFFSET  ((GetMaxX() + 1) >> 1)
    #else
        #define WIN_OFFSET  (((GetMaxX() + 1) * PaletteBpp) >> 5)
    #endif
    SetReg(REG_MAIN_WIN_ADDR_OFFSET0, WIN_OFFSET & 0x00FF);
    SetReg(REG_MAIN_WIN_ADDR_OFFSET1, (WIN_OFFSET >> 8) & 0x00FF);

    /////////////////////////////////////////////////////////////////////
    // Display Mode Register (reg 70h)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_DISPLAY_MODE, 0x04);         // 16 BPP, enable RAM content to screen
    
    /////////////////////////////////////////////////////////////////////
    // RGB Settings Register (reg 1a4h)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_RGB_SETTING, 0xc0);          // RGB format

    /////////////////////////////////////////////////////////////////////
    // LSHIFT Polarity Register (reg 38h)
    /////////////////////////////////////////////////////////////////////
    #ifdef DISP_INV_LSHIFT
    SetReg(REG_LSHIFT_POLARITY, 0x01);      // 1 means falling trigger
    #endif

    /////////////////////////////////////////////////////////////////////
    // Clear the display buffer with all zeros so the display will not
    // show garbage data when initially powered up
    /////////////////////////////////////////////////////////////////////
    SetColor(0);    
    ClearDevice();
    
    /////////////////////////////////////////////////////////////////////
    // Power Saving Configuration Register (reg a0h)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_POWER_SAVE_CONFIG, 0x00);    //  wake up
    
    // check until the controller is really awake
    while(GetReg(REG_POWER_SAVE_CONFIG) == 0x00);
    
    /////////////////////////////////////////////////////////////////////
    // LCD Power Control Register (reg adh)
    // If LCD_POWER is connected to the glass DISPON or RESET
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_GPIO_STATUS_CONTROL1, 0x80); // release the glass from reset
    
    /////////////////////////////////////////////////////////////////////
    // Panel TCON Programming - set up the TCON first before turning
    // on the display. 
    /////////////////////////////////////////////////////////////////////
#ifdef USE_TCON_MODULE
    GfxTconInit();
#endif
    /////////////////////////////////////////////////////////////////////
    // Turn on the backlight
    /////////////////////////////////////////////////////////////////////
    DisplayBacklightOn();
    
}

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel
*
* Note: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y)
{
    DWORD   address;

    if(_clipRgn)
    {
        if(x < _clipLeft)
            return;
        if(x > _clipRight)
            return;
        if(y < _clipTop)
            return;
        if(y > _clipBottom)
            return;
    }

    #ifndef USE_PALETTE
    address = (((DWORD) (GetMaxX() + 1)) * y + x) << 1;
    #else
    address = ((((DWORD) (GetMaxX() + 1)) * y + x) * PaletteBpp) >> 3;
    #endif
    DisplayEnable();      // enable SSD1926
    SetAddress(address);
    WritePixel(_color);
    DisplayDisable();    // disable SSD1926
}

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates 
*
* Output: pixel color
*
* Side Effects: none
*
* Overview: returns pixel color at x,y position
*
* Note: none
*
********************************************************************/
WORD GetPixel(SHORT x, SHORT y)
{
    DWORD   address;

    address = (((DWORD) (GetMaxX() + 1)) * y + x) << 1;

    #ifdef USE_16BIT_PMP

    WORD    value;

    DisplayEnable();

    SetAddress(address);
    value = DeviceRead();
    value = DeviceRead();

    DisplayDisable();

    return (value);
    #else

    WORD_VAL    value;

    DisplayEnable();

    SetAddress(address);
    
    #if defined(USE_GFX_PMP)
	    // this first two reads are a dummy reads
	    value.Val = SingleDeviceRead();
	    value.Val = SingleDeviceRead();
	    // these are the reads that will get the actual pixel data
	    value.v[1] = SingleDeviceRead();
	    value.v[0] = SingleDeviceRead();
    #endif
    
    #if defined(USE_GFX_EPMP)
	    value.Val = DeviceRead();
	    value.v[1] = DeviceRead();
	    value.v[0] = DeviceRead();
    #endif
    DisplayDisable();

    return (value.Val);
    #endif
}

#ifdef USE_TRANSPARENT_COLOR
/*********************************************************************
* Function:  void TransparentColorEnable(GFX_COLOR color)
*
* Overview: Sets current transparent color.
*
* PreCondition: none
*
* Input: color - Color value chosen.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void TransparentColorEnable(GFX_COLOR color)
{
    _colorTransparent = color;    
    _colorTransparentEnable = TRANSPARENT_COLOR_ENABLE;

}
#endif

#ifndef USE_PALETTE
    // "In SSD1926 2D-Acceleration is not supported in Palette mode. Use Line function of Primitive layer"

/*********************************************************************
* Function: WORD Line2D(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* PreCondition: none
*
* Input: x1,y1 - starting coordinates, x2,y2 - ending coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws solid line
*
* Note: none
*
********************************************************************/
static WORD Line2D(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0);

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif

    /* Line Boundaries */
    SetReg(REG_2D_1e4, x1 & 0xFF);
    SetReg(REG_2D_1e5, (x1 >> 8) & 0xFF);
    SetReg(REG_2D_1e8, y1 & 0xFF);
    SetReg(REG_2D_1e9, (y1 >> 8) & 0xFF);
    SetReg(REG_2D_1ec, x2 & 0xFF);
    SetReg(REG_2D_1ed, (x2 >> 8) & 0xFF);
    SetReg(REG_2D_1f0, y2 & 0xFF);
    SetReg(REG_2D_1f1, (y2 >> 8) & 0xFF);

    /* Source & Destination Window Start Addresses */
    SetReg(REG_2D_1d4, 0);
    SetReg(REG_2D_1d5, 0);
    SetReg(REG_2D_1d6, 0);
    SetReg(REG_2D_1f4, 0);
    SetReg(REG_2D_1f5, 0);
    SetReg(REG_2D_1f6, 0);

    /* Display width */
    SetReg(REG_2D_1f8, (GetMaxX() + 1) & 0xFF);
    SetReg(REG_2D_1f9, ((GetMaxX() + 1) >> 8) & 0xFF);

    /* Display 2d width */
    SetReg(REG_2D_1d8, (GetMaxY() + 1) & 0xFF);
    SetReg(REG_2D_1d9, ((GetMaxY() + 1) >> 8) & 0xFF);

    /* Set Color */
    SetReg(REG_2D_1fe, RED8(_color));
    SetReg(REG_2D_1fd, GREEN8(_color));
    SetReg(REG_2D_1fc, BLUE8(_color));

    /* 16bpp */
    SetReg(REG_2D_1dd, 0x00);

    /* Line command */
    SetReg(REG_2D_1d1, 0x01);

    /* Draw2d command */
    SetReg(REG_2D_1d2, 0x01);

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0);

    /* Ready */
        #endif
    return (1);
}

/*********************************************************************
* Function: WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* PreCondition: none
*
* Input: x1,y1 - starting coordinates, x2,y2 - ending coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws line
*
* Note: none
*
********************************************************************/
WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{

    SHORT   deltaX, deltaY;
    SHORT   error, stepErrorLT, stepErrorGE;
    SHORT   stepX, stepY;
    SHORT   steep;
    SHORT   temp;
    SHORT   style, type;

    stepX = 0;
    deltaX = x2 - x1;
    if(deltaX < 0)
    {
        deltaX = -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY = 0;
    deltaY = y2 - y1;
    if(deltaY < 0)
    {
        deltaY = -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep = 0;
    if(deltaX < deltaY)
    {
        ++steep;
    }

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0);

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif
    if(_lineType == 0)
    {
        if(!Line2D(x1, y1, x2, y2))
            return (0);
        if(_lineThickness)
        {
            if(steep)
            {
                while(!Line2D(x1 + 1, y1, x2 + 1, y2));
                while(!Line2D(x1 - 1, y1, x2 - 1, y2));
            }
            else
            {
                while(!Line2D(x1, y1 + 1, x2, y2 + 1));
                while(!Line2D(x1, y1 - 1, x2, y2 - 1));
            }
        }

        // Move cursor
        MoveTo(x2, y2);
        return (1);
    }

    // Move cursor
    MoveTo(x2, y2);

    if(x1 == x2)
    {
        if(y1 > y2)
        {
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = y1; temp < y2 + 1; temp++)
        {
            if((++style) == _lineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                PutPixel(x1, temp);
                if(_lineThickness)
                {
                    PutPixel(x1 + 1, temp);
                    PutPixel(x1 - 1, temp);
                }
            }
        }

        return (1);
    }

    if(y1 == y2)
    {
        if(x1 > x2)
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = x1; temp < x2 + 1; temp++)
        {
            if((++style) == _lineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                PutPixel(temp, y1);
                if(_lineThickness)
                {
                    PutPixel(temp, y1 + 1);
                    PutPixel(temp, y1 - 1);
                }
            }
        }

        return (1);
    }

    if(deltaX < deltaY)
    {
        temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = stepX;
        stepX = stepY;
        stepY = temp;
        PutPixel(y1, x1);
    }
    else
    {
        PutPixel(x1, y1);
    }

    // If the current error greater or equal zero
    stepErrorGE = deltaX << 1;

    // If the current error less than zero
    stepErrorLT = deltaY << 1;

    // Error for the first pixel
    error = stepErrorLT - deltaX;

    style = 0;
    type = 1;

    while(--deltaX >= 0)
    {
        if(error >= 0)
        {
            y1 += stepY;
            error -= stepErrorGE;
        }

        x1 += stepX;
        error += stepErrorLT;

        if((++style) == _lineType)
        {
            type ^= 1;
            style = 0;
        }

        if(type)
        {
            if(steep)
            {
                PutPixel(y1, x1);
                if(_lineThickness)
                {
                    PutPixel(y1 + 1, x1);
                    PutPixel(y1 - 1, x1);
                }
            }
            else
            {
                PutPixel(x1, y1);
                if(_lineThickness)
                {
                    PutPixel(x1, y1 + 1);
                    PutPixel(x1, y1 - 1);
                }
            }
        }
    }   // end of while

    return (1);
}

#endif // #ifndef USE_PALETTE

/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/
WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{

        #ifdef USE_PALETTE

    DWORD           address;
    register SHORT  x, y;

    if(_clipRgn)
    {
		left   = (left   < _clipLeft)   ? _clipLeft   : ((left   > _clipRight)  ? _clipRight  : left);
		right  = (right  > _clipRight)  ? _clipRight  : ((right  < _clipLeft)   ? _clipLeft   : right);
		top    = (top    < _clipTop)    ? _clipTop    : ((top    > _clipBottom) ? _clipBottom : top);
		bottom = (bottom > _clipBottom) ? _clipBottom : ((bottom < _clipTop)    ? _clipTop    : bottom);
    }

            #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
            #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
            #endif
    DisplayEnable();
    for(y = top; y < bottom + 1; y++)
    {
        SetAddress(address);
        for(x = left; x < right + 1; x++)
        {
            WritePixel(_color);
        }

                #ifndef USE_PALETTE
        address += (GetMaxX() + 1) << 1;
                #else
        address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
                #endif
    }

    DisplayDisable();
    return (1);

        #else

    DWORD   address;
    SHORT   width, height;

#ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0);

    /* Ready */
#else
    if(IsDeviceBusy() != 0)
        return (0);
#endif

    if(left > right)
    {
        return (1); /* Don't draw but return 1 */
    }

    if(top > bottom)
    {
        return (1); /* Don't draw but return 1 */
    }

    if(_clipRgn)
    {
		left   = (left   < _clipLeft)   ? _clipLeft   : ((left   > _clipRight)  ? _clipRight  : left);
		right  = (right  > _clipRight)  ? _clipRight  : ((right  < _clipLeft)   ? _clipLeft   : right);
		top    = (top    < _clipTop)    ? _clipTop    : ((top    > _clipBottom) ? _clipBottom : top);
		bottom = (bottom > _clipBottom) ? _clipBottom : ((bottom < _clipTop)    ? _clipTop    : bottom);
    }

#ifdef USE_ALPHABLEND_LITE
    if(GetAlpha() != 100)
    {
        if (BarAlpha(left,top,right,bottom))
            return 1;
        else
            return 0;
    }
#endif

    width = right - left + 1;
    height = bottom - top + 1;

    address = top * (GetMaxX() + (DWORD) 1) + left;

    PutPixel(left, top);

    /* Source, Destination & Brush Window Start Addresses */
    SetReg(REG_2D_1d4, address & 0xFF);
    SetReg(REG_2D_1d5, (address >> 8) & 0xFF);
    SetReg(REG_2D_1d6, (address >> 16) & 0xFF);
    SetReg(REG_2D_1f4, address & 0xFF);
    SetReg(REG_2D_1f5, (address >> 8) & 0xFF);
    SetReg(REG_2D_1f6, (address >> 16) & 0xFF);
    SetReg(REG_2D_204, address & 0xFF);
    SetReg(REG_2D_205, (address >> 8) & 0xFF);
    SetReg(REG_2D_206, (address >> 16) & 0xFF);

    /* Source & Destination Window Width */
    SetReg(REG_2D_1ec, width & 0xFF);
    SetReg(REG_2D_1ed, (width >> 8) & 0xFF);
    SetReg(REG_2D_1e4, width & 0xFF);
    SetReg(REG_2D_1e5, (width >> 8) & 0xFF);

    /* Source & Destination Window Height */
    SetReg(REG_2D_1f0, height & 0xFF);
    SetReg(REG_2D_1f1, (height >> 8) & 0xFF);
    SetReg(REG_2D_1e8, height & 0xFF);
    SetReg(REG_2D_1e9, (height >> 8) & 0xFF);

    /* Brush width */
    SetReg(REG_2D_214, 1);
    SetReg(REG_2D_215, 0);

    /* Brush height */
    SetReg(REG_2D_218, 1);
    SetReg(REG_2D_219, 0);

    /* Display width */
    SetReg(REG_2D_1f8, (GetMaxX() + 1) & 0xFF);
    SetReg(REG_2D_1f9, ((GetMaxX() + 1) >> 8) & 0xFF);

    /* Display 2d width */
    SetReg(REG_2D_1d8, (GetMaxX() + 1) & 0xFF);
    SetReg(REG_2D_1d9, ((GetMaxX() + 1) >> 8) & 0xFF);

    /* ROP3 Command */
    SetReg(REG_2D_1fc, 0xF0);

    /* 16bpp */
    SetReg(REG_2D_1dd, 0x00);

    /* ROP command */
    SetReg(REG_2D_1d1, 0x09);

    /* Draw2d command */
    SetReg(REG_2D_1d2, 0x01);

            #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0);

    /* Ready */
            #endif
    return (1);
        #endif
}

/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color 
*
* Note: none
*
********************************************************************/
void ClearDevice(void)
{
        #ifdef USE_PALETTE

    DWORD   counter;

    DisplayEnable();
    SetAddress(0);
    SetAddress(0);
    for(counter = 0; counter < (DWORD) (GetMaxX() + 1) * (GetMaxY() + 1); counter++)
    {
        WritePixel(_color);
    }

    DisplayDisable();

        #else
    while(GetReg(REG_2D_220) == 0);
    while(!Bar(0, 0, GetMaxX(), GetMaxY()));
    while(GetReg(REG_2D_220) == 0);

    /* Ready */
        #endif
}


#ifdef USE_BITMAP_FLASH
/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD      address;
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    BYTE                temp = 0, stretchY, mask;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                pallete[2];

    WORD                addressOffset = 0, adjOffset;
    BYTE                OffsetFlag = 0x01;     //Offset from BYTE color bit0 for the partial image 

    // Move pointer to size information
    flashAddress = image + 2;

    // Set start address
        #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
        #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
        #endif

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[0] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[1] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    if(sizeX & 0x07) 
        adjOffset = 1;
    else
        adjOffset = 0;
    
    // compute for addressOffset this is the offset needed to jump to the 
    // next line 
    addressOffset = (sizeX>>3)+adjOffset;

    if(pPartialImageData->width != 0)
    {

         WORD mod3 = ((pPartialImageData->xoffset) & 0x07);

        // adjust the flashAddress to the starting pixel location
        // adjust one address if the data is not byte aligned
         flashAddress += (pPartialImageData->yoffset)*((sizeX>>3)+adjOffset);

        // adjust flashAddress for x offset (if xoffset is zero address stays the same)
         flashAddress += (pPartialImageData->xoffset)>>3;

         OffsetFlag <<= mod3;

         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }

    // store current line data address 
    tempFlashAddress = flashAddress;

    DisplayEnable();      // enable SSD1926

   // Note: For speed the code for loops are repeated. A small code size increase for performance
    
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            SetAddress(address);

            // get flash address location of current line being processed
            flashAddress = tempFlashAddress;

            if (OffsetFlag != 0x01)
    		{
                // grab the first set of data then set up the mask to the starting pixel
    			temp = *flashAddress++;
            }
            mask = OffsetFlag;

            for(x = 0; x < sizeX; x++)
            {
            	// Read 8 pixels from flash
                if(mask == 0x01)
			    {
				    temp = *flashAddress++;
                    mask = 0x01;
                }

                // Set color
                if(mask & temp)
                {
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == 1) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else    
                        #endif
                            WritePixel(1);
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[1]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else    
                        #endif
                            WritePixel(pallete[1]);
                    }
                }
                else
                {
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == 0) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else    
                        #endif
                            WritePixel(0);
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[0]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else    
                        #endif
                            WritePixel(pallete[0]);
                    }
                }
                               // Read 8 pixels from flash
                if(mask == 0x80)
                {
                    mask = 0x01;
                }
                else
                {
                    // Shift to the next pixel
                    mask <<= 1;
                }    
            }
            tempFlashAddress += (addressOffset);
    
            #ifndef USE_PALETTE
                address += (GetMaxX() + 1) << 1;
            #else
                address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
            #endif
        }
   } 
   else
   {    
        for(y = 0; y < sizeY; y++)
        {
            for(stretchY = 0; stretchY < stretch; stretchY++)
            {
                // get flash address location of current line being processed
                flashAddress = tempFlashAddress;
                SetAddress(address);

                if (OffsetFlag != 0x01)
    		    {
                    // grab the first set of data then set up the mask to the starting pixel
    			    temp = *flashAddress++;
                }
                mask = OffsetFlag;

                for(x = 0; x < sizeX; x++)
                {
                    // Read 8 pixels from flash
                    if(mask == 0x01)
			        {
				        temp = *flashAddress++;
                        mask = 0x01;
                    }

                    // Set color
                    if(mask & temp)
                    {
                        #ifdef USE_PALETTE
                        if(IsPaletteEnabled())
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == 1) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }   
                            else 
                            #endif
                            {
                                WritePixel(1);
                                WritePixel(1);
                            }    

                        }
                        else
                        #endif
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == pallete[1]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }   
                            else 
                            #endif
                            {
                                WritePixel(pallete[1]);
                                WritePixel(pallete[1]);
                            }    
                        }
                    }
                    else
                    {
                        #ifdef USE_PALETTE
                        if(IsPaletteEnabled())
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == 0) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }   
                            else 
                            #endif
                            {
                                WritePixel(0);
                                WritePixel(0);
                            }    
                        }
                        else
                        #endif
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == pallete[0]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }   
                            else 
                            #endif
                            {
                                WritePixel(pallete[0]);
                                WritePixel(pallete[0]);
                            }    
                        }
                    }

                    if(mask == 0x80)
                    {
                        mask = 0x01;
                    }
                    else
                    {
                        // Shift to the next pixel
                        mask <<= 1;
                    }     
                }

                flashAddress += (addressOffset);

                #ifndef USE_PALETTE
                    address += (GetMaxX() + 1) << 1;
                #else
                    address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
                #endif
            }
            tempFlashAddress += (addressOffset);
        }
   }    
    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 16 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD      address;
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    BYTE                temp = 0, stretchY;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                pallete[16];
    WORD                counter;
    WORD                addressOffset = 0;
    BYTE                OffsetFlag = 0;

    // Move pointer to size information
    flashAddress = image + 2;

    // Set start address
        #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
        #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
        #endif

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter = 0; counter < 16; counter++)
    {
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    if(pPartialImageData->width != 0)
    {
         WORD mod1 = (sizeX & 1);
         WORD mod2 = (pPartialImageData->width) & 1; 
         WORD mod3 = (pPartialImageData->xoffset) & 1; 

         flashAddress += (pPartialImageData->yoffset)*((sizeX>>1)+mod1);
         flashAddress += ((pPartialImageData->xoffset)>>1);                

         addressOffset = (sizeX>>1)+mod1;
         addressOffset -= ((pPartialImageData->width)>>1);
         addressOffset -= mod3;
         OffsetFlag = (mod3);

          if(OffsetFlag == 0)
          {
            addressOffset -= mod2;
          }
         
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
         
    }

    DisplayEnable();      // enable SSD1926

   // Note: For speed the code for loops are repeated. A small code size increase for performance
    
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            SetAddress(address);
            temp = *flashAddress;
            flashAddress += OffsetFlag;
            for(x = OffsetFlag; x < (sizeX+OffsetFlag); x++)
            { 
                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == (temp >> 4)) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                        }
                        else
                        #endif
                            WritePixel(temp >> 4);
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[temp >> 4]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                        }        
                        else
                        #endif
                            WritePixel(pallete[temp >> 4]);
                    }
                }
                else
                {
                    temp = *flashAddress++;

                    // first pixel in byte
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == (temp & 0x0f)) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                        }
                        else
                        #endif    
                            WritePixel(temp & 0x0f);
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[temp & 0x0f]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                        }                            
                        else
                        #endif
                            WritePixel(pallete[temp & 0x0f]);
                    }
                }
            }

            flashAddress += (addressOffset);

            #ifndef USE_PALETTE
                address += (GetMaxX() + 1) << 1;
            #else
                address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
            #endif
        }
   } 
   else
   {    
        for(y = 0; y < sizeY; y++)
        {
            tempFlashAddress = flashAddress;
            for(stretchY = 0; stretchY < stretch; stretchY++)
            {
                flashAddress = tempFlashAddress;
                SetAddress(address);
                temp = *flashAddress;
                flashAddress += OffsetFlag;

            for(x = OffsetFlag; x < (sizeX+OffsetFlag); x++)
                {
                    // Read 2 pixels from flash
                    if(x & 0x0001)
                    {
    
                        // second pixel in byte
                        #ifdef USE_PALETTE
                        if(IsPaletteEnabled())
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == (temp >> 4)) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }
                            else
                            #endif
                            {
                                WritePixel(temp >> 4);
                                WritePixel(temp >> 4);
                            }    
                        }
                        else
                        #endif
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == pallete[temp >> 4]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else
                            #endif
                            {
                                WritePixel(pallete[temp >> 4]);
                                WritePixel(pallete[temp >> 4]);
                            }    
                        }
                    }
                    else
                    {
                        temp = *flashAddress++;
    
                        // first pixel in byte
                        #ifdef USE_PALETTE
                        if(IsPaletteEnabled())
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == (temp & 0x0f)) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else
                            #endif
                            {    
                                WritePixel(temp & 0x0f);
                                WritePixel(temp & 0x0f);
                            }    
                        }
                        else
                        #endif
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == pallete[temp & 0x0f]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                           }    
                            else
                            #endif
                            {
                                WritePixel(pallete[temp & 0x0f]);
                                WritePixel(pallete[temp & 0x0f]);
                            }    
                        }
                    }
                }

                flashAddress += (addressOffset);

                #ifndef USE_PALETTE
                    address += (GetMaxX() + 1) << 1;
                #else
                    address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
                #endif
            }
        }
   }    

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 256 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD      address;
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    BYTE                temp, stretchY;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                pallete[256];
    WORD                counter;
    WORD                addressOffset = 0;

    // Move pointer to size information
    flashAddress = image + 2;

    // Set start address
        #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
        #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
        #endif

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter = 0; counter < 256; counter++)
    {
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    if(pPartialImageData->width != 0)
    {
         flashAddress += pPartialImageData->xoffset + pPartialImageData->yoffset*(sizeX);
         addressOffset = sizeX - pPartialImageData->width;
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }

    DisplayEnable();      // enable SSD1926

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            SetAddress(address);
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress;
                flashAddress++;

                // Write pixel to screen
                #ifdef USE_PALETTE
                if(IsPaletteEnabled())
                {
                    #ifdef USE_TRANSPARENT_COLOR
                    if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                    {
                        DeviceReadWord();
                    }    
                    else
                    #endif
                        WritePixel(temp); 
                }
                else
                #endif
                {
                    #ifdef USE_TRANSPARENT_COLOR
                    if ((GetTransparentColor() == pallete[temp]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                    {
                        DeviceReadWord();
                    }    
                    else
                    #endif
                        WritePixel(pallete[temp]);
                }

            }

            flashAddress += addressOffset;

            #ifndef USE_PALETTE
                address += (GetMaxX() + 1) << 1;
            #else
                address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
            #endif
        }
   } 
   else
   {    
        for(y = 0; y < sizeY; y++)
        {
            tempFlashAddress = flashAddress;
            for(stretchY = 0; stretchY < stretch; stretchY++)
            {
                flashAddress = tempFlashAddress;
                SetAddress(address);
    
                for(x = 0; x < sizeX; x++)
                {
                    temp = *flashAddress;
                    flashAddress++;

                   // Write pixel to screen
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                            DeviceReadWord();
                        }    
                        else
                        #endif                        
                        {
                            WritePixel(temp); 
                            WritePixel(temp); 
                        }    
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[temp]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                            DeviceReadWord();
                        }    
                        else
                        #endif                        
                        {
                            WritePixel(pallete[temp]);
                            WritePixel(pallete[temp]);
                        }    
                    }

                }

                flashAddress += addressOffset;

                #ifndef USE_PALETTE
                    address += (GetMaxX() + 1) << 1;
                #else
                    address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
                #endif
            }
        }
   }    

   DisplayDisable();
}

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs hicolor image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD      address;
    register FLASH_WORD *flashAddress;
    register FLASH_WORD *tempFlashAddress;
    WORD                temp, stretchY;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                addressOffset = 0;

    // Move pointer to size information
    flashAddress = (FLASH_WORD *)image + 1;

    // Set start address
        #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
        #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
        #endif

    // Read image size
    sizeY = *flashAddress;
    flashAddress++;
    sizeX = *flashAddress;
    flashAddress++;

    if(pPartialImageData->width != 0)
    {
         flashAddress += pPartialImageData->xoffset + pPartialImageData->yoffset*(sizeX);
         addressOffset = sizeX - pPartialImageData->width;
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }
    
    DisplayEnable();      // enable SSD1926

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            SetAddress(address);
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress;
                flashAddress++;

                // Write pixel to screen
                #ifdef USE_PALETTE
                if(IsPaletteEnabled())
                {
                    #ifdef USE_TRANSPARENT_COLOR
                    if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                    {
                        DeviceReadWord();
                    }    
                    else                   
                    #endif
                        WritePixel(0); /* Paint first value of Palette instead of junk */
                }
                else
                #endif
                {
                    #ifdef USE_TRANSPARENT_COLOR
                    if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                    {
                        DeviceReadWord();
                    }    
                    else
                    #endif
                        WritePixel(temp);
                }

            }

            flashAddress += addressOffset;

            #ifndef USE_PALETTE
                address += (GetMaxX() + 1) << 1;
            #else
                address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
            #endif
        }
   } 
   else
   {    
        for(y = 0; y < sizeY; y++)
        {
            tempFlashAddress = flashAddress;
            for(stretchY = 0; stretchY < stretch; stretchY++)
            {
                flashAddress = tempFlashAddress;
                SetAddress(address);
    
                for(x = 0; x < sizeX; x++)
                {
                    temp = *flashAddress;
                    flashAddress++;

                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                            DeviceReadWord();
                        }    
                        else
                        #endif
                        {
                            WritePixel(0); /* Paint first value of Palette instead of junk */
                            WritePixel(0);
                        }    
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                            DeviceReadWord();
                        } 
                        else   
                        #endif
                        {
                            WritePixel(temp);
                            WritePixel(temp);
                        }    
                    }    
                }

                flashAddress += addressOffset;

                #ifndef USE_PALETTE
                    address += (GetMaxX() + 1) << 1;
                #else
                    address += ((GetMaxX() + 1) * PaletteBpp) >> 3;
                #endif
            }
        }
   }    
   
    DisplayDisable();
}

#endif
#ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  address;
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[2];
    BYTE            lineBuffer[((GetMaxX() + 1) / 8) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0, stretchY;
    BYTE            mask;
    WORD            sizeX, sizeY, lineLength;
    WORD            x, y;
    BYTE            OffsetFlag = 0x01;     //Offset from BYTE color bit0 for the partial image

    // Set start address
        #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
        #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
        #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (2 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 2 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 2 * sizeof(WORD);

    // Line width in bytes
    byteWidth = (bmp.width >> 3);
    if(bmp.width & 0x0007)
        byteWidth++;

    if(pPartialImageData->width != 0)
    {
         memOffset += pPartialImageData->yoffset*byteWidth;
         memOffset += (pPartialImageData->xoffset)>>3;

         OffsetFlag <<= ((pPartialImageData->xoffset) & 0x07);

         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

    // calculate the length of bytes needed per line
    lineLength = (sizeX >> 3) + 1;
    if (sizeX & 0x07)
        lineLength++;

    // Note: For speed the code for loops are repeated. A small code size increase for performance

    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
    
            // Get line
            ExternalMemoryCallback(image, memOffset, lineLength, lineBuffer);
            memOffset += byteWidth;
            DisplayEnable();      // enable SSD1926

            pData = lineBuffer;
            SetAddress(address);
            mask = OffsetFlag;
            temp = *pData++;
            for(x = 0; x < sizeX; x++)
            {
                // Set color
                if(mask & temp)
                {
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == 1) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else 
                        #endif
                            WritePixel(1);
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[1]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else 
                        #endif
                            WritePixel(pallete[1]);
                    }
                }
                else
                {
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == 0) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else 
                        #endif
                            WritePixel(0);
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[0]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else 
                        #endif
                            WritePixel(pallete[0]);
                    }
                }

                // Read 8 pixels from flash
                if(mask == 0x80)
                {
                    temp = *pData++;
                    mask = 0x01;
                }
                else
                {
                    // Shift to the next pixel
                    mask <<= 1;
                }  
            }
    
            address += (GetMaxX() + 1) << 1;
            DisplayDisable();
        }
   } 
   else
   {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, lineLength, lineBuffer);
            memOffset += byteWidth;
            DisplayEnable();      // enable SSD1926
            for(stretchY = 0; stretchY < stretch; stretchY++)
            {
                pData = lineBuffer;
                SetAddress(address);
                mask = OffsetFlag;
                temp = *pData++;

                for(x = 0; x < sizeX; x++)
                {
                    // Set color
                    if(mask & temp)
                    {
                        #ifdef USE_PALETTE
                        if(IsPaletteEnabled())
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == 1) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else
                            #endif
                            {
                                WritePixel(1);
                                WritePixel(1);
                            }    
                        }
                        else
                        #endif
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == pallete[1]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else
                            #endif
                            {
                                WritePixel(pallete[1]);
                                WritePixel(pallete[1]);
                            }    
                        }
                    }
                    else
                    {
                        #ifdef USE_PALETTE
                        if(IsPaletteEnabled())
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == 0) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else
                            #endif
                            {
                                WritePixel(0);
                                WritePixel(0);
                            }    
                        }
                        else
                        #endif
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == pallete[0]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else
                            #endif
                            {
                                WritePixel(pallete[0]);
                                WritePixel(pallete[0]);
                            }    
                        }
                    }

                // Read 8 pixels from flash
                if(mask == 0x80)
                {
                    temp = *pData++;
                    mask = 0x01;
                }
                else
                {
                    // Shift to the next pixel
                    mask <<= 1;
                } 
                }
                address += (GetMaxX() + 1) << 1;
            }
            DisplayDisable();
        }
   }    
}
/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  address;
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[16];
    BYTE            lineBuffer[((GetMaxX() + 1) / 2) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0, stretchY;
    WORD            sizeX, sizeY, lineLength;
    WORD            x, y;
    WORD            nibbleOffset = 0; 

    // Set start address
        #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
        #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
        #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (16 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);

    // Line width in bytes
    byteWidth = (bmp.width) >> 1;

    // if the width is odd, add another byte count since the shift 1 caused
    // us to loose a nibble
    if(bmp.width & 0x01)
        byteWidth++;
    
    if(pPartialImageData->width != 0)
    {
        // check the bit position of the starting pixel
        nibbleOffset = pPartialImageData->xoffset & 0x01;

        memOffset += (pPartialImageData->yoffset)*byteWidth;
        memOffset += (pPartialImageData->xoffset) >> 1;

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }


    // calculate the length of bytes needed per line
    lineLength = (sizeX >> 1) + 1;
    if (sizeX & 0x01)
        lineLength++;

    // Note: For speed the code for loops are repeated. A small code size increase for performance

    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
    
            // Get line
            ExternalMemoryCallback(image, memOffset, lineLength, lineBuffer);
            memOffset += byteWidth;

            pData = lineBuffer;
            if (nibbleOffset)
            {
                temp = *pData++;
            }

            DisplayEnable();      // enable SSD1926
            SetAddress(address);
    
            for(x = nibbleOffset; x < (sizeX + nibbleOffset); x++)
            {
                // Read 2 pixels from flash
                if(x & 0x0001)
                {
                    // second pixel in byte
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == (temp >> 4)) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else    
                        #endif
                            WritePixel(temp >> 4);
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[temp >> 4]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else    
                        #endif
                            WritePixel(pallete[temp >> 4]);
                    }
                }
                else
                {
                    temp = *pData++;

                    // first pixel in byte
                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == (temp & 0x0f)) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else    
                        #endif
                            WritePixel(temp & 0x0f);
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[temp & 0x0f]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            DeviceReadWord();
                        else    
                        #endif
                            WritePixel(pallete[temp & 0x0f]);
                    }
                }
            }
    
            address += (GetMaxX() + 1) << 1;
            DisplayDisable();
        }
   } 
   else
   {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, lineLength, lineBuffer);
            memOffset += byteWidth;


            DisplayEnable();      // enable SSD1926

            for(stretchY = 0; stretchY < stretch; stretchY++)
            {
                //pData = lineBuffer;
                pData = lineBuffer;
                if (nibbleOffset)
                {
                    temp = *pData++;
                }
                SetAddress(address);
    
                for(x = nibbleOffset; x < (sizeX + nibbleOffset); x++)
                {

                    // Read 2 pixels from flash
                    if(x & 0x0001)
                    {
                        // second pixel in byte
                        #ifdef USE_PALETTE
                        if(IsPaletteEnabled())
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == (temp >> 4)) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else    
                            #endif
                            {
                                WritePixel(temp >> 4);
                                WritePixel(temp >> 4);
                            }    
                        }
                        else
                        #endif
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == pallete[temp >> 4]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else    
                            #endif
                            {
                                WritePixel(pallete[temp >> 4]);
                                WritePixel(pallete[temp >> 4]);
                            }    
                        }
                    }
                    else
                    {
                        temp = *pData++;
    
                        // first pixel in byte
                        #ifdef USE_PALETTE
                        if(IsPaletteEnabled())
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == (temp & 0x0f)) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else    
                            #endif
                            {
                                WritePixel(temp & 0x0f);
                                WritePixel(temp & 0x0f);
                            }    
                        }
                        else
                        #endif
                        {
                            #ifdef USE_TRANSPARENT_COLOR
                            if ((GetTransparentColor() == pallete[temp & 0x0f]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                            {
                                DeviceReadWord();
                                DeviceReadWord();
                            }    
                            else    
                            #endif
                            {
                                WritePixel(pallete[temp & 0x0f]);
                                WritePixel(pallete[temp & 0x0f]);
                            }    
                        }
                    }
                }
                address += (GetMaxX() + 1) << 1;
            }
            DisplayDisable();
        }
   }    
}
/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  address;
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[256];
    BYTE            lineBuffer[(GetMaxX() + 1)];
    BYTE            *pData;

    BYTE            temp, stretchY;
    WORD            sizeX, sizeY;
    WORD            x, y;

    // Set start address
        #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
        #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
        #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (256 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 256 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(WORD);

    if(pPartialImageData->width != 0)
    {
         memOffset += pPartialImageData->xoffset + pPartialImageData->yoffset*(bmp.width);
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

    // Note: For speed the code for loops are repeated. A small code size increase for performance

    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
    
            // Get line
            ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
            memOffset += bmp.width;
            DisplayEnable();      // enable SSD1926

            pData = lineBuffer;
            SetAddress(address);
    
            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;
                
                #ifdef USE_PALETTE
                if(IsPaletteEnabled())
                {
                    #ifdef USE_TRANSPARENT_COLOR
                    if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        DeviceReadWord();
                    else    
                    #endif
                        WritePixel(temp);
                }
                else
                #endif
                {
                    #ifdef USE_TRANSPARENT_COLOR
                    if ((GetTransparentColor() == pallete[temp]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        DeviceReadWord();
                    else    
                    #endif
                        WritePixel(pallete[temp]);
                }
            }
    
            address += (GetMaxX() + 1) << 1;
            DisplayDisable();
        }
   } 
   else
   {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
            memOffset += bmp.width;
            DisplayEnable();      // enable SSD1926
            for(stretchY = 0; stretchY < stretch; stretchY++)
            {
                pData = lineBuffer;
                SetAddress(address);
    
                for(x = 0; x < sizeX; x++)
                {
                    temp = *pData++;

                    #ifdef USE_PALETTE
                    if(IsPaletteEnabled())
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                            DeviceReadWord();
                        }    
                        else    
                        #endif
                        {
                            WritePixel(temp);
                            WritePixel(temp);
                        }    
                    }
                    else
                    #endif
                    {
                        #ifdef USE_TRANSPARENT_COLOR
                        if ((GetTransparentColor() == pallete[temp]) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        {
                            DeviceReadWord();
                            DeviceReadWord();
                        }    
                        else    
                        #endif
                        {
                            WritePixel(pallete[temp]);
                            WritePixel(pallete[temp]);
                        }    
                    }
        
                }
                address += (GetMaxX() + 1) << 1;
            }
            DisplayDisable();
        }
   }    
}
/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  address;
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            lineBuffer[(GetMaxX() + 1)];
    WORD            *pData;
    WORD            byteWidth;

    WORD            temp, stretchY;
    WORD            sizeX, sizeY;
    WORD            x, y;

    // Set start address
        #ifndef USE_PALETTE
    address = ((DWORD) (GetMaxX() + 1) * top + left) << 1;
        #else
    address = (((DWORD) (GetMaxX() + 1) * top + left) * PaletteBpp) >> 3;
        #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER);

    if(pPartialImageData->width != 0)
    {
         memOffset += (pPartialImageData->xoffset + pPartialImageData->yoffset*(bmp.width))<<1;
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

    byteWidth = bmp.width << 1;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
  
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
    
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            DisplayEnable();      // enable SSD1926

            pData = lineBuffer;
            SetAddress(address);
    
            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;
    
                // Write pixel to screen
                    #ifdef USE_TRANSPARENT_COLOR
                    if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                        DeviceReadWord();
                    else
                    #endif
                        WritePixel(temp);
                        
            }
    
            address += (GetMaxX() + 1) << 1;
            DisplayDisable();
        }
   } 
   else
   {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            DisplayEnable();      // enable SSD1926
            for(stretchY = 0; stretchY < stretch; stretchY++)
            {
                pData = lineBuffer;
                SetAddress(address);
    
                for(x = 0; x < sizeX; x++)
                {
                    temp = *pData++;
    
                    #ifdef USE_TRANSPARENT_COLOR
                    if ((GetTransparentColor() == temp) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE))
                    {    
                        DeviceReadWord();
                        DeviceReadWord();
                    }    
                    else
                    #endif
                    {
                        WritePixel(temp);
                        WritePixel(temp);
                    }    
                }
                address += (GetMaxX() + 1) << 1;
            }
            DisplayDisable();
        }
   }    
}
#endif


/*********************************************************************
* Function: IsDeviceBusy()
*
* Overview: Returns non-zero if LCD controller is busy 
*           (previous drawing operation is not completed).
*
* PreCondition: none
*
* Input: none
*
* Output: Busy status.
*
* Side Effects: none
*
********************************************************************/
WORD IsDeviceBusy(void)
{  
    return ((GetReg(REG_2D_220) & 0x01) == 0);
}

#ifdef USE_PALETTE

/*********************************************************************
* Function: void PaletteInit(void)
*
* Overview: Initializes the CLUT.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: Drawing mode will change to support palettes
*
********************************************************************/
void PaletteInit(void)
{
    PaletteBpp = 16;
    blPaletteChangeError = 0;
    pPendingPalette = NULL;
    PendingStartEntry = 0;
    PendingLength = 0;
}

/*********************************************************************
* Function: BYTE SetPaletteBpp(BYTE bpp)
*
* Overview: Sets the CLUT's number of valid entries.
*
* PreCondition: PaletteInit() must be called before.
*
* Input: bpp -> Bits per pixel
*
* Output: Status: Zero -> Success, Non-zero -> Error.
*
* Side Effects: Drawing mode will change to support palettes
*
********************************************************************/
BYTE SetPaletteBpp(BYTE bpp)
{
    switch(bpp)
    {
        /*case 1: SetReg(REG_DISPLAY_MODE,0x00);
                break;

        case 2: SetReg(REG_DISPLAY_MODE,0x01);
                break;

        case 4: SetReg(REG_DISPLAY_MODE,0x02);
                break;*/
        case 8:     SetReg(REG_DISPLAY_MODE, 0x03); break;
        case 16:    SetReg(REG_DISPLAY_MODE, 0x04); break;
        default:    SetReg(REG_DISPLAY_MODE, 0x04); PaletteBpp = 16; return (-1);
    }

    PaletteBpp = bpp;

    /////////////////////////////////////////////////////////////////////
    // ROTATION MODE
        /////////////////////////////////////////////////////////////////////
        #if (DISP_ORIENTATION == 0)
            #define WIN_START_ADDR  0ul
            #define ROTATION        0

        #elif (DISP_ORIENTATION == 90)
            #ifndef USE_PALETTE
                #define WIN_START_ADDR  ((((DWORD) GetMaxX() + 1) >> 1) - 1)
            #else
                #define WIN_START_ADDR  (((((DWORD) GetMaxX() + 1) * PaletteBpp) >> 5) - 1)
            #endif
            #define ROTATION    1

        #elif (DISP_ORIENTATION == 180)
            #ifndef USE_PALETTE
                #define WIN_START_ADDR  (((((DWORD) GetMaxX() + 1) * (GetMaxY() + 1)) >> 1) - 1)
            #else
                #define WIN_START_ADDR  (((((DWORD) GetMaxX() + 1) * (GetMaxY() + 1) * PaletteBpp) >> 5) - 1)
            #endif
            #define ROTATION    2

        #elif (DISP_ORIENTATION == 270)
            #ifndef USE_PALETTE
                #define WIN_START_ADDR  ((((DWORD) GetMaxX() + 1) * GetMaxY()) >> 1)
            #else
                #define WIN_START_ADDR  ((((DWORD) GetMaxX() + 1) * GetMaxY() * PaletteBpp) >> 5)
            #endif
            #define ROTATION    3
        #endif

    /////////////////////////////////////////////////////////////////////
    // Special Effects Register (reg 71h)
    /////////////////////////////////////////////////////////////////////
        #ifndef USE_PALETTE
    SetReg(REG_SPECIAL_EFFECTS, 0x40 | ROTATION);
        #else
    SetReg(REG_SPECIAL_EFFECTS, 0x00 | ROTATION);
        #endif

    /////////////////////////////////////////////////////////////////////
    // Main Window Display Start Address (regs 74h, 75h, 76h)
    /////////////////////////////////////////////////////////////////////
    SetReg(REG_MAIN_WIN_DISP_START_ADDR0, WIN_START_ADDR & 0x00FF);
    SetReg(REG_MAIN_WIN_DISP_START_ADDR1, (WIN_START_ADDR >> 8) & 0x00FF);
    SetReg(REG_MAIN_WIN_DISP_START_ADDR2, (WIN_START_ADDR >> 16) & 0x00FF);

    /////////////////////////////////////////////////////////////////////
    // Main Window Display Offset (regs 78h, 79h)
    /////////////////////////////////////////////////////////////////////
        #ifndef USE_PALETTE
            #define WIN_OFFSET  ((GetMaxX() + 1) >> 1)
        #else
            #define WIN_OFFSET  (((GetMaxX() + 1) * PaletteBpp) >> 5)
        #endif
    SetReg(REG_MAIN_WIN_ADDR_OFFSET0, WIN_OFFSET & 0x00FF);
    SetReg(REG_MAIN_WIN_ADDR_OFFSET1, (WIN_OFFSET >> 8) & 0x00FF);

    return (0);
}

/*********************************************************************
* Function: void EnablePalette(void)
*
* Overview: Enables the Palette mode
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects:
*
********************************************************************/
void EnablePalette(void)
{
    SetPaletteBpp(PaletteBpp);
}

/*********************************************************************
* Function: void DisablePalette(void)
*
* Overview: Disables the Palette mode
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects:
*
********************************************************************/
void DisablePalette(void)
{
    SetPaletteBpp(16);
}

/*********************************************************************
* Function: BYTE IsPaletteEnabled(void)
*
* Overview: Returns if the Palette mode is enabled or not
*
* PreCondition: none
*
* Input: none
*
* Output: Enabled -> 1, Disabled -> 0
*
* Side Effects:
*
********************************************************************/
BYTE IsPaletteEnabled(void)
{
    return ((PaletteBpp == 16) ? 0 : 1);
}

/*********************************************************************
* Function: void StartVBlankInterrupt(void)
*
* Overview: Sets up the Vertical Blanking Interrupt
*
* PreCondition: Interrupts must be enabled
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void StartVBlankInterrupt(void)
{

    /* We don't use Vertical Blanking Interrupt in SSD1926 */
    if(pPendingPalette != NULL)
    {
        blPaletteChangeError = SetPalette(pPendingPalette, PendingStartEntry, PendingLength);
        if(!blPaletteChangeError)
        {
            _palette = pPendingPalette;
        }
    }
}

/*********************************************************************
* Function: BYTE SetPaletteFlash(PALETTE_ENTRY *pPaletteEntry, WORD startEntry, WORD length)
*
* Overview: Loads the palettes from the flash immediately.
*
* PreCondition: PaletteInit() must be called before.
*
* Input: pPaletteEntry   - Pointer to the palette table in ROM
*        startEntry      - Start entry to load (inclusive)
*        length          - Number of entries
*
* Output: Status: Zero -> Success, Non-zero -> Error.
*
* Side Effects: There may be a slight flicker when the Palette entries
*               are getting loaded one by one.
*
********************************************************************/
BYTE SetPaletteFlash(PALETTE_ENTRY *pPaletteEntry, WORD startEntry, WORD length)
{
    WORD    counter;

    if((pPaletteEntry == NULL) || ((startEntry + length) > 256))
    {
        return (-1);
    }

    for(counter = 0; counter < length; counter++)
    {
        SetReg(REG_LUT_RED_WRITE_DATA, RED8(pPaletteEntry[counter].value));
        SetReg(REG_LUT_GREEN_WRITE_DATA, GREEN8(pPaletteEntry[counter].value));
        SetReg(REG_LUT_BLUE_WRITE_DATA, BLUE8(pPaletteEntry[counter].value));
        SetReg(REG_LUT_WRITE_ADDR, startEntry + counter);
    }

    return (0);
}

#endif

#endif // #if (GFX_USE_DISPLAY_CONTROLLER_SSD1926) 
