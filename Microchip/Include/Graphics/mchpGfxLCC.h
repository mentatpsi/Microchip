/*********************************************************************
 *                  LCC Graphics Driver (mchpGfxLCC)
 *********************************************************************
 * FileName:        mchpGfxLCC.h
 * Dependencies:    plib.h
 *
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32 v1 or higher
 *                  MPLAB IDE v8 or higher
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ********************************************************************/

#ifndef _mchpGfxLCC_H
    #define _mchpGfxLCC_H

    #if defined(__PIC32MX__)
        #include <plib.h>
        #define PMDIN1  PMDIN
    #endif
    
    #include "GraphicsConfig.h"
    #include "GenericTypeDefs.h"

    #ifdef USE_PALETTE
        #include "Graphics/Palette.h"
    #endif

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
    #if (DISP_HOR_RESOLUTION % 8) != 0
        #error Horizontal resolution must be divisible by 8.
    #endif
    #if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180) && (DISP_ORIENTATION != 90) && (DISP_ORIENTATION != 270)
        #error The display orientation selected is not supported. It can be only 0,90,180 or 270.
    #endif

/*Macros for timing signals*/
#define DATA_ENABLE      LATDbits.LATD2
#define DATA_ENABLE_TRIS TRISDbits.TRISD2
#define HSYNC            LATBbits.LATB8
#define HSYNC_TRIS       TRISBbits.TRISB8
#define VSYNC            LATCbits.LATC3 
#define VSYNC_TRIS       TRISCbits.TRISC3

/*Macros for LCD IO*/ 
#define BACKLIGHT      LATDbits.LATD3
#define BACKLIGHT_TRIS TRISDbits.TRISD3
#define LCD_RESET      LATCbits.LATC1
#define LCD_RESET_TRIS TRISCbits.TRISC1
#define LCD_CS         LATCbits.LATC2
#define LCD_CS_TRIS    TRISCbits.TRISC2
#define LCD_DC         LATBbits.LATB3
#define LCD_DC_TRIS    TRISBbits.TRISB3

/*Macros for External SRAM*/
#define SRAM_CS       LATFbits.LATF13      
#define SRAM_TRIS     TRISFbits.TRISF13

#define ADDR15        LATAbits.LATA15
#define ADDR15_TRIS   TRISAbits.TRISA15
#define ADDR16        LATDbits.LATD8
#define ADDR16_TRIS   TRISDbits.TRISD8
#define ADDR17        LATEbits.LATE9
#define ADDR17_TRIS   TRISEbits.TRISE9
#define ADDR18        LATFbits.LATF12
#define ADDR18_TRIS   TRISFbits.TRISF12

#define PIXELCLOCK        LATDbits.LATD5
#define PIXELCLOCK_TRIS   TRISDbits.TRISD5

/*These define the size (in resolution) of the LCD being used*/
#define LINE_LENGTH              DISP_HOR_RESOLUTION
#define FRAME_HEIGHT             DISP_VER_RESOLUTION

#if defined(__32MX460F512L__) || defined (__32MX360F512L__) 
#define MAX_DMA_TRANSFER 254
#else
#define MAX_DMA_TRANSFER 65536
#endif

#if(MAX_DMA_TRANSFER < LINE_LENGTH)
#define LEGACY_MODE
#endif

/*This stabilizes the pixel refresh when drawing pixels*/
#ifdef LEGACY_MODE
#define PIXEL_DRAW_PER_DMA_TX          10  
#else
#define PIXEL_DRAW_PER_DMA_TX          20 
#endif

/*This defines the polarity of the pixel clock as defined in LCD specs*/
#ifdef DISP_INV_LSHIFT
#define PCLK_POLARITY PMP_READ_POL_LO
#else
#define PCLK_POLARITY PMP_READ_POL_HI
#endif

/*These define the size (in resolution) of the LCD being used*/
#define LINE_LENGTH              DISP_HOR_RESOLUTION
#define FRAME_HEIGHT             DISP_VER_RESOLUTION

#define VER_BLANK                 (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH)

#define  PMADDR_OVERFLOW               32768          /* Set for 2^15 because that is how many address lines are connected from the PIC32*/

#if defined (USE_PIP)
void SetPIPWindow(WORD left, WORD top, WORD hlength, WORD vlength, WORD pipx,WORD pipy);
#define PIP_BUFFER  (2)
#endif

#if defined (USE_LCC_SCROLLING)
extern WORD scroll,scrollLine,scrollPage;
#endif

#if defined (USE_DOUBLE_BUFFERING)

typedef struct
{
    WORD X;
    WORD Y;
    WORD W;
    WORD H;
} RectangleArea;

    #define GFX_BUFFER1 (0)
    #define GFX_BUFFER2 (1)

    #define GFX_MAX_INVALIDATE_AREAS 5
#endif

#endif // _mchpGfxLCC_H
