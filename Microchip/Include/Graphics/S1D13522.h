/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Epson S1D13522 EPD controllers driver
 *****************************************************************************
 * FileName:        S1D13522.h
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
 *
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *****************************************************************************/
#include "HardwareProfile.h" 
#ifndef _S1D13522_H
    #define _S1D13522_H

#if defined ( USE_SPI_CHANNEL )
	#include "drv_spi.h"
	extern const DRV_SPI_INIT_DATA SPI_Init_Data;  
#endif

	#define AutoUpdFull    (DefAutoUpd = 0)
	#define AutoUpdPart    (DefAutoUpd = 1)

    #ifdef USE_TRANSITION_EFFECTS
        #include "Transitions.h"
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
* PARAMETERS VALIDATION
*********************************************************************/

    #if (COLOR_DEPTH != 1) && (COLOR_DEPTH != 4) && (COLOR_DEPTH != 8)
        #error This driver supports 1, 4 and 8 BPP only.
    #endif
    #if (DISP_HOR_RESOLUTION % 8) != 0
        #error Horizontal resolution must be divisible by 8.
    #endif
    #if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180) && (DISP_ORIENTATION != 90) && (DISP_ORIENTATION != 270)
        #error The display orientation selected is not supported. It can be only 0,90,180 or 270.
    #endif


#define GFX_UPDATE_AS_IT_DRAWS 			0x0001
#define GFX_UPDATE_HIGH_QUALITY			0x0002 
#define GFX_WAIT_IMAGE_UPDATED			0x0004
#define GFX_UPDATE_NO_FLASH				0x0008
#define GFX_WAIT_IMAGE_DISPLAYED		0x0010


/*********************************************************************
* Overview: S1D13522 registers definitions.
*********************************************************************/
	#define REG0000_REVISION_CODE							0x0000     // Revision Code Register [READONLY]
	#define REG0002_PRODUCT_CODE							0x0002     // Product Code Register [READONLY]
	#define REG0004_CONFIG_PIN_READ_VALUE					0x0004     // Config Pin Read Value Register [READONLY]
	#define REG0006_POWER_SAVE_MODE							0x0006     // Power Save Mode Register
	#define REG000A_SYSTEM_STATUS							0x000A     // System Status Register

	#define REG0010_PLL_CONFIG_0							0x0010     // PLL Configuration Register 0
	#define REG0012_PLL_CONFIG_1							0x0012     // PLL Configuration Register 1
	#define REG0014_PLL_CONFIG_2							0x0014     // PLL Configuration Register 2
	#define REG0016_CLOCK_CONFIG							0x0016     // Clock Configuration Register
	#define REG0018_PIXEL_CLOCK_CONFIG						0x0018     // Pixel Clock Configuration Register
	#define REG001A_I2C_CLOCK								0x001A     // I2C Clock Configuration Register

	#define REG014C_PACKED_PIXEL_RECT_X_START				0x014C     // Packed Pixel Rectangular X-Start Register
	#define REG014E_PACKED_PIXEL_RECT_Y_START				0x014E     // Packed Pixel Rectangular Y-Start Register
	#define REG0150_PACKED_PIXEL_RECT_WIDTH					0x0150     // Packed Pixel Rectangular Width Register
	#define REG0152_PACKED_PIXEL_RECT_HEIGHT				0x0152     // Packed Pixel Rectangular Height Register
	#define REG0154_HOST_MEMORY_ACCESS_PORT					0x0154     // Host Memory Access Port Register

	#define REG0250_GPIO_CONFIG								0x0250     // GPIO Configuration Register
	#define REG0252_GPIO_STATUS_CONTROL						0x0252     // GPIO Status/Control Register
	#define REG0254_GPIO_IRQ_ENABLE							0x0254     // GPIO Interrupt Enable Register
	#define REG0256_GPIO_IRQ_STATUS							0x0256     // GPIO Interrupt Status Register
	
	#define REG0330_UPDATE_BUFFER_CONFIG					0x0330     // Update Buffer Configuration Register
	#define REG0334_DISPLAY_CONTROL_TRIGGER					0x0334     // Display Engine Control/Trigger Register
	#define REG0336_LOOKUP_TABLE_STATUS						0x0336     // Lookup Table Status Register [READONLY]
	#define REG0338_DISPLAY_BUSY_STATUS						0x0338     // Display Engine Busy Status Register
	#define REG033A_DISPLAY_INTERRUPT_RAW_STATUS			0x033A     // Display Engine Interrupt Raw Status Register
	
	#define REG033C_DISPLAY_INTERRUPT_STATUS				0x033C     // Display Engine Interrupt Masked Status Register
	
	#define REG0380_AREA_UPD_PXL_RECT_XSTART				0x0380
	#define REG0382_AREA_UPD_PXL_RECT_YSTART				0x0382
	#define REG0384_AREA_UPD_PXL_RECT_XEND					0x0384
	#define REG0386_AREA_UPD_PXL_RECT_YEND					0x0386
	
	#define REG0388_HOST_PXL_RECT_XSTART					0x0388
	#define REG038A_HOST_PXL_RECT_YSTART					0x038A
	#define REG038C_HOST_PXL_RECT_XEND						0x038C
	#define REG038E_HOST_PXL_RECT_YEND						0x038E
		
	#define REG03DE_CRS_MEM_LOAD							0x03DE
	
/*********************************************************************
* Overview: S1D13522 commands definitions.
*********************************************************************/
	#define RUN_SYS				0x02
	
	#define STBY				0x04
	#define SLP					0x05
	#define INIT_SYS_RUN		0x06
	
	#define INIT_ROTMODE		0x0B
	
	#define RD_REG				0x10
	#define WR_REG				0x11
	
	#define PIP_DISABLE			0x14
	#define PIP_ENABLE			0x15
	#define PIP_ADRCFG			0x16
	#define PIP_XYSETUP			0x17
	#define CSR_MAINCFG			0x18
	#define CSR_XYSETUP			0x19
	#define CSR_ADRCFG			0x1A
	
	#define BST_WR_SDR			0x1D
	#define BST_END_SDR			0x1E
	
	#define LD_IMG				0x20
	#define LD_IMG_AREA			0x22
	#define LD_IMG_END			0x23
	#define LD_IMG_SETADR		0x25
	#define LD_IMG_DSPEADR		0x26
	
	#define WAIT_DSPE_TRG		0x28
	#define WAIT_DSPE_FREND		0x29
	#define WAIT_DSPE_LUTFREE	0x2A
	#define WAIT_DSPE_MLUTFREE	0x2B
	
	#define RD_WFM_INFO			0x30
	
	//#define UPD_INIT			0x32
	#define UPD_FULL			0x33
	#define UPD_FULL_AREA		0x34
	#define UPD_PART			0x35
	#define UPD_PART_AREA		0x36
	#define UPD_GDRV_CLR		0x37
	#define UPD_SET_IMGADR		0x38

#ifndef FALSE
#define FALSE 					0
#endif
#ifndef TRUE
#define TRUE 					1
#endif
//===============================================================================
// Bit Field Defines
//===============================================================================

// defines for seInitRotMode()
#define ROTATE_0			0x0000			// Sets rotation select bits (bits 9-8)
#define ROTATE_90			0x0100
#define ROTATE_180			0x0200
#define ROTATE_270			0x0300



// defines for GFX_DRIVER_CreateLayer()
#define GFX_LAYER_TRANS_EN		0x0100			// Sets Cursor transparency enable bit (bit 8)

#define GFX_LAYER_OUTPUT_NORMAL	0x0000			// No change to Cursor Pixel value
#define GFX_LAYER_OUTPUT_XOR	0x0040			// (Image buffer data) XOR (Cursor pixel value)
#define GFX_LAYER_OUTPUT_XORNOT	0x0080			// (Image buffer data) XOR ~(Cursor pixel value)
#define GFX_LAYER_OUTPUT_NOT	0x00C0			// ~(Cursor pixel value)
	   
//defines for creating seLdImgHost() and seLdImgHostArea() HostOptions parameter
#define WRITE_TO_MAIN		0x0000			// Writes image data to Main Image Buffer
#define WRITE_TO_PIP		0x0400			// Writes image data to PIP Image Buffer

#define SRC_1BIT			0x0000			// Sets host packed pixel select bits (bits 5-4)
#define SRC_4BIT			0x0020
#define SRC_8BIT			0x0030


#define SRC_BPPtp(aaa)		SRC_##aaa##BIT
#define SRC_BPP(bbb)		SRC_BPPtp(bbb)
#define SRC_COLOR			SRC_BPP(COLOR_DEPTH)
#if (SRC_COLOR != SRC_1BIT ) && (SRC_COLOR != SRC_4BIT ) && (SRC_COLOR != SRC_8BIT )
	#error "Source color not defined."
#endif


///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Places the S1D13522 in Run Mode.
///
///	This function is used to place the S1D13522 in Run Mode (normal operation).
/// This includes enabling the PLL and disabling power save mode if required.
/// This function corresponds with the RUN_SYS command (0x02).
///
/// \param	- none
///
/// \return void
/// 
/// \see GFX_DRIVER_Standby(), GFX_DRIVER_Sleep()
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_RunSystem( void );


///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Places the S1D13522 in Standby Mode.
///
///	This function is used to place the S1D13522 in Standby Mode. To wake-up
/// from Standby Mode, use GFX_DRIVER_RunSystem(). This function corresponds 
/// with the STBY command (0x04).
///
/// \param	- none
///
/// \return void
/// 
/// \see GFX_DRIVER_RunSystem(), GFX_DRIVER_Sleep()
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_Standby( void );


///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Places the S1D13522 in Sleep Mode.
///
///	This function is used to place the S1D13522 in Sleep Mode. Sleep Mode differs
/// from Standby Mode in that the PLL is powered down. To wake-up from Standby
/// Mode, use seRunSys(). This function corresponds with the SLP command (0x05). 
///
/// \param	- none
///
/// \return void
/// 
/// \see GFX_DRIVER_RunSystem(), GFX_DRIVER_Standby()
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_Sleep( void );


///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Initializes the Rotation mode settings.
///
///	This function is used to initialize the Rotation mode settings. This function
/// corresponds with the INIT_ROTMODE command (0x0B).
///
/// \param	RotMode			Corresponds with parameter 1. The value configures
///							the Rotation mode settings that will likely need to be
///							set only once.
///
/// \return void
/// 
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_InitRotmode( WORD RotMode );



///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Loads an image to the specified area of the memory.
///
///	This function is used to load an image to the specified area of the
/// memory. The operation is performed using packed pixel mode according to the
/// settings, position, and size specified by parameters 1-5. This function
/// corresponds with the LD_IMG_AREA command (0x22).
///
/// \param	HostOptions		Corresponds with parameter 1. This value configures
///							various Host settings such as the input pixel mode.
///
/// \param	xStart			Corresponds with parameter 2. This value configures
///							the X start position for the packed pixel write.
///
/// \param	yStart			Corresponds with parameter 3. This value configures
///							the Y start position for the packed pixel write.
///
/// \param	xEnd			Corresponds with parameter 4. This value configures
///							the ending position of X corridate for the packed pixel write.
///
/// \param	yEnd			Corresponds with parameter 5. This value configures
///							the ending position of Y corridate for the packed pixel write.
///
/// \param	pData			Pointer to the data to write. If pData is NULL, the
///							image load will be setup, but no data will be
///							written. In this case, the seBurstWritePort() function
///							should be used to write the image data.
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_LoadImage( WORD HostOptions, WORD xStart, WORD yStart, WORD xEnd, WORD yEnd, const WORD* pData );


typedef enum { GFX_MAIN_LAYER, GFX_PIP_LAYER, GFX_CURSOR_LAYER } GFX_LAYER;
///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Creates a layer. 
///
///	This function is used to create an additional layer that can be used for PIP, 
/// Cursor or other porpouse.
///
/// \param	layer		    Corresponds with layer. 
///                         This value can be either GFX_PIP_LAYER or GFX_CURSOR_LAYER.
///
/// \param  config			This value makes configuration for the Layer that 
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
/// \param	xStart			This value configures the X start position for the Layer
///
/// \param	yStart			This value configures the Y start position for the Layer
///
/// \param	xEnd			This value configures the X start position for the Layer
///							 
///
/// \param	yEnd			This value configures the Y start position for the Layer
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_CreateLayer( GFX_LAYER layer, WORD config, WORD xStart,  WORD yStart,  WORD xEnd,  WORD yEnd );

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Moves a layer.
///
///	This function is used to move an additional layer, PIP or Cursor.
///
/// \param	Layer		    Corresponds with layer. 
///                         This value can be either GFX_PIP_LAYER or GFX_CURSOR_LAYER.
///
/// \param	X				This value of new X start position for the Layer
///
/// \param	Y			     This value of new Y start position for the Layer
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_MoveLayer( GFX_LAYER Layer, WORD X, WORD Y );

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
void GFX_DRIVER_ShowLayer( GFX_LAYER Layer );

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
void GFX_DRIVER_HideLayer( GFX_LAYER Layer );

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Selects a layer for writing from the host.
///
///	This function is used to activate an additional layer, Main, PIP or Cursor.
/// Must be used to change a layer before writing an image.
///
/// \param	Layer		    Corresponds with layer. 
///                         This value can be:
///							GFX_MAIN_LAYER, GFX_PIP_LAYER or GFX_CURSOR_LAYER.
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_ActivateLayer( GFX_LAYER Layer );

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Updates image buffer and display
///
///	This function is used to update an image in buffer and display.
///
/// \param	Layer		    Corresponds with layer. 
///                         This value can be flaged as:
///
///							GFX_UPDATE_AS_IT_DRAWS 
///							GFX_UPDATE_HIGH_QUALITY
///							GFX_UPDATE_NO_FLASH	
///							GFX_WAIT_IMAGE_UPDATED
///							GFX_WAIT_IMAGE_DISPLAYED
///							
///							Note that GFX_UPDATE_XXX can be combined with GFX_WAIT_XXX 
///
///							The folowing configures area for update:
/// \param	xStart			This value configures the X start position for update.
///
/// \param	yStart			This value configures the Y start position for update.
///
/// \param	xEnd			This value configures the X end position for update.
///							 
///
/// \param	yEnd			This value configures the Y end position for update.
///
/// \return void
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_UpdateEpd( WORD UpdateOptions, WORD xStart, WORD yStart, WORD xEnd, WORD yEnd );


///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Collision check and repair.
///
///	This function can be used to check for collision and issue a partial to repair.
/// It checks for any in-progress update buffer synthesis and wait for it to complete.
/// If there is a collision detected, it will wait until at least 1 of the currently
/// busy display pipe (LUT) to be done (display frame out completed), then issue
/// a partial update to the same area to repair the collision.
///
/// \param	- none
///
/// \return void
/// 
/// \see GFX_DRIVER_CompleteDrawUpdate()
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_CollisionCheckAndRepair(void);


///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Image tearing check before host image drawing.
///
///	This function can be used before doing image data writing in order to aviod
/// image tearing. It checks for any in-progress update buffer synthesis. If
/// the area to be written overlaps with that of the in-progress update buffer
/// synthesis, then with until the synthesis complete.
///
/// \param	StartX			Corresponds to Microchip Graphics Library left position.
///
/// \param	StartY			Corresponds to Microchip Graphics Library top position.
///
/// \param	EndX			Corresponds to Microchip Graphics Library right position.
///
/// \param	EndY			Corresponds to Microchip Graphics Library bottom position.
///
/// \return void
/// 
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_SetupDrawUpdate( WORD startx, WORD starty, WORD endx, WORD endy);


///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Performs an area update after collision checks and repair.
///
///	This function can be used to update and area, after doing a collision check
/// and repair for the previous update. If this is the last display update, then
/// GFX_DRIVER_CollisionCheckAndRepair() can be called after this to check for
/// and repair any collision resulted from this update.
/// 
/// GFX_DRIVER_AutoUpdDisable(), GFX_DRIVER_AutoUpdFull() & GFX_DRIVER_AutoUpdPart()
/// can be used to enable / disable and select the update mode for the auto update.
///
/// For widgets to call this function, USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH has
/// to be defined in GraphicsConfig.h.
///
/// \param	StartX			Corresponds to Microchip Graphics Library left position.
///
/// \param	StartY			Corresponds to Microchip Graphics Library top position.
///
/// \param	EndX			Corresponds to Microchip Graphics Library right position.
///
/// \param	EndY			Corresponds to Microchip Graphics Library bottom position.
///
/// \return void
/// 
/// \see GFX_DRIVER_CollisionCheckAndRepair()
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_CompleteDrawUpdate( WORD startx, WORD starty, WORD endx, WORD endy);


///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Enable the auto update of widgets or GFX_DRIVER_CompleteDrawUpdate() with full
/// display update.
///
///	This function can be used to dynamically enable the auto update of widgets
/// or GFX_DRIVER_CompleteDrawUpdate() function with full display update. It can
/// be used before creating, modifying the state, directly call the draw function
/// of any widgets, or before calling GFX_DRIVER_CompleteDrawUpdate() for updating
/// any display area modified by primitives. By default, the auto update is
/// disabled.
///
/// If USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH is defined, the widgets will still
/// aviod tearing and check for and repair collision even if auto update is
/// disabled.
///
/// \param	- none
///
/// \return void
/// 
/// \see GFX_DRIVER_CollisionCheckAndRepair()
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_AutoUpdFull(void);

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Enable the auto update of widgets or GFX_DRIVER_CompleteDrawUpdate() with
/// partial display update.
///
///	This function can be used to dynamically enable the auto update of widgets
/// or GFX_DRIVER_CompleteDrawUpdate() function with partial display update. It can
/// be used before creating, modifying the state, directly call the draw function
/// of any widgets, or before calling GFX_DRIVER_CompleteDrawUpdate() for updating
/// any display area modified by primitives. By default, the auto update is
/// disabled.
///
/// If USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH is defined, the widgets will still
/// aviod tearing and check for and repair collision even if auto update is
/// disabled.
///
/// \param	- none
///
/// \return void
/// 
/// \see GFX_DRIVER_CollisionCheckAndRepair()
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_AutoUpdPart(void);

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// Disable the auto update of widgets or GFX_DRIVER_CompleteDrawUpdate().
///
///	This function can be used to dynamically disable the auto update of widgets
/// or GFX_DRIVER_CompleteDrawUpdate() function It can be used before creating,
/// modifying the state, directly call the draw function of any widgets, or
/// before calling GFX_DRIVER_CompleteDrawUpdate() for updating any display area
/// modified by primitives. By default, the auto update is disabled.
///
/// If USE_BISTABLE_DISPLAY_GOL_AUTO_REFRESH is defined, the widgets will still
/// aviod tearing and check for and repair collision even if auto update is
/// disabled.
///
/// \param	- none
///
/// \return void
/// 
/// \see GFX_DRIVER_CollisionCheckAndRepair()
///
////////////////////////////////////////////////////////////////////////////////
void GFX_DRIVER_AutoUpdDisable(void);



#endif // _S1D13522_H
