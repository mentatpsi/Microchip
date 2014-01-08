/******************************************************************************

FileName:        boot_hal_pic24.c
Dependencies:    None
Processor:       PIC24
Compiler:        C30
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

// Include files
#include "Compiler.h"
#include "GenericTypedefs.h"
#include "HardwareProfile.h"

#include "custom_bin_boot.h"
#include "boot_config.h"
#include "boot_hal_pic24.h"

// Regions in program memory that we will be bootloading to
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
    DWORD_VAL address;
    BYTE i;

    // Iterate through each boot region
    for(i = 0;i < NUM_BOOT_REGIONS; i++)
    {
        // Iterate through each flash block of this boot region
        for(address.Val = my_boot_regions[i].addressStart; address.Val < my_boot_regions[i].addressEnd;address.Val+=(FLASH_BLOCK_SIZE))
        {
            NVMCON = 0x4042;				            // Initialize NVMCON
            TBLPAG = address.word.HW;                   // Initialize PM Page Boundary SFR
        	__builtin_tblwtl(address.word.LW, 0xFFFF);  // Set base address of erase block with dummy latch write
            asm("DISI #16");					        // Disable interrupts for next few instructions for unlock sequence
            __builtin_write_NVM();                      // C30 function to perform unlock sequence and set WR
            while(NVMCONbits.WR == 1){}                 // Wait until erase has completed
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

  Parameters:
    BOOT_REGION *entry  - The address range that we will write to
    BYTE *buffer        - A pointer to the buffer that holds the instructions
                          we will be writing

  Returns:
    None
***************************************************************************/
void WriteBlock(BOOT_REGION *entry, BYTE *buffer)
{
    UINT16_VAL data;
    DWORD_VAL address;
    
    // Iterate through region
    for(address.Val = entry->addressStart; address.Val < entry->addressEnd; address.Val+=2)
    {
	    TBLPAG = address.word.HW;           // Initialize PM Page Boundary SFR
	    
	    NVMCON = 0x4003;                    // Initialize NVMCON
	    
	    // Copy over buffer by byte, since might not be word aligned
	    data.byte.LB = *buffer++;
	    data.byte.HB = *buffer++;

        //Write the low word to the latch
	    __builtin_tblwtl(address.word.LW, data.Val);	
	    
	    // Copy over buffer by byte, since might not be word aligned
	    data.byte.LB = *buffer++;
	    data.byte.HB = *buffer++;
	    
        //Write the high word to the latch (8 bits of 
        //  data + 8 bits of "phantom data")	
	    __builtin_tblwth(address.word.LW, data.Val);	

        //Disable interrupts for next few instructions for unlock sequence
    	asm("DISI #16");					
    	__builtin_write_NVM();
        while(NVMCONbits.WR == 1){}
    }
    //Good practice to clear WREN bit anytime we are not
    //  expecting to do erase/write operations, further 
    //  reducing probability of accidental activation.
	NVMCONbits.WREN = 0;	
}

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
void WriteRomData(BOOT_REGION *entry, BYTE *buffer)
{
    UINT16_VAL data;
    DWORD_VAL address;

    // Iterate through region
    for(address.Val = entry->addressStart; address.Val < entry->addressEnd; address.Val+=2)
    {
	    TBLPAG = address.word.HW;           // Initialize PM Page Boundary SFR
	    
	    NVMCON = 0x4003;                    // Initialize NVMCON

        // Copy over buffer by byte, since might not be word aligned
	    data.byte.LB = *buffer++;
	    data.byte.HB = *buffer++;

        //Write the low word to the latch
	    __builtin_tblwtl(address.word.LW, data.Val);	
	    
        //Write the high word (F's) to the latch (8 bits of 
        //  data + 8 bits of "phantom data")	
	    __builtin_tblwth(address.word.LW, 0xFFFF);

        //Disable interrupts for next few instructions for unlock sequence
    	asm("DISI #16");					
    	__builtin_write_NVM();
        while(NVMCONbits.WR == 1){}
    }
    //Good practice to clear WREN bit anytime we are not
    //  expecting to do erase/write operations, further 
    //  reducing probability of accidental activation.
	NVMCONbits.WREN = 0;
}

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
UINT32 ValidateChecksum(ROM BOOT_REGION* entry)
{
    DWORD_VAL validation_code;
    DWORD_VAL address;
    DWORD_VAL flash_word;
    DWORD check_sum = 0x00000000;
    
    // Initialize the checksum to 0x01FFFFFE = 0x00FFFFFF + 0x00FFFFFF
    // Since first two words are used for validation code (left as 0x00FFFFFF
    //check_sum = 0x01FFFFFE;
    
    address.Val = entry->addressStart;  // Initialize start address to beginning of boot region
    TBLPAG = address.word.HW;           // Initialize PM Page Boundary SFR
    
    // Read and save the validation code
    validation_code.word.LW = __builtin_tblrdl(address.word.LW);
    address.Val += 2;
    validation_code.word.HW = __builtin_tblrdl(address.word.LW);
    
    // Iterate through the boot region
    for(address.Val = entry->addressStart+4; address.Val < entry->addressEnd; address.Val+=2)
    {
        TBLPAG = address.word.HW;           // Initialize PM Page Boundary SFR
        
        // Read the instruction
        flash_word.word.LW = __builtin_tblrdl(address.word.LW);
        flash_word.word.HW = __builtin_tblrdh(address.word.LW);
        
        // And add it to the checksum
	    check_sum += flash_word.Val;
    }
     
    // Return whether or not the checksum equals the validation code
    return (check_sum == validation_code.Val);
}
