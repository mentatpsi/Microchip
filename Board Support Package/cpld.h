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

#ifndef CPLD_H_FILE
#define CPLD_H_FILE

/******************************************************************************************
  Summary:
    CPLD SPI channel selection.

  Description:
    The application can select what SPI channel will be used as the communication interface.
    It will also select the chipo select use for the device.


  Example:
    <code>
    // select SPI channel two for SPI Flash
    CPLDSetSPIFlashConfiguration(CPLD_SPI2);
    </code>
  
  Remarks:
    Only one SPI channel can be select for a device. SPI channels 2 and 2A are located on the same pins.  
    SPI channels 2A and 3A are only available on PIC32MX5xx/6xx/7xx series parts.

  *****************************************************************************************/
typedef enum
{
    // PIC32 SPI Channel 2A and chip select PORT G bit 9
    CPLD_SPI2A, 
    // PIC32 SPI Channel 3A and chip select PORT F bit 12
    CPLD_SPI3A,
    // PIC32 SPI Channel 2 and chip select PORT G bit 9
    CPLD_SPI2   /*DOM-IGNORE-BEGIN*/= CPLD_SPI2A,/*DOM-IGNORE-END*/
}CPLD_SPI_CONFIGURATION;

/******************************************************************************************
  Summary:
    CPLD graphics comtroller PMP bus configuration.

  Description:
    The application can select what PMP bus configuration, 8 or 16-bit data bus, when 
    interfacing with the grapihcs controller.


  Example:
    <code>
    // select 16-bit PMP data bus
    CPLDSetGraphicsConfiguration(CPLD_GFX_CONFIG_16BIT);
    </code>
  
  Remarks:
    None.
  *****************************************************************************************/
typedef enum
{
    // Configure the Graphics Controller to use 8-bit PMP data bus
    CPLD_GFX_CONFIG_8BIT,
    // Configure the Graphics Controller to use 16-bit PMP data bus
    CPLD_GFX_CONFIG_16BIT
}CPLD_GFX_CONFIGURATION;

/******************************************************************************************
  Summary:
    CPLD device configuration.

  Description:
    The CPLD can be configured to communicate to three different devices.  The application
    may call routine, CPLDGetDeviceConfiguration, to obtain what device the CPLD is configured to
    communicate with.


  Example:
    <code>
    // select 16-bit PMP data bus
    if(CPLDGetDeviceConfiguration() != CPLD_DEVICE_SPI_FLASH)
    {
        // error - not default configuration
    }
    </code>
  
  Remarks:
    None.
  *****************************************************************************************/
typedef enum
{
    CPLD_DEVICE_SPI_FLASH,  // SPI Flash 
    CPLD_DEVICE_WiFi,       // Zero G Wi-Fi 
    CPLD_DEVICE_ZIGBEE      // Zigbee/MiWi 
}CPLD_DEVICE_CONFIGURATION;
/**************************************************************************************
  Function:
    void CPLDInitialize(void)
    
  Summary:
    Initializes the control I/O to the CPLD and places the CPLD in a know state.
    any.
  Description:
    This routine configures the control I/O and places the CPLD in a known state.
    * Graphics Controller Bus - 8-bit PMP data interface.
    * SPI Channel - SPI2/SPI2A.
    * Chip Select - PORT G bit 9.
    * External Interrupt 1 or 3
    * Device - SPI Flash.
  Conditions:
    None.
  Input:
    None.
  Return:
    None.
  Example:
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // CPLD is configured in the default state
    </code>
  Remarks:
    None.
  **************************************************************************************/
void CPLDInitialize(void);

/**************************************************************************************
  Function:
    void CPLDSetGraphicsConfiguration(CPLD_GFX_CONFIGURATION configuration)
    
  Summary:
    Selects the PMP bus, 8 or 16-bit, interface to the grapihcs controller.
  Description:
    This routine sets the configuration pins on the graphics controller to select between
    an 8 or 16-bit data bus interface.
  Conditions:
    The initialization routine, CPLDInitialize, must be called.
  Input:
    configuration - the type of interface configuration.
  Return:
    None.
  Example:
    Setting the graphics controller to a 16-bit interface
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the graphics controller for a 16-bit PMP interface.
    CPLDSetGraphicsConfiguration(CPLD_GFX_CONFIG_16BIT);
    </code>

    Setting the graphics controller to a 8-bit interface
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the graphics controller for a 8-bit PMP interface.
    CPLDSetGraphicsConfiguration(CPLD_GFX_CONFIG_8BIT);
    </code>
  Remarks:
    The graphics controller interface configuration must be done before intializing the
    graphics controller.
  **************************************************************************************/
void CPLDSetGraphicsConfiguration(CPLD_GFX_CONFIGURATION configuration);

/**************************************************************************************
  Function:
    void CPLDSetSPIFlashConfiguration(CPLD_SPI_CONFIGURATION configuration)
    
  Summary:
    Selects the SPI Flash device.
  Description:
    This routine confgiures the CPLD to communicate to the SPI Flash device with the 
    selected SPI channel and chip select.
  Conditions:
    The initialization routine, CPLDInitialize, must be called.
  Input:
    configuration - the type of SPI channel used by the SPI Flash device.
  Return:
    None.
  Example:
    Setting CPLD to SPI Flash using SPI channle 2 and chip select PORT G bit 9
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the SPI Flash to use SPI channel 2 and chip select PORT G bit 9
    CPLDSetSPIFlashConfiguration(CPLD_SPI2);
    </code>

    Setting CPLD to SPI Flash using SPI channle 2A and chip select PORT G bit 9
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the SPI Flash to use SPI channel 2A and chip select PORT G bit 9
    CPLDSetSPIFlashConfiguration(CPLD_SPI2A);
    </code>

    Setting CPLD to SPI Flash using SPI channle 3A and chip select PORT F bit 12
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the SPI Flash to use SPI channel 3A and chip select PORT F bit 12
    CPLDSetSPIFlashConfiguration(CPLD_SPI3A);
    </code>
  Remarks:
    SPI channels 2 and 2A are located on the same pins.  SPI channels 2A and 3A are only
    available on PIC32MX5xx/6xx/7xx series parts.
  **************************************************************************************/
void CPLDSetSPIFlashConfiguration(CPLD_SPI_CONFIGURATION configuration);

/**************************************************************************************
  Function:
    void CPLDSetWiFiConfiguration(CPLD_SPI_CONFIGURATION configuration)
    
  Summary:
    Selects the Wi-Fi device.
  Description:
    This routine confgiures the CPLD to communicate to the Wi-Fi device with the 
    selected SPI channel, chip select and external interrupt or change notice.
  Conditions:
    The initialization routine, CPLDInitialize, must be called.
  Input:
    configuration - the type of SPI channel used by the Wi-Fi device.
  Return:
    None.
  Example:
    Setting CPLD to Wi-Fi using SPI channle 2, chip select PORT G bit 9 and external interrupt 3 
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the Wi-Fi to use SPI channel 2, chip select PORT G bit 9 and external interrupt 3
    CPLDSetWiFiConfiguration(CPLD_SPI2);
    </code>

    Setting CPLD to Wi-Fi using SPI channle 2A, chip select PORT G bit 9 and external interrupt 3
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the Wi-Fi to use SPI channel 2A, chip select PORT G bit 9 and external interrupt 3
    CPLDSetWiFiConfiguration(CPLD_SPI2A);
    </code>

    Setting CPLD to Wi-Fi using SPI channle 3A, chip select PORT F bit 12 and change notice 9
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the Wi-Fi to use SPI channel 3A, chip select PORT F bit 12 and change notice 9
    CPLDSetWiFiConfiguration(CPLD_SPI3A);
    </code>
  Remarks:
    SPI channels 2 and 2A are located on the same pins.  SPI channels 2A and 3A are only
    available on PIC32MX5xx/6xx/7xx series parts.
  **************************************************************************************/
void CPLDSetWiFiConfiguration(CPLD_SPI_CONFIGURATION configuration);

/**************************************************************************************
  Function:
    void CPLDSetZigBeeConfiguration(CPLD_SPI_CONFIGURATION configuration)
    
  Summary:
    Selects the Zigbee/MiWi device.
  Description:
    This routine confgiures the CPLD to communicate to the Zigbee/MiWi device with the 
    selected SPI channel, chip select and external interrupt or change notice.
  Conditions:
    The initialization routine, CPLDInitialize, must be called.
  Input:
    configuration - the type of SPI channel used by the Zigbee/MiWi device.
  Return:
    None.
  Example:
    Setting CPLD to Zigbee/MiWi using SPI channle 2, chip select PORT G bit 9 and external interrupt 3 
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the Zigbee/MiWi to use SPI channel 2, chip select PORT G bit 9 and external interrupt 3
    CPLDSetZigBeeConfiguration(CPLD_SPI2);
    </code>

    Setting CPLD to Zigbee/MiWi using SPI channle 2A, chip select PORT G bit 9 and external interrupt 3
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the Zigbee/MiWi to use SPI channel 2A, chip select PORT G bit 9 and external interrupt 3
    CPLDSetZigBeeConfiguration(CPLD_SPI2A);
    </code>

    Setting CPLD to Zigbee/MiWi using SPI channle 3A, chip select PORT F bit 12 and change notice 9
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    // configure the Zigbee/MiWi to use SPI channel 3A, chip select PORT F bit 12 and change notice 9
    CPLDSetZigBeeConfiguration(CPLD_SPI3A);
    </code>
  Remarks:
    SPI channels 2 and 2A are located on the same pins.  SPI channels 2A and 3A are only
    available on PIC32MX5xx/6xx/7xx series parts.
  **************************************************************************************/
void CPLDSetZigBeeConfiguration(CPLD_SPI_CONFIGURATION configuration);

/**************************************************************************************
  Function:
    CPLD_GFX_CONFIGURATION CPLDGetGraphicsConfiguration(void)
    
  Summary:
    Returns the selected PMP bus, 8 or 16-bit, interface to the grapihcs controller.
  Description:
    This routine gets the configuration of the PMP, 8 or 16-bit, data bus interface.
  Conditions:
    The initialization routine, CPLDInitialize, must be called.
  Input:
    None.
  Return:
    CPLD_GFX_CONFIG_8BIT - Graphics controller is configured for 8-bit PMP data bus interface.
    CPLD_GFX_CONFIG_16BIT - Graphics controller is configured for 16-bit PMP data bus interface.
  Example:
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    if(CPLDGetGraphicsConfiguration() != CPLD_GFX_CONFIG_8BIT)
    {
        // error - not setup as default
    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
CPLD_GFX_CONFIGURATION CPLDGetGraphicsConfiguration(void);

/**************************************************************************************
  Function:
    CPLD_SPI_CONFIGURATION CPLDGetSPIConfiguration(void)
    
  Summary:
    Returns the selected SPI Channel.
  Description:
    This routine returns the selected SPI channel.
  Conditions:
    The initialization routine, CPLDInitialize, must be called.
  Input:
    None.
  Return:
    CPLD_SPI2A - SPI Channel 2A with chip select PORT G bit 9 and external interrupt 1 or 3 
    CPLD_SPI3A - SPI Channel 3A with chip select PORT F bit 12 and change notice 9 
    CPLD_SPI2 - SPI Channel 2 with chip select PORT G bit 9 and external interrupt 1 or 3 
  Example:
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    if(CPLDGetSPIConfiguration() != CPLD_SPI2A)
    {
        // error - not setup as default

    }
    </code>
  Remarks:
    SPI channels 2 and 2A are located on the same pins.  SPI channels 2A and 3A are only
    available on PIC32MX5xx/6xx/7xx series parts.
  **************************************************************************************/
CPLD_SPI_CONFIGURATION CPLDGetSPIConfiguration(void);

/**************************************************************************************
  Function:
    CPLD_DEVICE_CONFIGURATION CPLDGetDeviceConfiguration(void)
    
  Summary:
    Returns the selected device.
  Description:
    This routine returns the selected CPLD device.
  Conditions:
    The initialization routine, CPLDInitialize, must be called.
  Input:
    None.
  Return:
    CPLD_DEVICE_SPI_FLASH   - SPI Flash.
    CPLD_DEVICE_WiFi        - Zero G 802.11 Wi-Fi.
    CPLD_DEVICE_ZIGBEE      - Zigbee/MiWi.
  Example:
    <code>
    // Initialize the CPLD
    CPLDInitialize();

    if(CPLDGetDeviceConfiguration() != CPLD_DEVICE_SPI_FLASH)
    {
        // error - not setup as default

    }
    </code>
  Remarks:
    None.
  **************************************************************************************/
CPLD_DEVICE_CONFIGURATION CPLDGetDeviceConfiguration(void);
#endif

