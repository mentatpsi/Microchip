/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Primitives Layer.
 *****************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
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
 * 30/05/07	    ...
 * 03/09/11     Moved hardware initialization to InitializeBoard()
 *              function.
 *****************************************************************************/
#include "MainDemo.h"

// Configuration bits
#if defined(__dsPIC33F__) || defined(__PIC24H__)
_FOSCSEL(FNOSC_PRI);
_FOSC(FCKSM_CSECMD &OSCIOFNC_OFF &POSCMD_XT);
_FWDT(FWDTEN_OFF);
#elif defined(__dsPIC33E__) 
_FOSCSEL(FNOSC_FRC);			
_FOSC(FCKSM_CSECMD & POSCMD_XT & OSCIOFNC_OFF & IOL1WAY_OFF);
_FWDT(FWDTEN_OFF);
_FPOR(FPWRT_PWR128 & BOREN_ON & ALTI2C1_ON & ALTI2C2_ON);
_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF);
#elif defined(__PIC32MX__)
   #if defined(__32MX250F128D__)
    #pragma config FPLLODIV = DIV_2, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
    #pragma config OSCIOFNC = OFF, POSCMOD = XT, FSOSCEN = OFF, FNOSC = PRIPLL
    #pragma config CP = OFF, BWP = OFF, PWP = OFF
   #else
    #pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
    #pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
    #pragma config CP = OFF, BWP = OFF, PWP = OFF
   #endif
#else
    #if defined(__PIC24FJ256GB110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ256GA110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRIPLL & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ128GA010__)
_CONFIG2(FNOSC_PRIPLL & POSCMOD_XT) // Primary XT OSC with PLL
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF)   // JTAG off, watchdog timer off
    #endif
	#if defined (__PIC24FJ256DA210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif	  
	#if defined (__PIC24FJ256GB210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif
#endif

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void         InitializeBoard(void);          

/////////////////////////////////////////////////////////////////////////////
//                            FONTS
/////////////////////////////////////////////////////////////////////////////
extern const FONT_FLASH     Font25;
extern const FONT_FLASH     Font35;
extern const FONT_FLASH     Font35_Antialiased;
extern const XCHAR          AntialisedText[];

/////////////////////////////////////////////////////////////////////////////
//                            PICTURES
/////////////////////////////////////////////////////////////////////////////
extern const IMAGE_FLASH        flower16bit;
extern const IMAGE_FLASH        flower8bit;
extern const IMAGE_FLASH        flower4bit;
extern const IMAGE_FLASH        flower1bit;
extern const IMAGE_FLASH        Sun8bit;
extern const IMAGE_FLASH        Gaming4bit;
extern const GFX_IMAGE_HEADER   Gaming4bit_RLE;
extern const GFX_IMAGE_HEADER   Sun8bit_RLE;

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
#define WAIT_UNTIL_FINISH(x)    while(!x)
#define MIN(x,y)                ((x > y)? y: x)
#define DEMODELAY				1000


/////////////////////////////////////////////////////////////////////////////
//                            MAIN
/////////////////////////////////////////////////////////////////////////////
int main(void)
{
    SHORT       width, height, temp, x1, y1, x2, y2;
    SHORT       counter;
    const SHORT polyPoints[] = {
        (GetMaxX()+1)/2,    (GetMaxY()+1)/4,
        (GetMaxX()+1)*3/4,  (GetMaxY()+1)/2,
        (GetMaxX()+1)/2,    (GetMaxY()+1)*3/4,
        (GetMaxX()+1)/4,    (GetMaxY()+1)/2,
        (GetMaxX()+1)/2,    (GetMaxY()+1)/4,
    };
    
    // initialize hardware components and peripherals
    InitializeBoard();

    while(1)
    {
	    // draw border lines to show the limits of the 
	    // left, right, top and bottom pixels of the screen
	    // draw the top most horizontal line
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Line(0,0,GetMaxX(),0));
        // draw the right most vertical line
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(Line(GetMaxX(),0,GetMaxX(),GetMaxY()));
	    // draw the bottom most horizontal line
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Line(0,GetMaxY(),GetMaxX(),GetMaxY()));
        // draw the left most vertical line
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Line(0,0,0,GetMaxY()));

        DelayMs(DEMODELAY);

		// draw WHITE lines intersecting in the middle of the screen
        SetColor(WHITE);
        for(counter = 0; counter < GetMaxX(); counter += 20)
        {
            WAIT_UNTIL_FINISH(Line(counter, 0, GetMaxX() - 1 - counter, GetMaxY() - 1));
        }

        DelayMs(DEMODELAY);

		// draw concentric RED circles in the middle of the screen
        SetColor(BRIGHTRED);
        for(counter = 10; counter < MIN(GetMaxX(), GetMaxY()) >> 1; counter += 10)
        {
            WAIT_UNTIL_FINISH(Circle(GetMaxX() >> 1, GetMaxY() >> 1, counter));
        }

        DelayMs(DEMODELAY);

		// draw concentric filled circles in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 60));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 40));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 20));

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();

		// draw concentric beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - 60, (GetMaxY() >> 1) - 60, (GetMaxX() >> 1) + 60, (GetMaxY() >> 1) + 60, 30));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - 40, (GetMaxY() >> 1) - 40, (GetMaxX() >> 1) + 40, (GetMaxY() >> 1) + 40, 30));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - 20, (GetMaxY() >> 1) - 20, (GetMaxX() >> 1) + 20, (GetMaxY() >> 1) + 20, 30));

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();

		// draw concentric thick beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH
        (
            Arc
                (
                    (GetMaxX() >> 1) - 60,
                    (GetMaxY() >> 1) - 60,
                    (GetMaxX() >> 1) + 60,
                    (GetMaxY() >> 1) + 60,
                    20,
                    30,
                    0xFF
                )
        );
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH
        (
            Arc
                (
                    (GetMaxX() >> 1) - 40,
                    (GetMaxY() >> 1) - 40,
                    (GetMaxX() >> 1) + 40,
                    (GetMaxY() >> 1) + 40,
                    20,
                    30,
                    0xFF
                )
        );
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH
        (
            Arc
                (
                    (GetMaxX() >> 1) - 20,
                    (GetMaxY() >> 1) - 20,
                    (GetMaxX() >> 1) + 20,
                    (GetMaxY() >> 1) + 20,
                    20,
                    30,
                    0xFF
                )
        );

        DelayMs(DEMODELAY);

		// draw concentric filled beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH
        (
            FillBevel
                (
                    (GetMaxX() >> 1) - 60,
                    (GetMaxY() >> 1) - 60,
                    (GetMaxX() >> 1) + 60,
                    (GetMaxY() >> 1) + 60,
                    30
                )
        );
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH
        (
            FillBevel
                (
                    (GetMaxX() >> 1) - 40,
                    (GetMaxY() >> 1) - 40,
                    (GetMaxX() >> 1) + 40,
                    (GetMaxY() >> 1) + 40,
                    30
                )
        );
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH
        (
            FillBevel
                (
                    (GetMaxX() >> 1) - 20,
                    (GetMaxY() >> 1) - 20,
                    (GetMaxX() >> 1) + 20,
                    (GetMaxY() >> 1) + 20,
                    30
                )
        );

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();

		// draw concentric thick beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1) - 50, (GetMaxX() >> 1), (GetMaxY() >> 1) + 50, 50, 60, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1) - 50, (GetMaxX() >> 1), (GetMaxY() >> 1) + 50, 50, 60, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1) - 50, (GetMaxX() >> 1), (GetMaxY() >> 1) + 50, 50, 60, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1) - 50, (GetMaxX() >> 1), (GetMaxY() >> 1) + 50, 50, 60, 0x88));

        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1) - 30, (GetMaxX() >> 1), (GetMaxY() >> 1) + 30, 35, 45, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1) - 30, (GetMaxX() >> 1), (GetMaxY() >> 1) + 30, 35, 45, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1) - 30, (GetMaxX() >> 1), (GetMaxY() >> 1) + 30, 35, 45, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1) - 30, (GetMaxX() >> 1), (GetMaxY() >> 1) + 30, 35, 45, 0x88));

        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), 20, 30, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), 20, 30, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), 20, 30, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(Arc((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), 20, 30, 0x88));

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();


		// draw rectangles in the middle of the screen
        SetColor(BRIGHTBLUE);
        for(counter = 0; counter < MIN(GetMaxX(), GetMaxY()) >> 1; counter += 20)
        {
            WAIT_UNTIL_FINISH
            (
                Rectangle
                    (
                        GetMaxX() / 2 - counter,
                        GetMaxY() / 2 - counter,
                        GetMaxX() / 2 + counter,
                        GetMaxY() / 2 + counter
                    )
            );
        }

        DelayMs(DEMODELAY);

        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - 80, GetMaxY() / 2 - 80, GetMaxX() / 2 + 80, GetMaxY() / 2 + 80));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - 60, GetMaxY() / 2 - 60, GetMaxX() / 2 + 60, GetMaxY() / 2 + 60));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - 40, GetMaxY() / 2 - 40, GetMaxX() / 2 + 40, GetMaxY() / 2 + 40));

        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// draw ploygon shape in the middle of the screen
        SetColor(WHITE);
        WAIT_UNTIL_FINISH(DrawPoly(5, (SHORT *)polyPoints));

        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// draw fonts in the screen
        SetFont((void *) &Font25);
        SetColor(BRIGHTGREEN);
        width = GetTextWidth("Microchip Technology Inc.", (void *) &Font25);
        height = GetTextHeight((void *) &Font25);

        OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Technology Inc.");

        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        SetFont((void *) &Font35);
        SetColor(WHITE);
        width = GetTextWidth("ANTIALIASED", (void *) &Font35);
        height = GetTextHeight((void *) &Font35);
        x1 = (GetMaxX() - width) >> 1;
        y1 = ((GetMaxY() - height) >> 2) - 5;

        // Display Non-Antialised
        temp = GetTextWidth("NON-", (void *) &Font35);
        OutTextXY((GetMaxX() - temp) >> 1, ((GetMaxY() - height) >> 2) - height, "NON-");
        OutTextXY(x1, y1, "ANTIALIASED");

        // Display Antialised (Opaque)
        SetFont((void *) &Font35_Antialiased);
        width = GetTextWidth((XCHAR *)AntialisedText, (void *) &Font35_Antialiased);
        height = GetTextHeight((void *) &Font35_Antialiased);
        y1 = (GetMaxY() - height) >> 1;
        OutTextXY(x1, y1, (XCHAR *)AntialisedText);

        SetFont((void *) &Font25);
        temp = GetTextWidth("(Opaque)", (void *) &Font25);
        OutTextXY(x1 + ((width - temp) >> 1), y1 + height, "(Opaque)");

        // Display Background Gradient
        width = GetTextWidth((XCHAR *)AntialisedText, (void *) &Font35_Antialiased);
        y1 = 3 * ((GetMaxY() - height) >> 2) + GetTextHeight((void *) &Font25);
        x2 = x1 + width;
        y2 = y1 + height;
        BarGradient(x1, y1, x2, y2, BRIGHTRED, CYAN, 100, GRAD_RIGHT);
        while(IsDeviceBusy());

        // Display Antialised (Translucent)
        SetFont((void *) &Font35_Antialiased);
        SetColor(WHITE);
        GFX_Font_SetAntiAliasType(ANTIALIAS_TRANSLUCENT);
        OutTextXY(x1, y1, (XCHAR *)AntialisedText);

        SetFont((void *) &Font25);
        temp = GetTextWidth("(Translucent)", (void *) &Font25);
        OutTextXY(x1 + ((width - temp) >> 1), (3 * (GetMaxY() - height) >> 2) + height +  + GetTextHeight((void *) &Font25), "(Translucent)");

        DelayMs(5*DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// draw pictures in the screen with different bits per pixel
        WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower1bit, 2));
        SetColor(WHITE);
        OutTextXY(200, 0, "1BPP");
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower4bit, 2));
        SetColor(WHITE);
        OutTextXY(200, 0, "4BPP");
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower8bit, 2));
        SetColor(WHITE);
        OutTextXY(200, 0, "8BPP");
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower16bit, 2));
        SetColor(WHITE);
        OutTextXY(200, 0, "16BPP");
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        width = GetImageWidth((void *) &flower1bit);
        height = GetImageHeight((void *) &flower1bit);

        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2 - width  , (GetMaxY() + 1) / 2 - height  , (void *) &flower1bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2          , (GetMaxY() + 1) / 2 - height  , (void *) &flower4bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2 - width  , (GetMaxY() + 1) / 2           , (void *) &flower8bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2          , (GetMaxY() + 1) / 2           , (void *) &flower16bit, 1));
        DelayMs(DEMODELAY*2);
        SetColor(BLACK);
        ClearDevice();

        /* RLE Demonstration starts here */
        width = GetImageWidth((void *) &Sun8bit_RLE);
        height = GetImageHeight((void *) &Sun8bit_RLE);

        SetColor(WHITE);
        SetFont((void *) &Font25);
        OutTextXY(0, 0, "RLE Compression of 8BPP Image");
        WAIT_UNTIL_FINISH(PutImage(0, 40, (void *) &Sun8bit, 1));
        WAIT_UNTIL_FINISH(PutImage(width + 60, 40, (void *) &Sun8bit_RLE, 1));
        SetColor(WHITE);
        OutTextXY(0, height + 60, "Original");
        OutTextXY(width + 60, height + 60, "RLE compressed");
        OutTextXY(0, height + 90, "(5702 Bytes)");
        OutTextXY(width + 60, height + 90, "(3997 Bytes)");
        
        DelayMs(DEMODELAY*3);
        SetColor(BLACK);
        ClearDevice();

        SetColor(WHITE);
        SetFont((void *) &Font25);
        OutTextXY(0, 0, "RLE Compression of 4BPP Image");
        WAIT_UNTIL_FINISH(PutImage(0, 40, (void *) &Gaming4bit, 1));
        WAIT_UNTIL_FINISH(PutImage(width + 60, 40, (void *) &Gaming4bit_RLE, 1));
        SetColor(WHITE);
        OutTextXY(0, height + 60, "Original");
        OutTextXY(width + 60, height + 60, "RLE compressed");
        OutTextXY(0, height + 90, "(2630 Bytes)");
        OutTextXY(width + 60, height + 90, "(1378 Bytes)");

        DelayMs(DEMODELAY*3);
        SetColor(BLACK);
        ClearDevice();
    }
}


/////////////////////////////////////////////////////////////////////////////
// Function: InitializeBoard()
// Input: none
// Output: none
// Overview: Initializes the hardware components including the PIC device
//           used.
/////////////////////////////////////////////////////////////////////////////
void InitializeBoard(void)
{

#ifdef MEB_BOARD
    CPLDInitialize();
    CPLDSetGraphicsConfiguration(GRAPHICS_HW_CONFIG);
    CPLDSetSPIFlashConfiguration(SPI_FLASH_CHANNEL);
#endif

#if defined(__dsPIC33F__) || defined(__PIC24H__) || defined(__dsPIC33E__)

    // Configure Oscillator to operate the device at 40Mhz
    // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
    #if defined(__dsPIC33E__) || defined(__PIC24E__)
        //Fosc = 8M * 60/(2*2) = 120MHz for 8M input clock
        PLLFBD = 58;    			// M=60         
    #else
        // Fosc= 8M*40(2*2)=80Mhz for 8M input clock
        PLLFBD = 38;                    // M=40
    #endif

    CLKDIVbits.PLLPOST = 0;         // N1=2
    CLKDIVbits.PLLPRE = 0;          // N2=2
    OSCTUN = 0;                     // Tune FRC oscillator, if FRC is used

    // Disable Watch Dog Timer
    RCONbits.SWDTEN = 0;

    // Clock switching to incorporate PLL
    __builtin_write_OSCCONH(0x03);  // Initiate Clock Switch to Primary

    // Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONL(0x01);  // Start clock switching
    while(OSCCONbits.COSC != 0b011);

    // Wait for Clock switch to occur	
    // Wait for PLL to lock
    while(OSCCONbits.LOCK != 1)
    { };

#elif defined(__PIC32MX__)
    INTEnableSystemMultiVectoredInt();
    SYSTEMConfigPerformance(GetSystemClock());

    #if defined(__32MX250F128D__)
    DDPCONbits.JTAGEN = 0; // Disable JTAG
    ANSELA = ANSELB = ANSELC = 0x0000; // Disable all ADC inputs
    #endif
#endif

    /////////////////////////////////////////////////////////////////////////////
    // ADC Explorer 16 Development Board Errata (work around 2)
    // RB15 should be output
    /////////////////////////////////////////////////////////////////////////////
#ifdef EXPLORER_16
    LATBbits.LATB15 = 0;
    TRISBbits.TRISB15 = 0;
#endif

    /////////////////////////////////////////////////////////////////////////////
#if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    AD1PCFGL = 0xffff;
#elif defined(__dsPIC33E__) || defined(__PIC24E__)
    ANSELE = 0x00;
    ANSELDbits.ANSD6 = 0;
#endif

    InitGraph();

}    

