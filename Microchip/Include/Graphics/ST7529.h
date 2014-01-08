/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  ST7529 LCD controllers driver
 *****************************************************************************
 * FileName:        ST7529.h
 * Dependencies:    p24Fxxxx.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
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
 * 03/20/08		...
 * 04/12/11     Graphics Library Version 3.00 Support
 *****************************************************************************/
#ifndef _ST7529_H
    #define _ST7529_H

    #include "HardwareProfile.h"
    #include "GraphicsConfig.h"
    #include "GenericTypeDefs.h"

    #ifdef USE_16BIT_PMP
        #error This driver doesn't support 16-bit PMP (remove USE_16BIT_PMP option from GHardwareProfile.h)
    #endif
    #ifndef DISP_HOR_RESOLUTION
        #error DISP_HOR_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_VER_RESOLUTION
        #error DISP_VER_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef COLOR_DEPTH
        #error COLOR_DEPTH must be defined in GraphicsConfig.h
    #endif
    #ifndef DISP_ORIENTATION
        #error DISP_ORIENTATION must be defined in HardwareProfile.h
    #endif

/*********************************************************************
* Overview: Horizontal and vertical screen size.
*********************************************************************/
    #if (DISP_HOR_RESOLUTION != 256)
        #error This driver doesn't supports this resolution. Horizontal resolution must be 256 pixels.
    #endif
    #if (DISP_VER_RESOLUTION != 128)
        #error This driver doesn't supports this resolution. Vertical resolution must be 128 pixels.
    #endif

/*********************************************************************
* Overview: Display orientation.
*********************************************************************/
    #if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180)
        #error This driver doesn't support this orientation.
    #endif

/*********************************************************************
* Overview: Color depth.
*********************************************************************/
    #if (COLOR_DEPTH != 8)
        #error This driver doesn't support the selected color depth. It should be 8 but the device is actually using 5 bits. The 3 MSbits will be ignored.
    #endif

// Norm commands
    #define EXTIN   0x30
    #define EXTOUT  0x31
    #define DISON   0xaf
    #define DISOFF  0xae
    #define DISNOR  0xa6
    #define DISINV  0xa7
    #define COMSCN  0xbb
    #define DISCTRL 0xca
    #define SLPIN   0x95
    #define SLPOUT  0x94
    #define LASET   0x75
    #define CASET   0x15
    #define DATSDR  0xbc
    #define RAMWR   0x5c
    #define RAMRD   0x5d
    #define PTLIN   0xa8
    #define PTLOUT  0xa9
    #define RMWIN   0xe0
    #define RMWOUT  0xee
    #define ASCSET  0xaa
    #define SCSTART 0xab
    #define OSCON   0xd1
    #define OSCOFF  0xd2
    #define PWRCTRL 0x20
    #define VOLCTRL 0x81
    #define VOLUP   0xd6
    #define VOLDOWN 0xd7
    #define EPSRRD1 0x7c
    #define EPSRRD2 0x7d
    #define NOP     0x25
    #define EPINT   0x07

// Ext commands
    #define GRAY1SET    0x20
    #define GRAY2SET    0x21
    #define ANASET      0x32
    #define SWINT       0x34
    #define EPCTIN      0xcd
    #define EPCOUT      0xcc
    #define EPMWR       0xfc
    #define EPMRD       0xfd

/*********************************************************************
* Macros: SetPalette(colorNum, color)
*
* Overview:  Sets palette register.
*
* PreCondition: none
*
* Input: colorNum - Register number.
*        color - Color.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetPalette(colorNum, color)

/*********************************************************************
* Function: void ContrastSet(WORD contrast)
*
* PreCondition: none
*
* Input: contrast value (LSB 10-0 are valid, MSB 15-11 are not used)
*
* Output: none
*
* Side Effects: none
*
* Overview: sets contrast
*
* Note: none
*
********************************************************************/
void    ContrastSet(WORD contrast);

/*********************************************************************
* Function: void ContrastUp(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: increases contrast
*
* Note: none
*
********************************************************************/
void    ContrastUp(void);

/*********************************************************************
* Function: void ContrastDown(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: decreases contrast
*
* Note: none
*
********************************************************************/
void    ContrastDown(void);

/*********************************************************************
* Function: void DisplayOn(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: turns on display
*
* Note: none
*
********************************************************************/
void    DisplayOn(void);

/*********************************************************************
* Function: void DisplayOn(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: turns off display
*
* Note: none
*
********************************************************************/
void    DisplayOff(void);

/*********************************************************************
* Function: void SleepIn(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: enters sleep mode
*
* Note: none
*
********************************************************************/
void    SleepIn(void);

/*********************************************************************
* Function: void WakeUp(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: wakes up from sleep
*
* Note: none
*
********************************************************************/
void    WakeUp(void);
#endif // _ST7529_H
