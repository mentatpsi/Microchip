/******************************************************************************

FileName:        boot_commands.h
Dependencies:    None
Processor:       PIC24/dsPIC30/dsPIC33/PIC32MX
Compiler:        C30/C32
Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the "Company") for its PICmicro(R) Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

Change History
  Rev      Description
  -----    ----------------------------------
*******************************************************************************/
#ifndef BOOT_COMMANDS_H
#define BOOT_COMMANDS_H
      
// Size of the USB buffer
#define USB_BUFFER_SIZE 2048

// Return types for Bootloader
typedef enum {
    BOOT_SUCCESS,
    BOOT_DEVICE_DETACHED,
    BOOT_UNKNOWN_ERROR
} BOOT_LOADER_RETURN_CODES;

/****************************************************************************
  Function:
    BOOT_LOADER_RETURN_CODES EnterBootLoaderMode(void* handle)

  Description:
    This routine is the main state machine for the bootloader.

  Precondition:
    The device is attached and has been detected.

  Parameters:
    void* handle            - A generic pointer to the attached device

  Returns:
    BOOT_SUCCESS            - Device has been successfully Booted
                              (may still have a validation error, though)
    BOOT_DEVICE_DETACHED    - Device has been detached before Bootloading finished
    BOOT_UNKNOWN_ERROR      - An unknown error occurred with USB transfers
***************************************************************************/
extern BOOT_LOADER_RETURN_CODES EnterBootLoaderMode(void* handle);

/****************************************************************************
  Function:
    void DeviceDetached()

  Description:
    This routine notifies the bootloader state machine that the device
    has been detached.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None
***************************************************************************/
void DeviceDetached(void);

#endif
