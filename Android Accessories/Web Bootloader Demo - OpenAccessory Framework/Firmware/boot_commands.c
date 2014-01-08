/******************************************************************************

FileName:        boot_commands.c
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

#include "USB/usb.h"
#include "USB/usb_host_android.h"
#include "boot_commands.h"
#include "custom_bin_boot.h"

// Bootloader state machine's states
typedef enum {
    BOOT_START,
    BOOT_ENTER_WAIT,
    READ_FILE_REQ_SEND,
    READ_FILE_REQ_WAIT,
    READING_DATA,
    READING_DATA_WAIT,
    FLASHING_DATA,
    LOAD_COMPLETE_RSP_SEND,
    LOAD_COMPLETE_RSP_WAIT,
    BOOT_ERROR
} BOOT_LOADER_STATE;


enum {
    START_BOOT_REQ      = 1,
    READ_FILE_REQ       = 2,
    LOAD_COMPLETE_RSP   = 3
} BOOT_LOADER_COMMANDS;

static BYTE data[USB_BUFFER_SIZE];
static BOOL device_attached;

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
BOOT_LOADER_RETURN_CODES EnterBootLoaderMode(void* handle)
{
    BYTE errorCode;
    DWORD size;
    DWORD_VAL status;
    BOOT_LOADER_STATE state = BOOT_START;
    
    // Device was attached when this function was called
    device_attached = TRUE;
    
    // Run loop
    while(1)
    {
        //Keep the USB stack running
        USBTasks();
        
        // Return, if the device has been detached
        if(device_attached == FALSE)
        {
           return BOOT_DEVICE_DETACHED;
        }
        
        // Bootloader state machine
        switch(state)
        {
            case BOOT_START:
                // Try to read from the attached Android device
                if(AndroidAppRead(handle, data, sizeof(data)) != USB_SUCCESS)
                {
                    state = BOOT_ERROR;             // update state to error, if unsuccessful
                }
                else
                {
                    state = BOOT_ENTER_WAIT;        // Otherwise, wait until read is complete
                }
                break;
    
            case BOOT_ENTER_WAIT:
                // If read from Android device is complete
                if(AndroidAppIsReadComplete(handle, &errorCode, &size) == TRUE)
                {
                    // Then check to see if there was an error
                    if(errorCode != USB_SUCCESS)
                    {
                        state = BOOT_ERROR;             // Update state to error if there was an error
                    }
                    else
                    {
                        // Otherwise check the first byte in the packet to see if it was START_BOOT_REQ
                        if(data[0] != START_BOOT_REQ)
                        {
                            state = BOOT_ERROR;         // If not, then an error occurred
                        }
                        else
                        {
                            state = READ_FILE_REQ_SEND; // If so, then update state to send READ_FILE_REQ
                        }
                    }                
                }
                break;
    
            case READ_FILE_REQ_SEND:
                // Set the first byte of the packet to READ_FILE_REQ
                data[0] = READ_FILE_REQ;
                
                // And send packet to the Android device
                if(AndroidAppWrite(handle, data, 1) != USB_SUCCESS)
                {
                    state = BOOT_ERROR;             // If unsuccessful, then update state to error
                }
                else
                {
                    state = READ_FILE_REQ_WAIT;     // Otherwise wait until write is complete
                }
                break;
    
            case READ_FILE_REQ_WAIT:
                // If the write is complete
                if(AndroidAppIsWriteComplete(handle, &errorCode, &size) == TRUE)
                {
                    // Then check for errors
                    if(errorCode != USB_SUCCESS)
                    {
                        state = BOOT_ERROR;     // If there was an error, update state to error
                    }
                    else
                    {
                        state = READING_DATA;   // Otherwise start reading data that contains flash update
                    }
                }
                break;
    
            case READING_DATA:
                // Try to read enough to fill the buffer
                if(AndroidAppRead(handle, data, sizeof(data)) != USB_SUCCESS)
                {
                    state = BOOT_ERROR;         // Update state to error if unsuccessful
                }
                else
                {
                    state = READING_DATA_WAIT;  // Otherwise wait until the read is successful
                }
                break;
    
            case READING_DATA_WAIT:
                // If the read was successful
                if(AndroidAppIsReadComplete(handle, &errorCode, &size) == TRUE)
                {
                    // Then check to see if there were any errors
                    if(errorCode != USB_SUCCESS)
                    {
                        state = BOOT_ERROR;         // If so, then update state to error
                    }
                    else
                    {
                        state = FLASHING_DATA;      // Otherwise start re-flashing the new data into program memory
                    }                
                }
                break;
    
            case FLASHING_DATA:
                // Write the data from the buffer to program memory
                status.Val = CustomBinWrite(data,size);
                
                // If there is more to read from the Android device
                if(status.Val == DONE_WRITING_BUFFER)
                {
                    state = READING_DATA;           // Then update state to reading data
                }
                else
                {
                    state = LOAD_COMPLETE_RSP_SEND; // Otherwise update state to send load complete response
                }
                break;
                
            case LOAD_COMPLETE_RSP_SEND:
                // Write LOAD_COMPLETE_RSP command to first byte, and status of the write to the rest of the packet
                data[0] = LOAD_COMPLETE_RSP;
                data[1] = status.byte.MB;
                data[2] = status.byte.UB;
                data[3] = status.byte.HB;
                data[4] = status.byte.LB;
                
                // Send to Android device
                if(AndroidAppWrite(handle, data, 5) != USB_SUCCESS)
                {
                    state = BOOT_ERROR;             // If there are any errors, update state to error
                }
                else
                {
                    state = LOAD_COMPLETE_RSP_WAIT; // Otherwise, wait until write is complete
                }
                break;
    
            case LOAD_COMPLETE_RSP_WAIT:
                // If the write is finished
                if(AndroidAppIsWriteComplete(handle, &errorCode, &size) == TRUE)
                {
                    // Then check for errors
                    if(errorCode != USB_SUCCESS)
                    {
                        state = BOOT_ERROR;         // If there was an error, update state to error
                    }
                    else
                    {
                        return BOOT_SUCCESS;        // Otherwise return with BOOT_SUCCESS
                    }
                }
                break;
    
            case BOOT_ERROR:
                // If there was an error, then return BOOT_UNKNOWN_ERROR
                return BOOT_UNKNOWN_ERROR;
                break;
        } //switch
    } //while(1)
    
    return BOOT_UNKNOWN_ERROR;
}

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
void DeviceDetached()
{
    device_attached = FALSE;        // Update device_attached flag to false if device was detached
}    
