/******************************************************************************

FileName:        custom_bin_boot.c
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

// Include files
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "boot_config.h"
#include "custom_bin_boot.h"

// Include the proper hardware abstraction layer boot file
#if defined(__C30__) || defined __XC16__
#include "boot_hal_pic24.h"
#elif defined(__C32__)
#include "boot_hal_pic32.h"
#endif

// Binary File Reader state machine's states
typedef enum
{
    START_OF_FILE,
    USER_HEADER_SIZE,
    USER_HEADER,
    VALIDATION_CODE,
    DATA_SECTION_RANGE,
    DATA_SECTION_START,
    DATA_SECTION_INCOMPLETE,
    PARTIAL_INSTRUCTION
} CUSTOM_BIN_STATE;

// Variable to keept track of state machine, initialize to START_OF_FILE
static CUSTOM_BIN_STATE state = START_OF_FILE;

// The variables to keep track of how much is left for each section
static UINT32 amount_to_skip = 0;

// Size of user_header, pointer to user header, and validation codes (temporarily stored in RAM)
static UINT32 user_header_size;
static BYTE* user_header;
static BYTE val_codes_ram[NUM_BOOT_REGIONS*VALIDATION_CODE_SIZE];
static UINT32 partial_instruction;

// Ranges in program memory to keep track of
static BOOT_REGION binary_file_region;
static BOOT_REGION write_region;

// Regions in program memory that we will be bootloading to
extern ROM BOOT_REGION my_boot_regions[NUM_BOOT_REGIONS];


/****************************************************************************
  Function:
    BOOT_STATUS CustomBinWrite(BYTE *binary_file, DWORD size)

  Description:
    This routine is the main state machine for the binary file reader.

  Precondition:
    The device is attached and has sent data from the binary file to host.

  Parameters:
    BYTE *binary_file            - The pointer to the data from the binary file
    DWORD size              - The number of bytes available at that pointer

  Returns:
    VALIDATION_SUCCESSFUL   - Verifying the writes to program memory was successful
    VALIDATION_FAIL         - Verifying the writes to program memory was unsuccessful
    DONE_WRITING_BUFFER     - Reached the end of the buffer but not end of binary file,
                              need another USB read
    UNKNOWN_ERROR           - Unknown error occurred while reading binary file
***************************************************************************/
BOOT_STATUS CustomBinWrite(BYTE *binary_file, UINT32 size)
{
    UINT32 bytes_read;
    
    // Continue until we've exhausted the binary file buffer
    while(size)
    {
        // Binary File reader state machine
        switch(state)
        {
            case START_OF_FILE:
                // Erase the boot regions
                EraseBootRegions();
                
                // Fall through to next state
                state = USER_HEADER_SIZE;

            case USER_HEADER_SIZE:
                // Read the size of the header
                if(ReadObject(binary_file, size, &user_header_size, sizeof(user_header_size), &bytes_read))
                {
                    // If read is complete, check to see if there is a user header
                    if(user_header_size == 0)
                    {
                        // If zero, then proceed to reading validation code(s)
                        state = VALIDATION_CODE;
                    }
                    else
                    {
                        // Otherwise allocate memory for user header and proceed to reading user header
                        user_header = malloc(user_header_size);
                        state = USER_HEADER;
                    }
                }
                
                // Update pointer to binary file and number of bytes left in buffer
                size -= bytes_read;
                binary_file += bytes_read;
                break;
                
            case USER_HEADER:
                // Read the user header
                if(ReadObject(binary_file, size, user_header, user_header_size, &bytes_read))
                {
                    // If read is complete, proceed to reading validation code(s)
                    state = VALIDATION_CODE;
                }
                
                // Update pointer to binary file and number of bytes left in buffer
                size -= bytes_read;
                binary_file += bytes_read;
                break;
                
            case VALIDATION_CODE:
                // Read the validtion code(s)
                if(ReadObject(binary_file, size, &val_codes_ram, sizeof(val_codes_ram), &bytes_read))
                {
                    BYTE i;
                    
                    // When done reading, iterate through each boot region
                    for(i = 0; i < NUM_BOOT_REGIONS; i++)
                    {
                        // We'll write the validation code(s) at the beginning of each boot region
                        write_region.addressStart = my_boot_regions[i].addressStart;
                        write_region.addressEnd = my_boot_regions[i].addressStart + VALIDATION_CODE_SIZE;
                        WriteRomData(&write_region, &val_codes_ram[i*VALIDATION_CODE_SIZE]);
                    }
                    
                    // Go to next state where we'll read the table entry for the first data region
                    state = DATA_SECTION_RANGE;
                }
                
                // Update pointer to binary file and number of bytes left in buffer
                size -= bytes_read;
                binary_file += bytes_read;
                break;

            case DATA_SECTION_RANGE:
                // If the region we were looking at wasn't within one of the boot regions, then we need to finish skipping over it
                if(amount_to_skip > 0)
                {    
                    // If there aren't enough bytes in the buffer to fully skip over this region
                    if(amount_to_skip > size)
                    {
                        // Reduce the number of bytes to skip by how many bytes are in the buffer and set number of bytes in buffer to 0
                        amount_to_skip -= size;
                        size = 0;
                    }
                    else
                    {
                        // Otherwise increment buffer and reduce the number of bytes left in buffer
                        binary_file += amount_to_skip;
                        size -= amount_to_skip;
                        
                        // Reset number of bytes to skip to 0
                        amount_to_skip = 0;
                    }
                }
                
                // Read the address range for the next data region
                if(ReadObject(binary_file, size, &binary_file_region, sizeof(binary_file_region), &bytes_read))
                {
                    BYTE i;
                    
                    // When done reading, Check to see if we're at the end of the file (start address and end address are both 0s)
                    if(binary_file_region.addressStart == 0x00000000 && binary_file_region.addressEnd == 0x00000000)
                    {
                        // If so, then we're done bootloading the device, time to verify the boot regions
                        if(ValidationMethod())
                        {
                            // Return success
                            return VALIDATION_SUCCESSFUL;
                        }
                        else
                        {
                            // Otherwise return fail
                            return VALIDATION_FAIL;
                        }        
                    }
                        
                    // Otherwise, adjust to actual physical address range
                    binary_file_region.addressStart /= BYTES_PER_ADDRESS;
                    binary_file_region.addressEnd /= BYTES_PER_ADDRESS;
                        
                    // And check to see if this range is within one of the boot regions
                    for(i = 0; i < NUM_BOOT_REGIONS; i++)
                    {
                        // If it is within one of the boot regions
                        if((binary_file_region.addressStart >= my_boot_regions[i].addressStart) &&
                           (binary_file_region.addressEnd <= my_boot_regions[i].addressEnd))
                        {
                            // Then we can progress to the next state and start reading the data for this region
                            state = DATA_SECTION_START;
                            break;
                        } 
                    }
                        
                    // If it didn't fit in any of the boot regions
                    if(state != DATA_SECTION_START)
                    {
                        // Then we should skip over this region and read the next one
                        amount_to_skip = (binary_file_region.addressEnd - binary_file_region.addressStart)* BYTES_PER_ADDRESS;    
                    }    
                }      
                
                // Update pointer to binary file and number of bytes left in buffer
                size -= bytes_read;
                binary_file += bytes_read;
                break;
                
            case DATA_SECTION_START:
                // We will write to program memory for this whole binary file data region
                write_region.addressStart = binary_file_region.addressStart;
                write_region.addressEnd = binary_file_region.addressEnd;
                
                // And fall through to the next state
                state = DATA_SECTION_INCOMPLETE;

            case DATA_SECTION_INCOMPLETE:
                {
                    // Calculate how many bytes it is for that address range
                    DWORD write_size = (write_region.addressEnd - write_region.addressStart) * BYTES_PER_ADDRESS;
                    
                    // If there are less bytes in the buffer than the amount to write
                    if(size < write_size)
                    {
                        // Check to see if there's enough to write a whole instruction
                        if(size < BYTES_PER_INSTRUCTION)
                        {
                            // If not, then jump to the state that will properly save the partial instruction
                            state = PARTIAL_INSTRUCTION;
                            break;
                        }
                        else
                        {    
                            // If there is, then write all complete instructions within buffer, handle partial instructions after
                            write_region.addressEnd = write_region.addressStart + (((size / BYTES_PER_INSTRUCTION) * BYTES_PER_INSTRUCTION) / BYTES_PER_ADDRESS);
                            write_size = (write_region.addressEnd - write_region.addressStart) * BYTES_PER_ADDRESS;
                        }    
                    }
                    
                    // Write to that address range in program memory with the data from the buffer
                    WriteBlock(&write_region, binary_file);
                    
                    // Increment the buffer and decrement the number of bytes to be written
                    binary_file += write_size;
                    size -= write_size;
                    
                    // If we're at the end of a data region in the binary file
                    if(write_region.addressEnd == binary_file_region.addressEnd)
                    {
                        // Then try to read the next region
                        amount_to_skip = 0;
                        state = DATA_SECTION_RANGE;
                    }
                    else
                    {
                        // Otherwise update the write region to include the rest of the binary file data region
                        write_region.addressStart = write_region.addressEnd;
                        write_region.addressEnd = binary_file_region.addressEnd;
                    }    
                }
                break;
                
            case PARTIAL_INSTRUCTION:
                // Read the partial instruction
                if(ReadObject(binary_file, size, &partial_instruction, sizeof(partial_instruction), &bytes_read))
                {
                    BOOT_REGION single_instruction;
                    
                    // If complete instruction, then prepare to write, create one instruction range
                    single_instruction.addressStart = write_region.addressStart;
                    single_instruction.addressEnd = write_region.addressStart + (BYTES_PER_INSTRUCTION/BYTES_PER_ADDRESS);
                    
                    // And write to program memory
                    WriteBlock(&single_instruction, (BYTE*)&partial_instruction);
                    
                    // Update the write region start address by one instruction
                    write_region.addressStart = single_instruction.addressEnd;
                    
                    // And progress to read the rest of the data section
                    state = DATA_SECTION_INCOMPLETE;
                }
                
                // Update pointer to binary file and number of bytes left in buffer
                size -= bytes_read;
                binary_file += bytes_read;
                break;    
        }
    }
    
    // If we exit from the above loop, that means we've exhausted the buffer and need to read more
    return DONE_WRITING_BUFFER;
}

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
BOOL ReadObject(BYTE* binary_file, UINT32 bytes_left_in_buffer, void* object, UINT32 size_of_object, UINT32 *bytes_to_read)
{
    static UINT32 read_so_far = 0;
    BYTE* byte_parser = (BYTE*)object;  // Type cast object to byte pointer
    UINT32 i;
    
    *bytes_to_read = size_of_object - read_so_far;
    
    // If there are not enough bytes in buffer to fill entire object
    if(bytes_left_in_buffer < (size_of_object - read_so_far))
    {
        // Then we'll only copy over what's left inside buffer
        *bytes_to_read = bytes_left_in_buffer;
    }
    
    // Iterate through where we left off in object, and write until end of buffer or fully written object
    for(i = 0; i < *bytes_to_read; i++)
    {
        // Copy over, byte by byte while incrementing the buffer pointer
        byte_parser[read_so_far++] = *binary_file++;
    }
    
    // Return true if the entire object was written to, false otherwise
    if(read_so_far == size_of_object)
    {
        read_so_far = 0;
        return TRUE;
    }    
    
    return FALSE;
}    

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
    BOOL - True if validation passes for all boot regions, false otherwise
***************************************************************************/
BOOL ValidationMethod(void)
{
    BYTE i;
    BOOL validation_passed;
    
    // Iterate through each boot region
    for(i = 0; i < NUM_BOOT_REGIONS; i++)
    {
        // Verify boot region
        #ifdef CHECK_SUM
        validation_passed = ValidateChecksum(&my_boot_regions[i]);
        #else
        #error Choose a validation method
        #endif
        
        // Check to see if the validation has passed
        if(!validation_passed)
        {
            // Return false if it does not
            return FALSE;
        }
    }

    // If they all pass validation, then return true
    return TRUE;
}
