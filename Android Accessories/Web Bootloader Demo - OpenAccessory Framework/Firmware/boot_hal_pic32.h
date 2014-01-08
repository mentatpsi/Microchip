/******************************************************************************

FileName:        boot_hal_pic32.h
Dependencies:    None
Processor:       PIC32MX
Compiler:        C32
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
#ifndef __BOOT_HAL_PIC24_H_
#define __BOOT_HAL_PIC24_H_

// Number of bytes for each address in program memory
#define BYTES_PER_ADDRESS   1

// Number of bytes for each instruction
#define BYTES_PER_INSTRUCTION 4

/****************************************************************************
  Function:
    void EraseBootRegions(void)

  Description:
    This routine will erase the entire application memory space.

  Precondition:
    Each boot region is page aligned (Starts on address divisible by
    FLASH_BLOCK_SIZE and ends on address divisible by FLASH_BLOCK_SIZE)

  Parameters:
    None

  Returns:
    None
***************************************************************************/
void EraseBootRegions(void);

/****************************************************************************
  Function:
    void WriteBlock(BOOT_REGION *entry, BYTE *buffer)

  Description:
    This routine will write the instructions within the buffer to the program
    memory address range.

  Precondition:
    The number of bytes within *buffer will fit the entire range given by *entry

  Parameters:
    BOOT_REGION *entry  - The address range that we will write to
    BYTE *buffer        - A pointer to the buffer that holds the instructions
                          we will be writing

  Returns:
    None
***************************************************************************/
void WriteBlock(BOOT_REGION *entry, BYTE *buffer);

/****************************************************************************
  Function:
    void WriteRomData(BOOT_REGION *entry, BYTE *buffer)

  Description:
    This routine will write the data within the buffer to the program
    memory address range.

  Precondition:
    The number of bytes within *buffer will fit the entire range given by *entry

  Parameters:
    BOOT_REGION *entry  - The address range that we will write to
    BYTE *buffer        - A pointer to the buffer that holds the data we will be writing

  Returns:
    None
***************************************************************************/
#define WriteRomData(a, b)  WriteBlock(a, b)

/****************************************************************************
  Function:
    UINT32 ValidateChecksum(ROM BOOT_REGION* entry)

  Description:
    This routine will calculate the check sum for the boot region and check
    it against the validation code at the beginning of the boot region

  Precondition:
    The validation code is saved at the beginning of the region.

  Parameters:
    BOOT_REGION *entry  - The boot region that we are calculating check sum for

  Returns:
    None
***************************************************************************/
UINT32 ValidateChecksum(ROM BOOT_REGION* entry);

#endif
