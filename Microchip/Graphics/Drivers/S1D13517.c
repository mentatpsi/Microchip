/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Epson S1D13517 LCD controller driver
 *  to be used with GFX 3E PICtail board
 *****************************************************************************
 * FileName:        S1D13517.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:       	MPLAB C30, C32
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
 *****************************************************************************/
#include "HardwareProfile.h"

#ifdef GFX_USE_DISPLAY_CONTROLLER_S1D13517

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/gfxcolors.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/S1D13517.h"
#include "Graphics/gfxtcon.h"
#include "Graphics/Primitive.h"
#include "Graphics/Transitions.h"

#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_EPMP)
    #include "Graphics/gfxepmp.h"
#endif  

#if ((COLOR_DEPTH != 16) && (COLOR_DEPTH != 24))
    #error "The S1D13517 driver can currently support the COLOR_DEPTH of 16 and 24 only."
#endif

// Clipping region control
SHORT       _clipRgn;

// Clipping region borders
SHORT       _clipLeft;
SHORT       _clipTop;
SHORT       _clipRight;
SHORT       _clipBottom;

// Color
GFX_COLOR    _color;

//
SHORT _GFXActivePage = 0; //Current Active Page for writing

#ifdef USE_NONBLOCKING_CONFIG
BYTE alphablendActive =0;
#endif
 
#ifdef USE_ALPHABLEND
// Alpha Blending Variables
SHORT _GFXForegroundPage;
SHORT _GFXBackgroundPage;
SHORT _GFXDestinationPage;
#endif
#if (COLOR_DEPTH == 24)
#define RED8(color24)   ((color24 & 0x00FF0000) >> 16)
#define GREEN8(color24) ((color24 & 0x0000FF00) >> 8)
#define BLUE8(color24)  ((color24 & 0x000000FF))

#define RED5(color16)   (BYTE) ((color16 & 0xF800) >> 8)
#define GREEN6(color16) (BYTE) ((color16 & 0x07E0) >> 3)
#define BLUE5(color16)  (BYTE) ((color16 & 0x001F) << 3)

#else
#define RED8(color16)   (BYTE) ((color16 & 0xF800) >> 8)
#define GREEN8(color16) (BYTE) ((color16 & 0x07E0) >> 3)
#define BLUE8(color16)  (BYTE) ((color16 & 0x001F) << 3)
#endif 


#ifndef GFX_DEFAULT_FOREGROUND_PAGE
#define GFX_DEFAULT_FOREGROUND_PAGE		GFX_PAGE9
#endif

#ifndef GFX_DEFAULT_BACKGROUND_PAGE
#define GFX_DEFAULT_BACKGROUND_PAGE 	GFX_PAGE10
#endif

#ifndef GFX_DEFAULT_DESTINATION_PAGE
#define GFX_DEFAULT_DESTINATION_PAGE 	GFX_PAGE0
#endif

typedef struct{
	DWORD page0BaseAddress;
} GFX_PAGE_INFO;

typedef struct{
	GFX_PAGE windowPage;
	WORD xOrigin; 
	WORD yOrigin;
	WORD width;
	WORD height;
} GFX_WINDOW_INFO;


#ifndef TRANSPARENTCOLOR
#define TRANSPARENTCOLOR                RGBConvert(0xFF, 0x00, 0xCC)          //Transparent Color chosen for the application
#endif

GFX_COLOR    pixelWindow[8] = 
{   
    TRANSPARENTCOLOR,
    TRANSPARENTCOLOR,
    TRANSPARENTCOLOR,
    TRANSPARENTCOLOR,
    TRANSPARENTCOLOR,
    TRANSPARENTCOLOR,
    TRANSPARENTCOLOR,
    TRANSPARENTCOLOR
};

//Internal S1D13517 Driver functions	
DWORD 		GFXGetPageOriginAddress(SHORT pageNumber);
DWORD 		GFXGetPageXYAddress(SHORT pageNumber, WORD x, WORD y);
DWORD 		GFXGetWindowOriginAddress(GFX_WINDOW_INFO* GFXWindowInfo);
DWORD 		GFXGetWindowXYAddress(GFX_WINDOW_INFO* GFXWindowInfo, WORD relX, WORD relY);
DWORD 		GetPositionAddress(SHORT page, WORD left, WORD top);
void              PutImage24BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch);
void 		      PutImage24BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch);


#ifdef USE_DOUBLE_BUFFERING

    #define USE_ALPHABLEND
    BYTE blInvalidateAll;
    volatile BYTE blEnableDoubleBuffering;
    volatile BYTE NoOfInvalidatedRectangleAreas;
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

#if (COLOR_DEPTH == 16)
#define WritePixel(color)	    DeviceWrite(color)
#elif (COLOR_DEPTH == 24)
#define WritePixel(color)   { DeviceWrite((WORD)((color) >> 16)); DeviceWrite((WORD)(color));}
#endif

#else

#if (COLOR_DEPTH == 16)
#define WritePixel(color)	{ DeviceWrite(((WORD_VAL)color).v[1]); DeviceWrite(((WORD_VAL)color).v[0]);}
#elif (COLOR_DEPTH == 24)
#define WritePixel(color){ DeviceWrite(((DWORD_VAL)color).v[2]);DeviceWrite(((DWORD_VAL)color).v[1]); DeviceWrite(((DWORD_VAL)color).v[0]);}
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
    DisplayEnable();     // enable S1D13517
 
    DisplaySetCommand(); // set RS line to low for command
   
    DeviceWrite(index);

    DisplaySetData();    // set RS line to high for data

    DeviceWrite(value);

    DisplayDisable();   // disable S1D13517
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
    BYTE value;
 
    DisplayEnable();     // enable S1D13517

    DisplaySetCommand(); // set RS line to low for command

	DeviceWrite(index);

	DisplaySetData();    // set RS line to high for data

	value = DeviceRead();

	DisplayDisable();   // disable S1D13517

	return (value);

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

    /////////////////////////////////////////////////////////////////////
    // Initialize the device
    /////////////////////////////////////////////////////////////////////
	DriverInterfaceInit();

	SetReg(REG2A_DSP_MODE,0x00);
	  
	SetReg(REG68_POWER_SAVE,0x00);		
	SetReg(REG04_PLL_DDIVIDER,0x17);  	//PLLCLK = 1MHz (=24MHz/24=CLKI/PLL_D)
	SetReg(REG06_PLL_0,0x29);		 	//VCO = 142-180MHz
	SetReg(REG08_PLL_1,0x01);  			//PLLCLK is divided by 2
	SetReg(REG0A_PLL_2,0x08);  			//REG0A must be progremmed with value 08h

	SetReg(REG0C_PLL_NDIVIDER,0x59);	//Target Frequency = 90MHz; NN = 180; CLKI = 24MHz; PLL_D = 24:1
 
      #if defined(GFX_USE_DISPLAY_PANEL_PH480272T_005_I11Q) || defined(GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)
            SetReg(REG12_CLK_SRC_SELECT,0x02); 
       #else
		SetReg(REG12_CLK_SRC_SELECT,0x92);  //SYSCLK is the source of SS clock; Internal PLL is the source of SDCLK
       #endif 	

	SetReg(REG04_PLL_DDIVIDER,0x97);	//Enable PLL
	SetReg(REG0E_SS_CONTROL_0,0x8F);  	//Spread Spectrum modulation enabled with default spread of +/-0.25ns
	DelayMs(100);
		
	#if (COLOR_DEPTH == 24)
		SetReg(REG14_LCD_PANEL_TYPE,0x02);  //8:8:8 color format 24BPP Panel
	#else
		SetReg(REG14_LCD_PANEL_TYPE,0x05);  //5:6:5 color format
	#endif

	SetReg(REG16_HDISP_WIDTH,(DISP_HOR_RESOLUTION>>3)-1);          
	SetReg(REG18_HNDP_PERIOD,((DISP_HOR_PULSE_WIDTH+DISP_HOR_FRONT_PORCH+DISP_HOR_BACK_PORCH)>>1)-1);          
	SetReg(REG1A_VDISP_HEIGHT_0,(BYTE)DISP_VER_RESOLUTION-1);
	SetReg(REG1C_VDISP_HEIGHT_1,(DISP_VER_RESOLUTION-1)>>8);
	SetReg(REG1E_VNDP_PERIOD,((DISP_VER_PULSE_WIDTH+DISP_VER_FRONT_PORCH+DISP_VER_BACK_PORCH)>>1)-1);          
	SetReg(REG20_PHS_PULSE_WIDTH,DISP_HOR_PULSE_WIDTH -1);    
	SetReg(REG22_PHS_PULSE_START,DISP_HOR_FRONT_PORCH);     
	SetReg(REG24_PVS_PULSE_WIDTH,DISP_VER_PULSE_WIDTH -1);     
	SetReg(REG26_PVS_PULSE_START,DISP_VER_FRONT_PORCH);    

#ifdef DISP_INV_LSHIFT
        SetReg(REG28_PCLK_POLARITY,0x80);       //Polarity
#else
        SetReg(REG28_PCLK_POLARITY,0x00);       //Polarity
#endif
           	
	SetReg(REG82_SDRAM_CONTROL_0,0x03);		//SDRAM memory size = 128MBits
	SetReg(REG8C_SDRAM_RFS_CNT_0,0xFF);		//SDRAM Refresh Rate = 
	SetReg(REG8E_SDRAM_RFS_CNT_1,0x03);
	SetReg(REG90_SDRAM_BUF_SIZE,0x50);		//SDRAM Write Buffer Memory Size = 1.75MHz (max)	
	SetReg(REG68_POWER_SAVE,0xE8);			//Reset modules
	SetReg(REG68_POWER_SAVE,0x00);
	SetReg(REG68_POWER_SAVE,0x01);			//Enable SDCLK
	SetReg(REG84_SDRAM_STATUS_0,0x86);		// Enable SDRAM, SDRAM auto refresh and powersave

	SetReg(REG52_INPUT_MODE,0x08);        	//Transparency On = 0x08
    SetReg(REG54_TRANSP_KEY_RED, RED8(TRANSPARENTCOLOR));   	
    SetReg(REG56_TRANSP_KEY_GREEN, GREEN8(TRANSPARENTCOLOR)); 
    SetReg(REG58_TRANSP_KEY_BLUE, BLUE8(TRANSPARENTCOLOR)); 
	SetReg(REG6E_GPO_1,0x07);   			//GPO All High

	SetReg(REG60_WRITE_WIN_X_EP,(GetMaxX() >> 2));     // Write Window X End Position Register
	SetReg(REG62_WRITE_WIN_Y_EP_0,(GetMaxY() >> 2));   // Write Window Y End Position Register 0
	SetReg(REG64_WRITE_WIN_Y_EP_1,(BYTE)GetMaxY());    // Write Window Y End Position Register 1

	#if defined(GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)
        DisplayBrightness(0);       //Sets Backlight Brightness Level - PWM
    #else
        DisplayBrightness(100);      //Sets Backlight Brightness Level - PWM
    #endif

	SetReg(REG2A_DSP_MODE,0x01);   			//Enable display interface

	SetReg(REG50_DISPLAY_CONTROL,0x80);   	//Update registers 0x2A-0x4E

	DelayMs(500);             				//Delay of 500ms for SDRAM to stabilize

    /////////////////////////////////////////////////////////////////////
    // Panel TCON Programming
    /////////////////////////////////////////////////////////////////////
    #ifdef USE_TCON_MODULE
    GfxTconInit();
    #endif

    #ifdef USE_DOUBLE_BUFFERING
    // initialize double buffering feature
    blInvalidateAll = 1;            
    blDisplayUpdatePending = 0;
    NoOfInvalidatedRectangleAreas = 0;
    _drawbuffer = GFX_BUFFER1;
    SetActivePage(_drawbuffer);
    SwitchOnDoubleBuffering();
    #endif //USE_DOUBLE_BUFFERING

    #ifdef USE_ALPHABLEND
     _GFXForegroundPage = GFX_DEFAULT_FOREGROUND_PAGE;   //Initialize Alpha Values
     _GFXBackgroundPage = GFX_DEFAULT_BACKGROUND_PAGE;
     _GFXDestinationPage = GFX_DEFAULT_DESTINATION_PAGE;
	SetReg(REGB2_INTERRUPT_CTRL,0x01);				//Enable Alpha Blend Interrupt
    #endif
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

	static  BYTE xc=0;

#if (DISP_ORIENTATION == 90)

     WORD templ = x; 

     x = y;
     y = (DISP_VER_RESOLUTION-1) - templ;

#endif

#if (DISP_ORIENTATION == 270)
 
    WORD templ = x;

     x = (DISP_HOR_RESOLUTION-1)- y;
     y = templ;

#endif

#if (DISP_ORIENTATION == 180)

     x = (DISP_HOR_RESOLUTION-1)- x;
     y = (DISP_VER_RESOLUTION-1)- y;

#endif

    xc = x & 7;  

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif      
  
	DisplayEnable();
	
    DisplaySetCommand(); 
    DeviceWrite(REG5A_WRITE_WIN_X_SP);   
    
	DisplaySetData();    
 
    DeviceWrite(x >> 2);    //Setup the start window size/position    
    DeviceWrite(y >> 2);   
    DeviceWrite(y);   

    DisplaySetCommand(); 
    DeviceWrite(REG66_MEM_DATA_PORT_0);   
	DisplaySetData();      

    pixelWindow[xc] = _color;

    WritePixel(pixelWindow[0]);    //Output the pixelWindow
    WritePixel(pixelWindow[1]);    //Output the pixelWindow
    WritePixel(pixelWindow[2]);    //Output the pixelWindow
    WritePixel(pixelWindow[3]);    //Output the pixelWindow
    WritePixel(pixelWindow[4]);    //Output the pixelWindow
    WritePixel(pixelWindow[5]);    //Output the pixelWindow
    WritePixel(pixelWindow[6]);    //Output the pixelWindow
    WritePixel(pixelWindow[7]);    //Output the pixelWindow
    
    pixelWindow[xc] = TRANSPARENTCOLOR;      
    
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
GFX_COLOR GetPixel(SHORT x, SHORT y)
{
    return (0);
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
	
    DWORD xdelta,ydelta;
    DWORD loffset,roffset;

    if(left > right) 
        return(1);        //Added to make sure function call is accurate
    
    if(bottom < top) 
        return(1);        //Added to make sure function call is accurate
	
   #ifdef USE_NONBLOCKING_CONFIG
   if(IsDeviceBusy())
   {
     return 0;
   }
   #endif

#if (DISP_ORIENTATION == 90)       //Added for rotation capabilities
    DWORD templ,tempr;
    templ = left; tempr = right;

    left = top;
    right = bottom;

    top = (DISP_VER_RESOLUTION-1) - tempr;
    bottom = (DISP_VER_RESOLUTION-1) - templ;
#endif

#if (DISP_ORIENTATION == 270)         //Added for rotation capabilities
    DWORD templ,tempr;
    templ = left; tempr = right;

    left = (DISP_HOR_RESOLUTION-1) - bottom;
    right = (DISP_HOR_RESOLUTION-1) - top;

    top = templ;
    bottom = tempr;
#endif

#if (DISP_ORIENTATION == 180)         //Added for rotation capabilities
    DWORD templ,tempr;
    templ = left; tempr = top;

    left = (DISP_HOR_RESOLUTION-1) - right;
    right = (DISP_HOR_RESOLUTION-1) - templ;

    top = (DISP_VER_RESOLUTION-1) - bottom;
    bottom = (DISP_VER_RESOLUTION-1) - tempr;
#endif

        #ifdef USE_ALPHABLEND_LITE
    if(_alpha != 100)
    {
        BarAlpha(left,top,right,bottom);
        return(1);
    }
        #endif

	if(left>0) 
        loffset = left & 7;
	else 
        loffset = 0;
	
    roffset = right & 7;
	roffset = 8 - roffset;

	DisplayEnable();     
	DisplaySetCommand();       
	DeviceWrite(REG5A_WRITE_WIN_X_SP);   

	DisplaySetData();    
    
    DeviceWrite(left >> 2);     
    DeviceWrite(top >> 2);     
    DeviceWrite(top);     
    DeviceWrite(((right+roffset)-8) >> 2);     
    DeviceWrite(bottom >> 2);     
    DeviceWrite(bottom); 
   
    WORD j,i;

	xdelta = (right - left) + loffset;
    roffset += xdelta;
	ydelta = bottom - top;

    for(i=0;i <= ydelta; i++)  
    {  
        j = 0;
        while(j < loffset)
        {
            WritePixel(TRANSPARENTCOLOR);
            j++;
        }

        while(j <= xdelta)
        {
            WritePixel(_color);
            j++;
        }

        while(j < roffset)
        {
            WritePixel(TRANSPARENTCOLOR);
            j++;
        }
    }

    //Change Write Window Settings back to max
	SetReg(REG60_WRITE_WIN_X_EP,(GetMaxX() >> 2));     // Write Window X End Position Register
	SetReg(REG62_WRITE_WIN_Y_EP_0,(GetMaxY() >> 2));   // Write Window Y End Position Register 0
	SetReg(REG64_WRITE_WIN_Y_EP_1,(BYTE)GetMaxY());    // Write Window Y End Position Register 1

    DisplayDisable();     // disable S1D13517

    return (1);		
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

    #ifdef USE_NONBLOCKING_CONFIG
    if(alphablendActive)
    {

    if(!GetReg((REGB4_INTERRUPT_STAT)))
    {
      return 1;
    }
    else
    {
	  SetReg(REGB6_INTERRUPT_CLEAR,0x01);               //Send Clear Interrupt Command
	  SetReg(REGB6_INTERRUPT_CLEAR,0x00);				//Bring value back to 0
      alphablendActive =0;
    }
    }
    #endif
    return 0;
 
}

#ifdef USE_DRV_OUTCHAR
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
	#ifdef USE_FONT_FLASH	
    GLYPH_ENTRY *pChTable = NULL;
	#endif
    BYTE        *pChImage = NULL;

    #ifdef USE_FONT_EXTERNAL
    GLYPH_ENTRY chTable;
    BYTE        chImage[EXTERNAL_FONT_BUFFER_SIZE];
    WORD        imageSize;
    DWORD_VAL   glyphOffset;
    #endif
    SHORT       chWidth = 0;
    SHORT       xCnt, yCnt, x = 0, y;
    BYTE        temp = 0, mask;


     #ifdef USE_NONBLOCKING_CONFIG
     if(IsDeviceBusy())
     {
        return 0;
     }
     #endif

    if((XCHAR)ch < (XCHAR)_fontFirstChar)
        return (-1);

    if((XCHAR)ch > (XCHAR)_fontLastChar)
        return (-1);

    switch(*((SHORT *)_font))
    {
#ifdef USE_FONT_FLASH
        case FLASH:
            pChTable = (GLYPH_ENTRY *) (((FONT_FLASH *)_font)->address + sizeof(FONT_HEADER)) + ((XCHAR)ch - (XCHAR)_fontFirstChar);

            pChImage = (BYTE *) (((FONT_FLASH *)_font)->address + ((DWORD)(pChTable->offsetMSB) << 8) + pChTable->offsetLSB);

            chWidth = pChTable->width;

            break;
#endif
#ifdef USE_FONT_EXTERNAL
        case EXTERNAL:

            // get glyph entry
            ExternalMemoryCallback
            (
                _font,
                sizeof(FONT_HEADER) + ((XCHAR)ch - (XCHAR)_fontFirstChar) * sizeof(GLYPH_ENTRY),
                sizeof(GLYPH_ENTRY),
                &chTable
            );

            chWidth = chTable.width;

            // width of glyph in bytes
            imageSize = 0;
            if(chWidth & 0x0007)
                imageSize = 1;
            imageSize += (chWidth >> 3);

            // glyph image size
            imageSize *= _fontHeight;

            // get glyph image
            glyphOffset.w[1] = (chTable.offsetMSB >> 8);
            glyphOffset.w[0] = (chTable.offsetMSB << 8) + (chTable.offsetLSB);

            ExternalMemoryCallback(_font, glyphOffset.Val, imageSize, &chImage);
            pChImage = (BYTE *) &chImage;

            break;
#endif

        default:
            return (-1);
    }

    if(_fontOrientation == ORIENT_HOR)
    {
        SHORT left, right;
        SHORT top, bottom;
        DWORD leftOffset, rightOffset;
        WORD charDelta;

#if (DISP_ORIENTATION == 90)
        left = GetY();
        right = left + _fontHeight;

        bottom = (DISP_VER_RESOLUTION - 1) - GetX();
        top = bottom + chWidth;

#elif (DISP_ORIENTATION == 270)
        top = GetX();
        bottom = left + chWidth;

        right = (DISP_HOR_RESOLUTION - 1) - Get();
        left = right + _fontHeight;

#elif (DISP_ORIENTATION == 180)
        right = (DISP_HOR_RESOLUTION - 1) - GetX();
        left = right + chWidth;

        bottom = (DISP_VER_RESOLUTION - 1) - GetY();
        top = bottom + _fontHeight;
#else
        left = GetX();
        right = left + chWidth;
        top = GetY();
        bottom = top + _fontHeight;
#endif
        if(left > 0)
            leftOffset = left & 0x07;
        else
            leftOffset = 0;

        rightOffset = right & 0x07;
        rightOffset = 8 - rightOffset;

        DisplayEnable();     
	    DisplaySetCommand();       
	    DeviceWrite(REG5A_WRITE_WIN_X_SP);   

	    DisplaySetData();    
        
        DeviceWrite(left >> 2);     
        DeviceWrite(top >> 2);     
        DeviceWrite(top);     
        DeviceWrite(((right + rightOffset) - 8) >> 2);     
        DeviceWrite(bottom >> 2);     
        DeviceWrite(bottom); 

        charDelta = chWidth + leftOffset;
        rightOffset += charDelta;

        for(yCnt = 0; yCnt < _fontHeight; yCnt++)
        {
            xCnt = 0;

            while(xCnt < leftOffset)
            {
                WritePixel(TRANSPARENTCOLOR);
                xCnt++;
            }

            mask = 0;

            while(xCnt < charDelta)
            {
                if(mask == 0)
                {
                    temp = *pChImage++;
                    mask = 1;
                }

                if(temp & mask)
                {
                    WritePixel(_color);
                }else
                {
                    WritePixel(TRANSPARENTCOLOR);
                }
                
                xCnt++;
                mask <<= 1;
            }

            while(xCnt < rightOffset)
            {
                WritePixel(TRANSPARENTCOLOR);
                xCnt++;
            }
        }
        _cursorX += chWidth;
        DisplayDisable();     // disable S1D13517

    }
    else
    {
        y = GetX();
        for(yCnt = 0; yCnt < _fontHeight; yCnt++)
        {
            x = GetY();
            mask = 0; 
            for(xCnt = 0; xCnt < chWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *pChImage++;
                    mask = 0x01; 
                }

                if(temp & mask)
                {
                    PutPixel(y, x);
                }

                x--;
                mask <<= 1;
            }

            y++;
        }

        // move cursor
        _cursorY = x;
    }

    return (1);
}
#endif 

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

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif
	
    DisplayEnable();     
    DisplaySetCommand();      
    DeviceWrite(REG5A_WRITE_WIN_X_SP);

	DisplaySetData();   
	DeviceWrite(0x00);                  // X Start Position  
    DeviceWrite(0x00);                  // Y Start Position
    DeviceWrite(0x00);                  // Y Start Position
    DeviceWrite((DISP_HOR_RESOLUTION-1) >>2);     // X End Position 
    DeviceWrite((DISP_VER_RESOLUTION-1) >>2);     // Y End Position
    DeviceWrite((BYTE)(DISP_VER_RESOLUTION-1));         // Y End Position

    DWORD i;

    for(i = 0; i < ((DWORD)DISP_VER_RESOLUTION * (DWORD)DISP_HOR_RESOLUTION); i++)
        WritePixel(_color);

	DisplayDisable();
		
}

#ifdef USE_BITMAP_FLASH
/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        bitmap - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - partial image pointer
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
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    BYTE                temp = 0;
    WORD                sizeX, sizeY;
    WORD                x, y;
    BYTE                stretchX, stretchY;
    GFX_COLOR           pallete[2];
    BYTE                mask;
    WORD 				loffset,roffset;
    WORD                addressOffset = 0;
    BYTE                OffsetFlag = 0x01;     //Offset from BYTE color bit0 for the partial image 

    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

#if (COLOR_DEPTH == 16)	
    pallete[0] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[1] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
#elif (COLOR_DEPTH == 24)	
    flashAddress +=2;
    pallete[0] = *((FLASH_DWORD *)flashAddress);
    flashAddress += 4;
    pallete[1] = *((FLASH_DWORD *)flashAddress);
    flashAddress += 4;
#endif

    if(pPartialImageData->width != 0)
    {
        WORD mod1;

        if(sizeX & 0x07) 
         mod1 = 1;
        else
         mod1 = 0;

         WORD mod3 = ((pPartialImageData->xoffset) & 0x07);

         flashAddress += (pPartialImageData->yoffset)*((sizeX>>3)+mod1);
         flashAddress += (pPartialImageData->xoffset)>>3;

         addressOffset = (sizeX>>3)+mod1;
         addressOffset -= 1+((pPartialImageData->width+mod3)>>3);  

         OffsetFlag <<= mod3;

         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data
	
    for(y = 0; y < sizeY; y++)
    {

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            mask = OffsetFlag;
            temp = *flashAddress++;

            for(x = 0; x < sizeX + loffset + roffset; x++)
            {

			  if((x<=loffset) || (x > (sizeX+loffset)))
              {
					WritePixel(TRANSPARENTCOLOR);
              }
			  else
			  {
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

                // Read 8 pixels from flash
                if(mask == 0x80)
                {
                    temp = *flashAddress++;
                    mask = 0x01;
                }
                else
                {
                    // Shift to the next pixel
                    mask <<= 1;
                }    
            }
		  }
            flashAddress += (addressOffset);
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)

*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - partial image pointer
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
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    BYTE                temp = 0;
    register BYTE       stretchX, stretchY;
	WORD                image_x = 0;
	GFX_COLOR           pallete[16];

    WORD                counter;
	WORD 				loffset,roffset;
    WORD                addressOffset = 0;
    BYTE                OffsetFlag = 0;
	
    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
	
    // Read pallete
    for(counter = 0; counter < 16; counter++)
    {
	#if (COLOR_DEPTH == 16)	
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    #elif (COLOR_DEPTH == 24)
        flashAddress += 2;	
		pallete[counter] = *((FLASH_DWORD *)flashAddress);
		flashAddress += 2;
	#endif
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

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data
	
    for(y = 0; y < sizeY; y++)
    {

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {

            temp = *flashAddress;
            flashAddress += OffsetFlag;

            for(x = OffsetFlag; x < (sizeX + loffset + roffset + OffsetFlag); x++)
            {

			  if((x<=loffset) || (x > (sizeX+loffset)))
              {
				WritePixel(TRANSPARENTCOLOR);
                image_x = 0;
              }
			  else
			  {
                // Read 2 pixels from flash
                if(image_x & 0x0001)
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
                image_x++;

				// Write pixel to screen
				for(stretchX = 0; stretchX < stretch; stretchX++)
				{
					WritePixel(_color);
			      }
     		  }           
            }
              flashAddress += (addressOffset);
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - partial image pointer
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
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_BYTE *flashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y;
    BYTE                temp;
    BYTE                stretchX, stretchY;
	
    GFX_COLOR               pallete[256];

    WORD                counter;
	WORD 				loffset,roffset;
    WORD                addressOffset = 0;

    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
	
    // Read pallete
    for(counter = 0; counter < 256; counter++)
    {
	#if (COLOR_DEPTH == 16)	
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;       
    #elif (COLOR_DEPTH == 24)
        flashAddress += 2;	
		pallete[counter] = *((FLASH_DWORD *)flashAddress);
		flashAddress += 4;
	#endif
    }

    if(pPartialImageData->width != 0)
    {
         flashAddress += pPartialImageData->xoffset + pPartialImageData->yoffset*(sizeX);
         addressOffset = sizeX - pPartialImageData->width;
         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    }

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data


    for(y = 0; y < sizeY; y++)
    {

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {

            for(x = 0; x < (sizeX + loffset + roffset); x++)
            {
			  if((x<=loffset) || (x > (sizeX+loffset)))
              {
				WritePixel(TRANSPARENTCOLOR);
              }
			  else
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
            }
            flashAddress += addressOffset;
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - partial image pointer
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
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register FLASH_WORD *flashAddress;   
    WORD                sizeX, sizeY;
    register WORD       x, y;
    GFX_COLOR                	temp;
    register BYTE       stretchX, stretchY;
	WORD 				loffset,roffset;
    WORD                addressOffset = 0;
	
    // Move pointer to size information
    flashAddress = (FLASH_WORD *)bitmap + 1;

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
	
	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data
	
    for(y = 0; y < sizeY; y++)
    {

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {

            for(x = 0; x < (sizeX + loffset + roffset); x++)
            {

				if((x<=loffset) || (x > (sizeX+loffset)))
				{
					WritePixel(TRANSPARENTCOLOR);
					}
					else
					{
						// Read pixels from flash
						temp = *flashAddress;
						flashAddress++;

                      #if (COLOR_DEPTH == 24) 
                        temp = RGBConvert(RED5(temp),GREEN6(temp),BLUE5(temp)); 
                      #endif
						// Set color
						SetColor(temp);

						// Write pixel to screen
						for(stretchX = 0; stretchX < stretch; stretchX++)
						{
							WritePixel(_color);
						}
					}
			}
            flashAddress += addressOffset;
        }
    }

    DisplayDisable();
}


#if (COLOR_DEPTH == 24)	
/*********************************************************************
* Function: void PutImage24BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
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
void PutImage24BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch)
{
	register DWORD 	*flashAddress;
	WORD                	sizeX, sizeY;
	register WORD       	x, y;
	DWORD                	temp;
	register BYTE       	stretchX, stretchY;
	WORD 					loffset,roffset;
	
	// Move pointer to size information
	flashAddress = (DWORD *)bitmap + 1;

	// Read image size
	sizeY = *((FLASH_WORD *)flashAddress);
	((FLASH_WORD *)flashAddress)++;
	sizeX = *((FLASH_WORD *)flashAddress);
	((FLASH_WORD *)flashAddress)++;
	((FLASH_WORD *)flashAddress)++;

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data

    for(y = 0; y <= sizeY; y++)
    {

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {

            for(x = 0; x < (sizeX + loffset + roffset); x++)
            {
			  if((x<=loffset) || (x > (sizeX+loffset)))
              {
				WritePixel(TRANSPARENTCOLOR);
              }
			  else
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
            }

        }
    }
    
    DisplayDisable();
}
#endif

#endif
#ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - partial image pointer
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
void PutImage1BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;	
    GFX_COLOR       pallete[2];

    BYTE            lineBuffer[((GetMaxX() + 1) / 8) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0;
    BYTE            mask;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;
	WORD 			loffset,roffset;
    BYTE                OffsetFlag = 0x01;     //Offset from BYTE color bit0 for the partial image 
	
    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);
	
    // Get pallete (2 entries)
    ExternalMemoryCallback(bitmap, sizeof(BITMAP_HEADER), 2 * sizeof(GFX_COLOR), pallete);
    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 2 * sizeof(GFX_COLOR);

    // Line width in bytes
    byteWidth = bmp.width >> 3;
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

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data
	
    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(bitmap, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            //SetAddress(address);
            mask = OffsetFlag;
            temp = *pData++;
            for(x = 0; x < (sizeX + loffset + roffset); x++)
            {
			  if((x<=loffset) || (x > (sizeX+loffset)))
              {
				WritePixel(TRANSPARENTCOLOR);
              }
			  else
			  {

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
            }

        }
    }
    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)

*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - partial image pointer
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
void PutImage4BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
	
	GFX_COLOR       pallete[16];

    BYTE            lineBuffer[((GetMaxX() + 1) / 2) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;
	WORD 			loffset,roffset;
	
    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);
	
    // Get pallete (16 entries)
    ExternalMemoryCallback(bitmap, sizeof(BITMAP_HEADER), 16 * sizeof(GFX_COLOR), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(GFX_COLOR);

    // Line width in bytes
    byteWidth = bmp.width >> 1;
    if(bmp.width & 0x0001)
        byteWidth++;

    if(pPartialImageData->width != 0)
    {
         memOffset += pPartialImageData->yoffset*byteWidth;
         memOffset += pPartialImageData->xoffset>>1;

         sizeY = pPartialImageData->height;
         sizeX = pPartialImageData->width;
    } 
    else
    {
         // Get size
         sizeX = bmp.width;
         sizeY = bmp.height;
    }

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data
	
    for(y = 0; y < sizeY; y++)
    {
        // Get line
        ExternalMemoryCallback(bitmap, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            //SetAddress(address);

            for(x = 0; x < (sizeX + loffset + roffset); x++)
            {
			  if((x<=loffset) || (x > (sizeX+loffset)))
              {
				WritePixel(TRANSPARENTCOLOR);
              }
			  else
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

        }
		}
        
    }
    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - partial image pointer
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
void PutImage8BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;

    GFX_COLOR                pallete[256];

    BYTE            lineBuffer[(GetMaxX() + 1)];
    BYTE            *pData;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;
    WORD 		    loffset,roffset;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);
	
    // Get pallete (256 entries)
    ExternalMemoryCallback(bitmap, sizeof(BITMAP_HEADER), 256 * sizeof(GFX_COLOR), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(GFX_COLOR);

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

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data
	
    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(bitmap, memOffset, sizeX, lineBuffer);
        memOffset += bmp.width;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;


           for(x = 0; x < (sizeX + loffset + roffset); x++)
            {
			  if((x<=loffset) || (x > (sizeX+loffset)))
              {
				WritePixel(TRANSPARENTCOLOR);
              }
			  else
			  {
                temp = *pData++;
                SetColor(pallete[temp]);
                
                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
					WritePixel(_color);
                }
            }

              }
		}
        
    }
    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*        pPartialImageData - partial image pointer
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

    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            lineBuffer[(GetMaxX() + 1)];
    WORD            *pData;
    WORD            byteWidth;

    WORD            sizeX, sizeY;
    WORD            xc, yc;
    WORD 		    loffset,roffset;
    BYTE            stretchX, stretchY;

    // Get bitmap header
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

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data

    for(yc = 0; yc < sizeY; yc++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {

            memOffset += byteWidth;

            pData = lineBuffer;

           for(xc = 0; xc < (sizeX + loffset + roffset); xc++)
            {
			  if((xc<=loffset) || (xc > (sizeX+loffset)))
              {
				WritePixel(TRANSPARENTCOLOR);
              }
			  else
			  {           
                _color = *pData++;
                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
			    WritePixel(_color);
                }
              }
            }
        }
        
    }
    DisplayDisable();

}

#if (COLOR_DEPTH == 24)	
/*********************************************************************
* Function: void PutImage24BPPExt(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
* Output: none
*
* Side Effects: none
*
* Overview: outputs hicolor image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage24BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch)
{
    register DWORD  address;
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    DWORD            lineBuffer[(GetMaxX() + 1)];
    DWORD            *pData;
    WORD            byteWidth;
    

	WORD                	sizeX, sizeY;
	register WORD       	x, y;
	DWORD                	temp;
	register BYTE       	stretchX, stretchY;
	WORD 					loffset,roffset;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);
    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    byteWidth = sizeX << 2;

	loffset = left & 0x07;
	roffset = (left + (sizeX*stretch)) & 0x07;
	roffset = 8 - roffset;

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

	DisplayEnable();     // enable S1D13517
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(REG5A_WRITE_WIN_X_SP);
	DisplaySetData();    // set RS line to high for data
	DeviceWrite((left>>3)<<1);
	DeviceWrite(top>>2);
	DeviceWrite(top);
	DeviceWrite(((left+(sizeX*stretch)+roffset-8)>>3)<<1);
	DeviceWrite((top+(sizeY*stretch))>>2);
	DeviceWrite((top+(sizeY*stretch)));
	DisplaySetCommand(); // set RS line to low for command
	DeviceWrite(0x66);
	DisplaySetData();    // set RS line to high for data

    for(y = 0; y <= sizeY; y++)
    {
        // Get line
        ExternalMemoryCallback(bitmap, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
	        pData = lineBuffer;
            
            for(x = 0; x < (sizeX + loffset + roffset); x++)
            {
			  if((x<=loffset) || (x > (sizeX+loffset)))
              {
				WritePixel(TRANSPARENTCOLOR);
              }
			  else
			  {

					// Read pixels from flash
					temp = *pData++;

					// Set color
					SetColor(temp);

					// Write pixel to screen
					for(stretchX = 0; stretchX < stretch; stretchX++)
					{
						WritePixel(_color);
				    }
			  }
            }

        }
    }
    
    DisplayDisable();
}
#endif

#endif

/*********************************************************************
* Function: void DisplayBrightness(WORD level)
********************************************************************/
void DisplayBrightness(WORD level)
{

   if(level == 100)
   {
       SetReg(REG70_PWM_CONTROL,0x85);          //Turn on Backlight 
   }else if (level == 0) 
   {    
       SetReg(REG70_PWM_CONTROL,0x84);       //Turn off Backlight 
   }else if (level <=50)
   {
       level >>= 1;
       level *=  5;  //Sets the value from (0-250)

       SetReg(REG72_PWM_HIGH_DC_0,0xff);
       SetReg(REG74_PWM_HIGH_DC_1,level);
       SetReg(REG7A_PWM_LOW_DC_0,0xff);
       SetReg(REG7C_PWM_LOW_DC_1,0xff);
     
       SetReg(REG70_PWM_CONTROL,0x86);   //Turn off Backlight PWM
       }
       else
       {
       level >>= 1;
       level *=  5;  //Sets the value from (0-250)
       SetReg(REG72_PWM_HIGH_DC_0,level);
       SetReg(REG74_PWM_HIGH_DC_1,0xff);
       SetReg(REG7A_PWM_LOW_DC_0,0xff);
       SetReg(REG7C_PWM_LOW_DC_1,0xff);
     
       SetReg(REG70_PWM_CONTROL,0x86);   //Turn off Backlight PWM
   } 

  
}

#define BUFFER_SIZE 0x140000l  //WVGA

/*********************************************************************
* Function: DWORD GFXGetPageOriginAddress(SHORT pageNumber)
********************************************************************/
DWORD GFXGetPageOriginAddress(SHORT pageNumber)
{
	return(BUFFER_SIZE * (DWORD)pageNumber);
}

/*********************************************************************
* Function: DWORD GFXGetPageXYAddress(SHORT pageNumber, WORD x, WORD y)
********************************************************************/
DWORD GFXGetPageXYAddress(SHORT pageNumber, WORD x, WORD y)
{
    DWORD address;

    #if (DISP_ORIENTATION == 90)       //Added for rotation capabilities
    DWORD tempx;
    tempx = x;

    x=y;
    y = (DWORD)(DISP_VER_RESOLUTION-1) - tempx;
    if(tempx ==0) y=0;
    #endif

    address = (DWORD)y * (DWORD)DISP_HOR_RESOLUTION;
    address += (DWORD)x;
    address *= 3;
    address += GFXGetPageOriginAddress(pageNumber);

    return address;
}

/*********************************************************************
* Function: DWORD GFXGetWindowOriginAddress(GFX_WINDOW_INFO* GFXWindowInfo)
********************************************************************/
DWORD GFXGetWindowOriginAddress(GFX_WINDOW_INFO* GFXWindowInfo)
{
    return (0);
}

/*********************************************************************
* Function: DWORD GFXGetWindowXYAddress(GFX_WINDOW_INFO* GFXWindowInfo, WORD relX, WORD relY)
********************************************************************/
DWORD GFXGetWindowXYAddress(GFX_WINDOW_INFO* GFXWindowInfo, WORD relX, WORD relY)
{
    return (0);
}

 #ifdef USE_ALPHABLEND
/*********************************************************************
* Function: void AlphaBlendWindow(DWORD foregroundArea, SHORT foregroundLeft, SHORT foregroundTop,
                      DWORD backgroundArea, SHORT backgroundLeft, SHORT backgroundTop,
					  DWORD destinationArea, SHORT destinationLeft, SHORT destinationTop,		            
					  WORD  width, WORD height,  	
					  BYTE  alphaPercentage)
********************************************************************/
WORD AlphaBlendWindow(DWORD foregroundArea, SHORT foregroundLeft, SHORT foregroundTop,
                      DWORD backgroundArea, SHORT backgroundLeft, SHORT backgroundTop,
					  DWORD destinationArea, SHORT destinationLeft, SHORT destinationTop,		            
					  WORD  width, WORD height,  	
					  BYTE  alphaPercentage)
{   

   #ifdef USE_NONBLOCKING_CONFIG
   if(IsDeviceBusy())
   {
      return (0);
   }
   #endif

    DWORD_VAL foregroundWindowAddr,backgroundWindowAddr,destinationWindowAddr;

    foregroundWindowAddr.Val = GFXGetPageXYAddress(foregroundArea, foregroundLeft, foregroundTop);
    backgroundWindowAddr.Val = GFXGetPageXYAddress(backgroundArea, backgroundLeft, backgroundTop);
    destinationWindowAddr.Val = GFXGetPageXYAddress(destinationArea, destinationLeft, destinationTop);
	
    #if (DISP_ORIENTATION == 90)       //Added for rotation capabilities
    DWORD tempwidth;
    DWORD address;
    tempwidth = width;
    width = height;
    height = tempwidth;

    address = (DWORD)height * ((DWORD)DISP_HOR_RESOLUTION);
    address *= 3;

    foregroundWindowAddr.Val -= address;
    backgroundWindowAddr.Val -= address;
    destinationWindowAddr.Val -= address;
    #endif

    SetReg(REG98_ALP_HR_SIZE,((width-1)>>3)); 
    SetReg(REG9A_ALP_VR_SIZE_0,height); 
    SetReg(REG9C_ALP_VR_SIZE_1,(height>>8)); 

    foregroundWindowAddr.Val  -=(foregroundWindowAddr.Val %8);
    backgroundWindowAddr.Val  -=(backgroundWindowAddr.Val %8);
    destinationWindowAddr.Val -=(destinationWindowAddr.Val %8);
	
    SetReg(REGA0_ALP_IN_IMG1_SA_0,foregroundWindowAddr.v[0]);    
    SetReg(REGA2_ALP_IN_IMG1_SA_1,foregroundWindowAddr.v[1]); 
    SetReg(REGA4_ALP_IN_IMG1_SA_2,foregroundWindowAddr.v[2]); 
    SetReg(REGA6_ALP_IN_IMG2_SA_0,backgroundWindowAddr.v[0]);    
    SetReg(REGA8_ALP_IN_IMG2_SA_1,backgroundWindowAddr.v[1]); 
    SetReg(REGAA_ALP_IN_IMG2_SA_2,backgroundWindowAddr.v[2]); 
    SetReg(REGAC_ALP_OUT_IMG_SA_0,destinationWindowAddr.v[0]);  
    SetReg(REGAE_ALP_OUT_IMG_SA_1,destinationWindowAddr.v[1]); 
    SetReg(REGB0_ALP_OUT_IMG_SA_2,destinationWindowAddr.v[2]); 
	
	SetReg(REG9E_ALP_VALUE,0x80 | Percentage2Alpha(alphaPercentage));  

    SetReg(REG94_ALP_CONTROL,0x01); 				//Enable Alpha Blend  
	SetReg(REG94_ALP_CONTROL,0x00);                 // Added From DataSheet

#ifndef USE_NONBLOCKING_CONFIG
     while(!GetReg((REGB4_INTERRUPT_STAT)));
	  SetReg(REGB6_INTERRUPT_CLEAR,0x01);               //Send Clear Interrupt Command
	  SetReg(REGB6_INTERRUPT_CLEAR,0x00);				//Bring value back to 0
#else
    alphablendActive = 1;                           //Flag for Driver to know Alphablending is active
#endif 

   return 1;

}			

void CopyPageWindow( BYTE srcPage, BYTE dstPage,       
                        WORD srcX, WORD srcY, WORD dstX, WORD dstY, 
                        WORD width, WORD height)			
{
			while(!AlphaBlendWindow(srcPage,srcX,srcY, srcPage,srcX,srcY, dstPage,dstX,dstY, width, height,                               
                             100));	
}

#endif

/*********************************************************************
* Function:  SetActivePage(page)
*
* Overview: Sets active graphic page.
*
* PreCondition: none
*
* Input: page - Graphic page number.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SetActivePage(WORD page)
{
    _GFXActivePage = page;
    SetReg(REG52_INPUT_MODE, 0x08 | (page<<4));
}

/*********************************************************************
* Function: SetVisualPage(page)
*
* Overview: Sets graphic page to display.
*
* PreCondition: none
*
* Input: page - Graphic page number
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SetVisualPage(WORD page)
{
    SetReg(REG2A_DSP_MODE, 0x01 | (page<<4)); 
    SetReg(REG50_DISPLAY_CONTROL,0x80);
}  

#ifdef USE_TRANSITION_EFFECTS

extern void PlainCopyRectangle(void);

extern WORD _transitionpending, _left, _top, _right, _bottom, _type, _delay_ms, _param1, _param2;
extern DWORD _srcpageaddr, _destpageaddr;
extern WORD Startx;
extern WORD Starty;
extern WORD Width;
extern WORD Height;

/*********************************************************************
* Function: WORD CopyBlock(DWORD srcAddr, DWORD dstAddr, DWORD srcOffset, DWORD dstOffset, WORD width, WORD height)
*
* Overview: Moves a block of data from source specified by srcAddr 
*           and srcOffset to the destination specified by dstAddr 
*           and dstOffset.
*
* PreCondition: none
*
* Input: srcAddr - the base address of the data to be moved
*        dstAddr - the base address of the new location of the moved data 
*        srcOffset - offset of the data to be moved with respect to the 
*                    source base address.
*        dstOffset - offset of the new location of the moved data respect 
*                    to the source base address.
*        width - width of the block of data to be moved
*        height - height of the block of data to be moved
*
* Output: none
* Side Effects: none
* Note: none
********************************************************************/
WORD CopyBlock(DWORD srcAddr, DWORD dstAddr, DWORD srcOffset, DWORD dstOffset, WORD width, WORD height)
{

    DWORD_VAL source,destination;
    source.Val      = srcAddr + 3*srcOffset;
    destination.Val = dstAddr + 3*dstOffset;

    source.Val  -=(source.Val %8);
    destination.Val -=(destination.Val % 8);

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif
    
	SetReg(REG98_ALP_HR_SIZE,((width-1)>>3)); 
    SetReg(REG9A_ALP_VR_SIZE_0,height); 
    SetReg(REG9C_ALP_VR_SIZE_1,(height>>8)); 

    SetReg(REGA0_ALP_IN_IMG1_SA_0,source.v[0]);    
    SetReg(REGA2_ALP_IN_IMG1_SA_1,source.v[1]); 
    SetReg(REGA4_ALP_IN_IMG1_SA_2,source.v[2]); 
    SetReg(REGA6_ALP_IN_IMG2_SA_0,source.v[0]);    
    SetReg(REGA8_ALP_IN_IMG2_SA_1,source.v[1]); 
    SetReg(REGAA_ALP_IN_IMG2_SA_2,source.v[2]); 
    SetReg(REGAC_ALP_OUT_IMG_SA_0,destination.v[0]);  
    SetReg(REGAE_ALP_OUT_IMG_SA_1,destination.v[1]); 
    SetReg(REGB0_ALP_OUT_IMG_SA_2,destination.v[2]); 
	
	SetReg(REG9E_ALP_VALUE,0x80 | Percentage2Alpha(0));  
 
    SetReg(REG94_ALP_CONTROL,0x01); 				//Enable Alpha Blend  
	SetReg(REG94_ALP_CONTROL,0x00);                 // Added From DataSheet

#ifndef USE_NONBLOCKING_CONFIG
     while(!GetReg((REGB4_INTERRUPT_STAT)));
	  SetReg(REGB6_INTERRUPT_CLEAR,0x01);               //Send Clear Interrupt Command
	  SetReg(REGB6_INTERRUPT_CLEAR,0x00);				//Bring value back to 0
#else
    alphablendActive = 1;                           //Flag for Driver to know Alphablending is active
#endif 

    return (1);
}

/*********************************************************************
* Function: void PushRectangle(void)
*
* Overview: Moves window in a certain direction
*
* PreCondition: direction and window parameters defined
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void PushRectangle(void)
{
WORD direction = _param2;
int i;
DWORD_VAL shift;

    SetReg(REG2A_DSP_MODE,0x09);      //Turn on both PIP1 + PIP2
 
   //Shift PIP1 screen (which is the original screen user was on)///
	SetReg(REG32_PIP1_WIN_X_SP, Startx);
    SetReg(REG38_PIP1_WIN_X_EP, (Startx +(Width-1)) >> 2   );
	SetReg(REG34_PIP1_WIN_Y_SP_0, Starty >> 2);  
	SetReg(REG36_PIP1_WIN_Y_SP_1, Starty);
	SetReg(REG3A_PIP1_WIN_Y_EP_0, (Starty+Height) >> 2  );
	SetReg(REG3C_PIP1_WIN_Y_EP_1, Starty + Height );

    SetReg(REG44_PIP2_WIN_X_SP, Startx);
    SetReg(REG4A_PIP2_WIN_X_EP, (Startx + (Width - 1)) >> 2   );
	SetReg(REG46_PIP2_WIN_Y_SP_0, Starty >> 2);  
	SetReg(REG48_PIP2_WIN_Y_SP_1, Starty);
	SetReg(REG4C_PIP2_WIN_Y_EP_0, (Starty + Height) >> 2  );
	SetReg(REG4E_PIP2_WIN_Y_EP_1, (Starty + Height) );

    switch(direction)
    {

    case LEFT_TO_RIGHT:
            for(i=0; i < (Width-9); i=i+8)
            {
                 shift.Val = _destpageaddr;    //Page 2

                 SetReg(REG2C_PIP1_DSP_SA_0,shift.v[0]); 
	             SetReg(REG2E_PIP1_DSP_SA_1,shift.v[1]);
	             SetReg(REG30_PIP1_DSP_SA_2,shift.v[2]);
                 SetReg(REG32_PIP1_WIN_X_SP, (Startx+i) >> 2  );

                 shift.Val = _srcpageaddr;
                 shift.Val += (DISP_HOR_RESOLUTION-i-8)*3;

                 SetReg(REG3E_PIP2_DSP_SA_0,shift.v[0]); 
	             SetReg(REG40_PIP2_DSP_SA_1,shift.v[1]);
	             SetReg(REG42_PIP2_DSP_SA_2,shift.v[2]); 
                 SetReg(REG32_PIP1_WIN_X_SP, (Startx+i) >> 2  ); 
                 
                 SetReg(REG50_DISPLAY_CONTROL,0x80);          //Refresh must be callsed after swapping screens
                 DelayMs(_delay_ms);
                 
            }
        break;
    case RIGHT_TO_LEFT:
            for(i=0; i < (Width-9); i=i+8)
            {
                 shift.Val = _destpageaddr;    //Page 2
                 shift.Val += 3 * i;
                 SetReg(REG2C_PIP1_DSP_SA_0,shift.v[0]); 
	             SetReg(REG2E_PIP1_DSP_SA_1,shift.v[1]);
	             SetReg(REG30_PIP1_DSP_SA_2,shift.v[2]);
                 SetReg(REG38_PIP1_WIN_X_EP, ((Startx+Width-1)- 8-i) >> 2 );

                 shift.Val = _srcpageaddr;

                 SetReg(REG3E_PIP2_DSP_SA_0,shift.v[0]); 
	             SetReg(REG40_PIP2_DSP_SA_1,shift.v[1]);
	             SetReg(REG42_PIP2_DSP_SA_2,shift.v[2]); 
                 SetReg(REG44_PIP2_WIN_X_SP,((Startx+Width-1) - i) >> 2  ); 
                 
                 SetReg(REG50_DISPLAY_CONTROL,0x80);          //Refresh must be callsed after swapping screens
                 DelayMs(_delay_ms);

            }
        break;
    case TOP_TO_BOTTOM:
            for(i=0; i < Height; i=i+8)
            {
                 shift.Val = _destpageaddr;    //Page 2

                 SetReg(REG2C_PIP1_DSP_SA_0,shift.v[0]); 
	             SetReg(REG2E_PIP1_DSP_SA_1,shift.v[1]);
	             SetReg(REG30_PIP1_DSP_SA_2,shift.v[2]);
                 SetReg(REG34_PIP1_WIN_Y_SP_0, (Starty+i) >> 2);  
                 SetReg(REG36_PIP1_WIN_Y_SP_1, (Starty+i));

                 shift.Val = (long)DISP_VER_RESOLUTION - (long)i;
                 shift.Val *= (long)DISP_HOR_RESOLUTION;
                 shift.Val *= 3;
                 shift.Val += _srcpageaddr;

                 SetReg(REG3E_PIP2_DSP_SA_0,shift.v[0]); 
	             SetReg(REG40_PIP2_DSP_SA_1,shift.v[1]);
	             SetReg(REG42_PIP2_DSP_SA_2,shift.v[2]); 
                 SetReg(REG4C_PIP2_WIN_Y_EP_0, (Starty+i) >> 2  );
	             SetReg(REG4E_PIP2_WIN_Y_EP_1, (Starty+i) ); 
                 
                 SetReg(REG50_DISPLAY_CONTROL,0x80);          //Refresh must be callsed after swapping screens
                 DelayMs(_delay_ms);
            }
        break;
    case BOTTOM_TO_TOP:
            for(i=0; i < Height; i=i+8)
            {
                 shift.Val = (long)DISP_HOR_RESOLUTION * (long)i;
                 shift.Val *= 3;
                 shift.Val += _destpageaddr;

                 SetReg(REG2C_PIP1_DSP_SA_0,shift.v[0]); 
	             SetReg(REG2E_PIP1_DSP_SA_1,shift.v[1]);
	             SetReg(REG30_PIP1_DSP_SA_2,shift.v[2]);
                 SetReg(REG3A_PIP1_WIN_Y_EP_0, ((Starty+Height) - i) >> 2  );
	             SetReg(REG3C_PIP1_WIN_Y_EP_1, ((Starty+Height) - i) );

                 shift.Val = _srcpageaddr;

                 SetReg(REG3E_PIP2_DSP_SA_0,shift.v[0]); 
	             SetReg(REG40_PIP2_DSP_SA_1,shift.v[1]);
	             SetReg(REG42_PIP2_DSP_SA_2,shift.v[2]); 
                 SetReg(REG46_PIP2_WIN_Y_SP_0,((Starty+Height) - i) >> 2);  
                 SetReg(REG48_PIP2_WIN_Y_SP_1, ((Starty+Height)) - i); 
                 
                 SetReg(REG50_DISPLAY_CONTROL,0x80);          //Refresh must be callsed after swapping screens
                 DelayMs(_delay_ms);


            }
        break;
    default: 
        break;
    }

    PlainCopyRectangle();

    SetReg(REG2A_DSP_MODE,0x01);                           //Turn off PIP
    SetReg(REG50_DISPLAY_CONTROL,0x80);                    //Refresh must be callsed after swapping screens 

}
#endif

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
void GFX_DRIVER_CreateLayer( GFX_LAYER Layer, WORD Config, WORD XStart,  WORD YStart,  WORD Width,  WORD Height )
{
   //PIP1 screen
	SetReg(REG32_PIP1_WIN_X_SP, XStart>>2);
    SetReg(REG38_PIP1_WIN_X_EP, (XStart +(Width)) >> 2   );
	SetReg(REG34_PIP1_WIN_Y_SP_0, YStart >> 2);  
	SetReg(REG36_PIP1_WIN_Y_SP_1, YStart);
	SetReg(REG3A_PIP1_WIN_Y_EP_0, (YStart+Height) >> 2  );
	SetReg(REG3C_PIP1_WIN_Y_EP_1, YStart + Height );

}

void GFX_DRIVER_LayerStartAddress( GFX_LAYER Layer,WORD page, WORD X, WORD Y )
{
     X &= 0xf8; //X must be a multiple of 8

    DWORD PIPStartAddress = GFXGetPageXYAddress(page,X,Y);

    SetReg(REG2C_PIP1_DSP_SA_0,PIPStartAddress); 
	SetReg(REG2E_PIP1_DSP_SA_1,PIPStartAddress >> 8);
	SetReg(REG30_PIP1_DSP_SA_2,PIPStartAddress >> 16);
}

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
void GFX_DRIVER_ShowLayer( GFX_LAYER Layer )
{
    BYTE temp = GetReg(REG2A_DSP_MODE);

    SetReg(REG2A_DSP_MODE,temp|0x04);                      //Turn on PIP 1
    SetReg(REG50_DISPLAY_CONTROL,0x80);                    //Refresh must be callsed after swapping screens
}

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
void GFX_DRIVER_HideLayer( GFX_LAYER Layer )
{

   #ifdef USE_NONBLOCKING_CONFIG
   while(IsDeviceBusy());
   #endif

    BYTE temp = GetReg(REG2A_DSP_MODE);

    SetReg(REG2A_DSP_MODE,temp&0xf1);                           //Turn off PIP 1
    SetReg(REG50_DISPLAY_CONTROL,0x80);                    //Refresh must be callsed after swapping screens
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

     _drawbuffer = DestBuffer;
     SetActivePage(_drawbuffer);
 
     SetVisualPage(SourceBuffer);

    if(blInvalidateAll == 1 || (NoOfInvalidatedRectangleAreas > GFX_MAX_INVALIDATE_AREAS))
    {
        blInvalidateAll = 0;
        NoOfInvalidatedRectangleAreas = 0;
        CopyPageWindow( SourceBuffer, DestBuffer, 0, 0,0, 0,GetMaxX(), GetMaxY());
    }
    else if(NoOfInvalidatedRectangleAreas)
    {
        while(NoOfInvalidatedRectangleAreas)
        {
            NoOfInvalidatedRectangleAreas--;
            CopyPageWindow( SourceBuffer, DestBuffer, InvalidatedArea[NoOfInvalidatedRectangleAreas].X, InvalidatedArea[NoOfInvalidatedRectangleAreas].Y,InvalidatedArea[NoOfInvalidatedRectangleAreas].X, InvalidatedArea[NoOfInvalidatedRectangleAreas].Y,InvalidatedArea[NoOfInvalidatedRectangleAreas].W, InvalidatedArea[NoOfInvalidatedRectangleAreas].H);
        }
    }

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
      UpdateDisplayNow();
}

#endif //USE_DOUBLE_BUFFERING

#ifdef USE_ALPHABLEND_LITE
/*********************************************************************
* Function: WORD BarAlpha(SHORT Left, SHORT Top,WORD  Right, WORD Bottom)
********************************************************************/
WORD BarAlpha(SHORT Left, SHORT Top,WORD  Right, WORD Bottom)
{

SHORT y=Top;

WORD width = (Right-Left)+8;

BYTE currentAlpha = _alpha;

            if(_prevAlphaColor == BLACK)
            {

                CopyPageWindow(_GFXActivePage, _GFXBackgroundPage, Left, Top, Left, Bottom, 
                                       width, 1);
                
            }
            
            _alpha = 100;
            
            Bar(Left,Top,Right,Bottom);
            
            _alpha = currentAlpha;
            
            AlphaBlendWindow(_GFXActivePage, Left, Top,
                             _GFXBackgroundPage, Left, Top,
            		         _GFXActivePage, Left, Top,		            
            			     width, (Bottom-Top+1),  	
            			     _alpha);

return (1);
}
#endif //USE_ALPHABLEND_LITE
#endif //GFX_USE_DISPLAY_CONTROLLER_S1D13517
