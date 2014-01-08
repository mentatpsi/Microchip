/******************************************************************************

FileName:        custom_bin_boot.h
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
#ifndef CUSTOM_BIN_BOOT_H
#define CUSTOM_BIN_BOOT_H

#include "GenericTypeDefs.h"
#include "Compiler.h"

typedef struct __attribute__((packed)){
    UINT32 addressStart;
    UINT32 addressEnd;   
} BOOT_REGION;

typedef enum
{
    VALIDATION_SUCCESSFUL = 0,
    VALIDATION_FAIL,
    DONE_WRITING_BUFFER,
    UNKNOWN_ERROR
} BOOT_STATUS;

/****************************************************************************
  Function:
    BOOT_STATUS CustomBinWrite(BYTE *buffer, DWORD size)

  Description:
    This routine is the main state machine for the binary file reader.

  Precondition:
    The device is attached and has sent data from the binary file to host.

  Parameters:
    BYTE *buffer            - The pointer to the data from the binary file
    DWORD size              - The number of bytes available at that pointer

  Returns:
    VALIDATION_SUCCESSFUL   - Verifying the writes to program memory was successful
    VALIDATION_FAIL         - Verifying the writes to program memory was unsuccessful
    DONE_WRITING_BUFFER     - Reached the end of the buffer but not end of binary file,
                              need another USB read
    UNKNOWN_ERROR           - Unknown error occurred while reading binary file
***************************************************************************/
BOOT_STATUS CustomBinWrite(BYTE *buffer, DWORD size);

/****************************************************************************
  Function:
    BOOL ReadObject(BYTE* binary_file, UINT32 bytes_left_in_buffer, void* object, UINT32 size_of_object, UINT32 *bytes_to_read)

  Description:
    This routine reads and copies objects from the binary file and handles partial reads if
    the end of the buffer is reached. It will pick up reading the object where it
    left off when the buffer is filled again. It will return true if a complete read
    occurs, and false for partial reads.

  Precondition:
    This function cannot be called with a new object until a read has completed.

  Parameters:
    BYTE* binary_file           - A pointer to the buffer that contains the binary file
    UINT32 bytes_left_in_buffer - The number of bytes left inside this buffer
    void* object                - A pointer to the object that we are copying to
    UINT32 size_of_object       - The size of the object we're copying
    UINT32 *bytes_to_read       - A pointer to the variable we are updating with the number of bytes read

  Returns:
    BOOL    - True if reading the object has completed, false if only a partial read
***************************************************************************/
BOOL ReadObject(BYTE* binary_file, UINT32 bytes_left_in_buffer, void* object, UINT32 size_of_object, UINT32* bytes_read);

/****************************************************************************
  Function:
    BOOL ValidationMethod(void)

  Description:
    This routine properly saves the user header region of the binary file.

  Precondition:
    None

  Parameters:
    None

  Returns:
    BOOL - True if validation passes, false otherwise
***************************************************************************/
BOOL ValidationMethod(void);

#endif
