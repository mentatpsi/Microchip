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

	/*********************************************************************
     * Hardware Configuration for
     * Exlporer 16 or PIC32 Starter kit   
     * Graphics PicTail v3e
     * Display VGA
     ********************************************************************/
		//#include "Configs/HWP_GFXv3e_EX16_8PMP_VGAv1.h"
        #include "Configs/HWP_GFXv3e_EX16_16PMP_VGAv1.h"
	/*********************************************************************
     * Hardware Configuration for
     * Exlporer 16 or PIC32 Starter kit   
     * Graphics PicTail v3e
     * Display VGA
     * AR1020 Touch Controller
     ********************************************************************/
          //#include "Configs/HWP_GFXv3e_EX16_16PMP_AR1020_VGAv1.h"

    /*********************************************************************
     * Hardware Configuration for 
     * Explorer 16 or PIC32 Starter kit
     * Graphics PicTail v3e
     * Display WVGA
     ********************************************************************/
	    //#include "Configs/HWP_GFXv3e_EX16_8PMP_WVGAv1.h"
          //#include "Configs/HWP_GFXv3e_EX16_16PMP_WVGAv1.h"


#elif defined (__PIC32MX__) 

     /*********************************************************************
     * Hardware Configuration for
     * PIC32 Starter kit   
     * Graphics PicTail v3e
     * Display VGA
     * AR1020 Touch Controller
     ********************************************************************/
    //#include "Configs/HWP_GFXv3e_PIC_SK_16PMP_AR1020_VGAv1.h"

    /*********************************************************************
     * Hardware Configuration for
     * Exlporer 16 or PIC32 Starter kit   
     * Graphics PicTail v3e
     * Display VGA
     ********************************************************************/
    #include "Configs/HWP_GFXv3e_EX16_8PMP_VGAv1.h"
    //#include "Configs/HWP_GFXv3e_EX16_16PMP_VGAv1.h"
	//#include "Configs/HWP_GFXv3e_PIC_SK_8PMP_VGAv1.h"
    //#include "Configs/HWP_GFXv3e_PIC_SK_16PMP_VGAv1.h"

    /*********************************************************************
     * Hardware Configuration for 
     * Explorer 16 or PIC32 Starter kit
     * Graphics PicTail v3e
     * Display WVGA
     ********************************************************************/
	//#include "Configs/HWP_GFXv3e_EX16_8PMP_WVGAv1.h"
    //#include "Configs/HWP_GFXv3e_EX16_16PMP_WVGAv1.h"
	//#include "Configs/HWP_GFXv3e_PIC_SK_8PMP_WVGAv1.h"
    //#include "Configs/HWP_GFXv3e_PIC_SK_16PMP_WVGAv1.h"


#endif

#else
/*********************************************************************
 * Hardware Configuration for
 * Explorer 16 with a PIM
 * GFX version 3e 
 * 8 or 16-bit PMP
 * QVGA or WQVGA
 ********************************************************************/
#if defined(CFG_INCLUDE_GFXv3e_EX16_8PMP_VGAv1)
    #include "Configs/HWP_GFXv3e_EX16_8PMP_VGAv1.h"
#elif defined(CFG_INCLUDE_GFXv3e_EX16_16PMP_VGAv1)
    #include "Configs/HWP_GFXv3e_EX16_16PMP_VGAv1.h"
#elif defined(CFG_INCLUDE_GFXv3e_EX16_8PMP_WVGAv1)
    #include "Configs/HWP_GFXv3e_EX16_8PMP_WVGAv1.h"
#elif defined(CFG_INCLUDE_GFXv3e_EX16_16PMP_WVGAv1)
    #include "Configs/HWP_GFXv3e_EX16_16PMP_WVGAv1.h"
#elif defined(CFG_INCLUDE_GFXv3e_EX16_16PMP_AR1020_VGAv1)
    #include "Configs/HWP_GFXv3e_EX16_16PMP_AR1020_VGAv1.h"
/*********************************************************************
 * Hardware Configuration for
 * GFX version 3 with a Starter Kit 
 * 8 or 16-bit PMP
 * QVGA or WQVGA
 ********************************************************************/
#elif defined(CFG_INCLUDE_GFXv3e_PIC_SK_8PMP_VGAv1)
    #include "Configs/HWP_GFXv3e_PIC_SK_8PMP_VGAv1.h"
#elif defined(CFG_INCLUDE_GFXv3e_PIC_SK_16PMP_VGAv1)
    #include "Configs/HWP_GFXv3e_PIC_SK_16PMP_VGAv1.h"
#elif defined(CFG_INCLUDE_GFXv3e_PIC_SK_8PMP_WVGAv1)
    #include "Configs/HWP_GFXv3e_PIC_SK_8PMP_WVGAv1.h"
#elif defined(CFG_INCLUDE_GFXv3e_PIC_SK_16PMP_WVGAv1)
    #include "Configs/HWP_GFXv3e_PIC_SK_16PMP_WVGAv1.h"
#else
#error "Please make sure that you have a CFG_INCLUDE_xxxx as a compile preprocessor define in the MPLAB X configuration."
#endif

#endif

    
