/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  LCD controller driver
 *  LG LGDP4531 
 *  Renesas R61505 
 *  Renesas R61580
 *  Samsung S6D0129
 *  Samsung S6D0139
 *  Orise Tech. SPFD5408
 *  Ilitek ILI9320
 *****************************************************************************
 * FileName:        drvTFT001.c
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright ? 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED oAS ISo WITHOUT WARRANTY OF ANY
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07	    Version 1.0 release
 * 01/30/08	    combined version for landscape and portrait
 * 01/30/08     PIC32 support
 * 06/25/09     dsPIC & PIC24H support 
 * 06/26/09     16-bit PMP support
 * 03/11/11     - Modified dependencies
 *              - changes for Graphics Library Version 3.00
 * 07/02/12     Modified PutImageXBPPYYY() functions to use new API.
 *****************************************************************************/
//#include "Graphics/Graphics.h"

#include "HardwareProfile.h"

#if defined (GFX_USE_DISPLAY_CONTROLLER_S6D0129)  || defined (GFX_USE_DISPLAY_CONTROLLER_S6D0139) || \
	defined (GFX_USE_DISPLAY_CONTROLLER_LGDP4531) || defined (GFX_USE_DISPLAY_CONTROLLER_R61505)  || \
	defined (GFX_USE_DISPLAY_CONTROLLER_SPFD5408) || defined (GFX_USE_DISPLAY_CONTROLLER_ILI9320) || \
	defined (GFX_USE_DISPLAY_CONTROLLER_R61580)

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/drvTFT001.h"
#include "Graphics/Primitive.h"
  
#ifdef USE_GFX_PMP
    #include "Graphics/gfxpmp.h"
#elif USE_GFX_EPMP
    #include "Graphics/gfxepmp.h"
#endif    

// Unsupported Graphics Library Features
#ifdef USE_TRANSPARENT_COLOR
    #warning "This driver does not support the transparent feature on PutImage(). Build will use the PutImage() functions defined in the Primitive.c"
#endif    
//#define USE_PRIMITIVE_PUTIMAGE
#ifndef USE_PRIMITIVE_PUTIMAGE
    #warning "This driver does not support partial putImage feature. To enable partial putimage feature, uncomment the macro USE_PRIMITIVE_PUTIMAGE in this file. This will enable the PutImageXBPPYYY() in the Primitive.c implementation."
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

/////////////////////// LOCAL FUNCTIONS PROTOTYPES ////////////////////////////
#if !defined(USE_TRANSPARENT_COLOR) && !defined(USE_PRIMITIVE_PUTIMAGE)
void        PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);

void        PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
#endif //#if !defined(USE_TRANSPARENT_COLOR) && !defined(USE_PRIMITIVE_PUTIMAGE)

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
#define WritePixel(color)	{ DeviceWrite(((WORD_VAL)color).v[1]); DeviceWrite(((WORD_VAL)color).v[0]);}
#endif

/*********************************************************************
* Macros:  SetAddress(addr)
*
* Overview: Writes address pointer.
*
* PreCondition: none
*
* Input: add0 -  32-bit address.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#ifdef USE_16BIT_PMP
#define SetAddress(addr)                    \
	{                                       \
	DisplaySetCommand();                    \
    DeviceWrite(0x0020);                    \
	DisplaySetData();                       \
    DeviceWrite((WORD) addr & 0x00ff);      \
	DisplaySetCommand();                    \
    DeviceWrite(0x0021);                    \
	DisplaySetData();                       \
    DeviceWrite((WORD) ((DWORD) addr >> 8));\
	DisplaySetCommand();                    \
    DeviceWrite(0x0022);                    \
	DisplaySetData();                       \
	}
#else
#define SetAddress(addr)                    \
	{\
	DisplaySetCommand();                    \
    DeviceWrite(0);                         \
    DeviceWrite(0x20);                      \
	DisplaySetData();                       \
	DeviceWrite(0);                         \
    DeviceWrite(((DWORD_VAL) (DWORD) addr).v[0]);\
	DisplaySetCommand();                    \
    DeviceWrite(0);                         \
    DeviceWrite(0x21);                      \
	DisplaySetData();                       \
	DeviceWrite(((DWORD_VAL) (DWORD) addr).v[2]);\
	DeviceWrite(((DWORD_VAL) (DWORD) addr).v[1]);\
	DisplaySetCommand();                    \
    DeviceWrite(0);                         \
    DeviceWrite(0x22);                      \
	DisplaySetData();                       \
	}
#endif

/*********************************************************************
* Function:  void  SetReg(WORD index, WORD value)
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
* Overview: sets graphics controller register
*
* Note: none
*
********************************************************************/
void SetReg(WORD index, WORD value)
{
#ifdef USE_16BIT_PMP
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(index);
	DisplaySetData();
    DeviceWrite(value);
    DisplayDisable();
#else
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(((WORD_VAL)index).v[1]);
	DeviceWrite(((WORD_VAL)index).v[0]);
	DisplaySetData();
	DeviceWrite(((WORD_VAL)value).v[1]);
	DeviceWrite(((WORD_VAL)value).v[0]);
    DisplayDisable();
#endif
}

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
    // Set FLASH CS pin as output
    DisplayFlashConfig();

    // Initialize the device
	DriverInterfaceInit();

    DelayMs(5);

    // Power on LCD
    DisplayPowerOn();
    DisplayPowerConfig();

    DelayMs(2);

    #if defined (GFX_USE_DISPLAY_CONTROLLER_LGDP4531)

    /////////////////////////////////////////////////////////
    // Synchronization after reset
    DisplayEnable();
    DeviceWrite(0);
    DeviceWrite(0);
    DisplayDisable();

    // Setup display
    SetReg(0x0010, 0x0628);
    SetReg(0x0012, 0x0006);
    SetReg(0x0013, 0x0A32);
    SetReg(0x0011, 0x0040);
    SetReg(0x0015, 0x0050);
    SetReg(0x0012, 0x0016);
    DelayMs(15);
    SetReg(0x0010, 0x5660);
    DelayMs(15);
    SetReg(0x0013, 0x2A4E);
    SetReg(0x0001, 0x0100);
    SetReg(0x0002, 0x0300);

        #if (DISP_ORIENTATION == 0)
    SetReg(0x0003, 0x1030);
        #else
    SetReg(0x0003, 0x1038);
        #endif
    SetReg(0x0008, 0x0202);
    SetReg(0x000A, 0x0000);
    SetReg(0x0030, 0x0000);
    SetReg(0x0031, 0x0402);
    SetReg(0x0032, 0x0106);
    SetReg(0x0033, 0x0700);
    SetReg(0x0034, 0x0104);
    SetReg(0x0035, 0x0301);
    SetReg(0x0036, 0x0707);
    SetReg(0x0037, 0x0305);
    SetReg(0x0038, 0x0208);
    SetReg(0x0039, 0x0F0B);
    DelayMs(15);
    SetReg(0x0041, 0x0002);
    SetReg(0x0060, 0x2700);
    SetReg(0x0061, 0x0001);
    SetReg(0x0090, 0x0119);
    SetReg(0x0092, 0x010A);
    SetReg(0x0093, 0x0004);
    SetReg(0x00A0, 0x0100);
    SetReg(0x0007, 0x0001);
    DelayMs(15);
    SetReg(0x0007, 0x0021);
    DelayMs(15);
    SetReg(0x0007, 0x0023);
    DelayMs(15);
    SetReg(0x0007, 0x0033);
    DelayMs(15);
    SetReg(0x0007, 0x0133);
    DelayMs(15);
    SetReg(0x00A0, 0x0000);
    DelayMs(20);

    /////////////////////////////////////////////////////////
    #elif defined (GFX_USE_DISPLAY_CONTROLLER_R61505)

    // Setup display
    SetReg(0x0000, 0x0000);
    SetReg(0x0007, 0x0001);
    DelayMs(5);
    SetReg(0x0017, 0x0001);
    DelayMs(5);
    SetReg(0x0010, 0x17b0);
    SetReg(0x0011, 0x0007);
    SetReg(0x0012, 0x011a);
    SetReg(0x0013, 0x0f00);
    SetReg(0x0015, 0x0000);
    SetReg(0x0029, 0x0009);
    SetReg(0x00fd, 0x0000);
    DelayMs(5);
    SetReg(0x0012, 0x013a);
    DelayMs(50);
    SetReg(0x0001, 0x0100);
    SetReg(0x0002, 0x0700);

        #if (DISP_ORIENTATION == 0)
    SetReg(0x0003, 0x1030);
        #else
    SetReg(0x0003, 0x1038);
        #endif
    SetReg(0x0008, 0x0808);
    SetReg(0x0009, 0x0000);
    SetReg(0x000a, 0x0000);
    SetReg(0x000c, 0x0000);
    SetReg(0x000d, 0x0000);
    SetReg(0x0030, 0x0000);
    SetReg(0x0031, 0x0000);
    SetReg(0x0032, 0x0000);
    SetReg(0x0033, 0x0000);
    SetReg(0x0034, 0x0000);
    SetReg(0x0035, 0x0000);
    SetReg(0x0036, 0x0000);
    SetReg(0x0037, 0x0707);
    SetReg(0x0038, 0x0707);
    SetReg(0x0039, 0x0707);
    SetReg(0x003a, 0x0303);
    SetReg(0x003b, 0x0303);
    SetReg(0x003c, 0x0707);
    SetReg(0x003d, 0x0808);
    SetReg(0x0050, 0x0000);
    SetReg(0x0051, 0x00ef);
    SetReg(0x0052, 0x0000);
    SetReg(0x0053, 0x013f);
    SetReg(0x0060, 0x2700);
    SetReg(0x0061, 0x0001);
    SetReg(0x006a, 0x0000);
    SetReg(0x0090, 0x0010);
    SetReg(0x0092, 0x0000);
    SetReg(0x0093, 0x0000);
    SetReg(0x0007, 0x0021);
    DelayMs(1);
    SetReg(0x0007, 0x0061);
    DelayMs(50);
    SetReg(0x0007, 0x0173);
    SetReg(0x0020, 0x0000);
    SetReg(0x0021, 0x0000);
    SetReg(0x0022, 0x0000);
    SetReg(0x0030, 0x0707);
    SetReg(0x0031, 0x0407);
    SetReg(0x0032, 0x0203);
    SetReg(0x0033, 0x0303);
    SetReg(0x0034, 0x0303);
    SetReg(0x0035, 0x0202);
    SetReg(0x0036, 0x001f);
    SetReg(0x0037, 0x0707);
    SetReg(0x0038, 0x0407);
    SetReg(0x0039, 0x0203);
    SetReg(0x003a, 0x0303);
    SetReg(0x003b, 0x0303);
    SetReg(0x003c, 0x0202);
    SetReg(0x003d, 0x001f);
    SetReg(0x0020, 0x0000);
    SetReg(0x0021, 0x0000);

    /////////////////////////////////////////////////////////
    #elif defined (GFX_USE_DISPLAY_CONTROLLER_S6D0129) || defined (GFX_USE_DISPLAY_CONTROLLER_S6D0139)

    // Setup display
    SetReg(0x0000, 0x0001);
    SetReg(0x0011, 0x1a00);
    SetReg(0x0014, 0x2020);
    SetReg(0x0010, 0x0900);
    SetReg(0x0013, 0x0040);
    SetReg(0x0013, 0x0060);
    SetReg(0x0013, 0x0070);
    SetReg(0x0011, 0x1a04);
    SetReg(0x0010, 0x2f00);
    SetReg(0x0001, 0x0127);
    SetReg(0x0002, 0x0700);

        #if (DISP_ORIENTATION == 0)
    SetReg(0x0003, 0x1030);
        #else
    SetReg(0x0003, 0x1038);
        #endif
    SetReg(0x0007, 0x0000);
    SetReg(0x0008, 0x0808);
    SetReg(0x0009, 0x0000);
    SetReg(0x000b, 0x0000);
    SetReg(0x000c, 0x0000);
    SetReg(0x0040, 0x0000);
    SetReg(0x0041, 0x0000);
    SetReg(0x0042, 0x013f);
    SetReg(0x0043, 0x0000);
    SetReg(0x0044, 0x00ef);
    SetReg(0x0045, 0x0000);
    SetReg(0x0046, 0xef00);
    SetReg(0x0047, 0x013f);
    SetReg(0x0048, 0x0000);
    SetReg(0x0007, 0x0014);
    SetReg(0x0007, 0x0016);
    SetReg(0x0007, 0x0017);
    SetReg(0x0020, 0x0000);
    SetReg(0x0021, 0x0000);
    SetReg(0x0022, 0x0000);

    /////////////////////////////////////////////////////////
    #elif defined (GFX_USE_DISPLAY_CONTROLLER_SPFD5408)

    // Setup display
    SetReg(0x0000, 0x0000);
    SetReg(0x0001, 0x0000);
    SetReg(0x0002, 0x0700);

        #if (DISP_ORIENTATION == 0)
    SetReg(0x0003, 0x1010);
        #else
    SetReg(0x0003, 0x1028);
        #endif
    SetReg(0x0004, 0x0000);
    SetReg(0x0008, 0x0207);
    SetReg(0x0009, 0x0000);
    SetReg(0x000a, 0x0000);
    SetReg(0x000c, 0x0000);
    SetReg(0x000d, 0x0000);
    SetReg(0x000f, 0x0000);
    SetReg(0x0007, 0x0101);
    SetReg(0x0010, 0x12b0);
    SetReg(0x0011, 0x0007);
    SetReg(0x0017, 0x0001);
    SetReg(0x0012, 0x01bb);
    SetReg(0x0013, 0x1300);
    SetReg(0x0029, 0x0010);

    SetReg(0x0030, 0x0100);
    SetReg(0x0031, 0x0c19);
    SetReg(0x0032, 0x111e);
    SetReg(0x0033, 0x3819);
    SetReg(0x0034, 0x350b);
    SetReg(0x0035, 0x0e08);
    SetReg(0x0036, 0x0d07);
    SetReg(0x0037, 0x0318);
    SetReg(0x0038, 0x0705);
    SetReg(0x0039, 0x0303);
    SetReg(0x003a, 0x0905);
    SetReg(0x003b, 0x0801);
    SetReg(0x003c, 0x030e);
    SetReg(0x003d, 0x050d);
    SetReg(0x003e, 0x0106);
    SetReg(0x003f, 0x0408);

    SetReg(0x0050, 0x0000);
    SetReg(0x0051, 0x00ef);
    SetReg(0x0052, 0x0000);
    SetReg(0x0053, 0x013f);
    SetReg(0x0060, 0xa700);
    SetReg(0x0061, 0x0001);
    SetReg(0x006a, 0x0000);
    SetReg(0x0080, 0x0000);
    SetReg(0x0081, 0x0000);
    SetReg(0x0082, 0x0000);
    SetReg(0x0083, 0x0000);
    SetReg(0x0084, 0x0000);
    SetReg(0x0085, 0x0000);
    SetReg(0x0090, 0x0010);
    SetReg(0x0092, 0x0000);
    SetReg(0x0093, 0x0103);
    SetReg(0x0095, 0x0110);
    SetReg(0x0097, 0x0000);
    SetReg(0x0098, 0x0000);
    SetReg(0x00f0, 0x5408);
    SetReg(0x00f3, 0x0010);
    SetReg(0x00f4, 0x001f);
    SetReg(0x00f0, 0x0000);
    SetReg(0x0007, 0x0133);

    /////////////////////////////////////////////////////////
    #elif defined (GFX_USE_DISPLAY_CONTROLLER_ILI9320)
    SetReg(0x0000, 0x0001); //start Int. osc
    DelayMs(15);
    SetReg(0x0001, 0x0100); //Set SS bit (shift direction of outputs is from S720 to S1)
    SetReg(0x0002, 0x0700); //select  the line inversion
        #if (DISP_ORIENTATION == 0)
    SetReg(0x0003, 0x1030); //Entry mode(Horizontal : increment,Vertical : increment, AM=0)
        #else
    SetReg(0x0003, 0x1038); //Entry mode(Horizontal : increment,Vertical : increment, AM=1)
        #endif
    SetReg(0x0004, 0x0000); //Resize control(No resizing)
    SetReg(0x0008, 0x0202); //front and back porch 2 lines
    SetReg(0x0009, 0x0000); //select normal scan
    SetReg(0x000A, 0x0000); //display control 4
    SetReg(0x000C, 0x0000); //system interface(2 transfer /pixel), internal sys clock,  	
    SetReg(0x000D, 0x0000); //Frame marker position
    SetReg(0x000F, 0x0000); //selects clk, enable and sync signal polarity,
    SetReg(0x0010, 0x0000); //	
    SetReg(0x0011, 0x0000); //power control 2 reference voltages = 1:1,
    SetReg(0x0012, 0x0000); //power control 3 VRH
    SetReg(0x0013, 0x0000); //power control 4 VCOM amplitude
    DelayMs(20);
    SetReg(0x0010, 0x17B0); //power control 1 BT,AP
    SetReg(0x0011, 0x0137); //power control 2 DC,VC
    DelayMs(50);
    SetReg(0x0012, 0x0139); //power control 3 VRH
    DelayMs(50);
    SetReg(0x0013, 0x1d00); //power control 4 vcom amplitude
    SetReg(0x0029, 0x0011); //power control 7 VCOMH
    DelayMs(50);
    SetReg(0x0030, 0x0007);
    SetReg(0x0031, 0x0403);
    SetReg(0x0032, 0x0404);
    SetReg(0x0035, 0x0002);
    SetReg(0x0036, 0x0707);
    SetReg(0x0037, 0x0606);
    SetReg(0x0038, 0x0106);
    SetReg(0x0039, 0x0007);
    SetReg(0x003c, 0x0700);
    SetReg(0x003d, 0x0707);
    SetReg(0x0020, 0x0000); //starting Horizontal GRAM Address
    SetReg(0x0021, 0x0000); //starting Vertical GRAM Address
    SetReg(0x0050, 0x0000); //Horizontal GRAM Start Position
    SetReg(0x0051, 0x00EF); //Horizontal GRAM end Position
    SetReg(0x0052, 0x0000); //Vertical GRAM Start Position
    SetReg(0x0053, 0x013F); //Vertical GRAM end Position
    SetReg(0x0060, 0x2700); //starts scanning from G1, and 320 drive lines
    SetReg(0x0061, 0x0001); //fixed base display
    SetReg(0x006a, 0x0000); //no scroll
    SetReg(0x0090, 0x0010); //set Clocks/Line =16, Internal Operation Clock Frequency=fosc/1,
    SetReg(0x0092, 0x0000); //set gate output non-overlap period=0
    SetReg(0x0093, 0x0003); //set Source Output Position=3
    SetReg(0x0095, 0x0110); //RGB interface(Clocks per line period=16 clocks)
    SetReg(0x0097, 0x0110); //set Gate Non-overlap Period 0 locksc
    SetReg(0x0098, 0x0110); //
    SetReg(0x0007, 0x0173); //display On
    /////////////////////////////////////////////////////////
    #elif defined (GFX_USE_DISPLAY_CONTROLLER_R61580)

    // Synchronization after reset
    DelayMs(2);
    SetReg(0x0000, 0x0000);
    SetReg(0x0000, 0x0000);
    SetReg(0x0000, 0x0000);
    SetReg(0x0000, 0x0000);

    // Setup display
    SetReg(0x00A4, 0x0001); // CALB=1
    DelayMs(2);
    SetReg(0x0060, 0xA700); // Driver Output Control
    SetReg(0x0008, 0x0808); // Display Control BP=8, FP=8
    SetReg(0x0030, 0x0111); // y control
    SetReg(0x0031, 0x2410); // y control
    SetReg(0x0032, 0x0501); // y control
    SetReg(0x0033, 0x050C); // y control
    SetReg(0x0034, 0x2211); // y control
    SetReg(0x0035, 0x0C05); // y control
    SetReg(0x0036, 0x2105); // y control
    SetReg(0x0037, 0x1004); // y control
    SetReg(0x0038, 0x1101); // y control
    SetReg(0x0039, 0x1122); // y control
    SetReg(0x0090, 0x0019); // 80Hz
    SetReg(0x0010, 0x0530); // Power Control
    SetReg(0x0011, 0x0237);
    SetReg(0x0012, 0x01BF);
    SetReg(0x0013, 0x1300);
    DelayMs(100);

    SetReg(0x0001, 0x0100);
    SetReg(0x0002, 0x0200);
        #if (DISP_ORIENTATION == 0)
    SetReg(0x0003, 0x1030);
        #else
    SetReg(0x0003, 0x1038);
        #endif
    SetReg(0x0009, 0x0001);
    SetReg(0x000A, 0x0008);
    SetReg(0x000C, 0x0001);
    SetReg(0x000D, 0xD000);
    SetReg(0x000E, 0x0030);
    SetReg(0x000F, 0x0000);
    SetReg(0x0020, 0x0000);
    SetReg(0x0021, 0x0000);
    SetReg(0x0029, 0x0077);
    SetReg(0x0050, 0x0000);
    SetReg(0x0051, 0xD0EF);
    SetReg(0x0052, 0x0000);
    SetReg(0x0053, 0x013F);
    SetReg(0x0061, 0x0001);
    SetReg(0x006A, 0x0000);
    SetReg(0x0080, 0x0000);
    SetReg(0x0081, 0x0000);
    SetReg(0x0082, 0x005F);
    SetReg(0x0093, 0x0701);
    SetReg(0x0007, 0x0100);
    SetReg(0x0022, 0x0000);
    #else
        #error Graphics controller is not supported.
    #endif
    DelayMs(20);
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

    #if (DISP_ORIENTATION == 0)
    address = (long)LINE_MEM_PITCH * y + x;

    #else
    y = GetMaxY() - y;
    address = (long)LINE_MEM_PITCH * x + y;
    #endif
    DisplayEnable();
    SetAddress(address);
    WritePixel(_color);
    DisplayDisable();
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
#ifndef __PIC24FJ256GB210__
#ifdef USE_16BIT_PMP

/* */
WORD GetPixel(SHORT x, SHORT y)
{
    DWORD   address;
    WORD    result;
        #if (DISP_ORIENTATION == 0)
    address = (long)LINE_MEM_PITCH * y + x;
        #else
    y = GetMaxY() - y;
    address = (long)LINE_MEM_PITCH * x + y;
        #endif
    DisplayEnable();

    SetAddress(address);

    // Temporary change wait cycles for reading (250ns = 4 cycles)
        #if defined(__C30__)
    PMMODEbits.WAITM = 4;
        #elif defined(__PIC32MX__)
    PMMODEbits.WAITM = 8;
        #else
            #error Need wait states for the device
        #endif
    DisplaySetData();

    // First RD cycle to move data from GRAM to Read Data Latch
    result = PMDIN1;

    while(PMMODEbits.BUSY);

    // Second RD cycle to get data from Read Data Latch
    result = PMDIN1;

    while(PMMODEbits.BUSY);

    // Disable LCD
    DisplayDisable();

    // Disable PMP
    PMCONbits.PMPEN = 1;

    // Read result
    result = PMDIN1;

    // Restore wait cycles for writing (60ns)
        #if defined(__dsPIC33F__) || defined(__PIC24H__)
    PMMODEbits.WAITM = 2;
        #else
    PMMODEbits.WAITM = 1;
        #endif

    // Enable PMP
    PMCONbits.PMPEN = 1;

    return (result);
}

#else

/* */
WORD GetPixel(SHORT x, SHORT y)
{
    DWORD       address;
    WORD_VAL    result;

        #if (DISP_ORIENTATION == 0)
    address = (long)LINE_MEM_PITCH * y + x;
        #else
    y = GetMaxY() - y;
    address = (long)LINE_MEM_PITCH * x + y;
        #endif
    DisplayEnable();

    SetAddress(address);

    // Temporary change wait cycles for reading (250ns = 4 cycles)
        #if defined(__C30__)
    PMMODEbits.WAITM = 4;
        #elif defined(__PIC32MX__)
    PMMODEbits.WAITM = 8;
        #else
            #error Need wait states for the device
        #endif
    DisplaySetData();

    // First RD cycle to move data from GRAM to Read Data Latch
    result.v[1] = PMDIN1;

    while(PMMODEbits.BUSY);

        #if defined(GFX_USE_DISPLAY_CONTROLLER_ILI9320)
    DelayForSync();
        #endif

    // Second RD cycle to move data from GRAM to Read Data Latch
    result.v[1] = PMDIN1;

    while(PMMODEbits.BUSY);
        #if defined (GFX_USE_DISPLAY_CONTROLLER_ILI9320)
    DelayForSync();
        #endif

    // First RD cycle to get data from Read Data Latch
    // Read previous dummy value
    result.v[1] = PMDIN1;

    while(PMMODEbits.BUSY);
        #if defined (GFX_USE_DISPLAY_CONTROLLER_ILI9320)
    DelayForSync();
        #endif

    // Second RD cycle to get data from Read Data Latch
    // Read MSB
    result.v[1] = PMDIN1;

    while(PMMODEbits.BUSY);
        #if defined (GFX_USE_DISPLAY_CONTROLLER_ILI9320)
    DelayForSync();
        #endif

    // Disable LCD
    DisplayDisable();

    // Disable PMP
    PMCONbits.PMPEN = 1;

    // Read LSB
    result.v[0] = PMDIN1;
        #if defined (GFX_USE_DISPLAY_CONTROLLER_ILI9320)
    DelayForSync();
        #endif

    // Restore wait cycles for writing (60ns)
        #if defined(__dsPIC33F__) || defined(__PIC24H__)
    PMMODEbits.WAITM = 2;
        #else
    PMMODEbits.WAITM = 1;
        #endif

    // Enable PMP
    PMCONbits.PMPEN = 1;

    return (result.Val);
}

#endif
#endif

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
    DWORD           address;
    register SHORT  x, y;

    #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0);

    /* Ready */
    #else
    if(IsDeviceBusy() != 0)
        return (0);
    #endif
    if(_clipRgn)
    {
        if(left < _clipLeft)
            left = _clipLeft;
        if(right > _clipRight)
            right = _clipRight;
        if(top < _clipTop)
            top = _clipTop;
        if(bottom > _clipBottom)
            bottom = _clipBottom;
    }

    #if (DISP_ORIENTATION == 0)
    address = (DWORD) LINE_MEM_PITCH * top + left;

    DisplayEnable();
    for(y = top; y < bottom + 1; y++)
    {
        SetAddress(address);
        for(x = left; x < right + 1; x++)
        {
            WritePixel(_color);
        }

        address += LINE_MEM_PITCH;
    }

    DisplayDisable();

    #else
    top = GetMaxY() - top;
    bottom = GetMaxY() - bottom;
    address = (DWORD) LINE_MEM_PITCH * left + top;

    DisplayEnable();
    for(y = bottom; y < top + 1; y++)
    {
        SetAddress(address);
        for(x = left; x < right + 1; x++)
        {
            WritePixel(_color);
        }

        address -= 1;
    }

    DisplayDisable();
    #endif
    return (1);
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
    DWORD   counter;

    DisplayEnable();
    SetAddress(0);
    for(counter = 0; counter < (DWORD) (GetMaxX() + 1) * (GetMaxY() + 1); counter++)
    {
        WritePixel(_color);
    }

    DisplayDisable();
}

#if !defined(USE_TRANSPARENT_COLOR) && !defined(USE_PRIMITIVE_PUTIMAGE) 

    #ifdef USE_BITMAP_FLASH

/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
    BYTE                temp = 0;
    WORD                sizeX, sizeY;
    WORD                x, y;
    BYTE                stretchX, stretchY;
    WORD                pallete[2];
    BYTE                mask;

    #if (DISP_ORIENTATION == 90)
        top = GetMaxY() - top;
    #endif

    // Move pointer to size information
    flashAddress = image + 2;

    // Set start address
    #if (DISP_ORIENTATION == 0)
        address = (long)LINE_MEM_PITCH * top + left;
    #else
        address = (long)LINE_MEM_PITCH * left + top;
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

    DisplayEnable();
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            SetAddress(address);
            mask = 0;
            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *flashAddress;
                    flashAddress++;
                    mask = 0x01;
                }

                // Set color
                if(mask & temp)
                {
                    SetColor(pallete[1]);
                }
                else
                {
                    SetColor(pallete[0]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    WritePixel(_color);
                }

                // Shift to the next pixel
                mask <<= 1;
            }

            #if (DISP_ORIENTATION == 0)
                address += LINE_MEM_PITCH;
            #else
                address -= 1;
            #endif
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
    WORD                sizeX, sizeY;
    register WORD       x, y;
    BYTE                temp = 0;
    register BYTE       stretchX, stretchY;
    WORD                pallete[16];
    WORD                counter;

    #if (DISP_ORIENTATION == 90)
        top = GetMaxY() - top;
    #endif

    // Move pointer to size information
    flashAddress = image + 2;

    // Set start address
    #if (DISP_ORIENTATION == 0)
        address = (long)LINE_MEM_PITCH * top + left;
    #else
        address = (long)LINE_MEM_PITCH * left + top;
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

    DisplayEnable();
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            SetAddress(address);
            for(x = 0; x < sizeX; x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    SetColor(pallete[temp >> 4]);
                }
                else
                {
                    temp = *flashAddress;
                    flashAddress++;

                    // first pixel in byte
                    SetColor(pallete[temp & 0x0f]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    WritePixel(_color);
                }

                // Shift to the next pixel
                //temp >>= 4;
            }

            #if (DISP_ORIENTATION == 0)
                address += LINE_MEM_PITCH;
            #else
                address -= 1;
            #endif
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
    WORD                sizeX, sizeY;
    WORD                x, y;
    BYTE                temp;
    BYTE                stretchX, stretchY;
    WORD                pallete[256];
    WORD                counter;

    #if (DISP_ORIENTATION == 90)
        top = GetMaxY() - top;
    #endif

    // Move pointer to size information
    flashAddress = image + 2;

    // Set start address
            #if (DISP_ORIENTATION == 0)
    address = (long)LINE_MEM_PITCH * top + left;
            #else
    address = (long)LINE_MEM_PITCH * left + top;
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

    DisplayEnable();
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            SetAddress(address);
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *flashAddress;
                flashAddress++;

                // Set color
                SetColor(pallete[temp]);

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    WritePixel(_color);
                }
            }

                    #if (DISP_ORIENTATION == 0)
            address += LINE_MEM_PITCH;
                    #else
            address -= 1;
                    #endif
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                temp;
    register BYTE       stretchX, stretchY;

    #if (DISP_ORIENTATION == 90)
        top = GetMaxY() - top;
    #endif

    // Move pointer to size information
    flashAddress = (FLASH_WORD *)image + 1;

    // Set start address
    #if (DISP_ORIENTATION == 0)
        address = (long)LINE_MEM_PITCH * top + left;
    #else
        address = (long)LINE_MEM_PITCH * left + top;
    #endif

    // Read image size
    sizeY = *flashAddress;
    flashAddress++;
    sizeX = *flashAddress;
    flashAddress++;

    DisplayEnable();
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            SetAddress(address);
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *flashAddress;
                flashAddress++;

                // Set color
                SetColor(temp);

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    WritePixel(_color);
                }
            }

            #if (DISP_ORIENTATION == 0)
                address += LINE_MEM_PITCH;
            #else
                address -= 1;
            #endif
        }
    }

    DisplayDisable();
}

    #endif // USE_BITMAP_FLASH
    #ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
void PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  address;
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[2];
    BYTE            lineBuffer[((GetMaxX() + 1) / 8) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0;
    BYTE            mask;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;


    #if (DISP_ORIENTATION == 90)
        top = GetMaxY() - top;
    #endif

    // Set start address
    #if (DISP_ORIENTATION == 0)
        address = (long)LINE_MEM_PITCH * top + left;
    #else
        address = (long)LINE_MEM_PITCH * left + top;
    #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (2 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 2 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 2 * sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width >> 3;
    if(bmp.width & 0x0007)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        DisplayEnable();
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            SetAddress(address);
            mask = 0;
            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *pData++;
                    mask = 0x01;
                }

                // Set color
                if(mask & temp)
                {
                    SetColor(pallete[1]);
                }
                else
                {
                    SetColor(pallete[0]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    WritePixel(_color);
                }

                // Shift to the next pixel
                mask <<= 1;
            }

            #if (DISP_ORIENTATION == 0)
                address += LINE_MEM_PITCH;
            #else
                address -= 1;
            #endif
        }

        DisplayDisable();
    }
}

/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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
void PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  address;
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[16];
    BYTE            lineBuffer[((GetMaxX() + 1) / 2) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;

    #if (DISP_ORIENTATION == 90)
        top = GetMaxY() - top;
    #endif

    // Set start address
    #if (DISP_ORIENTATION == 0)
        address = (long)LINE_MEM_PITCH * top + left;
    #else
        address = (long)LINE_MEM_PITCH * left + top;
    #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (16 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width >> 1;
    if(bmp.width & 0x0001)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        DisplayEnable();
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            SetAddress(address);

            for(x = 0; x < sizeX; x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    SetColor(pallete[temp >> 4]);
                }
                else
                {
                    temp = *pData++;

                    // first pixel in byte
                    SetColor(pallete[temp & 0x0f]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    WritePixel(_color);
                }
            }

            #if (DISP_ORIENTATION == 0)
                address += LINE_MEM_PITCH;
            #else
                address -= 1;
            #endif
        }

        DisplayDisable();
    }
}

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;

    #if (DISP_ORIENTATION == 90)
        top = GetMaxY() - top;
    #endif

    // Set start address
    #if (DISP_ORIENTATION == 0)
        address = (long)LINE_MEM_PITCH * top + left;
    #else
        address = (long)LINE_MEM_PITCH * left + top;
    #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (256 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 256 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(WORD);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
        memOffset += sizeX;
        DisplayEnable();
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            SetAddress(address);

            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;
                SetColor(pallete[temp]);

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    WritePixel(_color);
                }
            }

            #if (DISP_ORIENTATION == 0)
                address += LINE_MEM_PITCH;
            #else
                address -= 1;
            #endif
        }

        DisplayDisable();
    }
}

/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - (currently not implemented in this driver)
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

    WORD            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;

    #if (DISP_ORIENTATION == 90)
        top = GetMaxY() - top;
    #endif

    // Set start address
    #if (DISP_ORIENTATION == 0)
        address = (long)LINE_MEM_PITCH * top + left;
    #else
        address = (long)LINE_MEM_PITCH * left + top;
    #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    byteWidth = sizeX << 1;

    for(y = 0; y < sizeY; y++)
    {
        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        DisplayEnable();
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            SetAddress(address);

            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;
                SetColor(temp);

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    WritePixel(_color);
                }
            }

            #if (DISP_ORIENTATION == 0)
                address += LINE_MEM_PITCH;
            #else
                address -= 1;
            #endif
        }

        DisplayDisable();
    }
}

    #endif //USE_BITMAP_EXTERNAL
#endif // #if !defined(USE_TRANSPARENT_COLOR) && !defined(USE_PRIMITIVE_PUTIMAGE) 

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
    return (0);
}

#endif // #if defined (GFX_USE_DISPLAY_CONTROLLER_S6D0129)  || defined (GFX_USE_DISPLAY_CONTROLLER_S6D0139) ||  ...


