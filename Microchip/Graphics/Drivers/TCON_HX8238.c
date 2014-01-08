/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Himax HX8238 TCON driver
 *****************************************************************************
 * FileName:        TCON_HX8238.c
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
  * Company:        Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/17/08	    ...
 * 04/13/11     - Added GFX_USE_DISPLAY_CONTROLLER_xxx check. 
 *              - Replaced TCON_Delay() with Delay10us() from TimeDelay.h
 *              - Renamed TCON_Init() to GfxTconInit() and the rest
 *                of the internal functions.
 *              - Graphics Library Version 3.00 Support
 *****************************************************************************/
#include "GraphicsConfig.h"
#include "HardwareProfile.h"
#include "TimeDelay.h"
    
#if defined (USE_TCON_HX8238)

#if defined (GFX_USE_DISPLAY_CONTROLLER_SSD1926)
    #include "Graphics/SSD1926.h"
#elif defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210) || defined (GFX_USE_DISPLAY_CONTROLLER_DMA)
    // do not need any driver header file
#else
    #warning "This Timing Controller Driver is written for SSD1926, Microchip Display Controller using DMA and Microchip Graphics Module driver. If you are not using those drivers you may need to re-write this driver."
#endif

/*
    This module can use SPI or bitbang the SPI transfer using
    the SSD1926 GPIO pins. 
    This assumes that the timing controller uses SPI to
    receive Timing Programming data.
    Select the usage using the hardware profile.
*/
    // BB means BitBang
    #define BB_CS                   0x01
    #define BB_SCL                  0x02
    #define BB_SDO                  0x04
    #define BB_DC                   0x08

#if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210) || defined (GFX_USE_DISPLAY_CONTROLLER_DMA) 

    #define TCON_CSLow()            (TCON_CS_LAT = 0)
    #define TCON_CSHigh()           (TCON_CS_LAT = 1)
    #define TCON_CLKLow()           (TCON_SCL_LAT = 0)
    #define TCON_CLKHigh()          (TCON_SCL_LAT = 1)
    #define TCON_DataLow()          (TCON_SDO_LAT = 0)
    #define TCON_DataHigh()         (TCON_SDO_LAT = 1)

    #define TCON_SetCommand()       (TCON_DC_LAT = 0)
    #define TCON_SetData()          (TCON_DC_LAT = 1)

    // set the IOs used to outputs. and initialize them all to "1"
    // set up the TRIS first for outputs, then set the pins
    // to digital in case it is needed and initialize the signals
    // to all high.
    #define InitBitBangedIO() {                     \
                                TCON_CS_TRIS = 0;   \
                                TCON_SCL_TRIS = 0;  \
                                TCON_SDO_TRIS = 0;  \
                                TCON_DC_TRIS = 0;   \
                        		TCON_CS_DIG();      \
                                TCON_SCL_DIG();     \
                        		TCON_SDO_DIG();     \
                                TCON_DC_DIG();      \
                                TCON_CSHigh();      \
                                TCON_CLKHigh();     \
                                TCON_DataHigh();    \
                                TCON_SetData();     \
                              }                                

// end of #if defined (USE_DISPLAY_CONTROLLER_MCHP_DA210) 
                                   

#elif defined (GFX_USE_DISPLAY_CONTROLLER_SSD1926)    
    
    // use the bitbang version using SSD1926 GPIO pins
    
    #define TCON_CSLow()            (GfxTconSetIO(BB_CS, 0))
    #define TCON_CSHigh()           (GfxTconSetIO(BB_CS, 1))
    #define TCON_CLKLow()           (GfxTconSetIO(BB_SCL, 0))
    #define TCON_CLKHigh()          (GfxTconSetIO(BB_SCL, 1))
    #define TCON_DataLow()          (GfxTconSetIO(BB_SDO, 0))
    #define TCON_DataHigh()         (GfxTconSetIO(BB_SDO, 1))

    #define TCON_SetCommand()       (GfxTconSetIO(BB_DC, 0))
    #define TCON_SetData()          (GfxTconSetIO(BB_DC, 1))

    // this is only needed here since the controller IO's are used
    // instead of the IO's from the PIC device.
    #define SetCtrlBitBangedIO(addr, data)    (SetReg(addr, data))

    // set the GPIO of SSD1926 to as outputs. (used for TCON signals)
    // and initialize them all to "1"
    #define InitBitBangedIO() {                                                     \
                                SetCtrlBitBangedIO(REG_GPIO_CONFIG0, 0x1F);         \
                                SetCtrlBitBangedIO(REG_GPIO_STATUS_CONTROL0, 0x1F); \
                              }                                
// end of #elif defined (USE_TCON_SSD1289) 

#endif 

/************************************************************************
* Function: void GfxTconSetIO(BYTE mask, BYTE level)                                           
*                                                                       
* Overview: This sets the IO specified by mask to the value set by 
*           level.
*                                                                       
* Input: mask - specifies the IO to be toggles.
*		 level - specifies the logic where the IO will be set.
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void GfxTconSetIO(BYTE mask, BYTE level)
{

 #if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210) || defined (GFX_USE_DISPLAY_CONTROLLER_DMA)

    switch(mask)
    {
        case BB_CS:    (level == 1 ? TCON_CSHigh() : TCON_CSLow());
                    break;

        case BB_SCL:   (level == 1 ? TCON_CLKHigh() : TCON_CLKLow());
                    break;

        case BB_SDO:   (level == 1 ? TCON_DataHigh() : TCON_DataLow());
                    break;

        case BB_DC:    (level == 1 ? TCON_SetData() : TCON_SetCommand());
                    break;
        default:
                    break;            
    }
    
    Nop();
    
 #elif defined (GFX_USE_DISPLAY_CONTROLLER_SSD1926)

    static BYTE value = 0xFF;

    if(level == 0)
    {
        value &= ~mask;
    }
    else
    {
        value |= mask;
    }

    SetCtrlBitBangedIO(REG_GPIO_STATUS_CONTROL0, value);

#endif    

}

/************************************************************************
* Function: void GfxTconWriteByte(BYTE value)                                           
*                                                                       
* Overview: This writes a bytes to SPI.
*                                                                       
* Input: value - the byte data to be written.
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void GfxTconWriteByte(BYTE value)
{
    BYTE    mask;

    mask = 0x80;
    while(mask)
    {
        GfxTconSetIO(BB_SCL, 0);
        Delay10us(1);
        if(mask & value)
        {
            GfxTconSetIO(BB_SDO, 1);
        }
        else
        {
            GfxTconSetIO(BB_SDO, 0);
        }

        GfxTconSetIO(BB_SCL, 1);
        mask >>= 1;
    }
}

/************************************************************************
* Function: void GfxTconWriteCommand(BYTE index, WORD value)                                           
*                                                                       
* Overview: This writes a word to SPI by calling the write byte 
*           routine.
*                                                                       
* Input: index - The index (or address) of the register to be written.
*        value - The value that will be written to the register.
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void GfxTconWriteCommand(WORD index, WORD value)
/* */
{
    GfxTconSetIO(BB_CS, 0);

    // Index
    GfxTconWriteByte(0x70);
    GfxTconWriteByte(((WORD_VAL) index).v[1]);
    GfxTconWriteByte(((WORD_VAL) index).v[0]);

    GfxTconSetIO(BB_CS, 1);
    Delay10us(1);
    GfxTconSetIO(BB_CS, 0);

    // Data
    GfxTconWriteByte(0x72);
    GfxTconWriteByte(((WORD_VAL) value).v[1]);
    GfxTconWriteByte(((WORD_VAL) value).v[0]);
    GfxTconSetIO(BB_CS, 1);
    Delay10us(1);
}


/************************************************************************
* Function: void GfxTconInit(void)                                           
*                                                                       
* Overview: Initialize the IOs to implement Bitbanged SPI interface
*           to connect to the Timing Controller through SPI.
*                                                                       
* Input: none
*                                                                       
* Output: none                                 
*                                                                       
************************************************************************/
void GfxTconInit(void)
{
    InitBitBangedIO();

    DelayMs(20);

    GfxTconWriteCommand(0x0001, 0x6300);  //Driver Output Control
    GfxTconWriteCommand(0x0002, 0x0200);  //LCD-Driving-Waveform Control:N-line inversion,N=0
    GfxTconWriteCommand(0x0003, 0x7184);  //Power control 1:Set the step-up cycle of the step-up circuit for 262k-color mode
    DelayMs(100);
    GfxTconWriteCommand(0x0004, 0x0447);  //Input Data and Color Filter Control
    GfxTconWriteCommand(0x0005, 0xb854);  //Function Control
    GfxTconWriteCommand(0x000a, 0x4008);  //Contrast/Brightness Control

    DelayMs(40);
    GfxTconWriteCommand(0x000b, 0xd400);  //Frame Cycle Control
    GfxTconWriteCommand(0x000d, 0x123a);  //Power Control 2
    DelayMs(200);
    GfxTconWriteCommand(0x000e, 0x3000);  //Power Control 3
    DelayMs(200);
    GfxTconWriteCommand(0x000f, 0x0000);  //Gate Scan Position
    GfxTconWriteCommand(0x0016, 0x9f80);  //Horizontal Porch XLIM = 0x13F or 320 pixels per line (see HX8238 data sheet)
    GfxTconWriteCommand(0x0017, 0x2212);  //Vertical Porch  HBP = 68 ;VBP = 18
    DelayMs(200);
    GfxTconWriteCommand(0x001e, 0x00ef);  //Set the VCOMH voltage
    DelayMs(200);

    GfxTconWriteCommand(0x0030, 0x0507);  //Gamma Control
    GfxTconWriteCommand(0x0031, 0x0004);  //Gamma Control
    GfxTconWriteCommand(0x0032, 0x0707);  //Gamma Control
    GfxTconWriteCommand(0x0033, 0x0000);  //Gamma Control
    GfxTconWriteCommand(0x0034, 0x0000);  //Gamma Control
    GfxTconWriteCommand(0x0035, 0x0307);  //Gamma Control
    GfxTconWriteCommand(0x0036, 0x0700);  //Gamma Control
    GfxTconWriteCommand(0x0037, 0x0000);  //Gamma Control
    GfxTconWriteCommand(0x003a, 0x140b);  //Gamma Control
    GfxTconWriteCommand(0x003b, 0x140b);  //Gamma Control
    DelayMs(20);

    TCON_CLKLow();    
}

#endif // #if defined (USE_TCON_HX8238)
