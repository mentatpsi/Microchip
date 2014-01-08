/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Custom display controller driver template
 *****************************************************************************
 * FileName:        mchpGfxDrvBuffer.c
 * Processor:       PIC24
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
 * 03/29/11          Initial Version
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210)

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "GraphicsConfig.h"
#include "HardwareProfile.h"
#include "Graphics/mchpGfxDrv.h"

#ifdef __C30__
    #ifndef __HAS_EDS__  
	    #error "Selected PIC device does not have EDS support. Please select device with EDS"
    #endif
#endif

// Protect the Display Buffer(s) by declaring the area as a large array.
// This module is used to declare the memory areas only. 
// NOTE: Since this module is using large arrays, compile option for this module must be set to -mlarge-arrays
//       To set in MPLAB IDE 8.6x :
//         1. Right click on the C-file of the module 
//         2. Select "Build Options" 
//         3. Click on "Use Alternate Settings" checkbox to enable alternate settings
//         4. Add at the end -mlarge-arrays
//         5. click OK.
//  If a larger area is to be protected (larger than a single display buffer), copy the syntax below and 
//  declare the large arrays in your application code/module. Make sure that the application code/module 
//  -mlarge-arrays build option is also set. A side effect of this build option is it can make the 
//  generated code size large:
/* 
    Excerpt from the C30 Compiler User's Manual:
    4.15 USING LARGE ARRAYS
    The compiler option -mlarge-arrays allows you to define and access arrays larger
    than 32K. You must ensure that there is enough space to allocate such an array by
    nominating a memory space large enough to contain such an object.
    Using this option will have some effect on how code is generated as it effects the definition
    of the size_t type, increasing it to an unsigned long int. If used as a global
    option, this will affect many operations used in indexing (making the operation more
    complex). Using this option locally may effect how variables can be accessed. With
    these considerations in mind, using large arrays is requires careful planning. This section
    discusses some techniques for its use.
    
    Two things occur when the -mlarge-arrays option is selected:
    1. The compiler generates code in a different way for accessing arrays.
    2. The compiler defines the size_t type to be unsigned long int.

    Item 1 can have a negative effect on code size, if used throughout the whole program.
    It is possible to only compile a single module with this option and have it work, but there
    are limitations which will be discussed shortly.
    Item 2 affects the calling convention when external functions receive or return objects
    of type size_t. The compiler provides libraries built to handle a larger size_t and
    these objects will be selected automatically by the linker (provided they exist).
    Mixing -mlarge-arrays and normal-sized arrays together is relatively straightforward
    and might be the best way to make use of this new feature. There are a few usage
    restrictions: functions defined in such a module should not call external routines that
    use size_t, and functions defined in such a module should not receive size_t as a
    parameter.

*/

#define GFX_DISPLAY_PIXEL_COUNT ((DWORD)DISP_HOR_RESOLUTION*DISP_VER_RESOLUTION)

/* There are three cases to set up the display buffer
   Case 1: Display Buffer is within the internal RAM 
   Case 2: Display Buffer starts in internal RAM and extends to external RAM (spans internal and external)
   Case 3: Display Buffer is in external RAM
 
   Case 2 is not supported in the configuration below. But this is not saying it is not allowed.
   If the display buffer is intentionally created to span the internal and external memory, the memory areas 
   must be protected by declaring two large arrays one for each region.
   
   The exact boundary for the internal RAM and external RAM must be set in the EPMP CSx base address.
   For example (if EPMP CS1 is used as the next area after the internal RAM):
   - EPMP CS1 base address (PMCS1BS) = 0x00018800 (for PIC24FJ256DA210)
   - If using the 24K variant, the boundary between internal RAM and external RAM is not contiguous.
     So using a 24K variant is not possible when spanning display buffer between internal and external RAM.
   
*/ 


// Define the buffers used for double buffering
#if defined (USE_DOUBLE_BUFFERING)

    // This assumes that the device attached to the CS1 is a RAM device.
    // If CS2 is also attached to a RAM device, and you want the CS2 to be
    // used as a display buffer (that you can read and write data on) then 
    // this code should be modified to allow CS2 to be used as buffer for double buffering.
	#if defined(GFX_EPMP_CS1_BASE_ADDRESS) 

        #if (COLOR_DEPTH == 16)
            
            __eds__ WORD GFXDisplayBuffer[GFX_DISPLAY_PIXEL_COUNT*2] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), noload, section("Graphics_Display_Buffer"))); 
       
        #elif (COLOR_DEPTH == 8)    
        
            __eds__ BYTE GFXDisplayBuffer[GFX_DISPLAY_PIXEL_COUNT*2] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), noload, section("Graphics_Display_Buffer"))); 
        
        #elif (COLOR_DEPTH == 4)
        
            __eds__ BYTE GFXDisplayBuffer[(DWORD)(GFX_DISPLAY_PIXEL_COUNT)] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), noload, section("Graphics_Display_Buffer"))); 
        
        #elif (COLOR_DEPTH == 1)
        
            __eds__ BYTE GFXDisplayBuffer[(DWORD)(GFX_DISPLAY_PIXEL_COUNT)/4] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), noload, section("Graphics_Display_Buffer"))); 
        
        #endif
    
     #else   
        
        // double buffering can also be enabled when using internal memory but only
	    // if the internal memory can accomodate the buffers.

        #if (COLOR_DEPTH == 16) || (COLOR_DEPTH == 8)

    	    #error "When using TFT displays 16 or 8 bpp color depth use external memory"
    
	    #elif (COLOR_DEPTH == 4)

            __eds__ BYTE GFXDisplayBuffer[(DWORD)(GFX_DISPLAY_PIXEL_COUNT)] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), section("Graphics_Display_Buffer")));   

		#elif (COLOR_DEPTH == 1)
	    
            __eds__ BYTE GFXDisplayBuffer[(DWORD)(GFX_DISPLAY_PIXEL_COUNT)/4] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), section("Graphics_Display_Buffer")));   
	        
		#endif    

    #endif

#else
    
    // not using Double Buffering
    
    #if (GFX_DISPLAY_BUFFER_START_ADDRESS >= GFX_EPMP_CS1_BASE_ADDRESS) || (GFX_DISPLAY_BUFFER_START_ADDRESS >= GFX_EPMP_CS2_BASE_ADDRESS)
        #if (COLOR_DEPTH == 16)
            
            __eds__ WORD GFXDisplayBuffer[GFX_DISPLAY_PIXEL_COUNT] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), noload, section("Graphics_Display_Buffer"))); 
       
        #elif (COLOR_DEPTH == 8)    
        
            __eds__ BYTE GFXDisplayBuffer[GFX_DISPLAY_PIXEL_COUNT] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), noload, section("Graphics_Display_Buffer"))); 
        
        #elif (COLOR_DEPTH == 4)
        
            __eds__ BYTE GFXDisplayBuffer[(DWORD)(GFX_DISPLAY_PIXEL_COUNT)/2] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), noload, section("Graphics_Display_Buffer"))); 
        
        #elif (COLOR_DEPTH == 1)
        
            __eds__ BYTE GFXDisplayBuffer[(DWORD)(GFX_DISPLAY_PIXEL_COUNT)/8] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), noload, section("Graphics_Display_Buffer"))); 
        
        #endif
    
    #else
        // allocating data in internal RAM, syntax for EDS variable declaration is different
    
        #if (COLOR_DEPTH == 16)
    
            __eds__ WORD GFXDisplayBuffer[GFX_DISPLAY_PIXEL_COUNT] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), section("Graphics_Display_Buffer")));   
        
        #elif (COLOR_DEPTH == 8)    
    
            __eds__ BYTE GFXDisplayBuffer[GFX_DISPLAY_PIXEL_COUNT] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), section("Graphics_Display_Buffer")));   
    
        #elif (COLOR_DEPTH == 4)
        
            __eds__ BYTE GFXDisplayBuffer[(DWORD)(GFX_DISPLAY_PIXEL_COUNT)/2] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), section("Graphics_Display_Buffer")));   
        
        #elif (COLOR_DEPTH == 1)
        
            __eds__ BYTE GFXDisplayBuffer[(DWORD)(GFX_DISPLAY_PIXEL_COUNT)/8] __attribute__((eds, address(GFX_DISPLAY_BUFFER_START_ADDRESS), section("Graphics_Display_Buffer")));   
        
        #endif
    
    #endif // end of not using Double Buffering
    
#endif    

// When using IPU the memory area used for the IPU operations must also be protected

#ifdef USE_COMP_IPU
    
    #if (GFX_DECOMPRESSED_DATA_RAM_ADDRESS >= GFX_EPMP_CS1_BASE_ADDRESS) 

        __eds__ BYTE GFXIPUDecompressedDataBuffer [GFX_DECOMPRESSED_BUFFER_SIZE] __attribute__((eds, address(GFX_DECOMPRESSED_DATA_RAM_ADDRESS), noload, section("Graphics_Decompressed_Data_Buffer"))); 
    
    #else

        __eds__ BYTE GFXIPUDecompressedDataBuffer [GFX_DECOMPRESSED_BUFFER_SIZE] __attribute__((eds, address(GFX_DECOMPRESSED_DATA_RAM_ADDRESS), section("Graphics_Decompressed_Data_Buffer"))); 

    #endif

    #if (GFX_COMPRESSED_DATA_RAM_ADDRESS >= GFX_EPMP_CS1_BASE_ADDRESS) 

        __eds__ BYTE GFXIPUCompressedDataBuffer [GFX_COMPRESSED_BUFFER_SIZE] __attribute__((eds, address(GFX_COMPRESSED_DATA_RAM_ADDRESS), noload, section("Graphics_Compressed_Data_Buffer"))); 

    #else

        __eds__ BYTE GFXIPUCompressedDataBuffer [GFX_COMPRESSED_BUFFER_SIZE] __attribute__((eds, address(GFX_COMPRESSED_DATA_RAM_ADDRESS), section("Graphics_Compressed_Data_Buffer"))); 

    #endif
    
   
#endif // #ifdef USE_COMP_IPU


#endif //#if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210)

