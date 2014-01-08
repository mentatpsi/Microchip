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
 ********************************************************************/

#ifndef CFG_INCLUDE_MPLAB_X

	/*********************************************************************
     * Hardware Configuration for
     * Exlporer 16 or PIC32 Starter kit   
     * Graphics PicTail Epson E-Paper
     * Display QVGA
     * 8-bit PMP
     ********************************************************************/
     //#include "Configs/HWP_EPD_EX16_8PMP_QVGAv1.h"

	/*********************************************************************
     * Hardware Configuration for
     * Exlporer 16 or PIC32 Starter kit   
     * Graphics PicTail Epson E-Paper
     * Display QVGA
     * 16-bit PMP
     ********************************************************************/
     #include "Configs/HWP_EPD_EX16_16PMP_QVGAv1.h"

	/*********************************************************************
     * Hardware Configuration for
     * Exlporer 16 or PIC32 Starter kit   
     * Graphics PicTail Epson E-Paper
     * Display QVGA
     * SPI
     ********************************************************************/
     //#include "Configs/HWP_EPD_EX16_SPI_QVGAv1.h"


#else

#ifdef CFG_INCLUDE_EPD_8PMP
     #include "Configs/HWP_EPD_EX16_8PMP_QVGAv1.h"
#endif

#ifdef CFG_INCLUDE_EPD_16PMP
     #include "Configs/HWP_EPD_EX16_16PMP_QVGAv1.h"
#endif

#ifdef CFG_INCLUDE_EPD_SPI
     #include "Configs/HWP_EPD_EX16_SPI_QVGAv1.h"
#endif

#endif

    
