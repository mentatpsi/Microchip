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
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT 
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
 * 10/03/06             Original, copied from Compiler.h
 * 06/25/09             dsPIC & PIC24H support 
 * 09/15/09             Added PIC24FJ256DA210 Development Board Support
 * 06/02/11             Added MPLAB X Support
 ********************************************************************/

#ifndef CFG_INCLUDE_MPLAB_X

#if defined (__PIC24F__) || defined(__dsPIC33F__) || defined(__PIC24H__)

	#if defined (__PIC24FJ256DA210__)
		/*********************************************************************
	     * Hardware Configuration for 
	     * PIC24FJ256DA210 Development Board
	     * Display TFT-G240320LTSW-118W-E
	     ********************************************************************/
		#include "Configs/HWP_DA210_BRD_INT_QVGAv1.h"

		/*********************************************************************
	     * Hardware Configuration for 
	     * PIC24FJ256DA210 Development Board
	     * Display TFT-640480-8-E
		 * Software resistive touch screen driver
	     ********************************************************************/
        //#include "Configs/HWP_DA210_BRD_16PMP_VGAv1.h"

		/*********************************************************************
	     * Hardware Configuration for 
	     * PIC24FJ256DA210 Development Board
	     * Display TFT-640480-8-E
         * AR1020 Touch Screen controller
	     ********************************************************************/
        //#include "Configs/HWP_DA210_BRD_16PMP_AR1020_VGAv1.h"

		/*********************************************************************
	     * Hardware Configuration for 
	     * PIC24FJ256DA210 Development Board
	     * Display TFT-800480-33-E
		 * Software resistive touch screen driver
	     ********************************************************************/
        //#include "Configs/HWP_DA210_BRD_16PMP_WVGAv1.h"
		
		/*********************************************************************
	     * Hardware Configuration for 
	     * PIC24FJ256DA210 Development Board
	     * Display TFT-800480-33-E
         * AR1020 Touch Screen controller
	     ********************************************************************/
        //#include "Configs/HWP_DA210_BRD_16PMP_AR1020_WVGAv1.h"
	#endif

#elif defined (__PIC32MX__) 

    /*********************************************************************
     * Hardware Configuration for 
     * LCC Graphics PicTail
     * Display TFT-G240320LTSW-118W-E
     ********************************************************************/
    #include "Configs/HWP_LCC_PIC_SK_INT_QVGAv1.h"
    //#include "Configs/HWP_LCC_EX16_INT_QVGAv1.h"
#endif

#else
/*********************************************************************
 * This is the auto generated configuration based on the MPLAB X
 * project configuration.
 ********************************************************************/

/*********************************************************************
 * Hardware Configuration for
 * DA210 Developement board
 * Internal RAM
 * QVGA
 ********************************************************************/
#if defined(CFG_INCLUDE_DA210_BRD_INTERNAL_QVGAv1)
    #include "Configs/HWP_DA210_BRD_INT_QVGAv1.h"
/*********************************************************************
 * Hardware Configuration for
 * DA210 Developement board
 * 16 bit PMP
 * VGA
 ********************************************************************/
#elif defined(CFG_INCLUDE_DA210_BRD_16PMP_VGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_VGAv1.h"
/*********************************************************************
 * Hardware Configuration for
 * DA210 Developement board
 * 16 bit PMP
 * VGA
 * AR1020
 ********************************************************************/
#elif defined(CFG_INCLUDE_DA210_BRD_16PMP_AR1020_VGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_AR1020_VGAv1.h"
/*********************************************************************
 * Hardware Configuration for
 * DA210 Developement board
 * 16 bit PMP
 * VGA
 * AR1020
 ********************************************************************/
#elif defined(CFG_INCLUDE_DA210_BRD_16PMP_WVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_WVGAv1.h"
/*********************************************************************
 * Hardware Configuration for
 * DA210 Developement board
 * 16 bit PMP
 * VGA
 * AR1020
 ********************************************************************/
#elif defined(CFG_INCLUDE_DA210_BRD_16PMP_AR1020_WVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_AR1020_WVGAv1.h"
/*********************************************************************
 * Hardware Configuration for
 * LCC Developement board
 * Internal Memory
 * QVGA
 ********************************************************************/
#elif defined(CFG_INCLUDE_LCC_SK_INT_QVGAv1)
    #include "Configs/HWP_LCC_PIC_SK_INT_QVGAv1.h"
#else

#error "Please make sure that you have a CFG_INCLUDE_xxxx as a compile preprocessor define in the MPLAB X configuration."
#endif

#endif

    
