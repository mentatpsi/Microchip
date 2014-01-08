/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Himax. HX8257 TCON  driver
 *****************************************************************************
 * FileName:        TCON_HX8257.c
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/12/09
 * 04/13/11     - Added GFX_USE_DISPLAY_CONTROLLER_xxx check. 
 *              - Replaced TCON_Delay() with Delay10us() from TimeDelay.h
 *              - Renamed TCON_Init() to GfxTconInit() and the rest
 *                of the internal functions.
 *              - Graphics Library Version 3.00 Support 
 *****************************************************************************/
#include "GraphicsConfig.h"
#include "HardwareProfile.h"
#include "TimeDelay.h"

#ifdef USE_TCON_HX8257

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
    
    #define TCON_CSLow()            (GfxTconSetIO(CS, 0))
    #define TCON_CSHigh()           (GfxTconSetIO(CS, 1))
    #define TCON_CLKLow()           (GfxTconSetIO(SCL, 0))
    #define TCON_CLKHigh()          (GfxTconSetIO(SCL, 1))
    #define TCON_DataLow()          (GfxTconSetIO(SDO, 0))
    #define TCON_DataHigh()         (GfxTconSetIO(SDO, 1))

    #define TCON_SetCommand()       (GfxTconSetIO(DC, 0))
    #define TCON_SetData()          (GfxTconSetIO(DC, 1))

    // this is only needed here since the controller IO's are used
    // instead of the IO's from the PIC device.
    #define SetCtrlBitBangedIO(addr, data)    (SetReg(addr, data))

    // set the GPIO of SSD1926 to as outputs. (used for TCON signals)
    // and initialize them all to "1"
    #define InitBitBangedIO() {                                     \
                                SetCtrlBitBangedIO(0xA8, 0x1F);     \
                                SetCtrlBitBangedIO(0xAC, 0x1F);     \
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

    SetCtrlBitBangedIO(0xAC, value);

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
{
    TCON_CTRL(BB_CS, 0);

    // Index
    TCONWriteByte(0x70);
    TCONWriteByte(((WORD_VAL) index).v[1]);
    TCONWriteByte(((WORD_VAL) index).v[0]);

    TCON_CTRL(BB_CS, 1);
    TCON_Delay();
    TCON_CTRL(BB_CS, 0);

    // Data
    TCONWriteByte(0x72);
    TCONWriteByte(((WORD_VAL) value).v[1]);
    TCONWriteByte(((WORD_VAL) value).v[0]);
    TCON_CTRL(BB_CS, 1);
    TCON_Delay();
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
    GPIO_TCON(0x0006, 0x2020);
    DelayMs(20);
}

#endif // #ifdef USE_TCON_HX8257

