/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Custom display controller driver template
 *****************************************************************************
 * FileName:        mchpGfxDrv.h
 * Processor:       PIC24F
 * Compiler:       	MPLAB C30
 * Company:         Microchip Technology Incorporated
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
 * Date				Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 08/20/09      	Initial Version
 * 12/03/09      	Added Double Buffering Support
 * 04/04/10      	Added block scrolling 
 * 07/12/10         Added EDS type images
 * 09/27/10         Added CHRGPU & IPU support
 * 11/29/10			Fixed InvalidateAll() error
 * 03/09/11         - for Graphics Library Version 3.00
 *                  - removed USE_DRV_xxx
 *                  - moved minimum required prototypes to DisplayDriver.h
 *                  - modified dependencies
 *                  - modified GFX_SetDisplayArea() to set the global
 *                    display buffer address variable at the same time
 *                    set the current display buffer location.
 *                  - GFX_BUFFER1 and GFX_BUFFER2 are now defined internally
 *                    and not in the hardware profile.
 * 03/29/11         - moved prototypes of double buffering feature to
 *                    DisplayDriver.h
 * 04/06/11         - renamed to mchpGfxDrv.h
 * 12/19/11         - fixed data types on macros.
 *                  - modified location of compressed and decompressed buffers  
 *                    to be placed after the display buffer. This will allow
 *                    to use IPU even if internal memory is used. 
 *****************************************************************************/
#ifndef _MCHP_DISPLAY_DRIVER_H
#define _MCHP_DISPLAY_DRIVER_H

#include "HardwareProfile.h"
#include "GraphicsConfig.h"
#include "GenericTypeDefs.h"

#if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210)

/*********************************************************************
* Error Checking
*********************************************************************/
    #ifndef DISP_HOR_RESOLUTION
        #error DISP_HOR_RESOLUTION must be defined in HardwareProfile.h
    #endif
    #ifndef DISP_VER_RESOLUTION
        #error DISP_VER_RESOLUTION must be defined in HardwareProfile.h
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
* Overview: Additional hardware-accelerated functions can be implemented
*           in the driver. These definitions exclude the PutPixel()-based
*           functions in the primitives layer (Primitive.c file) from compilation.
*********************************************************************/

extern volatile DWORD _displayAreaBaseAddr;
extern volatile DWORD _workArea1BaseAddr;
extern volatile DWORD _workArea2BaseAddr;

/*********************************************************************
* Overview: Sets the type of display glass used. Define this in the Hardware Profile.
*			- #define GFX_LCD_TYPE    GFX_LCD_TFT - sets type TFT display
*			- #define GFX_LCD_TYPE    GFX_LCD_CSTN - sets type color STN display
*			- #define GFX_LCD_TYPE    GFX_LCD_MSTN - sets type mon STN display
*			- #define GFX_LCD_TYPE    GFX_LCD_OFF - display is turned off
*
*********************************************************************/
#ifndef GFX_LCD_TYPE
#define GFX_LCD_TYPE
#endif

#define GFX_LCD_TFT  0x01			// Type TFT Display 
#define GFX_LCD_CSTN 0x03			// Type Color STN Display 
#define GFX_LCD_MSTN 0x02			// Type Mono STN Display 
#define GFX_LCD_OFF  0x00			// display is turned off

/*********************************************************************
* Overview: Sets the STN glass data width. Define this in the Hardware Profile.
*			- #define STN_DISPLAY_WIDTH    STN_DISPLAY_WIDTH_4 - use 4-bit wide interface
*			- #define STN_DISPLAY_WIDTH    STN_DISPLAY_WIDTH_8 - Use 8-bit wide interface
*			- #define STN_DISPLAY_WIDTH    STN_DISPLAY_WIDTH_16 - Use 16-bit wide interface
*
*********************************************************************/
#ifndef STN_DISPLAY_WIDTH
#define STN_DISPLAY_WIDTH
#endif

#define STN_DISPLAY_WIDTH_4  0x00	// display interface is 4 bits wide
#define STN_DISPLAY_WIDTH_8  0x01	// display interface is 8 bits wide
#define STN_DISPLAY_WIDTH_16 0x02	// display interface is 16 bits wide

#define GFX_1_BPP  0x00
#define GFX_2_BPP  0x01
#define GFX_4_BPP  0x02
#define GFX_8_BPP  0x03
#define GFX_16_BPP 0x04

#if (COLOR_DEPTH == 1)
    #define GFX_BITS_PER_PIXEL                  GFX_1_BPP
    #define GFX_COLOR_MASK                      0x0001
#elif (COLOR_DEPTH == 2)
    #define GFX_BITS_PER_PIXEL                  GFX_2_BPP
    #define GFX_COLOR_MASK                      0x0003
#elif (COLOR_DEPTH == 4)
    #define GFX_BITS_PER_PIXEL                  GFX_4_BPP
    #define GFX_COLOR_MASK                      0x000F
#elif (COLOR_DEPTH == 8)
    #define GFX_BITS_PER_PIXEL                  GFX_8_BPP
    #define GFX_COLOR_MASK                      0x00FF
#else
    #define GFX_BITS_PER_PIXEL                  GFX_16_BPP
    #define GFX_COLOR_MASK                      0xFFFF
#endif

#define GFX_COMMAND_QUEUE_LENGTH            16

#define GFX_ACTIVE_HIGH                     1
#define GFX_ACTIVE_LOW                      0

/* RCC GPU Commands */
#define RCC_SRCADDR                         0x62000000ul
#define RCC_DESTADDR                        0x63000000ul
#define RCC_RECTSIZE                        0x64000000ul
#define RCC_COLOR                           0x66000000ul
#define RCC_STARTCOPY                       0x67000000ul

/*********************************************************************
* Overview: Type of Rectangle Copy Operations. Select one of the following
*			rectangle copy operations and together with the ROP; the source,
*           destination, current color set and transparency are evaluated
*           on each pixel and the result written to the destination. 
*			- RCC_COPY - Copies the source data to the destination address with
*                        the selected ROP. 
*			- RCC_SOLID_FILL - Fills the specified rectangle with the current color set. 
*			- RCC_TRANSPARENT_COPY - Operation is the same as the COPY operation except that
*									 the source data is compared against the current color 
*									 set. If the values match, the source data is not 
*									 written to the destination. The source image is, 
*									 therefore, transparent at such a location, allowing
*
*********************************************************************/
#define RCC_COPY                            0x00000080ul
// <COMBINE RCC_COPY>
#define RCC_SOLID_FILL                      0x00000000ul
// <COMBINE RCC_COPY>
#define RCC_TRANSPARENT_COPY                0x00000300ul

/*********************************************************************
* Overview: Raster Operation (ROP) option. Select one of the following
*			16 raster operation options whenever Rectangle Copy
*			Processing Unit (RCCGPU) is used. The raster operation
*           is performed on the source (S) and destination (D) data.
*			and the result written to the destination (D).
*			- RCC_ROP_0 - 0 (BLACK)
*			- RCC_ROP_1 - not (S or D)
*			- RCC_ROP_2 - (not S) and D
*			- RCC_ROP_3 - not (S)
*			- RCC_ROP_4 - S and not (D))
*			- RCC_ROP_5 - not (D)
*			- RCC_ROP_6 - S xor D
*			- RCC_ROP_7 - not (S and D)
*			- RCC_ROP_8 - S and D
*			- RCC_ROP_9 - not (S xor D)
*			- RCC_ROP_A - D
*			- RCC_ROP_B - not (S) or D
*			- RCC_ROP_C - S
*			- RCC_ROP_D - S or not (D)
*			- RCC_ROP_E - S or D
*			- RCC_ROP_F - 1 (WHITE)
*
*********************************************************************/
#define RCC_ROP_0                           0x00000000ul		
// <COMBINE RCC_ROP_0>
#define RCC_ROP_1                           0x00000008ul		// not (S or D)
// <COMBINE RCC_ROP_0>
#define RCC_ROP_2                           0x00000010ul		// (not S) and D
// <COMBINE RCC_ROP_0>
#define RCC_ROP_3                           0x00000018ul		// not (S)
// <COMBINE RCC_ROP_0>
#define RCC_ROP_4                           0x00000020ul		// S and not (D)
// <COMBINE RCC_ROP_0>
#define RCC_ROP_5                           0x00000028ul		// not (D)
// <COMBINE RCC_ROP_0>
#define RCC_ROP_6                           0x00000030ul		// S xor D
// <COMBINE RCC_ROP_0>
#define RCC_ROP_7                           0x00000038ul		// not (S and D)
// <COMBINE RCC_ROP_0>
#define RCC_ROP_8                           0x00000040ul		// S and D
// <COMBINE RCC_ROP_0>
#define RCC_ROP_9                           0x00000048ul		// not (S xor D)
// <COMBINE RCC_ROP_0>
#define RCC_ROP_A                           0x00000050ul		// D
// <COMBINE RCC_ROP_0>
#define RCC_ROP_B                           0x00000058ul		// not (S) or D
// <COMBINE RCC_ROP_0>
#define RCC_ROP_C                           0x00000060ul		// S 
// <COMBINE RCC_ROP_0>
#define RCC_ROP_D                           0x00000068ul		// S or not (D)
// <COMBINE RCC_ROP_0>
#define RCC_ROP_E                           0x00000070ul		// S or D
// <COMBINE RCC_ROP_0>
#define RCC_ROP_F                           0x00000078ul		// 1 (WHITE)
// <COMBINE RCC_ROP_0>

/*********************************************************************
* Overview: Source (S) and Destination (D) data type. When performing 
*           executing commands on the Rectangle Copy Processing Unit 
*           (RCCGPU). The source and destination data can be treated 
*           as a continuous block of data in memory or a discontinuous
*			data in memory. This gives flexibility to the operation
*           where an copy operation can be performed to data already
*           present in the display buffer or anywhere else in data memory.
*           Both source and destination data can be set to 
*           continuous or discontinuous data. These macros
*           are only used in RCCGPU operations.
*			- RCC_SRC_ADDR_CONTINUOUS - source data is continuous
*			- RCC_SRC_ADDR_DISCONTINUOUS - source data is discontinuous
*			- RCC_DEST_ADDR_CONTINUOUS - destination data is continuous
*			- RCC_DEST_ADDR_DISCONTINUOUS - destination data is discontinuous
*
*********************************************************************/
#define RCC_SRC_ADDR_CONTINUOUS             0x00000002ul		
// <COMBINE RCC_SRC_ADDR_CONTINUOUS>
#define RCC_SRC_ADDR_DISCONTINUOUS          0x00000000ul		
// <COMBINE RCC_SRC_ADDR_CONTINUOUS>
#define RCC_DEST_ADDR_CONTINUOUS            0x00000004ul		
// <COMBINE RCC_SRC_ADDR_CONTINUOUS>
#define RCC_DEST_ADDR_DISCONTINUOUS         0x00000000ul		

#define GFX_SetCommand(c)                   { G1CMDL = ((DWORD_VAL)((DWORD)(c))).word.LW; G1CMDH = ((DWORD_VAL)((DWORD)(c))).word.HW; Nop(); }

#define GFX_SetDisplayArea(a)               { _displayAreaBaseAddr = (DWORD)a; G1DPADRL = ((DWORD_VAL)((DWORD)(a))).word.LW; G1DPADRH = ((DWORD_VAL)((DWORD)(a))).word.HW; }
#define GFX_SetWorkArea1(a)                 { G1W1ADRL = ((DWORD_VAL)((DWORD)(a))).word.LW; G1W1ADRH = ((DWORD_VAL)((DWORD)(a))).word.HW; }
#define GFX_SetWorkArea2(a)                 { G1W2ADRL = ((DWORD_VAL)((DWORD)(a))).word.LW; G1W2ADRH = ((DWORD_VAL)((DWORD)(a))).word.HW; }

#define GFX_RCC_SetSrcOffset(srcOffset)   	GFX_SetCommand(RCC_SRCADDR  | (DWORD)(srcOffset));      /* Don't do error checking or error corrections */           
#define GFX_RCC_SetDestOffset(dstOffset)  	GFX_SetCommand(RCC_DESTADDR | (DWORD)(dstOffset));      /* Don't do error checking or error corrections */          
#define GFX_RCC_SetSize(width, height)      GFX_SetCommand(RCC_RECTSIZE | (((DWORD)(width)) << 12) | (DWORD)height)     /* Don't do error checking or error corrections */  
#define GFX_RCC_SetColor(color)             GFX_SetCommand(RCC_COLOR    | (color & GFX_COLOR_MASK)) /* Don't do error checking or error corrections */
#define GFX_RCC_StartCopy(type_of_copy, rop, src_addr_type, dest_addr_type)      GFX_SetCommand(RCC_STARTCOPY | type_of_copy | rop | src_addr_type | dest_addr_type)     /* Don't do error checking or error corrections */

#define CHR_FGCOLOR                         0x50000000ul
#define CHR_BGCOLOR                         0x51000000ul
#define CHR_FONTBASE                        0x52000000ul
#define CHR_PRINTCHAR                       0x53000000ul
#define CHR_TEXTAREASTART                   0x58000000ul
#define CHR_TEXTAREAEND                     0x59000000ul
#define CHR_PRINTPOS                        0x5A000000ul

#define CHR_TRANSPARENT                     0x00800000ul
#define CHR_OPAQUE                          0x00000000ul

#define GFX_CHR_SetFgColor(color)           { _chrcolor = color; GFX_SetCommand(CHR_FGCOLOR | color); }
#define GFX_CHR_SetBgColor(color)           GFX_SetCommand(CHR_BGCOLOR | color)
#define GFX_CHR_SetFont(fontaddress)        { GFX_SetCommand(CHR_FONTBASE | fontaddress); }

#define GFX_CHR_SetTextAreaStart(x, y)      GFX_SetCommand(CHR_TEXTAREASTART | ((DWORD)x) << 12 | y)
#define GFX_CHR_SetTextAreaEnd(x, y)        GFX_SetCommand(CHR_TEXTAREAEND | ((DWORD)x) << 12 | y)
#define GFX_CHR_SetPrintPos(x, y)           GFX_SetCommand(CHR_PRINTPOS | ((DWORD)x) << 12 | y) 
#define GFX_CHR_PrintChar(character, transparency)   GFX_SetCommand(CHR_PRINTCHAR | transparency | character)   


#define IPU_SRCOFFSET                       0x71000000ul
#define IPU_DSTOFFSET                       0x72000000ul
#define IPU_DSTSIZE                         0x74000000ul

#define GFX_IPU_SetSrcOffset(offset)   		(GFX_SetCommand(IPU_SRCOFFSET | (DWORD) offset))
#define GFX_IPU_SetDestOffset(offset)   	(GFX_SetCommand(IPU_DSTOFFSET | (DWORD) offset))
#define GFX_IPU_Inflate(bytes)            	(GFX_SetCommand(IPU_DSTSIZE   | (DWORD) bytes ))

#define GFX_IPU_GetFinalBlock()           	(G1IPU & 0x0001)
#define GFX_IPU_GetDecompressionDone()    	(G1IPU & 0x0002)
#define GFX_IPU_GetDecompressionError()   	(G1IPU & 0x003C)
#define GFX_IPU_GetWrapAroundError()      	(G1IPU & 0x0004)
#define GFX_IPU_GetBlockLengthError()     	(G1IPU & 0x0008)
#define GFX_IPU_GetUndefinedBlockError()  	(G1IPU & 0x0010)
#define GFX_IPU_GetHuffmannError()        	(G1IPU & 0x0020)

#define GFX_GetFreeCommandSpace()       	(GFX_COMMAND_QUEUE_LENGTH - _GCMDCNT)
#define GFX_WaitForCommandQueue(n)      	while(GFX_GetFreeCommandSpace() < (n));
#define GFX_IsRccGpuBusy()              	(_RCCBUSY)
#define GFX_IsIpuGpuBusy()              	(_IPUBUSY)
#define GFX_IsChrGpuBusy()              	(_CHRBUSY)
#define GFX_IsPuGpuBusy()               	(_PUBUSY)
#define GFX_WaitForRccGpu()             	while(_RCCBUSY);
#define GFX_WaitForIpuGpu()             	while(_IPUBUSY);
#define GFX_WaitForChrGpu()             	while(_CHRBUSY);
#define GFX_WaitForGpu()                	while(_PUBUSY);

#define GFX_DATA_CONTINUOUS                 (0x01)
#define GFX_DATA_DISCONTINUOUS              (0x00)

/* Definition of macros

 * GFX_DISPLAY_BUFFER_START_ADDRESS - This defines the constant used to set the
 * location of the display buffer location. for the PIC24FJ256DA210 the
 * address used must be consistent with the "DATA SPACE MEMORY MAP FOR
 * PIC24FJ256DA210 FAMILY DEVICES"
 * GFX_DISPLAY_BUFFER_LENGTH - This defines the size of the display buffer in
 * bytes.

*/

// Calculate Display Buffer Size required in bytes

#define GFX_DISPLAY_PIXEL_COUNT ((DWORD)DISP_HOR_RESOLUTION*DISP_VER_RESOLUTION)

#if (COLOR_DEPTH == 16)
    #define GFX_REQUIRED_DISPLAY_BUFFER_SIZE_IN_BYTES       (GFX_DISPLAY_PIXEL_COUNT*2)
#elif (COLOR_DEPTH == 8)
    #define GFX_REQUIRED_DISPLAY_BUFFER_SIZE_IN_BYTES       (GFX_DISPLAY_PIXEL_COUNT)
#elif (COLOR_DEPTH == 4)
    #define GFX_REQUIRED_DISPLAY_BUFFER_SIZE_IN_BYTES       (GFX_DISPLAY_PIXEL_COUNT/2)
#elif (COLOR_DEPTH == 1)
    #define GFX_REQUIRED_DISPLAY_BUFFER_SIZE_IN_BYTES       (GFX_DISPLAY_PIXEL_COUNT/8)
#endif

#if defined (USE_DOUBLE_BUFFERING)
    #define GFX_BUFFER1 (GFX_DISPLAY_BUFFER_START_ADDRESS)
    #define GFX_BUFFER2 (GFX_DISPLAY_BUFFER_START_ADDRESS + GFX_REQUIRED_DISPLAY_BUFFER_SIZE_IN_BYTES)
#endif

#ifdef USE_COMP_IPU
	/*
		Notes: 
              When using IPU with internal memory only, make sure the buffers can fit in 
		      the internal memory.
			  IPU Buffer locations:	

			  - Using double buffering
			      GFX_DECOMPRESSED_DATA_RAM_ADDRESS = GFX_DISPLAY_BUFFER_START_ADDRESS + (GFX_DISPLAY_BUFFER_LENGTH*2)
			      GFX_COMPRESSED_DATA_RAM_ADDRESS = GFX_DECOMPRESSED_DATA_RAM_ADDRESS + GFX_DECOMPRESSED_BUFFER_SIZE

			  - NOT Using double buffering
			      GFX_DECOMPRESSED_DATA_RAM_ADDRESS = GFX_DISPLAY_BUFFER_START_ADDRESS + GFX_DISPLAY_BUFFER_LENGTH
			      GFX_COMPRESSED_DATA_RAM_ADDRESS = GFX_DECOMPRESSED_DATA_RAM_ADDRESS + GFX_DECOMPRESSED_BUFFER_SIZE
			  
			  Since the IPU buffers are located right after the display buffer(s) the memory locations
			  are dependent of the starting address of the display buffer.
			  To locate the IPU buffers manually (i.e. setting the location manually) define the 
			  following macros in the the HardwareProfile.h:
			  #define GFX_DECOMPRESSED_DATA_RAM_ADDRESS ????
			  #define GFX_COMPRESSED_DATA_RAM_ADDRESS   ????

			  The following macro values can be obtained from the output of the Graphics Resource Converter (GRC) 
			  when generating image files for IPU:
			  
              #define GFX_COMPRESSED_BUFFER_SIZE              (13950)   // value used in based on the GRC output
              #define GFX_DECOMPRESSED_BUFFER_SIZE            (19216)   // value used in based on the GRC output
              #define GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE   (1024)    // 1024 is the maximum value allowed for the transfer array
			  			  
	*/ 
    // Error checks 
    #ifndef GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE
            #error "GFX_IPU_TEMP_DATA_TRANSFER_ARRAY_SIZE must be defined in HardwareProfile.h. See documentation in mchpGfxDrv.h"
    #endif

    #ifndef GFX_COMPRESSED_BUFFER_SIZE
            #error "GFX_COMPRESSED_BUFFER_SIZE must be defined in HardwareProfile.h. See documentation in mchpGfxDrv.h"
    #endif

    #ifndef GFX_DECOMPRESSED_BUFFER_SIZE
            #error "GFX_DECOMPRESSED_BUFFER_SIZE must be defined in HardwareProfile.h. See documentation in mchpGfxDrv.h"
    #endif

    #ifdef USE_DOUBLE_BUFFERING
        #ifndef GFX_DECOMPRESSED_DATA_RAM_ADDRESS
	        #define GFX_DECOMPRESSED_DATA_RAM_ADDRESS   (GFX_DISPLAY_BUFFER_START_ADDRESS + (GFX_DISPLAY_BUFFER_LENGTH*2))
        #endif	
        #ifndef GFX_COMPRESSED_DATA_RAM_ADDRESS       
            #define GFX_COMPRESSED_DATA_RAM_ADDRESS     (GFX_DISPLAY_BUFFER_START_ADDRESS + (GFX_DISPLAY_BUFFER_LENGTH*2) + GFX_DECOMPRESSED_BUFFER_SIZE)     
        #endif    	
    #else
        #ifndef GFX_DECOMPRESSED_DATA_RAM_ADDRESS
	        #define GFX_DECOMPRESSED_DATA_RAM_ADDRESS   (GFX_DISPLAY_BUFFER_START_ADDRESS + GFX_DISPLAY_BUFFER_LENGTH)
        #endif	
        #ifndef GFX_COMPRESSED_DATA_RAM_ADDRESS       
            #define GFX_COMPRESSED_DATA_RAM_ADDRESS     (GFX_DISPLAY_BUFFER_START_ADDRESS + GFX_DISPLAY_BUFFER_LENGTH + GFX_DECOMPRESSED_BUFFER_SIZE)     
        #endif    	
    #endif
    
#endif

/*********************************************************************
* Macros:  GFX_GetDisplayArea()    
*
* Overview: Returns the current display buffer address. 
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
#define GFX_GetDisplayArea()    (_displayAreaBaseAddr)

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
void            DisplayBrightness(WORD level);


/*********************************************************************
* Function: WORD DrvMemCopy(DWORD srcAddr,   DWORD dstAddr, 
*						    DWORD srcOffset, DWORD dstOffset, 
*			                DWORD width,     DWORD height, 
*                           WORD copyType)
*
* Overview: Performs a memory copy from source to destination with
*           the parameters given.
*
* PreCondition: none
*
* Input: srcAddr - the base address of the data to be moved
*        dstAddr - the base address of the new location of the moved data 
*        srcOffset - offset of the data to be moved with respect to the 
*					 source base address.
*        dstOffset - offset of the new location of the moved data respect 
*					 to the source base address.
*        width - width of the block of data to be moved when copyType != 0, 
*                if copyType = 0 then this is the size of data to be transferred.
*        height - height of the block of data to be moved when copyType != 0,
*                if copyType = 0 then this parameter is ignored.
*        copyType - sets the source and destination data types
					0 - Source data is continuous, destination data is continuous
					1 - Source data is discontinuous, destination data is continuous
					2 - Source data is continuous, destination data is discontinuous
					3 - Source data is discontinuous, destination data is discontinuous
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and operation is not completely performed.
*         - Returns 1 when the operation is completely performed.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD DrvMemCopy(DWORD srcAddr, DWORD dstAddr, DWORD srcOffset, DWORD dstOffset, 
			    DWORD width,   DWORD height,   WORD copyType);

/*********************************************************************
* Function: WORD ROPBlock (DWORD srcAddr,   DWORD dstAddr, 
*						   DWORD srcOffset, DWORD dstOffset, 
*			               WORD  srcType,   WORD  dstType,  
*                          WORD  copyOp,    WORD rop, 
*			               WORD color,      WORD width,  WORD height)
*
* Overview: Performs a Raster Operation (ROP) on source and destination.
*			The type of ROP is decided by the rop and the copyOp parameter.
*			
* PreCondition: none
*
* Input: srcAddr - the base address of the data to be moved
*        dstAddr - the base address of the new location of the moved data 
*        srcOffset - offset of the data to be moved with respect to the 
*					 source base address.
*        dstOffset - offset of the new location of the moved data respect 
*					 to the source base address.
*        srcType - sets the source type (GFX_DATA_CONTINUOUS or GFX_DATA_DISCONTINUOUS)
*        dstType - sets the destination type (GFX_DATA_CONTINUOUS or GFX_DATA_DISCONTINUOUS) 
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
* Note: none
*
********************************************************************/
WORD ROPBlock(DWORD srcAddr, DWORD dstAddr, DWORD srcOffset, DWORD dstOffset, 
			   WORD srcType, WORD dstType,  WORD copyOp,     WORD rop, 
			   WORD color, WORD width, WORD height);

/*********************************************************************
* Function: WORD Scroll(SHORT left, SHORT top,  
*						SHORT right, SHORT bottom, SHORT delta)
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
* Overview: Scrolls the rectangular area defined by left, top, right, bottom by delta pixels.
*
* Note: none
*
********************************************************************/
WORD Scroll(SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT delta, WORD dir);
			   				
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
BYTE Decompress(DWORD SrcAddress, DWORD DestAddress, DWORD nbytes);


typedef struct
{
    WORD X;
    WORD Y;
    WORD W;
    WORD H;
} RectangleArea;

#ifdef USE_DOUBLE_BUFFERING

/*********************************************************************
* Macros:  GFX_MAX_INVALIDATE_AREAS
*
* Overview: Defines the maximum number of invalidated areas. 
*           If the number of invalidated areas equals GFX_MAX_INVALIDATE_AREAS
*           the whole frame buffer is invalidated.
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
#define GFX_MAX_INVALIDATE_AREAS 5

#endif //USE_DOUBLE_BUFFERING

#ifdef USE_PALETTE

    #include "Graphics/Palette.h"

#endif

#endif //#if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210)

#endif // _MCHP_DISPLAY_DRIVER_H
