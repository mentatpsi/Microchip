/*********************************************************************
 *
 *	Hardware specific definitions
 *
 *********************************************************************
 * FileName:        HardwareProfile.h
 * Dependencies:    None
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:        Microchip C30 and C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product ("Device") which is 
 *      integrated into Licensee�s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Date	                Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 12/19/11             ...
 * 01/23/12             Add MPLABX support
 ********************************************************************/

#ifndef CFG_INCLUDE_MPLAB_X

#if defined (__PIC24F__) 

	#if defined (__PIC24FJ256DA210__)
        
        /*********************************************************************
        * Hardware Configuration for
        * PIC24FJ256DA210 Development Board
        * Display TFT-G240320LTSW-118W-E
        *    
        *    These hardware profiles places the display buffer in internal RAM.
        *    For the mode where the images are compressed for IPU, the two 
        *    macros are defined:
        *    #define GFX_DECOMPRESSED_DATA_RAM_ADDRESS       (GFX_EPMP_CS1_BASE_ADDRESS)
        *    #define GFX_COMPRESSED_DATA_RAM_ADDRESS         (GFX_DECOMPRESSED_DATA_RAM_ADDRESS + GFX_DECOMPRESSED_BUFFER_SIZE)
        *    
        *    These definitions overrides the definition in the mchpGfxDrv.h file for 
        *    the compressed and decompressed buffers. In short the external 
        *    memory is only used for the two required IPU buffers when using
        *    compressed images for IPU.
        *    
        *    Note that the two values:
        *    #define GFX_COMPRESSED_BUFFER_SIZE              (23212)
        *    #define GFX_DECOMPRESSED_BUFFER_SIZE            (26698)
        *    are derived from the output of Graphics Resource Converter (GRC).
        *    These values are the worst case sizes which is for 8BPP images.   
        ********************************************************************/
        #include "Configs/HWP_DA210_BRD_16PMP_INT_QVGAv1.h"
        //#include "Configs/HWP_DA210_BRD_16PMP_INT_IPU_QVGAv1.h"

        /*********************************************************************
        * Hardware Configuration for
        * PIC24FJ256DA210 Development Board
        * Display PH480272T-005-I11Q
        *    
        * Use these hardware profile when running 4 or 1 BPP with WQVGA 
        * display.   
        *    
        *    This hardware profile places the display buffer in internal RAM.
        *    For the mode where the images are compressed for IPU, the two 
        *    macros are defined:
        *    #define GFX_DECOMPRESSED_DATA_RAM_ADDRESS       (GFX_EPMP_CS1_BASE_ADDRESS)
        *    #define GFX_COMPRESSED_DATA_RAM_ADDRESS         (GFX_DECOMPRESSED_DATA_RAM_ADDRESS + GFX_DECOMPRESSED_BUFFER_SIZE)
        *    
        *    These definitions overrides the definition in the mchpGfxDrv.h file for 
        *    the compressed and decompressed buffers. In short the external 
        *    memory is only used for the two required IPU buffers when using
        *    compressed images for IPU.
        *    
        *    Note that the two values:
        *    #define GFX_COMPRESSED_BUFFER_SIZE              (23212)
        *    #define GFX_DECOMPRESSED_BUFFER_SIZE            (26698)
        *    are derived from the output of Graphics Resource Converter (GRC).
        *    These values are the worst case sizes which is for 8BPP images.   
        ********************************************************************/
        //#include "Configs/HWP_DA210_BRD_16PMP_INT_WQVGAv1.h"
        //#include "Configs/HWP_DA210_BRD_16PMP_INT_IPU_WQVGAv1.h"

        /*********************************************************************
        * Hardware Configuration for
        * PIC24FJ256DA210 Development Board
        * Display PH480272T-005-I11Q
        *    
        * Use this hardware profile when running 8 BPP with WQVGA 
        * display.   
        *    
        *    This hardware profile places the display buffer in external RAM.
        *    For the mode where the images are compressed for IPU, the two 
        *    macros are defined in the mchpGfxDrv.h file:
        *    #define GFX_DECOMPRESSED_DATA_RAM_ADDRESS       (see mchpGfxDrv.h)
        *    #define GFX_COMPRESSED_DATA_RAM_ADDRESS         (see mchpGfxDrv.h)
        *    
        *    Note that the two values:
        *    #define GFX_COMPRESSED_BUFFER_SIZE              (23212)
        *    #define GFX_DECOMPRESSED_BUFFER_SIZE            (26698)
        *    are derived from the output of Graphics Resource Converter (GRC).
        *    These values are the worst case sizes which is for 8BPP images.   
        *
        ********************************************************************/
        //#include "Configs/HWP_DA210_BRD_16PMP_WQVGAv1.h"
		
        /*********************************************************************
        * Hardware Configuration for
        * Universal Remote Control Board
        * Display TFT-G320240DTSW-69W-TP-E
        *    
        * Use this hardware profile when running 8, 4, and 1 BPP with the Universal  
        * Remote Control Board. Do not enable IPU in this hardware profile since
        * memory for buffers in not enough.
        *
        ********************************************************************/
        //#include "Configs/HWP_REMOTECONTROL_INT_QVGA.h"

    #else 
        #error "This demo only supports PIC24FJ256DA210 device"
    #endif

#else
    #error "This demo only supports PIC24F (specifically PIC24FJ256DA210) device"
#endif

#else

#if defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_INTERNAL_QVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_QVGAv1) || \
    defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_QVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_INT_QVGAv1.h"
#elif defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_INTERNAL_IPU_QVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_IPU_QVGAv1) || \
      defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_IPU_QVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_INT_IPU_QVGAv1.h"
#elif defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_WQVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_WQVGAv1) || \
      defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_WQVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_INT_WQVGAv1.h"
#elif defined (CFG_INCLUDE_DA210_BRD_16PMP_8BPP_IPU_WQVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_WQVGAv1.h"
#elif defined (CFG_INCLUDE_DA210_BRD_16PMP_4BPP_INTERNAL_IPU_WQVGAv1) || defined (CFG_INCLUDE_DA210_BRD_16PMP_1BPP_INTERNAL_IPU_WQVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_INT_IPU_WQVGAv1.h"
#elif defined (CFG_INCLUDE_REMOTECONTROL_8BPP_INTERNAL_QVGA) || defined (CFG_INCLUDE_REMOTECONTROL_4BPP_INTERNAL_QVGA) || \
      defined (CFG_INCLUDE_REMOTECONTROL_1BPP_INTERNAL_QVGA)
    #include "Configs/HWP_REMOTECONTROL_INT_QVGA.h"
#endif

#endif

    
