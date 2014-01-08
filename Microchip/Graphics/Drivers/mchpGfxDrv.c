/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Custom display controller driver template
 *****************************************************************************
 * FileName:        mchpGfxDrv.c
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights reserved.
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
 * Date				Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 8/20/09          Initial Version
 * 12/03/09         Added Double Buffering Support
 * 03/29/10         Fixed EPMP base address programming
 * 04/04/10      	Added block scrolling 
 * 07/12/10         Added EDS type images
 * 09/27/10         Added CHRGPU & IPU support
 * 11/19/10         Fixed EPMP address enabling error
 * 03/09/11         - modified dependencies for Version 3.00 of 
 *                    Graphics Library
 *                  - Removed USE_DRV_xxxx switches. This is not needed anymore
 *                    since Primitive Layer implements weak attributes on 
 *                    Primitive Routines that can be implemented in hardware.
 *                  - Replace color data type from WORD_VAL to GFX_COLOR
 *                  - Optimized PutImageXBPP(), PutImageXBPPExt() and 
 *                    PutImageXEDS() for performance.
 *                  - Added transparent color feature in PutImageXBPP(), 
 *                    PutImageXBPPExt() and PutImageXEDS().
 *                  - Added DisplayBrightness() to control the backlight.
 *                  - Converted PutImage() to PutImageDrv() for optimized
 *                    processing of images in EDS and images that are compressed 
 *                    for IPU
 *                  - Modified calculation of AMWAIT to be based on user given
 *                    timing parameter and the graphics module clock used.
 * 03/27/11         - Replaced dynamic display buffer address setup to use 
 *                    GFX_GetDisplayArea(). This allows changing the display buffer
 *                    address at run time.
 *                  - Modified double buffering macros to functions.
 * 04/06/11        Renamed file to mchpGfxDrv.c
 * 05/04/11        Added CopyWindow(), CopyPageWindow() and GetPageAddress()
 * 07/05/11        Fixed PutImage8BPPExt() issue on incomplete lines.
 * 07/20/11        - Modified RopBlock to be inline. 
 *                 - convert CopyBlock() to function.
 * 09/01/11        - Fixed error on EPMP AMWAIT calculation.
 *                 - EPMP should not be enabled when not used.
 * 10/07/11        Modified GetImageHeight() and GetImageWidth() to support RLE
 *                 compressed images.
 * 12/2/11         Fixed issues on the following when palette is enabled
 *                 - PutImage4BppExt() 
 *                 - PutImage4Bpp() 
 *                 - PutImage1BppExt() 
 *                 - PutImage1Bpp() 
 * 12/13/11        Fixed offset limit issue on the following 
 *                 - PutImage1BPPEDS()
 *                 - PutImage4BPPEDS() 
 * 01/12/12        Modified OutChar(), GetTextWidth(), GetTextHeight and SetFont()
 *                 functions to use Primitive Layer routines when performing 
 *                 PutPixel() text rendering.
 * 03/16/12        - Fixed error in calculating AMWAIT when device access time is 
 *                   larger than 3 Tam.
 *                 - Fixed PMA singal enabling to match the actual device size. 
 *                 - Fixed SetPaletteFlash() to program colors from any
 *                   address loaction of the Color Look Up Table (CLUT).
 * 05/17/12        - corrected DPPOWER usage.
 *                 - Added PutImagePartial() support.    
 * 06/15/12        - Added Alpha Blend lite support to Bar().
 *****************************************************************************/
#include "HardwareProfile.h"
   
#if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210)

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/mchpGfxDrv.h"
#include "Graphics/gfxtcon.h"
#include "Graphics/Primitive.h"

#ifdef USE_PALETTE

    #include "Graphics/Palette.h"

    extern void *_palette;
    extern BYTE PaletteBpp;
    extern BYTE blPaletteChangeError;
    extern void *pPendingPalette;
    extern WORD PendingStartEntry;
    extern WORD PendingLength;

#endif

// Clipping region control
SHORT       _clipRgn;

// Clipping region borders
SHORT       _clipLeft;
SHORT       _clipTop;
SHORT       _clipRight;
SHORT       _clipBottom;

#ifdef USE_DOUBLE_BUFFERING

    BYTE blInvalidateAll;
    BYTE blEnableDoubleBuffering;
    BYTE NoOfInvalidatedRectangleAreas;
    RectangleArea InvalidatedArea[GFX_MAX_INVALIDATE_AREAS];

    volatile DWORD  _drawbuffer;
    volatile BYTE   blDisplayUpdatePending;
	
	/*********************************************************************
	* Function: WORD GetDrawBufferAddress(void)
	*
	* PreCondition: none
	*
	* Input: none
	*
	* Output: Address of the Draw Buffer
	*
	* Side Effects: none
	*
	* Overview: Returns the address of Draw Buffer
	*
	* Note:
	*
	********************************************************************/
    DWORD GetDrawBufferAddress(void)
	{
        return _drawbuffer;
    }
	
	/*********************************************************************
	* Function: DWORD GetFrameBufferAddress(void)
	*
	* PreCondition: _drawbuffer must be set to either GFX_BUFFER1 or GFX_BUFFER2
	*
	* Input: none
	*
	* Output: Address of the Frame Buffer
	*
	* Side Effects: none
	*
	* Overview: Returns the address of Frame Buffer
	*
	* Note: 
	*
	********************************************************************/
    DWORD GetFrameBufferAddress(void)
	{
        if(_drawbuffer == GFX_BUFFER1)
        {
            return GFX_BUFFER2;
        }
        else
        {
            return GFX_BUFFER1;
        }
    }

#endif //USE_DOUBLE_BUFFERING

#ifdef USE_COMP_IPU
    
    #if (GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE > 1024)
        #error "GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE must be <= 1024"
    #endif
    
    #ifndef GFX_DECOMPRESSED_BUFFER_SIZE
        #error "GFX_DECOMPRESSED_BUFFER_SIZE is not defined. This must be defined in Hardware Profile."
    #endif
    
#endif // #ifdef USE_COMP_IPU

// check if we can use the driver OutChar
#if (DISP_ORIENTATION == 0)
    #if !(defined(USE_ANTIALIASED_FONTS))
        #define USE_DRV_FONT
    #endif
#endif    

// Color
GFX_COLOR   _color;
GFX_COLOR   _chrcolor;
#ifdef USE_TRANSPARENT_COLOR
GFX_COLOR   _colorTransparent;
SHORT       _colorTransparentEnable;
#endif

// work areas base addresses
volatile DWORD 		_workArea1BaseAddr;
volatile DWORD 		_workArea2BaseAddr;

// display buffer base address
volatile DWORD 		_displayAreaBaseAddr;

#define GFX_FLIP(a,b) { SHORT t=a; a=b; b=t; }

/////////////////////// LOCAL FUNCTIONS PROTOTYPES ////////////////////////////
SHORT 	DrvGetImageColorDepth(void *pImage);
SHORT   DrvGetImageHeight(void *pImage);
SHORT   DrvGetImageWidth(void *pImage);

SHORT   GetTextWidthEds(XCHAR *textString, void *pFont);
SHORT   GetTextHeightEds(void *pFont);
void    SetFontEds(void *pFont);
WORD 	OutCharEds(XCHAR ch);

#define DrvGetX() (G1CHRX)
#define DrvGetY() (G1CHRY)

#ifdef GFX_DRV_PAGE_COUNT
    volatile DWORD _PageTable[GFX_DRV_PAGE_COUNT];
#endif

void EPMP_Init(void)
{
	/* Note: When using the EPMP to access external RAM or Flash, PMA0-PMA16 will only access a range of 
	         256K RAM. To increase this range enable higher Address lines. To do this set the GFX_EPMP_CS1_MEMORY_SIZE
			 or GFX_EPMP_CS2_MEMORY_SIZE in the Hardware Profile.
	*/

#if defined (GFX_EPMP_CS1_BASE_ADDRESS) || defined (GFX_EPMP_CS2_BASE_ADDRESS)

    volatile WORD gfxAccessTime; 
	    	 WORD deviceAccessTime;

    /* Depends on the selected clock source frequency calculate the divider if using 
       external memory, calcAMWAIT is the minimum EPMP Alternate Master Transfer 
       from (1000000000ul) / (DWORD)((CLKDIVbits.G1CLKSEL == 1)? 96000000ul : 48000000ul);
       there is a factor of 100 to take care of the two decimal places on the calculation

    */
    if (CLKDIVbits.G1CLKSEL == 1)
        gfxAccessTime = 1041; // from 1/96 Mhz 
    else
        gfxAccessTime = 2083; // from 1/48 Mhz 

    /* Note:
        When using devices that has Alternate PMP pin configuration (ALTPMP)
        there are address pins that are remapped to other pins to free
        up some analog pins. Make sure the pins assigned to address lines 
        PMA[x] are set to digital mode.
        See Device Data Sheet for details.
    */
    // Assuming ALTPMP = 0 
    ANSDbits.ANSD7 = 0;   // PMD15
    ANSDbits.ANSD6 = 0;   // PMD14
    ANSFbits.ANSF0 = 0;   // PMD11
    
    ANSBbits.ANSB15 = 0;  // PMA0
    ANSBbits.ANSB14 = 0;  // PMA1
    ANSGbits.ANSG9  = 0;  // PMA2
    ANSBbits.ANSB13 = 0;  // PMA10
    ANSBbits.ANSB12 = 0;  // PMA11
    ANSBbits.ANSB11 = 0;  // PMA12
    ANSBbits.ANSB10 = 0;  // PMA13
    ANSAbits.ANSA7 = 0;   // PMA17
    ANSGbits.ANSG6 = 0;   // PMA18

    // uncomment corresponding lines if PMA22:19] is/are used
    // as device address lines
    //ANSEbits.ANSE9 = 0;   // PMA19
    //ANSGbits.ANSG7 = 0;   // PMA20
    //ANSGbits.ANSG8 = 0;   // PMA21
    //ANSCbits.ANSC4 = 0;   // PMA22


	PMCON1bits.ADRMUX = 0;	    								// address is not multiplexed
	PMCON1bits.MODE = 3;        								// master mode
	PMCON1bits.CSF = 0;         								// PMCS1 pin used for chip select 1, PMCS2 pin used for chip select 2
	PMCON1bits.ALP = 0;                                         // Don't care (not used)          							
	PMCON1bits.ALMODE = 0;      								// Don't care (not used)
	PMCON1bits.BUSKEEP = 0;     								// bus keeper is not used

  	#if defined (GFX_EPMP_CS1_BASE_ADDRESS)

		PMCS1BS = (GFX_EPMP_CS1_BASE_ADDRESS>>8);				// CS1 start address
		
		// check if how many address lines to use 
        // Note: 
        //      - GFX_EPMP_CS1_MEMORY_SIZE is word (2 bytes) count
        //      - assumes that the PMA[x] signal/pin is mapped to an actual device
#if   (((GFX_EPMP_CS1_MEMORY_SIZE) > 0x10000ul)  && ((GFX_EPMP_CS1_MEMORY_SIZE) <= 0x20000ul))
        PMCON3 |= 0x0001;                                       // PMA16 address line is enabled 
#elif (((GFX_EPMP_CS1_MEMORY_SIZE) > 0x20000ul)  && ((GFX_EPMP_CS1_MEMORY_SIZE) <= 0x40000ul))
        PMCON3 |= 0x0003;                                       // PMA[17:16] address lines are enabled 
#elif (((GFX_EPMP_CS1_MEMORY_SIZE) > 0x40000ul)  && ((GFX_EPMP_CS1_MEMORY_SIZE) <= 0x80000ul))
        PMCON3 |= 0x0007;                                       // PMA[18:16] address lines are enabled 
#elif (((GFX_EPMP_CS1_MEMORY_SIZE) > 0x80000ul)  && ((GFX_EPMP_CS1_MEMORY_SIZE) <= 0x100000ul))
        PMCON3 |= 0x000F;                                       // PMA[19:16] address lines are enabled 
#elif (((GFX_EPMP_CS1_MEMORY_SIZE) > 0x100000ul) && ((GFX_EPMP_CS1_MEMORY_SIZE) <= 0x200000ul))
        PMCON3 |= 0x001F;                                       // PMA[20:16] address lines are enabled 
#elif (((GFX_EPMP_CS1_MEMORY_SIZE) > 0x200000ul) && ((GFX_EPMP_CS1_MEMORY_SIZE) <= 0x400000ul))
        PMCON3 |= 0x003F;                                       // PMA[21:16] address lines are enabled 
#elif (((GFX_EPMP_CS1_MEMORY_SIZE) > 0x400000ul) && ((GFX_EPMP_CS1_MEMORY_SIZE) <= 0x800000ul))
        PMCON3 |= 0x007F;                                       // PMA[22:16] address lines are enabled 
#endif           

		PMCS1CFbits.CSDIS = 0;      							// enable CS
		PMCS1CFbits.CSP = EPMPCS1_CS_POLARITY;        			// CS1 polarity 
		PMCS1CFbits.BEP = EPMPCS1_BE_POLARITY;       	 		// byte enable polarity
		PMCS1CFbits.WRSP = EPMPCS1_WR_POLARITY;       			// write strobe polarity
		PMCS1CFbits.RDSP = EPMPCS1_RD_POLARITY;       			// read strobe polarity
		PMCS1CFbits.CSPTEN = 1;     							// enable CS port
		PMCS1CFbits.SM = 0;         							// read and write strobes on separate lines 
		PMCS1CFbits.PTSZ = 2;       							// data bus width is 16-bit 
	
		PMCS1MDbits.ACKM = 0;        							// PMACK is not used
		

		// Calculate the AMWAIT cycles to satisfy the access time of the device
        deviceAccessTime = EPMPCS1_ACCESS_TIME*100;

        if ((deviceAccessTime) < gfxAccessTime)
		    PMCS1MDbits.AMWAIT = 0;
		else if (deviceAccessTime >= gfxAccessTime)
		{
            PMCS1MDbits.AMWAIT = (deviceAccessTime/gfxAccessTime);
            if ((deviceAccessTime % gfxAccessTime) > 0)
                PMCS1MDbits.AMWAIT += 1;
        }   

	#else
			PMCS1CFbits.CSDIS = 1;       						// disable CS1 functionality  
	#endif //#if defined (GFX_EPMP_CS1_BASE_ADDRESS)

	#if defined (GFX_EPMP_CS2_BASE_ADDRESS)

		PMCS2BS = (GFX_EPMP_CS2_BASE_ADDRESS>>8);				// CS2 start address

		// check if how many address lines to use 
        // Note: 
        //      - GFX_EPMP_CS2_MEMORY_SIZE is in word (2 bytes) count
        //      - assumes that the PMA[x] signal/pin is mapped to an actual device
#if   (((GFX_EPMP_CS2_MEMORY_SIZE) > 0x10000ul)  && ((GFX_EPMP_CS2_MEMORY_SIZE) <= 0x20000ul))
        PMCON3 |= 0x0001;                                       // PMA16 address line is enabled 
#elif (((GFX_EPMP_CS2_MEMORY_SIZE) > 0x20000ul)  && ((GFX_EPMP_CS2_MEMORY_SIZE) <= 0x40000ul))
        PMCON3 |= 0x0003;                                       // PMA[17:16] address lines are enabled 
#elif (((GFX_EPMP_CS2_MEMORY_SIZE) > 0x40000ul)  && ((GFX_EPMP_CS2_MEMORY_SIZE) <= 0x80000ul))
        PMCON3 |= 0x0007;                                       // PMA[18:16] address lines are enabled 
#elif (((GFX_EPMP_CS2_MEMORY_SIZE) > 0x80000ul)  && ((GFX_EPMP_CS2_MEMORY_SIZE) <= 0x100000ul))
        PMCON3 |= 0x000F;                                       // PMA[19:16] address lines are enabled 
#elif (((GFX_EPMP_CS2_MEMORY_SIZE) > 0x100000ul) && ((GFX_EPMP_CS2_MEMORY_SIZE) <= 0x200000ul))
        PMCON3 |= 0x001F;                                       // PMA[20:16] address lines are enabled 
#elif (((GFX_EPMP_CS2_MEMORY_SIZE) > 0x200000ul) && ((GFX_EPMP_CS2_MEMORY_SIZE) <= 0x400000ul))
        PMCON3 |= 0x003F;                                       // PMA[21:16] address lines are enabled 
#elif (((GFX_EPMP_CS2_MEMORY_SIZE) > 0x400000ul) && ((GFX_EPMP_CS2_MEMORY_SIZE) <= 0x800000ul))
        PMCON3 |= 0x007F;                                       // PMA[22:16] address lines are enabled 
#endif           

		PMCS2CFbits.CSDIS = 0;      							// enable CS

		PMCS2CFbits.CSP = EPMPCS2_CS_POLARITY;        			// CS2 polarity 
		PMCS2CFbits.BEP = EPMPCS2_BE_POLARITY;       	 		// byte enable polarity
		PMCS2CFbits.WRSP = EPMPCS2_WR_POLARITY;       			// write strobe polarity
		PMCS2CFbits.RDSP = EPMPCS2_RD_POLARITY;       			// read strobe polarity
		PMCS2CFbits.CSPTEN = 1;     							// enable CS port
		PMCS2CFbits.SM = 0;         							// read and write strobes on separate lines 
		PMCS2CFbits.PTSZ = 2;       							// data bus width is 16-bit 
	
		PMCS2MDbits.ACKM = 0;        							// PMACK is not used
		
		// Calculate the AMWAIT cycles to satisfy the access time of the device
        deviceAccessTime = EPMPCS2_ACCESS_TIME*100;

		if ((deviceAccessTime) < gfxAccessTime)
		    PMCS2MDbits.AMWAIT = 0;
		else if (deviceAccessTime >= gfxAccessTime)
		{
            PMCS2MDbits.AMWAIT = (deviceAccessTime/gfxAccessTime);
            if ((deviceAccessTime % gfxAccessTime) > 0)
                PMCS2MDbits.AMWAIT += 1;
        }          

	#else	
		PMCS2CFbits.CSDIS = 1;       							// disable CS2 functionality  
		PMCS2BS = 0x0000;
	#endif //#if defined (GFX_EPMP_CS2_BASE_ADDRESS)
	
	PMCON2bits.RADDR = 0xFF;									// set CS2 end address
	PMCON4 = 0xFFFF;            								// PMA0 - PMA15 address lines are enabled

	PMCON3bits.PTWREN = 1;      								// enable write strobe port
	PMCON3bits.PTRDEN = 1;      								// enable read strobe port
	PMCON3bits.PTBE0EN = 1;     								// enable byte enable port
	PMCON3bits.PTBE1EN = 1;     								// enable byte enable port
	PMCON3bits.AWAITM = 0;      								// set address latch pulses width to 1/2 Tcy
	PMCON3bits.AWAITE = 0;      								// set address hold time to 1/4 Tcy
	
	DelayMs(100);

	PMCON2bits.MSTSEL = 3;										// select EPMP bypass mode (for Graphics operation)
	PMCON1bits.PMPEN = 1;										// enable the module

	DelayMs(100);

#endif // end of #if defined (GFX_EPMP_CS1_BASE_ADDRESS) || defined (GFX_EPMP_CS2_BASE_ADDRESS)

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
* Overview: Resets LCD, initializes PMP. FRM Section 43. Graphics 
*			Controller Module (GFX) (Document #:DS39731) for details.
*
* Note: none
*
********************************************************************/
void ResetDevice(void)
{
#ifdef GFX_DRV_PAGE_COUNT
    int i;
#endif
	/* Switch On display if IO controlled; if controlled 
	   by the Graphics Module, hook up the power-on signal of the display to
	   GPWR pin of the Graphics Module and this code should not compile */
	   
    DisplayPowerOn();
    DisplayPowerConfig();

    /* Set up the Graphics Module clock source and the GCLK frequency. */
    _G1CLKSEL = 1;  				// 96MHz Enable
    _GCLKDIV = GFX_GCLK_DIVIDER;	// value used is dependent on the display panel specification

    EPMP_Init();

/************ End - Project Specific Code - Relocate ************/


    G1CON1   = 0; /* Switch off the module */
    G1CON2   = 0;
    G1CON3   = 0;
    G1IE     = 0;
    G1IR     = 0;
    G1CLUT   = 0;
    G1MRGN   = 0;
    G1CLUTWR = 0;
    G1CHRX   = 0;
    G1CHRY   = 0;
    G1CMDL   = 0;
    G1CMDH   = 0;
    
    // set the  processing unit bit per pixel
    _PUBPP   = GFX_BITS_PER_PIXEL;
    // set the  display controller bit per pixel
    _DPBPP   = GFX_BITS_PER_PIXEL;
    // set the LCD type used (TFT, MSTN or CSTN)
    _DPMODE  = GFX_LCD_TYPE;

    #if (GFX_LCD_TYPE == GFX_LCD_MSTN) || (GFX_LCD_TYPE == GFX_LCD_CSTN)
		// set the display width
        _DPGWDTH = STN_DISPLAY_WIDTH;

    #endif

    /* Port configurations */
    #ifdef GFX_DISPLAYENABLE_ENABLE
    	// set the display enable polarity
        _DPENPOL = GFX_DISPLAYENABLE_POLARITY;
        _DPENOE = 1;

    #endif
    
    #ifdef GFX_HSYNC_ENABLE
		// set the HSYNC signal polarity
        _DPHSPOL = GFX_HSYNC_POLARITY;
        _DPHSOE = 1;

    #endif
    
    #ifdef GFX_VSYNC_ENABLE
		// set the VSYNC signal polarity
        _DPVSPOL = GFX_VSYNC_POLARITY;
        _DPVSOE = 1;
    
    #endif
    
    #ifdef GFX_DISPLAYPOWER_ENABLE
        // enable the GPWR pin, when this is enabled it can be used
        // to control external power circuitry for the display
        _DPPWROE = 1;
    #endif
    
    /* Display timing signal configurations */
    // set the clock polarity	
    _DPCLKPOL = GFX_CLOCK_POLARITY;
    
    // set the display buffer dimension
    G1DPW = DISP_HOR_RESOLUTION;
    G1DPH = DISP_VER_RESOLUTION;
    
    // set the work area dimension
    G1PUW = DISP_HOR_RESOLUTION;
    G1PUH = DISP_VER_RESOLUTION;
    
    /* Note:
    	In some display panel the definition of porches (front and back porches) varies.
    	Example TFT display definitions (for horizontal timing):
    		1. Horizontal Cycle = horizontal front porch + horizontal back porch + horizontal display period
    		2. Horizontal Cycle = horizontal front porch + horizontal back porch + horizontal display period + horizontal sync pulse width
		In example (1) the horizontal sync pulse width must not exceed the horizontal back porch.
		For the vertical timing, the equations are the same (replace horizontal with vertical).
		
		For the Microchip graphics controller: the definition follows example (2). To accomodate displays like
		example (1), adjust the back porches and pulse widths accordingly. Refer to 
		FRM Section 43. Graphics Controller Module (GFX) (Document #:DS39731).
    */
    #define HT  (DISP_HOR_PULSE_WIDTH + DISP_HOR_BACK_PORCH + DISP_HOR_FRONT_PORCH + DISP_HOR_RESOLUTION)
    #define VT  (DISP_VER_PULSE_WIDTH + DISP_VER_BACK_PORCH + DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION)

    G1DPWT = HT;
    G1DPHT = VT;

	// set the horizontal pulse width
    _HSLEN = DISP_HOR_PULSE_WIDTH;
    _HSST  = 0; 
    
	// set the verrizontal pulse width
    _VSLEN = DISP_VER_PULSE_WIDTH;
    _VSST  = 0; 

	// set the horizontal & vertical start position
    _HENST = _HSST + DISP_HOR_PULSE_WIDTH + DISP_HOR_BACK_PORCH;
    _VENST = _VSST + DISP_VER_PULSE_WIDTH + DISP_VER_BACK_PORCH;

	// set the active pixel and active line start position
	_ACTPIX  = _HENST;
	_ACTLINE = _VENST;
	
	// initialize the work areas and display buffer base addresses global variables
    _workArea1BaseAddr   = GFX_DISPLAY_BUFFER_START_ADDRESS;
	_workArea2BaseAddr   = GFX_DISPLAY_BUFFER_START_ADDRESS;

    // Set the display buffer base address (SFR) (or the start addresses in RAM) 
    GFX_SetDisplayArea(GFX_DISPLAY_BUFFER_START_ADDRESS);	
    
    /* Switch On the Graphics Module */
    _GDBEN = 0xFFFF;
    _DPPINOE = 1;

    #ifdef USE_PALETTE
    	// initialize the color look up table (CLUT) if enabled
        PaletteInit();
    
    #endif
  
    _DPSTGER = 0;					// display data stagger (set to none)
    _GFX1IF = 0;					// clear graphics interrupt flag 
    _GFX1IE = 0;					// graphics interrupt are not enabled
    _G1EN = 1;						// turn on the graphics module

    DelayMs(100);
    
    // Turn on the display refresh sequence. This control signal
    // will be reflected in GPWR pin if  _DPPWROE is set to 1.
    // GPWR will immediately be set to 1 after DPPOWER bit is set.
    // Actual refresh sequence will be delayed and the delay is set 
    // by GICLUTWR<15:0> x 64 GCLK Cycles. 
    // Refer to the Graphics Module FRM document for details.
    // Section 43.7 DISPLAY POWER SEQUENCING
    _DPPOWER = 1;					

#ifdef USE_DOUBLE_BUFFERING
    // initialize double buffering feature
    blInvalidateAll = 1;            
    blDisplayUpdatePending = 0;
    NoOfInvalidatedRectangleAreas = 0;
    _drawbuffer = GFX_BUFFER1;
    SwitchOnDoubleBuffering();
#endif //USE_DOUBLE_BUFFERING

#ifdef GFX_DRV_PAGE_COUNT
    for (i = 0; i < GFX_DRV_PAGE_COUNT; i++)
    {
        _PageTable[i] = GFX_DISPLAY_BUFFER_START_ADDRESS + ((GFX_DISPLAY_BUFFER_LENGTH + 2) * i);
    }
#endif

	// clear the screen to all black first to remove the noise screen
	SetColor(0);
	Bar(0,0,GetMaxX(),GetMaxY());

    #if defined (USE_TCON_MODULE)
        GfxTconInit();    				// Panel Timing Controller (TCON) Programming
    #endif    

    DelayMs(100);

	// disable clipping (default setting)    
    SetClip(0);
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
* Side Effects: This is a blocking call. Succeeding RCCGPU using 
*               RCC_TRANSPARENT_COPY will use the color set as 
*               transparent color.
*
********************************************************************/
void TransparentColorEnable(GFX_COLOR color)
{
    _colorTransparent = color;    
    _colorTransparentEnable = TRANSPARENT_COLOR_ENABLE;

}
#endif

/*********************************************************************
* Function: SetClip(control)
*
* Overview: Enables/disables clipping.
*
* PreCondition: none
*
* Input: control - Enables or disables the clipping.
*			- 0: Disable clipping
*			- 1: Enable clipping
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SetClip(BYTE control)
{
    _clipRgn=control;
    
    if(_clipRgn)
    {
        GFX_WaitForCommandQueue(2);
        GFX_CHR_SetTextAreaStart(_clipLeft, _clipTop);
        GFX_CHR_SetTextAreaEnd(_clipRight, _clipBottom) ;
    }
    else
    {
        GFX_WaitForCommandQueue(2);
        GFX_CHR_SetTextAreaStart(0, 0);
        GFX_CHR_SetTextAreaEnd(GetMaxX(), GetMaxY());
    }
}

/*********************************************************************
* Function: SetClipRgn(left, top, right, bottom)
*
* Overview: Sets clipping region.
*
* PreCondition: none
*
* Input: left - Defines the left clipping region border.
*		 top - Defines the top clipping region border.
*		 right - Defines the right clipping region border.
*	     bottom - Defines the bottom clipping region border.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SetClipRgn(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    _clipLeft=left;
    _clipTop=top;
    _clipRight=right;
    _clipBottom=bottom;

    if(_clipRgn)
    {
        SetClip(_clipRgn);
    }
}

/*********************************************************************
* Function: WORD ROPBlockInline (DWORD srcAddr,   DWORD dstAddr, 
*						   DWORD srcOffset, DWORD dstOffset, 
*			               WORD  srcType,   WORD  dstType,  
*                          WORD  copyOp,    WORD rop, 
*			               WORD color,      WORD width,  WORD height)
*
* PreCondition: none
*
* Input: srcAddr - the base address of the data to be moved
*        dstAddr - the base address of the new location of the moved data 
*        srcOffset - offset of the data to be moved with respect to the 
*					 source base address.
*        dstOffset - offset of the new location of the moved data respect 
*					 to the source base address.
*        srcType - sets the source type (continuous or discontinuous)
*        dstType - sets the source type (continuous or discontinuous) 
*        copyOp - sets the type of copy operation
*			- RCC_SOLID_FILL: Solid fill of the set color
*			- RCC_COPY: direct copy of source to destination
*			- RCC_TRANSPARENT_COPY: copy with transparency. Transparency color is set by color
*        rop - sets the raster operation equation
*			- RCC_ROP_0: Solid black color fill 
*			- RCC_ROP_1: not (Source or Destination)
*			- RCC_ROP_2: (not Source) and Destination
*			- RCC_ROP_3: not Source 
*			- RCC_ROP_4: Source and (not Destination)
*			- RCC_ROP_5: not Destination
*			- RCC_ROP_6: Source xor Destination
*			- RCC_ROP_7: not (Source and Destination) 
*			- RCC_ROP_8: Source and Destination
*			- RCC_ROP_9: not (Source xor Destination) 
*			- RCC_ROP_A: Destination
*			- RCC_ROP_B: (not Source) or Destination 
*			- RCC_ROP_C: Source 
*			- RCC_ROP_D: Source or (not Destination)
*			- RCC_ROP_E: Source or Destination
*			- RCC_ROP_F: Solid white color fill 
*        color - color value used when transparency operation is set or using solid color fill
*        width - width of the block of data to be moved
*        height - height of the block of data to be moved
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and operation is not completely performed.
*         - Returns 1 when the operation is completely performed.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: This is an internal function. This is declared inline for maintenance purposes
*           and speed of operation. There will functions that will need the basic ROP 
*           and instead of using macros, they are declared as functions.
*           Performs the rectangle copy operation with the indicated 
*           type of copy and raster operation. 
*
* Note: none
*
********************************************************************/
inline WORD __attribute__ ((always_inline)) ROPBlockInline(DWORD srcAddr,   DWORD dstAddr,   \
                                                                  DWORD srcOffset, DWORD dstOffset, \
			                                                      WORD srcType,    WORD dstType,    \
                                                                  WORD copyOp,     WORD rop,        \
                                                                  WORD color,      WORD width,      \
                                                                  WORD height)
{
	DWORD_VAL workArea1Temp, workArea2Temp;
	
	GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    GFX_WaitForGpu();
    
    // store the work area settings temporarily
    workArea1Temp.w[0] = G1W1ADRL;
    workArea1Temp.w[1] = G1W1ADRH;
    workArea2Temp.w[0] = G1W2ADRL;
    workArea2Temp.w[1] = G1W2ADRH;

	GFX_SetWorkArea1(srcAddr);
	GFX_SetWorkArea2(dstAddr);
	
	GFX_RCC_SetSrcOffset(srcOffset);
	GFX_RCC_SetDestOffset(dstOffset);

	GFX_RCC_SetSize(width, height);
	
	if ((copyOp == RCC_TRANSPARENT_COPY) || (copyOp == RCC_SOLID_FILL))
		GFX_RCC_SetColor(color);

    if ((srcType == GFX_DATA_CONTINUOUS) || (srcType > 0))
        srcType = RCC_SRC_ADDR_CONTINUOUS;
    if ((dstType == GFX_DATA_CONTINUOUS) || (dstType > 0))
        dstType = RCC_DEST_ADDR_CONTINUOUS;
	GFX_RCC_StartCopy(copyOp, rop, srcType, dstType);

    GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    GFX_WaitForGpu();

    // restore the work area settings 
    G1W1ADRL = workArea1Temp.w[0];
    G1W1ADRH = workArea1Temp.w[1];
    G1W2ADRL = workArea2Temp.w[0];
    G1W2ADRH = workArea2Temp.w[1];

	return (1);
	
}	


/*********************************************************************
* Function: WORD ROPBlock (DWORD srcAddr,   DWORD dstAddr, 
*						   DWORD srcOffset, DWORD dstOffset, 
*			               WORD  srcType,   WORD  dstType,  
*                          WORD  copyOp,    WORD rop, 
*			               WORD color,      WORD width,  WORD height)
*
* PreCondition: none
*
* Input: srcAddr - the base address of the data to be moved
*        dstAddr - the base address of the new location of the moved data 
*        srcOffset - offset of the data to be moved with respect to the 
*					 source base address.
*        dstOffset - offset of the new location of the moved data respect 
*					 to the source base address.
*        srcType - sets the source type (continuous or discontinuous)
*        dstType - sets the source type (continuous or discontinuous) 
*        copyOp - sets the type of copy operation
*			- RCC_SOLID_FILL: Solid fill of the set color
*			- RCC_COPY: direct copy of source to destination
*			- RCC_TRANSPARENT_COPY: copy with transparency. Transparency color is set by color
*        rop - sets the raster operation equation
*			- RCC_ROP_0: Solid black color fill 
*			- RCC_ROP_1: not (Source or Destination)
*			- RCC_ROP_2: (not Source) and Destination
*			- RCC_ROP_3: not Source 
*			- RCC_ROP_4: Source and (not Destination)
*			- RCC_ROP_5: not Destination
*			- RCC_ROP_6: Source xor Destination
*			- RCC_ROP_7: not (Source and Destination) 
*			- RCC_ROP_8: Source and Destination
*			- RCC_ROP_9: not (Source xor Destination) 
*			- RCC_ROP_A: Destination
*			- RCC_ROP_B: (not Source) or Destination 
*			- RCC_ROP_C: Source 
*			- RCC_ROP_D: Source or (not Destination)
*			- RCC_ROP_E: Source or Destination
*			- RCC_ROP_F: Solid white color fill 
*        color - color value used when transparency operation is set or using solid color fill
*        width - width of the block of data to be moved
*        height - height of the block of data to be moved
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and operation is not completely performed.
*         - Returns 1 when the operation is completely performed.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: Performs the rectangle copy operation with the indicated 
*           type of copy and raster operation. 
*
* Note: none
*
********************************************************************/
WORD ROPBlock(DWORD srcAddr, DWORD dstAddr, DWORD srcOffset, DWORD dstOffset, 
			   WORD srcType, WORD dstType, WORD copyOp, WORD rop, WORD color, WORD width, WORD height)
{
    return (ROPBlockInline( srcAddr, dstAddr,       \
                            srcOffset, dstOffset,   \
                            srcType, dstType,       \
                            copyOp, rop,            \
                            color, width, height));
}	

/*********************************************************************
* Function: WORD CopyBlock(DWORD srcAddr, DWORD dstAddr, 
*						   DWORD srcOffset, DWORD dstOffset, 
*                          WORD width, WORD height)
*
* Overview: Copies a block of data from source specified by srcAddr 
*           and srcOffset to the destination specified by dstAddr 
*           and dstOffset.
*
* PreCondition: none
*
* Input: srcAddr - the base address of the data to be moved
*        dstAddr - the base address of the new location of the moved data 
*        srcOffset - offset of the data to be moved with respect to the 
*					 source base address.
*        dstOffset - offset of the new location of the moved data respect 
*					 to the source base address.
*        width - width of the block of data to be moved
*        height - height of the block of data to be moved
*
* Output: none
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD CopyBlock(DWORD srcAddr, DWORD dstAddr, DWORD srcOffset, DWORD dstOffset, WORD width, WORD height)
{

    return (ROPBlockInline( srcAddr, dstAddr,                                       \
                            srcOffset, dstOffset,                                   \
                            (GFX_DATA_DISCONTINUOUS), (GFX_DATA_DISCONTINUOUS),     \
                            RCC_COPY, RCC_ROP_C,                                    \
                            0, width, height));

}


/*********************************************************************
* Function: WORD CopyWindow(WORD srcAddr, WORD dstAddr, 
*                               WORD srcX, WORD srcY, 
*                               WORD dstX, WORD dstY, 
*                               WORD width, WORD height)  
*
* PreCondition: none
*
* Input: srcAddr - base address of the source window,
*        dstAddr - base address of the destination window,
*        srcX - x location of the left top corner of the source window 
*               respect to the srcPage.
*        srcY - y location of the left top corner of the source window 
*               respect to the srcPage.
*        dstX - x location of the left top corner of the destination window 
*               respect to the dstPage.
*        dstY - y location of the left top corner of the destination window 
*               respect to the dstPage.
*        width - the width in pixels of the window to copy
*        height - the height in pixels of the window to copy
*
* Output: None
*
* Side Effects: none
*
* Overview: This is a blocking function.
*           A windows is a rectangular area with the given width and height  
*           located in the given base address . The source and destination 
*           window can be located in any memory location. If the source and 
*           destination is located in one page, then the srcAddr = dstAddr.
*
********************************************************************/
WORD CopyWindow(DWORD srcAddr, DWORD dstAddr,       \
                WORD srcX, WORD srcY,               \
                WORD dstX, WORD dstY,               \
                WORD width, WORD height)                
{
    DWORD srcOffset, dstOffset;
    WORD  sLeft, sTop, dLeft, dTop, actualWidth, actualHeight;

    // calculate the offset based on the given x and y locations.
    // depending on the screen orientation, the actual x,y locations
    // are calculated.
    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
        actualHeight = width;
        actualWidth  = height;
    #else
        actualHeight = height;
        actualWidth  = width;
    #endif

    #if (DISP_ORIENTATION == 90)

        sLeft =   srcY;
        sTop  =   DISP_VER_RESOLUTION - srcX - actualHeight;
        dLeft =   dstY;
        dTop  =   DISP_VER_RESOLUTION - dstX - actualHeight;

    #elif (DISP_ORIENTATION == 180)

        sLeft =   DISP_HOR_RESOLUTION - srcX - actualWidth;
        sTop  =   DISP_VER_RESOLUTION - srcY - actualHeight;
        dLeft =   DISP_HOR_RESOLUTION - dstX - actualWidth;
        dTop  =   DISP_VER_RESOLUTION - dstY - actualHeight;

    #elif (DISP_ORIENTATION == 270)

        sTop  =   srcX;
        sLeft =   DISP_HOR_RESOLUTION - srcY - actualWidth;
        dTop  =   dstX;
        dLeft =   DISP_HOR_RESOLUTION - dstY - actualWidth;

    #else

        sLeft = srcX;
        sTop  = srcY;
        dLeft = dstX;
        dTop  = dstY;

    #endif

    // offsets are calculated in terms of pixel count. The effective address
    // gets resolved when the color depth used is taken into account.
    // the color depth is the value programmed into the 
    // PUBPP<2:0>: GPU bits-per-pixel Setting bits
    srcOffset = (DWORD)((sTop * (DWORD)DISP_HOR_RESOLUTION) + sLeft);
    dstOffset = (DWORD)((dTop * (DWORD)DISP_HOR_RESOLUTION) + dLeft);

    // do the block copy with RCC_COPY mode and RCC_ROP_C which is copy source
    // color value is irrelevant here since we are doing a direct source copy
    ROPBlock(   srcAddr, dstAddr, srcOffset, dstOffset, 
			    GFX_DATA_DISCONTINUOUS, GFX_DATA_DISCONTINUOUS, 
                RCC_COPY, RCC_ROP_C, 0, actualWidth, actualHeight);

    return (1);
}

#ifdef GFX_DRV_PAGE_COUNT

/*********************************************************************
* Function: WORD CopyPageWindow(BYTE srcPage, BYTE dstPage, 
*                               WORD srcX, WORD srcY, 
*                               WORD dstX, WORD dstY, 
*                               WORD width, WORD height)  
*
* PreCondition: none
*
* Input: srcPage - page number of the source window,
*        dstPage - page number of the destination window,
*        srcX - x location of the left top corner of the source window 
*               respect to the srcPage.
*        srcY - y location of the left top corner of the source window 
*               respect to the srcPage.
*        dstX - x location of the left top corner of the destination window 
*               respect to the dstPage.
*        dstY - y location of the left top corner of the destination window 
*               respect to the dstPage.
*        width - the width in pixels of the window to copy
*        height - the height in pixels of the window to copy
*
* Output: None
*
* Side Effects: none
*
* Overview: This is a blocking call.
*           A windows is a rectangular area with the given width and height  
*           of a page. The source and destination window can be located in 
*           different pages and each page is assumed to have the same dimensions
*           (equal width and height).
*
********************************************************************/
WORD CopyPageWindow(BYTE srcPage, BYTE dstPage,         \
                    WORD srcX, WORD srcY,               \
                    WORD dstX, WORD dstY,               \
                    WORD width, WORD height)                
{

    CopyWindow(GetPageAddress(srcPage), GetPageAddress(dstPage), srcX, srcY, dstX, WORD dstY, width, height);    

    return (1);
}
#endif // #ifdef GFX_DRV_PAGE_COUNT


/*********************************************************************
* Function: WORD Scroll(SHORT left, SHORT top, SHORT right, SHORT bottom, 
*                       SHORT delta, WORD dir)
*
* PreCondition: none
*
* Input: left - left position of the scrolled rectangle
*        top - top position of the scrolled rectangle
*        right - right position of the scrolled rectangle
*        bottom - bottom position of the scrolled rectangle
*        delta - defines the scroll delta
*        dir - defines the direction of the scroll. 
*		 - 0 : scroll to the left
*		 - 1 : scroll to the right
*
* Output: none
*
* Side Effects: none
*
* Overview: Scrolls the rectangular area defined by left, top, right, bottom by 
*           delta pixels with the dir defining the direction (left or right).
*
* Note: none
*
********************************************************************/
WORD Scroll(SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT delta, WORD dir)
{
	DWORD address;
    SHORT width, height;

	// make sure there are no pending RCC GPU commands
    if(GFX_GetFreeCommandSpace() < GFX_COMMAND_QUEUE_LENGTH)
    {
        #ifndef USE_NONBLOCKING_CONFIG
            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
        #else
            return (0);
        #endif
    }

    #if (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

        width   =   right - left + 1;
        height  =   bottom - top + 1;

    #elif (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

        height  =   right - left + 1;
        width   =   bottom - top + 1;

    #endif

    #if (DISP_ORIENTATION == 90)

        bottom  =   DISP_VER_RESOLUTION - left;
        left    =   top;
        right   =   left + width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 180)

        right   =   DISP_HOR_RESOLUTION - left;
        bottom  =   DISP_VER_RESOLUTION - top;
        left    =   right - width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 270)

        right   =   DISP_HOR_RESOLUTION - top;
        top     =   left;
        bottom  =   top + height;
        left    =   right - width;

    #endif
	
	
	GFX_SetWorkArea1(_workArea1BaseAddr);
	GFX_SetWorkArea2(_workArea2BaseAddr); 
		
	// get source address
	address = (top * (DWORD)_PUW) + left;
	GFX_RCC_SetSrcOffset(address);

	if (dir == 0) // go left
	{
		// get destination address
	    #if (DISP_ORIENTATION == 90)
			address = (bottom * (DWORD)_PUW) + left;
		#else	
			address = (top * (DWORD)_PUW) + left - delta;
		#endif	
	}
	else // go right
	{
		// get destination address
	    #if (DISP_ORIENTATION == 90)
			address = ((top-delta) * (DWORD)_PUW) + left;
		#else	
			address = (top * (DWORD)_PUW) + left + delta;
		#endif			
	}
	GFX_RCC_SetDestOffset(address);		
	
	GFX_RCC_SetSize(width, height);
	GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_DEST_ADDR_DISCONTINUOUS,RCC_DEST_ADDR_DISCONTINUOUS);
	
	return (1);
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

    #if (DISP_ORIENTATION == 90)
    {
        SHORT t = x;
        x = y;
        y = GetMaxX() - t;
    }
    #elif (DISP_ORIENTATION == 180)
    {
        x = GetMaxX() - x;
        y = GetMaxY() - y;
    }
    #elif (DISP_ORIENTATION == 270)
    {
        SHORT t = x;
        x = GetMaxY() - y;
        y = t;
    }
    #endif

    #ifdef USE_DOUBLE_BUFFERING
        GFX_SetWorkArea2(_drawbuffer);
    #else
        GFX_SetWorkArea2(_workArea2BaseAddr); 
    #endif

    GFX_WaitForCommandQueue(4);

    GFX_RCC_SetColor(GetColor());
    GFX_RCC_SetDestOffset((y * (DWORD)DISP_HOR_RESOLUTION) + x);
    GFX_RCC_SetSize(1, 1);
    GFX_RCC_StartCopy(RCC_SOLID_FILL, RCC_ROP_C, RCC_SRC_ADDR_DISCONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);

    /* Note: No need to wait for  complete execution of the command even for Blocking Mode. The next commands will be in the queue & hence will execute only after the completion of this command. */
}

/*********************************************************************
* Function: GFX_COLOR GetPixel(SHORT x, SHORT y)
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
GFX_COLOR GetPixel(SHORT x, SHORT y)
{
    volatile WORD getcolor = 0;
    
    if(x < 0 || y < 0 || x > GetMaxX() || y > GetMaxY())
    {
        return 1; /* Don't draw but return 1 */
    }

    #if (DISP_ORIENTATION == 90)
    {
        SHORT t = x;
        x = y;
        y = GetMaxX() - t;
    }
    #elif (DISP_ORIENTATION == 180)
    {
        x = GetMaxX() - x;
        y = GetMaxY() - y;
    }
    #elif (DISP_ORIENTATION == 270)
    {
        SHORT t = x;
        x = GetMaxY() - y;
        y = t;
    }
    #endif

    /* Wait till previous commands are fully executed */
    GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    GFX_WaitForGpu();

    #ifdef USE_DOUBLE_BUFFERING
        GFX_SetWorkArea1(_drawbuffer);
    #else
        GFX_SetWorkArea1(GFX_GetDisplayArea());
    #endif    

    GFX_SetWorkArea2((WORD)&getcolor);

    GFX_WaitForCommandQueue(4);
    
    GFX_RCC_SetSrcOffset((y * (DWORD)DISP_HOR_RESOLUTION) + x);
    GFX_RCC_SetDestOffset(0);
    GFX_RCC_SetSize(1, 1);
    GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_DISCONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);

    /* Wait till the command is fully executed */
    GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    GFX_WaitForGpu();
    
    return (GFX_COLOR)getcolor;
}

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
    DWORD srcOffsetAddr;
    SHORT width, height;

#ifdef USE_ALPHABLEND_LITE

    DWORD srcAddr;
    BYTE TempGPUBpp; 

    #define MASK_50_PERCENT 0b1111011111011110
    #define MASK_25_PERCENT 0b1110011110011100

    WORD resLineBuffer[GetMaxX()];      // resulting colors
    WORD rawLineBuffer[GetMaxX()];      // raw data 
    WORD sf1LineBuffer[GetMaxX()];      // shifted line buffer
    WORD sf2LineBuffer[GetMaxX()];      // shifted line buffer
    WORD mask, p, y, shift;

    GFX_COLOR fcolor;

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
        if(left<_clipLeft)
           left = _clipLeft;
        if(right>_clipRight)
           right= _clipRight;
        if(top<_clipTop)
           top = _clipTop;
        if(bottom>_clipBottom)
           bottom = _clipBottom;
    }
    else
    {
            left    = (left < 0)    ? 0: (left > GetMaxX())  ? GetMaxX(): left;
            top     = (top < 0)     ? 0: (top > GetMaxY())   ? GetMaxY(): top;
            right   = (right < 0)   ? 0: (right > GetMaxX()) ? GetMaxX(): right;
            bottom  = (bottom < 0)  ? 0: (bottom > GetMaxY())? GetMaxY(): bottom;
    }

    #if (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

        width   =   right - left + 1;
        height  =   bottom - top + 1;

    #elif (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

        height  =   right - left + 1;
        width   =   bottom - top + 1;

    #endif

    #if (DISP_ORIENTATION == 90)

        bottom  =   DISP_VER_RESOLUTION - left;
        left    =   top;
        right   =   left + width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 180)

        right   =   DISP_HOR_RESOLUTION - left;
        bottom  =   DISP_VER_RESOLUTION - top;
        left    =   right - width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 270)

        right   =   DISP_HOR_RESOLUTION - top;
        top     =   left;
        bottom  =   top + height;
        left    =   right - width;

    #endif

#ifdef USE_ALPHABLEND_LITE

    if (GetAlpha() != 100)
    {
    
        /*
            When alpha blending a bar to the display buffer the operation performed is this:
    
            for 50: newColor = ConvertColor50(fcolor) + ConvertColor50(bcolor); break;
            for 25: newColor = ConvertColor25(fcolor) + ConvertColor75(bcolor); break;
            for 75: newColor = ConvertColor75(fcolor) + ConvertColor25(bcolor); break;
    
            where:
                #define ConvertColor50(color)  (GFX_COLOR)((color & (0b1111011111011110))>>1)
                #define ConvertColor25(color)  (GFX_COLOR)((color & (0b1110011110011100))>>2)
                #define ConvertColor75(color)  (GFX_COLOR)(ConvertColor50(color) + ConvertColor25(color))
            
        */

        TempGPUBpp = _PUBPP;
    
        // Get the current set color (shift depending on the alpha percentage)
        // this calculates the resulting foreground color that will be added to the 
        // resulting background color (operation depends on the alpha percentage)
        fcolor = GetColor();

        switch(GetAlpha())
        {
            case 50: 
                fcolor = ConvertColor50(fcolor); 
                mask = MASK_50_PERCENT;
                shift = 1;
                break;
            case 25: 
                fcolor = ConvertColor25(fcolor); 
                mask = MASK_50_PERCENT;
                shift = 1;
                break;
            case 75: 
                fcolor = ConvertColor75(fcolor); 
                mask = MASK_25_PERCENT;
                shift = 2;
                break;
            default:   
                // any other alpha percentage value is not supported and should never happen
                // so just exit the function.
                return 1;
        }
    
        // just to make sure set the work area bpp to 16 bpp
        _PUBPP = GFX_16_BPP;
    
        // grab a line from the target 
        #ifdef USE_DOUBLE_BUFFERING
            srcAddr = _drawbuffer;
        #else
            srcAddr = _workArea2BaseAddr; 
        #endif
        
        for(y=top; y < bottom; y++)
        {
    
            srcOffsetAddr = (y * (DWORD)DISP_HOR_RESOLUTION) + left;
            
            // get a line from the target memory
            ROPBlock(   srcAddr, (DWORD)(WORD)rawLineBuffer, srcOffsetAddr, 0, 
        			    GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, 
                        RCC_COPY, RCC_ROP_C, 0, width, 1);
    
            if(GetPrevAlphaColor() != BLACK)
            {
                for (p=0; p<width;p++)
                {
                    switch(GetAlpha())
                    {
                        case 50: 
                            resLineBuffer[p] = (rawLineBuffer[p] - ConvertColor50(GetPrevAlphaColor())) + fcolor;
                            break;
                        case 75: 
                            resLineBuffer[p] = (rawLineBuffer[p] - ConvertColor75(GetPrevAlphaColor())) + fcolor;
                            break;
                        case 25: 
                            resLineBuffer[p] = (rawLineBuffer[p] - ConvertColor25(GetPrevAlphaColor())) + fcolor;
                            break;
                        default: 
                            break;
                    }
                }
            }
            else
            {    
                // now the rawLineBuffer has a the raw line data 
                // do an AND with the mask
               ROPBlock(    0, (DWORD)(WORD)rawLineBuffer, 0, 0, 
            			    GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, 
                            RCC_SOLID_FILL, RCC_ROP_8, mask, width, 1);
            
    
                // at this point rawLineBuffer has been masked
                // do the shift once
    
                // set to 1 bpp since we are shifting on the bit level
                _PUBPP = GFX_1_BPP;    
            
                //set the offset of source by the shift value (1 for 50% & 25% and 2 for 75%)
                ROPBlock(   (DWORD)(WORD)rawLineBuffer, (DWORD)(WORD)sf1LineBuffer, shift, 0, 
            			    GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, 
                            RCC_COPY, RCC_ROP_C, 0, (width<<4)-shift, 1);
    
                // set the MSBits and LSBits of the line to zero (first and last entry)
                if (shift == 1)
                {
                    sf1LineBuffer[0] = sf1LineBuffer[0] & (0x7FFF);
                    sf1LineBuffer[width-1] = sf1LineBuffer[width-1] & (0xFFFE);
                }
                if (shift == 2)
                {
                    sf1LineBuffer[0] = sf1LineBuffer[0] & (0x3FFF);
                    sf1LineBuffer[width-1] = sf1LineBuffer[width-1] & (0xFFFC);
                }
        
                // at this point sf1LineBuffer will contain 50% of the background when shift == 1
                // or contain 25% of the background when shift == 2
    
    
                if (GetAlpha() == 25)
                {
    
                    // rawLineBuffer still has the raw data but was masked for 50%
                    // therefore if we mask for 25% it does not matter
                    _PUBPP = GFX_16_BPP;   
    
                    // mask operation
                    ROPBlock(   0, (DWORD)(WORD)rawLineBuffer, 0, 0, 
            			        GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, 
                                RCC_SOLID_FILL, RCC_ROP_8, MASK_25_PERCENT, width, 1);
    
                    _PUBPP = GFX_1_BPP;    
    
                    // shift operation (shift to get 25%)
                    ROPBlock(   (DWORD)(WORD)rawLineBuffer, (DWORD)(WORD)sf2LineBuffer, 2, 0, 
                			    GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, 
                                RCC_COPY, RCC_ROP_C, 0, (width<<4)-2, 1);
    
    
                    // set the MSBs of the first entry in array to zero
                    sf2LineBuffer[0] = sf2LineBuffer[0] & 0x7FFF;
                    sf2LineBuffer[width-1] = sf2LineBuffer[width-1] & (0xFFFE);
                }
        
                for (p=0; p<width; p++)
                {
                    switch(GetAlpha())
                    {
                        case 50 : 
                        case 75 : 
                            resLineBuffer[p] = fcolor + sf1LineBuffer[p]; 
                            break;
                        case 25 : 
                            resLineBuffer[p] = fcolor + sf1LineBuffer[p] + sf2LineBuffer[p]; 
                            break;
                        default :
                            break;    
                    }
                }
    
                _PUBPP = GFX_16_BPP;    
            }   // end of -- if(GetPrevAlphaColor() != BLACK) else 
            
            // copy back the results to the display
            ROPBlock(   (DWORD)(WORD)resLineBuffer, srcAddr, 0, srcOffsetAddr, 
        			    GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, 
                        RCC_COPY, RCC_ROP_C, 0, width, 1);
    
        } // end of -- for(y=top; y < bottom; y++)
    
        _PUBPP = TempGPUBpp;

    }
    else
#endif

    {
        if(GFX_GetFreeCommandSpace() < GFX_COMMAND_QUEUE_LENGTH)
        {
            #ifndef USE_NONBLOCKING_CONFIG
                GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            #else
                return (0);
            #endif
        }
    
        srcOffsetAddr = (top * (DWORD)DISP_HOR_RESOLUTION) + left;
    
        #ifdef USE_DOUBLE_BUFFERING
            GFX_SetWorkArea2(_drawbuffer);
        #else
            GFX_SetWorkArea2(GFX_GetDisplayArea()); 
        #endif
    
        GFX_RCC_SetColor(GetColor());
        GFX_RCC_SetDestOffset(srcOffsetAddr);
        GFX_RCC_SetSize(width, height);
        GFX_RCC_StartCopy(RCC_SOLID_FILL, RCC_ROP_C, RCC_SRC_ADDR_DISCONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
    }

    /* Note: No need to wait for  complete execution of the command even for Blocking Mode. The next commands will be in the queue & hence will execute only after the completion of this command. */
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
    GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    GFX_WaitForGpu();

    #ifdef USE_DOUBLE_BUFFERING
        GFX_SetWorkArea2(_drawbuffer);
    #else
        GFX_SetWorkArea2(GFX_GetDisplayArea()); 
    #endif

    GFX_RCC_SetColor(GetColor());
    GFX_RCC_SetDestOffset(0);
    GFX_RCC_SetSize(DISP_HOR_RESOLUTION, DISP_VER_RESOLUTION);
    GFX_RCC_StartCopy(RCC_SOLID_FILL, RCC_ROP_C, RCC_SRC_ADDR_DISCONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

    GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    GFX_WaitForGpu();
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

    if((_lineType == 0) && ((x1 == x2) || (y1 == y2)))
    {
        if(x1 > x2)
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
        }
        else if(y1 > y2)
        {
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        if(_lineThickness)
        {
            if(x1 == x2)
            {
                if(!Bar(x1 - 1, y1, x2 + 1, y2)) return (0);
            }
            else
            {
                if(!Bar(x1, y1 - 1, x2, y2 + 1)) return (0);
            }
        }
        else
        {
            if(!Bar(x1, y1, x2, y2)) return (0);
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


#ifdef USE_DRV_FONT
/*********************************************************************
* Function: void SetFont(void* font)
*
* PreCondition: none
*
* Input: pointer to the font image
*
* Output: none
*
* Side Effects: none
*
* Overview: defines current font
*
* Note: none
*
********************************************************************/
void SetFont(void *font)
{
    switch(*((SHORT *)font))
    {
        case FLASH:
        case RAM:
            SetFontFlash(font);
            break;

        case EXTERNAL:
            SetFontExternal(font);
            break;

    	case EDS_EPMP:
        	SetFontEds(font);
			break;

        default:
            return;
    }
}


/*********************************************************************
* Function: void SetFontEds(void* pFont)
*
* PreCondition: none
*
* Input: font - Pointer to the font image in memory.
*
* Output: none
*
* Side Effects: none
*
* Overview: Sets the current active font.
*
* Note: none
*
********************************************************************/
void SetFontEds(void *pFont)
{
	WORD  TempGPUBpp;
	
    // make sure there are no GPU commands pending and GPUs are all idle
	GFX_WaitForCommandQueue(16);		
	while(GFX_IsPuGpuBusy() == 1);

    // make GPU operate on 8 bpp so the font table is easier to operate on
    TempGPUBpp = _PUBPP;
    _PUBPP = GFX_8_BPP;

    // grab the font header from memory
    while(!ROPBlock( ((FONT_EXTERNAL*) pFont)->address, (DWORD)(WORD)&(currentFont.fontHeader), 0, 0, 					\
	                   GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, RCC_COPY, RCC_ROP_C, 0,   \
                       sizeof(FONT_HEADER), 1));

    // make sure the data copy terminates
	GFX_WaitForCommandQueue(16);		
	while(GFX_IsPuGpuBusy() == 1);
	
	// reset the bpp back
    _PUBPP = TempGPUBpp;

	// set the font address in the CHRGPU	
    GFX_CHR_SetFont(((FONT_EXTERNAL*) pFont)->address);

    currentFont.pFont = pFont;

}

/*********************************************************************
* Function: SHORT GetTextWidth(XCHAR* textString, void* font)
*
* PreCondition: none
*
* Input: textString - pointer to the text string,
*        font - pointer to the font
*
* Output: text width in pixels
*
* Side Effects: none
*
* Overview: returns text width for the font
*
* Note: none
*
********************************************************************/
SHORT GetTextWidth(XCHAR *textString, void *font)
{
    switch(*((SHORT *)font))
    {
        case RAM:
            return GetTextWidthRam(textString, font);
                	    
        case FLASH:
            return GetTextWidthFlash(textString, font);

        case EXTERNAL:
            return GetTextWidthExternal(textString, font);

		case EDS_EPMP:	
   			return GetTextWidthEds(textString, font);
          
        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetTextHeight(void* font)
*
* PreCondition: none
*
* Input: pointer to the font
*
* Output: character height in pixels
*
* Side Effects: none
*
* Overview: returns characters height for the font
*
* Note: none
*
********************************************************************/
SHORT GetTextHeight(void *pFont)
{
#ifdef USE_FONT_EXTERNAL
    SHORT height;
#endif

    // if the current set font is the same just return with 
    // the already set value in currentFont
    if (pFont == currentFont.pFont)
        return currentFont.fontHeader.height;
    else
    {
        switch(*((SHORT *)pFont))
        {
#ifdef USE_FONT_RAM
            case RAM:
                return ((FONT_HEADER *) ((FONT_RAM *)pFont)->address)->height;
#endif
                    
#ifdef USE_FONT_FLASH
            case FLASH:
                return ((FONT_HEADER *) ((FONT_FLASH *)pFont)->address)->height;
#endif
                    
#ifdef USE_FONT_EXTERNAL
            case EXTERNAL:
                ExternalMemoryCallback(pFont, sizeof(FONT_HEADER) - 2, 2, &height);
                return (height);

            case EDS_EPMP:        
                return (GetTextHeightEds(pFont));

#endif

            default:
                return (0);
        }
    }
}

/*********************************************************************
* Function: WORD OutChar(XCHAR ch)
*
* PreCondition: none
*
* Input: character code
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs a character
*
* Note: none
*
********************************************************************/
WORD OutChar(XCHAR ch)
{
    static OUTCHAR_PARAM OutCharParam;

    // initialize variables
#ifdef USE_FONT_FLASH	
    OutCharParam.pChTable = NULL;
    OutCharParam.pChTableExtended = NULL;
#endif
#ifdef USE_FONT_EXTERNAL	
    OutCharParam.pChImage = NULL;
#endif
    OutCharParam.xAdjust = 0;
    OutCharParam.yAdjust = 0;
    OutCharParam.xWidthAdjust = 0;
    OutCharParam.heightOvershoot = 0;

    // check for error conditions (a return value of 0xFFFF means error)
    if((UXCHAR)ch < (UXCHAR)currentFont.fontHeader.firstChar)
        return (-1);
    if((UXCHAR)ch > (UXCHAR)currentFont.fontHeader.lastChar)
        return (-1);
#ifndef USE_ANTIALIASED_FONTS
    if(currentFont.fontHeader.bpp > 1)
        return (-1); 
#endif

#ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();
#else
    if(IsDeviceBusy() != 0)
        return (0);
#endif

    switch(*((SHORT *)currentFont.pFont))
    {

        case FLASH:
            OutCharGetInfoFlash(ch, &OutCharParam); 
            break;

        case EXTERNAL:
            OutCharGetInfoExternal(ch, &OutCharParam);
            break;

        case EDS_EPMP:
            // fonts with extended glyph is not supported by CHRGPU
            if(currentFont.fontHeader.extendedGlyphEntry)
                return (-1);
			if (OutCharEds(ch) == 0)
				return 0;
			else
			{			
				MoveTo(DrvGetX(), DrvGetY());
				return 1;
			}

        default:
            return 1;
    }    
    
    return (OutCharRender(ch, &OutCharParam));
}

/*********************************************************************
* Function: WORD OutCharEds(XCHAR ch)
*
* PreCondition: none
*
* Input: character code
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs a character
*
* Note: none
*
********************************************************************/
WORD OutCharEds(XCHAR ch)
{
	typedef enum
    {
        OUTCHAR_IDLE,
        OUTCHAR_WAITTILLDONE,
    } DRV_OUTCHAR_STATES;

    static DRV_OUTCHAR_STATES state = OUTCHAR_IDLE;

	// it is assumed that this code exexutes only when there is at least 
	// 4 empty spaces in the command buffer
	
	if (state == OUTCHAR_IDLE)
	{   
        // check if we have spots in the command buffer
        #ifdef USE_NONBLOCKING_CONFIG
            if (GFX_GetFreeCommandSpace() < 3)
                return 0;
    	#else
            // wait until pending commands are executed
	    	GFX_WaitForCommandQueue(16);
		    GFX_WaitForGpu();
	    #endif	

		// set the color if the color is being changed
		if(_chrcolor != GetColor())
		{
			GFX_CHR_SetFgColor(GetColor());
		}
	
		// if using double buffering, work area is managed	
	#ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea1(_drawbuffer);
	#else
		GFX_SetWorkArea1(_workArea1BaseAddr);
	#endif

		// set the position and render the character
		GFX_CHR_SetPrintPos(GetX(), GetY());
		GFX_CHR_PrintChar(ch, CHR_TRANSPARENT);

		// if font orientation is vertical force the x and y position to go to the next line	
		if(_fontOrientation == ORIENT_VER)
			GFX_CHR_PrintChar(0x0a, CHR_TRANSPARENT);
		state = OUTCHAR_WAITTILLDONE;	
	}
	if (state == OUTCHAR_WAITTILLDONE)
	{
		// check if the character was rendered
    #ifdef USE_NONBLOCKING_CONFIG
		if (GFX_GetFreeCommandSpace() != 16) 
			return 0;
		if (GFX_IsChrGpuBusy())
			return 0;
	#else
		GFX_WaitForCommandQueue(16);
		GFX_WaitForGpu();
	#endif	
		
		state = OUTCHAR_IDLE;
	}

	MoveTo(DrvGetX(), DrvGetY());
	return (1);
}

/*********************************************************************
* Function:  SHORT GetTextWidthEds(XCHAR *textString, void *pFont)
*
* Overview: This function returns the width of the given string for a given font in EDS.
*
* PreCondition: A valid font table must exist in the location specified 
*               by the pointer pFont.
*
* Input: textString - Pointer to the string.
*        pFont - Pointer to the font.
*
* Output: Returns the string width in pixels.
*
* Side Effects: none
*
********************************************************************/
SHORT GetTextWidthEds(XCHAR *pString, void *pFont)
{
    FONT_HEADER     header;
    WORD            fontFirstChar;
    WORD            fontLastChar;
    WORD 		    textWidth, TempGPUBpp;
    BYTE            temp;
    static XCHAR    ch, *pStr;	
    DWORD 		    offset;
	

	GFX_WaitForCommandQueue(16);		
	while(GFX_IsPuGpuBusy() == 1);
	
    // make GPU operate on 8 bpp so the font table is easier to operate on
    TempGPUBpp = _PUBPP;
    _PUBPP = GFX_8_BPP;
	
    while(!ROPBlock( ((FONT_EXTERNAL*) pFont)->address, (DWORD)(WORD)&header, 0, 0,        \
                       GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, RCC_COPY, RCC_ROP_C, 0,   \
                       sizeof(FONT_HEADER), 1));
				
	GFX_WaitForCommandQueue(16);		
	while(GFX_IsPuGpuBusy() == 1);

    fontFirstChar = header.firstChar;
    fontLastChar = header.lastChar;
    textWidth = 0;
    pStr = (XCHAR*) pString;
	ch = *pStr;
	
    while((XCHAR)15 < (XCHAR)(ch = *pStr++))
    {
        if((XCHAR)ch < (XCHAR)fontFirstChar)
            continue;
        if((XCHAR)ch > (XCHAR)fontLastChar)
            continue;

		// grab the width of the character
		offset = sizeof(FONT_HEADER) + (((XCHAR)ch - (XCHAR)fontFirstChar)*sizeof(GLYPH_ENTRY));
        while(!ROPBlock( ((FONT_EXTERNAL*) pFont)->address, (DWORD)(WORD)&temp, offset, 0,     \
                           GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, RCC_COPY, RCC_ROP_C, 0,   \
                           1, 1));

        GFX_WaitForCommandQueue(16);		
        while(GFX_IsPuGpuBusy() == 1);

        textWidth += temp;
    }

    _PUBPP = TempGPUBpp;
    return (textWidth);
}

/*********************************************************************
* Function:  SHORT GetTextHeightEds(void *pFont)
*
* Overview: This function returns the height of the given font in EDS.
*
* PreCondition: A valid font table must exist in the location specified 
*               by the pointer pFont.
*
* Input: pFont - Pointer to the font.
*
* Output: Returns the height in pixels.
*
* Side Effects: none
*
********************************************************************/
SHORT GetTextHeightEds(void *pFont)
{
	FONT_HEADER header;
    WORD 		TempGPUBpp;

	
	GFX_WaitForCommandQueue(16);		
	while(GFX_IsPuGpuBusy() == 1);

    // make GPU operate on 8 bpp so the font table is easier to operate on
    TempGPUBpp = _PUBPP;
    _PUBPP = GFX_8_BPP;
	
    while(!ROPBlock(((FONT_EXTERNAL *)pFont)->address, (DWORD)(WORD)&header, 0, 0,      		\
                    GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, RCC_COPY, RCC_ROP_C, 0,  \
                    sizeof(FONT_HEADER), 1));

	GFX_WaitForCommandQueue(16);		
	while(GFX_IsPuGpuBusy() == 1);

    _PUBPP = TempGPUBpp;
    return (header.height);
}

#endif //#ifdef USE_DRV_FONT

/*********************************************************************
* Function: SHORT GetImageWidth(void* image)
*
* PreCondition: none
*
* Input: image - image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: returns image width
*
* Note: none
*
********************************************************************/
SHORT GetImageWidth(void *image)
{

#ifdef USE_COMP_IPU
    
    if((*((SHORT *)image) & GFX_COMP_MASK) == COMP_IPU)
    {
	    return ((GFX_IMAGE_HEADER*)image)->width;
	}
	
#endif
	
#ifdef USE_BITMAP_EXTERNAL
    SHORT   width;
#endif

    switch(*((SHORT *)image))
    {
#ifdef USE_BITMAP_FLASH

        case FLASH:
            return (*((FLASH_WORD *) ((IMAGE_FLASH *)image)->address + 2));

    #if defined(USE_COMP_RLE)
        case FLASH | COMP_RLE | IMAGE_MBITMAP:
            return (((GFX_IMAGE_HEADER *)image)->width);
    #endif

#endif

#ifdef USE_BITMAP_EXTERNAL

		case EDS_EPMP:
        	return DrvGetImageWidth(image);
        	
        case EXTERNAL:
            ExternalMemoryCallback(image, 4, 2, &width);
            return (width);

    #if defined(USE_COMP_RLE)
        case EXTERNAL | COMP_RLE | IMAGE_MBITMAP:
            return (((GFX_IMAGE_HEADER *)image)->width);
    #endif


#endif

        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetImageHeight(void* image)
*
* PreCondition: none
*
* Input: image - image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: returns image height
*
* Note: none
*
********************************************************************/
SHORT GetImageHeight(void *image)
{

#ifdef USE_COMP_IPU
    
    if((*((SHORT *)image) & GFX_COMP_MASK) == COMP_IPU)
    {
	    return ((GFX_IMAGE_HEADER*)image)->height;
	}
	
#endif
	
#ifdef USE_BITMAP_EXTERNAL

    SHORT   height;
#endif

    switch(*((SHORT *)image))
    {
#ifdef USE_BITMAP_FLASH

        case FLASH:
            return (*((FLASH_WORD *) ((IMAGE_FLASH *)image)->address + 1));

    #if defined(USE_COMP_RLE)
        case FLASH | COMP_RLE | IMAGE_MBITMAP:
            return (((GFX_IMAGE_HEADER *)image)->height);
    #endif


#endif

#ifdef USE_BITMAP_EXTERNAL
            
		case EDS_EPMP:
        	return DrvGetImageHeight(image);

        case EXTERNAL:
            ExternalMemoryCallback(image, 2, 2, &height);
            return (height);

    #if defined(USE_COMP_RLE)
        case EXTERNAL | COMP_RLE | IMAGE_MBITMAP:
            return (((GFX_IMAGE_HEADER *)image)->height);
    #endif


#endif


        default:
            return (0);
    }
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
*        pPartialImageData - pointer to partial image parameters
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
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    BYTE                temp = 0;
    WORD                sizeX, sizeY;
    WORD                x, y, yc;
    BYTE                stretchY;
    WORD                palette[2], color;
    BYTE                mask;
    WORD                outputSize;
	WORD				xLimit;
	
    WORD            	lineBuffer[(GetMaxX() + (DWORD) 1)];
    WORD            	*pData; 
#ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; // To supress warning
#if (DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90)   
#if (COLOR_DEPTH == 4) 
    BYTE nibblePos = 0;
#endif
#if (COLOR_DEPTH == 1)
    BYTE bitPos = 0;
#endif
#endif
#endif

    CHAR                testStretch = (stretch == IMAGE_NORMAL) ? 0x00 : 0x01;
    WORD                addressOffset = 0, adjOffset;
    BYTE                maskOffset = 0;     // adjusted offset for partial image rendering


    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    palette[0] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    palette[1] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    if (sizeX & 0x07)
        adjOffset = 1; 
    else
        adjOffset = 0;

    // compute for addressOffset this is the offset needed to jump to the 
    // next line 
    addressOffset = ((sizeX >> 3) + adjOffset); 

    if(pPartialImageData->width != 0)
    {

        // adjust the flashAddress to the starting pixel location
        // adjust one address if the data is not byte aligned
        flashAddress += (pPartialImageData->yoffset*((sizeX >> 3) + adjOffset));

        // adjust flashAddress for x offset (if xoffset is zero address stays the same)
        flashAddress += ((pPartialImageData->xoffset) >> 3);


        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;

        // check the bit position of the starting pixel
        maskOffset = pPartialImageData->xoffset & 0x07;
    }
    
    if (stretch == IMAGE_X2) {
        outputSize = sizeX << 1;
    }   	
    else //if (stretch == IMAGE_NORMAL)
    {
        outputSize = sizeX;
    }

	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > GetMaxX()+1) || (left + outputSize > GetMaxX()+1))
		outputSize = GetMaxX() + 1 - left; 		
		
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

	GFX_SetWorkArea1((WORD)lineBuffer);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif
    
	GFX_RCC_SetSrcOffset(0);

    yc = top;
    // store current line data address 
    tempFlashAddress = flashAddress;

	// some pre-calculations
	if (outputSize <= GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

	for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {

        // get flash address location of current line being processed
        flashAddress = tempFlashAddress;
        if (maskOffset)
		{
            // grab the first set of data then set up the mask to the starting pixel
			temp = *flashAddress++;
            mask = 0x01 << maskOffset;
        }
        else
            mask = 0;

		// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
			pData = lineBuffer;
            #ifdef USE_PALETTE
	            if(IsPaletteEnabled())
    	        {
        	        pByteData = (BYTE*)lineBuffer;
            	}
            #endif
		#elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData = &lineBuffer[outputSize-1];
           	
            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pByteData = (BYTE *)lineBuffer;

                #if (COLOR_DEPTH == 8)
                    pByteData += outputSize;
                    pByteData--;
                #endif

                #if (COLOR_DEPTH == 4)

                    pByteData += outputSize / 2;
                    nibblePos = (outputSize & 0x01);
                    if(nibblePos == 0)
                    {
                        pByteData--;
                    }
                #endif

                #if (COLOR_DEPTH == 1)

                    pByteData += (outputSize / 8) ;
                    bitPos = (outputSize & 0x07);
                    if(bitPos != 0)
                        bitPos -= 1;  // why -1, since bit position starts at 0 (or first bit is at 0 position)    
                    else
                    {
                        bitPos = 7;
                        pByteData--;
                    }
                #endif
                
            }
			#endif
		#endif


        // check if previous RCCGPU copies are done so we do
        // not overwrite the buffers still being copied.
		GFX_WaitForCommandQueue(16);
    	GFX_WaitForGpu();

		// process the pixels of the current line
        for(x = 0; x < xLimit; x++)
        {            
			// Read 8 pixels from flash
            if(mask == 0)
			{
				temp = *flashAddress++;
                mask = 0x01;
            }

            // Set color
            if(mask & temp)
            {
				// Set color
	            #ifdef USE_PALETTE
					if (IsPaletteEnabled())
					{
						color = 1;
					}	
	                else 
	                {
	                  	color = palette[1];
	                }  	
	            #else
                   	color = palette[1];
	            #endif
            }
            else
            {
	            // Set color
	            #ifdef USE_PALETTE
                  	if(IsPaletteEnabled())
                  	{
	                   	color = 0;
	                } 	
	               	else 
	               	{
	                   	color = palette[0];
	                }   
	            #else
                  	color = palette[0];
	            #endif
            }

   	        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
                        *pByteData++ = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                        if(x & 0x01)
                        {
                            *pByteData++ |= color << 4;
                        }else
                        {
                            *pByteData = color & 0x0F;
                        }
                        #endif

                        #if (COLOR_DEPTH == 1)
                        {
                            BYTE pos = (BYTE)x & 0x07;
                            
                            if(pos == 0)
                                *pByteData = color;
                            else
                                *pByteData |= color << pos;

                            if(pos == 7)
                                *pByteData++;
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData++ = color;
    	          	}    
                #else
                    *pData++ = color;
                #endif
	        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
    	          	    *pByteData-- = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                        if (nibblePos == 1)
                        {
                            *pByteData = ((*pByteData & 0xF0) | (color & 0x0F));
                            pByteData--;
                            nibblePos = 0;
                        }else
                        {
                            *pByteData = color << 4;
                            nibblePos = 1;
                        }
                        #endif
                        
                        #if (COLOR_DEPTH == 1)
                        {
                            *pByteData &= ~(1 << bitPos);
                            *pByteData |= (color << bitPos);

                            if (bitPos == 0)
                            {
                                bitPos = 7;
                                pByteData--;
                            }
                            else
                            {
                                bitPos--;
                            }
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData-- = color;
    	          	}    
                #else
                    *pData-- = color;
                #endif
           	#endif			

           	// Shift to the next pixel (note that if image is stretched, shifting is delayed)
       	    if (((x+1) & testStretch) == 0) 
              	    mask <<= 1;		

        } // for(x = 0; x < xLimit; x++)

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize(x, 1);
		#else	
			GFX_RCC_SetSize(1, x);
		#endif
			
		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
	        #if (DISP_ORIENTATION == 0) 
				GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
	        #elif (DISP_ORIENTATION == 90)
	    		GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
	        #elif (DISP_ORIENTATION == 180)
	    		GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
	        #elif (DISP_ORIENTATION == 270)
	    		GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 

			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

	        yc++;

    	} // for(stretchY = 0; stretchY < stretch; stretchY++)

        tempFlashAddress += addressOffset;

    } // for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
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
        #if (COLOR_DEPTH >= 4)
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y, yc;

    BYTE                temp = 0;
    BYTE       			stretchY;
    WORD                palette[16], color;
    WORD            	*pData; 
    WORD                outputSize;
	WORD				xLimit;
    WORD                counter;

    WORD            	lineBuffer[(GetMaxX() + (DWORD) 1)];

#ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; 
#if (COLOR_DEPTH == 4) && ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
    BYTE                nibblePos = 0;
#endif
#endif

    CHAR                testStretch = (stretch == IMAGE_NORMAL) ? 0x01 : 0x03;
    WORD                nibbleOffset = 0, addressOffset = 0, adjOffset;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read palette
    for(counter = 0; counter < 16; counter++)
    {
        palette[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    if (sizeX & 0x01)
        adjOffset = 1; 
    else
        adjOffset = 0;

    // compute for addressOffset this is the offset needed to jump to the 
    // next line 
    addressOffset = ((sizeX >> 1) + adjOffset); 

    if(pPartialImageData->width != 0)
    {
        // check the bit position of the starting pixel
        nibbleOffset = pPartialImageData->xoffset & 0x01;

        // adjust the flashAddress to the starting pixel location
        // adjust one address if the data is not byte aligned
        flashAddress += (pPartialImageData->yoffset*((sizeX >> 1) + adjOffset));

        // adjust flashAddress for x offset (if xoffset is zero address stays the same)
        flashAddress += ((pPartialImageData->xoffset - nibbleOffset) >> 1);

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width + nibbleOffset;

    }

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}
	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > (GetMaxX()+1)) || ((left + outputSize) > (GetMaxX()+1)))
		outputSize = GetMaxX() + 1 - left; 		

	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

   	GFX_SetWorkArea1((WORD)lineBuffer);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif

    #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
    	GFX_RCC_SetSrcOffset(nibbleOffset*stretch);
    #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
    	GFX_RCC_SetSrcOffset(0);
    #endif    

    yc = top;

	// some pre-calculations
	if (outputSize <= (GetMaxX() - left))
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

    // store current line data address 
    tempFlashAddress = flashAddress;

	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

    for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {

		// get flash address location of current line being processed
        flashAddress = tempFlashAddress;

		// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData = lineBuffer;
            #ifdef USE_PALETTE
            	if(IsPaletteEnabled())
            	{
            	    pByteData = (BYTE*)lineBuffer;
            	}    
            #endif
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData = &lineBuffer[outputSize-1];

            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pByteData = (BYTE*)lineBuffer;

                #if (COLOR_DEPTH == 8)
                    pByteData += outputSize;
                    pByteData--;
                #endif

                #if (COLOR_DEPTH == 4)

                    pByteData += outputSize / 2;
                    nibblePos = (outputSize & 0x01);	
                    if(nibblePos == 0)
                    {
                        pByteData--;
                    }
                #endif
            }
			#endif

        #endif

        // check if previous RCCGPU copies are done so we do
        // not overwrite the buffers still being copied.
		GFX_WaitForCommandQueue(16);
    	GFX_WaitForGpu();

		// process the pixels of the current line
        for(x = 0; x < xLimit; x++)
        {            
            // take one pixel data when needed 
            if ((x & testStretch) == 0)
            	temp = *flashAddress++; 

            #ifdef USE_PALETTE
               	if(IsPaletteEnabled())
	                color = temp >> (((x>>(stretch-1))&0x0001)*4) & 0x000F;
               	else 
	                color = palette[(temp >> (((x>>(stretch-1))&0x0001)*4)) & 0x000F];
            #else
                color = palette[(temp >> (((x>>(stretch-1))&0x0001)*4)) & 0x000F];
            #endif	            

            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
                            *pByteData++ = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                            if(x & 0x01)
                            {
                                *pByteData++ |= color << 4;
                            } else
                            {
                                *pByteData = color & 0x0F;
                            }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData++ = color;
    	          	}    
                #else
                    *pData++ = color;
                #endif
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
    	          	    *pByteData-- = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
						if (nibblePos == 1)
                        {
                            *pByteData = ((*pByteData & 0xF0) | (color & 0x0F));
                            pByteData--;
                            nibblePos = 0;
                        }else
                        {
                            *pByteData = color << 4;
                            nibblePos = 1;
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData-- = color;
    	          	}    
                #else
                    *pData-- = color;
                #endif
       	    #endif			           	    
       	    		
        } // for(x = 0; x < xLimit; x++)

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize((x - (nibbleOffset*stretch)), 1);
		#else	
			GFX_RCC_SetSize(1, (x - (nibbleOffset*stretch)));
		#endif
	
		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize - (nibbleOffset*stretch))) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left + outputSize - (nibbleOffset*stretch)));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 

			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

			// update the vertical position counter
            yc++;
        }  // for(stretchY = 0; stretchY < stretch; stretchY++)

   	    // adjust for the next line data address 
       	tempFlashAddress += addressOffset;
					
    } // for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}
        #endif //#if (COLOR_DEPTH >= 4)

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
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
        #if (COLOR_DEPTH >= 8)

void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y, yc;
    BYTE                temp = 0;
    BYTE                stretchY;
    WORD                palette[256];
    WORD                counter;
    WORD                outputSize;
	WORD				xLimit;

    WORD            	lineBuffer[(GetMaxX() + (DWORD) 1)];
    WORD            	*pData; 

    #ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; // To supress warning 
    #endif

    WORD                addressOffset;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read palette
    for(counter = 0; counter < 256; counter++)
    {
        palette[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    // compute for addressOffset this is the offset needed to jump to the 
    // next line 
    addressOffset = sizeX; 

    if(pPartialImageData->width != 0)
    {
        // adjust the flashAddress to the starting pixel location
        // adjust one address if the data is not byte aligned
        flashAddress += (pPartialImageData->yoffset*sizeX);

        // adjust flashAddress for x offset (if xoffset is zero address stays the same)
        flashAddress += (pPartialImageData->xoffset);

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;

    }

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}

	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > GetMaxX()+1) || (left + outputSize > GetMaxX()+1))
		outputSize = GetMaxX() + 1 - left; 

	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();
		 	
	GFX_SetWorkArea1((WORD)lineBuffer);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif
    
	GFX_RCC_SetSrcOffset(0);
    
    yc = top;

    // store current line data address 
    tempFlashAddress = flashAddress;

	// some pre-calculations
	if (outputSize <= GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

    for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
	{
        // get flash address location of current line being processed
        flashAddress = tempFlashAddress;

			// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))

	        pData = lineBuffer;

            #ifdef USE_PALETTE
                if(IsPaletteEnabled())
                {
                    pByteData = (BYTE*)lineBuffer;
                }    
            #endif

        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   

	        pData = &lineBuffer[outputSize - 1];

            #ifdef USE_PALETTE
                if(IsPaletteEnabled())
                {
                    pByteData = (BYTE*)lineBuffer;
                    pByteData += (outputSize - 1);
                }
			#endif

	    #endif

        // check if previous RCCGPU copies are done so we do
        // not overwrite the buffers still being copied.
		GFX_WaitForCommandQueue(16);
		GFX_WaitForGpu();

		// process the pixels of the current line
		for(x = 0; x < xLimit; x++)
        {

  		    // get pixel data
            temp = *flashAddress++; 

            if (stretch == IMAGE_X2)
            {
                #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                    #ifdef USE_PALETTE
        	          	if(IsPaletteEnabled())
        	          	{
        	          	    *pByteData++ = (BYTE)temp;
        	          	}
        	          	else
        	          	{
                            *pData++ = palette[temp];
        	          	}    
                    #else
    	                    *pData++ = palette[temp];
                    #endif
                #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                   #ifdef USE_PALETTE
        	          	if(IsPaletteEnabled())
        	          	{
        	          	    *pByteData-- = (BYTE)temp;
        	          	}
        	          	else
        	          	{
                            *pData-- = palette[temp];
        	          	}    
                    #else
    	                    *pData-- = palette[temp];
                    #endif
           	    #endif			
                x++;
            }

            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
    	          	    *pByteData++ = (BYTE)temp;
    	          	}
    	          	else
    	          	{
                        *pData++ = palette[temp];
    	          	}    
                #else
	                    *pData++ = palette[temp];
                #endif
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
               #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
    	          	    *pByteData-- = (BYTE)temp;
    	          	}
    	          	else
    	          	{
                        *pData-- = palette[temp];
    	          	}    
                #else
	                    *pData-- = palette[temp];
                #endif
       	    #endif			
        } // for(x = 0; x < xLimit; x++)

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize(x, 1);
        #else	
		    GFX_RCC_SetSize(1, x);
        #endif

		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 
			
			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

            yc++;
        }  // for(stretchY = 0; stretchY < stretch; stretchY++)

   	    // adjust for the next line data address 
        tempFlashAddress += (addressOffset);

    } // for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}

        #endif //#if (COLOR_DEPTH >= 8)

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
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
 		#if (COLOR_DEPTH == 16)
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_WORD *flashAddress;
    register FLASH_WORD *tempFlashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                yc, temp = 0;
    BYTE       			stretchY;
	WORD 				outputSize;
	WORD				xLimit;


    WORD            	lineBuffer[(GetMaxX() + (DWORD) 1)];
    WORD            	*pData; 

    WORD                addressOffset;

	// Move pointer to size information
    flashAddress = (FLASH_WORD *)image + 1;

    // Read image size
    sizeY = *flashAddress;
    flashAddress++;
    sizeX = *flashAddress;
    flashAddress++;

    // compute for addressOffset this is the offset needed to jump to the 
    // next line 
    addressOffset = sizeX; 

    if(pPartialImageData->width != 0)
    {
        // adjust the flashAddress to the starting pixel location
        // adjust one address if the data is not byte aligned
        flashAddress += (pPartialImageData->yoffset*sizeX);

        // adjust flashAddress for x offset (if xoffset is zero address stays the same)
        flashAddress += (pPartialImageData->xoffset);

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;

    }

	if (stretch == IMAGE_X2) 
	    outputSize = sizeX << 1;
	else 
	    outputSize = sizeX;

	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > GetMaxX()+1) || (left + outputSize > GetMaxX()+1))
		outputSize = GetMaxX() + 1 - left; 

	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();
			    
   	GFX_SetWorkArea1((WORD)lineBuffer);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif
	GFX_RCC_SetSrcOffset(0);

	// some pre-calculations
	if (outputSize <= GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() + 1 - left;

    yc = top; 

    // store current line data address 
    tempFlashAddress = flashAddress;

	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

    for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {
        // get flash address location of current line being processed
        flashAddress = tempFlashAddress;

		// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData = lineBuffer;
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData = &lineBuffer[outputSize-1];
        #endif

        // check if previous RCCGPU copies are done so we do
        // not overwrite the buffers still being copied.
		GFX_WaitForCommandQueue(16);
		GFX_WaitForGpu();

		// process the pixels of the current line
        for(x = 0; x < xLimit; x++)
		{
    		// get pixel data
           	temp = *flashAddress++;

            if (stretch == IMAGE_X2)
            {
            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                *pData++ = temp;
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                *pData-- = temp;
       	    #endif			           	    
       	        x++;
            }
            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                *pData++ = temp;
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                *pData-- = temp;
       	    #endif			           	    
		} // for(x = 0; x < xLimit; x++)

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize(x, 1);
		#else	
			GFX_RCC_SetSize(1, x);
		#endif

		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 
			
			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

            yc++;
        }  // for(stretchY = 0; stretchY < stretch; stretchY++)

   	    // adjust for the next line data address 
        tempFlashAddress += (addressOffset);
    } // for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}	
	#endif // #if (COLOR_DEPTH == 16)
    #endif //USE_BITMAP_FLASH
	
    #ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
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
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    BYTE            lineBuffer[((GetMaxX() + (DWORD) 1) / 8) + 1];
    WORD            palette[2], color;
    BYTE            *pData; 
    WORD 			outputSize;
    WORD			*pBufAddr, *pData2;
    WORD            lineBuffer2[(GetMaxX() + (DWORD) 1)];
	WORD			xLimit;

    SHORT           byteWidth;
    BYTE            mask, temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            yc;
    BYTE            stretchY;
#ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; 
#if (DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90)   
#if (COLOR_DEPTH == 4) 
    BYTE nibblePos = 0;
#endif
#if (COLOR_DEPTH == 1)
    BYTE bitPos = 0;
#endif
#endif
#endif

    CHAR            testStretch = (stretch == IMAGE_NORMAL) ? 0x00 : 0x01;
    BYTE            maskOffset = 0;     // adjusted offset for partial image rendering


    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);
    // Get palette (2 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 2 * sizeof(WORD), palette);
    // Set offset to the image data
	memOffset = sizeof(BITMAP_HEADER) + 2 * sizeof(WORD);

	// Get the image line width in bytes
    byteWidth = (bmp.width) >> 3;
    if(bmp.width & 0x07)
        byteWidth++;	
	
    if(pPartialImageData->width != 0)
    {
        // adjust to the location of the starting data for partial image rendering 
        memOffset += pPartialImageData->yoffset*byteWidth;
        memOffset += pPartialImageData->xoffset>>3;

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;

        // check the bit position of the starting pixel
        maskOffset = pPartialImageData->xoffset & 0x07;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}		
	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > GetMaxX()+1) || (left + outputSize > GetMaxX()+1))
		outputSize = GetMaxX() + 1 - left; 		

	pBufAddr = lineBuffer2;

	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

   	GFX_SetWorkArea1((WORD)pBufAddr);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif
	GFX_RCC_SetSrcOffset(0);

	// some pre-calculations
	if (outputSize <= GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

    yc = top;

    #ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	   

    for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {

		// Get line
	    ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
    	memOffset += byteWidth;

	  	pData = lineBuffer;

        if (maskOffset)
		{
            // grab the first set of data then set up the mask to the starting pixel
			temp = *pData++;
            mask = 0x01 << maskOffset;
        }
        else
            mask = 0;
    	
		// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData2 = lineBuffer2;
            #ifdef USE_PALETTE
	            if(IsPaletteEnabled())
    	        {
        	        pByteData = (BYTE*)lineBuffer2;
            	}
            #endif
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData2 = &lineBuffer2[outputSize-1];
           	
            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pByteData = (BYTE *)lineBuffer2;

                #if (COLOR_DEPTH == 8)
                    pByteData += outputSize;
                    pByteData--;
                #endif

                #if (COLOR_DEPTH == 4)

                    pByteData += outputSize / 2;
					nibblePos = (outputSize & 0x01);
                    if(nibblePos == 0)
                    {
                        pByteData--;
                    }
                #endif

                #if (COLOR_DEPTH == 1)

                    pByteData += (outputSize / 8) ;
                    bitPos = (outputSize & 0x07);
                    if(bitPos != 0)
                        bitPos -= 1;  // why -1, since bit position starts at 0 (or first bit is at 0 position)    
                    else
                    {
                        bitPos = 7;
                        pByteData--;
                    }				
                #endif
                
            }
			#endif
        #endif
            
		// process the pixels of the current line
		for(x = 0; x < xLimit; x++)
        {

			if (mask == 0) 
			{
				temp = *pData++;
				mask = 0x01;
			}	
			if (mask & temp)
			{
	            #ifdef USE_PALETTE
               	if(IsPaletteEnabled())
	            	color = 1;
               	else 
	            	color = palette[1];
	            #else	
	            	color = palette[1];
	            #endif
			} 
			else
			{
	            #ifdef USE_PALETTE
               	if(IsPaletteEnabled())
	            	color = 0;
               	else 
	            	color = palette[0];
	            #else	
	            	color = palette[0];
	            #endif
			}		

            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
                        *pByteData++ = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                        if(x & 0x01)
                        {
                            *pByteData++ |= color << 4;
                        }else
                        {
                            *pByteData = color & 0x0F;
                        }
                        #endif

                        #if (COLOR_DEPTH == 1)
                        {
                            BYTE pos = (BYTE)x & 0x07;
                            
                            if(pos == 0)
                                *pByteData = color;
                            else
                                *pByteData |= color << pos;

                            if(pos == 7)
                                *pByteData++;
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData2++ = color;
    	          	}    
                #else
                    *pData2++ = color;
                #endif
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
    	          	    *pByteData-- = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                        if (nibblePos == 1)
                        {
                            *pByteData = ((*pByteData & 0xF0) | (color & 0x0F));
                            pByteData--;
                            nibblePos = 0;
                        }else
                        {
                            *pByteData = color << 4;
                            nibblePos = 1;
                        }
                        #endif
                        
                        #if (COLOR_DEPTH == 1)
                        {
                            *pByteData &= ~(1 << bitPos);
                            *pByteData |= (color << bitPos);

                            if (bitPos == 0)
                            {
                                bitPos = 7;
                                pByteData--;
                            }
                            else
                            {
                                bitPos--;
                            }
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData2-- = color;
    	          	}    
                #else
                    *pData2-- = color;
                #endif
       	    #endif			           	    
       	    
           	// Shift to the next pixel (note that if image is stretched, shifting is delayed)
           	if (((x+1) & testStretch) == 0) 
              	mask <<= 1;		       	    
        } // for(x = 0; x < xLimit; x++)

		GFX_WaitForCommandQueue(16);
    	GFX_WaitForGpu();

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize(x, 1);
		#else	
			GFX_RCC_SetSize(1, x);
		#endif

		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 

			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

			// update the vertical position counter
            yc++;
        }  // for(stretchY = 0; stretchY < stretch; stretchY++)

    }  // for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)
    
	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();
    
}


/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
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
        #if (COLOR_DEPTH >= 4)
void PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD      memOffset;
    BITMAP_HEADER       bmp;
    BYTE                readLineBuffer[((GetMaxX() + (DWORD) 1) / 2) + 1];
    WORD                sizeX, sizeY;
    WORD                x, y, yc;

    BYTE                temp = 0;
    BYTE       			stretchY;
    WORD                palette[16], color;
    BYTE                *pReadData; 
    WORD            	*pData; 
    WORD                outputSize;
	WORD				xLimit;

    WORD            	lineBuffer[(GetMaxX() + (DWORD) 1)];

    SHORT               byteWidth;

#ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; 
#if (COLOR_DEPTH == 4) && ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
    BYTE                nibblePos = 0;
#endif
#endif

    CHAR                testStretch = (stretch == IMAGE_NORMAL) ? 0x01 : 0x03;
    WORD                nibbleOffset = 0; 
	
    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);
    // Get palette (16 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(WORD), palette);
    // Set offset to the image data
	memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);

	// Get the image line width in bytes
    byteWidth = (bmp.width) >> 1;
    if(bmp.width & 0x01)
        byteWidth++;	
	
    if(pPartialImageData->width != 0)
    {
        // check the bit position of the starting pixel
        nibbleOffset = pPartialImageData->xoffset & 0x01;

        // adjust to the location of the starting data for partial image rendering 
        memOffset += pPartialImageData->yoffset * byteWidth;
        memOffset += ((pPartialImageData->xoffset - nibbleOffset) >> 1);

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width + nibbleOffset;

    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}
	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > (GetMaxX()+1)) || ((left + outputSize) > (GetMaxX()+1)))
		outputSize = GetMaxX() + 1 - left; 		

	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

   	GFX_SetWorkArea1((WORD)lineBuffer);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif

    #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
    	GFX_RCC_SetSrcOffset(nibbleOffset*stretch);
    #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
    	GFX_RCC_SetSrcOffset(0);
    #endif    

    yc = top;

	// some pre-calculations
	if (outputSize <= (GetMaxX() - left))
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

    for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {

		// Get line
	    ExternalMemoryCallback(image, memOffset, byteWidth, readLineBuffer);
    	memOffset += byteWidth;
    	
	  	pReadData = readLineBuffer;

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData = lineBuffer;
            #ifdef USE_PALETTE
            	if(IsPaletteEnabled())
            	{
            	    pByteData = (BYTE*)lineBuffer;
            	}    
            #endif
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData = &lineBuffer[outputSize-1];

            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pByteData = (BYTE*)lineBuffer;

                #if (COLOR_DEPTH == 8)
                    pByteData += outputSize;
                    pByteData--;
                #endif

                #if (COLOR_DEPTH == 4)

                    pByteData += outputSize / 2;
                    nibblePos = (outputSize & 0x01);	
                    if(nibblePos == 0)
                    {
                        pByteData--;
                    }
                #endif
            }
			#endif

        #endif

        // check if previous RCCGPU copies are done so we do
        // not overwrite the buffers still being copied.
		GFX_WaitForCommandQueue(16);
    	GFX_WaitForGpu();

		// process the pixels of the current line
        for(x = 0; x < xLimit; x++)
        {            
            // take one pixel data when needed 
            if ((x & testStretch) == 0)
            	temp = *pReadData++; 

            #ifdef USE_PALETTE
               	if(IsPaletteEnabled())
	                color = temp >> (((x>>(stretch-1))&0x0001)*4) & 0x000F;
               	else 
	                color = palette[(temp >> (((x>>(stretch-1))&0x0001)*4)) & 0x000F];
            #else
                color = palette[(temp >> (((x>>(stretch-1))&0x0001)*4)) & 0x000F];
            #endif	            

            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
                            *pByteData++ = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                            if(x & 0x01)
                            {
                                *pByteData++ |= color << 4;
                            } else
                            {
                                *pByteData = color & 0x0F;
                            }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData++ = color;
    	          	}    
                #else
                    *pData++ = color;
                #endif
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
    	          	    *pByteData-- = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
						if (nibblePos == 1)
                        {
                            *pByteData = ((*pByteData & 0xF0) | (color & 0x0F));
                            pByteData--;
                            nibblePos = 0;
                        }else
                        {
                            *pByteData = color << 4;
                            nibblePos = 1;
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData-- = color;
    	          	}    
                #else
                    *pData-- = color;
                #endif
       	    #endif			           	    
       	    		
        } // for(x = 0; x < xLimit; x++)

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize((x - (nibbleOffset*stretch)), 1);
		#else	
			GFX_RCC_SetSize(1, (x - (nibbleOffset*stretch)));
		#endif
	
		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize - (nibbleOffset*stretch))) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left + outputSize - (nibbleOffset*stretch)));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 

			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

			// update the vertical position counter
            yc++;
        }  // for(stretchY = 0; stretchY < stretch; stretchY++)
    }  // for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}
        #endif //#if (COLOR_DEPTH >= 4)

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
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
        #if (COLOR_DEPTH >= 8)

void PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    BYTE            lineBuffer[(GetMaxX() + (DWORD) 1)];
    WORD            lineBuffer2[(GetMaxX() + (DWORD) 1)];

    WORD            palette[256];    
    BYTE            *pData; 
    WORD			*pData2;
    WORD 			outputSize;
	WORD			xLimit;

    BYTE            temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            yc;
    BYTE            stretchY;

    #ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; 
    #endif

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);
    // Get palette (256 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 256 * sizeof(WORD), palette);
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

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}

	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > GetMaxX()+1) || (left + outputSize - 1 > GetMaxX()))
		outputSize = GetMaxX() + 1 - left; 		
		
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

	GFX_SetWorkArea1((WORD)lineBuffer2);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif
	GFX_RCC_SetSrcOffset(0);

    yc = top;

	// some pre-calculations
	if (outputSize < GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

    for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {

		// Get line
	    ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
        memOffset += bmp.width;
    	
      	pData = lineBuffer;
		// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData2 = lineBuffer2;
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData2 = &lineBuffer2[outputSize-1];
            
            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pData2 -= outputSize/2;
            }
            #endif
        #endif

        #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                    pByteData = (BYTE*)lineBuffer2;
                #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                    pByteData = (BYTE*)lineBuffer2;
                    pByteData += (outputSize - 1); 
                #endif           	
            }
        #endif


		// process the pixels of the current line
        for(x = 0; x < xLimit; x++)
        {
            // take one pixel data 
          	temp = *pData++;
  
            if (stretch == IMAGE_X2)
            {
                #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                    #ifdef USE_PALETTE
        	          	if(IsPaletteEnabled())
        	          	{
        	          	    *pByteData++ = (BYTE)temp;
        	          	}
        	          	else
        	          	{
                            *pData2++ = palette[temp];
        	          	}    
                    #else
                        *pData2++ = palette[temp];
                    #endif
                #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                   #ifdef USE_PALETTE
        	          	if(IsPaletteEnabled())
        	          	{
        	          	    *pByteData-- = (BYTE)temp;
        	          	}
        	          	else
        	          	{
                            *pData2-- = palette[temp];
        	          	}    
                    #else
                    	*pData2-- = palette[temp];
    				#endif
           	    #endif			
           	    x++;
            }
                      
            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
    	          	    *pByteData++ = (BYTE)temp;
    	          	}
    	          	else
    	          	{
                        *pData2++ = palette[temp];
    	          	}    
                #else
                    *pData2++ = palette[temp];
                #endif
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
               #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
    	          	    *pByteData-- = (BYTE)temp;
    	          	}
    	          	else
    	          	{
                        *pData2-- = palette[temp];
    	          	}    
                #else
                	*pData2-- = palette[temp];
				#endif
       	    #endif			
        } // for(x = 0; (x < outputSize) && (x + left < GetMaxX()); x++)

		GFX_WaitForCommandQueue(16);
        GFX_WaitForGpu();

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize(x, 1);
		#else	
			GFX_RCC_SetSize(1, x);
		#endif

		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 
			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

			// update the vertical position counter
            yc++;
        }  // for(stretchY = 0; stretchY < stretch; stretchY++)
    }  // for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}

        #endif //#if (COLOR_DEPTH >= 8)

/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
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
        #if (COLOR_DEPTH == 16)

void PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            lineBuffer[(GetMaxX() + (DWORD) 1)];
    WORD            *pData; 
    WORD 			outputSize;
    WORD			*pData2;
    WORD            lineBuffer2[(GetMaxX() + (DWORD) 1)];
    WORD            byteWidth;
	WORD			xLimit;

    WORD            temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD			yc;
    BYTE            stretchY;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER);

    if(pPartialImageData->width != 0)
    {
         memOffset += ((pPartialImageData->xoffset + pPartialImageData->yoffset*(bmp.width)) << 1);
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }
    else
    {
        // Get size
        sizeX = bmp.width;
        sizeY = bmp.height;
    }

	// calculate how many bytes to fetch
    byteWidth = sizeX << 1;

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}

	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > GetMaxX()+1) || (left + outputSize > GetMaxX()+1))
		outputSize = GetMaxX() + 1 - left; 
		
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

   	GFX_SetWorkArea1((WORD)lineBuffer2);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif
	GFX_RCC_SetSrcOffset(0);

    yc = top; 

	// some pre-calculations
	if (outputSize <= GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

    for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {

		// Get line
	    ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += (bmp.width << 1);
    	
      	pData = lineBuffer;
		// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData2 = lineBuffer2;
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData2 = &lineBuffer2[outputSize-1];
        #endif

		// process the pixels of the current line
	    for(x = 0; x < xLimit; x++)
        {
            // take one pixel data 
           	temp = *pData++;

            if (stretch == IMAGE_X2)
            {
                #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                    *pData2++ = temp;
                #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                    *pData2-- = temp;
           	    #endif			
           	    x++;
           	}    
            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                *pData2++ = temp;
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                *pData2-- = temp;
            #endif			

        } // for(x = 0; x < xLimit; x++)

		GFX_WaitForCommandQueue(16);
    	GFX_WaitForGpu();

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize(x, 1);
		#else	
			GFX_RCC_SetSize(1, x);
		#endif

		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 

			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 


			// update the vertical position counter
            yc++;
            
        }  // for(stretchY = 0; stretchY < stretch; stretchY++)
    }  // for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}
        #endif //#if (COLOR_DEPTH == 16)
    #endif //#ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPEDS(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: Image must be located in EDS memory that is accessible by the GPUs.
*
********************************************************************/
void PutImage1BPPEDS(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    BYTE			TempGPUBpp;
    BYTE            lineBuffer[((GetMaxX() + (DWORD) 1) / 8) + 1];
    WORD            lineBuffer2[(GetMaxX() + (DWORD) 1)];

    WORD            palette[2], color;
    BYTE            *pData; 
    WORD			*pData2;
    DWORD 			offsetCount;
	WORD			outputSize;
	WORD			xLimit;

    BYTE            mask, temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            yc;
    BYTE            stretchY;
    
    CHAR            testStretch = (stretch == IMAGE_NORMAL) ? 0x00 : 0x01;
    WORD            lineSize;           // this includes the padding

	#ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; 
#if (DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90)   
#if (COLOR_DEPTH == 4) 
    BYTE nibblePos = 0;
#endif
#if (COLOR_DEPTH == 1)
    BYTE bitPos = 0;
#endif
#endif
#endif

    // First get palette (256 entries)

	// make sure there are no pending RCC GPU commands
    GFX_WaitForCommandQueue(16);
    GFX_WaitForGpu();

	GFX_SetWorkArea1(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress);
	GFX_SetWorkArea2((WORD)palette);
	GFX_RCC_SetSrcOffset((WORD)(sizeof(BITMAP_HEADER)) >> 1);
	GFX_RCC_SetDestOffset(0);
	GFX_RCC_SetSize(2, 1);
	GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

	// 32 is used here since the palette size is 2 words or 32 bits
	offsetCount = 32 + (sizeof(BITMAP_HEADER)*8);

	// Get the image line width in bytes
    lineSize = DrvGetImageWidth(image);
    // account for padding    
    if (lineSize&0x07)
        lineSize += (8 - (lineSize&0x07));

    if(pPartialImageData->width != 0)
    {
        // adjust to the location of the starting data for partial image rendering 
        offsetCount += (pPartialImageData->yoffset*lineSize);
        offsetCount += (pPartialImageData->xoffset);

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;
    }
    else
    {
        // Get the image size
        sizeY = DrvGetImageHeight(image);
        sizeX = DrvGetImageWidth(image);
    }

    yc = top;

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}

	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > (GetMaxX()+1)) || (left + outputSize > (GetMaxX()+1)))
		outputSize = GetMaxX() + 1 - left; 		

	// some pre-calculations
	if (outputSize <= GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

   	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

    for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {
	    	    
		// copy one line from the source
		// make sure there are no pending RCC GPU commands & 
		// make sure the GPUs are not operating since changing the base addresses
		// will break the currently executing GPU command.
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();
	    
	    // the image data is 1 nibble per pixel and normally
	    // if the data is outside the PIC (in extended EDS) we cannot do
	    // GPU operation with less than 8bpp color depth.
	    // But since this will be read and the destination is in internal RAM we can use
	    // 1bpp on the GPU operation.
	    TempGPUBpp = _PUBPP;
	    _PUBPP = GFX_1_BPP;

        // copy one line of data to the buffer
		GFX_SetWorkArea1(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress);
		GFX_SetWorkArea2((DWORD)(WORD)lineBuffer);
				
		GFX_RCC_SetSrcOffset(offsetCount);
		GFX_RCC_SetDestOffset(0);

        GFX_RCC_SetSize(sizeX, 1);
		GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

		// wait until copying is done
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();
	    
	    // reset the _PUBPP to what it was
	    _PUBPP = TempGPUBpp;

       	pData = lineBuffer;
        mask = 0;

		// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData2 = lineBuffer2;
            #ifdef USE_PALETTE
	            if(IsPaletteEnabled())
    	        {
        	        pByteData = (BYTE*)lineBuffer2;
            	}
            #endif
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData2 = &lineBuffer2[outputSize-1];
           	
            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pByteData = (BYTE *)lineBuffer2;

                #if (COLOR_DEPTH == 8)
                    pByteData += outputSize;
                    pByteData--;
                #endif

                #if (COLOR_DEPTH == 4)

                    pByteData += outputSize / 2;
					nibblePos = (outputSize & 0x01);
                    if(nibblePos == 0)
                    {
                        pByteData--;
                    }
                #endif

                #if (COLOR_DEPTH == 1)

                    pByteData += (outputSize / 8) ;
                    bitPos = (outputSize & 0x07);
                    if(bitPos != 0)
                        bitPos -= 1;  // why -1, since bit position starts at 0 (or first bit is at 0 position)    
                    else
                    {
                        bitPos = 7;
                        pByteData--;
                    }				
                #endif
                
            }
			#endif
        #endif

        // process the pixels on the current line
        for(x = 0; x < xLimit; x++)
        {
			if (mask == 0) 
			{
				temp = *pData++;
				mask = 0x01;
			}	

			if (mask & temp)
			{
	            #ifdef USE_PALETTE
               	if(IsPaletteEnabled())
	            	color = 1;
               	else 
	            	color = palette[1];
	            #else	
	            	color = palette[1];
	            #endif
			} 
			else
			{
	            #ifdef USE_PALETTE
               	if(IsPaletteEnabled())
	            	color = 0;
               	else 
	            	color = palette[0];
	            #else	
	            	color = palette[0];
	            #endif
			}
           

           #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
                        *pByteData++ = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                        if(x & 0x01)
                        {
                            *pByteData++ |= color << 4;
                        }else
                        {
                            *pByteData = color & 0x0F;
                        }
                        #endif

                        #if (COLOR_DEPTH == 1)
                        {
                            BYTE pos = (BYTE)x & 0x07;
                            
                            if(pos == 0)
                                *pByteData = color;
                            else
                                *pByteData |= color << pos;

                            if(pos == 7)
                                *pByteData++;
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData2++ = color;
    	          	}    
                #else
                    *pData2++ = color;
                #endif
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
    	          	    *pByteData-- = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                        if (nibblePos == 1)
                        {
                            *pByteData = ((*pByteData & 0xF0) | (color & 0x0F));
                            pByteData--;
                            nibblePos = 0;
                        }else
                        {
                            *pByteData = color << 4;
                            nibblePos = 1;
                        }
                        #endif
                        
                        #if (COLOR_DEPTH == 1)
                        {
                            *pByteData &= ~(1 << bitPos);
                            *pByteData |= (color << bitPos);

                            if (bitPos == 0)
                            {
                                bitPos = 7;
                                pByteData--;
                            }
                            else
                            {
                                bitPos--;
                            }
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData2-- = color;
    	          	}    
                #else
                    *pData2-- = color;
                #endif
       	    #endif			           	     



       	    // Shift to the next pixel (note that if image is stretched, shifting is delayed)
       	    //if (x%(stretch==IMAGE_X2?2:1) == 0) 
       	    if (((x+1) & testStretch) == 0)
            	mask <<= 1;		

        } // for(x = 0; x < xLimit; x++)

		// make sure there are no pending RCC GPU commands
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();

		GFX_SetWorkArea1((DWORD)(WORD)lineBuffer2);
    #ifdef USE_DOUBLE_BUFFERING
		GFX_SetWorkArea2(_drawbuffer);
	#else
        GFX_SetWorkArea2(GFX_GetDisplayArea());    
	#endif
		GFX_RCC_SetSrcOffset(0);

    #if (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
        GFX_RCC_SetSize(x, 1);
    #else
        GFX_RCC_SetSize(1, x);
    #endif	



		for(stretchY = 0; stretchY < stretch; stretchY++)
		{
            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 
		
	        #ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 			
			
			
            yc++;
        } // for(stretchY = 0; stretchY < stretch; stretchY++)
                    
        // adjust the offset counter (padding is already taken cared 
        // of in calculating lineSize
        offsetCount += lineSize;

    } //for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}

/*********************************************************************
* Function: void PutImage4BPPEDS(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: Image must be located in EDS memory that is accessible by the GPUs.
*
********************************************************************/
        #if (COLOR_DEPTH >= 4)

void PutImage4BPPEDS(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    BYTE			TempGPUBpp;
    BYTE            lineBuffer[((GetMaxX() + (DWORD) 1) / 2) + 1];
    WORD            lineBuffer2[(GetMaxX() + (DWORD) 1)];

    WORD            palette[16], color;
    BYTE            *pData; 
    WORD			*pData2;
	WORD			outputSize;
	WORD			xLimit;

    BYTE            temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            yc;
    BYTE            stretchY;

    DWORD 			offsetCount;

    CHAR            testStretch = (stretch == IMAGE_NORMAL) ? 0x01 : 0x03;
    WORD            lineSize;

#ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; 
#if (COLOR_DEPTH == 4) && ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
    BYTE nibblePos = 0;
#endif
#endif

    // First get palette (256 entries)

	// make sure there are no pending RCC GPU commands
    GFX_WaitForCommandQueue(16);
    GFX_WaitForGpu();

	GFX_SetWorkArea1(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress);
	GFX_SetWorkArea2((WORD)palette);
	GFX_RCC_SetSrcOffset((WORD)(sizeof(BITMAP_HEADER)) >> 1);
	GFX_RCC_SetDestOffset(0);
	GFX_RCC_SetSize(16, 1);
	GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

	// 64 is used here since the palette size is 16 words or 64 nibbles
	offsetCount = 64 + (sizeof(BITMAP_HEADER)<<1);

	// Get the image line width in bytes
    lineSize = DrvGetImageWidth(image);
    // account for padding    
    lineSize += (lineSize&0x01);

    if(pPartialImageData->width != 0)
    {
        // adjust to the location of the starting data for partial image rendering 
        offsetCount += (pPartialImageData->yoffset*lineSize);
        offsetCount += (pPartialImageData->xoffset);

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;
    }
    else
    {
        // Get the image size
        sizeY = DrvGetImageHeight(image);
        sizeX = DrvGetImageWidth(image);
    }

    yc = top;

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}
		
	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > (GetMaxX()+1)) || (left + outputSize > (GetMaxX()+1)))
		outputSize = GetMaxX() + 1 - left; 		

	// some pre-calculations
	if (outputSize <= GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

   	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    

	for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {
		// copy one line from the source
		// make sure there are no pending RCC GPU commands & 
		// make sure the GPUs are not operating since changing the base addresses
		// will break the currently executing GPU command.
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();
	    
	    // the image data is 1 nibble per pixel and normally
	    // if the data is outside the PIC (in extended EDS) we cannot do
	    // GPU operation with less than 8bpp color depth.
	    // But since this will be read and the destination is in internal RAM we can use
	    // 4bpp on the GPU operation.
	    TempGPUBpp = _PUBPP;
	    _PUBPP = GFX_4_BPP;

		GFX_SetWorkArea1(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress);
		GFX_SetWorkArea2((DWORD)(WORD)lineBuffer);
		GFX_RCC_SetSrcOffset(offsetCount);
		GFX_RCC_SetDestOffset(0);
		GFX_RCC_SetSize(sizeX, 1);
		GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

		// wait until copying is done
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();
	    
   	    // reset the _PUBPP to what it was
   	    _PUBPP = TempGPUBpp;

       	pData = lineBuffer;

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData2 = lineBuffer2;
            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pByteData = (BYTE*)lineBuffer2;
            }    
            #endif
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData2 = &lineBuffer2[outputSize-1];

            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pByteData = (BYTE*)lineBuffer2;

                #if (COLOR_DEPTH == 8)
                    pByteData += outputSize;
                    pByteData--;
                #endif

                #if (COLOR_DEPTH == 4)

                    pByteData += outputSize / 2;
					nibblePos = (outputSize & 0x01);
                    if(nibblePos == 0)
					{
                        pByteData--;
					}
				#endif

            }
			#endif

        #endif

        // process the pixels on the current line
        for(x = 0; x < xLimit; x++)
        {
            // take one pixel data when needed 
            if ((x & testStretch) == 0)
            	temp = *pData++; 

            #ifdef USE_PALETTE
               	if(IsPaletteEnabled())
	                color = temp >> (((x>>(stretch-1))&0x0001)*4) & 0x000F;
               	else 
	                color = palette[(temp >> (((x>>(stretch-1))&0x0001)*4)) & 0x000F];
            #else
                color = palette[(temp >> (((x>>(stretch-1))&0x0001)*4)) & 0x000F];
            #endif	            
            
            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
                        *pByteData++ = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
                        if(x & 0x01)
                        {
                            *pByteData++ |= color << 4;
                        }else
                        {
                            *pByteData = color & 0x0F;
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData2++ = color;
    	          	}    
                #else
                	*pData2++ = color;
                #endif
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
               #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
                        #if (COLOR_DEPTH == 8)
    	          	    *pByteData-- = (BYTE)color;
                        #endif

                        #if (COLOR_DEPTH == 4)
						if (nibblePos == 1)
                        {
                            *pByteData = ((*pByteData & 0xF0) | (color & 0x0F));
                            pByteData--;
                            nibblePos = 0;
                        }else
                        {
                            *pByteData = color << 4;
                            nibblePos = 1;
                        }
                        #endif
    	          	}
    	          	else
    	          	{
                        *pData2-- = color;
    	          	}    
                #else
                	*pData2-- = color;
                #endif
       	    #endif		

        } // for(x = 0; x < xLimit; x++)

		// make sure there are no pending RCC GPU commands
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();
		GFX_SetWorkArea1((DWORD)(WORD)lineBuffer2);
        #ifdef USE_DOUBLE_BUFFERING
	    	GFX_SetWorkArea2(_drawbuffer);
	    #else
            GFX_SetWorkArea2(GFX_GetDisplayArea());    
	    #endif
		GFX_RCC_SetSrcOffset(0);

        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180))
			GFX_RCC_SetSize(x, 1);
		#else	
			GFX_RCC_SetSize(1, x);
		#endif

		for(stretchY = 0; stretchY < stretch; stretchY++)
		{

            #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 
		
			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

            yc++;
        } // for(stretchY = 0; stretchY < stretch; stretchY++)
        
        // adjust the offset counter (adjust the offset if there are padded bits)
        offsetCount += lineSize;

    } //for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}

        #endif //#if (COLOR_DEPTH >= 4)
    
/*********************************************************************
* Function: void PutImage8BPPEDS(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: Image must be located in EDS memory that is accessible by the GPUs.
*
********************************************************************/
        #if (COLOR_DEPTH >= 8)

void PutImage8BPPEDS(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    BYTE			TempGPUBpp;
    BYTE            lineBuffer[(GetMaxX() + (DWORD) 1)];
    WORD            lineBuffer2[(GetMaxX() + (DWORD) 1)];
    
    WORD            palette[256];
    BYTE            *pData; 
    WORD			*pData2;
    WORD 			outputSize;
	WORD			xLimit;

    BYTE            temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            yc;
    BYTE            stretchY;

    DWORD 			offsetCount;
    WORD            lineSize;

    #ifdef USE_PALETTE
    BYTE            	*pByteData = NULL; // To supress warning 
    #endif


    // First get palette (256 entries)

	// make sure there are no pending RCC GPU commands
    GFX_WaitForCommandQueue(16);
    GFX_WaitForGpu();

	GFX_SetWorkArea1(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress);
	GFX_SetWorkArea2((WORD)palette);
	GFX_RCC_SetSrcOffset((WORD)(sizeof(BITMAP_HEADER)) >> 1);
	GFX_RCC_SetDestOffset(0);
	GFX_RCC_SetSize(256, 1);
	GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

    // 512 is used here since the palette size is 256 words or 512 bytes
	offsetCount = 512 + sizeof(BITMAP_HEADER);

    lineSize = DrvGetImageWidth(image);

    if(pPartialImageData->width != 0)
    {
        // adjust to the location of the starting data for partial image rendering 
        offsetCount += (pPartialImageData->yoffset*lineSize);
        offsetCount += (pPartialImageData->xoffset);

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;
    }
    else
    {
        // Get the image size
        sizeY = DrvGetImageHeight(image);
        sizeX = DrvGetImageWidth(image);
    }
	
    yc = top;

	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}
	
	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > (GetMaxX()+1)) || (left + outputSize - 1 > GetMaxX()))
		outputSize = GetMaxX() + 1 - left; 		

	// some pre-calculations
	if (outputSize < GetMaxX() - left)
		xLimit = outputSize;
	else	 
		xLimit = GetMaxX() - left + 1;

   	#ifdef USE_TRANSPARENT_COLOR
        if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
            GFX_RCC_SetColor(_colorTransparent);
    #endif	    
	
	for(y = 0; (y < sizeY) && (yc <= GetMaxY()); y++)
    {
		// copy one line from the source
		// make sure there are no pending RCC GPU commands & 
		// make sure the GPUs are not operating since changing the base addresses
		// will break the currently executing GPU command.
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();
	    
	    // since the image data is 1 byte per pixel, set the GPU bpp to 8 bpp.
	    TempGPUBpp = _PUBPP;
	    _PUBPP = GFX_8_BPP;

		GFX_SetWorkArea1(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress);
		GFX_SetWorkArea2((DWORD)(WORD)lineBuffer);
		// 512 is used here since the palette size is 256 words or 512 bytes
		GFX_RCC_SetSrcOffset(offsetCount);
		GFX_RCC_SetDestOffset(0);
		GFX_RCC_SetSize(sizeX, 1);
		GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

		// wait until copying is done
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();
	    
        // reset the _PUBPP to what it was
	    _PUBPP = TempGPUBpp;

       	pData = lineBuffer;

		// get the location of the line buffer 
        #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
          	pData2 = lineBuffer2;
        #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
           	pData2 = &lineBuffer2[outputSize-1];
            
            #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                pData2 -= outputSize/2;
            }
            #endif
        #endif

        #ifdef USE_PALETTE
            if(IsPaletteEnabled())
            {
                #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                    pByteData = (BYTE*)lineBuffer2;
                #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                    pByteData = (BYTE*)lineBuffer2;
                    pByteData += (outputSize - 1); 
                #endif           	
            }
        #endif
       	

		// process the pixels of the current line
        for(x = 0; x < xLimit; x++)
        {
            // take one pixel data 
       	    temp = *pData++;
       	    
            if (stretch == IMAGE_X2)
            {
                #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                    #ifdef USE_PALETTE
        	          	if(IsPaletteEnabled())
        	          	{
        	          	    *pByteData++ = (BYTE)temp;
        	          	}
        	          	else
        	          	{
                            *pData2++ = palette[temp];
        	          	}    
                    #else
                        *pData2++ = palette[temp];
                    #endif
                #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
                   #ifdef USE_PALETTE
        	          	if(IsPaletteEnabled())
        	          	{
        	          	    *pByteData-- = (BYTE)temp;
        	          	}
        	          	else
        	          	{
                            *pData2-- = palette[temp];
        	          	}    
                    #else
                    	*pData2-- = palette[temp];
    				#endif
           	    #endif			
           	    x++;

            }

            // Write pixel to buffer
            #if ((DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270))
                #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
    	          	    *pByteData++ = (BYTE)temp;
    	          	}
    	          	else
    	          	{
                        *pData2++ = palette[temp];
    	          	}    
                #else
                    *pData2++ = palette[temp];
                #endif
            #elif ((DISP_ORIENTATION == 180) || (DISP_ORIENTATION == 90))   
               #ifdef USE_PALETTE
    	          	if(IsPaletteEnabled())
    	          	{
    	          	    *pByteData-- = (BYTE)temp;
    	          	}
    	          	else
    	          	{
                        *pData2-- = palette[temp];
    	          	}    
                #else
                	*pData2-- = palette[temp];
				#endif
       	    #endif					  


        } // for(x = 0; x < xLimit; x++)

		// make sure there are no pending RCC GPU commands
	    GFX_WaitForCommandQueue(16);
	    GFX_WaitForGpu();
		GFX_SetWorkArea1((DWORD)(WORD)lineBuffer2);
        #ifdef USE_DOUBLE_BUFFERING
    		GFX_SetWorkArea2(_drawbuffer);
    	#else
            GFX_SetWorkArea2(GFX_GetDisplayArea());    
    	#endif
		GFX_RCC_SetSrcOffset(0);

	    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
			GFX_RCC_SetSize(1, x);
		#else
			GFX_RCC_SetSize(x, 1);
		#endif	

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
           #if (DISP_ORIENTATION == 0) 
    			GFX_RCC_SetDestOffset((yc * (DWORD)DISP_HOR_RESOLUTION) + left);
            #elif (DISP_ORIENTATION == 90)
    			GFX_RCC_SetDestOffset(((GetMaxX() + 1 - (left + outputSize)) * (DWORD)DISP_HOR_RESOLUTION) + yc);
            #elif (DISP_ORIENTATION == 180)
    			GFX_RCC_SetDestOffset(((GetMaxY() - yc) * (DWORD)DISP_HOR_RESOLUTION) + GetMaxX() + 1 - (left+outputSize));
            #elif (DISP_ORIENTATION == 270)
    			GFX_RCC_SetDestOffset((left * (DWORD)DISP_HOR_RESOLUTION) + (GetMaxY() - yc));
			#endif	 
	
			#ifdef USE_TRANSPARENT_COLOR
			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
			    {
        		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 		    
        		else
        	#endif	    
        	    {
    		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                } 

			// update the vertical position counter
            yc++;
        } // for(stretchY = 0; stretchY < stretch; stretchY++)

        offsetCount += lineSize;

    } //for(y = 0; (y < sizeY) && (yc < GetMaxY()); y++)

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();

}

        #endif //#if (COLOR_DEPTH >= 8)
       
/*********************************************************************
* Function: void PutImage16BPPEDS(SHORT left, SHORT top, void* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: Image must be located in EDS memory that is accessible by the GPUs.
*
********************************************************************/
        #if (COLOR_DEPTH == 16)
void PutImage16BPPEDS(SHORT left, SHORT top, void *image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register WORD       x, y; 
    register BYTE       stretchY;

    WORD            	lineBuffer[(GetMaxX() + (DWORD) 1)];
    WORD            	lineBuffer2[(GetMaxX() + (DWORD) 1)];
	WORD 				outputSize, sizeX, sizeY, actualY;
    WORD				*pSrc;
    SHORT               xc, yc;

    WORD                lineSize;
    DWORD               offsetCount;

    lineSize = DrvGetImageWidth(image);
    offsetCount = 0;

    if(pPartialImageData->width != 0)
    {
        // adjust to the location of the starting data for partial image rendering 
        offsetCount += (pPartialImageData->yoffset*lineSize);
        offsetCount += (pPartialImageData->xoffset);

        sizeY = pPartialImageData->height;
        sizeX = pPartialImageData->width;
    }
    else
    {
        // Read image size
        sizeY = DrvGetImageHeight(image);
        sizeX = lineSize;
    }
    
	if (stretch == IMAGE_X2) {
		outputSize = sizeX << 1;
	}   	
	else //if (stretch == IMAGE_NORMAL)
	{
		outputSize = sizeX;
	}
			
	// area of the buffer to use is only the area that
	// spans the viewable area
	if ((outputSize > (GetMaxX()+1)) || (left + outputSize > (GetMaxX()+1)))
		outputSize = GetMaxX() + 1 - left; 		

	// When the DISP_ORIENTATION = 0, the image can be transferred to the display buffer
	// using one transfer. If doing partial image, stretched or using other orientation, 
	// the transfers are performed line by line.
#if (DISP_ORIENTATION == 0) 
	if ((stretch != IMAGE_X2) && (pPartialImageData->width == 0))
	{
    	// Since the data in memory is oriented in the same way as the display we 
    	// can move the whole block. The source is continuous and destination is  
    	// discontinuous since it is the display buffer. The magic number 3 is the 
    	// offset of the actual image data in the image structure.

        // Known issue:
        // When the image exceeds the width of the display buffer, the portion of the
        // image that is beyond the width will go to the next line. 
            

#ifdef USE_TRANSPARENT_COLOR
    #ifdef USE_DOUBLE_BUFFERING
		ROPBlock(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress, _drawbuffer,
                  3, (top * (DWORD)DISP_HOR_RESOLUTION) + left, 
                  GFX_DATA_CONTINUOUS, GFX_DATA_DISCONTINUOUS, 
                  RCC_TRANSPARENT_COPY, RCC_ROP_C, GetTransparentColor(),
                  sizeX, sizeY);
	#else
		ROPBlock(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress, GFX_GetDisplayArea(), 
				  3, (top * (DWORD)DISP_HOR_RESOLUTION) + left, 
                  GFX_DATA_CONTINUOUS, GFX_DATA_DISCONTINUOUS, 
                  RCC_TRANSPARENT_COPY, RCC_ROP_C, GetTransparentColor(),
                  sizeX, sizeY);
	#endif

#else
    #ifdef USE_DOUBLE_BUFFERING

        ROPBlock(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress, _drawbuffer,          
                  3, (top * (DWORD)DISP_HOR_RESOLUTION) + left, 
                  GFX_DATA_CONTINUOUS, GFX_DATA_DISCONTINUOUS, 
                  RCC_COPY, RCC_ROP_C,    
                  0, sizeX, sizeY);
	#else

        ROPBlock(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress, GFX_GetDisplayArea(), 
                  3, (top * (DWORD)DISP_HOR_RESOLUTION) + left, 
                  GFX_DATA_CONTINUOUS, GFX_DATA_DISCONTINUOUS, 
                  RCC_COPY, RCC_ROP_C,    
                  0, sizeX, sizeY);
	#endif
#endif				  
	}
	else
#endif   // #if (DISP_ORIENTATION == 0) 
	{
		// copy the data line by line and block copy it into the destination
		actualY = top;

    	#ifdef USE_TRANSPARENT_COLOR
            if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
                GFX_RCC_SetColor(_colorTransparent);
        #endif	    

	    for(y = 0; (y < sizeY) && (actualY <= GetMaxY()); y++)
		{
			// make sure there are no pending RCC GPU commands & 
			// make sure the GPUs are not operating since changing the base addresses
			// will break the currently executing GPU command.
            GFX_WaitForCommandQueue(16);
	        GFX_WaitForGpu();
	        
			// copy one line from the source
			GFX_SetWorkArea1(((GFX_IMAGE_HEADER*)image)->LOCATION.extAddress); // image location is the source
			GFX_SetWorkArea2((WORD)lineBuffer);					  // buffer is the destination
			GFX_RCC_SetSize(outputSize, 1);						  // copy one line of the image
				
			GFX_RCC_SetSrcOffset(3 + offsetCount);                // set the offset of the source
            
			GFX_RCC_SetDestOffset(0);							  // no offset for the destination

			// initiate the copy, the source is continuous and the 
			// destination is also continuous
			GFX_RCC_StartCopy(	RCC_COPY, RCC_ROP_C, 			\
							RCC_SRC_ADDR_CONTINUOUS, 			\
							RCC_DEST_ADDR_CONTINUOUS);		

			// make sure the copy was executed fully
            GFX_WaitForCommandQueue(16);
	        GFX_WaitForGpu();

            // stretch and swap if needed
	        if (stretch == IMAGE_X2)
	       	{
		       	// stretch the data first
				for (x = 0; x < (outputSize>>1); x += 1)
				{
        		    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 180)
    					lineBuffer2[outputSize-(x*2)-1] = lineBuffer[x];
    					lineBuffer2[outputSize-(x*2)-2] = lineBuffer[x];
        		    #else
    					lineBuffer2[x*2] = lineBuffer[x];
    					lineBuffer2[x*2+1] = lineBuffer[x];
					#endif
				}	
				pSrc = lineBuffer2;		
		    }   
		    else
		    {
    		    // swap data
				for (x = 0; x < (outputSize); x += 1)
				{
        		    #if (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 270)
       					lineBuffer2[x] = lineBuffer[x];
                    #else
       					lineBuffer2[outputSize-x-1] = lineBuffer[x];
                    #endif
   	            }				
		    	
		    	pSrc = lineBuffer2;
		    }	
			
			GFX_SetWorkArea1((WORD)pSrc);
            #ifdef USE_DOUBLE_BUFFERING
        		GFX_SetWorkArea2(_drawbuffer);
        	#else
                GFX_SetWorkArea2(GFX_GetDisplayArea());    
        	#endif
			GFX_RCC_SetSrcOffset(0);

			for(stretchY = 0; stretchY < stretch; stretchY++)
        	{								
				// adjust the x and y coordinate positions based on the screen rotation
				#if (DISP_ORIENTATION == 0)
			    {
			        xc = left;
	        		yc = actualY;
				} 
			    #elif (DISP_ORIENTATION == 90)
			    {
		        	xc = actualY;
		        	yc = GetMaxX() + 1 - (left + outputSize);		        	
			    }
			    #elif (DISP_ORIENTATION == 180)
			    {
			        xc = GetMaxX() + 1 - (left + outputSize);
			        yc = GetMaxY() - actualY;
			    }
			    #elif (DISP_ORIENTATION == 270)
			    {
		        	xc = GetMaxY() - actualY;
			        yc = left;
			    }
				#endif		
			
				GFX_RCC_SetDestOffset((yc*(DWORD)DISP_HOR_RESOLUTION)+xc);
			
	    		#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
					GFX_RCC_SetSize(1, outputSize);
				#else
					GFX_RCC_SetSize(outputSize, 1);
				#endif	
			
    			#ifdef USE_TRANSPARENT_COLOR
    			    if (_colorTransparentEnable == TRANSPARENT_COLOR_ENABLE)
    			    {
            		    GFX_RCC_StartCopy(RCC_TRANSPARENT_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                    } 		    
            		else
            	#endif	    
            	    {
        		        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
                    } 

				actualY++;		
			} //for(stretchY = 0; stretchY < stretch; stretchY++)
    
            offsetCount += lineSize;

		} // for(y = 0; (y < sizeY) && (y < GetMaxY()); y++)
	}	

	// make sure the GPUs are done before exiting
	GFX_WaitForCommandQueue(16);
	GFX_WaitForGpu();
	
}
        #endif //#if (COLOR_DEPTH == 16)

/*********************************************************************
* Function: WORD PutImageDrv(SHORT left, SHORT top, void* image, BYTE stretch, void *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - pointer to the partial image parameters
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
WORD PutImageDrv(SHORT left, SHORT top, void *image, BYTE stretch, void *pPartialImageData)
{
    BYTE        colorDepth;
    WORD        colorTemp;
    WORD        resType;
    PUTIMAGE_PARAM *pParImageData = pPartialImageData;

    if(GFX_GetFreeCommandSpace() < 4)
    {
        #ifndef USE_NONBLOCKING_CONFIG
            GFX_WaitForCommandQueue(4);
        #else
            return (0);
        #endif
    }

    // Save current color
    colorTemp = GetColor();

    resType = *((WORD *)image);

#ifdef USE_COMP_IPU

    if((resType & GFX_COMP_MASK) == COMP_IPU)
    {
        GFX_IMAGE_HEADER *piHdr;
        GFX_IMAGE_HEADER NewiHdr;

        piHdr = (GFX_IMAGE_HEADER*)image;
        memcpy(&NewiHdr, image, sizeof(GFX_IMAGE_HEADER));
        NewiHdr.type = (EDS_EPMP | IMAGE_MBITMAP | COMP_IPU);
        NewiHdr.ID = 1;
        NewiHdr.LOCATION.extAddress = GFX_DECOMPRESSED_DATA_RAM_ADDRESS;

        if(((resType & GFX_MEM_MASK) == EDS_EPMP) || ((resType & GFX_MEM_MASK) == RAM))
        {
            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            GFX_WaitForGpu();
            
            GFX_SetWorkArea1(piHdr->LOCATION.edsAddress);
            
            GFX_SetWorkArea2(GFX_DECOMPRESSED_DATA_RAM_ADDRESS);
            GFX_IPU_SetSrcOffset(0);
            GFX_IPU_SetDestOffset(0);
            GFX_IPU_Inflate(piHdr->param2);

            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            GFX_WaitForGpu();

            if((GFX_IPU_GetDecompressionDone() || GFX_IPU_GetFinalBlock()) == 0)
            {
                return -1;
            }

            image = &NewiHdr;
        }    
            #ifdef USE_BITMAP_FLASH
        else if((resType & GFX_MEM_MASK) == FLASH)
        {
            BYTE aTemp[GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE];
            WORD  quotient, remainder, i;
            DWORD srcaddress, offset;
            FLASH_BYTE *pFlashData = piHdr->LOCATION.progByteAddress;
            BYTE  savedpubpp = _PUBPP;
            
            quotient = piHdr->param1 / GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE;
            remainder = piHdr->param1 - (quotient * GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE);
            
            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            GFX_WaitForGpu();

            _PUBPP = GFX_8_BPP;

            srcaddress = (DWORD)(WORD)aTemp;
            offset = 0;
            
            while(quotient)
            {
                for(i=0; i < GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE; i++)
                {
                    aTemp[i] = pFlashData[i + offset];                
                }
                
                GFX_WaitForCommandQueue(4);
                
                ROPBlock(srcaddress, GFX_COMPRESSED_DATA_RAM_ADDRESS, 0, offset,        \
                         GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, RCC_COPY, RCC_ROP_C, \
                         0, GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE, 1);

                offset += GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE;
                
                quotient--;
            }

            for(i=0; i < remainder; i++)
            {
                aTemp[i] = pFlashData[i + offset];                
            }
            
            GFX_WaitForCommandQueue(4);
            ROPBlock(srcaddress, GFX_COMPRESSED_DATA_RAM_ADDRESS, 0, offset,            \
                     GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, RCC_COPY, RCC_ROP_C,     \
                     0, remainder, 1);

            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            GFX_WaitForGpu();

            _PUBPP = savedpubpp;


            GFX_SetWorkArea1(GFX_COMPRESSED_DATA_RAM_ADDRESS);
            GFX_SetWorkArea2(GFX_DECOMPRESSED_DATA_RAM_ADDRESS);
            GFX_IPU_SetSrcOffset(0);
            GFX_IPU_SetDestOffset(0);
            GFX_IPU_Inflate(piHdr->param2);

            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            GFX_WaitForGpu();

            if((GFX_IPU_GetDecompressionDone() || GFX_IPU_GetFinalBlock()) == 0)
            {
                return -1;
            }
            
            image = &NewiHdr;

        }
            #endif // #ifdef USE_BITMAP_FLASH
            #ifdef USE_BITMAP_EXTERNAL 
        else if((resType & GFX_MEM_MASK) == EXTERNAL)
        {
            BYTE aTemp[GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE];
            WORD  quotient, remainder;
            DWORD srcaddress, offset;
            BYTE  savedpubpp = _PUBPP;
            
            quotient = piHdr->param1 / GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE;
            remainder = piHdr->param1 - (quotient * GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE);
            
            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            GFX_WaitForGpu();

            _PUBPP = GFX_8_BPP;

            srcaddress = (DWORD)(WORD)aTemp;
            offset = 0;
            
            while(quotient)
            {
                ExternalMemoryCallback(image, offset, GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE, (void*)aTemp);        
                
                GFX_WaitForCommandQueue(4);
                ROPBlock(srcaddress, GFX_COMPRESSED_DATA_RAM_ADDRESS, 0, offset,            \
                         GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, RCC_COPY, RCC_ROP_C,     \
                         0, GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE, 1);


                offset += GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE;
                quotient--;
            }

            ExternalMemoryCallback(image, offset, remainder, (void*)aTemp);        
            
            GFX_WaitForCommandQueue(4);
            ROPBlock(srcaddress, GFX_COMPRESSED_DATA_RAM_ADDRESS, 0, offset,            \
                     GFX_DATA_CONTINUOUS, GFX_DATA_CONTINUOUS, RCC_COPY, RCC_ROP_C,     \
                     0, remainder, 1);


            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            GFX_WaitForGpu();

            _PUBPP = savedpubpp;

            GFX_SetWorkArea1(GFX_COMPRESSED_DATA_RAM_ADDRESS);
            GFX_SetWorkArea2(GFX_DECOMPRESSED_DATA_RAM_ADDRESS);
            GFX_IPU_SetSrcOffset(0);
            GFX_IPU_SetDestOffset(0);
            GFX_IPU_Inflate(piHdr->param2);

            GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
            GFX_WaitForGpu();

            if((GFX_IPU_GetDecompressionDone() || GFX_IPU_GetFinalBlock()) == 0)
            {
                return -1;
            }

            image = &NewiHdr;
        }
            #endif // #ifdef USE_BITMAP_EXTERNAL
        else
        {
            return -1;
        }

    }


#endif // #ifdef USE_COMP_IPU

    // Read color depth
    colorDepth = DrvGetImageColorDepth(image);
            
    // Draw picture
    switch(colorDepth)
    {
        case 1:     PutImage1BPPEDS(left, top, image, stretch, pParImageData); break; 
        
            #if (COLOR_DEPTH >= 4)
        case 4:     PutImage4BPPEDS(left, top, image, stretch, pParImageData); break; 
            #endif
                    
            #if (COLOR_DEPTH >= 8)
        case 8:     PutImage8BPPEDS(left, top, image, stretch, pParImageData); break;
            #endif
                    
            #if (COLOR_DEPTH == 16)
        case 16:    PutImage16BPPEDS(left, top, image, stretch, pParImageData); break;
            #endif
                    
        default:    break;
    }

    // Restore current color
    SetColor(colorTemp);
   
    return (1);
}

/*********************************************************************
* Function:  SHORT DrvGetImageHeight(void* pImage)
*
* Overview: This function returns the image height for images in EDS.
*
* PreCondition: A valid image must exist in the location specified 
*               by the pointer pImage.
*
* Input: pImage - Pointer to the image.
*
* Output: Returns the image height in pixels.
*
* Side Effects: none
*
********************************************************************/
SHORT   DrvGetImageHeight(void *pImage)
{
    DWORD_VAL tempWorkArea1, tempWorkArea2;
    SHORT height;

#ifdef USE_COMP_IPU
    SHORT       resType;

    resType = *((SHORT *)pImage);
    
    if((resType & GFX_COMP_MASK) == COMP_IPU)
    {
        GFX_IMAGE_HEADER *iHeader = (GFX_IMAGE_HEADER*)pImage;
        return iHeader->height;
    }    
#endif

	// grab the image height using the RCCGPU

	// make sure there are no pending RCC GPU commands
	GFX_WaitForCommandQueue(16);

	// make sure the GPUs are not operating since changing the base addresses
	// will break the currently executing GPU command.
    GFX_WaitForGpu();

	// store the work areas temporarily
	tempWorkArea1.w[0] = G1W1ADRL; 
	tempWorkArea1.w[1] = G1W1ADRH; 
	tempWorkArea2.w[0] = G1W2ADRL; 
	tempWorkArea2.w[1] = G1W2ADRH; 

	// now change the work areass
	GFX_SetWorkArea1(((IMAGE_EXTERNAL*)pImage)->address);
    GFX_SetWorkArea2((WORD)&height);

    GFX_RCC_SetSrcOffset(1);
    GFX_RCC_SetDestOffset(0);
    GFX_RCC_SetSize(16/COLOR_DEPTH, 1);
    GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

	// make sure there are no pending RCC GPU commands
	GFX_WaitForCommandQueue(16);

	// make sure the GPUs are not operating since changing the base addresses
	// will break the currently executing GPU command.
    GFX_WaitForGpu();
    
	GFX_SetWorkArea1(tempWorkArea1.Val);
	GFX_SetWorkArea2(tempWorkArea2.Val);
    
    /* Note: No need to wait for  complete execution of the command even for Blocking Mode. The next commands will be in the queue & hence will execute only after the completion of this command. */
    return (height);
}	

/*********************************************************************
* Function:  SHORT DrvGetImageWidth(void* pImage)
*
* Overview: This function returns the image width for images in EDS.
*
* PreCondition: A valid image must exist in the location specified 
*               by the pointer pImage.
*
* Input: pImage - Pointer to the image.
*
* Output: Returns the image height in pixels.
*
* Side Effects: none
*
********************************************************************/
SHORT   DrvGetImageWidth(void *pImage)
{
    DWORD_VAL tempWorkArea1, tempWorkArea2;
    SHORT width;

#ifdef USE_COMP_IPU
    SHORT       resType;

    resType = *((SHORT *)pImage);
    
    if((resType & GFX_COMP_MASK) == COMP_IPU)
    {
        GFX_IMAGE_HEADER *iHeader = (GFX_IMAGE_HEADER*)pImage;
        return iHeader->width;
    }    
#endif
	// grab the image width using the RCCGPU
    
	// make sure there are no pending RCC GPU commands
	GFX_WaitForCommandQueue(16);

	// make sure the GPUs are not operating since changing the base addresses
	// will break the currently executing GPU command.
    GFX_WaitForGpu();

	// store the work areas temporarily
	tempWorkArea1.w[0] = G1W1ADRL; 
	tempWorkArea1.w[1] = G1W1ADRH; 
	tempWorkArea2.w[0] = G1W2ADRL; 
	tempWorkArea2.w[1] = G1W2ADRH; 

	// now change the work areass
	GFX_SetWorkArea1(((IMAGE_EXTERNAL*)pImage)->address);
    GFX_SetWorkArea2((WORD)&width);
    

    GFX_RCC_SetSrcOffset(2);
    GFX_RCC_SetDestOffset(0);
    GFX_RCC_SetSize(16/COLOR_DEPTH, 1);
    GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

	// make sure there are no pending RCC GPU commands
	GFX_WaitForCommandQueue(16);

	// make sure the GPUs are not operating since changing the base addresses
	// will break the currently executing GPU command.
    GFX_WaitForGpu();
    
	GFX_SetWorkArea1(tempWorkArea1.Val);
	GFX_SetWorkArea2(tempWorkArea2.Val);
    
    /* Note: No need to wait for  complete execution of the command even for Blocking Mode. The next commands will be in the queue & hence will execute only after the completion of this command. */
    return (width);

}

/*********************************************************************
* Function:  SHORT DrvGetImageColorDepth(void *pImage)
*
* Overview: This function returns the color depth for images in EDS.
*
* PreCondition: A valid image must exist in the location specified 
*               by the pointer pImage.
*
* Input: pImage - Pointer to the image.
*
* Output: Returns the color depth (bits per pixel) of the image.
*
* Side Effects: none
*
********************************************************************/
SHORT DrvGetImageColorDepth(void *pImage)
{
	// grab the image color depth using the RCCGPU
    DWORD_VAL tempWorkArea1, tempWorkArea2;
    SHORT bpp;
    
#ifdef USE_COMP_IPU
    GFX_IMAGE_HEADER *iHeader = (GFX_IMAGE_HEADER*)pImage;
    
    if((iHeader->type & GFX_COMP_MASK) == COMP_IPU)
    {
        return iHeader->colorDepth;
    }    
#endif

	// make sure there are no pending RCC GPU commands
	GFX_WaitForCommandQueue(16);

	// make sure the GPUs are not operating since changing the base addresses
	// will break the currently executing GPU command.
    GFX_WaitForGpu();

	// store the work areas temporarily
	tempWorkArea1.w[0] = G1W1ADRL; 
	tempWorkArea1.w[1] = G1W1ADRH; 
	tempWorkArea2.w[0] = G1W2ADRL; 
	tempWorkArea2.w[1] = G1W2ADRH; 

	// now change the work areass
	GFX_SetWorkArea1(((IMAGE_EXTERNAL*)pImage)->address);
    GFX_SetWorkArea2((WORD)&bpp);

    GFX_RCC_SetSrcOffset(0);
    GFX_RCC_SetDestOffset(0);
    GFX_RCC_SetSize(16/COLOR_DEPTH, 1);
    GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);

	// make sure there are no pending RCC GPU commands
	GFX_WaitForCommandQueue(16);

	// make sure the GPUs are not operating since changing the base addresses
	// will break the currently executing GPU command.
    GFX_WaitForGpu();
    
	GFX_SetWorkArea1(tempWorkArea1.Val);
	GFX_SetWorkArea2(tempWorkArea2.Val);
    
    /* Note: No need to wait for  complete execution of the command even for Blocking Mode. 
             The next commands will be in the queue & hence will execute only after the completion of this command. */
    return (bpp>>8);

}

/*********************************************************************
* Function:  BYTE Decompress(DWORD SrcAddress, DWORD DestAddress, DWORD nbytes);
*
* Overview: Decompresses the nbytes number of data at SrcAddress and
*           places starting from DestAddress. (Blocking)
*
* PreCondition: SrcAddress must point to the start of a compressed block.
*
* Input: SrcAddress  - Source address
*        DestAddress - Destination address
*        nbytes      - Number of bytes to be decompressed
*
* Output: error flag
*
* Side Effects: Modifies workarea_1 & workarea_2 registers.
*
********************************************************************/
BYTE Decompress(DWORD SrcAddress, DWORD DestAddress, DWORD nbytes)
{
    GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    while(GFX_IsPuGpuBusy());
    
    GFX_SetWorkArea1(SrcAddress);
    GFX_IPU_SetSrcOffset(0);
    GFX_SetWorkArea2(DestAddress);
    GFX_IPU_SetDestOffset(0);
    GFX_IPU_Inflate(nbytes);
    
    while(!GFX_IPU_GetDecompressionDone())
    {
        if(GFX_IPU_GetDecompressionError())
        {
            return GFX_IPU_GetDecompressionError();
        }
    }
    
    return GFX_IPU_GetDecompressionError();
}

/*********************************************************************
* Function: void __GFX1Interrupt(void)
*
* Overview: This is an ISR to handle VMRGN Interrupt
*
* PreCondition: Interrupts must be enabled
*
* Input: none
*
* Output: none
*
* Side Effects: Sets the error flag blPaletteChangeError when using palette
*               and there is a pending palette change.
*
********************************************************************/
void __attribute__((interrupt, shadow, no_auto_psv)) _GFX1Interrupt(void)
{
    _GFX1IF = 0;
    if(_VMRGNIF != 0)
    {
        _VMRGNIF = 0;

        #ifdef USE_PALETTE
        
        if(pPendingPalette != NULL)
        {
            blPaletteChangeError = SetPalette(pPendingPalette, PendingStartEntry, PendingLength);
            if(!blPaletteChangeError)
            {
                _palette = pPendingPalette;
                pPendingPalette = NULL;
            }
        }
        
        #endif
        
        #ifdef USE_DOUBLE_BUFFERING
        
        if(blDisplayUpdatePending)            
        {
            UpdateDisplayNow();
            blDisplayUpdatePending = 0;
        }
        
        #endif
    }    
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
    #if (GFX_LCD_TYPE == GFX_LCD_TFT)
    
        _VMRGNIF = 0;
        _VMRGNIE = 1;
        _GFX1IE = 1;
    
    #else
    
    #ifdef USE_PALETTE
    
    if(pPendingPalette != NULL)
    {
        blPaletteChangeError = SetPalette(pPendingPalette, PendingStartEntry, PendingLength);
        
        if(!blPaletteChangeError)
        {
            _palette = pPendingPalette;
            pPendingPalette = NULL;
        }
    }

    #endif
        
    #ifdef USE_DOUBLE_BUFFERING
    
	if(blDisplayUpdatePending)            
	{
		UpdateDisplayNow();
		blDisplayUpdatePending = 0;
	}
        
    #endif
    
    #endif
}

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
    return (((GFX_GetFreeCommandSpace() < GFX_COMMAND_QUEUE_LENGTH)) || GFX_IsPuGpuBusy());
}

#ifdef USE_DOUBLE_BUFFERING

/*********************************************************************
* Function:  SwitchOnDoubleBuffering()
*
* Overview: Switches on the double buffering.
*			Double buffering utilizes two buffers. The frame buffer and the
*           draw buffer. The frame buffer is the buffer that is being displayed
*			while the draw buffer is used for all rendering. 
*           When this function is called, it copies the contents of the frame buffer 
*           to the draw buffer once and all succeeding rendering will be performed on 
*           the draw buffer. To update the frame buffer with newly drawn 
*           items on the draw buffer call UpdateDisplayNow() or RequestDisplayUpdate().
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
void SwitchOnDoubleBuffering(void)
{
    if(blEnableDoubleBuffering == 0) 
    { 
        blEnableDoubleBuffering = 1; 
        InvalidateAll(); 
    }  
}

/*********************************************************************
* Function:  SwitchOffDoubleBuffering()
*
* Overview: Switches off the double buffering.
*           All rendering will be performed on the frame buffer. Calls
*           to UpdateDisplayNow() or RequestDisplayUpdate() will 
*           have no effect.
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
void SwitchOffDoubleBuffering(void)
{
    if(blEnableDoubleBuffering == 1) 
    { 
        UpdateDisplayNow(); 
        _drawbuffer = (_drawbuffer == GFX_BUFFER1)? GFX_BUFFER2: GFX_BUFFER1; 
        blEnableDoubleBuffering = 0; 
    }
}

/*********************************************************************
* Function:  void InvalidateRectangle(WORD left, WORD top, WORD right, WORD bottom)
*
* Overview: Invalidates the specified rectangular area and if the
*           invalidated areas exceed the GFX_MAX_INVALIDATE_AREAS,
*           whole area is marked as invalidate
*
* PreCondition: None
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: None
*
* Side Effects: Only copies back the invalidated areas to the draw -
*               buffer after the exchange of draw and frame buffers
*
********************************************************************/
void InvalidateRectangle(WORD left, WORD top, WORD right, WORD bottom)
{
    if(blInvalidateAll == 1 || blEnableDoubleBuffering == 0)
    {
        return;
    }
    
    if(NoOfInvalidatedRectangleAreas >= GFX_MAX_INVALIDATE_AREAS)
    {
        blInvalidateAll = 1;
        return;
    }
    else
    {
        WORD width, height;

    #if (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

        width   =   right - left + 1;
        height  =   bottom - top + 1;

    #elif (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

        height  =   right - left + 1;
        width   =   bottom - top + 1;

    #endif

    #if (DISP_ORIENTATION == 90)

        bottom  =   DISP_VER_RESOLUTION - left;
        left    =   top;
        right   =   left + width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 180)

        right   =   DISP_HOR_RESOLUTION - left;
        bottom  =   DISP_VER_RESOLUTION - top;
        left    =   right - width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 270)

        right   =   DISP_HOR_RESOLUTION - top;
        top     =   left;
        bottom  =   top + height;
        left    =   right - width;

    #endif

        InvalidatedArea[NoOfInvalidatedRectangleAreas].X = left;
        InvalidatedArea[NoOfInvalidatedRectangleAreas].Y = top;
        InvalidatedArea[NoOfInvalidatedRectangleAreas].W = width;
        InvalidatedArea[NoOfInvalidatedRectangleAreas].H = height;
        NoOfInvalidatedRectangleAreas++;
    }
}

/*********************************************************************
* Function:  static void ExchangeDrawAndFrameBuffers(void);
*
* Overview: Interchanges Draw and Frame buffers and copies the contents
*           of current frame buffer to the draw buffer
*
* PreCondition: The graphical frame must be completely drawn.
*
* Input: None
*
* Output: None
*
* Side Effects: Always draw on draw buffer & not on frame buffer
*
********************************************************************/
static void ExchangeDrawAndFrameBuffers(void)
{
    DWORD SourceBuffer, DestBuffer;
    
    if(blEnableDoubleBuffering == 0)
    {
        return;
    }
    
    if(_drawbuffer == GFX_BUFFER1)
    {
        SourceBuffer = GFX_BUFFER1;
        DestBuffer   = GFX_BUFFER2;
    }
    else
    {
        SourceBuffer = GFX_BUFFER2;
        DestBuffer   = GFX_BUFFER1;
    }
    
    GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    while(GFX_IsPuGpuBusy());

    _drawbuffer = DestBuffer;
    GFX_SetDisplayArea(SourceBuffer);

    GFX_SetWorkArea1(SourceBuffer);
    GFX_SetWorkArea2(DestBuffer);

    if(blInvalidateAll == 1 || NoOfInvalidatedRectangleAreas > GFX_MAX_INVALIDATE_AREAS)
    {
        blInvalidateAll = 0;
        NoOfInvalidatedRectangleAreas = 0;
        GFX_RCC_SetSrcOffset(0);
        GFX_RCC_SetDestOffset(0);
        GFX_RCC_SetSize((WORD)DISP_HOR_RESOLUTION, (WORD)DISP_VER_RESOLUTION);
        GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_CONTINUOUS, RCC_DEST_ADDR_CONTINUOUS);
    }
    else if(NoOfInvalidatedRectangleAreas)
    {
        while(NoOfInvalidatedRectangleAreas)
        {
            DWORD address;
            NoOfInvalidatedRectangleAreas--;
            address = ((DWORD)InvalidatedArea[NoOfInvalidatedRectangleAreas].Y * DISP_HOR_RESOLUTION) + InvalidatedArea[NoOfInvalidatedRectangleAreas].X;
            GFX_WaitForCommandQueue(4);
            GFX_RCC_SetSrcOffset(address);
            GFX_RCC_SetDestOffset(address);
            GFX_RCC_SetSize(InvalidatedArea[NoOfInvalidatedRectangleAreas].W, InvalidatedArea[NoOfInvalidatedRectangleAreas].H);
            GFX_RCC_StartCopy(RCC_COPY, RCC_ROP_C, RCC_SRC_ADDR_DISCONTINUOUS, RCC_DEST_ADDR_DISCONTINUOUS);
        }
    }

    GFX_WaitForCommandQueue(GFX_COMMAND_QUEUE_LENGTH);
    while(GFX_IsPuGpuBusy());
}

/*********************************************************************
* Function:  void UpdateDisplayNow(void)
*
* Overview: Synchronizes the draw and frame buffers immediately
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
void UpdateDisplayNow(void)
{
    ExchangeDrawAndFrameBuffers();
}

/*********************************************************************
* Function:  void RequestDisplayUpdate(void)
*
* Overview: Synchronizes the draw and frame buffers at next VBlank
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
void RequestDisplayUpdate(void)
{
    if(blInvalidateAll == 1 || NoOfInvalidatedRectangleAreas > 0)
    {
        blDisplayUpdatePending = 1;
        StartVBlankInterrupt();
    }
}

#endif //USE_DOUBLE_BUFFERING

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
* Side Effects: none
*
********************************************************************/
void PaletteInit(void)
{
    while(_CLUTBUSY != 0)
    {
        Nop();
    }
    _CLUTRWEN = 0;
    _CLUTEN = 0;
}

/*********************************************************************
* Function: void EnablePalette(void)
*
* Overview: Enables the Palette mode
*
* PreCondition: A system palette has been set using RequestPaletteChange()
*               or SetPalette() and SetPaletteBpp() must have been called
*
* Input: none
*
* Output: none
*
* Side Effects: There may be a little flicker with SetPalette() which
*               can be avoided for TFT with RequestPaletteChange()
*
********************************************************************/
void EnablePalette(void)
{
    if(PaletteBpp <= GFX_16_BPP)
    {
        _PUBPP = PaletteBpp;
        _DPBPP = PaletteBpp;
        _CLUTEN = 1;
    }    
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
* Side Effects: There may be a little flicker which can be avoided for TFT
*               by preceding with a dummy RequestPaletteChange()
*
********************************************************************/
void DisablePalette(void)
{
    _CLUTEN = 0;
    _PUBPP  = GFX_BITS_PER_PIXEL;
    _DPBPP  = GFX_BITS_PER_PIXEL;
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
    return _CLUTEN;
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
        case 1:         PaletteBpp = GFX_1_BPP;
                        break;

        case 2:         PaletteBpp = GFX_2_BPP;
                        break;

        case 4:         PaletteBpp = GFX_4_BPP;
                        break;

        case 8:         PaletteBpp = GFX_8_BPP;
                        break;

        default:        PaletteBpp = GFX_16_BPP;
                        return -1;                        
    }
    
    return 0;
}

/*********************************************************************
* Function: BYTE SetPaletteFlash(PALETTE_ENTRY *pPaletteEntry, WORD startEntry, WORD length)
*
* Overview: Loads the palettes from the flash immediately.
*
* PreCondition: PaletteInit() must be called before.
*
* Input: pPaletteEntry   - Pointer to the palette table in ROM
*        startEntry      - CLUT table will be loaded from this address
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
    WORD counter;

    if((pPaletteEntry == NULL) || ((startEntry + length) > 256))
    {
        return -1;
    }

    _CLUTRWEN = 1;
    for(counter = 0; counter < length; counter++)
    {
        _CLUTADR = startEntry + counter;
        _CLUTWR  = pPaletteEntry[counter].value;
        while(_CLUTBUSY)
        {
            Nop();
        }
    }
    _CLUTRWEN = 0;

    return 0;
}

#endif // USE_PALETTE

#endif //#if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210)



