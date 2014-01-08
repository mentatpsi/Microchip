/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Epson S1D13522 EPD controller driver
 *****************************************************************************
 * FileName:        S1D13522.c
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2012 Microchip Technology Inc.  All rights reserved.
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

#ifdef GFX_USE_DISPLAY_CONTROLLER_S1D13522

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/gfxcolors.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/S1D13522.h"
#include "Graphics/gfxtcon.h"
#include "Graphics/Primitive.h"


#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_EPMP)
    #include "Graphics/gfxepmp.h"
#endif  

#if defined ( USE_SPI_CHANNEL )
	#include "drv_spi.h"
#endif

// Pip/Main/Cursor
static WORD g_HostMemAccessCfg;
static GFX_LAYER g_ActiveLayer = GFX_MAIN_LAYER;
static WORD g_PipLayerWidth;
static WORD g_PipLayerHeight;
static WORD g_PipConfig;
static WORD g_CursorLayerWidth;
static WORD g_CursorLayerHeight;
static WORD g_CursorConfig;

#if (COLOR_DEPTH > 8)
#error Color depths that are greater than 8bpp can not be used with this controller.  It is recommended that a color depth of 4bpp is used.
#endif

#if (COLOR_DEPTH == 8)
#warning It is recommended that a color depth of 4bpp or less it used
#endif


WORD DefAutoUpd;

// Color
GFX_COLOR    _color;

// Clipping region control
SHORT       _clipRgn;

// Clipping region borders
SHORT       _clipLeft;
SHORT       _clipTop;
SHORT       _clipRight;
SHORT       _clipBottom;

//#define USE_PRIMITIVE_PUTIMAGE
#ifndef USE_PRIMITIVE_PUTIMAGE
    #warning "This driver does not support partial put image feature. To enable partial put image feature, uncomment the macro USE_PRIMITIVE_PUTIMAGE in this file. This will enable the PutImageXBPPYYY() in the Primitive.c implementation. It is also necessary to regenerate the resources (images) and uncheck the No Line Padding Option."

    #ifndef USE_BITMAP_NO_PADDING_LINE
      #warning "This driver requires that USE_BITMAP_NO_PADDING_LINE is defined in GraphicsConfig.h when 1bpp and 4bpp images are used."
    #endif
#endif

void        PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);

void        PutImage1BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage4BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);
void        PutImage8BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData);

static BOOL g_UpdateRequest;
		
//=============================================================================
//	STATIC (PRIVATE) FUNCTIONS
//=============================================================================
// ----------------------------------------------------------------------------------
// seDeviceWrite()
// 
// ----------------------------------------------------------------------------------
static inline void seDeviceWrite(WORD data) 
{
#if defined ( USE_SPI_CHANNEL )
	
// See definitions are in HWP_S1D13522_EPD.h file

//***********************************************
// SPI Setup 
//***********************************************										 
	// When SPI settings are being changed, even if the settings are the same as before,
	// for some reasons, the SCK output from the MCU will not be driven for a period of time.
	// The change of this signal from driven low to tri-state may be treated by S1D13522 as 1
	// extra serial host input clock if at that moment the HCS_L is low. Because of this extra
	// clock, the subsequent data read will all be shifted. To avoid this, make sure HCS_L is
	// high when writing to MCU's SPI configurations.										 
	DisplayDisable();				// Set HCS_L high before writing to SPI configurations	
        EPD_SPI_STAT(USE_SPI_CHANNEL)           = 0;
       
        EPD_SPI_CONbits(USE_SPI_CHANNEL).MSTEN  = SPI_MSTEN;
        EPD_SPI_CONbits(USE_SPI_CHANNEL).MODE16 = SPI_MODE;
        EPD_SPI_CONbits(USE_SPI_CHANNEL).CKP    = SPI_CKP;
        EPD_SPI_CONbits(USE_SPI_CHANNEL).CKE    = SPI_CKE;
        EPD_SPI_CONbits(USE_SPI_CHANNEL).SMP    = SPI_SMP;       
    #ifdef __PIC32MX
        EPD_SPI_CONbits(USE_SPI_CHANNEL).MODE32 = 0;
        EPD_SPI_BRG(USE_SPI_CHANNEL) 			= SPI_BAUDRATE;							
        EPD_SPI_CONbits(USE_SPI_CHANNEL).ON 	= SPI_MODULE_ENABLE;
    #else
        EPD_SPI_CON2(USE_SPI_CHANNEL) 			= 0;
        EPD_SPI_CONbits(USE_SPI_CHANNEL).PPRE   = SPI_PPRE; 
        EPD_SPI_CONbits(USE_SPI_CHANNEL).SPRE   = SPI_SPRE; 
        EPD_SPI_STATbits(USE_SPI_CHANNEL).SPIEN = SPI_MODULE_ENABLE;
    #endif 
	DisplayEnable();				// Set HCS_L low after writing to SPI configurations
    
//***********************************************
// SPI	Write
//***********************************************
    #if defined(__PIC32MX__)
		SpiChnPutC(USE_SPI_CHANNEL, data);
        volatile WORD value = (WORD)SpiChnGetC(USE_SPI_CHANNEL);
    #else
		#if (USE_SPI_CHANNEL == 1)
			IFS0bits.EPD_SPI_IF(USE_SPI_CHANNEL) = 0;
			// remove from the buffer if any
			volatile WORD value = EPD_SPI_BUF(USE_SPI_CHANNEL);
			// write
	   	   	EPD_SPI_BUF(USE_SPI_CHANNEL) = data;
	   	   	while( IFS0bits.EPD_SPI_IF(USE_SPI_CHANNEL) == 0 );
		#else		// assumed to be 2
			IFS2bits.EPD_SPI_IF(USE_SPI_CHANNEL) = 0;
			// remove from the buffer if any
			volatile WORD value = EPD_SPI_BUF(USE_SPI_CHANNEL);
			// write
	   	   	EPD_SPI_BUF(USE_SPI_CHANNEL) = data;
	   	   	while( IFS2bits.EPD_SPI_IF(USE_SPI_CHANNEL) == 0 );
		#endif
	#endif

#elif defined ( USE_16BIT_PMP )
	
	DeviceWrite( data );
		
#else

	DeviceWrite( (BYTE)(data>>0) );
	DeviceWrite( (BYTE)(data>>8) );       
       
#endif

}

// ----------------------------------------------------------------------------------
// seDeviceRead()
// 
// ----------------------------------------------------------------------------------
static inline WORD seDeviceRead()
{	
#if defined ( USE_SPI_CHANNEL )
	WORD value;
	
    #if defined(__PIC32MX__)
        // remove from the buffer if any
        value = (WORD)getcSPI1();
        
        // dummy read
        putcSPI1(0x00);
        value = (WORD)getcSPI1();
        
		// dummy read	
        putcSPI1(0x00);
        value = (WORD)getcSPI1();
    #else	
		// remove from the buffer if any
		value = EPD_SPI_BUF(USE_SPI_CHANNEL);
		
		// dummy read
		IFS0bits.EPD_SPI_IF(USE_SPI_CHANNEL) = 0;
		EPD_SPI_BUF(USE_SPI_CHANNEL) = 0;
	   	while(IFS0bits.EPD_SPI_IF(USE_SPI_CHANNEL) == 0){};
		value = EPD_SPI_BUF(USE_SPI_CHANNEL);
		
		// read
		IFS0bits.EPD_SPI_IF(USE_SPI_CHANNEL) = 0;
		EPD_SPI_BUF(USE_SPI_CHANNEL) = 0;
		while(IFS0bits.EPD_SPI_IF(USE_SPI_CHANNEL) == 0){};
		value = EPD_SPI_BUF(USE_SPI_CHANNEL);
	#endif
	
	return value;

#elif defined ( USE_16BIT_PMP )
	
	WORD value = DeviceRead();

	return value;
	
#else

	WORD value;
	
	value  = (WORD)DeviceRead() << 0;
	value |= (WORD)DeviceRead() << 8;

	return value;	
	
#endif

}

// ----------------------------------------------------------------------------------
// seWAIT_HRDY()
// 
// ----------------------------------------------------------------------------------
static inline void __attribute__ ((always_inline)) seWAIT_HRDY( void )
{
#if defined (EPD_BUSY_LINE)	
	
	while ( EPD_BUSY_LINE );
	
#else

	DisplaySetCommand();					// set RS line to low for command
	seDeviceWrite(RD_REG);					// write command
	DisplaySetData();						// set RS line to high for data		
	seDeviceWrite( REG000A_SYSTEM_STATUS );	// write data	
	while ( seDeviceRead() & 0x20 );		// read data	
	
#endif
}

//=============================================================================
//	GFX FUNCTIONS
//=============================================================================

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
    
#if defined( USE_SPI_CHANNEL )  

	BusyLineConfig();
	DisplayResetEnable();               // hold in reset by default
        DisplayResetConfig();               // enable RESET line
        DisplayCmdDataConfig();             // enable RS line
        DisplayDisable();                   // not selected by default
        DisplayConfig();                    // enable chip select line
	Delay10us(20);  

        DisplayResetDisable();              // release from reset

        Delay10us(20);
	
#else
    // use the same PMP/EPMP initialization
    DriverInterfaceInit();
#endif

	DisplayEnable();					// enable S1D13522

	// INIT_SYS_RUN
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( INIT_SYS_RUN );	// write command
	
	// RD_WFM_INFO	
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( RD_WFM_INFO );		// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( (WORD)((DWORD)WFM_ST_ADR >>  0) );
	seDeviceWrite( (WORD)((DWORD)WFM_ST_ADR >> 16) );
		
	// UPD_GDRV_CLR	
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( UPD_GDRV_CLR );	// write command
	
	// WAIT_DSPE_TRG		
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( WAIT_DSPE_TRG );	// write command

        // UPD_SET_IMGADR
        seWAIT_HRDY();
        DisplaySetCommand();                                                 // set RS line to low for command
        seDeviceWrite( UPD_SET_IMGADR );     // write command
        DisplaySetData();                                                    // set RS line to high for data
        seDeviceWrite( (WORD)((DWORD)IMG_BUF_ST_ADR >>  0) );
        seDeviceWrite( (WORD)((DWORD)IMG_BUF_ST_ADR >> 16) );

        // LD_IMG_DSPEADR
        seWAIT_HRDY();
        DisplaySetCommand();                                                 // set RS line to low for command
        seDeviceWrite( LD_IMG_DSPEADR );     // write command

        // PIP_ADRCFG
        seWAIT_HRDY();
        DisplaySetCommand();                                                 // set RS line to low for command
        seDeviceWrite( PIP_ADRCFG ); // write command
        DisplaySetData();                                                    // set RS line to high for data
        seDeviceWrite( (WORD)((DWORD)PIP_BUF_ST_ADR >>  0) );
        seDeviceWrite( (WORD)((DWORD)PIP_BUF_ST_ADR >> 16) );

        // LD_IMG
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG );		// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( SRC_1BIT );


	seWAIT_HRDY();	
	DisplaySetCommand();					// set RS line to low for command
	seDeviceWrite( WR_REG );				// write command
	DisplaySetData();						// set RS line to high for data	
	seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );
	
	DWORD i, Count = ( (DWORD)DISP_VER_RESOLUTION / 16 ) * DISP_HOR_RESOLUTION;
	for ( i = 0; i < Count; i++ )
	{
		seDeviceWrite( 0xffff );
	}

	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_END );	// write command

	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( UPD_FULL );		// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( WFM_INIT );
	
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( WAIT_DSPE_TRG );	// write command	
	
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( WAIT_DSPE_FREND );	// write command
				
				
	// AutoWaveform enable			
	seWAIT_HRDY();
    DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite(RD_REG);
	DisplaySetData();				// set RS line to high for data
	seDeviceWrite(REG0330_UPDATE_BUFFER_CONFIG);	
	DWORD value = seDeviceRead();
	value |= (1<<6);
	value &= 0x7FFF;
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite(WR_REG);
	DisplaySetData();				// set RS line to high for data
	seDeviceWrite(REG0330_UPDATE_BUFFER_CONFIG);
	seDeviceWrite(value);	
					
	DisplayDisable();				// disable S1D13522	

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
	if ( g_ActiveLayer == GFX_CURSOR_LAYER )
	{
		// not available for cursor layer
		return;	
	}
	else
	{	
		WORD pxl = _color;
	
		DisplayEnable();				// enable S1D13522	
			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_AREA );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( SRC_COLOR | g_HostMemAccessCfg );
		seDeviceWrite( x );
		seDeviceWrite( y );
		seDeviceWrite( 1 );
		seDeviceWrite( 1 );
	
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WR_REG );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );
		seDeviceWrite( pxl );

		seWAIT_HRDY();	
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_END );	// write command

		DisplayDisable();				// disable S1D13522	
	}

	g_UpdateRequest = TRUE;	
      
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
#if (COLOR_DEPTH == 1)
    return (1);
#elif (COLOR_DEPTH == 4)
    return (0x0F);
#else
    return (0xFF);
#endif
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
	if ( g_ActiveLayer == GFX_CURSOR_LAYER )
	{
		// not available for cursor layer
		return 1;	
	}
	else
	{	
		WORD pxl    = _color;
		WORD SrcColor = SRC_COLOR;
		WORD Width  = right - left + 1;
		WORD Height = bottom - top + 1;
		DWORD Count = (DWORD)Width * (DWORD)Height;
	
	    if(left > right) 
	        return(1);        //Added to make sure function call is accurate
	    
	    if(bottom < top) 
	        return(1);        //Added to make sure function call is accurate
	        
	#if ( SRC_COLOR == SRC_1BIT )
			pxl = (_color==WHITE) ? 0xFFFF : 0;
			Count = (Count%16) ? (1+Count/16) : (Count/16);
	#else
		if ( _color == WHITE || _color == BLACK )
		{
			pxl = (_color==WHITE) ? 0xFFFF : 0;
			Count = (Count%16) ? (1+Count/16) : (Count/16);
			SrcColor = SRC_1BIT;
		}
		else
		{	
		#if ( SRC_COLOR == SRC_8BIT )				//input packed pixel = 1 byte-per-pixel (8 bpp)
			Count = (Count%2) ? 1+Count/2 : Count/2;						
			pxl &= 0xFF;
            pxl   = (pxl<<8) | (pxl<<0);
		#elif ( SRC_COLOR == SRC_4BIT )				//packed pixel = 4 bpp
			Count = (Count%4) ? 1+Count/4 : Count/4;		
			pxl &= 0x0F;
			pxl   = (pxl<<12) | (pxl<<8) | (pxl<<4) | (pxl<<0);
		#endif
		}
	#endif
	
		DisplayEnable();				// enable S1D13522	
			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_AREA );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( SrcColor | g_HostMemAccessCfg );
		seDeviceWrite( left );
		seDeviceWrite( top );
		seDeviceWrite( Width );
		seDeviceWrite( Height );
	
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WR_REG );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );
	
		while ( Count-- )
		{
			seDeviceWrite( pxl );
		}
	
		seWAIT_HRDY();	
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_END );	// write command
	
		DisplayDisable();				// disable S1D13522	
	
		g_UpdateRequest = TRUE;	

        return 1;	
	}   	
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
#if defined (USE_NONBLOCKING_CONFIG)

	#if defined (EPD_BUSY_LINE)     
	     
	     return EPD_BUSY_LINE;
	     
	#else
	
	     DisplayEnable();                           // enable S1D13522
	     DisplaySetCommand();                       // set RS line to low for command
	     seDeviceWrite(RD_REG);                     // write command
	     DisplaySetData();                          // set RS line to high for data      
	     seDeviceWrite( REG000A_SYSTEM_STATUS );    // write data   
	     WORD val = ( seDeviceRead() & 0x20 )>>5;   // read data
	     DisplayDisable();                          // disable S1D13522  
	     
		 return val;     
	     
	#endif 
		
#else

	#if defined (EPD_BUSY_LINE)	
		seWAIT_HRDY();
	#else
		DisplayEnable();
		seWAIT_HRDY();
		DisplayDisable();
	#endif	
	
		return 0;	   

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
	WORD pxl  = _color;
	
	if ( g_ActiveLayer == GFX_CURSOR_LAYER )
	{
		/* The cursor layer is 4 Bpp */
		
		if ( pxl == WHITE || pxl == BLACK )	
		{
			pxl = (pxl == WHITE) ? 0xffff : 0;
		}
		else
		{			
			pxl   = (pxl<<12) | (pxl<<8) | (pxl<<4) | (pxl<<0);
		}			
	   	
	   	DWORD nWordsToWrite = (DWORD)g_CursorLayerWidth*g_CursorLayerHeight*4/16;
	   	DisplayEnable();				// enable S1D13522

		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_ADRCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( 0 );		   	
	   			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WR_REG );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( REG03DE_CRS_MEM_LOAD );
		
		while( nWordsToWrite-- )
		{
			seDeviceWrite( pxl );
		}	 

		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_ADRCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( 0 );		   	
		    
		DisplayDisable();				// disable S1D13522	
	}
	else 
	{	
		WORD SrcColor = SRC_COLOR;
		DWORD Count = (DWORD)DISP_VER_RESOLUTION * (DWORD)DISP_HOR_RESOLUTION; // if layer is main
		 
		if ( g_ActiveLayer == GFX_PIP_LAYER )
			Count = (DWORD)g_PipLayerHeight * (DWORD)g_PipLayerWidth;
				
	#if ( SRC_COLOR == SRC_1BIT )
			pxl = _color == WHITE ? 0xFFFF : 0;
			Count = (Count%16) ? (1+Count/16) : (Count/16);
	#else
		if ( _color == WHITE || _color == BLACK )
		{
			pxl = _color == WHITE ? 0xFFFF : 0;
			Count = (Count%16) ? (1+Count/16) : (Count/16);
			SrcColor = SRC_1BIT;
		}
		else
		{	
		#if ( SRC_COLOR == SRC_8BIT )				//input packed pixel = 1 byte-per-pixel (8 bpp)
			Count = (Count%2) ? 1+Count/2 : Count/2;						
			pxl &= 0xFF;
            pxl   = (pxl<<8) | (pxl<<0);
		#elif ( SRC_COLOR == SRC_4BIT )				//packed pixel = 4 bpp
			Count = (Count%4) ? 1+Count/4 : Count/4;		
			pxl &= 0x0F;
			pxl   = (pxl<<12) | (pxl<<8) | (pxl<<4) | (pxl<<0);
		#endif
		}
	#endif
				
		DisplayEnable();
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( g_HostMemAccessCfg | SrcColor );

		seWAIT_HRDY();	
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WR_REG );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );
		
		while( Count-- )
		{
			seDeviceWrite( pxl );
		}
	
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_END );	// write command
			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( UPD_FULL );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( WFM_GC8 );
			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WAIT_DSPE_TRG );	// write command
	
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WAIT_DSPE_FREND );	// write command

		DisplayDisable();				// disable S1D13522	
	}	

	g_UpdateRequest = TRUE;		
}

#if !defined(USE_PRIMITIVE_PUTIMAGE) 

#ifdef USE_BITMAP_FLASH
/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
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
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    if ( g_ActiveLayer == GFX_CURSOR_LAYER )
   		return;
    
    FLASH_BYTE          *flashAddress;
    FLASH_WORD          *imageAddress;
    DWORD               bmpTotalSize;
    WORD                sizeX, sizeY;
	
    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
	

    // skip palette
    flashAddress += 4;              // 2 entries at 16bpp
    
	DisplayEnable();				// enable S1D13522		
	
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_AREA );	// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( SRC_1BIT | g_HostMemAccessCfg );
	seDeviceWrite( left );
	seDeviceWrite( top );
	seDeviceWrite( sizeX );
	seDeviceWrite( sizeY );

	seWAIT_HRDY();
	DisplaySetCommand();		// set RS line to low for command
	seDeviceWrite( WR_REG );	// write command
	DisplaySetData();			// set RS line to high for data	
	seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );

    imageAddress = (FLASH_WORD *)flashAddress;
    
    bmpTotalSize = (DWORD)sizeX * (DWORD)sizeY;
    bmpTotalSize += 7;
    bmpTotalSize /= 8;  // 8 bit    
    bmpTotalSize++;
    bmpTotalSize /= 2;  // 16 bit

	while( bmpTotalSize-- )
	{
        seDeviceWrite( *imageAddress++ );
	}	

	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_END );	// write command
	
	DisplayDisable();				// disable S1D13522	

    g_UpdateRequest = TRUE;		
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
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
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    FLASH_BYTE          *flashAddress;
    FLASH_WORD          *imageAddress;
    DWORD               bmpTotalSize;
    WORD                sizeX, sizeY;
	
    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
	
	
    // skip palette
    flashAddress += 32;              // 16 entries at 16bpp

    if ( g_ActiveLayer == GFX_CURSOR_LAYER )
    {	    
	   	WORD x, y;
	   	WORD stride16 = (4*sizeX)%16 ? (4*sizeX)/16 + 1 : (4*sizeX)/16;
	   	DisplayEnable();				// enable S1D13522

		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_ADRCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( 0 );		   	
	   		
        bmpTotalSize = (DWORD)sizeX * (DWORD)sizeY;
        bmpTotalSize++;
        bmpTotalSize /= 2;  // 8 bit    
        bmpTotalSize++;
        bmpTotalSize /= 2;  // 16 bit	
		imageAddress = (FLASH_WORD *)flashAddress;
			   			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WR_REG );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( REG03DE_CRS_MEM_LOAD );
		
		while( bmpTotalSize-- )
		{
			seDeviceWrite( *imageAddress++ );
		}	 

		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_ADRCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( 0 );		   	
		    
		DisplayDisable();				// disable S1D13522	 
	} 
	else
	{
		DisplayEnable();				// enable S1D13522	
			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_AREA );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( SRC_4BIT | g_HostMemAccessCfg );
		seDeviceWrite( left );
		seDeviceWrite( top );
		seDeviceWrite( sizeX );
		seDeviceWrite( sizeY );
	
        bmpTotalSize = (DWORD)sizeX * (DWORD)sizeY;
        bmpTotalSize++;
        bmpTotalSize /= 2;  // 8 bit    
        bmpTotalSize++;
        bmpTotalSize /= 2;  // 16 bit
		
		seWAIT_HRDY();
		DisplaySetCommand();		// set RS line to low for command
		seDeviceWrite( WR_REG );	// write command
		DisplaySetData();			// set RS line to high for data	
		seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );
	
	    imageAddress = (FLASH_WORD *)flashAddress;
	
		while( bmpTotalSize-- )
		{
			seDeviceWrite( *imageAddress++ );
		}	
	
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_END );	// write command
		
		DisplayDisable();				// disable S1D13522	
	}
	g_UpdateRequest = TRUE;		
}

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
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
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    if ( g_ActiveLayer == GFX_CURSOR_LAYER )
    	return;
    		
    FLASH_BYTE          *flashAddress;
    FLASH_WORD          *imageAddress;
    WORD                sizeX, sizeY;
    DWORD               bmpTotalSize;
	
    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
	
    // skip palette
    flashAddress += 512;              // 256 entries at 16bpp

	DisplayEnable();				// enable S1D13522
			
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_AREA );		// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( SRC_8BIT | g_HostMemAccessCfg );
	seDeviceWrite( left );
	seDeviceWrite( top );
	seDeviceWrite( sizeX );
	seDeviceWrite( sizeY );

	//setup Count using specified area size
    bmpTotalSize = (DWORD)sizeX * (DWORD)sizeY;
    bmpTotalSize++;
    bmpTotalSize /= 2;  // 16 bit
	
	seWAIT_HRDY();
	DisplaySetCommand();		// set RS line to low for command
	seDeviceWrite( WR_REG );	// write command
	DisplaySetData();			// set RS line to high for data	
	seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );

    imageAddress = (FLASH_WORD *)flashAddress;

	while( bmpTotalSize-- )
	{
		seDeviceWrite( *imageAddress++ );
	}	
	
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_END );	// write command

	DisplayDisable();				// disable S1D13522	

	g_UpdateRequest = TRUE;		
}
#endif

#ifdef USE_BITMAP_EXTERNAL
/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
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
void PutImage1BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    if ( g_ActiveLayer == GFX_CURSOR_LAYER )
    	return;
    		
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;

    BYTE            lineBuffer[((GetMaxX() + 7) / 8)];
    WORD            *pData16;
    DWORD           bmpTotalSize;
    WORD            sizeX, sizeY;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);
    // Set offset to the image data
	memOffset = sizeof(BITMAP_HEADER) + 4;
    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    bmpTotalSize = (DWORD)sizeX * (DWORD)sizeY;
    bmpTotalSize += 7;
    bmpTotalSize /= 8;    
    
   	DisplayEnable();				// enable S1D13522	
   		
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_AREA );		// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( SRC_1BIT | g_HostMemAccessCfg );
	seDeviceWrite( left );
	seDeviceWrite( top );
	seDeviceWrite( sizeX );
	seDeviceWrite( sizeY );
	
	seWAIT_HRDY();
	DisplaySetCommand();		// set RS line to low for command
	seDeviceWrite( WR_REG );	// write command
	DisplaySetData();			// set RS line to high for data	
	seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );

    // sizeX - the amount of data to get from the external memory
    // sizeY - loop counter for the device write
    while(bmpTotalSize)
    {
        if(bmpTotalSize > (((GetMaxX() + 7) / 8) & 0xFFFE))
            sizeX = ((GetMaxX() + 7) / 8) & 0xFFFE;
        else
            sizeX = bmpTotalSize;

        ExternalMemoryCallback(bitmap, memOffset, sizeX, lineBuffer);
        memOffset += sizeX;
        bmpTotalSize -= sizeX;

        sizeX++;
        sizeX >>= 1;
        pData16 = (WORD *)lineBuffer;

        for(sizeY = 0; sizeY < sizeX; sizeY++)
            seDeviceWrite(*pData16++); 
        
    }


	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_END );		// write command

	DisplayDisable();				// disable S1D13522	

	g_UpdateRequest = TRUE;		
}
/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
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
void PutImage4BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;

    BYTE            lineBuffer[((GetMaxX() + 1) / 2)];
    WORD            *pData16;
    DWORD           bmpTotalSize;
    WORD            sizeX, sizeY;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);
    // Set offset to the image data
	memOffset = sizeof(BITMAP_HEADER) + 32;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;
    
    bmpTotalSize = (DWORD)sizeX * (DWORD)sizeY;
    bmpTotalSize++;
    bmpTotalSize /= 2;    

    if ( g_ActiveLayer == GFX_CURSOR_LAYER )
    {	    
	   	DisplayEnable();				// enable S1D13522

		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_ADRCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( 0 );		   	
	   			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WR_REG );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( REG03DE_CRS_MEM_LOAD );
	
        // sizeX - the amount of data to get from the external memory
        // sizeY - loop counter for the device write
        while(bmpTotalSize)
        {
            if(bmpTotalSize > (((GetMaxX() + 1) / 2) & 0xFFFE))
                sizeX = ((GetMaxX() + 1) / 2) & 0xFFFE;
            else
                sizeX = bmpTotalSize;
    
            ExternalMemoryCallback(bitmap, memOffset, sizeX, lineBuffer);
            memOffset += sizeX;
            bmpTotalSize -= sizeX;
    
            sizeX++;
            sizeX >>= 1;
            pData16 = (WORD *)lineBuffer;
    
            for(sizeY = 0; sizeY < sizeX; sizeY++)
                seDeviceWrite(*pData16++); 
            
        }
	    
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_ADRCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( 0 );		   	
		    
		DisplayDisable();				// disable S1D13522	 
	} 
	else
	{
	   	DisplayEnable();				// enable S1D13522		
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_AREA );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( SRC_4BIT | g_HostMemAccessCfg );
		seDeviceWrite( left );
		seDeviceWrite( top );
		seDeviceWrite( sizeX );
		seDeviceWrite( sizeY );
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( WR_REG );		// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );
	
        // sizeX - the amount of data to get from the external memory
        // sizeY - loop counter for the device write
        while(bmpTotalSize)
        {
            if(bmpTotalSize > (((GetMaxX() + 1) / 2) & 0xFFFE))
                sizeX = ((GetMaxX() + 1) / 2) & 0xFFFE;
            else
                sizeX = bmpTotalSize;
    
            ExternalMemoryCallback(bitmap, memOffset, sizeX, lineBuffer);
            memOffset += sizeX;
            bmpTotalSize -= sizeX;
    
            sizeX++;
            sizeX >>= 1;
            pData16 = (WORD *)lineBuffer;
    
            for(sizeY = 0; sizeY < sizeX; sizeY++)
                seDeviceWrite(*pData16++); 
            
        }
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_END );	// write command
	
		DisplayDisable();				// disable S1D13522	
	}
	g_UpdateRequest = TRUE;		
}

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
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
void PutImage8BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch, PUTIMAGE_PARAM *pPartialImageData)
{
    if ( g_ActiveLayer == GFX_CURSOR_LAYER )
    	return;
    		
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;

    BYTE            lineBuffer[GetMaxX()];
    WORD            *pData16;
    DWORD           bmpTotalSize;
    WORD            sizeX, sizeY;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data
	memOffset = sizeof(BITMAP_HEADER) + 512;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;
    
    bmpTotalSize = (DWORD)sizeX * (DWORD)sizeY;

   	DisplayEnable();				// enable S1D13522
   			
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_AREA );	// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( SRC_8BIT | g_HostMemAccessCfg );
	seDeviceWrite( left );
	seDeviceWrite( top );
	seDeviceWrite( sizeX );
	seDeviceWrite( sizeY );
	
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( WR_REG );		// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );

    // sizeX - the amount of data to get from the external memory
    // sizeY - loop counter for the device write
    while(bmpTotalSize)
    {
        if(bmpTotalSize > (GetMaxX() & 0xFFFE))
            sizeX = GetMaxX() & 0xFFFE;
        else
            sizeX = bmpTotalSize;

        ExternalMemoryCallback(bitmap, memOffset, sizeX, lineBuffer);
        memOffset += sizeX;
        bmpTotalSize -= sizeX;

        sizeX++;
        sizeX >>= 1;
        pData16 = (WORD *)lineBuffer;

        for(sizeY = 0; sizeY < sizeX; sizeY++)
            seDeviceWrite(*pData16++); 
        
    }

	
	seWAIT_HRDY();	
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( LD_IMG_END );		// write command
	
	DisplayDisable();				// disable S1D13522	
	g_UpdateRequest = TRUE;		
}
#endif
#endif //#if !defined(USE_PRIMITIVE_PUTIMAGE) 

//---------------------------------------------------------------------------
//	GFX_DRIVER_CreateLayer()
//---------------------------------------------------------------------------
void GFX_DRIVER_CreateLayer( GFX_LAYER layer, WORD config, WORD xStart, WORD yStart, WORD xEnd, WORD yEnd )
{
	WORD width, height;

    width = xEnd - xStart;
    width++;
    height = yEnd - yStart;
    height++;

    if ( layer == GFX_PIP_LAYER )
	{
		g_PipLayerWidth  = width;
		g_PipLayerHeight = height;
		g_PipConfig      = 0x01C0 & config;
		g_PipConfig     |= (0x000F & config) << 12;	
		g_PipConfig     |= 0x0020;
		
		DisplayEnable();				// enable S1D13522	
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( PIP_ENABLE );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( g_PipConfig );
		seDeviceWrite( width );
		seDeviceWrite( height );
		seDeviceWrite( 0 );
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( PIP_XYSETUP );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( xStart );
		seDeviceWrite( yStart );
	
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( PIP_DISABLE );	// write command
		
		DisplayDisable();				// disable S1D13522			
	}
	else if ( layer == GFX_CURSOR_LAYER )		 
	{
#if ( DISP_ORIENTATION == 0 || DISP_ORIENTATION == 180 )
        g_CursorLayerWidth  = width;     //< Width must be multiple by 2 bytes or 4 pixels for 0 and 180 degree rotation
        g_CursorLayerHeight = height;
#else 
        g_CursorLayerWidth  = height;   //< Height must be multiple by 2 bytes or 4 pixels for 90 and 270 degree rotation
        g_CursorLayerHeight = width;     
#endif
		g_CursorConfig      = 0x01C0 & config;		// Transparency enable, Cursor Pixel Output Value control
		g_CursorConfig     |= (0x000F & config) << 12;	// Transparency value		
		g_CursorConfig     |= 0x0020;
		
		DisplayEnable();				// enable S1D13522
				
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_MAINCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( g_CursorConfig );
		seDeviceWrite( g_CursorLayerWidth );
		seDeviceWrite( g_CursorLayerHeight );
		seDeviceWrite( 0 ); 			
		
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_XYSETUP );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( xStart );
		seDeviceWrite( yStart );		
	
		DisplayDisable();				// disable S1D13522			
	}	
	
	g_UpdateRequest = TRUE;		
}
	
//---------------------------------------------------------------------------
//	GFX_DRIVER_MoveLayer()
//---------------------------------------------------------------------------	
void GFX_DRIVER_MoveLayer( GFX_LAYER Layer, WORD X, WORD Y )
{
	if ( Layer == GFX_PIP_LAYER )
	{
		DisplayEnable();				// enable S1D13522		
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( PIP_XYSETUP );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( X );
		seDeviceWrite( Y );
	
		DisplayDisable();				// disable S1D13522	
	}
	else if ( Layer == GFX_CURSOR_LAYER )		 
	{
		DisplayEnable();				// enable S1D13522	
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_XYSETUP );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( X );
		seDeviceWrite( Y );
		
		DisplayDisable();				// disable S1D13522	
	}	
	g_UpdateRequest = TRUE;		
}
	
//---------------------------------------------------------------------------
//	GFX_DRIVER_ShowLayer()
//---------------------------------------------------------------------------	
void GFX_DRIVER_ShowLayer( GFX_LAYER Layer )
{
	if ( Layer == GFX_PIP_LAYER )
	{
		DisplayEnable();				// enable S1D13522	
			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( PIP_ENABLE );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( g_PipConfig );
		seDeviceWrite( g_PipLayerWidth );
		seDeviceWrite( g_PipLayerHeight );
		seDeviceWrite( 0 );
	
		DisplayDisable();				// disable S1D13522	
	}
	else if ( Layer == GFX_CURSOR_LAYER )		 
	{
		DisplayEnable();				// enable S1D13522	
			
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_MAINCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( g_CursorConfig | 1 );
		seDeviceWrite( g_CursorLayerWidth );
		seDeviceWrite( g_CursorLayerHeight );
		seDeviceWrite( 0 ); 
		
		DisplayDisable();				// disable S1D13522	
	}	
	g_UpdateRequest = TRUE;		
}
	
//---------------------------------------------------------------------------
//	GFX_DRIVER_HideLayer()
//---------------------------------------------------------------------------	
void GFX_DRIVER_HideLayer( GFX_LAYER Layer )
{
	if ( Layer == GFX_PIP_LAYER )
	{
		DisplayEnable();				// enable S1D13522		
		
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( PIP_DISABLE );	// write command
	
		DisplayDisable();				// disable S1D13522	
	}
	else if ( Layer == GFX_CURSOR_LAYER )		 
	{
		DisplayEnable();				// enable S1D13522
				
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( CSR_MAINCFG );	// write command
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( g_CursorConfig ); 
		seDeviceWrite( g_CursorLayerWidth );
		seDeviceWrite( g_CursorLayerHeight );
		seDeviceWrite( 0 ); 
		
		DisplayDisable();				// disable S1D13522	
	}	
	g_UpdateRequest = TRUE;		
}
	
//---------------------------------------------------------------------------
//	GFX_DRIVER_ActivateLayer()
//---------------------------------------------------------------------------	
void GFX_DRIVER_ActivateLayer( GFX_LAYER Layer )
{
	if ( Layer == GFX_MAIN_LAYER )
	{
		g_ActiveLayer = GFX_MAIN_LAYER;
		g_HostMemAccessCfg &= ~WRITE_TO_PIP;
	}
	else if ( Layer == GFX_PIP_LAYER )
	{
		g_ActiveLayer = GFX_PIP_LAYER;
		g_HostMemAccessCfg |=  WRITE_TO_PIP;
	}
	else if ( Layer == GFX_CURSOR_LAYER )		 
	{
		g_ActiveLayer = GFX_CURSOR_LAYER;
	}	
	g_UpdateRequest = TRUE;		
}	

//---------------------------------------------------------------------------
//	GFX_DRIVER_RunSystem()
//---------------------------------------------------------------------------
void GFX_DRIVER_RunSystem( void )
{
	DisplayEnable();				// enable S1D13522		
	
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( RUN_SYS );		// write command

	DisplayDisable();				// disable S1D13522	
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_Standby()
//---------------------------------------------------------------------------
void GFX_DRIVER_Standby( void )
{
	DisplayEnable();				// enable S1D13522		
	seWAIT_HRDY();
	
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( STBY );			// write command

	DisplayDisable();				// disable S1D13522	
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_Sleep()
//---------------------------------------------------------------------------
void GFX_DRIVER_Sleep( void )
{
	DisplayEnable();				// enable S1D13522		
	seWAIT_HRDY();
	
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( SLP );				// write command

	DisplayDisable();				// disable S1D13522	
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_InitRotmode()
//---------------------------------------------------------------------------
void GFX_DRIVER_InitRotmode( WORD RotMode )
{
	DisplayEnable();				// enable S1D13522
			
	seWAIT_HRDY();
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( INIT_ROTMODE );	// write command
	DisplaySetData();				// set RS line to high for data	
	seDeviceWrite( RotMode );

	DisplayDisable();				// disable S1D13522	
}



//---------------------------------------------------------------------------
// GFX_DRIVER_LoadImage()
// If specified, image data is sent in pData (otherwise pData=NULL).
//---------------------------------------------------------------------------
void GFX_DRIVER_LoadImage( WORD HostOptions, WORD xStart, WORD yStart, WORD xEnd, WORD yEnd, const WORD* pData )
{
	if ( g_ActiveLayer == GFX_CURSOR_LAYER )
	{
        if ((HostOptions & 0x0030) != SRC_4BIT)    //packed pixel is not 4 bpp
            return;
		
            DWORD nWordsToWrite = (((DWORD)g_CursorLayerHeight*g_CursorLayerWidth*4)+15)/16;                 
         
            DisplayEnable();                     // enable S1D13522
          
           seWAIT_HRDY();
           DisplaySetCommand();            // set RS line to low for command
           seDeviceWrite( CSR_ADRCFG );    // write command
           DisplaySetData();                    // set RS line to high for data 
           seDeviceWrite( 0 );  
                           
           seWAIT_HRDY();
     
           DisplaySetCommand();            // set RS line to low for command
           seDeviceWrite( WR_REG );        // write command
     
           DisplaySetData();                    // set RS line to high for data 
           seDeviceWrite( REG03DE_CRS_MEM_LOAD );

           while( nWordsToWrite-- )
           {
                seDeviceWrite( *pData++ );
           }              
           seWAIT_HRDY();
           DisplaySetCommand();            // set RS line to low for command
           seDeviceWrite( CSR_ADRCFG );    // write command
           DisplaySetData();                    // set RS line to high for data 
           seDeviceWrite( 0 );  
                    
           DisplayDisable();                    // disable S1D13522       
	}
	else
	{
        WORD width;
        WORD height;

        width = xEnd - xStart;
        width++;
        height = yEnd - yStart;
        height++;

		DisplayEnable();				// enable S1D13522		
		seWAIT_HRDY();
		
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_AREA );	// write command
	
		DisplaySetData();				// set RS line to high for data	
		seDeviceWrite( HostOptions | g_HostMemAccessCfg );
		seDeviceWrite( xStart );
		seDeviceWrite( yStart );
		seDeviceWrite( width );
		seDeviceWrite( height );
	
		if ( pData )					//if no data specified, skip image load (data write)
		{
			//setup Count using specified area size
			DWORD Count = (DWORD)width * (DWORD)height;
	
			if ((HostOptions & 0x0030) == SRC_8BIT)		//input packed pixel = 1 byte-per-pixel (8 bpp)
				Count = (Count%2) ? (1+Count/2) : (Count/2);	
			else if ((HostOptions & 0x0030) == SRC_4BIT)	//packed pixel = 4 bpp
				Count = (Count%4) ? (1+Count/4) : (Count/4);
			else if ((HostOptions & 0x0030) == SRC_1BIT)	//packed pixel = 1 bpp
				Count = (Count%16) ? (1+Count/16) : (Count/16);
	
			seWAIT_HRDY();
			DisplaySetCommand();		// set RS line to low for command
			seDeviceWrite( WR_REG );	// write command
	
			DisplaySetData();			// set RS line to high for data	
			seDeviceWrite( REG0154_HOST_MEMORY_ACCESS_PORT );
	
			while( Count-- )
			{
				seDeviceWrite( *pData++ );
			}	
		}
	
		seWAIT_HRDY();
		DisplaySetCommand();			// set RS line to low for command
		seDeviceWrite( LD_IMG_END );	// write command
	
		DisplayDisable();				// disable S1D13522	
	}
	g_UpdateRequest = TRUE;		
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_UpdateEpd()
//---------------------------------------------------------------------------
void GFX_DRIVER_UpdateEpd( WORD UpdateOptions, WORD xStart, WORD yStart, WORD xEnd, WORD yEnd )
{
    WORD value;
    WORD width, height;

    DisplayEnable();							// enable S1D13522
	seWAIT_HRDY();
    DisplaySetCommand();						// set RS line to low for command
	seDeviceWrite(RD_REG);
	DisplaySetData();							// set RS line to high for data
	seDeviceWrite(REG0338_DISPLAY_BUSY_STATUS);	
	value = seDeviceRead();
	DisplayDisable();							// disable S1D13522	
	
	value &= 0x21;
	
	if ( UpdateOptions & GFX_UPDATE_AS_IT_DRAWS )
	{	
		if ( value != 0x20 )
			return;
			
		UpdateOptions |= GFX_UPDATE_NO_FLASH;
	}
	else
	{
		if ( value != 0x20 )
		{
			DisplayEnable();					// enable S1D13522
					
			seWAIT_HRDY();
			DisplaySetCommand();				// set RS line to low for command
			seDeviceWrite( WAIT_DSPE_LUTFREE );	// write command
			
			DisplayDisable();					// disable S1D13522				
		}		
	}			
	
	width = xEnd - xStart;
    width++;                                    // zero indexed
    height = yEnd - yStart;
    height++;                                   // zero indexed

    if ( UpdateOptions & GFX_UPDATE_NO_FLASH )
	{	
		DisplayEnable();						// enable S1D13522	
		
		seWAIT_HRDY();
		DisplaySetCommand();					// set RS line to low for command
		seDeviceWrite( UPD_PART_AREA );			// write command
		DisplaySetData();						// set RS line to high for data	
		seDeviceWrite( 0 );
		seDeviceWrite( xStart );
		seDeviceWrite( yStart );
		seDeviceWrite( width );
		seDeviceWrite( height );
		
		DisplayDisable();						// disable S1D13522			
	}	
	else if ( UpdateOptions & GFX_UPDATE_HIGH_QUALITY )
	{
		DisplayEnable();						// enable S1D13522		
		
		seWAIT_HRDY();
		DisplaySetCommand();					// set RS line to low for command
		seDeviceWrite( UPD_FULL_AREA );			// write command
		DisplaySetData();						// set RS line to high for data	
		seDeviceWrite( 0 );
		seDeviceWrite( xStart );
		seDeviceWrite( yStart );
		seDeviceWrite( width );
		seDeviceWrite( height );
	
		DisplayDisable();						// disable S1D13522			
	}
	
	if ( UpdateOptions & (GFX_WAIT_IMAGE_UPDATED | GFX_WAIT_IMAGE_DISPLAYED)  )
	{
		DisplayEnable();						// enable S1D13522		
		
		seWAIT_HRDY();	
		DisplaySetCommand();					// set RS line to low for command
		seDeviceWrite( WAIT_DSPE_TRG );			// write command
		
		DisplayDisable();
	}
	
	if ( UpdateOptions & GFX_WAIT_IMAGE_DISPLAYED )
	{	
		DisplayEnable();						// enable S1D13522	
		
		seWAIT_HRDY();
		DisplaySetCommand();					// set RS line to low for command
		seDeviceWrite( WAIT_DSPE_FREND );		// write command
		
		DisplayDisable();						// disable S1D13522		
	}
	
}	

//---------------------------------------------------------------------------
//	GFX_DRIVER_IsUpdateRequested()
//---------------------------------------------------------------------------
BOOL GFX_DRIVER_IsUpdateRequested(void)
{
	if ( g_UpdateRequest )
	{
		g_UpdateRequest = FALSE;
		
		return TRUE;
	}
	
	return FALSE;
}


//---------------------------------------------------------------------------
//** Update Related Functions for GOL Autoupdate enabled and special cases **
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Definitions and local functions
//---------------------------------------------------------------------------
#define DISPLAY_STATUS_TIRGGER_BUSY 	(1 << 0)
#define LUT_CONFLICT_DETECT				(1 << 7)
#define UPDATE_RECT_MODE_MASK			(3 <<12)
#define UPDATE_RECT_MODE_FULL           0x0000
#define UPDATE_RECT_MODE_HOST           0x1000
#define UPDATE_RECT_MODE_SPEC           0x2000

//---------------------------------------------------------------------------
// GFX_DRIVER_RdReg()
// Returns the register value.
//---------------------------------------------------------------------------
static WORD GFX_DRIVER_ReadRegister( WORD index ) 
{

    WORD value;
 
	index &= ~1;			// index must be 2 bytes aligned

    DisplayEnable();		// enable S1D13522

    DisplaySetCommand();	// set RS line to low for command
	seDeviceWrite(RD_REG);
	DisplaySetData();		// set RS line to high for data
	seDeviceWrite(index);	
	value = seDeviceRead();

	DisplayDisable();		// disable S1D13522

	return value;
}


//---------------------------------------------------------------------------
//	GFX_DRIVER_WrReg()
//---------------------------------------------------------------------------
static void GFX_DRIVER_WriteRegister(WORD index, WORD value)
{	
	if ( (index&1) == 0 )		// index must be 2 bytes aligned
	{	
		DisplayEnable();		// enable S1D13522
		
		seWAIT_HRDY();
		DisplaySetCommand();	// set RS line to low for command
		seDeviceWrite(WR_REG);
		DisplaySetData();		// set RS line to high for data
		seDeviceWrite(index);
		seDeviceWrite(value);

		DisplayDisable();		// disable S1D13522
	}
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_WaitDisplayEngineOperationComplete()
//---------------------------------------------------------------------------
static void GFX_DRIVER_WaitDisplayEngineOperationComplete( void )
{
	DisplayEnable();				// enable S1D13522		
	seWAIT_HRDY();
	
	DisplaySetCommand();			// set RS line to low for command
	seDeviceWrite( WAIT_DSPE_TRG );	// write command

	DisplayDisable();				// disable S1D13522	
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_WaitDisplayEngineMaskedLUTFree()
//---------------------------------------------------------------------------
static void GFX_DRIVER_WaitDisplayEngineMaskedLUTFree( WORD LutMask )
{
	DisplayEnable();					// enable S1D13522		
	seWAIT_HRDY();
	
	DisplaySetCommand();				// set RS line to low for command
	seDeviceWrite( WAIT_DSPE_MLUTFREE );	// write command

	DisplaySetData();					// set RS line to high for data	
	seDeviceWrite( LutMask );

	DisplayDisable();					// disable S1D13522	
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_IsDisplayEngineTriggerBusy()
//---------------------------------------------------------------------------
static WORD GFX_DRIVER_IsDisplayEngineTriggerBusy(void)
{
	DisplayEnable();				// enable S1D13522		
    seWAIT_HRDY();
    return GFX_DRIVER_ReadRegister(REG0338_DISPLAY_BUSY_STATUS) & DISPLAY_STATUS_TIRGGER_BUSY; 
}
//---------------------------------------------------------------------------
//	GFX_DRIVER_CollisionOccurred()
//---------------------------------------------------------------------------
static WORD GFX_DRIVER_CollisionOccurred(void)
{
	DisplayEnable();				// enable S1D13522		
    seWAIT_HRDY();
    return GFX_DRIVER_ReadRegister(REG033A_DISPLAY_INTERRUPT_RAW_STATUS) & LUT_CONFLICT_DETECT; 
}
//---------------------------------------------------------------------------
//	GFX_DRIVER_DisplayEngineGetAreaUpdate(startx, starty, endx, endy)
//---------------------------------------------------------------------------
static void GFX_DRIVER_DisplayEngineGetAreaUpdate(WORD *startx, WORD *starty, WORD *endx, WORD *endy)
{
    WORD updateMode;

	DisplayEnable();				// enable S1D13522		
    seWAIT_HRDY();
    updateMode = GFX_DRIVER_ReadRegister(REG0334_DISPLAY_CONTROL_TRIGGER) & UPDATE_RECT_MODE_MASK;

    switch(updateMode)
    {
    case UPDATE_RECT_MODE_FULL:
        *startx = 0;
        *starty = 0;
        *endx = GetMaxX();
        *endy = GetMaxY();
        break;

    case UPDATE_RECT_MODE_HOST:
        *startx = GFX_DRIVER_ReadRegister(REG0388_HOST_PXL_RECT_XSTART);
        *starty = GFX_DRIVER_ReadRegister(REG038A_HOST_PXL_RECT_YSTART);
        *endx = *startx + GFX_DRIVER_ReadRegister(REG038C_HOST_PXL_RECT_XEND - 1);
        *endy = *starty + GFX_DRIVER_ReadRegister(REG038E_HOST_PXL_RECT_YEND - 1);
        break;

    case UPDATE_RECT_MODE_SPEC:
        *startx = GFX_DRIVER_ReadRegister(REG0380_AREA_UPD_PXL_RECT_XSTART);
        *starty = GFX_DRIVER_ReadRegister(REG0382_AREA_UPD_PXL_RECT_YSTART);
        *endx = *startx + GFX_DRIVER_ReadRegister(REG0384_AREA_UPD_PXL_RECT_XEND - 1);
        *endy = *starty + GFX_DRIVER_ReadRegister(REG0386_AREA_UPD_PXL_RECT_YEND - 1);
        break;

    default:
        *startx = 0;
        *starty = 0;
        *endx = 0;
        *endy = 0;
        break;
    }
}

//---------------------------------------------------------------------------
//	WORD GFX_DRIVER_CheckOverlap(WORD start, WORD end, WORD startOverlap, WORD endOverlap)
//---------------------------------------------------------------------------
static WORD GFX_DRIVER_CheckOverlap(WORD start, WORD end, WORD startOverlap, WORD endOverlap)
{
	if ((end < startOverlap) || (start > endOverlap))
		return 0;

	return 1;
}


//---------------------------------------------------------------------------
//	GFX_DRIVER_OverlapUpdateArea(startx, starty, endx, endy)
//---------------------------------------------------------------------------
static WORD GFX_DRIVER_OverlapUpdateArea(WORD startx, WORD starty, WORD endx, WORD endy)
{
    WORD updateAreaStartX;
    WORD updateAreaStartY;
    WORD updateAreaEndX;
    WORD updateAreaEndY;
    WORD overlap;

    if(GFX_DRIVER_IsDisplayEngineTriggerBusy() == 0)
        return 0;

    GFX_DRIVER_DisplayEngineGetAreaUpdate(  &updateAreaStartX,
                                            &updateAreaStartY,
                                            &updateAreaEndX,
                                            &updateAreaEndY);

    overlap = GFX_DRIVER_CheckOverlap(startx, endx, updateAreaStartX, updateAreaEndX);
    overlap &= GFX_DRIVER_CheckOverlap(starty, endy, updateAreaStartY, updateAreaEndY);

    return overlap;
}


//---------------------------------------------------------------------------
// **                            Public functions                          **
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	GFX_DRIVER_AutoDisable(void)
//---------------------------------------------------------------------------
void GFX_DRIVER_AutoUpdDisable(void)
{
	DefAutoUpd = 0;
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_AutoFull(void)
//---------------------------------------------------------------------------
void GFX_DRIVER_AutoUpdFull(void)
{
	DefAutoUpd = 1;
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_AutoPart(void)
//---------------------------------------------------------------------------
void GFX_DRIVER_AutoUpdPart(void)
{
	DefAutoUpd = 2;
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_CollisionCheckAndRepair(void)
//---------------------------------------------------------------------------
void GFX_DRIVER_CollisionCheckAndRepair(void)
{
    WORD updateAreaStartX;
    WORD updateAreaStartY;
    WORD updateAreaEndX;
    WORD updateAreaEndY;
    WORD lutStatus;


    if(GFX_DRIVER_IsDisplayEngineTriggerBusy())
        GFX_DRIVER_WaitDisplayEngineOperationComplete();


    while(GFX_DRIVER_CollisionOccurred())
    {
        GFX_DRIVER_WriteRegister(REG033C_DISPLAY_INTERRUPT_STATUS, LUT_CONFLICT_DETECT);

        lutStatus = GFX_DRIVER_ReadRegister(REG0336_LOOKUP_TABLE_STATUS);

        GFX_DRIVER_WaitDisplayEngineMaskedLUTFree(lutStatus);


        GFX_DRIVER_DisplayEngineGetAreaUpdate(  &updateAreaStartX,
                                                &updateAreaStartY,
                                                &updateAreaEndX,
                                                &updateAreaEndY);

		GFX_DRIVER_UpdateEpd(	GFX_UPDATE_NO_FLASH|GFX_WAIT_IMAGE_UPDATED,
                                updateAreaStartX,
                                updateAreaStartY,
                                updateAreaEndX,
                                updateAreaEndY);

		GFX_DRIVER_WaitDisplayEngineOperationComplete();
    }
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_SetupDrawUpdate(WORD startx, WORD starty, WORD endx, WORD endy)
//---------------------------------------------------------------------------
void GFX_DRIVER_SetupDrawUpdate(WORD startx, WORD starty, WORD endx, WORD endy)
{
    if(GFX_DRIVER_OverlapUpdateArea(startx, starty, endx, endy) == 0)
        return;

    GFX_DRIVER_WaitDisplayEngineOperationComplete();
}

//---------------------------------------------------------------------------
//	GFX_DRIVER_CompleteDrawUpdate(WORD startx, WORD starty, WORD endx, WORD endy)
//---------------------------------------------------------------------------
void GFX_DRIVER_CompleteDrawUpdate(WORD startx, WORD starty, WORD endx, WORD endy)
{
	GFX_DRIVER_CollisionCheckAndRepair();
	
	DisplayEnable();					// enable S1D13522
	seWAIT_HRDY();
	DisplaySetCommand();				// set RS line to low for command
	seDeviceWrite( WAIT_DSPE_LUTFREE );	// write command
	DisplayDisable();					// disable S1D13522	
			
	if (DefAutoUpd==1)		// Full Update
	{
		GFX_DRIVER_UpdateEpd(	GFX_UPDATE_HIGH_QUALITY,
                                startx,
                                starty,
                                endx,
                                endy);
	}
	else if (DefAutoUpd==2)	// Partial Update
	{
		GFX_DRIVER_UpdateEpd(	GFX_UPDATE_NO_FLASH,
                                startx,
                                starty,
                                endx,
                                endy);
	}
}
	
#endif //GFX_USE_DISPLAY_CONTROLLER_S1D13522
