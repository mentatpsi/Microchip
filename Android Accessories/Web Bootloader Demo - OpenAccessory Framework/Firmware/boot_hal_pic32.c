/******************************************************************************

FileName:        boot_hal_pic32.c
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

#include "Compiler.h"
#include "GenericTypedefs.h"
#include "HardwareProfile.h"

#include "custom_bin_boot.h"
#include "boot_config.h"
#include "boot_hal_pic32.h"

extern ROM BOOT_REGION my_boot_regions[NUM_BOOT_REGIONS];

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
void EraseBootRegions(void)
{
    DWORD address;
    BYTE i;

    // Iterate through each boot region
    for(i = 0;i < NUM_BOOT_REGIONS; i++)
    {
        // Iterate through each flash block of this boot region
        for(address = my_boot_regions[i].addressStart; address < my_boot_regions[i].addressEnd;address += (FLASH_BLOCK_SIZE))
        {
            NVMErasePage((void *)address);  // Erase page
        }    
    }
}

/****************************************************************************
  Function:
    void WriteBlock(BOOT_REGION *entry, BYTE *buffer)

  Description:
    This routine will write the instructions within the buffer to the program
    memory address range.

  Precondition:
    The number of bytes within *buffer will fit the entire range given by *entry
    *entry is word aligned byte addresses

  Parameters:
    BOOT_REGION *entry  - The address range that we will write to
    BYTE *buffer        - A pointer to the buffer that holds the instructions
                          we will be writing

  Returns:
    None
***************************************************************************/
void WriteBlock(BOOT_REGION *entry, BYTE *buffer)
{
    DWORD_VAL data;
    DWORD address;

    // Iterate through region
    for(address = entry->addressStart; address < entry->addressEnd; address += 4)
    {
        // Byte pointer might not be word aligned, so read per byte to get 32-bit value (little endian)
        data.byte.LB = *buffer++;
        data.byte.HB = *buffer++;
        data.byte.UB = *buffer++;
        data.byte.MB = *buffer++;
        
	    NVMWriteWord( (void *)address, data.Val );	// Write 4-byte word
    }
}

/****************************************************************************
  Function:
    UINT32 ValidateChecksum(ROM BOOT_REGION* entry)

  Description:
    This routine will calculate the check sum for the boot region and check
    it against the validation code at the beginning of the boot region

  Precondition:
    The validation code is saved at the beginning of the region.
    *entry is word aligned

  Parameters:
    BOOT_REGION *entry  - The boot region that we are calculating check sum for

  Returns:
    None
***************************************************************************/
UINT32 ValidateChecksum(ROM BOOT_REGION* entry)
{
    DWORD check_sum;
    DWORD validation_code;
    DWORD address;
    
    // Initialize the checksum to 0xFFFFFFFF since first four bytes used for validation code
    check_sum = 0x00000000;//FFFFFFFF;
    
    // Read and save the validation code
    address = entry->addressStart;
    validation_code = *((DWORD *)PA_TO_KVA1(address));
    
    // Iterate through the boot region
    for(address = entry->addressStart + 4; address < entry->addressEnd; address += 4)
    {   
        // Read the instruction and add it towards the check sum
	    check_sum += *((DWORD *)PA_TO_KVA1(address));
    }
     
    // Return whether or not the checksum equals the validation code
    return (check_sum == validation_code);
}

