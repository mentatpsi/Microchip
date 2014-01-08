/*****************************************************************************
 *
 * Basic SPI Driver 
 *
 *****************************************************************************
 * FileName:        spi_drv.c
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

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "drv_spi.h"

// error checks
#if !defined(SPI_CHANNEL_1_ENABLE) && !defined(SPI_CHANNEL_2_ENABLE) && !defined(SPI_CHANNEL_3_ENABLE) && !defined(SPI_CHANNEL_4_ENABLE)   
    #warning "No SPI Channel defined! Please define in Hardware Profile"
#endif

static int spiMutex[4] = { 0, 0, 0, 0 };

/*****************************************************************************
 * void DRV_SPI_Initialize(const unsigned int channel, DRV_SPI_INIT_DATA *pData)
 *****************************************************************************/
void DRV_SPI_Initialize(const unsigned int channel, DRV_SPI_INIT_DATA *pData)
{

#ifdef SPI_CHANNEL_1_ENABLE        
    if (channel == 1)
    {

        DRV_SPI_STAT(1) = 0;

        DRV_SPI_CON(1) = 0;
        DRV_SPI_CONbits(1).MSTEN = SPI_MST_MODE_ENABLE;
        DRV_SPI_CONbits(1).MODE16 = (pData->mode)& 0x01;
        DRV_SPI_CONbits(1).CKP = pData->ckp;
        DRV_SPI_CONbits(1).CKE = pData->cke;
        DRV_SPI_CONbits(1).SMP = pData->smp;
    
    #ifdef __PIC32MX
        DRV_SPI_CONbits(1).MODE32 = ((pData->mode) >> 1)& 0x01;
        DRV_SPI_BRG(1) = pData->baudRate;
        DRV_SPI_CONbits(1).ON = SPI_MODULE_ENABLE;
    #else
        DRV_SPI_CON2(1) = 0;
        DRV_SPI_CONbits(1).PPRE= pData->primaryPrescale;
        DRV_SPI_CONbits(1).SPRE= pData->secondaryPrescale;
        DRV_SPI_STATbits(1).SPIEN = SPI_MODULE_ENABLE;
    #endif
        return;
    }    
#endif // #ifdef SPI_CHANNEL_1_ENABLE           
#ifdef SPI_CHANNEL_2_ENABLE        
    if (channel == 2)
    {

        DRV_SPI_STAT(2) = 0;

        DRV_SPI_CON(2) = 0;
        DRV_SPI_CONbits(2).MSTEN = SPI_MST_MODE_ENABLE;
        DRV_SPI_CONbits(2).MODE16 = (pData->mode)& 0x01;
        DRV_SPI_CONbits(2).CKP = pData->ckp;
        DRV_SPI_CONbits(2).CKE = pData->cke;
        DRV_SPI_CONbits(2).SMP = pData->smp;

    #ifdef __PIC32MX
        DRV_SPI_CONbits(2).MODE32 = ((pData->mode) >> 1)& 0x01;
        DRV_SPI_BRG(2) = pData->baudRate;
        DRV_SPI_CONbits(2).ON = SPI_MODULE_ENABLE;
    #else
        DRV_SPI_CON2(2) = 0;
        DRV_SPI_CONbits(2).PPRE= pData->primaryPrescale;
        DRV_SPI_CONbits(2).SPRE= pData->secondaryPrescale;
        DRV_SPI_STATbits(2).SPIEN = SPI_MODULE_ENABLE;
    #endif
        return;
    }
#endif // #ifdef SPI_CHANNEL_2_ENABLE           
#ifdef SPI_CHANNEL_3_ENABLE           
    if (channel == 3)
    {

        DRV_SPI_STAT(3) = 0;

        DRV_SPI_CON(3) = 0;
        DRV_SPI_CONbits(3).MSTEN = SPI_MST_MODE_ENABLE;
        DRV_SPI_CONbits(3).MODE16 = (pData->mode)& 0x01;
        DRV_SPI_CONbits(3).CKP = pData->ckp;
        DRV_SPI_CONbits(3).CKE = pData->cke;
        DRV_SPI_CONbits(3).SMP = pData->smp;

    #ifdef __PIC32MX
        DRV_SPI_CONbits(3).MODE32 = ((pData->mode) >> 1)& 0x01;
        DRV_SPI_BRG(3) = pData->baudRate;
        DRV_SPI_CONbits(3).ON = SPI_MODULE_ENABLE;
    #else
        DRV_SPI_CON2(3) = 0;
        DRV_SPI_CONbits(3).PPRE= pData->primaryPrescale;
        DRV_SPI_CONbits(3).SPRE= pData->secondaryPrescale;
        DRV_SPI_STATbits(3).SPIEN = SPI_MODULE_ENABLE;
    #endif                
        return;
    } 
#endif // #ifdef SPI_CHANNEL_3_ENABLE           
#ifdef SPI_CHANNEL_4_ENABLE           
    if (channel == 4)
    {

        DRV_SPI_STAT(4) = 0;

        DRV_SPI_CON(4) = 0;
        DRV_SPI_CONbits(4).MSTEN = SPI_MST_MODE_ENABLE;
        DRV_SPI_CONbits(4).MODE16 = (pData->mode)& 0x01;
        DRV_SPI_CONbits(4).CKP = pData->ckp;
        DRV_SPI_CONbits(4).CKE = pData->cke;
        DRV_SPI_CONbits(4).SMP = pData->smp;

    #ifdef __PIC32MX
        DRV_SPI_CONbits(4).MODE32 = ((pData->mode) >> 1)& 0x01;
        DRV_SPI_BRG(4) = pData->baudRate;
        DRV_SPI_CONbits(4).ON = SPI_MODULE_ENABLE;
    #else
        DRV_SPI_CON2(4) = 0;
        DRV_SPI_CONbits(4).PPRE= pData->primaryPrescale;
        DRV_SPI_CONbits(4).SPRE= pData->secondaryPrescale;
        DRV_SPI_STATbits(4).SPIEN = SPI_MODULE_ENABLE;
    #endif                
        return;
    } 
#endif // #ifdef SPI_CHANNEL_4_ENABLE

}

/*****************************************************************************
 * void SPIPut(unsigned int channel, unsigned char data)
 *****************************************************************************/
void SPIPut(unsigned int channel, unsigned char data)
{
    
#ifdef SPI_CHANNEL_1_ENABLE    
    if (channel == 1)
    {
    #ifdef __PIC32MX
        // Wait for free buffer
        while(!DRV_SPI_STATbits(1).SPITBE);
    #else
        // Wait for free buffer
        while(DRV_SPI_STATbits(1).SPITBF);
    #endif
        DRV_SPI_BUF(1) = data;
    
        // Wait for data byte
        while(!DRV_SPI_STATbits(1).SPIRBF);
        return;
    }    
#endif // #ifdef SPI_CHANNEL_1_ENABLE           
#ifdef SPI_CHANNEL_2_ENABLE           
    if (channel == 2)
    {
    #ifdef __PIC32MX
        // Wait for free buffer
        while(!DRV_SPI_STATbits(2).SPITBE);
    #else
        // Wait for free buffer
        while(DRV_SPI_STATbits(2).SPITBF);
    #endif

        DRV_SPI_BUF(2) = data;
        // Wait for data byte
        while(!DRV_SPI_STATbits(2).SPIRBF);
        return;
    }
#endif // #ifdef SPI_CHANNEL_2_ENABLE 
#ifdef SPI_CHANNEL_3_ENABLE           
    if (channel == 3)
    {
    #ifdef __PIC32MX
        // Wait for free buffer
        while(!DRV_SPI_STATbits(3).SPITBE);
    #else
        // Wait for free buffer
        while(DRV_SPI_STATbits(3).SPITBF);
    #endif

        DRV_SPI_BUF(3) = data;
        // Wait for data byte
        while(!DRV_SPI_STATbits(3).SPIRBF);
        return;
    }
#endif // #ifdef SPI_CHANNEL_3_ENABLE           
#ifdef SPI_CHANNEL_4_ENABLE           
    if (channel == 4)
    {
    #ifdef __PIC32MX
        // Wait for free buffer
        while(!DRV_SPI_STATbits(4).SPITBE);
    #else
        // Wait for free buffer
        while(DRV_SPI_STATbits(4).SPITBF);
    #endif

        DRV_SPI_BUF(4) = data;
        // Wait for data byte
        while(!DRV_SPI_STATbits(4).SPIRBF);
        return;
    }
#endif // #ifdef SPI_CHANNEL_4_ENABLE           
}

/*****************************************************************************
 * BYTE SPIGet (unsigned int channel)
 *****************************************************************************/
BYTE SPIGet (unsigned int channel)
{
    BYTE spiData = 0;

#ifdef SPI_CHANNEL_1_ENABLE           
    if (channel == 1)
    {
        spiData = DRV_SPI_BUF(1);
        return spiData;
    }
#endif //#ifdef SPI_CHANNEL_1_ENABLE           
#ifdef SPI_CHANNEL_2_ENABLE           
    if (channel == 2)
    {
        spiData = DRV_SPI_BUF(2);
        return spiData;
    }
#endif // #ifdef SPI_CHANNEL_2_ENABLE           
#ifdef SPI_CHANNEL_3_ENABLE           
    if (channel == 3)
    {
        spiData = DRV_SPI_BUF(3);
        return spiData;
    }    
#endif // #ifdef SPI_CHANNEL_3_ENABLE           
#ifdef SPI_CHANNEL_4_ENABLE
    if (channel == 4)
    {
        spiData = DRV_SPI_BUF(4);
        return spiData;
    }    
#endif // #ifdef SPI_CHANNEL_4_ENABLE           
    return spiData;   
}
/*****************************************************************************
 * int SPILock(unsigned int channel)
 *****************************************************************************/
int SPILock(unsigned int channel)
{
#ifdef SPI_CHANNEL_1_ENABLE           
    if (channel == 1)
    {
        if(!spiMutex[0])
        {
            spiMutex[0] = 1;
            return 1;
        }

        return 0; 
    }
#endif //#ifdef SPI_CHANNEL_1_ENABLE           
#ifdef SPI_CHANNEL_2_ENABLE           
    if (channel == 2)
    {
        if(!spiMutex[1])
        {
            spiMutex[1] = 1;
            return 1;
        }

        return 0; 
    }
#endif // #ifdef SPI_CHANNEL_2_ENABLE           
#ifdef SPI_CHANNEL_3_ENABLE           
    if (channel == 3)
    {
        if(!spiMutex[2])
        {
            spiMutex[2] = 1;
            return 1;
        }

        return 0; 
    }    
#endif // #ifdef SPI_CHANNEL_3_ENABLE           
#ifdef SPI_CHANNEL_4_ENABLE
    if (channel == 4)
    {
        if(!spiMutex[3])
        {
            spiMutex[3] = 1;
            return 1;
        }

        return 0; 
    }    
#endif // #ifdef SPI_CHANNEL_4_ENABLE           
    return -1;
}
/*****************************************************************************
 * int SPIUnLock(unsigned int channel)
 *****************************************************************************/
void SPIUnLock(unsigned int channel)
{
#ifdef SPI_CHANNEL_1_ENABLE           
    if (channel == 1)
    {
        spiMutex[0] = 0;
    }
#endif //#ifdef SPI_CHANNEL_1_ENABLE           
#ifdef SPI_CHANNEL_2_ENABLE           
    if (channel == 2)
    {
        spiMutex[1] = 0;
    }
#endif // #ifdef SPI_CHANNEL_2_ENABLE           
#ifdef SPI_CHANNEL_3_ENABLE           
    if (channel == 3)
    {
        spiMutex[2] = 0;
    }    
#endif // #ifdef SPI_CHANNEL_3_ENABLE           
#ifdef SPI_CHANNEL_4_ENABLE
    if (channel == 4)
    {
        spiMutex[3] = 0;
    }    
#endif // #ifdef SPI_CHANNEL_4_ENABLE           
}
