/*******************************************************************************
  CUPLD 

  Summary:
    This file contains the interface definition for the CUPLD controller.
    
  Description:
    This library provides a low-level abstraction of the CUPLD device.  It 
    can be used to simplify low-level access to the device without the necessity of 
    interacting directly with the communication module's registers, thus hiding differences from 
    one serial device variant to another.
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
FileName:       CUPLD.c
Dependencies:   See includes
Processor:      PIC32MX

Complier:       Microchip MPLAB C32 v1.06 or higher
Company:        Microchip Technology Inc.

Copyright © 2008-2009 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

/********************************************************************
 Section: Includes 
********************************************************************/
#include "HardwareProfile.h"

/********************************************************************
 This code is specific to the MultiMedia Developement Board. 
********************************************************************/
#if defined (USE_XC2C64A)

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include <cpld.h>
#include <stdint.h>

/********************************************************************
 Section: Code 
********************************************************************/
inline void SetSPIChannel(CPLD_SPI_CONFIGURATION channel_config)
{
    switch(channel_config)
    {
    case CPLD_SPI2A:
        PORTSetBits(IOPORT_G,  BIT_12);     
        break;

    case CPLD_SPI3A:
        PORTClearBits(IOPORT_G,  BIT_12);     
        break;
    }

}
inline CPLD_SPI_CONFIGURATION GetSPIChannel(void)
{
    if(PORTReadBits(IOPORT_G, BIT_12))
        return CPLD_SPI3A;

    return CPLD_SPI2A;
}
/********************************************************************
 Funciton: void CUPLDInitialize(void)
********************************************************************/
void CPLDInitialize(void)
{
    
    CPLDSetGraphicsConfiguration(CPLD_GFX_CONFIG_8BIT);
    CPLDSetSPIFlashConfiguration(CPLD_SPI2);

    PORTSetPinsDigitalOut(IOPORT_G, BIT_14 | BIT_12);
    PORTSetPinsDigitalOut(IOPORT_A, BIT_7 | BIT_6);

}
/********************************************************************
 Funciton: void CUPLDConfigure(CPLD_CONFIGURATION configuration
********************************************************************/
void CPLDSetGraphicsConfiguration(CPLD_GFX_CONFIGURATION configuration)
{
    if(configuration == CPLD_GFX_CONFIG_16BIT)
    {
        PORTSetBits(IOPORT_G,  BIT_14);     
    }else
    {
        PORTClearBits(IOPORT_G,  BIT_14);     
    }
}
/********************************************************************
 Funciton: void CUPLDConfigure(CPLD_CONFIGURATION configuration
********************************************************************/
void CPLDSetSPIFlashConfiguration(CPLD_SPI_CONFIGURATION configuration)
{
    SetSPIChannel(configuration);
    PORTClearBits(IOPORT_A,  BIT_7 | BIT_6);     
}
/********************************************************************
 Funciton: void CUPLDConfigure(CPLD_CONFIGURATION configuration
********************************************************************/
void CPLDSetWiFiConfiguration(CPLD_SPI_CONFIGURATION configuration)
{
    SetSPIChannel(configuration);
    PORTClearBits(IOPORT_A,  BIT_7);     
    PORTSetBits(IOPORT_A,  BIT_6);     
}
/********************************************************************
 Funciton: void CUPLDConfigure(CPLD_CONFIGURATION configuration
********************************************************************/
void CPLDSetZigBeeConfiguration(CPLD_SPI_CONFIGURATION configuration)
{
    SetSPIChannel(configuration);
    PORTClearBits(IOPORT_A,  BIT_6);     
    PORTSetBits(IOPORT_A,  BIT_7);     
}
/********************************************************************
 Funciton: void CUPLDConfigure(CPLD_CONFIGURATION configuration
********************************************************************/
CPLD_GFX_CONFIGURATION CPLDGetGraphicsConfiguration(void)
{
    if(PORTReadBits(IOPORT_G, BIT_14))
        return CPLD_GFX_CONFIG_16BIT;

    return CPLD_GFX_CONFIG_8BIT;

}
/********************************************************************
 Funciton: void CPLDGetSPIConfiguration(CPLD_CONFIGURATION configuration
********************************************************************/
CPLD_SPI_CONFIGURATION CPLDGetSPIConfiguration(void)
{
    return GetSPIChannel();
}
/********************************************************************
 Funciton: void CPLDGetSPIConfiguration(CPLD_CONFIGURATION configuration
********************************************************************/
CPLD_DEVICE_CONFIGURATION CPLDGetDeviceConfiguration(void)
{
    uint8_t device;

    device = (uint8_t)PORTReadBits(IOPORT_A, (BIT_7 | BIT_6)) & (BIT_7 | BIT_6);
    device >>= 6;

    switch(device)
    {
    case 0:
        return CPLD_DEVICE_SPI_FLASH;
    case 1:
        return CPLD_DEVICE_WiFi;
    case 2:
        return CPLD_DEVICE_ZIGBEE;
    }

    return -1;
}

#endif //#if defined (USE_XC2C64A)
