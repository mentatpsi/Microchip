/*****************************************************************************
 *
 * Basic SPI Driver 
 *
 *****************************************************************************
 * FileName:        spi_drv.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 02/10/11	   	...
 *****************************************************************************/

#ifndef _DRV_SPI_H
#define _DRV_SPI_H

#include "GenericTypeDefs.h"

typedef struct
{
    int    channel;
#ifdef __PIC32MX
	int	    baudRate;
	int     dummy;
#else
	int     primaryPrescale;
	int     secondaryPrescale;
#endif
	char    cke;
	char    ckp;
	char    smp;
	char    mode;
	
} DRV_SPI_INIT_DATA;

/* SPI SFR definitions. i represents the SPI
   channel number.
   valid i values are: 1, 2, 3
   for PIC24F & PIC32MX device families
*/
    #define DRV_SPI_STAT(i)      SPI##i##STAT
    #define DRV_SPI_STATbits(i)  SPI##i##STATbits
#ifdef __PIC32MX__
    #define DRV_SPI_CON(i)       SPI##i##CON
    #define DRV_SPI_CONbits(i)   SPI##i##CONbits
    #define DRV_SPI_BRG(i)       SPI##i##BRG
    #define DRV_SPI_BRGbits(i)   SPI##i##BRGbits
#else
    #define DRV_SPI_CON(i)       SPI##i##CON1
    #define DRV_SPI_CONbits(i)   SPI##i##CON1bits
    #define DRV_SPI_CON2(i)      SPI##i##CON2
    #define DRV_SPI_CON2bits(i)  SPI##i##CON2bits
#endif
    #define DRV_SPI_BUF(i)       SPI##i##BUF
    #define DRV_SPI_BUFbits(i)   SPI##i##BUFbits

/* macros that defines the SPI signal polarities */
    #define SPI_CKE_IDLE_ACT     0        // data change is on active clock to idle clock state
    #define SPI_CKE_ACT_IDLE     1        // data change is on idle clock to active clock state

    #define SPI_CKP_ACT_HIGH     0        // clock active state is high level
    #define SPI_CKP_ACT_LOW      1        // clock active state is low level

    #define SPI_SMP_PHASE_MID    0        // master mode data sampled at middle of data output time 
    #define SPI_SMP_PHASE_END    1        // master mode data sampled at end of data output time

    #define SPI_MODE_8BITS       0        // communication mode set at 8-bits
    #define SPI_MODE_16BITS      1        // communication mode set at 16-bits
    #define SPI_MODE_32BITS      2        // communication mode set at 32-bits

    #define SPI_MST_MODE_ENABLE  1        // SPI master mode enabled
    #define SPI_MST_MODE_DISABLE 0        // SPI master mode disabled, use SPI in slave mode

    #define SPI_MODULE_ENABLE    1        // Enable SPI 
    #define SPI_MODULE_DISABLE   0        // Disable SPI 



void DRV_SPI_Initialize(unsigned int channel, DRV_SPI_INIT_DATA *pData);
void SPIPut(unsigned int channel, unsigned char data);
BYTE SPIGet(unsigned int channel);
int SPILock(unsigned int channel);
void SPIUnLock(unsigned int channel);
#endif // #ifndef _DRV_SPI_H


